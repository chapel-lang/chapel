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
  YYSYMBOL_interface_expr = 250,           /* interface_expr  */
  YYSYMBOL_interface_expr_ls = 251,        /* interface_expr_ls  */
  YYSYMBOL_unary_implements_stmt = 252,    /* unary_implements_stmt  */
  YYSYMBOL_binary_implements_stmt = 253,   /* binary_implements_stmt  */
  YYSYMBOL_ifc_constraint = 254,           /* ifc_constraint  */
  YYSYMBOL_try_stmt = 255,                 /* try_stmt  */
  YYSYMBOL_catch_expr_ls = 256,            /* catch_expr_ls  */
  YYSYMBOL_catch_expr = 257,               /* catch_expr  */
  YYSYMBOL_catch_expr_inner = 258,         /* catch_expr_inner  */
  YYSYMBOL_throw_stmt = 259,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 260,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 261,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 262,                /* when_stmt  */
  YYSYMBOL_manager_expr = 263,             /* manager_expr  */
  YYSYMBOL_manager_expr_ls = 264,          /* manager_expr_ls  */
  YYSYMBOL_manage_stmt = 265,              /* manage_stmt  */
  YYSYMBOL_class_decl_stmt = 266,          /* class_decl_stmt  */
  YYSYMBOL_class_start = 267,              /* class_start  */
  YYSYMBOL_class_tag = 268,                /* class_tag  */
  YYSYMBOL_opt_inherit = 269,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 270,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 271,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header_lcbr = 272,         /* enum_header_lcbr  */
  YYSYMBOL_enum_ls = 273,                  /* enum_ls  */
  YYSYMBOL_274_5 = 274,                    /* $@5  */
  YYSYMBOL_275_6 = 275,                    /* $@6  */
  YYSYMBOL_enum_item = 276,                /* enum_item  */
  YYSYMBOL_lambda_decl_start = 277,        /* lambda_decl_start  */
  YYSYMBOL_lambda_decl_expr = 278,         /* lambda_decl_expr  */
  YYSYMBOL_279_7 = 279,                    /* $@7  */
  YYSYMBOL_linkage_spec_empty = 280,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 281,             /* linkage_spec  */
  YYSYMBOL_opt_fn_type_formal_ls = 282,    /* opt_fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal_ls = 283,        /* fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal = 284,           /* fn_type_formal  */
  YYSYMBOL_opt_fn_type_ret_type = 285,     /* opt_fn_type_ret_type  */
  YYSYMBOL_fn_type = 286,                  /* fn_type  */
  YYSYMBOL_fn_expr = 287,                  /* fn_expr  */
  YYSYMBOL_288_8 = 288,                    /* $@8  */
  YYSYMBOL_289_9 = 289,                    /* $@9  */
  YYSYMBOL_fn_decl_stmt_complete = 290,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 291,             /* fn_decl_stmt  */
  YYSYMBOL_292_10 = 292,                   /* $@10  */
  YYSYMBOL_fn_decl_stmt_inner = 293,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 294,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 295,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 296,                 /* fn_ident  */
  YYSYMBOL_op_ident = 297,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 298,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 299,              /* all_op_name  */
  YYSYMBOL_formal_var_arg_expr = 300,      /* formal_var_arg_expr  */
  YYSYMBOL_opt_formal_ls = 301,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 302,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 303,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 304,                /* formal_ls  */
  YYSYMBOL_formal = 305,                   /* formal  */
  YYSYMBOL_formal_ident_def = 306,         /* formal_ident_def  */
  YYSYMBOL_named_formal = 307,             /* named_formal  */
  YYSYMBOL_opt_formal_intent_tag = 308,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 309,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 310,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 311,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 312,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 313,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 314,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 315,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 316,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 317,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 318, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 319,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 320,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 321,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 322, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 323, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 324, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 325,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 326,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 327,            /* var_decl_stmt  */
  YYSYMBOL_328_11 = 328,                   /* $@11  */
  YYSYMBOL_var_decl_stmt_inner_ls = 329,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 330,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 331, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 332, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 333,            /* opt_init_expr  */
  YYSYMBOL_formal_or_ret_type_expr = 334,  /* formal_or_ret_type_expr  */
  YYSYMBOL_ret_type = 335,                 /* ret_type  */
  YYSYMBOL_colon_ret_type = 336,           /* colon_ret_type  */
  YYSYMBOL_opt_ret_type = 337,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 338,                 /* opt_type  */
  YYSYMBOL_formal_type = 339,              /* formal_type  */
  YYSYMBOL_colon_formal_type = 340,        /* colon_formal_type  */
  YYSYMBOL_opt_colon_formal_type = 341,    /* opt_colon_formal_type  */
  YYSYMBOL_expr_ls = 342,                  /* expr_ls  */
  YYSYMBOL_tuple_component = 343,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 344,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 345,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 346,                /* actual_ls  */
  YYSYMBOL_actual_expr = 347,              /* actual_expr  */
  YYSYMBOL_ident_expr = 348,               /* ident_expr  */
  YYSYMBOL_sub_type_level_expr = 349,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 350,                 /* for_expr  */
  YYSYMBOL_for_expr_base = 351,            /* for_expr_base  */
  YYSYMBOL_bracket_loop_expr = 352,        /* bracket_loop_expr  */
  YYSYMBOL_bracket_loop_expr_base = 353,   /* bracket_loop_expr_base  */
  YYSYMBOL_cond_expr = 354,                /* cond_expr  */
  YYSYMBOL_nil_expr = 355,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 356,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 357,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 358,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 359,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 360,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 361,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 362,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 363,          /* task_var_prefix  */
  YYSYMBOL_new_maybe_decorated = 364,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 365,                 /* new_expr  */
  YYSYMBOL_let_expr = 366,                 /* let_expr  */
  YYSYMBOL_range_literal_expr = 367,       /* range_literal_expr  */
  YYSYMBOL_cast_expr = 368,                /* cast_expr  */
  YYSYMBOL_tuple_expand_expr = 369,        /* tuple_expand_expr  */
  YYSYMBOL_super_expr = 370,               /* super_expr  */
  YYSYMBOL_expr = 371,                     /* expr  */
  YYSYMBOL_opt_expr = 372,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 373,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 374,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 375,           /* call_base_expr  */
  YYSYMBOL_call_expr = 376,                /* call_expr  */
  YYSYMBOL_dot_expr = 377,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 378,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 379,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 380,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 381,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 382,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 383,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 384,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 385,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 386,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 387       /* reduce_scan_op_expr  */
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

#line 511 "bison-chpl-lib.cpp"

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
#define YYLAST   20962

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  207
/* YYNRULES -- Number of rules.  */
#define YYNRULES  749
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1321

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
       0,   638,   638,   642,   643,   648,   649,   657,   661,   671,
     676,   683,   684,   685,   686,   687,   688,   689,   690,   692,
     693,   694,   707,   708,   709,   718,   727,   731,   738,   746,
     747,   748,   749,   750,   751,   752,   753,   767,   782,   790,
     802,   813,   825,   837,   848,   864,   865,   869,   879,   880,
     884,   888,   889,   893,   897,   904,   916,   924,   927,   937,
     939,   946,   951,   960,   964,   984,   999,  1014,  1030,  1031,
    1033,  1038,  1039,  1044,  1062,  1066,  1070,  1081,  1100,  1101,
    1105,  1109,  1113,  1118,  1122,  1126,  1136,  1141,  1147,  1154,
    1159,  1166,  1177,  1178,  1182,  1187,  1195,  1204,  1214,  1222,
    1231,  1244,  1252,  1253,  1254,  1255,  1256,  1257,  1258,  1259,
    1260,  1261,  1262,  1263,  1264,  1265,  1266,  1267,  1268,  1280,
    1284,  1289,  1294,  1300,  1309,  1310,  1314,  1323,  1327,  1331,
    1335,  1339,  1343,  1352,  1353,  1357,  1358,  1359,  1360,  1361,
    1365,  1366,  1378,  1379,  1384,  1385,  1386,  1387,  1388,  1389,
    1390,  1391,  1392,  1393,  1394,  1395,  1396,  1397,  1398,  1399,
    1400,  1401,  1402,  1406,  1407,  1408,  1409,  1410,  1411,  1412,
    1413,  1414,  1415,  1416,  1417,  1424,  1425,  1426,  1427,  1431,
    1432,  1436,  1443,  1453,  1457,  1463,  1463,  1469,  1469,  1478,
    1479,  1480,  1481,  1482,  1483,  1484,  1485,  1489,  1494,  1499,
    1504,  1511,  1519,  1520,  1524,  1533,  1539,  1547,  1566,  1565,
    1575,  1574,  1587,  1593,  1596,  1597,  1601,  1620,  1634,  1648,
    1652,  1656,  1660,  1664,  1668,  1672,  1676,  1696,  1700,  1704,
    1708,  1712,  1716,  1720,  1724,  1728,  1732,  1736,  1740,  1744,
    1748,  1752,  1756,  1760,  1764,  1768,  1772,  1776,  1780,  1784,
    1789,  1797,  1806,  1810,  1814,  1818,  1822,  1826,  1830,  1834,
    1838,  1843,  1848,  1853,  1861,  1876,  1894,  1898,  1905,  1906,
    1911,  1916,  1917,  1918,  1919,  1920,  1921,  1922,  1923,  1924,
    1925,  1926,  1927,  1928,  1937,  1938,  1939,  1940,  1949,  1950,
    1954,  1968,  1972,  1979,  1986,  1993,  1997,  2001,  2008,  2012,
    2016,  2020,  2027,  2028,  2032,  2036,  2040,  2047,  2060,  2076,
    2084,  2088,  2097,  2098,  2102,  2106,  2111,  2120,  2125,  2129,
    2136,  2137,  2141,  2150,  2155,  2166,  2173,  2174,  2175,  2179,
    2180,  2184,  2188,  2192,  2196,  2200,  2207,  2224,  2237,  2244,
    2249,  2256,  2255,  2265,  2271,  2270,  2285,  2292,  2304,  2322,
    2319,  2347,  2351,  2352,  2354,  2359,  2360,  2364,  2365,  2369,
    2372,  2374,  2379,  2380,  2391,  2411,  2410,  2426,  2425,  2443,
    2453,  2450,  2484,  2492,  2500,  2511,  2522,  2531,  2546,  2547,
    2551,  2552,  2553,  2562,  2563,  2564,  2565,  2566,  2567,  2568,
    2569,  2570,  2571,  2572,  2573,  2574,  2575,  2576,  2577,  2578,
    2579,  2580,  2581,  2582,  2583,  2584,  2585,  2586,  2590,  2591,
    2592,  2593,  2594,  2595,  2596,  2597,  2598,  2599,  2600,  2601,
    2606,  2607,  2611,  2612,  2616,  2617,  2621,  2622,  2626,  2627,
    2631,  2632,  2636,  2640,  2641,  2645,  2649,  2654,  2659,  2664,
    2669,  2677,  2681,  2689,  2690,  2691,  2692,  2693,  2694,  2695,
    2696,  2697,  2701,  2702,  2703,  2704,  2705,  2706,  2710,  2711,
    2712,  2716,  2717,  2718,  2719,  2720,  2721,  2722,  2726,  2727,
    2730,  2731,  2735,  2736,  2737,  2741,  2746,  2747,  2749,  2751,
    2753,  2758,  2760,  2765,  2767,  2769,  2771,  2773,  2775,  2777,
    2782,  2783,  2787,  2796,  2800,  2808,  2812,  2819,  2840,  2841,
    2846,  2847,  2848,  2849,  2850,  2855,  2854,  2863,  2871,  2875,
    2882,  2900,  2919,  2923,  2927,  2934,  2936,  2938,  2945,  2946,
    2947,  2951,  2955,  2956,  2957,  2961,  2962,  2966,  2967,  2971,
    2972,  2973,  2974,  2978,  2979,  2983,  2987,  2988,  2994,  2995,
    2999,  3000,  3004,  3008,  3015,  3016,  3020,  3025,  3034,  3035,
    3039,  3040,  3047,  3048,  3049,  3050,  3051,  3052,  3054,  3056,
    3058,  3060,  3066,  3068,  3071,  3073,  3075,  3077,  3079,  3081,
    3083,  3085,  3087,  3089,  3094,  3098,  3108,  3118,  3127,  3142,
    3157,  3172,  3182,  3192,  3201,  3216,  3231,  3249,  3253,  3257,
    3261,  3265,  3269,  3273,  3277,  3284,  3302,  3310,  3311,  3312,
    3313,  3314,  3315,  3316,  3320,  3321,  3325,  3334,  3335,  3339,
    3348,  3349,  3353,  3367,  3371,  3378,  3379,  3380,  3381,  3382,
    3383,  3387,  3389,  3391,  3393,  3395,  3401,  3408,  3420,  3432,
    3445,  3462,  3469,  3474,  3479,  3484,  3490,  3496,  3526,  3533,
    3540,  3541,  3545,  3547,  3548,  3550,  3552,  3553,  3554,  3555,
    3558,  3559,  3560,  3561,  3562,  3563,  3564,  3565,  3566,  3570,
    3571,  3575,  3576,  3577,  3581,  3582,  3583,  3584,  3593,  3594,
    3597,  3598,  3599,  3603,  3615,  3627,  3634,  3636,  3638,  3640,
    3642,  3648,  3661,  3662,  3666,  3670,  3677,  3678,  3682,  3683,
    3687,  3688,  3689,  3690,  3691,  3692,  3693,  3694,  3699,  3704,
    3709,  3714,  3719,  3728,  3733,  3742,  3743,  3744,  3745,  3746,
    3747,  3748,  3749,  3750,  3751,  3752,  3753,  3754,  3755,  3756,
    3757,  3758,  3759,  3760,  3761,  3762,  3763,  3764,  3768,  3769,
    3770,  3771,  3772,  3773,  3776,  3780,  3784,  3788,  3792,  3799,
    3803,  3807,  3811,  3819,  3820,  3821,  3822,  3823,  3824,  3825
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
  "implements_type_error_ident", "interface_expr", "interface_expr_ls",
  "unary_implements_stmt", "binary_implements_stmt", "ifc_constraint",
  "try_stmt", "catch_expr_ls", "catch_expr", "catch_expr_inner",
  "throw_stmt", "select_stmt", "when_stmt_ls", "when_stmt", "manager_expr",
  "manager_expr_ls", "manage_stmt", "class_decl_stmt", "class_start",
  "class_tag", "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt",
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

#define YYPACT_NINF (-1072)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-750)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1072,    96,  4975, -1072,   -36,    59, -1072, -1072, -1072, -1072,
   -1072, -1072, -1072, 13900,    13,   146,   198,  4493,   215, 20716,
      13,   267,    54,   218,   146,  6375, 13900,   164, 20771, -1072,
     330,   291, -1072, 11450,   346,   460,   262, -1072,   414, 20771,
   20771, 20771, -1072,  3104, 12500,   415, 13900, 13900,   216, -1072,
     435,   446, 13900, -1072,  4493, -1072, 13900,   431,   319,   244,
     333,  4686,   451, 20846, -1072, 13900,  9875, 13900, 12500,  4493,
   13900,   423,   493,   399,  6375,   502, 13900,   507,  8475,  8475,
   -1072,   532, -1072,  4493, -1072,   544, 11625, -1072,   600, 13900,
   -1072, 13900, -1072, -1072, 15928, 13900, -1072, 11800, -1072, -1072,
   -1072,  5325,  9000, 13900, -1072,  6025, -1072, -1072, -1072, -1072,
   20318,   468, -1072,    44,   439, -1072,    56, -1072, -1072, -1072,
   -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072,
    8650, -1072,  9175, -1072, -1072,    77, -1072, -1072, -1072,   562,
   -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072,   484,
   20771, -1072, 20456,   459,   387, -1072,   365, -1072, -1072, -1072,
     353,   277, -1072, -1072, 20771,  3104, -1072,   476,   494, -1072,
   -1072, -1072,   512,   514, 13900,   515,   516, -1072, -1072, -1072,
   20130,   517,   189,   523,   528, -1072, -1072,   448, -1072, -1072,
   -1072, -1072, -1072,   176, -1072, -1072, -1072, 13900, 20771, -1072,
   13900, 13900,    17,   606,   449, -1072, -1072, -1072, -1072, 20130,
     454, -1072, -1072,   495,  6375, -1072, -1072,   531,   119,   529,
   -1072,   196, -1072,   536, -1072,   120, 20130, 10050, -1072, -1072,
   -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072,
   -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072,
   -1072, -1072, -1072,   533, -1072, -1072, 20771, 20771,     3,  4750,
   -1072, -1072,   620, 10050,   537, -1072,   219,   457,  6375,  1871,
      89,    41, -1072,  6375, -1072, -1072, 17577, -1072,    30, 18241,
     754, -1072,   538,   539, -1072, 17577,   119,   754, -1072, 10050,
     851,   851,    51,    27, -1072,    68, -1072, -1072, -1072, -1072,
   10225, 10225, -1072,   386, -1072,   535,   558, -1072, -1072,  2162,
     567, -1072, -1072, 20130,   230, 13900, 13900, -1072,   129, -1072,
   -1072, 20130,   554, 18286, -1072, 17577,   119, 20130,   542, 10050,
   -1072, 20130, 18393, -1072, -1072, -1072, -1072, -1072,   119,   555,
   -1072,   251,   251,   580,   754,   754,   203, -1072, -1072,  5500,
     -23, -1072, 13900,   166,   202, -1072,   580,   719, -1072, -1072,
   20771, -1072, 20771, -1072,  5325, -1072,   113, -1072,   620, -1072,
   -1072, -1072,   727,   559,  5675, 13900, -1072, 13900,   672, -1072,
   -1072,  3485,    78,   484, 20130,   300, 13900,  6375, 11975, 12675,
   12675, 11450, 12150, -1072, -1072, -1072, 20771, 13900,   568, -1072,
     570, 20511,   617,   154, -1072,   279,   353, -1072, -1072, -1072,
   -1072,   655, -1072, -1072, -1072, -1072,    79,   659, -1072, -1072,
   -1072, 15758,   625,   367, -1072,   368,   485, -1072,   603, 13900,
   13900, 13900, 12675, 12675, 13900,   496, 13900, 13900, 13900, 13900,
   13900,   261, 15928, 13900, 13900, 13900, 13900, 13900, 13900, 13900,
   13900, 13900, 13900, 13900, 13900, 13900, 13900, 13900, 13900,   685,
   -1072, -1072, -1072, -1072, -1072, 11625, 11625, -1072, -1072, -1072,
   -1072, 11625, -1072, -1072, 11625, 11625, 10050, 10400, 12675, 12675,
   18438,   537, 13900, 13900, 12850, 20771,  9350, -1072, -1072, -1072,
    3104, -1072, 13900, -1072,   629,   583,   610, -1072, -1072, -1072,
     634,   635,  6375,   728,  6375, -1072,   729, 13900,   696,   592,
   11625, 20771, -1072, 20771, -1072, -1072, -1072,  1871, -1072, -1072,
     249, -1072, 14075,   641, 13900,  3104, -1072, -1072, 13900, -1072,
   20586, 13900, 13900, -1072,   596, -1072, -1072, -1072, -1072, 20641,
   -1072,   353, 15622, -1072,  1871, -1072,   626, 14075, -1072, 20130,
   20130, -1072,   529, -1072,    58, -1072, 11625,   598, -1072,   752,
     752, -1072, -1072, -1072, -1072, 13025, -1072, 18478,  9525, -1072,
    9700, -1072, -1072, -1072, 11975, 12675, 12675, -1072,   627, 10575,
   -1072,   228, -1072,  5850, -1072,   369, 18598,   384, 17197, 20771,
    8300,  8125, -1072,   484,   604, -1072,   196, -1072, 17653,   660,
   20771,   608,    39, 17273,    24,  2670,    24,  4241,    39, 17577,
   16100,   -13,    63, 18644,   -19,   270,   613,   637,  1405, -1072,
   -1072, 13900,   132, -1072, -1072,   283, -1072, -1072,    46, -1072,
   -1072,  7950, -1072,   676, -1072,   614,   633, -1072,   638, -1072,
     639,   640,   642,   643, -1072,   645,   646,   647,   649,   651,
     652,   654, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072,
   -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072,
   -1072, -1072, -1072, -1072, 13900,   663,   656,   614, -1072,   614,
   -1072, 13900, -1072, 20771, -1072, -1072,  2819,  2819,   549, -1072,
     549, -1072,   549, 20295,  1119,  1331,  1432,   119,   251, -1072,
     500, -1072, -1072, -1072, -1072, -1072,   580, 19242,   549,   447,
     447,  2819,   447,   447,  1243,   251, 19242,  3539,  1243,   754,
     754,   251,   580,   648,   650,   653,   657,   658,   668,   644,
     615, -1072,   549, -1072,   549, 13900, 17729, 17805,    40,   636,
      84, -1072, -1072, -1072,   294, -1072,  1641, 20175,   396,   388,
   20130, 11625, -1072, 11625, 13900, 13900,   773,  6375,   776, 18684,
    6375, 17881, 20771, -1072,   298, -1072, -1072,   299, -1072,   304,
    1871,    89, -1072, 20130, 10750, -1072, 20130, -1072, -1072, -1072,
   20771, 18760, 18836, -1072,   626,   666, -1072,   310, 14075, -1072,
     151, -1072, 13900, -1072,   665,     5,   669, -1072,  2474, -1072,
   -1072, -1072, -1072, -1072, -1072, -1072, 17349, 17105,    66, -1072,
   -1072,   326, -1072, -1072, -1072, -1072, 13900, -1072,   127, 16205,
   13900, -1072, 13200,  8300,  8125, -1072, 10925,   429, 11100,   432,
     475, 11275,  8825,   505,   390, -1072, -1072, -1072, 18876,   667,
     679,   671, -1072, 20771,  3104, 12675,    39, 13900,   772, 13900,
   -1072,  6550, 12675, -1072,   673, -1072,    39,  6725, 12675, -1072,
      39, -1072,    39, 12675, -1072,    39, -1072, -1072, 20130,  6375,
     675, 12675, 13375,  5150,   677, 13900, 13550, -1072, 11625, -1072,
     680,  1971, 20130, 20771,   468,   585, -1072,   676, -1072, -1072,
   -1072, -1072, -1072,     9,   385, -1072, 18922, -1072, 16364, -1072,
   -1072, 20130, -1072,   681,   678, -1072, -1072, -1072, -1072, -1072,
   -1072, -1072, -1072, 14250, 12675, 14425, 12675, 12675, 16100, -1072,
   -1072,  9350, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072,
   -1072, -1072, -1072, -1072,    60, 12675, 12675, -1072, -1072, -1072,
   20130, 20130,  6375, -1072,  6375, 13900, -1072,  6375,   787,   683,
   -1072, 20771,   529, -1072, -1072,   641, -1072, -1072, -1072,   674,
     684,   151, 13900,   676,   626, -1072, 12325, -1072, -1072, 20130,
   -1072,  6375, 13900, -1072, -1072, -1072, 20771, -1072,   682,   529,
   12675, 12675, 12675,  6375, 11625, -1072, -1072, -1072, -1072, -1072,
     500, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072,
   -1072, -1072, -1072, -1072, -1072,  8300, -1072, -1072, 19028,   211,
   17973,   391,   692,  8300, -1072, 13900, -1072, -1072,  2582,   404,
     420,    13, 19082, -1072, 19122, 13900,   334, 11450, 20130,    61,
   18049,  9350, -1072, 11450, 20130,    32, 17425, -1072, -1072,    24,
   17501, -1072, -1072,  6375,   122, 18125, -1072, -1072, 20130,  6375,
   20130, -1072, 19202,   335, -1072, -1072, -1072, -1072,  3938,  2254,
   -1072, -1072, 20511, -1072,     9,    65, 13900, -1072,   712,   687,
   -1072,   614,   614, -1072, -1072, 13900,   792, 19282, 13900,   793,
   19362,   686, 19322, -1072,   641,   337,   339, -1072, -1072, 20130,
     828,  6375, -1072, -1072, -1072, -1072, 10050, 10050, -1072, -1072,
   -1072, -1072,   151, 20130, -1072,   118,    31,   690, 13900, -1072,
      39, 17577,    24, 17501,   130, 18201, -1072,   167, 13725,  8300,
    8125,  8300,  8125, -1072, -1072, -1072, 19482, 20130,  3215, -1072,
   -1072, -1072,    39,    39, -1072, 17577, -1072, 16445,  6900, -1072,
    7075, -1072,   340, -1072, 16525,  7250, -1072,    39,  7425, -1072,
      39, -1072,    39, -1072,    39, -1072,  7600,   691,  7775,   693,
   -1072, 13900, -1072,  2254, -1072, -1072, -1072, -1072, -1072,   158,
   -1072,   141, -1072,   253, 19442,    33, -1072, -1072, -1072, 19558,
   14600, 14775, 19634, 14950, 15125, 15300, 15475, -1072,  6375, -1072,
     697,   699, -1072, -1072,  6375, -1072,   529, 20130,  6375,  6375,
   -1072, -1072, 20130,   436,   700,   437,   709, 13900, -1072, -1072,
   -1072, -1072,  6375, 11450, 20130, 11450, 20130,  9350, -1072,  6375,
   11450, 20130, -1072, 11450, 20130, -1072, -1072, -1072, 11450, -1072,
   20130,  6375, 11450, -1072, 20130,  6375, 20130, -1072,    34, -1072,
   13900,    65,   158,   158,   158,   158,   158,   158,    65,  6200,
   -1072, -1072, -1072, -1072, -1072, 13900, 13900, 13900, 13900, 13900,
   13900, 13900, 13900, -1072,   711,   715,   660, -1072, -1072, -1072,
   -1072, -1072, 20130, 18684, 16605, 16685, -1072, 18684, 16765, 16845,
   16925, -1072, 17005, -1072, -1072, 20130, -1072, -1072, -1072, -1072,
   -1072, -1072, -1072,   734, -1072, 19710, 19786, 19862, 19938, 20014,
   20090, -1072, -1072,  6375,  6375,  6375,  6375,  6375,  6375, 13900,
   13900, 13900, 13900, 13900, 13900, 18684, 18684, 18684, 18684, 18684,
   18684
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   142,   475,   692,   693,   694,
     688,   689,   695,   213,   604,   133,   163,   570,   170,   572,
     604,   169,   505,   503,   133,     0,   213,   288,   164,   203,
     202,   686,   201,   213,   168,    68,   289,   353,   165,     0,
       0,     0,   348,     0,   213,   172,   213,   213,   621,   596,
     696,   173,   213,   354,   564,   500,   213,     0,     0,   187,
     185,   441,   167,   573,   502,   213,   213,   213,   213,   568,
     213,     0,   171,     0,     0,   143,   213,   687,   213,   213,
     493,   166,   328,   566,   504,   174,   213,   745,     0,   213,
     747,   213,   748,   749,   637,   213,   746,   213,   645,   183,
     744,     0,   213,   213,     4,     0,     5,    10,    11,    12,
      47,    50,    51,    57,     0,    45,    71,    13,    77,    14,
      15,    16,    17,    29,   550,   551,    23,    46,   184,   194,
     213,   204,   659,   195,    18,     0,    31,    30,    20,     0,
     283,   196,    19,   655,    22,    35,    32,    33,   193,   329,
       0,   191,     0,     0,   652,   352,     0,   649,   189,   369,
     461,   452,   647,   192,     0,     0,   190,   664,   643,   642,
     646,   554,   552,     0,   213,   653,   654,   658,   657,   656,
       0,   553,     0,   665,   666,   667,   690,   691,   648,   556,
     555,   650,   651,     0,    28,   572,   164,   213,     0,   573,
     213,   213,     0,     0,   652,   664,   552,   653,   654,   562,
     553,   665,   666,     0,     0,   605,   134,     0,   571,     0,
     494,     0,   501,     0,    21,     0,   538,   213,   140,   144,
     156,   150,   149,   158,   137,   148,   159,   145,   160,   135,
     161,   154,   147,   155,   153,   151,   152,   136,   138,   146,
     157,   162,   141,     0,   139,   212,     0,     0,     0,     0,
      70,    69,    71,   213,   290,   291,     0,     0,     0,     0,
       0,     0,   508,     0,   180,    40,     0,   320,     0,   319,
     730,   625,   622,   623,   624,     0,   565,   731,     7,   213,
     351,   351,   448,   177,   443,   178,   444,   445,   449,   450,
     176,   175,   451,   441,   534,     0,   356,   357,   359,     0,
     442,   533,   361,   521,     0,   213,   213,   181,   649,   640,
     663,   641,     0,     0,    43,     0,   569,   557,     0,   213,
      44,   563,     0,   298,    47,   302,   299,   302,   567,     0,
      56,   732,   734,   635,   729,   728,     0,    74,    78,     0,
       0,   540,   213,     0,     0,   541,   636,     0,     6,   326,
       0,   202,     0,   327,     0,    48,     0,     9,    71,    49,
      52,    55,     0,    54,     0,   213,    72,   213,     0,   505,
     200,     0,   688,   329,   660,   208,   213,     0,   213,   213,
     213,   213,   213,   215,   574,   587,     0,   213,     0,   325,
       0,     0,   346,     0,   339,   441,   461,   459,   460,   458,
     377,   463,   462,   466,   465,   467,     0,   456,   453,   454,
     457,     0,   498,     0,   495,     0,   644,    34,   626,   213,
     213,   213,   213,   213,   213,   733,   213,   213,   213,   213,
     213,     0,   634,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   408,
     415,   416,   417,   412,   414,   213,   213,   410,   413,   411,
     409,   213,   419,   418,   213,   213,   213,   213,   213,   213,
       0,     0,   213,   213,   213,     0,   213,    36,    24,    37,
       0,    25,   213,    38,   550,     0,   545,   546,   549,   338,
       0,     0,     0,   257,     0,   408,   253,   213,     0,     0,
     213,     0,   293,     0,   267,    26,   512,     0,   513,   515,
       0,   532,   213,   518,   213,     0,   179,    39,   213,   322,
       0,   213,   213,    41,     0,   188,   186,   446,   447,     0,
     442,   461,   441,   434,     0,   433,   536,   213,   126,   661,
     662,   367,     0,   182,     0,    42,   213,     0,   309,   300,
     301,    27,    76,    75,    79,   213,   697,     0,   213,   682,
     213,   684,     8,   290,   213,   213,   213,   214,    57,   213,
      53,     0,    65,     0,   124,     0,   119,     0,    86,     0,
     213,   213,   197,   329,     0,   210,     0,   207,   604,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     588,     0,     0,   538,     0,   296,     0,   330,     0,   337,
     343,   213,   341,   336,   426,     0,   428,   432,     0,   464,
     526,     0,   528,   468,   455,   424,   140,   404,   144,   402,
     150,   149,   148,   145,   406,   160,   161,   147,   151,   136,
     146,   162,   401,   383,   386,   384,   385,   407,   396,   387,
     400,   392,   390,   403,   391,   389,   394,   399,   388,   393,
     397,   398,   395,   405,   213,   380,     0,   424,   381,   424,
     378,   213,   497,     0,   492,   507,   725,   724,   727,   736,
     735,   740,   739,   721,   718,   719,   720,   638,   708,   142,
       0,   678,   679,   143,   677,   676,   632,   712,   723,   717,
     715,   726,   716,   714,   706,   711,   713,   722,   705,   709,
     710,   707,   633,     0,     0,     0,     0,     0,     0,     0,
       0,   738,   737,   742,   741,   213,     0,     0,     0,   296,
     615,   616,   618,   620,     0,   607,     0,     0,     0,     0,
     539,   213,   559,   213,   213,   213,   256,     0,   252,     0,
       0,     0,     0,   558,     0,   292,   270,     0,   268,     0,
     516,     0,   531,   530,   213,   510,   631,   509,   321,   318,
       0,     0,     0,   675,   536,   362,   358,     0,   213,   537,
     518,   360,   213,   366,     0,     0,     0,   560,     0,   303,
     698,   639,   683,   542,   685,   543,     0,     0,     0,    58,
      59,     0,    61,    63,    67,    66,   213,   101,     0,     0,
     213,    94,   213,   213,   213,    64,   213,   383,   213,   384,
     385,   213,   213,   395,     0,   420,   421,    81,    80,    93,
       0,     0,   331,     0,     0,   213,     0,   213,     0,   213,
     225,     0,   213,   224,     0,   233,     0,     0,   213,   231,
       0,   241,     0,   213,   239,     0,   218,   217,   589,     0,
       0,   213,   213,     0,     0,   213,   213,   701,   213,   294,
       0,   351,   347,     0,     0,   441,   427,   468,   524,   523,
     522,   525,   469,   476,   441,   376,     0,   382,     0,   372,
     373,   499,   496,     0,     0,   132,   130,   131,   129,   128,
     127,   673,   674,   213,   213,   213,   213,   213,   699,   617,
     619,   213,   606,   163,   170,   169,   168,   165,   172,   173,
     167,   171,   166,   174,     0,   213,   213,   506,   548,   547,
     265,   264,     0,   259,     0,   213,   255,     0,   261,     0,
     295,     0,     0,   514,   517,   518,   519,   520,   317,     0,
       0,   518,   213,   468,   536,   535,   213,   437,   435,   368,
     311,     0,   213,   310,   313,   561,     0,   304,   307,     0,
     213,   213,   213,     0,   213,    60,   125,   122,   102,   114,
     108,   107,   106,   116,   103,   117,   112,   105,   113,   111,
     109,   110,   104,   115,   118,   213,   121,   120,    89,    88,
      87,     0,     0,   213,   198,   213,   199,   331,   351,     0,
       0,   604,   604,   221,     0,   213,     0,   213,   577,     0,
       0,   213,   234,   213,   583,     0,     0,   232,   242,     0,
       0,   240,   249,     0,     0,     0,   700,   247,   590,     0,
     703,   702,     0,     0,   324,   187,   185,   323,   351,   351,
     332,   342,     0,   429,   476,     0,   213,   370,   431,     0,
     379,   424,   424,   680,   681,   213,     0,     0,   213,     0,
       0,     0,     0,   608,   518,   664,   664,   258,   254,   595,
     260,     0,    73,   269,   266,   511,   213,   213,   438,   436,
     363,   364,   518,   423,   315,     0,     0,     0,   213,   305,
       0,     0,     0,     0,     0,     0,    62,     0,   213,   213,
     213,   213,   213,    95,    98,    84,    83,    82,   351,   205,
     211,   209,     0,     0,   216,     0,   251,     0,     0,   223,
       0,   222,     0,   610,     0,     0,   229,     0,     0,   227,
       0,   237,     0,   235,     0,   250,     0,     0,     0,     0,
     248,   213,   297,   351,   334,   333,   345,   349,   490,     0,
     491,   478,   481,     0,   477,     0,   425,   374,   375,     0,
     213,   213,     0,   213,   213,   213,   213,   612,     0,   263,
       0,     0,   440,   439,     0,   314,     0,   308,     0,     0,
     123,    91,    90,     0,     0,     0,     0,   213,   206,   220,
     219,   226,     0,   213,   576,   213,   575,   213,   609,     0,
     213,   582,   230,   213,   581,   228,   238,   236,   213,   245,
     592,     0,   213,   243,   591,     0,   704,   335,     0,   489,
     213,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     470,   472,   474,   371,   471,   213,   213,   213,   213,   213,
     213,   213,   213,   262,   627,   628,   316,   306,    97,   100,
      96,    99,    85,   580,     0,     0,   611,   586,     0,     0,
       0,   246,     0,   244,   350,   480,   482,   483,   486,   487,
     488,   484,   485,   479,   473,     0,     0,     0,     0,     0,
       0,   629,   630,     0,     0,     0,     0,     0,     0,   213,
     213,   213,   213,   213,   213,   579,   578,   585,   584,   594,
     593
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1072, -1072, -1072,    -1,   -54,  2844, -1072,   311,   774, -1072,
   -1072,   401,  -149,  -103, -1072,   307, -1072, -1072,   -97, -1072,
   -1072,    62,   628, -1072,  -541,  3514,   518,  -578, -1072,  -787,
   -1072, -1072, -1072,    75, -1072, -1072, -1072,   872, -1072,  4071,
    -214, -1072, -1072,  -462,   -30, -1071,  -839, -1072, -1072,  -207,
   -1072, -1072, -1072, -1072, -1072, -1072, -1072, -1072,     7, -1072,
     546,  -298, -1072,   506, -1072, -1072,   -53,  1796, -1072,  -322,
   -1072, -1072, -1072, -1072, -1072,   571, -1072,   -72, -1072, -1072,
   -1072, -1072,   377, -1072, -1072, -1072,   -91, -1072,  -335,  -789,
   -1072, -1072, -1072, -1072, -1072,  -391, -1072,    42, -1072, -1072,
   -1072, -1072, -1072,   371, -1072,   241, -1072, -1072, -1072, -1072,
     534, -1072, -1072, -1072, -1072,    18,  -402,  -164,  -774,  -907,
    -646, -1072,    23, -1072,    35,   379,   -39,   621,   -37, -1072,
   -1072,  -333,  -798, -1072,  -312, -1072,  -136,  -318,  -310,  -926,
   -1072, -1072,    93,   254, -1072,  -193,   802, -1072,  -159,   413,
     173,  -244,  -750,  -602, -1072, -1072, -1072,  -589,  -504, -1072,
    -738,   -10,   108, -1072,  -259,  -476,  -529,    -2, -1072, -1072,
   -1072, -1072, -1072, -1072,   419, -1072,   -18, -1072, -1072,  -586,
   -1072,  -885, -1072, -1072,   561,  1052, -1072, -1072, -1072, -1072,
    2986, -1072,   -51,  1179, -1072,  1558,  1586, -1072, -1072, -1072,
   -1072, -1072, -1072, -1072, -1072, -1072,  -481
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   348,   105,   758,   107,   108,   109,   366,
     367,   110,   111,   112,   113,   373,   580,   811,   812,   114,
     115,   116,   378,   117,   118,   119,   349,   839,   587,   840,
     120,   121,  1006,   584,   585,   122,   123,   217,   252,   270,
     124,   254,   125,   304,   853,   126,   127,   291,   290,   128,
     129,   130,   131,   132,   133,   596,   843,   134,   202,   136,
     393,   602,   137,   258,   138,   767,   768,   203,   140,   265,
     266,   141,   142,   143,   144,   559,   799,   979,   145,   146,
     795,   974,   277,   278,   147,   148,   149,   150,   398,   881,
     151,   152,   403,   883,   884,   404,   153,   204,  1238,   155,
     156,   305,   306,   307,   963,   157,   319,   552,   792,   158,
     159,  1175,   160,   161,   676,   677,   835,   836,   837,   967,
     895,   406,   625,  1069,   626,   546,   627,   309,   540,   421,
     410,   416,   893,  1253,  1254,   162,  1067,  1171,  1172,  1173,
     163,   164,   423,   424,   682,   165,   166,   221,   271,   272,
     519,   520,   775,   311,   891,   632,   633,   523,   312,   789,
     790,   350,   353,   354,   495,   496,   497,   205,   168,   169,
     394,   170,   395,   171,   206,   173,   214,   215,   744,   860,
    1142,   745,   746,   174,   207,   208,   177,   178,   179,   320,
     180,   385,   498,   210,   182,   211,   212,   185,   186,   187,
     188,   614,   189,   190,   191,   192,   193
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   104,   219,   401,   509,   748,   425,   303,   370,   135,
     620,   793,   834,   494,   275,   322,   225,   475,   856,   678,
     862,   865,   308,   167,   310,   870,   874,   796,   490,   890,
     534,   899,   135,   900,   764,   339,  1083,  1012,   324,   887,
     968,   383,  1060,   791,   154,  1007,   961,   521,   594,   494,
     813,   355,   371,  1018,  1098,   314,   273,   482,   483,   794,
     772,   521,   273,   273,  1145,  1249,  1249,   154,  1168,  1065,
     557,   273,   167,   628,   616,   494,   167,   167,  -529,   971,
     630,   135,  -288,   535,   536,   135,   135,   917,   524,  1064,
     521,   604,   606,  1138,   611,   507,     3,   262,   537,   167,
     359,   386,   375,   167,  1252,   502,  -529,   854,   135,   387,
     871,  -527,   135,   982,  -271,   494,   154,   388,   389,   390,
     154,   154,   972,  -289,   565,    66,    66,  1066,   876,   194,
     699,   919,  -312,   213,   689,   691,  -529,   386,  -529,  -527,
     376,   538,   386,   154,   854,   387,  1143,   154,   566,   216,
    1194,  -529,   854,   574,   575,   390,   877,  1169,   388,   389,
     390,  1168,   869,   220,  -529,  1101,   363,  1252,  1170,   889,
     377,  -527,   368,   364,   920,  -312,   973,   528,   492,  1060,
     731,   733,   955,   854,  -527,  -529,   854,   872,   525,   765,
      82,   484,   522,   372,   364,  1192,   391,  -527,  1250,   227,
     364,   364,   364,   364,   364,  1095,   522,  -529,   785,   364,
     872,  1099,   167,   492,  -529,   918,  -529,   729,   730,  1164,
    1165,   135,   874,    23,   939,   631,  1102,   705,  1128,  -312,
     703,   364,   391,  -529,   281,   522,  -529,   391,   873,  1125,
     263,   983,   854,  1239,  -527,  1011,   527,  1119,   529,  -527,
     854,   392,   551,  -272,  -529,   533,   154,   679,   841,  1240,
     435,  1170,   494,   494,   699,   478,   167,   492,   441,   479,
    -278,   167,  -344,   769,    55,   135,   774,   604,   611,  -340,
     135,   700,   431,  1120,   965,   493,    64,   576,  1241,  1060,
     -70,   593,   392,   282,   701,   555,   494,  1156,   370,  -365,
     787,   622,   966,  -340,   417,  1198,   292,   318,   222,   -70,
     154,    84,   283,   568,  1013,   154,  1287,  1288,  1289,  1290,
    1291,  1292,  -277,   702,  1237,   623,   294,   318,   -70,   284,
     296,   749,  1276,  1204,  1187,  1206,   227,   780,  1200,   569,
     432,   255,   494,   318,   433,  1084,  -286,   167,   564,   570,
      37,   303,  1193,  -351,   297,   418,   135,   298,   -70,    57,
    1100,   476,   167,   477,   703,   494,   511,   419,   194,   299,
     704,   135,   167,  -351,   562,   571,    53,   492,  1242,   -69,
     411,   135,   612,  -351,   512,   167,   420,   617,   302,   333,
     336,   154,   435,   194,   135,   548,   770,   439,   -69,   814,
     441,  -276,  1053,   844,   444,  1243,   154,  1244,  1245,   595,
    1246,  1247,   292,   292,   724,   725,   154,   -69,   407,   680,
     726,   172,   771,   727,   728,  1177,  1178,  1117,   412,   154,
     885,   413,   294,   294,   263,  -290,   296,   296,   408,   288,
     748,   921,   878,   414,   172,   753,   951,   -69,   409,  1147,
    1150,   770,   624,  1152,  1154,   813,   886,   770,  1157,  1159,
     297,   297,   415,   298,   298,    57,   357,   922,   318,  -273,
    -280,   950,   952,   984,   738,   299,   299,   953,   431,   334,
     334,   492,   753,   964,  -614,   936,  -613,  1217,   303,   479,
    -284,   289,  1061,   172,   302,   302,   678,   172,   172,   985,
     167,  -281,   167,   308,   318,   310,  -275,  1136,  1162,   135,
    -614,   135,  -613,  1218,   683,   525,   816,   355,  -747,   355,
     172,  -748,  -747,   328,   172,  -748,  1147,  1150,  1157,  1159,
     318,   820,   684,   685,   817,   525,   432,  1013,  1013,   494,
     433,  1203,   260,  1205,   154,   261,   154,  1021,  -279,   821,
     748,   683,  -601,   937,  1029,  1014,  1123,  -285,  -430,  -671,
    1035,  -671,  -287,   175,  -749,  1039,   808,   525,  -749,  1130,
     318,   329,   850,  1044,   855,   859,   861,   864,   866,   867,
     846,   167,   564,  1013,  1013,  1131,   175,  -274,   435,   436,
     135,   437,   438,   439,  -744,   440,   441,   442,  -744,  -282,
     444,  1268,  1270,   340,   987,   705,   450,   451,    88,   374,
     454,   431,   292,   455,   456,   457,  1076,   396,  1079,  1081,
    -672,  -671,  -672,  -671,   458,   154,  -668,   364,  -668,   513,
     397,   405,   294,   172,  1251,   175,   296,   689,   731,   175,
     175,  -598,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,  -670,   426,  -670,
     297,   485,   175,   298,   494,    57,   175,   486,  -669,   432,
    -669,  1166,   903,   433,   904,   299,   803,  -597,   805,   427,
    -602,  -603,  1110,  1112,  1114,  1020,   474,   172,  -600,  -668,
     435,  -668,   172,  -599,   302,   439,   488,  1251,   441,   364,
     938,   491,   444,   499,   376,   542,   318,   318,   541,   510,
     531,   532,   318,   547,   556,   318,   318,   318,   318,   553,
     561,   435,   436,   957,   437,   438,   439,   572,   440,   441,
     578,   579,  1059,   444,  1072,  1062,   748,   589,   618,   450,
     451,   619,   621,   454,   934,   629,   455,   456,   457,   634,
     681,   318,   441,   723,   751,   167,   752,   753,   167,   754,
     755,   762,   757,   760,   135,   763,   774,   135,   172,   783,
     494,   797,   788,   798,   842,   175,   372,   859,   879,   847,
     849,   892,  -142,   172,   492,   431,   894,  -163,  -170,  -169,
     912,  -168,  -165,   172,  -172,  -173,  -167,   318,  -171,   154,
    -143,  -166,   154,  -174,   897,   898,   172,   942,   878,   318,
     944,   318,   962,   905,  1013,   906,  1023,   911,   907,  1025,
     318,  1091,   908,   909,  1180,  1183,  1032,  1058,  1108,   175,
    1037,   303,  1038,   910,   175,  1041,   970,  1190,  1191,  1026,
     303,  1017,   975,   432,  1016,  1031,  1096,   433,  1092,   167,
    1043,  1054,  1049,  1074,  1073,   167,  1097,  1124,   135,   885,
    1176,  1185,  1188,  1196,   135,  1269,  1231,   167,  1235,  1059,
    1264,   167,  1265,   359,  1271,  1301,   135,    22,    23,  1302,
     135,  1241,   494,   494,   365,   809,   360,  1116,    29,   361,
     508,   986,   583,   154,    32,   435,   223,   608,  1093,   154,
     439,    37,   440,   441,  1107,   778,   362,   444,   560,  1163,
     175,   154,   577,   786,   451,   154,  1071,  1068,   784,   597,
    1063,   172,   457,   172,   539,   175,  1284,    53,  1167,    55,
    1293,  1286,   380,  1085,  1086,   175,  1019,   902,   777,   363,
     167,    64,   167,   954,     0,   167,     0,     0,   175,   135,
       0,   135,     0,     0,   135,     0,   370,     0,     0,   370,
      80,     0,  1106,    82,  1058,     0,    84,     0,     0,   167,
       0,     0,     0,   507,     0,     0,     0,     0,  1105,  1059,
     507,   167,     0,     0,   154,     0,   154,     0,     0,   154,
     135,     0,   318,     0,   318,     0,     0,     0,     0,  1139,
    1141,     0,   172,  1132,  1133,  1146,  1149,     0,     0,  1151,
    1153,     0,     0,   154,     0,   318,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   154,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   167,     0,     0,     0,     0,     0,   167,     0,     0,
     135,     0,     0,     0,   176,     0,   135,     0,     0,     0,
     370,     0,     0,   175,     0,   175,     0,     0,     0,     0,
       0,     0,     0,     0,  1058,     0,  1195,   176,     0,     0,
    1139,  1141,  1146,  1149,     0,   154,     0,     0,     0,   167,
       0,   154,     0,     0,     0,     0,     0,     0,   135,     0,
       0,     0,  1209,  1210,     0,  1211,     0,     0,     0,     0,
     507,   507,     0,     0,   507,   507,   507,  1222,   507,   318,
    1225,     0,  1226,     0,  1227,     0,   176,     0,     0,     0,
     176,   176,     0,   154,     0,     0,   167,     0,   167,     0,
       0,     0,     0,   167,   175,   135,   167,   135,     0,     0,
     431,     0,   135,   176,   167,   135,   167,   176,     0,     0,
       0,     0,     0,   135,     0,   135,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   172,     0,     0,   172,
     154,   181,   154,     0,     0,     0,   167,   154,     0,     0,
     154,     0,   167,     0,     0,   135,   167,   167,   154,     0,
     154,   135,     0,     0,   181,   135,   135,     0,   432,     0,
     167,     0,   433,     0,     0,     0,     0,   167,     0,   135,
       0,     0,     0,     0,     0,   318,   135,     0,     0,   167,
     154,     0,     0,   167,     0,     0,   154,     0,   135,     0,
     154,   154,   135,     0,     0,     0,     0,   167,  1294,     0,
       0,     0,     0,   181,   154,     0,   135,   181,   181,     0,
     435,   154,     0,     0,     0,   439,   176,   440,   441,     0,
     172,     0,   444,   154,   431,     0,   172,   154,     0,   451,
     181,     0,     0,     0,   181,   455,   456,   457,   172,     0,
       0,   154,   172,     0,     0,     0,     0,     0,     0,     0,
       0,   167,   167,   167,   167,   167,   167,     0,     0,     0,
     135,   135,   135,   135,   135,   135,     0,     0,   175,     0,
     176,   175,     0,     0,     0,   176,     0,     0,     0,     0,
       0,     0,   432,     0,     0,     0,   433,   318,   318,     0,
       0,     0,     0,     0,     0,   154,   154,   154,   154,   154,
     154,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   172,   431,   172,     0,     0,   172,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   435,   436,     0,   437,   438,   439,
     172,   440,   441,   181,     0,     0,   444,     0,     0,     0,
       0,   176,   172,   451,     0,     0,   880,     0,     0,   455,
     456,   457,   175,     0,     0,     0,   176,     0,   175,     0,
     432,     0,     0,     0,   433,     0,   176,  -331,     0,     0,
     175,  -331,  -331,     0,   175,     0,     0,     0,     0,   176,
    -331,     0,  -331,  -331,     0,     0,     0,   181,  -331,     0,
       0,     0,   181,     0,     0,  -331,     0,     0,  -331,     0,
    -331,     0,   172,   431,     0,     0,     0,     0,   172,     0,
       0,     0,   435,   436,     0,     0,   438,   439,  -331,   440,
     441,  -331,     0,  -331,   444,  -331,     0,  -331,  -331,     0,
    -331,   451,     0,  -331,     0,  -331,     0,   455,   456,   457,
       0,     0,     0,   175,     0,   175,     0,     0,   175,     0,
     172,     0,     0,     0,  -331,     0,     0,  -331,     0,     0,
    -331,   432,     0,     0,     0,   433,     0,     0,   181,     0,
       0,     0,   175,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   181,   175,  -331,     0,     0,     0,     0,
       0,     0,     0,   181,   176,     0,   176,   172,     0,   172,
     183,     0,     0,     0,   172,     0,   181,   172,     0,     0,
    -331,     0,     0,   435,   436,   172,  -331,   172,   439,     0,
     440,   441,     0,   183,     0,   444,     0,     0,   184,     0,
       0,     0,   451,     0,     0,     0,     0,     0,   455,   456,
     457,     0,     0,     0,   175,     0,     0,   172,     0,     0,
     175,   184,     0,   172,     0,     0,     0,   172,   172,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   172,   183,     0,     0,   176,   183,   183,   172,     0,
       0,     0,     0,     0,   699,     0,     0,     0,     0,     0,
     172,     0,   175,     0,   172,     0,     0,     0,   923,   183,
     184,   924,     0,   183,   184,   184,   925,     0,   172,     0,
       0,     0,     0,     0,     0,     0,   196,     0,     0,     0,
       0,   181,     0,   181,     0,     0,   926,   184,     0,     0,
       0,   184,     0,   927,     0,     0,     0,     0,     0,   175,
       0,   175,     0,   928,     0,     0,   175,     0,     0,   175,
       0,   929,     0,     0,     0,     0,     0,   175,     0,   175,
       0,     0,   172,   172,   172,   172,   172,   172,   930,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     931,     0,     0,     0,   703,     0,     0,     0,     0,   175,
       0,   932,     0,     0,     0,   175,     0,   933,     0,   175,
     175,     0,   181,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   183,   175,     0,     0,     0,     0,     0,     0,
     175,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   175,     0,     0,     0,   175,     0,   139,     0,
     184,     0,     0,     0,     0,     0,     0,     0,     0,   176,
     175,     0,   176,     0,     0,     0,     0,     0,     0,     0,
       0,   139,     0,     0,     0,     0,   183,     0,     0,     0,
       0,   183,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   184,     0,     0,     0,     0,   184,
       0,     0,     0,     0,   175,   175,   175,   175,   175,   175,
     139,     0,     0,     0,   228,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   229,   230,
       0,   231,     0,     0,     0,     0,   232,   139,     0,     0,
       0,   139,     0,   176,   233,     0,     0,   183,     0,   176,
     234,     0,     0,     0,     0,     0,   235,     0,     0,     0,
     236,   176,   183,   237,     0,   176,     0,     0,     0,     0,
       0,     0,   183,   238,     0,   184,   181,     0,     0,   181,
     239,   240,     0,     0,     0,   183,     0,     0,   241,     0,
     184,     0,     0,     0,     0,     0,     0,     0,   242,     0,
     184,     0,     0,     0,     0,     0,     0,   243,   244,     0,
     245,     0,   246,   184,   247,     0,     0,   248,     0,     0,
       0,   249,   516,     0,   250,     0,     0,   251,     0,     0,
       0,     0,     0,   359,   176,     0,   176,    22,    23,   176,
       0,     0,     0,     0,     0,     0,   360,     0,    29,   361,
     139,     0,     0,     0,    32,     0,     0,     0,     0,     0,
       0,    37,     0,   176,     0,     0,   362,     0,     0,     0,
     181,     0,     0,     0,     0,   176,   181,     0,     0,     0,
       0,     0,     0,   517,     0,     0,     0,    53,   181,    55,
       0,    57,   181,  1055,     0,     0,  1056,     0,     0,   363,
     183,    64,   183,     0,   139,     0,     0,     0,     0,   139,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,   184,     0,
     184,     0,     0,     0,     0,   176,     0,     0,     0,     0,
       0,   176,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   181,     0,   181,     0,     0,   181,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,     0,
       0,   183,  1057,   176,     0,   139,     0,     0,     0,     0,
     181,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     139,     0,   181,     0,     0,   228,     0,     0,     0,   184,
     139,     0,     0,     0,     0,     0,     0,     0,     0,   229,
     230,     0,   231,   139,     0,     0,     0,   232,     0,     0,
     176,     0,   176,     0,     0,   233,     0,   176,     0,     0,
     176,   234,     0,     0,     0,     0,     0,   235,   176,     0,
     176,   236,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   181,     0,   238,     0,     0,     0,   181,     0,
       0,   239,   240,     0,     0,     0,     0,     0,     0,   241,
     176,     0,     0,     0,     0,     0,   176,     0,     0,   242,
     176,   176,     0,     0,     0,     0,     0,     0,   243,   244,
       0,   245,     0,   246,   176,   247,     0,     0,   248,     0,
     181,   176,   249,   543,     0,   250,   359,     0,   251,     0,
      22,    23,     0,   176,     0,     0,     0,   176,     0,   360,
       0,    29,   361,     0,     0,     0,     0,    32,   139,     0,
     139,   176,     0,     0,    37,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,   183,     0,   181,   183,   181,
       0,     0,     0,     0,   181,     0,     0,   181,     0,     0,
      53,     0,    55,     0,   544,   181,  1055,   181,     0,  1056,
       0,     0,   363,   184,    64,     0,   184,     0,     0,     0,
       0,     0,     0,     0,     0,   176,   176,   176,   176,   176,
     176,     0,     0,    80,     0,     0,    82,   181,     0,    84,
       0,     0,     0,   181,     0,     0,     0,   181,   181,   139,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   181,     0,     0,    88,     0,     0,     0,   181,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   183,
     181,     0,     0,     0,   181,   183,     0,     0,     0,    99,
       0,     0,     0,     0,     0,     0,     0,   183,   181,     0,
       0,   183,     0,     0,     0,     0,     0,   184,     0,     0,
       0,     0,     0,   184,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   184,     0,     0,     0,   184,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   228,     0,     0,
       0,     0,   181,   181,   181,   181,   181,   181,     0,     0,
       0,   229,   230,     0,   231,     0,     0,     0,     0,   232,
     183,     0,   183,     0,     0,   183,     0,   233,     0,     0,
       0,     0,     0,   234,     0,     0,     0,     0,     0,   235,
       0,     0,     0,   236,     0,     0,   237,     0,   184,   183,
     184,     0,     0,   184,     0,     0,   238,     0,     0,     0,
       0,   183,     0,   239,   240,     0,     0,     0,     0,     0,
       0,   241,     0,   139,     0,     0,   139,   184,     0,     0,
       0,   242,     0,     0,     0,     0,     0,     0,     0,   184,
     243,   244,     0,   245,     0,   246,     0,   247,     0,     0,
     248,     0,     0,     0,   249,     0,     0,   250,     0,     0,
     251,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   183,     0,     0,   359,     0,     0,   183,    22,    23,
       0,     0,     0,     0,     0,     0,     0,   360,     0,    29,
     361,     0,     0,     0,     0,    32,     0,     0,     0,   184,
       0,     0,    37,     0,     0,   184,     0,   362,     0,     0,
       0,     0,     0,     0,   364,     0,   976,   139,     0,   183,
       0,     0,     0,   139,     0,     0,     0,     0,    53,     0,
      55,     0,    57,     0,  1055,   139,     0,  1056,     0,   139,
     363,     0,    64,     0,     0,     0,     0,   184,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,   430,
       0,    80,     0,     0,    82,     0,   183,    84,   183,     0,
       0,   431,   857,   183,     0,     0,   183,     0,     0,     0,
       0,     0,     0,     0,   183,     0,   183,   858,     0,     0,
       0,     0,    88,     0,   184,     0,   184,     0,     0,     0,
       0,   184,     0,     0,   184,     0,     0,     0,   139,     0,
     139,     0,   184,   139,   184,     0,   183,    99,     0,     0,
       0,     0,   183,  1129,     0,     0,   183,   183,     0,   432,
       0,     0,     0,   433,     0,     0,     0,   139,     0,     0,
     183,     0,     0,     0,   184,     0,     0,   183,     0,   139,
     184,     0,     0,     0,   184,   184,     0,     0,     0,   183,
     854,     0,     0,   183,   434,     0,     0,     0,   184,     0,
       0,     0,     0,     0,     0,   184,     0,   183,     0,     0,
       0,   435,   436,     0,   437,   438,   439,   184,   440,   441,
     442,   184,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,     0,   184,   455,   456,   457,   139,
     364,     0,     0,     0,     0,   139,   106,   458,     0,     0,
     431,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   183,   183,   183,   183,   183,   183,     0,     0,   224,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   139,     0,   184,
     184,   184,   184,   184,   184,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   432,     0,
       0,     0,   433,     0,     0,     0,     0,     0,   330,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   139,     0,   139,     0,     0,     0,
       0,   139,     0,   434,   139,   106,     0,     0,     0,   358,
       0,     0,   139,     0,   139,     0,     0,     0,     0,     0,
     435,   436,     0,   437,   438,   439,     0,   440,   441,   442,
       0,   443,   444,   445,   446,     0,   448,   449,   450,   451,
     452,   453,   454,     0,   139,   455,   456,   457,     0,     0,
     139,     0,     0,     0,   139,   139,   458,     0,     0,   209,
       0,     0,     0,   218,     0,     0,     0,     0,   139,     0,
       0,     0,   226,     0,     0,   139,     0,     0,     0,   259,
       0,     0,     0,     0,     0,     0,     0,   139,     0,     0,
     276,   139,   279,   280,     0,     0,     0,     0,   285,     0,
     286,     0,   287,     0,     0,   139,     0,   313,     0,     0,
       0,   226,   321,   323,   325,   326,   327,     0,   487,     0,
     331,     0,   332,     0,     0,     0,     0,     0,     0,   338,
       0,     0,   321,     0,     0,   341,     0,   342,     0,     0,
     343,   344,     0,   345,     0,     0,     0,   226,   321,   356,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   139,
     139,   139,   139,   139,   139,     0,     0,   228,     0,     0,
       0,     0,   515,     0,     0,     0,   381,   526,   384,     0,
       0,   229,   230,     0,   231,     0,     0,     0,     0,   232,
       0,     0,     0,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,   234,     0,     0,     0,     0,     0,   235,
       0,     0,     0,   236,     0,     0,   237,     0,     0,     0,
     428,     0,     0,     0,     0,     0,   238,     0,     0,     0,
       0,     0,     0,   239,   240,     0,     0,     0,     0,     0,
       0,   241,     0,   480,     0,     0,   331,   226,     0,     0,
       0,   242,     0,   106,     0,     0,     0,     0,     0,     0,
     243,   244,     0,   245,     0,   246,     0,   247,   106,     0,
     248,     0,     0,   321,   249,     0,     0,   250,   106,     0,
     251,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   599,     0,     0,     0,     0,     0,   359,     0,     0,
       0,    22,    23,     0,     0,     0,     0,     0,     0,   321,
     360,     0,    29,   361,     0,     0,     0,     0,    32,     0,
       0,     0,     0,     0,     0,    37,     0,     0,     0,     0,
     362,     0,     0,     0,     0,   321,   269,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   327,   331,     0,     0,
       0,    53,     0,    55,     0,    57,     0,  1055,     0,     0,
    1056,   549,   550,   363,     0,    64,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   321,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,   567,     0,
       0,     0,     0,     0,     0,     0,   756,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,   586,     0,   588,     0,     0,     0,     0,     0,     0,
       0,     0,   598,     0,   603,   605,   607,   609,   613,     0,
      99,     0,     0,   226,     0,     0,  1208,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   686,   687,   688,   690,   692,
     693,     0,   694,   695,   696,   697,   698,   106,   706,   707,
     708,   709,   710,   711,   712,   713,   714,   715,   716,   717,
     718,   719,   720,   721,   722,     0,     0,     0,     0,     0,
       0,   321,   321,     0,     0,     0,     0,   321,     0,     0,
     321,   321,   321,   321,   732,   734,     0,     0,   736,   737,
     613,     0,   747,     0,     0,     0,     0,     0,   750,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     759,     0,     0,   761,     0,     0,   321,   429,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,   773,     0,
     776,     0,     0,     0,   279,     0,   431,   781,   782,     0,
       0,   590,     0,     0,     0,     0,     0,     0,   313,     0,
       0,     0,     0,   313,     0,     0,     0,     0,     0,     0,
       0,     0,   321,     0,     0,     0,     0,     0,     0,     0,
       0,   750,     0,     0,   321,     0,   321,   591,   274,     0,
     806,   807,   226,     0,     0,   321,     0,     0,     0,     0,
     431,     0,     0,     0,   432,     0,   838,   838,   433,     0,
       0,     0,   274,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   335,   337,     0,     0,   868,     0,     0,     0,
       0,   943,     0,     0,   946,     0,     0,   882,     0,   434,
       0,     0,     0,     0,     0,     0,     0,   313,     0,     0,
       0,     0,     0,     0,   369,     0,   435,   436,   432,   437,
     438,   439,   433,   440,   441,   442,     0,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,     0,
     592,   455,   456,   457,     0,     0,     0,     0,     0,     0,
     896,     0,   458,   434,     0,     0,     0,   901,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     435,   436,     0,   437,   438,   439,     0,   440,   441,   442,
       0,   443,   444,   445,   446,   526,   448,   449,   450,   451,
     452,   526,   454,     0,     0,   455,   456,   457,     0,     0,
       0,     0,     0,  1042,     0,     0,   458,  1047,     0,     0,
       0,   759,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   489,     0,     0,     0,   321,     0,   321,
     940,   941,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     321,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   503,   506,   313,     0,     0,     0,   969,     0,
       0,   514,     0,     0,     0,     0,  1087,     0,  1088,     0,
     274,  1090,   274,     0,     0,     0,     0,     0,     0,   274,
       0,     0,   586,     0,     0,     0,  1008,     0,  1010,   838,
     838,     0,   341,     0,   342,  1104,     0,   344,   345,     0,
       0,     0,     0,     0,     0,     0,     0,  1047,     0,     0,
       0,  1022,     0,  1024,     0,   226,     0,  1028,  1030,   274,
       0,     0,     0,  1034,  1036,     0,     0,     0,     0,  1040,
       0,     0,     0,     0,     0,     0,     0,  1045,   750,  1048,
       0,  1050,  1052,     0,   321,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1155,     0,     0,
       0,     0,     0,  1160,     0,     0,     0,     0,     0,  1028,
    1077,  1034,  1080,  1082,  1048,     0,     0,   747,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   690,   732,     0,     0,     0,     0,     0,     0,     0,
       0,  1089,     0,     0,     0,  1189,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   313,     0,
       0,     0,  1103,     0,     0,     0,     0,     0,   226,     0,
     359,     0,     0,     0,    22,    23,  1111,  1113,  1115,     0,
     321,     0,     0,   360,     0,    29,   361,     0,     0,     0,
       0,    32,   526,     0,   526,     0,     0,     0,    37,   526,
       0,   838,   526,   362,     0,     0,     0,     0,     0,  1126,
    1229,  1127,  1233,     0,     0,     0,     0,     0,     0,     0,
       0,  1135,     0,  1137,    53,     0,    55,   747,   357,  1144,
    1055,     0,     0,  1056,     0,     0,   363,     0,    64,     0,
       0,     0,  1263,     0,     0,     0,     0,     0,  1266,     0,
       0,     0,  1229,  1233,     0,     0,     0,    80,     0,     0,
      82,     0,  1174,    84,     0,     0,     0,     0,     0,     0,
       0,  1179,     0,     0,  1182,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1281,     0,     0,    88,  1283,
       0,     0,   321,   321,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   106,  1197,     0,     0,     0,     0,   253,
       0,     0,     0,    99,  1202,   838,   838,   838,   838,     0,
     264,   267,   268,     0,     0,     0,   274,   274,   274,   274,
     274,   274,   274,   274,  1214,     0,  1216,     0,     0,     0,
       0,  1221,     0,     0,  1224,     0,     0,     0,     0,     0,
       0,     0,  1230,     0,  1234,     0,     0,  1236,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1214,  1216,     0,  1221,
    1224,  1230,  1234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1272,     0,     0,     0,     0,  1273,  1274,
       0,  1275,     0,   747,     0,  1277,  1278,     0,     0,  1279,
       0,     0,     0,     0,  1280,     0,     0,     0,  1282,     0,
       0,   399,     0,   402,     0,     0,  1285,     0,     0,     0,
       0,     0,     0,     0,     0,   422,     0,     0,     0,     0,
       0,  1273,  1295,  1296,  1277,  1297,  1298,  1299,  1300,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
     430,     0,     0,     0,     0,     0,     0,     0,     0,   481,
       0,     0,   431,   273,     0,   948,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   863,  1315,
    1316,  1317,  1318,  1319,  1320,  1315,  1316,  1317,  1318,  1319,
    1320,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   977,     0,     0,     0,     0,     0,     0,     0,
     274,   274,     0,     0,     0,     0,     0,   500,   501,     0,
     432,     0,     0,     0,   433,     0,     0,     0,     0,     0,
     518,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     274,   854,     0,     0,     0,   434,     0,     0,     0,     0,
     274,     0,     0,     0,   274,     0,   274,     0,     0,   274,
     545,     0,   435,   436,     0,   437,   438,   439,     0,   440,
     441,   442,     0,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,     0,     0,   455,   456,   457,
       0,   364,     0,     0,     0,     0,     0,     0,   458,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   253,     0,   573,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1094,   615,     0,     0,
       0,     0,   402,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   675,  1109,     0,     0,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   195,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   196,     0,
       0,     0,    31,  -213,  -213,     0,     0,   197,    34,     0,
       0,     0,    36,   274,   274,    38,     0,     0,   198,   274,
     274,    42,    43,   274,   274,    45,   739,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
      62,   199,   573,     0,   766,     0,     0,     0,   518,    69,
      70,    71,    72,    73,   200,     0,    75,     0,     0,    77,
       0,   779,     0,    81,     0,     0,    83,     0,     0,    85,
     545,     0,     0,     0,     0,   518,     0,     0,     0,     0,
     274,     0,     0,     0,   274,   274,   274,   274,     0,     0,
       0,     0,     0,     0,    89,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   274,   274,     0,   274,
       0,   506,     0,     0,     0,     0,     0,    98,   506,     0,
     825,   274,     0,   201,   274,   102,   274,  -213,   274,     0,
       0,   848,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,     0,    17,     0,     0,     0,   195,     0,
    1267,     0,     0,   292,     0,     0,     0,     0,     0,     0,
       0,   196,     0,     0,     0,     0,  -213,  -213,     0,     0,
     197,     0,     0,   294,     0,     0,     0,   296,     0,     0,
       0,   198,     0,     0,    42,    43,     0,     0,     0,     0,
      47,     0,    48,    49,   422,     0,     0,     0,     0,     0,
       0,   297,   429,    54,   298,    56,    57,    58,     0,   430,
       0,     0,    61,     0,   199,     0,   299,     0,     0,     0,
       0,   431,    69,     0,    71,     0,    73,     0,   506,   506,
       0,     0,   506,   506,   506,   302,   506,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,   949,     0,     0,    94,     0,     0,   432,
       0,   518,     0,   433,     0,    95,     0,     0,    96,    97,
      98,   958,   504,     0,   100,     0,   201,     0,   102,  -355,
    -213,     0,     0,   103,     0,     0,     0,     0,     0,   978,
       0,     0,     0,     0,   434,   505,   460,   461,   462,   463,
     464,     0,     0,   467,   468,   469,   470,     0,   472,   473,
       0,   435,   436,     0,   437,   438,   439,     0,   440,   441,
     442,     0,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   422,     0,   455,   456,   457,     0,
     364,     0,     0,     0,     0,     0,     0,   458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   402,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   675,
       0,     0,     0,     0,     0,    -2,     4,     0,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -213,
      21,    22,    23,    24,    25,    26,     0,  -213,    27,     0,
      28,     0,    29,    30,    31,  -213,  -213,  -213,    32,    33,
      34,   -68,   766,    35,    36,    37,     0,    38,  -351,     0,
      39,    40,    41,    42,    43,     0,    44,    45,    46,    47,
     -68,    48,    49,     0,    50,    51,    52,   978,  -351,     0,
       0,    53,    54,    55,    56,    57,    58,    59,  -351,   -68,
      60,    61,    62,    63,     0,    64,    65,    66,     0,    67,
      68,    69,    70,    71,    72,    73,    74,     0,    75,    76,
       0,    77,    78,    79,    80,    81,     0,    82,    83,   -68,
      84,    85,     0,     0,  -213,     0,    86,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,   402,    95,     0,     0,    96,    97,    98,
      99,     0,     0,   100,     0,   101,     0,   102,     0,  -213,
       0,     4,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,  -699,     0,    13,    14,    15,    16,    17,  -699,
      18,     0,    19,    20,  -213,    21,    22,    23,    24,    25,
      26,  -699,  -213,    27,  -699,    28,     0,    29,    30,    31,
    -213,  -213,  -213,    32,    33,    34,   -68,     0,    35,    36,
      37,     0,    38,  -351,     0,    39,    40,    41,    42,    43,
       0,    44,    45,    46,    47,   -68,    48,    49,     0,    50,
      51,    52,     0,  -351,     0,     0,    53,    54,    55,    56,
       0,    58,    59,  -351,   -68,    60,    61,    62,    63,  -699,
      64,    65,    66,  -699,    67,    68,    69,    70,    71,    72,
      73,    74,     0,    75,    76,     0,    77,    78,    79,    80,
      81,     0,    82,    83,   -68,    84,    85,     0,     0,  -213,
       0,    86,     0,     0,  -699,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,  -699,    91,  -699,  -699,  -699,  -699,  -699,  -699,
    -699,     0,  -699,  -699,  -699,  -699,  -699,  -699,  -699,  -699,
    -699,  -699,  -699,  -699,    98,    99,  -699,  -699,  -699,     0,
     101,  -699,   102,     0,  -213,     0,   346,  -699,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -213,
      21,    22,    23,    24,    25,    26,     0,  -213,    27,     0,
      28,     0,    29,    30,    31,  -213,  -213,  -213,    32,    33,
      34,   -68,     0,    35,    36,    37,     0,    38,  -351,     0,
      39,    40,    41,    42,    43,     0,    44,    45,    46,    47,
     -68,    48,    49,     0,    50,    51,    52,     0,  -351,     0,
       0,    53,    54,    55,    56,    57,    58,    59,  -351,   -68,
      60,    61,    62,    63,     0,    64,    65,    66,     0,    67,
      68,    69,    70,    71,    72,    73,    74,     0,    75,    76,
       0,    77,    78,    79,    80,    81,     0,    82,    83,   -68,
      84,    85,     0,     0,  -213,     0,    86,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
      99,     0,     0,   100,     0,   101,   347,   102,     0,  -213,
       0,     4,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -213,    21,    22,    23,    24,    25,
      26,     0,  -213,    27,     0,    28,     0,    29,    30,    31,
    -213,  -213,  -213,    32,    33,    34,   -68,     0,    35,    36,
      37,     0,    38,  -351,     0,    39,    40,    41,    42,    43,
       0,    44,    45,    46,    47,   -68,    48,    49,     0,    50,
      51,    52,     0,  -351,     0,     0,    53,    54,    55,    56,
      57,    58,    59,  -351,   -68,    60,    61,    62,    63,     0,
      64,    65,    66,     0,    67,    68,    69,    70,    71,    72,
      73,    74,     0,    75,    76,     0,    77,    78,    79,    80,
      81,     0,    82,    83,   -68,    84,    85,     0,     0,  -213,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,    99,     0,     0,   100,     0,
     101,   563,   102,     0,  -213,     0,   581,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -213,
      21,    22,    23,    24,    25,    26,     0,  -213,    27,     0,
      28,     0,    29,    30,    31,  -213,  -213,  -213,    32,    33,
      34,   -68,     0,    35,    36,    37,     0,    38,  -351,     0,
      39,    40,    41,    42,    43,     0,    44,    45,    46,    47,
     -68,    48,    49,     0,    50,    51,    52,     0,  -351,     0,
       0,    53,    54,    55,    56,    57,    58,    59,  -351,   -68,
      60,    61,    62,    63,     0,    64,    65,    66,     0,    67,
      68,    69,    70,    71,    72,    73,    74,     0,    75,    76,
       0,    77,    78,    79,    80,    81,     0,    82,    83,   -68,
      84,    85,     0,     0,  -213,     0,    86,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
      99,     0,     0,   100,     0,   101,   582,   102,     0,  -213,
       0,     4,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -213,    21,    22,    23,    24,    25,
      26,     0,  -213,    27,     0,    28,     0,    29,    30,    31,
    -213,  -213,  -213,    32,    33,    34,   -68,     0,    35,    36,
      37,     0,    38,  -351,     0,    39,    40,    41,    42,    43,
       0,    44,    45,    46,    47,   -68,    48,    49,     0,    50,
      51,    52,     0,  -351,     0,     0,    53,    54,    55,    56,
      57,    58,    59,  -351,   -68,    60,    61,    62,    63,     0,
      64,    65,    66,     0,    67,    68,    69,    70,    71,    72,
      73,    74,     0,    75,    76,     0,    77,    78,    79,    80,
      81,     0,    82,    83,   -68,    84,    85,     0,     0,  -213,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,    99,     0,     0,   100,     0,
     101,   815,   102,     0,  -213,     0,     4,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -213,
      21,    22,    23,    24,    25,    26,     0,  -213,    27,     0,
      28,     0,    29,    30,    31,  -213,  -213,  -213,    32,    33,
      34,   -68,     0,    35,    36,    37,     0,    38,  -351,     0,
      39,    40,    41,    42,    43,     0,    44,    45,    46,    47,
     -68,    48,    49,     0,    50,    51,    52,     0,  -351,     0,
       0,    53,    54,    55,    56,   357,    58,    59,  -351,   -68,
      60,    61,    62,    63,     0,    64,    65,    66,     0,    67,
      68,    69,    70,    71,    72,    73,    74,     0,    75,    76,
       0,    77,    78,    79,    80,    81,     0,    82,    83,   -68,
      84,    85,     0,     0,  -213,     0,    86,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
      99,     0,     0,   100,     0,   101,     0,   102,     0,  -213,
       0,     4,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -213,    21,    22,    23,    24,    25,
      26,     0,  -213,    27,     0,    28,     0,    29,    30,    31,
    -213,  -213,  -213,    32,    33,    34,   -68,     0,    35,    36,
      37,     0,    38,  -351,     0,    39,    40,    41,    42,    43,
       0,    44,    45,    46,    47,   -68,    48,    49,     0,    50,
      51,    52,     0,  -351,     0,     0,    53,    54,    55,    56,
      57,    58,    59,  -351,   -68,    60,    61,    62,    63,     0,
      64,    65,    66,     0,    67,    68,    69,    70,    71,    72,
      73,    74,     0,    75,    76,     0,    77,    78,    79,    80,
      81,     0,    82,    83,   -68,    84,    85,     0,     0,  -213,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,    99,     0,     0,   100,     0,
     101,     0,   102,     0,  -213,     0,     4,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -213,
      21,    22,    23,    24,    25,    26,     0,  -213,    27,     0,
      28,     0,    29,    30,    31,  -213,  -213,  -213,    32,    33,
      34,   -68,     0,    35,    36,    37,     0,    38,  -351,     0,
      39,    40,    41,    42,    43,     0,    44,    45,    46,    47,
     -68,    48,    49,     0,    50,    51,    52,     0,  -351,     0,
       0,    53,    54,    55,    56,     0,    58,    59,  -351,   -68,
      60,    61,    62,    63,     0,    64,    65,    66,     0,    67,
      68,    69,    70,    71,    72,    73,    74,     0,    75,    76,
       0,    77,    78,    79,    80,    81,     0,    82,    83,   -68,
      84,    85,     0,     0,  -213,     0,    86,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
      99,     0,     0,   100,     0,   101,     0,   102,     0,  -213,
       0,     4,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -213,    21,    22,    23,    24,    25,
      26,     0,  -213,    27,     0,    28,     0,    29,    30,    31,
    -213,  -213,  -213,    32,  1027,    34,   -68,     0,    35,    36,
      37,     0,    38,  -351,     0,    39,    40,    41,    42,    43,
       0,    44,    45,    46,    47,   -68,    48,    49,     0,    50,
      51,    52,     0,  -351,     0,     0,    53,    54,    55,    56,
       0,    58,    59,  -351,   -68,    60,    61,    62,    63,     0,
      64,    65,    66,     0,    67,    68,    69,    70,    71,    72,
      73,    74,     0,    75,    76,     0,    77,    78,    79,    80,
      81,     0,    82,    83,   -68,    84,    85,     0,     0,  -213,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,    99,     0,     0,   100,     0,
     101,     0,   102,     0,  -213,     0,     4,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -213,
      21,    22,    23,    24,    25,    26,     0,  -213,    27,     0,
      28,     0,    29,    30,    31,  -213,  -213,  -213,    32,  1033,
      34,   -68,     0,    35,    36,    37,     0,    38,  -351,     0,
      39,    40,    41,    42,    43,     0,    44,    45,    46,    47,
     -68,    48,    49,     0,    50,    51,    52,     0,  -351,     0,
       0,    53,    54,    55,    56,     0,    58,    59,  -351,   -68,
      60,    61,    62,    63,     0,    64,    65,    66,     0,    67,
      68,    69,    70,    71,    72,    73,    74,     0,    75,    76,
       0,    77,    78,    79,    80,    81,     0,    82,    83,   -68,
      84,    85,     0,     0,  -213,     0,    86,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
      99,     0,     0,   100,     0,   101,     0,   102,     0,  -213,
       0,     4,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -213,    21,    22,    23,    24,    25,
      26,     0,  -213,    27,     0,    28,     0,    29,    30,    31,
    -213,  -213,  -213,    32,  1213,    34,   -68,     0,    35,    36,
      37,     0,    38,  -351,     0,    39,    40,    41,    42,    43,
       0,    44,    45,    46,    47,   -68,    48,    49,     0,    50,
      51,    52,     0,  -351,     0,     0,    53,    54,    55,    56,
       0,    58,    59,  -351,   -68,    60,    61,    62,    63,     0,
      64,    65,    66,     0,    67,    68,    69,    70,    71,    72,
      73,    74,     0,    75,    76,     0,    77,    78,    79,    80,
      81,     0,    82,    83,   -68,    84,    85,     0,     0,  -213,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,    99,     0,     0,   100,     0,
     101,     0,   102,     0,  -213,     0,     4,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -213,
      21,    22,    23,    24,    25,    26,     0,  -213,    27,     0,
      28,     0,    29,    30,    31,  -213,  -213,  -213,    32,  1215,
      34,   -68,     0,    35,    36,    37,     0,    38,  -351,     0,
      39,    40,    41,    42,    43,     0,    44,    45,    46,    47,
     -68,    48,    49,     0,    50,    51,    52,     0,  -351,     0,
       0,    53,    54,    55,    56,     0,    58,    59,  -351,   -68,
      60,    61,    62,    63,     0,    64,    65,    66,     0,    67,
      68,    69,    70,    71,    72,    73,    74,     0,    75,    76,
       0,    77,    78,    79,    80,    81,     0,    82,    83,   -68,
      84,    85,     0,     0,  -213,     0,    86,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
      99,     0,     0,   100,     0,   101,     0,   102,     0,  -213,
       0,     4,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -213,    21,    22,    23,    24,    25,
      26,     0,  -213,    27,     0,    28,     0,    29,    30,    31,
    -213,  -213,  -213,    32,  1220,    34,   -68,     0,    35,    36,
      37,     0,    38,  -351,     0,    39,    40,    41,    42,    43,
       0,    44,    45,    46,    47,   -68,    48,    49,     0,    50,
      51,    52,     0,  -351,     0,     0,    53,    54,    55,    56,
       0,    58,    59,  -351,   -68,    60,    61,    62,    63,     0,
      64,    65,    66,     0,    67,    68,    69,    70,    71,    72,
      73,    74,     0,    75,    76,     0,    77,    78,    79,    80,
      81,     0,    82,    83,   -68,    84,    85,     0,     0,  -213,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,    99,     0,     0,   100,     0,
     101,     0,   102,     0,  -213,     0,     4,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -213,
      21,    22,    23,    24,    25,    26,     0,  -213,    27,     0,
      28,     0,    29,    30,    31,  -213,  -213,  -213,    32,  1223,
      34,   -68,     0,    35,    36,    37,     0,    38,  -351,     0,
      39,    40,    41,    42,    43,     0,    44,    45,    46,    47,
     -68,    48,    49,     0,    50,    51,    52,     0,  -351,     0,
       0,    53,    54,    55,    56,     0,    58,    59,  -351,   -68,
      60,    61,    62,    63,     0,    64,    65,    66,     0,    67,
      68,    69,    70,    71,    72,    73,    74,     0,    75,    76,
       0,    77,    78,    79,    80,    81,     0,    82,    83,   -68,
      84,    85,     0,     0,  -213,     0,    86,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
      99,     0,     0,   100,     0,   101,     0,   102,     0,  -213,
       0,     4,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -213,    21,    22,    23,    24,    25,
      26,     0,  -213,    27,     0,    28,     0,    29,    30,    31,
    -213,  -213,  -213,    32,  1228,    34,   -68,     0,    35,    36,
      37,     0,    38,  -351,     0,    39,    40,    41,    42,    43,
       0,    44,    45,    46,    47,   -68,    48,    49,     0,    50,
      51,    52,     0,  -351,     0,     0,    53,    54,    55,    56,
       0,    58,    59,  -351,   -68,    60,    61,    62,    63,     0,
      64,    65,    66,     0,    67,    68,    69,    70,    71,    72,
      73,    74,     0,    75,    76,     0,    77,    78,    79,    80,
      81,     0,    82,    83,   -68,    84,    85,     0,     0,  -213,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,    99,     0,     0,   100,     0,
     101,     0,   102,     0,  -213,     0,     4,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -213,
      21,    22,    23,    24,    25,    26,     0,  -213,    27,     0,
      28,     0,    29,    30,    31,  -213,  -213,  -213,    32,  1232,
      34,   -68,     0,    35,    36,    37,     0,    38,  -351,     0,
      39,    40,    41,    42,    43,     0,    44,    45,    46,    47,
     -68,    48,    49,     0,    50,    51,    52,     0,  -351,     0,
       0,    53,    54,    55,    56,     0,    58,    59,  -351,   -68,
      60,    61,    62,    63,     0,    64,    65,    66,     0,    67,
      68,    69,    70,    71,    72,    73,    74,     0,    75,    76,
       0,    77,    78,    79,    80,    81,     0,    82,    83,   -68,
      84,    85,     0,     0,  -213,     0,    86,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
      99,     0,     0,   100,     0,   101,     0,   102,     0,  -213,
       0,   888,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,   293,     0,   196,     0,     0,     0,    31,
    -213,  -213,     0,     0,   197,    34,     0,     0,     0,   295,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
       0,     0,     0,     0,     0,     0,    69,   300,    71,    72,
      73,   301,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,     0,  -213,     0,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,   637,     0,    13,
       0,     0,    16,    17,   639,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   644,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,   200,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
     505,   460,   461,   462,   463,   464,     0,     0,   467,   468,
     469,   470,     0,   472,   473,     0,   826,   827,   828,   829,
     830,   657,     0,   658,     0,    94,     0,   659,   660,   661,
     662,   663,   664,   665,   831,   667,   668,    96,   832,    98,
     -92,   670,   671,   833,   673,   201,     0,   102,     0,     0,
       0,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,   637,     0,    13,     0,     0,    16,    17,   639,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   644,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,   505,   460,   461,   462,   463,
     464,     0,     0,   467,   468,   469,   470,     0,   472,   473,
       0,   826,   827,   828,   829,   830,   657,     0,   658,     0,
      94,     0,   659,   660,   661,   662,   663,   664,   665,   831,
     667,   668,    96,   832,    98,     0,   670,   671,   833,   673,
     201,     0,   102,     0,     0,     0,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,     0,    16,    17,     0,    18,     0,   195,    20,     0,
      21,     0,     0,     0,     0,    26,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,    33,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,    44,    45,    46,    47,
       0,    48,    49,     0,    50,    51,    52,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,    72,    73,    74,     0,    75,    76,
       0,    77,     0,     0,     0,    81,     0,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   101,     0,   102,     0,     0,
       0,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,   379,    23,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,    55,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
      64,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,     0,    83,     0,    84,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,     0,     0,     0,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,  -743,     0,     0,     0,  -743,     0,
       0,    69,    70,    71,    72,    73,   200,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,  -393,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
    -393,     0,     0,   100,     0,   201,  -393,   102,     0,     0,
       0,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,   315,   316,     0,
      81,   351,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,   352,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,     0,     0,     0,     0,   103,     5,     6,
       7,     8,     9,   382,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,    19,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,  -213,  -213,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,    63,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,   200,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,    82,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   201,     0,   102,     0,  -213,
       0,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,   740,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,   741,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
     742,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,     0,    83,     0,   743,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,     0,     0,     0,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,   200,     0,    75,     0,
       0,    77,   315,   316,     0,    81,   351,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   201,     0,   102,   802,     0,
       0,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,   315,   316,     0,
      81,   351,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,   804,     0,     0,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,   200,     0,    75,     0,
       0,    77,   315,   316,     0,    81,     0,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
     317,     0,     0,   100,     0,   201,     0,   102,     0,     0,
       0,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,   315,   316,     0,
      81,     0,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,  -544,     0,     0,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,  -213,  -213,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,   200,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   201,     0,   102,     0,  -213,
       0,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,   315,   316,     0,
      81,     0,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,     0,     0,  -544,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,   200,     0,    75,     0,
       0,    77,   315,   316,     0,    81,     0,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   201,     0,   102,   810,     0,
       0,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,   956,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,   315,   316,     0,
      81,     0,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,     0,     0,     0,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,   200,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,  -401,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
    -401,     0,     0,   100,     0,   201,  -401,   102,     0,     0,
       0,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,  -386,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,  -386,     0,     0,   100,     0,
     201,  -386,   102,     0,     0,     0,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,   200,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,  -394,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
    -394,     0,     0,   100,     0,   201,  -394,   102,     0,     0,
       0,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,   256,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,     0,    83,     0,   257,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,     0,     0,     0,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,   200,     0,    75,     0,
       0,    77,   315,   316,     0,    81,     0,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   201,     0,   102,     0,     0,
       0,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,  -743,
       0,     0,     0,  -743,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,     0,     0,     0,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,   600,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,   200,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,   601,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   201,     0,   102,     0,     0,
       0,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,   601,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,     0,     0,   610,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,   200,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,  -422,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   201,     0,   102,  -422,     0,
       0,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,   273,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     101,     0,   102,     0,     0,     0,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,   200,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,   601,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   201,     0,   102,     0,     0,
       0,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,     0,     0,   610,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,   200,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   201,   800,   102,     0,     0,
       0,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,     0,     0,     0,
      81,  1009,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,     0,     0,     0,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,   200,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   201,     0,   102,     0,     0,
    1046,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,     0,     0,  1051,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,   200,     0,    75,     0,
       0,    77,     0,     0,     0,    81,  1201,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   201,     0,   102,     0,     0,
       0,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,     0,     0,     0,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,   293,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,   295,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,     0,     0,     0,     0,     0,     0,
       0,    69,   300,    71,    72,    73,   301,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   201,     0,   102,     0,     0,
       0,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,  1075,    34,     0,     0,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,     0,     0,     0,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,  1078,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,   200,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   201,     0,   102,     0,     0,
       0,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,  1256,    34,     0,     0,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,     0,     0,     0,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,  1257,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,   200,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   201,     0,   102,     0,     0,
       0,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,  1259,    34,     0,     0,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,     0,     0,     0,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,  1260,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,   200,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   201,     0,   102,     0,     0,
       0,     0,   103,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,  1261,    34,     0,     0,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,     0,     0,     0,     0,   103,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,  1262,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     198,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   199,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,   200,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     6,     7,     8,     9,
      10,    11,    12,     0,    95,     0,    13,    96,    97,    98,
      17,     0,     0,   100,   195,   201,     0,   102,     0,   292,
       0,     0,   103,     0,     0,     0,     0,   196,     0,     0,
       0,     0,  -213,  -213,     0,     0,   197,     0,     0,   294,
       0,     0,     0,   296,     0,     0,     0,   198,     0,     0,
      42,    43,     0,     0,     0,     0,    47,     0,    48,    49,
       0,     0,     0,     0,     0,     0,     0,   297,     0,    54,
     298,    56,    57,    58,     0,     0,     0,     0,    61,     0,
     199,     0,   299,     0,     0,     0,     0,     0,    69,     0,
      71,     0,    73,     0,     0,     0,     0,     0,     0,     0,
       0,   302,     0,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   635,
       0,   636,     0,    89,    90,    91,    92,    93,     0,     0,
     637,     0,    94,     0,     0,   638,   230,   639,   640,     0,
       0,    95,     0,   641,    96,    97,    98,     0,     0,     0,
     100,   233,   201,   196,   102,     0,  -213,   234,     0,   103,
       0,     0,     0,   642,     0,     0,     0,   236,     0,     0,
     643,     0,   644,     0,     0,     0,     0,     0,     0,     0,
     645,     0,     0,     0,     0,     0,     0,   239,   646,     0,
       0,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   647,     0,     0,     0,     0,
       0,     0,     0,     0,   243,   244,     0,   648,     0,   246,
       0,   649,     0,     0,   248,     0,     0,     0,   650,     0,
       0,   250,     0,     0,   651,     0,     0,     0,     0,     0,
       0,     0,     0,   505,   460,   461,   462,   463,   464,     0,
       0,   467,   468,   469,   470,     0,   472,   473,     0,   652,
     653,   654,   655,   656,   657,     0,   658,     0,     0,     0,
     659,   660,   661,   662,   663,   664,   665,   666,   667,   668,
       0,   669,     0,     0,   670,   671,   672,   673,     0,     0,
     674,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,  -213,  -213,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,     0,   198,     0,     0,    42,    43,     0,     0,
      45,     0,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,    62,   199,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,    73,   200,
       0,    75,     0,     0,    77,     0,     0,     0,    81,     0,
       0,    83,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
       0,    97,    98,     0,     0,     0,   100,     0,   201,     0,
     102,     0,  -213,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
    -213,  -213,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,     0,   198,     0,     0,    42,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   199,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
      73,   200,     0,    75,     0,     0,    77,     0,   699,     0,
      81,     0,     0,    83,     0,     0,    85,   637,     0,     0,
       0,     0,   988,   989,   639,   990,     0,     0,     0,     0,
     991,     0,     0,     0,     0,     0,     0,     0,   701,     0,
       0,    89,     0,    91,     0,     0,     0,     0,     0,     0,
     992,     0,     0,     0,   993,     0,     0,   994,     0,   644,
       0,     0,     0,     0,    98,     0,     0,   702,     0,     0,
       0,     0,   102,     0,  -213,   995,     0,     0,     0,     0,
       0,     0,   996,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   997,     0,     0,     0,     0,     0,     0,     0,
       0,   998,   999,     0,  1000,     0,  1001,     0,   703,     0,
       0,     0,     0,     0,   704,  1002,     0,     0,  1003,     0,
       0,  1004,     0,     0,     0,     0,     0,     0,     0,     0,
     505,   460,   461,   462,   463,   464,     0,     0,   467,   468,
     469,   470,     0,   472,   473,     0,   652,   653,   654,   655,
     656,   657,     0,   658,     0,     0,     0,   659,   660,   661,
     662,   663,   664,   665,   666,   667,   668,   228,   669,     0,
       0,   670,   671,   672,   673,  1005,   637,     0,     0,     0,
       0,   229,   230,   639,   231,     0,     0,     0,     0,   232,
       0,     0,     0,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,   234,     0,     0,     0,     0,     0,   235,
       0,     0,     0,   236,     0,     0,   237,     0,   644,     0,
       0,     0,     0,     0,     0,     0,   238,     0,     0,     0,
       0,     0,     0,   239,   240,     0,     0,     0,     0,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   242,     0,     0,     0,     0,     0,   429,     0,     0,
     243,   244,     0,   245,   430,   246,     0,   247,     0,     0,
     248,     0,     0,     0,   249,     0,   431,   250,     0,     0,
     251,     0,     0,     0,     0,     0,     0,     0,     0,   505,
     460,   461,   462,   463,   464,     0,     0,   467,   468,   469,
     470,     0,   472,   473,     0,   652,   653,   654,   655,   656,
     657,     0,   658,     0,     0,     0,   659,   660,   661,   662,
     663,   664,   665,   666,   667,   668,     0,   669,     0,     0,
     670,   671,   672,   673,   432,     0,     0,   429,   433,     0,
       0,     0,     0,     0,   430,     0,     0,  1212,     0,     0,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
     505,   460,   461,   462,   463,   464,     0,     0,   467,   468,
     469,   470,     0,   472,   473,     0,   435,   436,     0,   437,
     438,   439,     0,   440,   441,   442,     0,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,     0,
       0,   455,   456,   457,   432,   364,     0,   429,   433,     0,
       0,     0,   458,     0,   430,     0,     0,  1219,     0,     0,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
     505,   460,   461,   462,   463,   464,     0,     0,   467,   468,
     469,   470,     0,   472,   473,     0,   435,   436,     0,   437,
     438,   439,     0,   440,   441,   442,     0,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,     0,
       0,   455,   456,   457,   432,   364,     0,   429,   433,     0,
       0,     0,   458,     0,   430,     0,     0,  1303,     0,     0,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
     505,   460,   461,   462,   463,   464,     0,     0,   467,   468,
     469,   470,     0,   472,   473,     0,   435,   436,     0,   437,
     438,   439,     0,   440,   441,   442,     0,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,     0,
       0,   455,   456,   457,   432,   364,     0,   429,   433,     0,
       0,     0,   458,     0,   430,     0,     0,  1304,     0,     0,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
     505,   460,   461,   462,   463,   464,     0,     0,   467,   468,
     469,   470,     0,   472,   473,     0,   435,   436,     0,   437,
     438,   439,     0,   440,   441,   442,     0,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,     0,
       0,   455,   456,   457,   432,   364,     0,   429,   433,     0,
       0,     0,   458,     0,   430,     0,     0,  1305,     0,     0,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
     505,   460,   461,   462,   463,   464,     0,     0,   467,   468,
     469,   470,     0,   472,   473,     0,   435,   436,     0,   437,
     438,   439,     0,   440,   441,   442,     0,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,     0,
       0,   455,   456,   457,   432,   364,     0,   429,   433,     0,
       0,     0,   458,     0,   430,     0,     0,  1306,     0,     0,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
     505,   460,   461,   462,   463,   464,     0,     0,   467,   468,
     469,   470,     0,   472,   473,     0,   435,   436,     0,   437,
     438,   439,     0,   440,   441,   442,     0,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,     0,
       0,   455,   456,   457,   432,   364,     0,   429,   433,     0,
       0,     0,   458,     0,   430,     0,     0,  1307,     0,     0,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
     505,   460,   461,   462,   463,   464,     0,     0,   467,   468,
     469,   470,     0,   472,   473,     0,   435,   436,     0,   437,
     438,   439,     0,   440,   441,   442,     0,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,     0,
       0,   455,   456,   457,   432,   364,     0,     0,   433,     0,
       0,     0,   458,     0,     0,     0,     0,  1308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,     0,   434,
     505,   460,   461,   462,   463,   464,   431,   273,   467,   468,
     469,   470,     0,   472,   473,     0,   435,   436,     0,   437,
     438,   439,   981,   440,   441,   442,     0,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,     0,
       0,   455,   456,   457,     0,   364,     0,     0,     0,     0,
       0,     0,   458,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   432,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
     822,     0,     0,     0,     0,     0,   430,     0,     0,     0,
       0,     0,     0,     0,     0,   854,     0,     0,   431,   434,
       0,     0,     0,   823,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   435,   436,     0,   437,
     438,   439,     0,   440,   441,   442,     0,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   824,
       0,   455,   456,   457,     0,   364,     0,     0,     0,     0,
       0,     0,   458,     0,     0,   429,   432,     0,     0,     0,
     433,     0,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   431,   851,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     852,   434,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   435,   436,
       0,   437,   438,   439,     0,   440,   441,   442,     0,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   429,   432,   455,   456,   457,   433,     0,   430,     0,
       0,     0,     0,     0,   458,     0,     0,     0,     0,     0,
     431,   273,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   980,   434,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   435,   436,     0,   437,   438,   439,
       0,   440,   441,   442,     0,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   429,   432,   455,
     456,   457,   433,   364,   430,     0,     0,     0,     0,     0,
     458,     0,     0,     0,     0,     0,   431,  1148,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   434,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     435,   436,     0,   437,   438,   439,     0,   440,   441,   442,
       0,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   429,   432,   455,   456,   457,   433,   364,
     430,     0,     0,     0,     0,     0,   458,     0,     0,     0,
       0,     0,   431,   273,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   854,     0,     0,     0,   434,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   435,   436,     0,   437,
     438,   439,     0,   440,   441,   442,     0,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   429,
     432,   455,   456,   457,   433,   364,   430,     0,     0,     0,
       0,     0,   458,     0,     0,     0,     0,     0,   431,   273,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   854,     0,     0,     0,   434,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   435,   436,     0,   437,   438,   439,     0,   440,
     441,   442,     0,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   429,   432,   455,   456,   457,
     433,   364,   430,     0,     0,     0,     0,     0,   458,     0,
       0,     0,     0,     0,   431,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     845,   434,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   435,   436,
       0,   437,   438,   439,     0,   440,   441,   442,     0,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   429,   432,   455,   456,   457,   433,   364,   430,     0,
       0,     0,     0,     0,   458,     0,     0,     0,     0,     0,
     431,   913,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   213,     0,     0,   914,   434,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   435,   436,     0,   437,   438,   439,
       0,   440,   441,   442,     0,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   429,   432,   455,
     456,   457,   433,     0,   430,     0,     0,     0,     0,     0,
     458,     0,     0,     0,     0,     0,   431,   915,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   916,   434,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     435,   436,     0,   437,   438,   439,     0,   440,   441,   442,
       0,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   429,   432,   455,   456,   457,   433,     0,
     430,     0,     0,     0,     0,     0,   458,     0,     0,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   435,   436,     0,   437,
     438,   439,     0,   440,   441,   442,     0,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,     0,
     432,   455,   456,   457,   433,     0,     0,     0,     0,     0,
       0,     0,   458,   947,     0,   429,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   431,   434,     0,     0,     0,  1121,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   435,   436,     0,   437,   438,   439,     0,   440,
     441,   442,     0,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,  1122,     0,   455,   456,   457,
       0,   364,     0,     0,     0,     0,     0,     0,   458,     0,
       0,   429,   432,     0,     0,     0,   433,     0,   430,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,  1140,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   434,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   435,   436,     0,   437,   438,   439,
       0,   440,   441,   442,     0,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   429,   432,   455,
     456,   457,   433,     0,   430,     0,     0,     0,     0,     0,
     458,     0,     0,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   434,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     435,   436,     0,   437,   438,   439,     0,   440,   441,   442,
       0,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   429,   432,   455,   456,   457,   433,   364,
     430,     0,     0,     0,     0,     0,   458,     0,     0,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   854,     0,     0,     0,   434,
       0,     0,     0,   429,   530,     0,     0,     0,     0,     0,
     430,     0,     0,     0,     0,     0,   435,   436,     0,   437,
     438,   439,   431,   440,   441,   442,     0,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,     0,
     432,   455,   456,   457,   433,     0,     0,     0,   429,     0,
    1158,     0,   458,     0,     0,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   431,     0,     0,
       0,   854,     0,     0,     0,   434,     0,     0,     0,     0,
     432,     0,     0,     0,   433,     0,     0,     0,     0,     0,
       0,     0,   435,   436,     0,   437,   438,   439,     0,   440,
     441,   442,     0,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   434,     0,   455,   456,   457,
       0,     0,     0,     0,     0,   432,  1199,     0,   458,   433,
       0,     0,   435,   436,     0,   437,   438,   439,     0,   440,
     441,   442,     0,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   429,     0,   455,   456,   457,
     434,     0,   430,     0,     0,     0,     0,     0,   458,     0,
       0,     0,     0,     0,   431,     0,     0,   435,   436,     0,
     437,   438,   439,     0,   440,   441,   442,     0,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     429,     0,   455,   456,   457,     0,   554,   430,     0,     0,
       0,     0,     0,   458,     0,     0,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   432,     0,     0,     0,   433,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,     0,   434,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,     0,     0,
       0,   433,     0,     0,   435,   436,     0,   437,   438,   439,
     735,   440,   441,   442,     0,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,     0,   558,   455,
     456,   457,   434,     0,     0,     0,     0,   432,     0,     0,
     458,   433,     0,     0,     0,     0,     0,     0,     0,   435,
     436,     0,   437,   438,   439,     0,   440,   441,   442,     0,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   434,     0,   455,   456,   457,     0,     0,     0,
     429,   818,     0,     0,     0,   458,     0,   430,     0,   435,
     436,     0,   437,   438,   439,     0,   440,   441,   442,   431,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,     0,     0,   455,   456,   457,     0,     0,     0,
       0,   801,     0,     0,     0,   458,   429,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,     0,     0,
       0,   433,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   431,     0,     0,   945,     0,
       0,     0,   434,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   432,     0,     0,     0,   433,     0,   435,
     436,     0,   437,   438,   439,     0,   440,   819,   442,     0,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,     0,     0,   455,   456,   457,   875,   434,     0,
       0,     0,   429,   432,     0,   458,     0,   433,     0,   430,
       0,     0,     0,     0,     0,   435,   436,     0,   437,   438,
     439,   431,   440,   441,   442,     0,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   434,     0,
     455,   456,   457,     0,     0,     0,     0,     0,     0,     0,
       0,   458,     0,     0,     0,   435,   436,     0,   437,   438,
     439,     0,   440,   441,   442,     0,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   429,   432,
     455,   456,   457,   433,     0,   430,     0,     0,     0,     0,
       0,   458,     0,     0,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,     0,     0,     0,   429,  1015,
       0,     0,     0,     0,     0,   430,     0,     0,     0,     0,
       0,   435,   436,     0,   437,   438,   439,   431,   440,   441,
     442,     0,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,     0,   432,   455,   456,   457,   433,
       0,     0,     0,   959,   429,     0,     0,   458,     0,     0,
       0,   430,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
     434,     0,     0,     0,     0,   432,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,   435,   436,     0,
     437,   438,   439,     0,   440,   441,   442,     0,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     434,     0,   455,   456,   457,     0,     0,     0,     0,   960,
       0,   432,     0,   458,     0,   433,     0,   435,   436,     0,
     437,   438,   439,     0,   440,   441,   442,     0,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     429,  1118,   455,   456,   457,     0,   434,   430,     0,     0,
       0,     0,     0,   458,     0,     0,     0,     0,     0,   431,
       0,     0,     0,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,     0,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,     0,     0,   455,   456,
     457,     0,     0,     0,   429,  1070,     0,     0,     0,   458,
       0,   430,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   431,     0,     0,     0,   432,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,   430,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,   431,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   435,
     436,   432,   437,   438,   439,   433,   440,   441,   442,     0,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,     0,     0,   455,   456,   457,     0,     0,     0,
       0,     0,   213,     0,     0,   458,   434,     0,     0,     0,
       0,   432,     0,     0,   429,   433,     0,     0,     0,     0,
       0,   430,     0,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,   431,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   434,     0,   455,   456,
     457,     0,     0,     0,     0,     0,     0,     0,     0,   458,
       0,     0,     0,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,   431,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,     0,  1134,   455,   456,
     457,   432,     0,     0,   429,   433,     0,     0,     0,   458,
       0,   430,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   431,  1181,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1161,   434,     0,     0,     0,
       0,   432,     0,     0,   429,   433,     0,     0,     0,     0,
       0,   430,     0,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,   431,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,     0,     0,   455,   456,
     457,   432,     0,     0,   429,   433,     0,     0,     0,   458,
       0,   430,     0,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,   431,  1184,   444,   445,   446,     0,   448,
     449,   450,   451,     0,     0,   454,   434,     0,   455,   456,
     457,   432,     0,     0,     0,   433,     0,     0,     0,   458,
       0,     0,     0,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,     0,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   434,     0,   455,   456,
     457,   432,     0,     0,   429,   433,     0,     0,     0,   458,
       0,   430,     0,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,   431,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   434,     0,   455,   456,
     457,     0,     0,     0,   429,  1207,     0,  1186,     0,   458,
       0,   430,  1248,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,   431,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,     0,     0,   455,   456,
     457,   432,     0,     0,     0,   433,     0,     0,     0,   458,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   434,     0,     0,     0,
     429,   432,     0,     0,     0,   433,     0,   430,     0,     0,
       0,     0,     0,   435,   436,     0,   437,   438,   439,   431,
     440,   441,   442,     0,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   434,     0,   455,   456,
     457,     0,     0,     0,     0,     0,     0,     0,     0,   458,
       0,     0,     0,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,     0,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   429,   432,   455,   456,
     457,   433,     0,   430,     0,     0,     0,     0,     0,   458,
    1255,     0,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   435,
     436,     0,   437,   438,   439,     0,   440,   441,   442,     0,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   429,   432,   455,   456,   457,   433,     0,   430,
       0,     0,     0,     0,     0,   458,  1258,     0,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   435,   436,     0,   437,   438,
     439,     0,   440,   441,   442,     0,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   429,   432,
     455,   456,   457,   433,     0,   430,     0,     0,     0,     0,
       0,   458,  1309,     0,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   435,   436,     0,   437,   438,   439,     0,   440,   441,
     442,     0,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   429,   432,   455,   456,   457,   433,
       0,   430,     0,     0,     0,     0,     0,   458,  1310,     0,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   435,   436,     0,
     437,   438,   439,     0,   440,   441,   442,     0,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     429,   432,   455,   456,   457,   433,     0,   430,     0,     0,
       0,     0,     0,   458,  1311,     0,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   434,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,     0,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   429,   432,   455,   456,
     457,   433,     0,   430,     0,     0,     0,     0,     0,   458,
    1312,     0,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   435,
     436,     0,   437,   438,   439,     0,   440,   441,   442,     0,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   429,   432,   455,   456,   457,   433,     0,   430,
       0,     0,     0,     0,     0,   458,  1313,     0,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,   435,   436,     0,   437,   438,
     439,   431,   440,   441,   442,     0,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,     0,   432,
     455,   456,   457,   433,     0,     0,     0,   429,     0,     0,
       0,   458,  1314,     0,   430,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,   434,     0,     0,     0,     0,   432,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,   435,   436,     0,   437,   438,   439,     0,   440,   441,
     442,     0,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   434,     0,   455,   456,   457,     0,
       0,     0,     0,     0,   935,     0,     0,   458,   433,     0,
       0,   435,   436,     0,   437,   438,   439,     0,   440,   441,
     442,     0,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,     0,     0,   455,   456,   457,   434,
       0,     0,     0,     0,     0,     0,     0,   458,     0,     0,
       0,     0,     0,     0,     0,     0,   435,   436,     0,   437,
     438,   439,     0,   440,   441,   442,   431,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,     0,
     359,   455,   456,   457,    22,    23,     0,     0,     0,     0,
       0,     0,   458,   360,     0,    29,   361,     0,     0,     0,
       0,    32,     0,     0,     0,     0,     0,     0,    37,     0,
       0,  -351,     0,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   -68,   432,     0,     0,     0,   433,     0,
       0,  -351,     0,     0,    53,     0,    55,     0,     0,     0,
      59,  -351,   -68,    60,     0,     0,   363,     0,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,   435,   436,     0,   437,
     438,   439,     0,   440,   441,   442,     0,   443,   444,   445,
     446,     0,   448,   449,   450,   451,   452,   400,   454,   228,
       0,   455,   456,   457,     0,     0,     0,     0,     0,     0,
       0,     0,   458,   229,   230,     0,   231,     0,     0,     0,
       0,   232,     0,    99,     0,     0,     0,     0,   364,   233,
       0,     0,     0,     0,     0,   234,     0,     0,     0,     0,
       0,   235,     0,     0,     0,   236,     0,     0,   237,     0,
       0,     0,     0,     0,   228,     0,     0,     0,   238,     0,
       0,     0,     0,     0,     0,   239,   240,     0,   229,   230,
       0,   231,     0,   241,     0,     0,   232,     0,     0,     0,
       0,     0,     0,   242,   233,     0,     0,     0,     0,     0,
     234,     0,   243,   244,     0,   245,   235,   246,     0,   247,
     236,     0,   248,   237,     0,     0,   249,     0,     0,   250,
       0,     0,   251,   238,     0,     0,     0,     0,     0,     0,
     239,   240,     0,     0,     0,     0,     0,     0,   241,   228,
       0,     0,     0,     0,     0,     0,    88,     0,   242,     0,
       0,     0,     0,   229,   230,     0,   231,   243,   244,     0,
     245,   232,   246,    23,   247,     0,     0,   248,     0,   233,
       0,   249,     0,     0,   250,   234,     0,   251,     0,     0,
       0,   235,     0,     0,     0,   236,     0,     0,   237,     0,
       0,     0,     0,     0,   228,     0,     0,     0,   238,     0,
       0,    88,     0,     0,     0,   239,   240,     0,   229,   230,
       0,   231,     0,   241,    55,     0,   232,     0,     0,     0,
       0,     0,     0,   242,   233,     0,    64,     0,     0,     0,
     234,     0,   243,   244,     0,   245,   235,   246,     0,   247,
     236,     0,   248,   237,     0,     0,   249,     0,     0,   250,
       0,    84,   251,   238,     0,     0,     0,     0,     0,     0,
     239,   240,     0,     0,     0,     0,     0,     0,   241,  -326,
       0,     0,     0,     0,     0,     0,     0,     0,   242,     0,
       0,     0,     0,  -326,  -326,     0,  -326,   243,   244,     0,
     245,  -326,   246,     0,   247,     0,     0,   248,     0,  -326,
       0,   249,   543,     0,   250,  -326,     0,   251,     0,     0,
       0,  -326,     0,     0,     0,  -326,     0,     0,  -326,     0,
       0,     0,     0,     0,   228,     0,     0,     0,  -326,     0,
       0,     0,     0,     0,     0,  -326,  -326,     0,   229,   230,
       0,   231,     0,  -326,     0,     0,   232,     0,     0,     0,
       0,     0,     0,  -326,   233,     0,     0,     0,     0,     0,
     234,     0,  -326,  -326,     0,  -326,   235,  -326,     0,  -326,
     236,     0,  -326,   237,     0,     0,  -326,     0,     0,  -326,
       0,     0,  -326,   238,     0,     0,     0,     0,     0,     0,
     239,   240,     0,     0,     0,     0,     0,     0,   241,  -327,
       0,     0,     0,     0,     0,     0,     0,     0,   242,     0,
       0,     0,     0,  -327,  -327,     0,  -327,   243,   244,     0,
     245,  -327,   246,     0,   247,     0,     0,   248,     0,  -327,
       0,   249,     0,     0,   250,  -327,     0,   251,     0,     0,
       0,  -327,     0,     0,     0,  -327,     0,     0,  -327,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -327,     0,
       0,     0,     0,     0,     0,  -327,  -327,     0,     0,     0,
       0,     0,     0,  -327,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -327,     0,     0,     0,     0,     0,     0,
       0,     0,  -327,  -327,     0,  -327,     0,  -327,     0,  -327,
       0,     0,  -327,     0,     0,     0,  -327,     0,     0,  -327,
       0,     0,  -327
};

static const yytype_int16 yycheck[] =
{
       2,     2,    20,   152,   263,   486,   165,    61,   111,     2,
     401,   552,   590,   227,    44,    66,    26,   181,   604,   421,
     606,   607,    61,    25,    61,   611,   612,   556,   221,   631,
     289,   677,    25,   679,   510,    86,   921,   824,    68,   628,
     790,   132,   881,   547,     2,   819,   784,     1,   383,   263,
     579,   102,     8,   842,   961,    65,    32,    40,    41,     1,
     522,     1,    32,    32,    32,    32,    32,    25,     3,    60,
     329,    32,    74,   406,   396,   289,    78,    79,    32,    74,
       1,    74,    55,   290,   291,    78,    79,    47,    47,   887,
       1,   389,   390,    32,   392,   259,     0,    35,    47,   101,
      22,    24,    46,   105,  1175,   102,    60,   120,   101,    32,
      47,    32,   105,    47,    55,   329,    74,    40,    41,    42,
      78,    79,   117,    55,   147,    92,    92,   118,   147,   165,
       3,    47,    74,   120,   432,   433,    47,    24,    92,    60,
      84,    90,    24,   101,   120,    32,  1031,   105,   171,     3,
      32,   105,   120,    40,    41,    42,   175,    92,    40,    41,
      42,     3,   175,   109,   118,   963,    88,  1238,   103,   631,
     114,    92,   110,   170,    90,   117,   171,   147,   147,  1018,
     478,   479,   771,   120,   105,   125,   120,   147,   147,   511,
     112,   174,   146,   149,   170,  1102,   119,   118,   165,   172,
     170,   170,   170,   170,   170,   955,   146,   147,   541,   170,
     147,   961,   214,   147,   125,   175,   170,   476,   477,  1058,
    1059,   214,   808,    27,   753,   146,   964,   441,  1017,   171,
     103,   170,   119,   173,    18,   146,   147,   119,   175,  1013,
     172,   175,   120,  1169,   165,   823,   276,    36,   278,   170,
     120,   174,   123,    55,   165,   285,   214,   421,   593,   118,
     141,   103,   476,   477,     3,    89,   268,   147,   149,    93,
      55,   273,   140,   517,    78,   268,   125,   575,   576,   147,
     273,    20,    31,    72,   788,   165,    90,   174,   147,  1128,
      46,   382,   174,    77,    33,   325,   510,   175,   401,   170,
     544,   147,   151,   171,    27,   175,    27,    66,    90,    65,
     268,   115,    96,   147,   147,   273,  1242,  1243,  1244,  1245,
    1246,  1247,    55,    62,  1163,   171,    47,    86,    84,   113,
      51,   490,  1217,  1120,  1084,  1122,   172,   530,   171,   173,
      89,    11,   556,   102,    93,   934,    55,   349,   349,   147,
      50,   405,  1102,    53,    75,    78,   349,    78,   114,    80,
     962,   172,   364,   174,   103,   579,   147,    90,   165,    90,
     109,   364,   374,    73,   171,   173,    76,   147,   125,    46,
      27,   374,   392,    83,   165,   387,   109,   397,   109,    78,
      79,   349,   141,   165,   387,   165,   147,   146,    65,   171,
     149,    55,   878,   596,   153,   152,   364,   154,   155,   109,
     157,   158,    27,    27,   465,   466,   374,    84,    53,   421,
     471,     2,   173,   474,   475,  1071,  1072,  1005,    75,   387,
     147,    78,    47,    47,   172,   165,    51,    51,    73,     8,
     921,   147,   172,    90,    25,   147,   147,   114,    83,  1035,
    1036,   147,   173,  1039,  1040,   984,   173,   147,  1044,  1045,
      75,    75,   109,    78,    78,    80,    80,   173,   227,    55,
      55,   173,   173,   147,   484,    90,    90,   173,    31,    78,
      79,   147,   147,   173,   147,    89,   147,   147,   542,    93,
      55,   172,   883,    74,   109,   109,   898,    78,    79,   173,
     502,    55,   504,   542,   263,   542,    55,   173,   173,   502,
     173,   504,   173,   173,   147,   147,   147,   568,    89,   570,
     101,    89,    93,   100,   105,    93,  1112,  1113,  1114,  1115,
     289,   147,   165,   165,   165,   147,    89,   147,   147,   753,
      93,  1119,    82,  1121,   502,    85,   504,   845,    55,   165,
    1031,   147,   165,   165,   852,   165,   165,    55,   173,   172,
     858,   174,    55,     2,    89,   863,   576,   147,    93,   165,
     329,   172,   602,   871,   604,   605,   606,   607,   608,   609,
     598,   583,   583,   147,   147,   165,    25,    55,   141,   142,
     583,   144,   145,   146,    89,   148,   149,   150,    93,    55,
     153,   165,   165,     3,   818,   819,   159,   160,   140,   170,
     163,    31,    27,   166,   167,   168,   914,    55,   916,   917,
     172,   172,   174,   174,   177,   583,   172,   170,   174,   172,
     146,   172,    47,   214,  1175,    74,    51,   935,   936,    78,
      79,   165,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   172,   164,   174,
      75,    55,   101,    78,   878,    80,   105,   172,   172,    89,
     174,  1062,   172,    93,   174,    90,   568,   165,   570,   165,
     165,   165,   980,   981,   982,   844,   169,   268,   165,   172,
     141,   174,   273,   165,   109,   146,   165,  1238,   149,   170,
     751,   165,   153,   170,    84,   147,   465,   466,   173,   172,
     172,   172,   471,   146,   172,   474,   475,   476,   477,   165,
     165,   141,   142,   774,   144,   145,   146,     8,   148,   149,
       3,   172,   881,   153,   898,   884,  1217,    65,   170,   159,
     160,   171,   125,   163,   746,    90,   166,   167,   168,    90,
     125,   510,   149,    68,   125,   757,   173,   147,   760,   125,
     125,    65,    34,    34,   757,   173,   125,   760,   349,   173,
     984,   173,   146,    21,   170,   214,   149,   807,   165,   119,
     172,   105,   149,   364,   147,    31,   172,   149,   149,   149,
     175,   149,   149,   374,   149,   149,   149,   556,   149,   757,
     149,   149,   760,   149,   141,   149,   387,    34,   172,   568,
      34,   570,   146,   165,   147,   165,   846,   173,   165,    47,
     579,    34,   165,   165,    32,    32,   856,   881,   146,   268,
     860,   885,   862,   165,   273,   865,   171,  1096,  1097,   849,
     894,   170,   173,    89,   165,   172,   172,    93,   165,   851,
     175,   171,   175,   175,   173,   857,   172,   165,   851,   147,
     173,   175,    34,   173,   857,   165,   175,   869,   175,  1018,
     173,   873,   173,    22,   165,   164,   869,    26,    27,   164,
     873,   147,  1096,  1097,   110,   578,    35,   984,    37,    38,
     262,   816,   374,   851,    43,   141,    24,   391,   951,   857,
     146,    50,   148,   149,   976,   528,    55,   153,   337,  1058,
     349,   869,   366,   542,   160,   873,   898,   894,   539,   385,
     885,   502,   168,   504,   303,   364,  1238,    76,  1064,    78,
    1248,  1241,   130,   935,   936,   374,   843,   683,   525,    88,
     942,    90,   944,   770,    -1,   947,    -1,    -1,   387,   942,
      -1,   944,    -1,    -1,   947,    -1,  1059,    -1,    -1,  1062,
     109,    -1,   972,   112,  1018,    -1,   115,    -1,    -1,   971,
      -1,    -1,    -1,  1137,    -1,    -1,    -1,    -1,   971,  1128,
    1144,   983,    -1,    -1,   942,    -1,   944,    -1,    -1,   947,
     983,    -1,   751,    -1,   753,    -1,    -1,    -1,    -1,  1029,
    1030,    -1,   583,  1021,  1022,  1035,  1036,    -1,    -1,  1039,
    1040,    -1,    -1,   971,    -1,   774,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   983,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1043,    -1,    -1,    -1,    -1,    -1,  1049,    -1,    -1,
    1043,    -1,    -1,    -1,     2,    -1,  1049,    -1,    -1,    -1,
    1163,    -1,    -1,   502,    -1,   504,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1128,    -1,  1106,    25,    -1,    -1,
    1110,  1111,  1112,  1113,    -1,  1043,    -1,    -1,    -1,  1091,
      -1,  1049,    -1,    -1,    -1,    -1,    -1,    -1,  1091,    -1,
      -1,    -1,  1132,  1133,    -1,  1135,    -1,    -1,    -1,    -1,
    1274,  1275,    -1,    -1,  1278,  1279,  1280,  1147,  1282,   878,
    1150,    -1,  1152,    -1,  1154,    -1,    74,    -1,    -1,    -1,
      78,    79,    -1,  1091,    -1,    -1,  1138,    -1,  1140,    -1,
      -1,    -1,    -1,  1145,   583,  1138,  1148,  1140,    -1,    -1,
      31,    -1,  1145,   101,  1156,  1148,  1158,   105,    -1,    -1,
      -1,    -1,    -1,  1156,    -1,  1158,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   757,    -1,    -1,   760,
    1138,     2,  1140,    -1,    -1,    -1,  1188,  1145,    -1,    -1,
    1148,    -1,  1194,    -1,    -1,  1188,  1198,  1199,  1156,    -1,
    1158,  1194,    -1,    -1,    25,  1198,  1199,    -1,    89,    -1,
    1212,    -1,    93,    -1,    -1,    -1,    -1,  1219,    -1,  1212,
      -1,    -1,    -1,    -1,    -1,   984,  1219,    -1,    -1,  1231,
    1188,    -1,    -1,  1235,    -1,    -1,  1194,    -1,  1231,    -1,
    1198,  1199,  1235,    -1,    -1,    -1,    -1,  1249,  1249,    -1,
      -1,    -1,    -1,    74,  1212,    -1,  1249,    78,    79,    -1,
     141,  1219,    -1,    -1,    -1,   146,   214,   148,   149,    -1,
     851,    -1,   153,  1231,    31,    -1,   857,  1235,    -1,   160,
     101,    -1,    -1,    -1,   105,   166,   167,   168,   869,    -1,
      -1,  1249,   873,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1303,  1304,  1305,  1306,  1307,  1308,    -1,    -1,    -1,
    1303,  1304,  1305,  1306,  1307,  1308,    -1,    -1,   757,    -1,
     268,   760,    -1,    -1,    -1,   273,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,  1096,  1097,    -1,
      -1,    -1,    -1,    -1,    -1,  1303,  1304,  1305,  1306,  1307,
    1308,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   942,    31,   944,    -1,    -1,   947,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
     971,   148,   149,   214,    -1,    -1,   153,    -1,    -1,    -1,
      -1,   349,   983,   160,    -1,    -1,     1,    -1,    -1,   166,
     167,   168,   851,    -1,    -1,    -1,   364,    -1,   857,    -1,
      89,    -1,    -1,    -1,    93,    -1,   374,    22,    -1,    -1,
     869,    26,    27,    -1,   873,    -1,    -1,    -1,    -1,   387,
      35,    -1,    37,    38,    -1,    -1,    -1,   268,    43,    -1,
      -1,    -1,   273,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,  1043,    31,    -1,    -1,    -1,    -1,  1049,    -1,
      -1,    -1,   141,   142,    -1,    -1,   145,   146,    73,   148,
     149,    76,    -1,    78,   153,    80,    -1,    82,    83,    -1,
      85,   160,    -1,    88,    -1,    90,    -1,   166,   167,   168,
      -1,    -1,    -1,   942,    -1,   944,    -1,    -1,   947,    -1,
    1091,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    89,    -1,    -1,    -1,    93,    -1,    -1,   349,    -1,
      -1,    -1,   971,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   364,   983,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   374,   502,    -1,   504,  1138,    -1,  1140,
       2,    -1,    -1,    -1,  1145,    -1,   387,  1148,    -1,    -1,
     165,    -1,    -1,   141,   142,  1156,   171,  1158,   146,    -1,
     148,   149,    -1,    25,    -1,   153,    -1,    -1,     2,    -1,
      -1,    -1,   160,    -1,    -1,    -1,    -1,    -1,   166,   167,
     168,    -1,    -1,    -1,  1043,    -1,    -1,  1188,    -1,    -1,
    1049,    25,    -1,  1194,    -1,    -1,    -1,  1198,  1199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1212,    74,    -1,    -1,   583,    78,    79,  1219,    -1,
      -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
    1231,    -1,  1091,    -1,  1235,    -1,    -1,    -1,    17,   101,
      74,    20,    -1,   105,    78,    79,    25,    -1,  1249,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,
      -1,   502,    -1,   504,    -1,    -1,    45,   101,    -1,    -1,
      -1,   105,    -1,    52,    -1,    -1,    -1,    -1,    -1,  1138,
      -1,  1140,    -1,    62,    -1,    -1,  1145,    -1,    -1,  1148,
      -1,    70,    -1,    -1,    -1,    -1,    -1,  1156,    -1,  1158,
      -1,    -1,  1303,  1304,  1305,  1306,  1307,  1308,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,  1188,
      -1,   110,    -1,    -1,    -1,  1194,    -1,   116,    -1,  1198,
    1199,    -1,   583,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,  1212,    -1,    -1,    -1,    -1,    -1,    -1,
    1219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1231,    -1,    -1,    -1,  1235,    -1,     2,    -1,
     214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   757,
    1249,    -1,   760,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,   268,    -1,    -1,    -1,
      -1,   273,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   268,    -1,    -1,    -1,    -1,   273,
      -1,    -1,    -1,    -1,  1303,  1304,  1305,  1306,  1307,  1308,
      74,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,
      -1,    20,    -1,    -1,    -1,    -1,    25,   101,    -1,    -1,
      -1,   105,    -1,   851,    33,    -1,    -1,   349,    -1,   857,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,   869,   364,    52,    -1,   873,    -1,    -1,    -1,    -1,
      -1,    -1,   374,    62,    -1,   349,   757,    -1,    -1,   760,
      69,    70,    -1,    -1,    -1,   387,    -1,    -1,    77,    -1,
     364,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
     374,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      99,    -1,   101,   387,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    22,   942,    -1,   944,    26,    27,   947,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    37,    38,
     214,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,
      -1,    50,    -1,   971,    -1,    -1,    55,    -1,    -1,    -1,
     851,    -1,    -1,    -1,    -1,   983,   857,    -1,    -1,    -1,
      -1,    -1,    -1,   172,    -1,    -1,    -1,    76,   869,    78,
      -1,    80,   873,    82,    -1,    -1,    85,    -1,    -1,    88,
     502,    90,   504,    -1,   268,    -1,    -1,    -1,    -1,   273,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,   502,    -1,
     504,    -1,    -1,    -1,    -1,  1043,    -1,    -1,    -1,    -1,
      -1,  1049,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   942,    -1,   944,    -1,    -1,   947,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
      -1,   583,   171,  1091,    -1,   349,    -1,    -1,    -1,    -1,
     971,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     364,    -1,   983,    -1,    -1,     3,    -1,    -1,    -1,   583,
     374,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,   387,    -1,    -1,    -1,    25,    -1,    -1,
    1138,    -1,  1140,    -1,    -1,    33,    -1,  1145,    -1,    -1,
    1148,    39,    -1,    -1,    -1,    -1,    -1,    45,  1156,    -1,
    1158,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1043,    -1,    62,    -1,    -1,    -1,  1049,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
    1188,    -1,    -1,    -1,    -1,    -1,  1194,    -1,    -1,    87,
    1198,  1199,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    99,    -1,   101,  1212,   103,    -1,    -1,   106,    -1,
    1091,  1219,   110,   111,    -1,   113,    22,    -1,   116,    -1,
      26,    27,    -1,  1231,    -1,    -1,    -1,  1235,    -1,    35,
      -1,    37,    38,    -1,    -1,    -1,    -1,    43,   502,    -1,
     504,  1249,    -1,    -1,    50,    -1,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,   757,    -1,  1138,   760,  1140,
      -1,    -1,    -1,    -1,  1145,    -1,    -1,  1148,    -1,    -1,
      76,    -1,    78,    -1,   172,  1156,    82,  1158,    -1,    85,
      -1,    -1,    88,   757,    90,    -1,   760,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1303,  1304,  1305,  1306,  1307,
    1308,    -1,    -1,   109,    -1,    -1,   112,  1188,    -1,   115,
      -1,    -1,    -1,  1194,    -1,    -1,    -1,  1198,  1199,   583,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1212,    -1,    -1,   140,    -1,    -1,    -1,  1219,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   851,
    1231,    -1,    -1,    -1,  1235,   857,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   869,  1249,    -1,
      -1,   873,    -1,    -1,    -1,    -1,    -1,   851,    -1,    -1,
      -1,    -1,    -1,   857,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   869,    -1,    -1,    -1,   873,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,
      -1,    -1,  1303,  1304,  1305,  1306,  1307,  1308,    -1,    -1,
      -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,
     942,    -1,   944,    -1,    -1,   947,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,   942,   971,
     944,    -1,    -1,   947,    -1,    -1,    62,    -1,    -1,    -1,
      -1,   983,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,   757,    -1,    -1,   760,   971,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   983,
      96,    97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1043,    -1,    -1,    22,    -1,    -1,  1049,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    37,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,  1043,
      -1,    -1,    50,    -1,    -1,  1049,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,   170,    -1,   172,   851,    -1,  1091,
      -1,    -1,    -1,   857,    -1,    -1,    -1,    -1,    76,    -1,
      78,    -1,    80,    -1,    82,   869,    -1,    85,    -1,   873,
      88,    -1,    90,    -1,    -1,    -1,    -1,  1091,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,   109,    -1,    -1,   112,    -1,  1138,   115,  1140,    -1,
      -1,    31,    32,  1145,    -1,    -1,  1148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1156,    -1,  1158,    47,    -1,    -1,
      -1,    -1,   140,    -1,  1138,    -1,  1140,    -1,    -1,    -1,
      -1,  1145,    -1,    -1,  1148,    -1,    -1,    -1,   942,    -1,
     944,    -1,  1156,   947,  1158,    -1,  1188,   165,    -1,    -1,
      -1,    -1,  1194,   171,    -1,    -1,  1198,  1199,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,   971,    -1,    -1,
    1212,    -1,    -1,    -1,  1188,    -1,    -1,  1219,    -1,   983,
    1194,    -1,    -1,    -1,  1198,  1199,    -1,    -1,    -1,  1231,
     120,    -1,    -1,  1235,   124,    -1,    -1,    -1,  1212,    -1,
      -1,    -1,    -1,    -1,    -1,  1219,    -1,  1249,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,  1231,   148,   149,
     150,  1235,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,  1249,   166,   167,   168,  1043,
     170,    -1,    -1,    -1,    -1,  1049,     2,   177,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1303,  1304,  1305,  1306,  1307,  1308,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1091,    -1,  1303,
    1304,  1305,  1306,  1307,  1308,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1138,    -1,  1140,    -1,    -1,    -1,
      -1,  1145,    -1,   124,  1148,   101,    -1,    -1,    -1,   105,
      -1,    -1,  1156,    -1,  1158,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    -1,  1188,   166,   167,   168,    -1,    -1,
    1194,    -1,    -1,    -1,  1198,  1199,   177,    -1,    -1,    13,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,  1212,    -1,
      -1,    -1,    26,    -1,    -1,  1219,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1231,    -1,    -1,
      44,  1235,    46,    47,    -1,    -1,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,  1249,    -1,    61,    -1,    -1,
      -1,    65,    66,    67,    68,    69,    70,    -1,   214,    -1,
      74,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      -1,    -1,    86,    -1,    -1,    89,    -1,    91,    -1,    -1,
      94,    95,    -1,    97,    -1,    -1,    -1,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1303,
    1304,  1305,  1306,  1307,  1308,    -1,    -1,     3,    -1,    -1,
      -1,    -1,   268,    -1,    -1,    -1,   130,   273,   132,    -1,
      -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,
     174,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,   197,    -1,    -1,   200,   201,    -1,    -1,
      -1,    87,    -1,   349,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    -1,    99,    -1,   101,    -1,   103,   364,    -1,
     106,    -1,    -1,   227,   110,    -1,    -1,   113,   374,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   387,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,   263,
      35,    -1,    37,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,   289,   172,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   300,   301,    -1,    -1,
      -1,    76,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      85,   315,   316,    88,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   329,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   352,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   502,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,   375,    -1,   377,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   386,    -1,   388,   389,   390,   391,   392,    -1,
     165,    -1,    -1,   397,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   429,   430,   431,   432,   433,
     434,    -1,   436,   437,   438,   439,   440,   583,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,    -1,    -1,    -1,    -1,    -1,
      -1,   465,   466,    -1,    -1,    -1,    -1,   471,    -1,    -1,
     474,   475,   476,   477,   478,   479,    -1,    -1,   482,   483,
     484,    -1,   486,    -1,    -1,    -1,    -1,    -1,   492,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     504,    -1,    -1,   507,    -1,    -1,   510,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,   522,    -1,
     524,    -1,    -1,    -1,   528,    -1,    31,   531,   532,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,   542,    -1,
      -1,    -1,    -1,   547,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   556,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   565,    -1,    -1,   568,    -1,   570,    72,    44,    -1,
     574,   575,   576,    -1,    -1,   579,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    89,    -1,   590,   591,    93,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    -1,    -1,   610,    -1,    -1,    -1,
      -1,   757,    -1,    -1,   760,    -1,    -1,   621,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   631,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,   141,   142,    89,   144,
     145,   146,    93,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
     165,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,
     674,    -1,   177,   124,    -1,    -1,    -1,   681,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   851,   157,   158,   159,   160,
     161,   857,   163,    -1,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,   869,    -1,    -1,   177,   873,    -1,    -1,
      -1,   735,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   219,    -1,    -1,    -1,   751,    -1,   753,
     754,   755,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     774,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   258,   259,   788,    -1,    -1,    -1,   792,    -1,
      -1,   267,    -1,    -1,    -1,    -1,   942,    -1,   944,    -1,
     276,   947,   278,    -1,    -1,    -1,    -1,    -1,    -1,   285,
      -1,    -1,   816,    -1,    -1,    -1,   820,    -1,   822,   823,
     824,    -1,   826,    -1,   828,   971,    -1,   831,   832,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   983,    -1,    -1,
      -1,   845,    -1,   847,    -1,   849,    -1,   851,   852,   325,
      -1,    -1,    -1,   857,   858,    -1,    -1,    -1,    -1,   863,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   871,   872,   873,
      -1,   875,   876,    -1,   878,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1043,    -1,    -1,
      -1,    -1,    -1,  1049,    -1,    -1,    -1,    -1,    -1,   913,
     914,   915,   916,   917,   918,    -1,    -1,   921,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   935,   936,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   945,    -1,    -1,    -1,  1091,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   962,    -1,
      -1,    -1,   966,    -1,    -1,    -1,    -1,    -1,   972,    -1,
      22,    -1,    -1,    -1,    26,    27,   980,   981,   982,    -1,
     984,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,
      -1,    43,  1138,    -1,  1140,    -1,    -1,    -1,    50,  1145,
      -1,  1005,  1148,    55,    -1,    -1,    -1,    -1,    -1,  1013,
    1156,  1015,  1158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1025,    -1,  1027,    76,    -1,    78,  1031,    80,  1033,
      82,    -1,    -1,    85,    -1,    -1,    88,    -1,    90,    -1,
      -1,    -1,  1188,    -1,    -1,    -1,    -1,    -1,  1194,    -1,
      -1,    -1,  1198,  1199,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,  1066,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1075,    -1,    -1,  1078,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1231,    -1,    -1,   140,  1235,
      -1,    -1,  1096,  1097,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1249,  1108,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,   165,  1118,  1119,  1120,  1121,  1122,    -1,
      39,    40,    41,    -1,    -1,    -1,   602,   603,   604,   605,
     606,   607,   608,   609,  1138,    -1,  1140,    -1,    -1,    -1,
      -1,  1145,    -1,    -1,  1148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1156,    -1,  1158,    -1,    -1,  1161,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1180,  1181,    -1,  1183,
    1184,  1185,  1186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1207,    -1,    -1,    -1,    -1,  1212,  1213,
      -1,  1215,    -1,  1217,    -1,  1219,  1220,    -1,    -1,  1223,
      -1,    -1,    -1,    -1,  1228,    -1,    -1,    -1,  1232,    -1,
      -1,   150,    -1,   152,    -1,    -1,  1240,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,
      -1,  1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,
      -1,    -1,    31,    32,    -1,   761,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,  1303,
    1304,  1305,  1306,  1307,  1308,  1309,  1310,  1311,  1312,  1313,
    1314,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   798,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     806,   807,    -1,    -1,    -1,    -1,    -1,   256,   257,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
     269,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     846,   120,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
     856,    -1,    -1,    -1,   860,    -1,   862,    -1,    -1,   865,
     309,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   360,    -1,   362,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   952,   396,    -1,    -1,
      -1,    -1,   401,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   421,   979,    -1,    -1,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,  1029,  1030,    52,    -1,    -1,    55,  1035,
    1036,    58,    59,  1039,  1040,    62,   485,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,   511,    -1,   513,    -1,    -1,    -1,   517,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,   530,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
     539,    -1,    -1,    -1,    -1,   544,    -1,    -1,    -1,    -1,
    1106,    -1,    -1,    -1,  1110,  1111,  1112,  1113,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1132,  1133,    -1,  1135,
      -1,  1137,    -1,    -1,    -1,    -1,    -1,   164,  1144,    -1,
     589,  1147,    -1,   170,  1150,   172,  1152,   174,  1154,    -1,
      -1,   600,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    18,    -1,    -1,    -1,    22,    -1,
    1196,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,
      64,    -1,    66,    67,   683,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    12,    77,    78,    79,    80,    81,    -1,    19,
      -1,    -1,    86,    -1,    88,    -1,    90,    -1,    -1,    -1,
      -1,    31,    96,    -1,    98,    -1,   100,    -1,  1274,  1275,
      -1,    -1,  1278,  1279,  1280,   109,  1282,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,   762,    -1,    -1,   150,    -1,    -1,    89,
      -1,   770,    -1,    93,    -1,   159,    -1,    -1,   162,   163,
     164,   780,   102,    -1,   168,    -1,   170,    -1,   172,   173,
     174,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,   798,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,    -1,   138,   139,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   843,    -1,   166,   167,   168,    -1,
     170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   883,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   898,
      -1,    -1,    -1,    -1,    -1,     0,     1,    -1,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,   951,    48,    49,    50,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,   976,    73,    -1,
      -1,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    90,    91,    92,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,   103,   104,
      -1,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1062,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    12,    -1,    14,    15,    16,    17,    18,    19,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    -1,    73,    -1,    -1,    76,    77,    78,    79,
      -1,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    -1,
     170,   171,   172,    -1,   174,    -1,     1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    90,    91,    92,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,   103,   104,
      -1,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,   174,
      -1,     1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    -1,    73,    -1,    -1,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,   171,   172,    -1,   174,    -1,     1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    90,    91,    92,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,   103,   104,
      -1,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,   174,
      -1,     1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    -1,    73,    -1,    -1,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,   171,   172,    -1,   174,    -1,     1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    90,    91,    92,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,   103,   104,
      -1,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    -1,    73,    -1,    -1,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    76,    77,    78,    79,    -1,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    90,    91,    92,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,   103,   104,
      -1,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    -1,    73,    -1,    -1,    76,    77,    78,    79,
      -1,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    76,    77,    78,    79,    -1,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    90,    91,    92,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,   103,   104,
      -1,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    -1,    73,    -1,    -1,    76,    77,    78,    79,
      -1,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    76,    77,    78,    79,    -1,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    90,    91,    92,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,   103,   104,
      -1,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    -1,    73,    -1,    -1,    76,    77,    78,    79,
      -1,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    76,    77,    78,    79,    -1,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    90,    91,    92,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,   103,   104,
      -1,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    -1,    73,    -1,    -1,    76,    77,    78,    79,
      -1,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    76,    77,    78,    79,    -1,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    90,    91,    92,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,   103,   104,
      -1,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    12,    -1,    14,
      -1,    -1,    17,    18,    19,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,   143,   144,
     145,   146,    -1,   148,    -1,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,    -1,   172,    -1,    -1,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    12,    -1,    14,    -1,    -1,    17,    18,    19,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,    -1,   138,   139,
      -1,   141,   142,   143,   144,   145,   146,    -1,   148,    -1,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,   166,   167,   168,   169,
     170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    -1,    17,    18,    -1,    20,    -1,    22,    23,    -1,
      25,    -1,    -1,    -1,    -1,    30,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    61,    62,    63,    64,
      -1,    66,    67,    -1,    69,    70,    71,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,   103,   104,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    26,    27,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    89,    -1,    -1,    -1,    93,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,   147,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,    -1,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,
     110,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,   112,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,   107,   108,    -1,   110,   111,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,   173,    -1,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,
     110,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,   173,    -1,    -1,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,   107,   108,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,   173,    -1,    -1,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,   175,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,   107,   108,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,   173,    -1,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,   147,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,    -1,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,   147,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,   171,   172,    -1,    -1,    -1,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,   147,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,    -1,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,   107,   108,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    89,
      -1,    -1,    -1,    93,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    78,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,   122,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,   175,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,   147,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,   173,    -1,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,   175,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,   171,   172,    -1,    -1,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,
     175,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,   175,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,   111,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,     4,     5,     6,     7,
       8,     9,    10,    -1,   159,    -1,    14,   162,   163,   164,
      18,    -1,    -1,   168,    22,   170,    -1,   172,    -1,    27,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    -1,    -1,    64,    -1,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,
      78,    79,    80,    81,    -1,    -1,    -1,    -1,    86,    -1,
      88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      98,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      12,    -1,   150,    -1,    -1,    17,    18,    19,    20,    -1,
      -1,   159,    -1,    25,   162,   163,   164,    -1,    -1,    -1,
     168,    33,   170,    35,   172,    -1,   174,    39,    -1,   177,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,    -1,   138,   139,    -1,   141,
     142,   143,   144,   145,   146,    -1,   148,    -1,    -1,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,   163,    -1,    -1,   166,   167,   168,   169,    -1,    -1,
     172,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
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
     142,   143,   144,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
      -1,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,     3,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    12,    -1,    -1,
      -1,    -1,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,   141,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    -1,    -1,    -1,   164,    -1,    -1,    62,    -1,    -1,
      -1,    -1,   172,    -1,   174,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,    -1,
      -1,    -1,    -1,    -1,   109,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,   143,   144,
     145,   146,    -1,   148,    -1,    -1,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,     3,   163,    -1,
      -1,   166,   167,   168,   169,   170,    12,    -1,    -1,    -1,
      -1,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      96,    97,    -1,    99,    19,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    31,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,    -1,   138,   139,    -1,   141,   142,   143,   144,   145,
     146,    -1,   148,    -1,    -1,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,   163,    -1,    -1,
     166,   167,   168,   169,    89,    -1,    -1,    12,    93,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    89,   170,    -1,    12,    93,    -1,
      -1,    -1,   177,    -1,    19,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    89,   170,    -1,    12,    93,    -1,
      -1,    -1,   177,    -1,    19,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    89,   170,    -1,    12,    93,    -1,
      -1,    -1,   177,    -1,    19,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    89,   170,    -1,    12,    93,    -1,
      -1,    -1,   177,    -1,    19,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    89,   170,    -1,    12,    93,    -1,
      -1,    -1,   177,    -1,    19,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    89,   170,    -1,    12,    93,    -1,
      -1,    -1,   177,    -1,    19,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    89,   170,    -1,    -1,    93,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    31,    32,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,    -1,   144,
     145,   146,    47,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    -1,   170,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    31,   124,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    72,
      -1,   166,   167,   168,    -1,   170,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    12,    89,    -1,    -1,    -1,
      93,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      47,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    47,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,   170,    19,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    12,    89,   166,   167,   168,    93,   170,
      19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    12,
      89,   166,   167,   168,    93,   170,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    12,    89,   166,   167,   168,
      93,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      47,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,   170,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    47,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    47,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    12,    89,   166,   167,   168,    93,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      89,   166,   167,   168,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,   102,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,   124,    -1,    -1,    -1,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    72,    -1,   166,   167,   168,
      -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    12,    89,    -1,    -1,    -1,    93,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    12,    89,   166,   167,   168,    93,   170,
      19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    31,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      89,   166,   167,   168,    93,    -1,    -1,    -1,    12,    -1,
     175,    -1,   177,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,   120,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   124,    -1,   166,   167,   168,
      -1,    -1,    -1,    -1,    -1,    89,   175,    -1,   177,    93,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    12,    -1,   166,   167,   168,
     124,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      12,    -1,   166,   167,   168,    -1,   170,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,   141,   142,    -1,   144,   145,   146,
     102,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,   165,   166,
     167,   168,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,
     177,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      12,    13,    -1,    -1,    -1,   177,    -1,    19,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    31,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,   173,    -1,    -1,    -1,   177,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,   123,   124,    -1,
      -1,    -1,    12,    89,    -1,   177,    -1,    93,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    31,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   124,    -1,
     166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    12,    89,
     166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    12,    13,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    31,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    89,   166,   167,   168,    93,
      -1,    -1,    -1,   173,    12,    -1,    -1,   177,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,   173,
      -1,    89,    -1,   177,    -1,    93,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      12,    13,   166,   167,   168,    -1,   124,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    -1,    -1,    -1,    12,   173,    -1,    -1,    -1,   177,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    89,   144,   145,   146,    93,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,   177,   124,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,    -1,
      -1,    19,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    31,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    31,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,   165,   166,   167,
     168,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,   177,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,   124,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,    -1,
      -1,    19,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    31,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,   177,
      -1,    19,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    31,    32,   153,   154,   155,    -1,   157,
     158,   159,   160,    -1,    -1,   163,   124,    -1,   166,   167,
     168,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,   177,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,   177,
      -1,    19,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    31,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    -1,    -1,    -1,    12,    13,    -1,   175,    -1,   177,
      -1,    19,    60,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    31,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      12,    89,    -1,    -1,    -1,    93,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    31,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    12,    89,   166,   167,
     168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    12,    89,   166,   167,
     168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,
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
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    31,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    89,
     166,   167,   168,    93,    -1,    -1,    -1,    12,    -1,    -1,
      -1,   177,   102,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,   177,    93,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    31,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      22,   166,   167,   168,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    35,    -1,    37,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    89,    -1,    -1,    -1,    93,    -1,
      -1,    73,    -1,    -1,    76,    -1,    78,    -1,    -1,    -1,
      82,    83,    84,    85,    -1,    -1,    88,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,     1,   163,     3,
      -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,   165,    -1,    -1,    -1,    -1,   170,    33,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    17,    18,
      -1,    20,    -1,    77,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    96,    97,    -1,    99,    45,   101,    -1,   103,
      49,    -1,   106,    52,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,     3,
      -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,    87,    -1,
      -1,    -1,    -1,    17,    18,    -1,    20,    96,    97,    -1,
      99,    25,   101,    27,   103,    -1,    -1,   106,    -1,    33,
      -1,   110,    -1,    -1,   113,    39,    -1,   116,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    62,    -1,
      -1,   140,    -1,    -1,    -1,    69,    70,    -1,    17,    18,
      -1,    20,    -1,    77,    78,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    33,    -1,    90,    -1,    -1,    -1,
      39,    -1,    96,    97,    -1,    99,    45,   101,    -1,   103,
      49,    -1,   106,    52,    -1,    -1,   110,    -1,    -1,   113,
      -1,   115,   116,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    17,    18,    -1,    20,    96,    97,    -1,
      99,    25,   101,    -1,   103,    -1,    -1,   106,    -1,    33,
      -1,   110,   111,    -1,   113,    39,    -1,   116,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    17,    18,
      -1,    20,    -1,    77,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    96,    97,    -1,    99,    45,   101,    -1,   103,
      49,    -1,   106,    52,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    17,    18,    -1,    20,    96,    97,    -1,
      99,    25,   101,    -1,   103,    -1,    -1,   106,    -1,    33,
      -1,   110,    -1,    -1,   113,    39,    -1,   116,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116
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
     249,   252,   253,   254,   255,   259,   260,   265,   266,   267,
     268,   271,   272,   277,   278,   280,   281,   286,   290,   291,
     293,   294,   316,   321,   322,   326,   327,   348,   349,   350,
     352,   354,   355,   356,   364,   365,   366,   367,   368,   369,
     371,   374,   375,   376,   377,   378,   379,   380,   381,   383,
     384,   385,   386,   387,   165,    22,    35,    44,    55,    88,
     101,   170,   239,   248,   278,   348,   355,   365,   366,   371,
     374,   376,   377,   120,   357,   358,     3,   218,   371,   357,
     109,   328,    90,   218,   186,   342,   371,   172,     3,    17,
      18,    20,    25,    33,    39,    45,    49,    52,    62,    69,
      70,    77,    87,    96,    97,    99,   101,   103,   106,   110,
     113,   116,   219,   220,   222,    11,    27,   115,   244,   371,
      82,    85,   202,   172,   220,   250,   251,   220,   220,   172,
     220,   329,   330,    32,   206,   225,   371,   263,   264,   371,
     371,    18,    77,    96,   113,   371,   371,   371,     8,   172,
     229,   228,    27,    33,    47,    49,    51,    75,    78,    90,
      97,   101,   109,   185,   224,   282,   283,   284,   307,   308,
     309,   334,   339,   371,   342,   107,   108,   165,   286,   287,
     370,   371,   373,   371,   225,   371,   371,   371,   100,   172,
     186,   371,   371,   188,   192,   206,   188,   206,   371,   373,
       3,   371,   371,   371,   371,   371,     1,   171,   184,   207,
     342,   111,   151,   343,   344,   373,   371,    80,   186,    22,
      35,    38,    55,    88,   170,   189,   190,   191,   202,   206,
     194,     8,   149,   196,   170,    46,    84,   114,   203,    26,
     327,   371,     8,   267,   371,   372,    24,    32,    40,    41,
      42,   119,   174,   241,   351,   353,    55,   146,   269,   220,
       1,   193,   220,   273,   276,   172,   302,    53,    73,    83,
     311,    27,    75,    78,    90,   109,   312,    27,    78,    90,
     109,   310,   220,   323,   324,   329,   164,   165,   371,    12,
      19,    31,    89,    93,   124,   141,   142,   144,   145,   146,
     148,   149,   150,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   166,   167,   168,   177,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   169,   298,   172,   174,    89,    93,
     371,   220,    40,    41,   174,    55,   172,   186,   165,   206,
     326,   165,   147,   165,   221,   345,   346,   347,   373,   170,
     220,   220,   102,   206,   102,   125,   206,   298,   203,   345,
     172,   147,   165,   172,   206,   186,   111,   172,   220,   331,
     332,     1,   146,   338,    47,   147,   186,   225,   147,   225,
      13,   172,   172,   225,   345,   230,   230,    47,    90,   308,
     309,   173,   147,   111,   172,   220,   306,   146,   165,   371,
     371,   123,   288,   165,   170,   225,   172,   345,   165,   256,
     256,   165,   171,   171,   184,   147,   171,   371,   147,   173,
     147,   173,     8,   220,    40,    41,   174,   241,     3,   172,
     197,     1,   171,   207,   214,   215,   371,   209,   371,    65,
      36,    72,   165,   267,   269,   109,   236,   291,   371,   186,
      78,   122,   242,   371,   242,   371,   242,   371,   244,   371,
     175,   242,   342,   371,   382,   220,   250,   342,   170,   171,
     276,   125,   147,   171,   173,   303,   305,   307,   312,    90,
       1,   146,   336,   337,    90,     1,     3,    12,    17,    19,
      20,    25,    45,    52,    54,    62,    70,    87,    99,   103,
     110,   116,   141,   142,   143,   144,   145,   146,   148,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   163,
     166,   167,   168,   169,   172,   220,   295,   296,   297,   298,
     348,   125,   325,   147,   165,   165,   371,   371,   371,   242,
     371,   242,   371,   371,   371,   371,   371,   371,   371,     3,
      20,    33,    62,   103,   109,   221,   371,   371,   371,   371,
     371,   371,   371,   371,   371,   371,   371,   371,   371,   371,
     371,   371,   371,    68,   373,   373,   373,   373,   373,   345,
     345,   242,   371,   242,   371,   102,   371,   371,   342,   220,
      27,    47,    90,   115,   359,   362,   363,   371,   387,   329,
     371,   125,   173,   147,   125,   125,   186,    34,   186,   371,
      34,   371,    65,   173,   346,   250,   220,   246,   247,   332,
     147,   173,   224,   371,   125,   333,   371,   330,   263,   220,
     326,   371,   371,   173,   306,   312,   284,   332,   146,   340,
     341,   339,   289,   205,     1,   261,   347,   173,    21,   257,
     171,   173,   173,   343,   173,   343,   371,   371,   342,   196,
     173,   198,   199,   347,   171,   171,   147,   165,    13,   149,
     147,   165,    13,    36,    72,   220,   141,   142,   143,   144,
     145,   159,   163,   168,   208,   297,   298,   299,   371,   208,
     210,   269,   170,   237,   326,    47,   357,   119,   220,   172,
     225,    32,    47,   225,   120,   225,   360,    32,    47,   225,
     360,   225,   360,    47,   225,   360,   225,   225,   371,   175,
     360,    47,   147,   175,   360,   123,   147,   175,   172,   165,
       1,   270,   371,   274,   275,   147,   173,   338,     1,   224,
     334,   335,   105,   313,   172,   301,   371,   141,   149,   301,
     301,   371,   324,   172,   174,   165,   165,   165,   165,   165,
     165,   173,   175,    32,    47,    32,    47,    47,   175,    47,
      90,   147,   173,    17,    20,    25,    45,    52,    62,    70,
      87,    99,   110,   116,   348,    89,    89,   165,   373,   347,
     371,   371,    34,   186,    34,    34,   186,   102,   206,   220,
     173,   147,   173,   173,   331,   338,    68,   373,   220,   173,
     173,   341,   146,   285,   173,   339,   151,   300,   333,   371,
     171,    74,   117,   171,   262,   173,   172,   206,   220,   258,
      47,    47,    47,   175,   147,   173,   214,   221,    17,    18,
      20,    25,    45,    49,    52,    70,    77,    87,    96,    97,
      99,   101,   110,   113,   116,   170,   213,   299,   371,   111,
     371,   208,   210,   147,   165,    13,   165,   170,   270,   323,
     329,   242,   371,   225,   371,    47,   342,    44,   371,   242,
     371,   172,   225,    44,   371,   242,   371,   225,   225,   242,
     371,   225,   186,   175,   242,   371,   175,   186,   371,   175,
     371,   175,   371,   346,   171,    82,    85,   171,   185,   193,
     227,   276,   193,   305,   313,    60,   118,   317,   303,   304,
     173,   296,   298,   173,   175,    44,   242,   371,    44,   242,
     371,   242,   371,   362,   338,   348,   348,   186,   186,   371,
     186,    34,   165,   247,   206,   333,   172,   172,   300,   333,
     334,   313,   341,   371,   186,   239,   342,   258,   146,   206,
     242,   371,   242,   371,   242,   371,   199,   208,    13,    36,
      72,    36,    72,   165,   165,   299,   371,   371,   270,   171,
     165,   165,   357,   357,   165,   371,   173,   371,    32,   225,
      32,   225,   361,   362,   371,    32,   225,   360,    32,   225,
     360,   225,   360,   225,   360,   186,   175,   360,   175,   360,
     186,   123,   173,   193,   227,   227,   276,   317,     3,    92,
     103,   318,   319,   320,   371,   292,   173,   301,   301,   371,
      32,    32,   371,    32,    32,   175,   175,   333,    34,   186,
     345,   345,   300,   333,    32,   225,   173,   371,   175,   175,
     171,   111,   371,   208,   210,   208,   210,    13,   171,   225,
     225,   225,   102,    44,   371,    44,   371,   147,   173,   102,
      44,   371,   225,    44,   371,   225,   225,   225,    44,   186,
     371,   175,    44,   186,   371,   175,   371,   227,   279,   320,
     118,   147,   125,   152,   154,   155,   157,   158,    60,    32,
     165,   205,   226,   314,   315,   102,    44,    44,   102,    44,
      44,    44,    44,   186,   173,   173,   186,   206,   165,   165,
     165,   165,   371,   371,   371,   371,   362,   371,   371,   371,
     371,   186,   371,   186,   315,   371,   319,   320,   320,   320,
     320,   320,   320,   318,   184,   371,   371,   371,   371,   371,
     371,   164,   164,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   371,   371,   371,   371,   371,
     371
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   181,   182,   183,   183,   184,   184,   185,   185,   186,
     186,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   189,   189,   190,   191,   191,
     192,   193,   193,   194,   194,   194,   195,   196,   196,   197,
     197,   198,   198,   199,   200,   201,   201,   201,   202,   202,
     202,   203,   203,   204,   205,   205,   205,   206,   207,   207,
     208,   208,   208,   208,   208,   208,   209,   209,   209,   209,
     209,   209,   210,   210,   211,   211,   211,   211,   211,   211,
     211,   212,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   214,
     214,   214,   214,   214,   215,   215,   216,   217,   217,   217,
     217,   217,   217,   218,   218,   219,   219,   219,   219,   219,
     220,   220,   221,   221,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   224,   224,   224,   224,   225,
     225,   226,   226,   227,   227,   228,   227,   229,   227,   230,
     230,   230,   230,   230,   230,   230,   230,   231,   231,   231,
     231,   232,   233,   233,   234,   235,   235,   235,   236,   235,
     237,   235,   238,   239,   240,   240,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   242,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   244,   244,   245,   245,   246,   246,
     247,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   249,   249,   249,   249,   249,   249,
     250,   251,   251,   252,   253,   254,   254,   254,   255,   255,
     255,   255,   256,   256,   257,   257,   257,   258,   258,   259,
     260,   260,   261,   261,   262,   262,   262,   263,   263,   263,
     264,   264,   265,   266,   266,   267,   268,   268,   268,   269,
     269,   270,   270,   270,   270,   270,   271,   271,   272,   273,
     273,   274,   273,   273,   275,   273,   276,   276,   277,   279,
     278,   280,   281,   281,   281,   282,   282,   283,   283,   284,
     284,   284,   285,   285,   286,   288,   287,   289,   287,   290,
     292,   291,   293,   293,   293,   293,   293,   294,   295,   295,
     296,   296,   296,   297,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     299,   299,   300,   300,   301,   301,   302,   302,   303,   303,
     304,   304,   305,   306,   306,   307,   307,   307,   307,   307,
     307,   308,   308,   309,   309,   309,   309,   309,   309,   309,
     309,   309,   310,   310,   310,   310,   310,   310,   311,   311,
     311,   312,   312,   312,   312,   312,   312,   312,   313,   313,
     314,   314,   315,   315,   315,   316,   317,   317,   317,   317,
     317,   318,   318,   319,   319,   319,   319,   319,   319,   319,
     320,   320,   321,   322,   322,   323,   323,   324,   325,   325,
     326,   326,   326,   326,   326,   328,   327,   327,   329,   329,
     330,   330,   331,   331,   331,   332,   332,   332,   333,   333,
     333,   334,   335,   335,   335,   336,   336,   337,   337,   338,
     338,   338,   338,   339,   339,   340,   341,   341,   342,   342,
     343,   343,   344,   344,   345,   345,   346,   346,   347,   347,
     348,   348,   349,   349,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   350,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   352,   353,   353,
     353,   353,   353,   353,   353,   354,   355,   356,   356,   356,
     356,   356,   356,   356,   357,   357,   358,   359,   359,   360,
     361,   361,   362,   362,   362,   363,   363,   363,   363,   363,
     363,   364,   364,   364,   364,   364,   365,   365,   365,   365,
     365,   366,   367,   367,   367,   367,   367,   367,   368,   369,
     370,   370,   371,   371,   371,   371,   371,   371,   371,   371,
     371,   371,   371,   371,   371,   371,   371,   371,   371,   372,
     372,   373,   373,   373,   374,   374,   374,   374,   375,   375,
     375,   375,   375,   376,   376,   376,   377,   377,   377,   377,
     377,   377,   378,   378,   378,   378,   379,   379,   380,   380,
     381,   381,   381,   381,   381,   381,   381,   381,   381,   381,
     381,   381,   381,   382,   382,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   384,   384,
     384,   384,   384,   384,   384,   385,   385,   385,   385,   386,
     386,   386,   386,   387,   387,   387,   387,   387,   387,   387
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     1,     3,     3,     3,     3,     2,     1,
       1,     1,     1,     1,     2,     1,     3,     3,     3,     3,
       2,     3,     3,     2,     2,     1,     1,     0,     1,     1,
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
       1,     1,     1,     1,     1,     1,     1,     3,     5,     5,
       2,     1,     1,     1,     1,     6,     7,     3,     0,     6,
       0,     6,     2,     0,     3,     2,     5,     3,     3,     6,
       6,     4,     5,     5,     3,     3,     6,     5,     6,     5,
       6,     3,     4,     3,     4,     5,     6,     5,     6,     3,
       4,     3,     4,     6,     7,     6,     7,     4,     5,     4,
       5,     4,     4,     3,     6,     5,     4,     3,     6,     5,
       6,     5,     8,     7,     4,     4,     6,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     4,     5,     3,     6,     2,     2,
       3,     3,     0,     2,     2,     3,     5,     1,     3,     3,
       5,     5,     0,     2,     3,     2,     4,     4,     3,     1,
       1,     3,     3,     5,     5,     2,     1,     1,     1,     0,
       2,     0,     2,     3,     3,     4,     3,     3,     3,     1,
       2,     0,     4,     2,     0,     5,     1,     3,     1,     0,
       8,     0,     1,     1,     1,     0,     1,     1,     3,     1,
       3,     1,     0,     2,     6,     0,     3,     0,     4,     1,
       0,     7,     4,     4,     6,     6,     4,     2,     1,     3,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     0,     3,     2,     3,     1,     3,
       0,     1,     1,     1,     1,     4,     5,     4,     5,     6,
       6,     0,     1,     1,     1,     1,     2,     2,     1,     1,
       1,     1,     0,     1,     1,     2,     1,     1,     1,     1,
       1,     0,     1,     1,     2,     1,     1,     1,     0,     1,
       1,     1,     1,     2,     1,     1,     0,     2,     2,     4,
       4,     1,     3,     3,     3,     3,     3,     3,     3,     2,
       1,     1,     3,     1,     2,     1,     3,     2,     0,     2,
       1,     2,     1,     1,     1,     0,     5,     3,     1,     3,
       3,     5,     1,     1,     3,     1,     2,     3,     0,     2,
       2,     1,     1,     1,     1,     2,     1,     0,     1,     0,
       2,     2,     1,     1,     1,     2,     0,     1,     1,     3,
       1,     1,     3,     3,     0,     1,     1,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     4,     4,
       4,     5,     2,     2,     1,     2,     1,     2,     1,     2,
       1,     2,     1,     1,     2,     6,     6,     4,     9,     9,
       7,     6,     6,     4,     9,     9,     7,     2,     2,     3,
       4,     6,     6,     9,     9,     6,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     4,     1,     3,     4,
       1,     3,     4,     3,     3,     1,     1,     2,     1,     2,
       1,     1,     2,     2,     2,     2,     2,     8,     8,     9,
       9,     4,     3,     3,     2,     2,     2,     1,     3,     4,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     4,     4,     4,     3,     3,     3,     3,
       5,     5,     3,     4,     3,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     4,     4,
       5,     4,     5,     3,     5,     3,     3,     3,     3,     3,
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
#line 638 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6603 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 642 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6609 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 643 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6615 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 650 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6623 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 658 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6631 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 662 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6640 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 672 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6648 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 677 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6656 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 684 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6662 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 685 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6668 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6674 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 687 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6680 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 688 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6686 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 689 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6692 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 690 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6698 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: binary_implements_stmt  */
#line 692 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6704 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 693 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6710 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: TDEFER stmt  */
#line 695 "chpl.ypp"
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
#line 6727 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 707 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6733 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 708 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6739 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 710 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6752 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 719 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6765 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TLABEL ident_def stmt  */
#line 728 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6773 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 732 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6784 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: error TSEMI  */
#line 739 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6793 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: assignment_stmt  */
#line 746 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6799 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: if_stmt  */
#line 747 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6805 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: loop_stmt  */
#line 748 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6811 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: select_stmt  */
#line 749 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6817 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: manage_stmt  */
#line 750 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6823 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 751 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6829 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: throw_stmt  */
#line 752 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6835 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 754 "chpl.ypp"
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
#line 6853 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 768 "chpl.ypp"
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
#line 6872 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 783 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6884 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 791 "chpl.ypp"
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
#line 6900 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TLOCAL do_stmt  */
#line 803 "chpl.ypp"
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
#line 6915 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TON expr do_stmt  */
#line 814 "chpl.ypp"
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
#line 6931 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 826 "chpl.ypp"
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
#line 6947 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSERIAL do_stmt  */
#line 838 "chpl.ypp"
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
#line 6962 "bison-chpl-lib.cpp"
    break;

  case 44: /* tryable_stmt: TSYNC stmt  */
#line 849 "chpl.ypp"
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
#line 6979 "bison-chpl-lib.cpp"
    break;

  case 47: /* collect_attributes: %empty  */
#line 869 "chpl.ypp"
  {
    auto attributeGroup = context->buildAttributeGroup((yyloc));
    if (attributeGroup != nullptr) {
      context->resetAttributeGroupPartsState();
    }
    context->loopAttributes.push_back(std::move(attributeGroup));
  }
#line 6991 "bison-chpl-lib.cpp"
    break;

  case 53: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 894 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), true, (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6999 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace  */
#line 898 "chpl.ypp"
 {
  // this allows us to distinguish between @attribute() and @attribute
  // but should not be needed once `@unstable "msg"` is deprecated and gone
  auto lst = new MaybeNamedActualList();
  context->noteAttribute((yylsp[-1]), (yyvsp[-1].expr), false, (yyvsp[0].exprList), lst);
 }
#line 7010 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_stmt: attribute_decl_begin STRINGLITERAL  */
#line 905 "chpl.ypp"
 {
  // this allows us to maintain @unstable "message" without requiring parens
  // but should not be needed once `@unstable "msg"` is deprecated and gone
  auto lst = new MaybeNamedActualList();
  auto actual = makeMaybeNamedActual((yyvsp[0].expr));
  lst->push_back(actual);
  context->noteAttribute((yylsp[-1]), (yyvsp[-1].expr), false, context->makeList(), lst);
 }
#line 7023 "bison-chpl-lib.cpp"
    break;

  case 56: /* attribute_decl_begin: TATMARK TIDENT  */
#line 917 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 7032 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: %empty  */
#line 924 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 7040 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 928 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 7051 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP TRP  */
#line 938 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 7057 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 940 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 7065 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 947 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 7074 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 952 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 7084 "bison-chpl-lib.cpp"
    break;

  case 64: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 965 "chpl.ypp"
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
#line 7105 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 985 "chpl.ypp"
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
#line 7124 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 1000 "chpl.ypp"
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
#line 7143 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1015 "chpl.ypp"
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
#line 7160 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1030 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 7166 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1031 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 7173 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1033 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 7180 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1038 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 7186 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1039 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 7193 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1045 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 7203 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1063 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 7211 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1067 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 7219 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR error TRCBR  */
#line 1071 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 7231 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt: block_stmt_body  */
#line 1082 "chpl.ypp"
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
#line 7250 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1100 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 7256 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1101 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7262 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1106 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 7270 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1110 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7278 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1114 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 7287 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1119 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 7295 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1123 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7303 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1127 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 7312 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1137 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7321 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1142 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7331 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1148 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7342 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1155 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 7351 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1160 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7362 "bison-chpl-lib.cpp"
    break;

  case 91: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1167 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7374 "bison-chpl-lib.cpp"
    break;

  case 92: /* opt_only_ls: %empty  */
#line 1177 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7380 "bison-chpl-lib.cpp"
    break;

  case 93: /* opt_only_ls: renames_ls  */
#line 1178 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7386 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1183 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7395 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1188 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7407 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1196 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7420 "bison-chpl-lib.cpp"
    break;

  case 97: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1205 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7434 "bison-chpl-lib.cpp"
    break;

  case 98: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1215 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7446 "bison-chpl-lib.cpp"
    break;

  case 99: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1223 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7459 "bison-chpl-lib.cpp"
    break;

  case 100: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1232 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7473 "bison-chpl-lib.cpp"
    break;

  case 101: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1245 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7482 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_expr: expr  */
#line 1281 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 7490 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_expr: expr TDOT all_op_name  */
#line 1285 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7499 "bison-chpl-lib.cpp"
    break;

  case 121: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1290 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7508 "bison-chpl-lib.cpp"
    break;

  case 122: /* import_expr: expr TAS ident_use  */
#line 1295 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 7518 "bison-chpl-lib.cpp"
    break;

  case 123: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1301 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 7528 "bison-chpl-lib.cpp"
    break;

  case 124: /* import_ls: import_expr  */
#line 1309 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7534 "bison-chpl-lib.cpp"
    break;

  case 125: /* import_ls: import_ls TCOMMA import_expr  */
#line 1310 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7540 "bison-chpl-lib.cpp"
    break;

  case 126: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1315 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7550 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1324 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7558 "bison-chpl-lib.cpp"
    break;

  case 128: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1328 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7566 "bison-chpl-lib.cpp"
    break;

  case 129: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1332 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7574 "bison-chpl-lib.cpp"
    break;

  case 130: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1336 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7582 "bison-chpl-lib.cpp"
    break;

  case 131: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1340 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7590 "bison-chpl-lib.cpp"
    break;

  case 132: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1344 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7598 "bison-chpl-lib.cpp"
    break;

  case 133: /* opt_label_ident: %empty  */
#line 1352 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7604 "bison-chpl-lib.cpp"
    break;

  case 134: /* opt_label_ident: TIDENT  */
#line 1353 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7610 "bison-chpl-lib.cpp"
    break;

  case 142: /* ident_use: TIDENT  */
#line 1378 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7616 "bison-chpl-lib.cpp"
    break;

  case 143: /* ident_use: TTHIS  */
#line 1379 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7622 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TBOOL  */
#line 1406 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7628 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TENUM  */
#line 1407 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7634 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TINT  */
#line 1408 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7640 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TUINT  */
#line 1409 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7646 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TREAL  */
#line 1410 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7652 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TIMAG  */
#line 1411 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7658 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TCOMPLEX  */
#line 1412 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7664 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TBYTES  */
#line 1413 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7670 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TSTRING  */
#line 1414 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7676 "bison-chpl-lib.cpp"
    break;

  case 172: /* scalar_type: TLOCALE  */
#line 1415 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7682 "bison-chpl-lib.cpp"
    break;

  case 173: /* scalar_type: TNOTHING  */
#line 1416 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7688 "bison-chpl-lib.cpp"
    break;

  case 174: /* scalar_type: TVOID  */
#line 1417 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7694 "bison-chpl-lib.cpp"
    break;

  case 179: /* do_stmt: TDO stmt  */
#line 1431 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7700 "bison-chpl-lib.cpp"
    break;

  case 180: /* do_stmt: block_stmt  */
#line 1432 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7706 "bison-chpl-lib.cpp"
    break;

  case 181: /* return_stmt: TRETURN TSEMI  */
#line 1437 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7717 "bison-chpl-lib.cpp"
    break;

  case 182: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1444 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7728 "bison-chpl-lib.cpp"
    break;

  case 183: /* class_level_stmt: TSEMI  */
#line 1454 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7736 "bison-chpl-lib.cpp"
    break;

  case 184: /* class_level_stmt: inner_class_level_stmt  */
#line 1458 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7746 "bison-chpl-lib.cpp"
    break;

  case 185: /* $@1: %empty  */
#line 1463 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7753 "bison-chpl-lib.cpp"
    break;

  case 186: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1465 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7762 "bison-chpl-lib.cpp"
    break;

  case 187: /* $@2: %empty  */
#line 1469 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7769 "bison-chpl-lib.cpp"
    break;

  case 188: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1471 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7778 "bison-chpl-lib.cpp"
    break;

  case 197: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1490 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7787 "bison-chpl-lib.cpp"
    break;

  case 198: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1495 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7796 "bison-chpl-lib.cpp"
    break;

  case 199: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1500 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7805 "bison-chpl-lib.cpp"
    break;

  case 200: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1505 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7813 "bison-chpl-lib.cpp"
    break;

  case 201: /* forwarding_decl_start: TFORWARDING  */
#line 1512 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7822 "bison-chpl-lib.cpp"
    break;

  case 202: /* extern_or_export: TEXTERN  */
#line 1519 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7828 "bison-chpl-lib.cpp"
    break;

  case 203: /* extern_or_export: TEXPORT  */
#line 1520 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7834 "bison-chpl-lib.cpp"
    break;

  case 204: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1525 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7844 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1535 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7853 "bison-chpl-lib.cpp"
    break;

  case 206: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1541 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7864 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1548 "chpl.ypp"
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
#line 7886 "bison-chpl-lib.cpp"
    break;

  case 208: /* $@3: %empty  */
#line 1566 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7894 "bison-chpl-lib.cpp"
    break;

  case 209: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1570 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7903 "bison-chpl-lib.cpp"
    break;

  case 210: /* $@4: %empty  */
#line 1575 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7912 "bison-chpl-lib.cpp"
    break;

  case 211: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1580 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7921 "bison-chpl-lib.cpp"
    break;

  case 212: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1588 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7929 "bison-chpl-lib.cpp"
    break;

  case 213: /* no_loop_attributes: %empty  */
#line 1593 "chpl.ypp"
                    { context->loopAttributes.push_back(nullptr); }
#line 7935 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1596 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7941 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1597 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7947 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1602 "chpl.ypp"
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
#line 7970 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1621 "chpl.ypp"
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
#line 7988 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1635 "chpl.ypp"
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
#line 8006 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1649 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8014 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1653 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8022 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1657 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8030 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: TFOR expr TIN expr do_stmt  */
#line 1661 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8038 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: TFOR expr TIN zippered_iterator do_stmt  */
#line 1665 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8046 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: TFOR expr do_stmt  */
#line 1669 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8054 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: TFOR zippered_iterator do_stmt  */
#line 1673 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8062 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1677 "chpl.ypp"
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
#line 8086 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: TFORALL expr TIN expr do_stmt  */
#line 1697 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8094 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1701 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8102 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1705 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8110 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1709 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8118 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: TFORALL expr do_stmt  */
#line 1713 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8126 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TFORALL expr forall_intent_clause do_stmt  */
#line 1717 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8134 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TFORALL zippered_iterator do_stmt  */
#line 1721 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8142 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1725 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8150 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: TFOREACH expr TIN expr do_stmt  */
#line 1729 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8158 "bison-chpl-lib.cpp"
    break;

  case 236: /* loop_stmt_base: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1733 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8166 "bison-chpl-lib.cpp"
    break;

  case 237: /* loop_stmt_base: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1737 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8174 "bison-chpl-lib.cpp"
    break;

  case 238: /* loop_stmt_base: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1741 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8182 "bison-chpl-lib.cpp"
    break;

  case 239: /* loop_stmt_base: TFOREACH expr do_stmt  */
#line 1745 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8190 "bison-chpl-lib.cpp"
    break;

  case 240: /* loop_stmt_base: TFOREACH expr forall_intent_clause do_stmt  */
#line 1749 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8198 "bison-chpl-lib.cpp"
    break;

  case 241: /* loop_stmt_base: TFOREACH zippered_iterator do_stmt  */
#line 1753 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8206 "bison-chpl-lib.cpp"
    break;

  case 242: /* loop_stmt_base: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1757 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8214 "bison-chpl-lib.cpp"
    break;

  case 243: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1761 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8222 "bison-chpl-lib.cpp"
    break;

  case 244: /* loop_stmt_base: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1765 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8230 "bison-chpl-lib.cpp"
    break;

  case 245: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1769 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8238 "bison-chpl-lib.cpp"
    break;

  case 246: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1773 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8246 "bison-chpl-lib.cpp"
    break;

  case 247: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1777 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8254 "bison-chpl-lib.cpp"
    break;

  case 248: /* loop_stmt_base: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1781 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8262 "bison-chpl-lib.cpp"
    break;

  case 249: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1785 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8271 "bison-chpl-lib.cpp"
    break;

  case 250: /* loop_stmt_base: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1790 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8280 "bison-chpl-lib.cpp"
    break;

  case 251: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1798 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 8290 "bison-chpl-lib.cpp"
    break;

  case 252: /* if_stmt: TIF expr TTHEN stmt  */
#line 1807 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8298 "bison-chpl-lib.cpp"
    break;

  case 253: /* if_stmt: TIF expr block_stmt  */
#line 1811 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8306 "bison-chpl-lib.cpp"
    break;

  case 254: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1815 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8314 "bison-chpl-lib.cpp"
    break;

  case 255: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1819 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8322 "bison-chpl-lib.cpp"
    break;

  case 256: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1823 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8330 "bison-chpl-lib.cpp"
    break;

  case 257: /* if_stmt: TIF ifvar block_stmt  */
#line 1827 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8338 "bison-chpl-lib.cpp"
    break;

  case 258: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1831 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8346 "bison-chpl-lib.cpp"
    break;

  case 259: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1835 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8354 "bison-chpl-lib.cpp"
    break;

  case 260: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1839 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8363 "bison-chpl-lib.cpp"
    break;

  case 261: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1844 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8372 "bison-chpl-lib.cpp"
    break;

  case 262: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1849 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8381 "bison-chpl-lib.cpp"
    break;

  case 263: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1854 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8390 "bison-chpl-lib.cpp"
    break;

  case 264: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1862 "chpl.ypp"
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
#line 8409 "bison-chpl-lib.cpp"
    break;

  case 265: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1877 "chpl.ypp"
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
#line 8428 "bison-chpl-lib.cpp"
    break;

  case 266: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1895 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8436 "bison-chpl-lib.cpp"
    break;

  case 267: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1899 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8444 "bison-chpl-lib.cpp"
    break;

  case 268: /* ifc_formal_ls: ifc_formal  */
#line 1905 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8450 "bison-chpl-lib.cpp"
    break;

  case 269: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1906 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8456 "bison-chpl-lib.cpp"
    break;

  case 270: /* ifc_formal: ident_def  */
#line 1911 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8462 "bison-chpl-lib.cpp"
    break;

  case 283: /* implements_type_ident: implements_type_error_ident  */
#line 1929 "chpl.ypp"
  {
    context->syntax((yyloc), "type '%s' cannot implement an interface.", (yyvsp[0].uniqueStr).c_str());
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8471 "bison-chpl-lib.cpp"
    break;

  case 290: /* interface_expr: ident_def  */
#line 1955 "chpl.ypp"
  {
    (yyval.expr) = context->buildInterfaceExpr((yylsp[0]), (yyvsp[0].uniqueStr), nullptr).release();
  }
#line 8479 "bison-chpl-lib.cpp"
    break;

  case 291: /* interface_expr_ls: interface_expr  */
#line 1969 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 8487 "bison-chpl-lib.cpp"
    break;

  case 292: /* interface_expr_ls: interface_expr_ls TCOMMA interface_expr  */
#line 1973 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 8495 "bison-chpl-lib.cpp"
    break;

  case 293: /* unary_implements_stmt: TIMPLEMENTS interface_expr_ls TSEMI  */
#line 1980 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yyvsp[-1].exprList));
  }
#line 8503 "bison-chpl-lib.cpp"
    break;

  case 294: /* binary_implements_stmt: implements_type_ident TIMPLEMENTS interface_expr TSEMI  */
#line 1987 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), context->makeList((yyvsp[-1].expr)));
  }
#line 8511 "bison-chpl-lib.cpp"
    break;

  case 295: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1994 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8519 "bison-chpl-lib.cpp"
    break;

  case 296: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1998 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 8527 "bison-chpl-lib.cpp"
    break;

  case 297: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2002 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8535 "bison-chpl-lib.cpp"
    break;

  case 298: /* try_stmt: TTRY tryable_stmt  */
#line 2009 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8543 "bison-chpl-lib.cpp"
    break;

  case 299: /* try_stmt: TTRYBANG tryable_stmt  */
#line 2013 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8551 "bison-chpl-lib.cpp"
    break;

  case 300: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 2017 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8559 "bison-chpl-lib.cpp"
    break;

  case 301: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 2021 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8567 "bison-chpl-lib.cpp"
    break;

  case 302: /* catch_expr_ls: %empty  */
#line 2027 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8573 "bison-chpl-lib.cpp"
    break;

  case 303: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2028 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8579 "bison-chpl-lib.cpp"
    break;

  case 304: /* catch_expr: TCATCH block_stmt  */
#line 2033 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8587 "bison-chpl-lib.cpp"
    break;

  case 305: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2037 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8595 "bison-chpl-lib.cpp"
    break;

  case 306: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2041 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8603 "bison-chpl-lib.cpp"
    break;

  case 307: /* catch_expr_inner: ident_def  */
#line 2048 "chpl.ypp"
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
#line 8620 "bison-chpl-lib.cpp"
    break;

  case 308: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2061 "chpl.ypp"
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
#line 8637 "bison-chpl-lib.cpp"
    break;

  case 309: /* throw_stmt: TTHROW expr TSEMI  */
#line 2077 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8647 "bison-chpl-lib.cpp"
    break;

  case 310: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2085 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8655 "bison-chpl-lib.cpp"
    break;

  case 311: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2089 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8665 "bison-chpl-lib.cpp"
    break;

  case 312: /* when_stmt_ls: %empty  */
#line 2097 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8671 "bison-chpl-lib.cpp"
    break;

  case 313: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2098 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8677 "bison-chpl-lib.cpp"
    break;

  case 314: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2103 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8685 "bison-chpl-lib.cpp"
    break;

  case 315: /* when_stmt: TOTHERWISE stmt  */
#line 2107 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8694 "bison-chpl-lib.cpp"
    break;

  case 316: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2112 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 8704 "bison-chpl-lib.cpp"
    break;

  case 317: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2121 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8713 "bison-chpl-lib.cpp"
    break;

  case 318: /* manager_expr: expr TAS ident_def  */
#line 2126 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8721 "bison-chpl-lib.cpp"
    break;

  case 319: /* manager_expr: expr  */
#line 2130 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8729 "bison-chpl-lib.cpp"
    break;

  case 320: /* manager_expr_ls: manager_expr  */
#line 2136 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8735 "bison-chpl-lib.cpp"
    break;

  case 321: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2137 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8741 "bison-chpl-lib.cpp"
    break;

  case 322: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2142 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8749 "bison-chpl-lib.cpp"
    break;

  case 323: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2151 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8758 "bison-chpl-lib.cpp"
    break;

  case 324: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2156 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8769 "bison-chpl-lib.cpp"
    break;

  case 325: /* class_start: class_tag ident_def  */
#line 2167 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8777 "bison-chpl-lib.cpp"
    break;

  case 326: /* class_tag: TCLASS  */
#line 2173 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8783 "bison-chpl-lib.cpp"
    break;

  case 327: /* class_tag: TRECORD  */
#line 2174 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8789 "bison-chpl-lib.cpp"
    break;

  case 328: /* class_tag: TUNION  */
#line 2175 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8795 "bison-chpl-lib.cpp"
    break;

  case 329: /* opt_inherit: %empty  */
#line 2179 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8801 "bison-chpl-lib.cpp"
    break;

  case 330: /* opt_inherit: TCOLON expr_ls  */
#line 2180 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8807 "bison-chpl-lib.cpp"
    break;

  case 331: /* class_level_stmt_ls: %empty  */
#line 2184 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8816 "bison-chpl-lib.cpp"
    break;

  case 332: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2189 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8824 "bison-chpl-lib.cpp"
    break;

  case 333: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2193 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8832 "bison-chpl-lib.cpp"
    break;

  case 334: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2197 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8840 "bison-chpl-lib.cpp"
    break;

  case 335: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2201 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8848 "bison-chpl-lib.cpp"
    break;

  case 336: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2208 "chpl.ypp"
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
#line 8869 "bison-chpl-lib.cpp"
    break;

  case 337: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2225 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8883 "bison-chpl-lib.cpp"
    break;

  case 338: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2238 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8891 "bison-chpl-lib.cpp"
    break;

  case 339: /* enum_ls: enum_item  */
#line 2245 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8900 "bison-chpl-lib.cpp"
    break;

  case 340: /* enum_ls: enum_ls TCOMMA  */
#line 2250 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8910 "bison-chpl-lib.cpp"
    break;

  case 341: /* $@5: %empty  */
#line 2256 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8919 "bison-chpl-lib.cpp"
    break;

  case 342: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2261 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8928 "bison-chpl-lib.cpp"
    break;

  case 343: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2266 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8937 "bison-chpl-lib.cpp"
    break;

  case 344: /* $@6: %empty  */
#line 2271 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8946 "bison-chpl-lib.cpp"
    break;

  case 345: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2276 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8955 "bison-chpl-lib.cpp"
    break;

  case 346: /* enum_item: ident_def  */
#line 2286 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8966 "bison-chpl-lib.cpp"
    break;

  case 347: /* enum_item: ident_def TASSIGN expr  */
#line 2293 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8979 "bison-chpl-lib.cpp"
    break;

  case 348: /* lambda_decl_start: TLAMBDA  */
#line 2305 "chpl.ypp"
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
#line 8995 "bison-chpl-lib.cpp"
    break;

  case 349: /* $@7: %empty  */
#line 2322 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 9005 "bison-chpl-lib.cpp"
    break;

  case 350: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2328 "chpl.ypp"
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
#line 9027 "bison-chpl-lib.cpp"
    break;

  case 352: /* linkage_spec: linkage_spec_empty  */
#line 2351 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 9033 "bison-chpl-lib.cpp"
    break;

  case 353: /* linkage_spec: TINLINE  */
#line 2352 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 9040 "bison-chpl-lib.cpp"
    break;

  case 354: /* linkage_spec: TOVERRIDE  */
#line 2354 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 9047 "bison-chpl-lib.cpp"
    break;

  case 355: /* opt_fn_type_formal_ls: %empty  */
#line 2359 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 9053 "bison-chpl-lib.cpp"
    break;

  case 356: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2360 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9059 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_type_formal_ls: fn_type_formal  */
#line 2364 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9065 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2365 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9071 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_type_formal: named_formal  */
#line 2370 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9077 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2373 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 9083 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_type_formal: formal_type  */
#line 2375 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 9089 "bison-chpl-lib.cpp"
    break;

  case 362: /* opt_fn_type_ret_type: %empty  */
#line 2379 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 9095 "bison-chpl-lib.cpp"
    break;

  case 363: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2380 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9101 "bison-chpl-lib.cpp"
    break;

  case 364: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2393 "chpl.ypp"
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
#line 9120 "bison-chpl-lib.cpp"
    break;

  case 365: /* $@8: %empty  */
#line 2411 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 9130 "bison-chpl-lib.cpp"
    break;

  case 366: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2417 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 9143 "bison-chpl-lib.cpp"
    break;

  case 367: /* $@9: %empty  */
#line 2426 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 9153 "bison-chpl-lib.cpp"
    break;

  case 368: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2432 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 9166 "bison-chpl-lib.cpp"
    break;

  case 369: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2444 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 9174 "bison-chpl-lib.cpp"
    break;

  case 370: /* $@10: %empty  */
#line 2453 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();

    // May never have been built if there was a syntax error.
    if ((yyvsp[-4].functionParts).errorExpr == nullptr) {
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name->name());
    }
  }
#line 9188 "bison-chpl-lib.cpp"
    break;

  case 371: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2463 "chpl.ypp"
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
#line 9211 "bison-chpl-lib.cpp"
    break;

  case 372: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2485 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9223 "bison-chpl-lib.cpp"
    break;

  case 373: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2493 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9235 "bison-chpl-lib.cpp"
    break;

  case 374: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2501 "chpl.ypp"
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
#line 9250 "bison-chpl-lib.cpp"
    break;

  case 375: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2512 "chpl.ypp"
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
#line 9265 "bison-chpl-lib.cpp"
    break;

  case 376: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2523 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 9275 "bison-chpl-lib.cpp"
    break;

  case 377: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2532 "chpl.ypp"
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
#line 9291 "bison-chpl-lib.cpp"
    break;

  case 379: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2547 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 9297 "bison-chpl-lib.cpp"
    break;

  case 382: /* fn_ident: ident_def TBANG  */
#line 2554 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 9307 "bison-chpl-lib.cpp"
    break;

  case 422: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2611 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 9313 "bison-chpl-lib.cpp"
    break;

  case 423: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2612 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9319 "bison-chpl-lib.cpp"
    break;

  case 424: /* opt_formal_ls: %empty  */
#line 2616 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 9325 "bison-chpl-lib.cpp"
    break;

  case 425: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2617 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9331 "bison-chpl-lib.cpp"
    break;

  case 426: /* req_formal_ls: TLP TRP  */
#line 2621 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 9337 "bison-chpl-lib.cpp"
    break;

  case 427: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2622 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9343 "bison-chpl-lib.cpp"
    break;

  case 428: /* formal_ls_inner: formal  */
#line 2626 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9349 "bison-chpl-lib.cpp"
    break;

  case 429: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2627 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9355 "bison-chpl-lib.cpp"
    break;

  case 430: /* formal_ls: %empty  */
#line 2631 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 9361 "bison-chpl-lib.cpp"
    break;

  case 431: /* formal_ls: formal_ls_inner  */
#line 2632 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9367 "bison-chpl-lib.cpp"
    break;

  case 435: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2646 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9375 "bison-chpl-lib.cpp"
    break;

  case 436: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2651 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 9383 "bison-chpl-lib.cpp"
    break;

  case 437: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2656 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9391 "bison-chpl-lib.cpp"
    break;

  case 438: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2661 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 9399 "bison-chpl-lib.cpp"
    break;

  case 439: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2666 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9407 "bison-chpl-lib.cpp"
    break;

  case 440: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2671 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 9415 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_formal_intent_tag: %empty  */
#line 2677 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 9424 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_formal_intent_tag: required_intent_tag  */
#line 2682 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 9433 "bison-chpl-lib.cpp"
    break;

  case 443: /* required_intent_tag: TIN  */
#line 2689 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 9439 "bison-chpl-lib.cpp"
    break;

  case 444: /* required_intent_tag: TINOUT  */
#line 2690 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 9445 "bison-chpl-lib.cpp"
    break;

  case 445: /* required_intent_tag: TOUT  */
#line 2691 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 9451 "bison-chpl-lib.cpp"
    break;

  case 446: /* required_intent_tag: TCONST TIN  */
#line 2692 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 9457 "bison-chpl-lib.cpp"
    break;

  case 447: /* required_intent_tag: TCONST TREF  */
#line 2693 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9463 "bison-chpl-lib.cpp"
    break;

  case 448: /* required_intent_tag: TCONST  */
#line 2694 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9469 "bison-chpl-lib.cpp"
    break;

  case 449: /* required_intent_tag: TPARAM  */
#line 2695 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9475 "bison-chpl-lib.cpp"
    break;

  case 450: /* required_intent_tag: TREF  */
#line 2696 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9481 "bison-chpl-lib.cpp"
    break;

  case 451: /* required_intent_tag: TTYPE  */
#line 2697 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9487 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_this_intent_tag: %empty  */
#line 2701 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9493 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_this_intent_tag: TPARAM  */
#line 2702 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9499 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_this_intent_tag: TREF  */
#line 2703 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9505 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_this_intent_tag: TCONST TREF  */
#line 2704 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9511 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_this_intent_tag: TCONST  */
#line 2705 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9517 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_this_intent_tag: TTYPE  */
#line 2706 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9523 "bison-chpl-lib.cpp"
    break;

  case 458: /* proc_iter_or_op: TPROC  */
#line 2710 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9529 "bison-chpl-lib.cpp"
    break;

  case 459: /* proc_iter_or_op: TITER  */
#line 2711 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9535 "bison-chpl-lib.cpp"
    break;

  case 460: /* proc_iter_or_op: TOPERATOR  */
#line 2712 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9541 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_ret_tag: %empty  */
#line 2716 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9547 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_ret_tag: TOUT  */
#line 2717 "chpl.ypp"
                    { (yyval.returnTag) = Function::OUT; }
#line 9553 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_ret_tag: TCONST  */
#line 2718 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 9559 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_ret_tag: TCONST TREF  */
#line 2719 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 9565 "bison-chpl-lib.cpp"
    break;

  case 465: /* opt_ret_tag: TREF  */
#line 2720 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 9571 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_ret_tag: TPARAM  */
#line 2721 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 9577 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_ret_tag: TTYPE  */
#line 2722 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 9583 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_throws_error: %empty  */
#line 2726 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9589 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_throws_error: TTHROWS  */
#line 2727 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9595 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_function_body_stmt: TSEMI  */
#line 2730 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9601 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_function_body_stmt: function_body_stmt  */
#line 2731 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9607 "bison-chpl-lib.cpp"
    break;

  case 472: /* function_body_stmt: block_stmt_body  */
#line 2735 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9613 "bison-chpl-lib.cpp"
    break;

  case 473: /* function_body_stmt: TDO toplevel_stmt  */
#line 2736 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 9619 "bison-chpl-lib.cpp"
    break;

  case 474: /* function_body_stmt: return_stmt  */
#line 2737 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt), &((yylsp[0]))); }
#line 9625 "bison-chpl-lib.cpp"
    break;

  case 475: /* query_expr: TQUERIEDIDENT  */
#line 2741 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9631 "bison-chpl-lib.cpp"
    break;

  case 476: /* opt_lifetime_where: %empty  */
#line 2746 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9637 "bison-chpl-lib.cpp"
    break;

  case 477: /* opt_lifetime_where: TWHERE expr  */
#line 2748 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9643 "bison-chpl-lib.cpp"
    break;

  case 478: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2750 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9649 "bison-chpl-lib.cpp"
    break;

  case 479: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2752 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9655 "bison-chpl-lib.cpp"
    break;

  case 480: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2754 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9661 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_components_expr: lifetime_expr  */
#line 2759 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9667 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2761 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9673 "bison-chpl-lib.cpp"
    break;

  case 483: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2766 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9679 "bison-chpl-lib.cpp"
    break;

  case 484: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2768 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9685 "bison-chpl-lib.cpp"
    break;

  case 485: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2770 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9691 "bison-chpl-lib.cpp"
    break;

  case 486: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2772 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9697 "bison-chpl-lib.cpp"
    break;

  case 487: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2774 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9703 "bison-chpl-lib.cpp"
    break;

  case 488: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2776 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9709 "bison-chpl-lib.cpp"
    break;

  case 489: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2778 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9715 "bison-chpl-lib.cpp"
    break;

  case 490: /* lifetime_ident: TIDENT  */
#line 2782 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9721 "bison-chpl-lib.cpp"
    break;

  case 491: /* lifetime_ident: TTHIS  */
#line 2783 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9727 "bison-chpl-lib.cpp"
    break;

  case 492: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2788 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9736 "bison-chpl-lib.cpp"
    break;

  case 493: /* type_alias_decl_stmt_start: TTYPE  */
#line 2797 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9744 "bison-chpl-lib.cpp"
    break;

  case 494: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2801 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9753 "bison-chpl-lib.cpp"
    break;

  case 495: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2809 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9761 "bison-chpl-lib.cpp"
    break;

  case 496: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2813 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9769 "bison-chpl-lib.cpp"
    break;

  case 497: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2820 "chpl.ypp"
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
#line 9791 "bison-chpl-lib.cpp"
    break;

  case 498: /* opt_init_type: %empty  */
#line 2840 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9797 "bison-chpl-lib.cpp"
    break;

  case 499: /* opt_init_type: TASSIGN expr  */
#line 2842 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9803 "bison-chpl-lib.cpp"
    break;

  case 500: /* var_decl_type: TPARAM  */
#line 2846 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9809 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_type: TCONST TREF  */
#line 2847 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9815 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_type: TREF  */
#line 2848 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9821 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_type: TCONST  */
#line 2849 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9827 "bison-chpl-lib.cpp"
    break;

  case 504: /* var_decl_type: TVAR  */
#line 2850 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9833 "bison-chpl-lib.cpp"
    break;

  case 505: /* $@11: %empty  */
#line 2855 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9842 "bison-chpl-lib.cpp"
    break;

  case 506: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2859 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9851 "bison-chpl-lib.cpp"
    break;

  case 507: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2864 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9860 "bison-chpl-lib.cpp"
    break;

  case 508: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2872 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9868 "bison-chpl-lib.cpp"
    break;

  case 509: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2876 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9876 "bison-chpl-lib.cpp"
    break;

  case 510: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2883 "chpl.ypp"
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
#line 9898 "bison-chpl-lib.cpp"
    break;

  case 511: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2901 "chpl.ypp"
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
#line 9918 "bison-chpl-lib.cpp"
    break;

  case 512: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2920 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9926 "bison-chpl-lib.cpp"
    break;

  case 513: /* tuple_var_decl_component: ident_def  */
#line 2924 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9934 "bison-chpl-lib.cpp"
    break;

  case 514: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2928 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9942 "bison-chpl-lib.cpp"
    break;

  case 515: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2935 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9948 "bison-chpl-lib.cpp"
    break;

  case 516: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2937 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9954 "bison-chpl-lib.cpp"
    break;

  case 517: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2939 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9960 "bison-chpl-lib.cpp"
    break;

  case 518: /* opt_init_expr: %empty  */
#line 2945 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9966 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2946 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9972 "bison-chpl-lib.cpp"
    break;

  case 520: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2947 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9978 "bison-chpl-lib.cpp"
    break;

  case 521: /* formal_or_ret_type_expr: expr  */
#line 2951 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9984 "bison-chpl-lib.cpp"
    break;

  case 522: /* ret_type: formal_or_ret_type_expr  */
#line 2955 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9990 "bison-chpl-lib.cpp"
    break;

  case 523: /* ret_type: reserved_type_ident_use  */
#line 2956 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9996 "bison-chpl-lib.cpp"
    break;

  case 524: /* ret_type: error  */
#line 2957 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 10002 "bison-chpl-lib.cpp"
    break;

  case 525: /* colon_ret_type: TCOLON ret_type  */
#line 2961 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10008 "bison-chpl-lib.cpp"
    break;

  case 526: /* colon_ret_type: error  */
#line 2962 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 10014 "bison-chpl-lib.cpp"
    break;

  case 527: /* opt_ret_type: %empty  */
#line 2966 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 10020 "bison-chpl-lib.cpp"
    break;

  case 529: /* opt_type: %empty  */
#line 2971 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 10026 "bison-chpl-lib.cpp"
    break;

  case 530: /* opt_type: TCOLON expr  */
#line 2972 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10032 "bison-chpl-lib.cpp"
    break;

  case 531: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2973 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10038 "bison-chpl-lib.cpp"
    break;

  case 532: /* opt_type: error  */
#line 2974 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 10044 "bison-chpl-lib.cpp"
    break;

  case 533: /* formal_type: formal_or_ret_type_expr  */
#line 2978 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 10050 "bison-chpl-lib.cpp"
    break;

  case 534: /* formal_type: reserved_type_ident_use  */
#line 2979 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10056 "bison-chpl-lib.cpp"
    break;

  case 535: /* colon_formal_type: TCOLON formal_type  */
#line 2983 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10062 "bison-chpl-lib.cpp"
    break;

  case 536: /* opt_colon_formal_type: %empty  */
#line 2987 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 10068 "bison-chpl-lib.cpp"
    break;

  case 537: /* opt_colon_formal_type: colon_formal_type  */
#line 2988 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 10074 "bison-chpl-lib.cpp"
    break;

  case 538: /* expr_ls: expr  */
#line 2994 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10080 "bison-chpl-lib.cpp"
    break;

  case 539: /* expr_ls: expr_ls TCOMMA expr  */
#line 2995 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10086 "bison-chpl-lib.cpp"
    break;

  case 540: /* tuple_component: TUNDERSCORE  */
#line 2999 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10092 "bison-chpl-lib.cpp"
    break;

  case 541: /* tuple_component: opt_try_expr  */
#line 3000 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 10098 "bison-chpl-lib.cpp"
    break;

  case 542: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 3005 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 10106 "bison-chpl-lib.cpp"
    break;

  case 543: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 3009 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10114 "bison-chpl-lib.cpp"
    break;

  case 544: /* opt_actual_ls: %empty  */
#line 3015 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 10120 "bison-chpl-lib.cpp"
    break;

  case 545: /* opt_actual_ls: actual_ls  */
#line 3016 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 10126 "bison-chpl-lib.cpp"
    break;

  case 546: /* actual_ls: actual_expr  */
#line 3021 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10135 "bison-chpl-lib.cpp"
    break;

  case 547: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3026 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10145 "bison-chpl-lib.cpp"
    break;

  case 548: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3034 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10151 "bison-chpl-lib.cpp"
    break;

  case 549: /* actual_expr: opt_try_expr  */
#line 3035 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10157 "bison-chpl-lib.cpp"
    break;

  case 550: /* ident_expr: ident_use  */
#line 3039 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10163 "bison-chpl-lib.cpp"
    break;

  case 551: /* ident_expr: scalar_type  */
#line 3040 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10169 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TSINGLE expr  */
#line 3053 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10175 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3055 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10181 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3057 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10187 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3059 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10193 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 3061 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 10203 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TATOMIC expr  */
#line 3067 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10209 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TSYNC expr  */
#line 3069 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10215 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TOWNED  */
#line 3072 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10221 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TOWNED expr  */
#line 3074 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10227 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TUNMANAGED  */
#line 3076 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10233 "bison-chpl-lib.cpp"
    break;

  case 567: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3078 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10239 "bison-chpl-lib.cpp"
    break;

  case 568: /* sub_type_level_expr: TSHARED  */
#line 3080 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10245 "bison-chpl-lib.cpp"
    break;

  case 569: /* sub_type_level_expr: TSHARED expr  */
#line 3082 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10251 "bison-chpl-lib.cpp"
    break;

  case 570: /* sub_type_level_expr: TBORROWED  */
#line 3084 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10257 "bison-chpl-lib.cpp"
    break;

  case 571: /* sub_type_level_expr: TBORROWED expr  */
#line 3086 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10263 "bison-chpl-lib.cpp"
    break;

  case 572: /* sub_type_level_expr: TCLASS  */
#line 3088 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10269 "bison-chpl-lib.cpp"
    break;

  case 573: /* sub_type_level_expr: TRECORD  */
#line 3090 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10275 "bison-chpl-lib.cpp"
    break;

  case 574: /* for_expr: no_loop_attributes for_expr_base  */
#line 3094 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 10281 "bison-chpl-lib.cpp"
    break;

  case 575: /* for_expr_base: TFOR expr TIN expr TDO expr  */
#line 3099 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10295 "bison-chpl-lib.cpp"
    break;

  case 576: /* for_expr_base: TFOR expr TIN zippered_iterator TDO expr  */
#line 3109 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10309 "bison-chpl-lib.cpp"
    break;

  case 577: /* for_expr_base: TFOR expr TDO expr  */
#line 3119 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10322 "bison-chpl-lib.cpp"
    break;

  case 578: /* for_expr_base: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 3128 "chpl.ypp"
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
#line 10341 "bison-chpl-lib.cpp"
    break;

  case 579: /* for_expr_base: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
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
#line 10360 "bison-chpl-lib.cpp"
    break;

  case 580: /* for_expr_base: TFOR expr TDO TIF expr TTHEN expr  */
#line 3158 "chpl.ypp"
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
#line 10379 "bison-chpl-lib.cpp"
    break;

  case 581: /* for_expr_base: TFORALL expr TIN expr TDO expr  */
#line 3173 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10393 "bison-chpl-lib.cpp"
    break;

  case 582: /* for_expr_base: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3183 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10407 "bison-chpl-lib.cpp"
    break;

  case 583: /* for_expr_base: TFORALL expr TDO expr  */
#line 3193 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10420 "bison-chpl-lib.cpp"
    break;

  case 584: /* for_expr_base: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3202 "chpl.ypp"
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
#line 10439 "bison-chpl-lib.cpp"
    break;

  case 585: /* for_expr_base: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3217 "chpl.ypp"
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
#line 10458 "bison-chpl-lib.cpp"
    break;

  case 586: /* for_expr_base: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3232 "chpl.ypp"
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
#line 10477 "bison-chpl-lib.cpp"
    break;

  case 587: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3249 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 10483 "bison-chpl-lib.cpp"
    break;

  case 588: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3254 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 10491 "bison-chpl-lib.cpp"
    break;

  case 589: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3258 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 10499 "bison-chpl-lib.cpp"
    break;

  case 590: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3262 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10507 "bison-chpl-lib.cpp"
    break;

  case 591: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3266 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10515 "bison-chpl-lib.cpp"
    break;

  case 592: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3270 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10523 "bison-chpl-lib.cpp"
    break;

  case 593: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3274 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10531 "bison-chpl-lib.cpp"
    break;

  case 594: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3278 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10539 "bison-chpl-lib.cpp"
    break;

  case 595: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3285 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10553 "bison-chpl-lib.cpp"
    break;

  case 596: /* nil_expr: TNIL  */
#line 3302 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10559 "bison-chpl-lib.cpp"
    break;

  case 604: /* opt_task_intent_ls: %empty  */
#line 3320 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10565 "bison-chpl-lib.cpp"
    break;

  case 605: /* opt_task_intent_ls: task_intent_clause  */
#line 3321 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10571 "bison-chpl-lib.cpp"
    break;

  case 606: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3326 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10581 "bison-chpl-lib.cpp"
    break;

  case 607: /* task_intent_ls: intent_expr  */
#line 3334 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10587 "bison-chpl-lib.cpp"
    break;

  case 608: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3335 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10593 "bison-chpl-lib.cpp"
    break;

  case 609: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3340 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10603 "bison-chpl-lib.cpp"
    break;

  case 610: /* forall_intent_ls: intent_expr  */
#line 3348 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10609 "bison-chpl-lib.cpp"
    break;

  case 611: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3349 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10615 "bison-chpl-lib.cpp"
    break;

  case 612: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3354 "chpl.ypp"
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
#line 10633 "bison-chpl-lib.cpp"
    break;

  case 613: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3368 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10641 "bison-chpl-lib.cpp"
    break;

  case 614: /* intent_expr: expr TREDUCE ident_expr  */
#line 3372 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10649 "bison-chpl-lib.cpp"
    break;

  case 615: /* task_var_prefix: TCONST  */
#line 3378 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10655 "bison-chpl-lib.cpp"
    break;

  case 616: /* task_var_prefix: TIN  */
#line 3379 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10661 "bison-chpl-lib.cpp"
    break;

  case 617: /* task_var_prefix: TCONST TIN  */
#line 3380 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10667 "bison-chpl-lib.cpp"
    break;

  case 618: /* task_var_prefix: TREF  */
#line 3381 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10673 "bison-chpl-lib.cpp"
    break;

  case 619: /* task_var_prefix: TCONST TREF  */
#line 3382 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10679 "bison-chpl-lib.cpp"
    break;

  case 620: /* task_var_prefix: TVAR  */
#line 3383 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10685 "bison-chpl-lib.cpp"
    break;

  case 621: /* new_maybe_decorated: TNEW  */
#line 3388 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10691 "bison-chpl-lib.cpp"
    break;

  case 622: /* new_maybe_decorated: TNEW TOWNED  */
#line 3390 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10697 "bison-chpl-lib.cpp"
    break;

  case 623: /* new_maybe_decorated: TNEW TSHARED  */
#line 3392 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10703 "bison-chpl-lib.cpp"
    break;

  case 624: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3394 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10709 "bison-chpl-lib.cpp"
    break;

  case 625: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3396 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10715 "bison-chpl-lib.cpp"
    break;

  case 626: /* new_expr: new_maybe_decorated expr  */
#line 3402 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10723 "bison-chpl-lib.cpp"
    break;

  case 627: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3409 "chpl.ypp"
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
#line 10739 "bison-chpl-lib.cpp"
    break;

  case 628: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3421 "chpl.ypp"
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
#line 10755 "bison-chpl-lib.cpp"
    break;

  case 629: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3433 "chpl.ypp"
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
#line 10772 "bison-chpl-lib.cpp"
    break;

  case 630: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3446 "chpl.ypp"
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
#line 10790 "bison-chpl-lib.cpp"
    break;

  case 631: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3463 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10798 "bison-chpl-lib.cpp"
    break;

  case 632: /* range_literal_expr: expr TDOTDOT expr  */
#line 3470 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10807 "bison-chpl-lib.cpp"
    break;

  case 633: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3475 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10816 "bison-chpl-lib.cpp"
    break;

  case 634: /* range_literal_expr: expr TDOTDOT  */
#line 3480 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10825 "bison-chpl-lib.cpp"
    break;

  case 635: /* range_literal_expr: TDOTDOT expr  */
#line 3485 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10835 "bison-chpl-lib.cpp"
    break;

  case 636: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3491 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10845 "bison-chpl-lib.cpp"
    break;

  case 637: /* range_literal_expr: TDOTDOT  */
#line 3497 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10855 "bison-chpl-lib.cpp"
    break;

  case 638: /* cast_expr: expr TCOLON expr  */
#line 3527 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10863 "bison-chpl-lib.cpp"
    break;

  case 639: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3534 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10871 "bison-chpl-lib.cpp"
    break;

  case 640: /* super_expr: fn_expr  */
#line 3540 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10877 "bison-chpl-lib.cpp"
    break;

  case 644: /* expr: sub_type_level_expr TQUESTION  */
#line 3549 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10883 "bison-chpl-lib.cpp"
    break;

  case 645: /* expr: TQUESTION  */
#line 3551 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10889 "bison-chpl-lib.cpp"
    break;

  case 649: /* expr: fn_type  */
#line 3556 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10895 "bison-chpl-lib.cpp"
    break;

  case 659: /* opt_expr: %empty  */
#line 3570 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10901 "bison-chpl-lib.cpp"
    break;

  case 660: /* opt_expr: expr  */
#line 3571 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10907 "bison-chpl-lib.cpp"
    break;

  case 661: /* opt_try_expr: TTRY expr  */
#line 3575 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10913 "bison-chpl-lib.cpp"
    break;

  case 662: /* opt_try_expr: TTRYBANG expr  */
#line 3576 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10919 "bison-chpl-lib.cpp"
    break;

  case 663: /* opt_try_expr: super_expr  */
#line 3577 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10925 "bison-chpl-lib.cpp"
    break;

  case 669: /* call_base_expr: expr TBANG  */
#line 3594 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10933 "bison-chpl-lib.cpp"
    break;

  case 670: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3597 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10939 "bison-chpl-lib.cpp"
    break;

  case 673: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3604 "chpl.ypp"
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
#line 10955 "bison-chpl-lib.cpp"
    break;

  case 674: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3616 "chpl.ypp"
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
#line 10971 "bison-chpl-lib.cpp"
    break;

  case 675: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3628 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10979 "bison-chpl-lib.cpp"
    break;

  case 676: /* dot_expr: expr TDOT ident_use  */
#line 3635 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10985 "bison-chpl-lib.cpp"
    break;

  case 677: /* dot_expr: expr TDOT TTYPE  */
#line 3637 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10991 "bison-chpl-lib.cpp"
    break;

  case 678: /* dot_expr: expr TDOT TDOMAIN  */
#line 3639 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10997 "bison-chpl-lib.cpp"
    break;

  case 679: /* dot_expr: expr TDOT TLOCALE  */
#line 3641 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11003 "bison-chpl-lib.cpp"
    break;

  case 680: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3643 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 11013 "bison-chpl-lib.cpp"
    break;

  case 681: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3649 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 11023 "bison-chpl-lib.cpp"
    break;

  case 682: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3661 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 11029 "bison-chpl-lib.cpp"
    break;

  case 683: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3663 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 11037 "bison-chpl-lib.cpp"
    break;

  case 684: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3667 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11045 "bison-chpl-lib.cpp"
    break;

  case 685: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3671 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11053 "bison-chpl-lib.cpp"
    break;

  case 686: /* bool_literal: TFALSE  */
#line 3677 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 11059 "bison-chpl-lib.cpp"
    break;

  case 687: /* bool_literal: TTRUE  */
#line 3678 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 11065 "bison-chpl-lib.cpp"
    break;

  case 688: /* str_bytes_literal: STRINGLITERAL  */
#line 3682 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11071 "bison-chpl-lib.cpp"
    break;

  case 689: /* str_bytes_literal: BYTESLITERAL  */
#line 3683 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11077 "bison-chpl-lib.cpp"
    break;

  case 692: /* literal_expr: INTLITERAL  */
#line 3689 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 11083 "bison-chpl-lib.cpp"
    break;

  case 693: /* literal_expr: REALLITERAL  */
#line 3690 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 11089 "bison-chpl-lib.cpp"
    break;

  case 694: /* literal_expr: IMAGLITERAL  */
#line 3691 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 11095 "bison-chpl-lib.cpp"
    break;

  case 695: /* literal_expr: CSTRINGLITERAL  */
#line 3692 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 11101 "bison-chpl-lib.cpp"
    break;

  case 696: /* literal_expr: TNONE  */
#line 3693 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 11107 "bison-chpl-lib.cpp"
    break;

  case 697: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3695 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11116 "bison-chpl-lib.cpp"
    break;

  case 698: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3700 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11125 "bison-chpl-lib.cpp"
    break;

  case 699: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3705 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 11134 "bison-chpl-lib.cpp"
    break;

  case 700: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3710 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 11143 "bison-chpl-lib.cpp"
    break;

  case 701: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3715 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 11152 "bison-chpl-lib.cpp"
    break;

  case 702: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3720 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 11161 "bison-chpl-lib.cpp"
    break;

  case 703: /* assoc_expr_ls: expr TALIAS expr  */
#line 3729 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 11170 "bison-chpl-lib.cpp"
    break;

  case 704: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3734 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 11180 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TPLUS expr  */
#line 3742 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11186 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TMINUS expr  */
#line 3743 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11192 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TSTAR expr  */
#line 3744 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11198 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TDIVIDE expr  */
#line 3745 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11204 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3746 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11210 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3747 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11216 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TMOD expr  */
#line 3748 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11222 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TEQUAL expr  */
#line 3749 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11228 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3750 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11234 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3751 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11240 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3752 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11246 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TLESS expr  */
#line 3753 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11252 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TGREATER expr  */
#line 3754 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11258 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr TBAND expr  */
#line 3755 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11264 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr TBOR expr  */
#line 3756 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11270 "bison-chpl-lib.cpp"
    break;

  case 720: /* binary_op_expr: expr TBXOR expr  */
#line 3757 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11276 "bison-chpl-lib.cpp"
    break;

  case 721: /* binary_op_expr: expr TAND expr  */
#line 3758 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11282 "bison-chpl-lib.cpp"
    break;

  case 722: /* binary_op_expr: expr TOR expr  */
#line 3759 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11288 "bison-chpl-lib.cpp"
    break;

  case 723: /* binary_op_expr: expr TEXP expr  */
#line 3760 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11294 "bison-chpl-lib.cpp"
    break;

  case 724: /* binary_op_expr: expr TBY expr  */
#line 3761 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11300 "bison-chpl-lib.cpp"
    break;

  case 725: /* binary_op_expr: expr TALIGN expr  */
#line 3762 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11306 "bison-chpl-lib.cpp"
    break;

  case 726: /* binary_op_expr: expr THASH expr  */
#line 3763 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11312 "bison-chpl-lib.cpp"
    break;

  case 727: /* binary_op_expr: expr TDMAPPED expr  */
#line 3764 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11318 "bison-chpl-lib.cpp"
    break;

  case 728: /* unary_op_expr: TPLUS expr  */
#line 3768 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11324 "bison-chpl-lib.cpp"
    break;

  case 729: /* unary_op_expr: TMINUS expr  */
#line 3769 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11330 "bison-chpl-lib.cpp"
    break;

  case 730: /* unary_op_expr: TMINUSMINUS expr  */
#line 3770 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11336 "bison-chpl-lib.cpp"
    break;

  case 731: /* unary_op_expr: TPLUSPLUS expr  */
#line 3771 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11342 "bison-chpl-lib.cpp"
    break;

  case 732: /* unary_op_expr: TBANG expr  */
#line 3772 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11348 "bison-chpl-lib.cpp"
    break;

  case 733: /* unary_op_expr: expr TBANG  */
#line 3773 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 11356 "bison-chpl-lib.cpp"
    break;

  case 734: /* unary_op_expr: TBNOT expr  */
#line 3776 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11362 "bison-chpl-lib.cpp"
    break;

  case 735: /* reduce_expr: expr TREDUCE expr  */
#line 3781 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11370 "bison-chpl-lib.cpp"
    break;

  case 736: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3785 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11378 "bison-chpl-lib.cpp"
    break;

  case 737: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3789 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11386 "bison-chpl-lib.cpp"
    break;

  case 738: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3793 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11394 "bison-chpl-lib.cpp"
    break;

  case 739: /* scan_expr: expr TSCAN expr  */
#line 3800 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11402 "bison-chpl-lib.cpp"
    break;

  case 740: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3804 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11410 "bison-chpl-lib.cpp"
    break;

  case 741: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3808 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11418 "bison-chpl-lib.cpp"
    break;

  case 742: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3812 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11426 "bison-chpl-lib.cpp"
    break;


#line 11430 "bison-chpl-lib.cpp"

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
