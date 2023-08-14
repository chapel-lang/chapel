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
  YYSYMBOL_unary_implements_stmt = 250,    /* unary_implements_stmt  */
  YYSYMBOL_binary_implements_stmt = 251,   /* binary_implements_stmt  */
  YYSYMBOL_ifc_constraint = 252,           /* ifc_constraint  */
  YYSYMBOL_try_stmt = 253,                 /* try_stmt  */
  YYSYMBOL_catch_expr_ls = 254,            /* catch_expr_ls  */
  YYSYMBOL_catch_expr = 255,               /* catch_expr  */
  YYSYMBOL_catch_expr_inner = 256,         /* catch_expr_inner  */
  YYSYMBOL_throw_stmt = 257,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 258,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 259,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 260,                /* when_stmt  */
  YYSYMBOL_manager_expr = 261,             /* manager_expr  */
  YYSYMBOL_manager_expr_ls = 262,          /* manager_expr_ls  */
  YYSYMBOL_manage_stmt = 263,              /* manage_stmt  */
  YYSYMBOL_class_decl_stmt = 264,          /* class_decl_stmt  */
  YYSYMBOL_class_start = 265,              /* class_start  */
  YYSYMBOL_class_tag = 266,                /* class_tag  */
  YYSYMBOL_opt_inherit = 267,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 268,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 269,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header_lcbr = 270,         /* enum_header_lcbr  */
  YYSYMBOL_enum_ls = 271,                  /* enum_ls  */
  YYSYMBOL_272_5 = 272,                    /* $@5  */
  YYSYMBOL_273_6 = 273,                    /* $@6  */
  YYSYMBOL_enum_item = 274,                /* enum_item  */
  YYSYMBOL_lambda_decl_start = 275,        /* lambda_decl_start  */
  YYSYMBOL_lambda_decl_expr = 276,         /* lambda_decl_expr  */
  YYSYMBOL_277_7 = 277,                    /* $@7  */
  YYSYMBOL_linkage_spec_empty = 278,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 279,             /* linkage_spec  */
  YYSYMBOL_opt_fn_type_formal_ls = 280,    /* opt_fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal_ls = 281,        /* fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal = 282,           /* fn_type_formal  */
  YYSYMBOL_opt_fn_type_ret_type = 283,     /* opt_fn_type_ret_type  */
  YYSYMBOL_fn_type = 284,                  /* fn_type  */
  YYSYMBOL_fn_expr = 285,                  /* fn_expr  */
  YYSYMBOL_286_8 = 286,                    /* $@8  */
  YYSYMBOL_287_9 = 287,                    /* $@9  */
  YYSYMBOL_fn_decl_stmt_complete = 288,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 289,             /* fn_decl_stmt  */
  YYSYMBOL_290_10 = 290,                   /* $@10  */
  YYSYMBOL_fn_decl_stmt_inner = 291,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 292,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 293,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 294,                 /* fn_ident  */
  YYSYMBOL_op_ident = 295,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 296,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 297,              /* all_op_name  */
  YYSYMBOL_formal_var_arg_expr = 298,      /* formal_var_arg_expr  */
  YYSYMBOL_opt_formal_ls = 299,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 300,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 301,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 302,                /* formal_ls  */
  YYSYMBOL_formal = 303,                   /* formal  */
  YYSYMBOL_formal_ident_def = 304,         /* formal_ident_def  */
  YYSYMBOL_named_formal = 305,             /* named_formal  */
  YYSYMBOL_opt_formal_intent_tag = 306,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 307,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 308,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 309,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 310,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 311,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 312,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 313,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 314,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 315,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 316, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 317,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 318,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 319,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 320, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 321, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 322, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 323,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 324,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 325,            /* var_decl_stmt  */
  YYSYMBOL_326_11 = 326,                   /* $@11  */
  YYSYMBOL_var_decl_stmt_inner_ls = 327,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 328,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 329, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 330, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 331,            /* opt_init_expr  */
  YYSYMBOL_formal_or_ret_type_expr = 332,  /* formal_or_ret_type_expr  */
  YYSYMBOL_ret_type = 333,                 /* ret_type  */
  YYSYMBOL_colon_ret_type = 334,           /* colon_ret_type  */
  YYSYMBOL_opt_ret_type = 335,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 336,                 /* opt_type  */
  YYSYMBOL_formal_type = 337,              /* formal_type  */
  YYSYMBOL_colon_formal_type = 338,        /* colon_formal_type  */
  YYSYMBOL_opt_colon_formal_type = 339,    /* opt_colon_formal_type  */
  YYSYMBOL_expr_ls = 340,                  /* expr_ls  */
  YYSYMBOL_tuple_component = 341,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 342,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 343,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 344,                /* actual_ls  */
  YYSYMBOL_actual_expr = 345,              /* actual_expr  */
  YYSYMBOL_ident_expr = 346,               /* ident_expr  */
  YYSYMBOL_sub_type_level_expr = 347,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 348,                 /* for_expr  */
  YYSYMBOL_for_expr_base = 349,            /* for_expr_base  */
  YYSYMBOL_bracket_loop_expr = 350,        /* bracket_loop_expr  */
  YYSYMBOL_bracket_loop_expr_base = 351,   /* bracket_loop_expr_base  */
  YYSYMBOL_cond_expr = 352,                /* cond_expr  */
  YYSYMBOL_nil_expr = 353,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 354,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 355,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 356,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 357,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 358,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 359,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 360,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 361,          /* task_var_prefix  */
  YYSYMBOL_new_maybe_decorated = 362,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 363,                 /* new_expr  */
  YYSYMBOL_let_expr = 364,                 /* let_expr  */
  YYSYMBOL_range_literal_expr = 365,       /* range_literal_expr  */
  YYSYMBOL_cast_expr = 366,                /* cast_expr  */
  YYSYMBOL_tuple_expand_expr = 367,        /* tuple_expand_expr  */
  YYSYMBOL_super_expr = 368,               /* super_expr  */
  YYSYMBOL_expr = 369,                     /* expr  */
  YYSYMBOL_opt_expr = 370,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 371,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 372,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 373,           /* call_base_expr  */
  YYSYMBOL_call_expr = 374,                /* call_expr  */
  YYSYMBOL_dot_expr = 375,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 376,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 377,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 378,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 379,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 380,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 381,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 382,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 383,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 384,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 385       /* reduce_scan_op_expr  */
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

#line 509 "bison-chpl-lib.cpp"

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
#define YYLAST   20954

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  205
/* YYNRULES -- Number of rules.  */
#define YYNRULES  748
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1327

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
       0,   636,   636,   640,   641,   646,   647,   655,   659,   669,
     674,   681,   682,   683,   684,   685,   686,   687,   688,   690,
     691,   692,   705,   706,   707,   716,   725,   729,   736,   744,
     745,   746,   747,   748,   749,   750,   751,   765,   780,   788,
     800,   811,   823,   835,   846,   862,   863,   867,   877,   878,
     882,   886,   887,   891,   895,   902,   914,   922,   925,   935,
     937,   944,   949,   958,   962,   982,   997,  1012,  1028,  1029,
    1031,  1036,  1037,  1042,  1060,  1064,  1068,  1079,  1098,  1099,
    1103,  1107,  1111,  1116,  1120,  1124,  1134,  1139,  1145,  1152,
    1157,  1164,  1175,  1176,  1180,  1185,  1193,  1202,  1212,  1220,
    1229,  1242,  1250,  1251,  1252,  1253,  1254,  1255,  1256,  1257,
    1258,  1259,  1260,  1261,  1262,  1263,  1264,  1265,  1266,  1278,
    1282,  1287,  1292,  1298,  1307,  1308,  1312,  1321,  1325,  1329,
    1333,  1337,  1341,  1350,  1351,  1355,  1356,  1357,  1358,  1359,
    1363,  1364,  1376,  1377,  1382,  1383,  1384,  1385,  1386,  1387,
    1388,  1389,  1390,  1391,  1392,  1393,  1394,  1395,  1396,  1397,
    1398,  1399,  1400,  1404,  1405,  1406,  1407,  1408,  1409,  1410,
    1411,  1412,  1413,  1414,  1415,  1422,  1423,  1424,  1425,  1429,
    1430,  1434,  1441,  1451,  1455,  1461,  1461,  1467,  1467,  1476,
    1477,  1478,  1479,  1480,  1481,  1482,  1483,  1487,  1492,  1497,
    1502,  1509,  1517,  1518,  1522,  1531,  1537,  1545,  1564,  1563,
    1573,  1572,  1585,  1591,  1594,  1595,  1599,  1618,  1632,  1646,
    1650,  1654,  1658,  1662,  1666,  1670,  1674,  1694,  1698,  1702,
    1706,  1710,  1714,  1718,  1722,  1726,  1730,  1734,  1738,  1742,
    1746,  1750,  1754,  1758,  1762,  1766,  1770,  1774,  1778,  1782,
    1787,  1795,  1804,  1808,  1812,  1816,  1820,  1824,  1828,  1832,
    1836,  1841,  1846,  1851,  1859,  1874,  1892,  1896,  1903,  1904,
    1909,  1914,  1915,  1916,  1917,  1918,  1919,  1920,  1921,  1922,
    1923,  1924,  1925,  1926,  1935,  1936,  1937,  1938,  1947,  1948,
    1952,  1956,  1963,  1967,  1974,  1978,  1982,  1989,  1993,  1997,
    2001,  2008,  2009,  2013,  2017,  2021,  2028,  2041,  2057,  2065,
    2069,  2078,  2079,  2083,  2087,  2092,  2101,  2106,  2110,  2117,
    2118,  2122,  2131,  2136,  2147,  2154,  2155,  2156,  2160,  2161,
    2165,  2169,  2173,  2177,  2181,  2188,  2205,  2218,  2225,  2230,
    2237,  2236,  2246,  2252,  2251,  2266,  2273,  2285,  2303,  2300,
    2328,  2332,  2333,  2335,  2340,  2341,  2345,  2346,  2350,  2353,
    2355,  2360,  2361,  2372,  2392,  2391,  2407,  2406,  2424,  2434,
    2431,  2465,  2473,  2481,  2492,  2503,  2512,  2527,  2528,  2532,
    2533,  2534,  2543,  2544,  2545,  2546,  2547,  2548,  2549,  2550,
    2551,  2552,  2553,  2554,  2555,  2556,  2557,  2558,  2559,  2560,
    2561,  2562,  2563,  2564,  2565,  2566,  2567,  2571,  2572,  2573,
    2574,  2575,  2576,  2577,  2578,  2579,  2580,  2581,  2582,  2587,
    2588,  2592,  2593,  2597,  2598,  2602,  2603,  2607,  2608,  2612,
    2613,  2617,  2621,  2622,  2626,  2630,  2635,  2640,  2645,  2650,
    2658,  2662,  2670,  2671,  2672,  2673,  2674,  2675,  2676,  2677,
    2678,  2682,  2683,  2684,  2685,  2686,  2687,  2691,  2692,  2693,
    2697,  2698,  2699,  2700,  2701,  2702,  2703,  2707,  2708,  2711,
    2712,  2716,  2717,  2718,  2722,  2727,  2728,  2730,  2732,  2734,
    2739,  2741,  2746,  2748,  2750,  2752,  2754,  2756,  2758,  2763,
    2764,  2768,  2777,  2781,  2789,  2793,  2800,  2821,  2822,  2827,
    2828,  2829,  2830,  2831,  2836,  2835,  2844,  2852,  2856,  2863,
    2881,  2900,  2904,  2908,  2915,  2917,  2919,  2926,  2927,  2928,
    2932,  2936,  2937,  2938,  2942,  2943,  2947,  2948,  2952,  2953,
    2954,  2955,  2959,  2960,  2964,  2968,  2969,  2975,  2976,  2980,
    2981,  2985,  2989,  2996,  2997,  3001,  3006,  3015,  3016,  3020,
    3021,  3028,  3029,  3030,  3031,  3032,  3033,  3035,  3037,  3039,
    3041,  3047,  3049,  3052,  3054,  3056,  3058,  3060,  3062,  3064,
    3066,  3068,  3070,  3075,  3079,  3089,  3099,  3108,  3123,  3138,
    3153,  3163,  3173,  3182,  3197,  3212,  3230,  3234,  3238,  3242,
    3246,  3250,  3254,  3258,  3265,  3283,  3291,  3292,  3293,  3294,
    3295,  3296,  3297,  3301,  3302,  3306,  3315,  3316,  3320,  3329,
    3330,  3334,  3348,  3352,  3359,  3360,  3361,  3362,  3363,  3364,
    3368,  3370,  3372,  3374,  3376,  3382,  3389,  3401,  3413,  3426,
    3443,  3450,  3455,  3460,  3465,  3471,  3477,  3507,  3514,  3521,
    3522,  3526,  3528,  3529,  3531,  3533,  3534,  3535,  3536,  3539,
    3540,  3541,  3542,  3543,  3544,  3545,  3546,  3547,  3551,  3552,
    3556,  3557,  3558,  3562,  3563,  3564,  3565,  3574,  3575,  3578,
    3579,  3580,  3584,  3596,  3608,  3615,  3617,  3619,  3621,  3623,
    3629,  3642,  3643,  3647,  3651,  3658,  3659,  3663,  3664,  3668,
    3669,  3670,  3671,  3672,  3673,  3674,  3675,  3680,  3685,  3690,
    3695,  3700,  3709,  3714,  3723,  3724,  3725,  3726,  3727,  3728,
    3729,  3730,  3731,  3732,  3733,  3734,  3735,  3736,  3737,  3738,
    3739,  3740,  3741,  3742,  3743,  3744,  3745,  3749,  3750,  3751,
    3752,  3753,  3754,  3757,  3761,  3765,  3769,  3773,  3780,  3784,
    3788,  3792,  3800,  3801,  3802,  3803,  3804,  3805,  3806
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
  "implements_type_error_ident", "unary_implements_stmt",
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

#define YYPACT_NINF (-1105)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-749)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1105,    93,  4312, -1105,   -21,   126, -1105, -1105, -1105, -1105,
   -1105, -1105, -1105, 13221,   100,   265,   232, 15595,   297, 20708,
     100,   306,   220,   310,   265,  5871, 13221,   243, 20763, -1105,
     442,   412, -1105, 10771,   427,   401,   348, -1105,   480, 20763,
   20763, 20763, -1105,  3166, 11821,   490, 13221, 13221,   230, -1105,
     512,   515, 13221, -1105, 15595, -1105, 13221,   497,   409,   201,
     298, 14943,   546, 20838, -1105, 13221,  9196, 13221, 11821, 15595,
   13221,   502,   550,   441,  5871,   559, 13221,   562,  7796,  7796,
   -1105,   566, -1105, 15595, -1105,   571, 10946, -1105,   626, 13221,
   -1105, 13221, -1105, -1105, 15423, 13221, -1105, 11121, -1105, -1105,
   -1105,  4821,  8321, 13221, -1105,  5521, -1105, -1105, -1105, -1105,
   20252,   495, -1105,    45,   460, -1105,   248, -1105, -1105, -1105,
   -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105,
    7971, -1105,  8496, -1105, -1105,    65, -1105, -1105, -1105,   585,
   -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105,   498,
   20763, -1105, 20448,   473,   192, -1105,   235, -1105, -1105, -1105,
     226,   247, -1105, -1105, 20763,  3166, -1105,   483,   488, -1105,
   -1105, -1105,   507,   508, 13221,   510,   511, -1105, -1105, -1105,
   19903,  2176,   382,   513,   520, -1105, -1105,   446, -1105, -1105,
   -1105, -1105, -1105,     2, -1105, -1105, -1105, 13221, 20763, -1105,
   13221, 13221,    33,   600,   450, -1105, -1105, -1105, -1105, 19903,
     464, -1105, -1105,   489,  5871, -1105, -1105,   521,   161,   518,
   -1105,    35, -1105,   525, -1105,   149, 19903,  9371, -1105, -1105,
   -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105,
   -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105,
   -1105, -1105, -1105,   523, -1105, -1105, 20763, 20763,   -33, 16083,
   -1105, -1105,   596,  9371,   200,   471,  5871,  2012,   130,    40,
   -1105,  5871, -1105, -1105, 17387, -1105,    27,  3321,   778, -1105,
     519,   522, -1105, 17387,   161,   778, -1105,  9371,  2448,  2448,
      49,     1, -1105,    13, -1105, -1105, -1105, -1105,  9546,  9546,
   -1105,   463, -1105,   524,   551, -1105, -1105,  2355,   553, -1105,
   -1105, 19903,   299, 13221, 13221, -1105,   142, -1105, -1105, 19903,
     536, 18051, -1105, 17387,   161, 19903,   531,  9371, -1105, 19903,
   18096, -1105, -1105, -1105, -1105, -1105,   161,   540, -1105,   308,
     308,  1504,   778,   778,   135, -1105, -1105,  4996,   -43, -1105,
   13221,   -59,   158, -1105,  1504,   698, -1105, -1105, 20763, -1105,
   20763, -1105,  4821, -1105,    77, -1105,   596, -1105, -1105, -1105,
     704,   538,  5171, 13221, -1105, 13221,   643, -1105, -1105, 16915,
      58,   498, 19903,   435, 13221,  5871, 11296, 11996, 11996, 10771,
   11471, -1105, -1105, -1105, 20763, 13221,   544, -1105,   545, 20503,
     590,    -5, -1105,    73,   226, -1105, -1105, -1105, -1105,   627,
   -1105, -1105, -1105, -1105,   157,   629, -1105, -1105, -1105, 15253,
     595,   372, -1105,   377,   477, -1105,   573, 13221, 13221, 13221,
   11996, 11996, 13221,   484, 13221, 13221, 13221, 13221, 13221,   347,
   15423, 13221, 13221, 13221, 13221, 13221, 13221, 13221, 13221, 13221,
   13221, 13221, 13221, 13221, 13221, 13221, 13221,   656, -1105, -1105,
   -1105, -1105, -1105, 10946, 10946, -1105, -1105, -1105, -1105, 10946,
   -1105, -1105, 10946, 10946,  9371,  9721, 11996, 11996, 18203,   563,
   13221, 13221, 12171, 20763,  8671, -1105, -1105, -1105,  3166, -1105,
   13221, -1105,   606,   567,   589, -1105, -1105, -1105,   614,   617,
    5871,   710,  5871, -1105,   711, 13221,   682,   575, -1105, 10946,
   20763, -1105, -1105, -1105,  2012, -1105, -1105,   267, -1105, 13396,
     624, 13221,  3166, -1105, -1105, 13221, -1105, 20578, 13221, 13221,
   -1105,   577, -1105, -1105, -1105, -1105, 20633, -1105,   226, 15117,
   -1105,  2012, -1105,   605, 13396, -1105, 19903, 19903, -1105,   518,
   -1105,    47, -1105, 10946,   580, -1105,   735,   735, -1105, -1105,
   -1105, -1105, 12346, -1105, 18248,  8846, -1105,  9021, -1105, -1105,
     404, 11296, 11996, 11996, -1105,   608,  9896, -1105,   206, -1105,
    5346, -1105,   393, 18288,   403, 17007, 20763,  7621,  7446, -1105,
     498,   592, -1105,    35, -1105, 17463,   640, 20763,   588,    28,
   17083,    66,   634,    66,  4411,    28, 17387, 15738,   -42,   136,
   18364,   -64,   428,   619,  1869, -1105, -1105, 13221,   228, -1105,
   -1105,   269, -1105, -1105,    70, -1105, -1105,  4041, -1105,   663,
   -1105,   597,   621, -1105,   623, -1105,   625,   628,   632,   636,
   -1105,   650,   657,   658,   659,   661,   664,   665, -1105, -1105,
   -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105,
   -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105,
   13221,   630,   667,   597, -1105,   597, -1105, 13221, -1105, 20763,
   -1105, -1105,  1990,  1990,   466, -1105,   466, -1105,   466,  3217,
    1268,  1687,  2106,   161,   308, -1105,   496, -1105, -1105, -1105,
   -1105, -1105,  1504,  3784,   466,   877,   877,  1990,   877,   877,
    1398,   308,  3784,  3476,  1398,   778,   778,   308,  1504,   647,
     653,   660,   666,   668,   669,   648,   598, -1105,   466, -1105,
     466, 13221, 10946, 17539, 17615,   114,   633,    69, -1105, -1105,
   -1105,   274, -1105,   622, 19944,    67,   408, 19903, 10946, -1105,
   10946, 13221, 13221,   788,  5871,   790, 18440,  5871, 17691, 20763,
   -1105,   286, -1105,   287, -1105,   304,  2012,   130, -1105, 19903,
   10071, -1105, 19903, -1105, -1105, -1105, 20763, 18480, 18525, -1105,
     605,   680, -1105,   305, 13396, -1105,   330, -1105, 13221, -1105,
     670,   180,   655, -1105,  2819, -1105, -1105, -1105, -1105, -1105,
   -1105, -1105, 10946, 17159, 16823,   166, -1105, -1105,   316, -1105,
   -1105, -1105, -1105, 13221, -1105,   124, 15843, 13221, -1105, 12521,
    7621,  7446, -1105, 10246,   313, 10421,   350,   369, 10596,  8146,
     503,   414, -1105, -1105, -1105, 18632,   683,   672,   673, -1105,
   20763,  3166, 11996,    28, 13221,   782, 13221, -1105,  6046, 11996,
   -1105,   675, -1105,    28,  6221, 11996, -1105,    28, -1105,    28,
   11996, -1105,    28, -1105, -1105, 19903,  5871,   674, 11996, 12696,
    4646,   676, 13221, 12871, -1105, -1105, 10946,   671, 20087, 19903,
   20763,   495,   485, -1105,   663, -1105, -1105, -1105, -1105, -1105,
      62,   125, -1105, 18677, -1105, 16002, -1105, -1105, 19903, -1105,
     662,   679, -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105,
     324, 13571, 11996, 13746, 11996, 11996, 15738, 10946, -1105, -1105,
    8671, -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105,
   -1105, -1105, -1105,    46, 11996, 11996, -1105, -1105, -1105, 19903,
   19903,  5871, -1105,  5871, 13221, -1105,  5871,   804,   685,   693,
   20763,   518, -1105, -1105,   624, -1105, -1105, -1105,   687,   688,
     330, 13221,   663,   605, -1105, 11646, -1105, -1105, 19903, -1105,
    5871, 13221, -1105, -1105, -1105, 20763, -1105,   694,   518,   326,
   11996, 11996, 11996,  5871, 10946, -1105, -1105, -1105, -1105, -1105,
     496, -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105,
   -1105, -1105, -1105, -1105, -1105,  7621, -1105, -1105, 18717,    43,
   17783,   419,   697,  7621, -1105, 13221, -1105, -1105, 20133,   424,
     430,   100, 18793, -1105, 18869, 13221,   329, 10771, 19903,    32,
   17859,  8671, -1105, 10771, 19903,   123, 17235, -1105, -1105,    66,
   17311, -1105, -1105,  5871,   -28, 17935, -1105, -1105, 19903,  5871,
   19903, -1105, 18952,   333, -1105, -1105, -1105, -1105, 20319, 20365,
   -1105, -1105, 20503, -1105,    62,    51, 13221, -1105,   701,   690,
   -1105,   597,   597, -1105, -1105, -1105, 13221,   812, 19028, 13221,
     813, 19104,   691, 19144,   340, -1105,   624,   349,   354, -1105,
   -1105, 19903,   831,  5871, -1105, -1105, -1105, -1105, -1105,  9371,
    9371, -1105, -1105, -1105, -1105,   330, 19903, -1105,   239,    97,
     696, 13221, -1105,   693,    28, 17387,    66, 17311,   -26, 18011,
   -1105,   202, 13046,  7621,  7446,  7621,  7446, -1105, -1105, -1105,
   19189, 19903, 20179, -1105, -1105, -1105,    28,    28, -1105, 17387,
   -1105, 16163,  6396, -1105,  6571, -1105,   356, -1105, 16243,  6746,
   -1105,    28,  6921, -1105,    28, -1105,    28, -1105,    28, -1105,
    7096,   695,  7271,   700, -1105, 13221,   707, 20365, -1105, -1105,
   -1105, -1105, -1105,   138, -1105,   181, -1105,   253, 19305,    34,
   -1105, -1105, -1105, 19350, 13921, 14096, 19391, 14271, 14446, 14621,
   14796, -1105, -1105,  5871, -1105,   705,   708, -1105, -1105,  5871,
   -1105,   518, 19903,  5871,  5871, -1105, -1105, 19903,   439,   719,
     444,   720, 13221, -1105, -1105, -1105, -1105,  5871, 10771, 19903,
   10771, 19903,  8671, -1105,  5871, 10771, 19903, -1105, 10771, 19903,
   -1105, -1105, -1105, 10771, -1105, 19903,  5871, 10771, -1105, 19903,
    5871, 19903, -1105, -1105,    31, -1105, 13221,    51,   138,   138,
     138,   138,   138,   138,    51,  5696, -1105, -1105, -1105, -1105,
   -1105, 13221, 13221, 13221, 13221, 13221, 13221, 13221, 13221, -1105,
     723,   724,   640, -1105, -1105, -1105, -1105, -1105, 19903, 18440,
   16323, 16403, -1105, 18440, 16483, 16563, 16643, -1105, 16723, -1105,
   -1105, 19903, -1105, -1105, -1105, -1105, -1105, -1105, -1105,   743,
   -1105, 19507, 19548, 19589, 19705, 19746, 19787, -1105, -1105,  5871,
    5871,  5871,  5871,  5871,  5871, 13221, 13221, 13221, 13221, 13221,
   13221, 18440, 18440, 18440, 18440, 18440, 18440
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   142,   474,   691,   692,   693,
     687,   688,   694,   213,   603,   133,   163,   569,   170,   571,
     603,   169,   504,   502,   133,     0,   213,   288,   164,   203,
     202,   685,   201,   213,   168,    68,   289,   352,   165,     0,
       0,     0,   347,     0,   213,   172,   213,   213,   620,   595,
     695,   173,   213,   353,   563,   499,   213,     0,     0,   187,
     185,   440,   167,   572,   501,   213,   213,   213,   213,   567,
     213,     0,   171,     0,     0,   143,   213,   686,   213,   213,
     492,   166,   327,   565,   503,   174,   213,   744,     0,   213,
     746,   213,   747,   748,   636,   213,   745,   213,   644,   183,
     743,     0,   213,   213,     4,     0,     5,    10,    11,    12,
      47,    50,    51,    57,     0,    45,    71,    13,    77,    14,
      15,    16,    17,    29,   549,   550,    23,    46,   184,   194,
     213,   204,   658,   195,    18,     0,    31,    30,    20,     0,
     283,   196,    19,   654,    22,    35,    32,    33,   193,   328,
       0,   191,     0,     0,   651,   351,     0,   648,   189,   368,
     460,   451,   646,   192,     0,     0,   190,   663,   642,   641,
     645,   553,   551,     0,   213,   652,   653,   657,   656,   655,
       0,   552,     0,   664,   665,   666,   689,   690,   647,   555,
     554,   649,   650,     0,    28,   571,   164,   213,     0,   572,
     213,   213,     0,     0,   651,   663,   551,   652,   653,   561,
     552,   664,   665,     0,     0,   604,   134,     0,   570,     0,
     493,     0,   500,     0,    21,     0,   537,   213,   140,   144,
     156,   150,   149,   158,   137,   148,   159,   145,   160,   135,
     161,   154,   147,   155,   153,   151,   152,   136,   138,   146,
     157,   162,   141,     0,   139,   212,     0,     0,     0,     0,
      70,    69,    71,   213,     0,     0,     0,     0,     0,     0,
     507,     0,   180,    40,     0,   319,     0,   318,   729,   624,
     621,   622,   623,     0,   564,   730,     7,   213,   350,   350,
     447,   177,   442,   178,   443,   444,   448,   449,   176,   175,
     450,   440,   533,     0,   355,   356,   358,     0,   441,   532,
     360,   520,     0,   213,   213,   181,   648,   639,   662,   640,
       0,     0,    43,     0,   568,   556,     0,   213,    44,   562,
       0,   297,    47,   301,   298,   301,   566,     0,    56,   731,
     733,   634,   728,   727,     0,    74,    78,     0,     0,   539,
     213,     0,     0,   540,   635,     0,     6,   325,     0,   202,
       0,   326,     0,    48,     0,     9,    71,    49,    52,    55,
       0,    54,     0,   213,    72,   213,     0,   504,   200,     0,
     687,   328,   659,   208,   213,     0,   213,   213,   213,   213,
     213,   215,   573,   586,     0,   213,     0,   324,     0,     0,
     345,     0,   338,   440,   460,   458,   459,   457,   376,   462,
     461,   465,   464,   466,     0,   455,   452,   453,   456,     0,
     497,     0,   494,     0,   643,    34,   625,   213,   213,   213,
     213,   213,   213,   732,   213,   213,   213,   213,   213,     0,
     633,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   407,   414,   415,
     416,   411,   413,   213,   213,   409,   412,   410,   408,   213,
     418,   417,   213,   213,   213,   213,   213,   213,     0,     0,
     213,   213,   213,     0,   213,    36,    24,    37,     0,    25,
     213,    38,   549,     0,   544,   545,   548,   337,     0,     0,
       0,   257,     0,   407,   253,   213,     0,     0,   291,   213,
       0,   267,    26,   511,     0,   512,   514,     0,   531,   213,
     517,   213,     0,   179,    39,   213,   321,     0,   213,   213,
      41,     0,   188,   186,   445,   446,     0,   441,   460,   440,
     433,     0,   432,   535,   213,   126,   660,   661,   366,     0,
     182,     0,    42,   213,     0,   308,   299,   300,    27,    76,
      75,    79,   213,   696,     0,   213,   681,   213,   683,     8,
       0,   213,   213,   213,   214,    57,   213,    53,     0,    65,
       0,   124,     0,   119,     0,    86,     0,   213,   213,   197,
     328,     0,   210,     0,   207,   603,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   587,     0,     0,
     537,     0,   295,   329,     0,   336,   342,   213,   340,   335,
     425,     0,   427,   431,     0,   463,   525,     0,   527,   467,
     454,   423,   140,   403,   144,   401,   150,   149,   148,   145,
     405,   160,   161,   147,   151,   136,   146,   162,   400,   382,
     385,   383,   384,   406,   395,   386,   399,   391,   389,   402,
     390,   388,   393,   398,   387,   392,   396,   397,   394,   404,
     213,   379,     0,   423,   380,   423,   377,   213,   496,     0,
     491,   506,   724,   723,   726,   735,   734,   739,   738,   720,
     717,   718,   719,   637,   707,   142,     0,   677,   678,   143,
     676,   675,   631,   711,   722,   716,   714,   725,   715,   713,
     705,   710,   712,   721,   704,   708,   709,   706,   632,     0,
       0,     0,     0,     0,     0,     0,     0,   737,   736,   741,
     740,   213,   213,     0,     0,     0,   295,   614,   615,   617,
     619,     0,   606,     0,     0,     0,     0,   538,   213,   558,
     213,   213,   213,   256,     0,   252,     0,     0,     0,     0,
     557,     0,   270,     0,   268,     0,   515,     0,   530,   529,
     213,   509,   630,   508,   320,   317,     0,     0,     0,   674,
     535,   361,   357,     0,   213,   536,   517,   359,   213,   365,
       0,     0,     0,   559,     0,   302,   697,   638,   682,   541,
     684,   542,   213,     0,     0,     0,    58,    59,     0,    61,
      63,    67,    66,   213,   101,     0,     0,   213,    94,   213,
     213,   213,    64,   213,   382,   213,   383,   384,   213,   213,
     394,     0,   419,   420,    81,    80,    93,     0,     0,   330,
       0,     0,   213,     0,   213,     0,   213,   225,     0,   213,
     224,     0,   233,     0,     0,   213,   231,     0,   241,     0,
     213,   239,     0,   218,   217,   588,     0,     0,   213,   213,
       0,     0,   213,   213,   700,   292,   213,     0,   350,   346,
       0,     0,   440,   426,   467,   523,   522,   521,   524,   468,
     475,   440,   375,     0,   381,     0,   371,   372,   498,   495,
       0,     0,   132,   130,   131,   129,   128,   127,   672,   673,
       0,   213,   213,   213,   213,   213,   698,   213,   616,   618,
     213,   605,   163,   170,   169,   168,   165,   172,   173,   167,
     171,   166,   174,     0,   213,   213,   505,   547,   546,   265,
     264,     0,   259,     0,   213,   255,     0,   261,     0,   294,
       0,     0,   513,   516,   517,   518,   519,   316,     0,     0,
     517,   213,   467,   535,   534,   213,   436,   434,   367,   310,
       0,   213,   309,   312,   560,     0,   303,   306,     0,     0,
     213,   213,   213,     0,   213,    60,   125,   122,   102,   114,
     108,   107,   106,   116,   103,   117,   112,   105,   113,   111,
     109,   110,   104,   115,   118,   213,   121,   120,    89,    88,
      87,     0,     0,   213,   198,   213,   199,   330,   350,     0,
       0,   603,   603,   221,     0,   213,     0,   213,   576,     0,
       0,   213,   234,   213,   582,     0,     0,   232,   242,     0,
       0,   240,   249,     0,     0,     0,   699,   247,   589,     0,
     702,   701,     0,     0,   323,   187,   185,   322,   350,   350,
     331,   341,     0,   428,   475,     0,   213,   369,   430,     0,
     378,   423,   423,   679,   680,   294,   213,     0,     0,   213,
       0,     0,     0,     0,     0,   607,   517,   663,   663,   258,
     254,   594,   260,     0,    73,   290,   269,   266,   510,   213,
     213,   437,   435,   362,   363,   517,   422,   314,     0,     0,
       0,   213,   304,     0,     0,     0,     0,     0,     0,     0,
      62,     0,   213,   213,   213,   213,   213,    95,    98,    84,
      83,    82,   350,   205,   211,   209,     0,     0,   216,     0,
     251,     0,     0,   223,     0,   222,     0,   609,     0,     0,
     229,     0,     0,   227,     0,   237,     0,   235,     0,   250,
       0,     0,     0,     0,   248,   213,   296,   350,   333,   332,
     344,   348,   489,     0,   490,   477,   480,     0,   476,     0,
     424,   373,   374,     0,   213,   213,     0,   213,   213,   213,
     213,   296,   611,     0,   263,     0,     0,   439,   438,     0,
     313,     0,   307,     0,     0,   123,    91,    90,     0,     0,
       0,     0,   213,   206,   220,   219,   226,     0,   213,   575,
     213,   574,   213,   608,     0,   213,   581,   230,   213,   580,
     228,   238,   236,   213,   245,   591,     0,   213,   243,   590,
       0,   703,   293,   334,     0,   488,   213,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   469,   471,   473,   370,
     470,   213,   213,   213,   213,   213,   213,   213,   213,   262,
     626,   627,   315,   305,    97,   100,    96,    99,    85,   579,
       0,     0,   610,   585,     0,     0,     0,   246,     0,   244,
     349,   479,   481,   482,   485,   486,   487,   483,   484,   478,
     472,     0,     0,     0,     0,     0,     0,   628,   629,     0,
       0,     0,     0,     0,     0,   213,   213,   213,   213,   213,
     213,   578,   577,   584,   583,   593,   592
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1105, -1105, -1105,     0,   -55,  3070, -1105,   315,   781, -1105,
   -1105,   352,  -144,  -108, -1105,   318, -1105, -1105,   -92, -1105,
   -1105,    55,   637, -1105,  -548,  2714,   526,  -561, -1105,  -786,
   -1105, -1105, -1105,    82, -1105, -1105, -1105,   876, -1105,  3707,
    -217, -1105, -1105,  -458,  1210, -1104,  -850, -1105, -1105,   237,
   -1105, -1105, -1105, -1105, -1105, -1105, -1105, -1105,     7, -1105,
     537,  -252, -1105,   514, -1105, -1105,   -45,  1536, -1105, -1105,
   -1105, -1105, -1105,   572, -1105,   -73, -1105, -1105, -1105, -1105,
     381, -1105, -1105, -1105,   -96, -1105,  -338,  -787, -1105, -1105,
   -1105, -1105, -1105,  -385, -1105,   132, -1105, -1105, -1105, -1105,
   -1105,   370, -1105,   -37, -1105, -1105, -1105, -1105,   527, -1105,
   -1105, -1105, -1105,    17,  -403,  -177,  -775,  -910,  -648, -1105,
      22, -1105,    36,   379,   -48,   615,   -46, -1105, -1105,  -347,
    -817, -1105,  -324, -1105,  -142,  -331,  -319,  -864, -1105, -1105,
      95,   257, -1105,  -203,   807, -1105,  -154,   418,   176,  -269,
    -731,  -608, -1105, -1105, -1105,  -600,  -514, -1105,  -735,   -14,
     104, -1105,  -243,  -472,  -536,    -2, -1105, -1105, -1105, -1105,
   -1105, -1105,   317, -1105,   -13, -1105, -1105,  -570, -1105,  -899,
   -1105, -1105,   532,   549, -1105, -1105, -1105, -1105,  2580, -1105,
     -44,   851, -1105,  1027,  1348, -1105, -1105, -1105, -1105, -1105,
   -1105, -1105, -1105, -1105,  -479
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   346,   105,   755,   107,   108,   109,   364,
     365,   110,   111,   112,   113,   371,   577,   808,   809,   114,
     115,   116,   376,   117,   118,   119,   347,   836,   584,   837,
     120,   121,  1006,   581,   582,   122,   123,   217,   252,   268,
     124,   254,   125,   302,   850,   126,   127,   289,   288,   128,
     129,   130,   131,   132,   133,   593,   840,   134,   202,   136,
     391,   599,   137,   258,   138,   763,   764,   203,   140,   141,
     142,   143,   144,   556,   795,   978,   145,   146,   791,   973,
     275,   276,   147,   148,   149,   150,   396,   878,   151,   152,
     401,   880,   881,   402,   153,   204,  1244,   155,   156,   303,
     304,   305,   962,   157,   317,   549,   788,   158,   159,  1179,
     160,   161,   672,   673,   832,   833,   834,   966,   892,   404,
     621,  1069,   622,   543,   623,   307,   537,   419,   408,   414,
     890,  1259,  1260,   162,  1067,  1175,  1176,  1177,   163,   164,
     421,   422,   678,   165,   166,   221,   269,   270,   516,   517,
     771,   309,   888,   628,   629,   520,   310,   785,   786,   348,
     351,   352,   493,   494,   495,   205,   168,   169,   392,   170,
     393,   171,   206,   173,   214,   215,   741,   857,  1146,   742,
     743,   174,   207,   208,   177,   178,   179,   318,   180,   383,
     496,   210,   182,   211,   212,   185,   186,   187,   188,   611,
     189,   190,   191,   192,   193
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   789,   104,   368,   473,   745,   301,   219,   399,   135,
     492,   423,   225,   306,   616,   308,   674,   792,   488,   887,
     507,  1085,   320,   167,   884,   896,   831,   897,  1060,   316,
     787,   853,   135,   859,   862,  1012,   381,   761,   867,   871,
     810,  1007,   337,   591,   531,   960,   492,   518,   790,   316,
    1101,   312,  1018,   369,  1172,   967,  -288,   624,   353,   271,
     271,   768,    23,  1255,  1142,   316,  1255,  1064,  -289,   500,
     492,   518,   167,   480,   481,  1258,   167,   167,   851,  1123,
     357,   135,   505,   873,   554,   135,   135,   521,   565,   384,
     262,   476,   851,     3,   851,   477,   534,   385,   271,   167,
     290,   384,  -528,   167,   562,   386,   387,   388,   135,   385,
     492,   874,   135,    55,   566,  1124,   918,   571,   572,   388,
     292,  -311,  1065,    66,   294,    64,    66,   695,   563,   271,
    -528,   518,  1147,   866,   154,   601,   603,   362,   608,   535,
    1258,  1172,   618,  1173,   194,  1104,   361,  1160,   295,  1203,
      84,   296,   290,    57,  1174,  1149,   935,   154,   626,   919,
     477,   915,  -528,   297,  -311,   366,   619,   954,  1060,   886,
      82,  -528,   292,   227,   525,  -528,   294,  -528,   685,   687,
    1066,  -271,   300,   868,   389,   263,   851,   522,  -528,  -526,
     316,   781,   519,  -528,   370,  1197,   389,   362,   362,  1256,
     295,   362,   362,   296,   362,    57,   154,   482,  1168,  1169,
     154,   154,   167,   982,   938,   297,   519,  -526,  -311,  -528,
     213,   135,   701,  1098,   727,   729,   316,   699,  1105,  1102,
    1132,   725,   726,   154,   300,   871,   362,   154,  1129,   390,
    -528,  1174,   675,   851,   490,   765,   620,   -70,   279,  -526,
     316,   573,   838,   409,   970,  -528,   851,   492,   492,  1011,
     910,   869,  -526,   384,   167,   548,   -70,   362,   216,   167,
     964,  1199,   783,   135,   415,  -526,   519,  -528,   135,   386,
     387,   388,  1060,   869,   590,   -70,   851,  -272,   405,   916,
     316,   368,   492,   362,   373,  -528,   490,   971,  -429,  1246,
     194,   410,   433,   627,   411,   567,   559,   280,   406,  1245,
     439,   870,  -364,   490,   491,   -70,   412,  1243,   407,   172,
     601,   608,  -526,  1282,   776,   416,   281,  -526,  1247,   220,
     979,   568,   374,  1086,   746,   413,   492,   417,  1209,   429,
    1211,   983,   172,   282,   -69,   167,   154,   561,   301,  1013,
     695,   972,  -278,  1103,   135,  1192,   418,  -600,   389,   492,
     167,  -277,   375,   -69,  -670,   508,  -670,   696,  -343,   135,
     167,   194,   509,  1205,  1198,  -339,   609,   811,  1248,   135,
     697,   613,   -69,   167,  1293,  1294,  1295,  1296,  1297,  1298,
     841,   172,   135,   331,   334,   172,   172,   430,   154,  -339,
     222,   431,  -746,   154,  1053,  1249,  -746,  1250,  1251,   698,
    1252,  1253,   -69,   390,   766,   227,   882,   676,   172,   720,
     721,   920,   172,  1181,  1182,   722,   316,   316,   723,   724,
     332,   332,   316,   750,   950,   316,   316,   316,   316,  -747,
     767,   745,   883,  -747,  1121,  1084,   490,   921,   810,   433,
     699,   766,   766,   255,   437,   770,   700,   439,  -748,   949,
     951,   442,  -748,   984,   545,  1151,  1154,  -286,   735,  1156,
    1158,   750,   316,   750,  1161,  1163,   490,   952,   963,   154,
     750,   965,  -276,   260,   301,    37,   261,   750,  -350,   985,
     290,   306,   674,   308,   154,  1061,  -613,  1075,   167,  1113,
     167,  -612,  1140,  1222,   154,   286,  1166,   135,  -350,   135,
     292,    53,   290,  1191,   294,   492,   316,   154,  -350,   679,
     263,   353,  -613,   353,   522,   532,   533,  -612,   316,  1223,
     316,   172,   292,   492,   175,  -273,   294,   680,   295,   316,
     813,   296,   681,   355,   592,  -280,  1151,  1154,  1161,  1163,
     817,   176,   745,   297,   474,   522,   475,   175,   814,   805,
     295,  1013,  1208,   296,  1210,    57,  1013,  -284,   818,   508,
    -281,   679,   300,   936,   176,   297,   802,   522,   167,  1014,
     561,   287,   843,   172,  1127,   492,  1013,   135,   172,  1134,
    1021,  1013,  -743,   875,   300,  1135,  -743,  1029,   987,   701,
     876,  -275,   326,  1035,  1274,  -279,   175,   433,  1039,  1276,
     175,   175,   437,   327,  -285,   439,  1044,  -287,  -671,   442,
    -671,  -274,  -670,   176,  -670,   695,  -282,   176,   176,   338,
     372,  1257,   154,   175,   154,    88,  -667,   175,  -667,   922,
     394,   362,   923,   510,   395,   403,   427,   924,  -597,  -669,
     176,  -669,   424,   428,   176,   483,  -668,   196,  -668,   492,
    1077,   484,  1080,  1082,   172,   429,   854,   925,   900,   799,
     901,   801,  -596,   425,   926,  -601,  -602,  1170,  -599,   172,
     374,   855,   685,   727,   927,  -598,   486,  1020,   362,   172,
     489,   528,   928,   497,   529,   316,  1257,   538,   539,   544,
     492,   550,   172,   553,   937,   558,   569,   575,   586,   929,
     576,   316,   154,   316,   614,   617,   615,   625,  1072,   630,
     677,   930,   439,   430,   719,   699,   956,   431,  1114,  1116,
    1118,   748,   931,   316,  1059,   732,   750,  1062,   932,   751,
     749,   933,   752,   745,   754,   757,   175,   759,   760,   770,
     779,   784,   167,   793,   851,   167,   794,   370,   432,   844,
     846,   135,   839,   176,   135,   316,   490,   492,   889,   891,
    -142,   894,  -163,   909,  -170,   433,   434,  -169,   435,   436,
     437,  -168,   438,   439,   440,  -165,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   175,  -172,
     453,   454,   455,   175,   362,   917,  -173,  -167,  -171,   429,
    -143,   456,   902,  -166,  -174,   176,   895,   172,   903,   172,
     176,   908,   941,  1058,   943,   904,   961,   301,   974,  1025,
    1013,   905,  1026,   906,   907,  1073,   301,  1016,  1093,   316,
    1111,   969,  1054,  1017,  1184,  1187,   167,  1031,   882,  1043,
    1094,  1049,   167,   181,  1074,   135,  1195,  1196,  1095,  1099,
    1100,   135,  1128,  1180,   167,  1193,  1189,   430,   167,  1201,
    1236,   431,  1242,   135,  1059,  1240,   181,   135,  1270,   175,
     316,  1271,   492,   492,  1275,  1277,   154,  1307,  1308,   154,
    1247,   363,  1120,   806,   175,   986,   176,   172,   580,   506,
     223,   574,  1110,   605,   175,  1096,   774,   557,   429,   782,
     594,   176,  1071,  1068,  1167,   780,   536,   175,  1063,   433,
    1290,   176,  1171,  1299,   437,   181,   438,   439,  1292,   181,
     181,   442,  1087,  1088,   176,  1019,   899,   378,   449,   167,
     773,   167,   953,     0,   167,     0,   455,   316,   135,     0,
     135,   368,   181,   135,   368,     0,   181,  1109,     0,     0,
       0,     0,     0,  1058,   505,     0,   430,     0,   167,     0,
     431,   505,     0,     0,     0,     0,     0,  1108,     0,     0,
     154,   167,     0,     0,     0,     0,   154,     0,  1059,     0,
     135,     0,     0,     0,     0,     0,     0,     0,   154,     0,
       0,     0,   154,     0,     0,     0,     0,     0,  1136,  1137,
       0,     0,     0,     0,     0,     0,     0,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   183,
     442,     0,   175,     0,   175,     0,   448,   449,     0,     0,
     452,   167,     0,   453,   454,   455,     0,   167,     0,   176,
     135,   176,   183,     0,   456,     0,   135,     0,     0,   368,
       0,     0,   316,   316,     0,   181,     0,     0,     0,     0,
       0,   172,     0,   154,   172,   154,     0,  1058,   154,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   167,     0,     0,     0,     0,     0,     0,     0,     0,
     135,   183,   154,   505,   505,   183,   183,   505,   505,   505,
       0,   505,   175,     0,     0,   154,     0,   181,     0,     0,
       0,     0,   181,     0,     0,     0,     0,     0,   183,   176,
       0,     0,   183,     0,     0,     0,     0,     0,     0,     0,
     167,     0,   167,     0,     0,     0,     0,   167,     0,   135,
     167,   135,     0,     0,     0,     0,   135,     0,   167,   135,
     167,     0,     0,     0,     0,   172,     0,   135,     0,   135,
       0,   172,     0,     0,     0,   154,     0,     0,     0,     0,
       0,   154,     0,   172,     0,     0,     0,   172,     0,     0,
       0,   167,     0,     0,     0,     0,     0,   167,   181,     0,
     135,   167,   167,     0,     0,     0,   135,     0,     0,     0,
     135,   135,     0,   181,     0,   167,     0,     0,     0,     0,
       0,     0,   167,   181,   135,   154,     0,     0,     0,     0,
       0,   135,     0,     0,   167,     0,   181,     0,   167,     0,
       0,   183,     0,   135,     0,     0,     0,   135,     0,     0,
       0,     0,     0,   167,   273,  1300,     0,     0,   172,     0,
     172,     0,   135,   172,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   154,     0,   154,     0,   322,     0,
       0,   154,     0,     0,   154,     0,   175,   172,     0,   175,
       0,     0,   154,   183,   154,     0,     0,     0,   183,   429,
     172,     0,     0,   176,     0,     0,   176,   167,   167,   167,
     167,   167,   167,     0,     0,     0,   135,   135,   135,   135,
     135,   135,     0,     0,     0,   154,     0,     0,     0,     0,
       0,   154,     0,     0,     0,   154,   154,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   154,
     184,   181,     0,   181,     0,     0,   154,   430,     0,     0,
     172,   431,     0,     0,     0,     0,   172,     0,   154,     0,
       0,     0,   154,   184,   183,     0,     0,     0,     0,     0,
     175,     0,     0,     0,     0,     0,   175,   154,     0,   183,
       0,     0,     0,     0,     0,     0,     0,   176,   175,   183,
       0,     0,   175,   176,     0,     0,     0,     0,     0,   433,
     172,     0,   183,     0,   437,   176,   438,   439,     0,   176,
       0,   442,   184,     0,     0,     0,   184,   184,   449,   429,
       0,   181,     0,     0,   453,   454,   455,     0,     0,     0,
       0,   154,   154,   154,   154,   154,   154,     0,     0,   184,
       0,     0,     0,   184,     0,     0,     0,     0,     0,   172,
       0,   172,     0,     0,     0,     0,   172,     0,     0,   172,
       0,     0,     0,   175,     0,   175,     0,   172,   175,   172,
       0,     0,     0,     0,   524,     0,   526,   430,     0,     0,
     176,   431,   176,   530,     0,   176,     0,     0,     0,     0,
       0,     0,   175,     0,     0,     0,     0,     0,     0,     0,
     172,     0,     0,     0,     0,   175,   172,     0,     0,   176,
     172,   172,     0,     0,     0,     0,     0,   183,     0,   183,
       0,     0,   176,   552,   172,   429,     0,     0,   139,   433,
     434,   172,   435,   436,   437,     0,   438,   439,     0,     0,
       0,   442,     0,   172,     0,     0,     0,   172,   449,     0,
       0,   139,   184,     0,   453,   454,   455,     0,     0,     0,
       0,     0,   172,     0,     0,   175,     0,     0,     0,     0,
       0,   175,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   176,   430,     0,     0,     0,   431,   176,     0,
       0,     0,     0,     0,     0,   181,     0,   183,   181,     0,
     139,     0,     0,     0,   184,     0,     0,     0,     0,   184,
       0,     0,     0,     0,     0,   175,   172,   172,   172,   172,
     172,   172,     0,     0,     0,     0,     0,   139,     0,     0,
       0,   139,   176,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,     0,     0,     0,   442,     0,     0,
       0,     0,     0,   448,   449,     0,     0,   452,     0,     0,
     453,   454,   455,     0,   175,     0,   175,     0,     0,     0,
       0,   175,     0,     0,   175,     0,     0,     0,     0,     0,
       0,   176,   175,   176,   175,   184,     0,     0,   176,   181,
       0,   176,     0,     0,     0,   181,     0,     0,     0,   176,
     184,   176,     0,     0,     0,     0,     0,   181,   429,     0,
     184,   181,     0,     0,     0,   175,     0,     0,     0,     0,
       0,   175,     0,   184,     0,   175,   175,     0,     0,     0,
       0,     0,   176,     0,     0,     0,     0,     0,   176,   175,
     139,     0,   176,   176,     0,     0,   175,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   176,     0,   175,     0,
       0,     0,   175,   176,     0,     0,   430,     0,     0,     0,
     431,   183,     0,     0,   183,   176,     0,   175,     0,   176,
       0,     0,   181,     0,   181,     0,     0,   181,     0,     0,
       0,     0,   139,     0,   176,     0,     0,   139,     0,   847,
       0,   852,   856,   858,   861,   863,   864,     0,     0,     0,
       0,   181,     0,     0,     0,     0,     0,     0,   433,   434,
       0,     0,   436,   437,   181,   438,   439,     0,     0,     0,
     442,   175,   175,   175,   175,   175,   175,   449,   184,     0,
     184,     0,     0,   453,   454,   455,     0,     0,   176,   176,
     176,   176,   176,   176,     0,     0,     0,     0,     0,     0,
     877,     0,     0,     0,     0,   183,     0,     0,     0,     0,
       0,   183,     0,   139,     0,     0,     0,     0,     0,     0,
       0,  -330,     0,   183,   181,  -330,  -330,   183,   139,     0,
     181,     0,     0,     0,  -330,     0,  -330,  -330,   139,     0,
       0,     0,  -330,     0,     0,     0,     0,     0,     0,  -330,
       0,   139,  -330,     0,  -330,     0,     0,     0,   184,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -330,     0,   181,  -330,     0,  -330,     0,  -330,
       0,  -330,  -330,     0,  -330,     0,     0,  -330,     0,  -330,
       0,     0,     0,     0,     0,     0,     0,     0,   183,     0,
     183,     0,     0,   183,     0,     0,     0,     0,  -330,     0,
       0,  -330,     0,     0,  -330,     0,     0,     0,     0,     0,
       0,     0,     0,   181,     0,   181,     0,   183,     0,     0,
     181,     0,     0,   181,     0,     0,     0,     0,     0,  -330,
     183,   181,     0,   181,   856,   228,     0,     0,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,   229,
     230,     0,   231,     0,  -330,     0,   139,   232,   139,     0,
    -330,     0,     0,     0,   181,   233,     0,     0,     0,     0,
     181,   234,     0,  1023,   181,   181,     0,   235,     0,     0,
       0,   236,     0,  1032,   237,     0,     0,  1037,   181,  1038,
     183,     0,  1041,     0,   238,   181,   183,     0,     0,   430,
       0,   239,   240,   431,     0,     0,     0,   181,     0,   241,
       0,   181,     0,     0,     0,     0,     0,     0,     0,   242,
       0,     0,   184,     0,     0,   184,   181,     0,   243,   244,
       0,   245,     0,   246,   432,   247,   139,     0,   248,     0,
     183,     0,   249,   513,     0,   250,     0,     0,   251,     0,
       0,   433,   434,     0,   435,   436,   437,   429,   438,   439,
     440,     0,   441,   442,   443,   444,     0,   446,   447,   448,
     449,   450,   451,   452,     0,     0,   453,   454,   455,     0,
     181,   181,   181,   181,   181,   181,     0,   456,     0,   183,
       0,   183,     0,     0,     0,     0,   183,     0,     0,   183,
       0,     0,     0,     0,   514,     0,     0,   183,     0,   183,
       0,     0,     0,     0,     0,   430,   184,     0,     0,   431,
       0,     0,   184,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   184,     0,     0,     0,   184,     0,
     183,     0,     0,     0,     0,     0,   183,     0,     0,     0,
     183,   183,     0,     0,     0,     0,     0,     0,     0,  1143,
    1145,     0,     0,     0,   183,  1150,  1153,   433,   434,  1155,
    1157,   183,   437,     0,   438,   439,     0,     0,     0,   442,
       0,     0,     0,   183,     0,     0,   449,   183,     0,     0,
       0,     0,   453,   454,   455,     0,     0,     0,     0,     0,
       0,     0,   183,     0,     0,     0,     0,     0,     0,   184,
     139,   184,     0,   139,   184,     0,     0,     0,     0,     0,
       0,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,   468,   469,   470,   471,     0,     0,   184,  1200,
       0,     0,     0,     0,  1143,  1145,  1150,  1153,     0,     0,
       0,   184,     0,     0,     0,     0,   183,   183,   183,   183,
     183,   183,     0,     0,     0,   472,  1214,  1215,  -667,  1216,
    -667,     0,     0,     0,     0,     0,     0,     0,   228,     0,
       0,  1227,     0,     0,  1230,     0,  1231,     0,  1232,     0,
       0,     0,   229,   230,     0,   231,     0,     0,     0,     0,
     232,     0,     0,     0,   139,     0,     0,     0,   233,     0,
     139,   184,     0,     0,   234,     0,     0,   184,     0,     0,
     235,     0,   139,     0,   236,     0,   139,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,     0,
       0,     0,     0,     0,   239,   240,     0,     0,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
       0,   184,   242,     0,     0,     0,     0,     0,     0,     0,
       0,   243,   244,     0,   245,     0,   246,     0,   247,     0,
       0,   248,     0,     0,     0,   249,   540,     0,   250,     0,
     357,   251,     0,     0,    22,    23,     0,   139,     0,   139,
       0,     0,   139,   358,     0,    29,   359,     0,     0,     0,
     184,    32,   184,     0,     0,     0,     0,   184,    37,     0,
     184,     0,     0,   360,     0,     0,   139,     0,   184,     0,
     184,     0,     0,     0,     0,     0,     0,     0,     0,   139,
       0,     0,     0,     0,    53,     0,    55,   541,     0,     0,
       0,     0,     0,     0,     0,     0,   361,     0,    64,     0,
       0,   184,     0,     0,     0,     0,     0,   184,     0,     0,
       0,   184,   184,     0,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,   184,     0,     0,     0,     0,
       0,     0,   184,     0,     0,     0,     0,     0,     0,   139,
       0,     0,     0,     0,   184,   139,     0,     0,   184,     0,
       0,     0,     0,   209,     0,     0,     0,   218,     0,     0,
       0,     0,     0,   184,     0,     0,   226,     0,     0,     0,
       0,     0,     0,   259,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   274,     0,   277,   278,     0,   139,
       0,     0,   283,     0,   284,     0,   285,     0,     0,     0,
       0,   311,     0,     0,     0,   226,   319,   321,   323,   324,
     325,     0,     0,     0,   329,     0,   330,   184,   184,   184,
     184,   184,   184,   336,     0,     0,   319,     0,     0,   339,
       0,   340,     0,     0,   341,   342,     0,   343,   139,     0,
     139,   226,   319,   354,     0,   139,     0,     0,   139,     0,
       0,     0,     0,     0,     0,     0,   139,     0,   139,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     379,     0,   382,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   139,
       0,     0,     0,     0,     0,   139,     0,     0,     0,   139,
     139,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   139,   426,     0,     0,     0,   272,     0,
     139,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   139,     0,     0,     0,   139,   478,     0,     0,
     329,   226,   272,     0,     0,     0,     0,     0,     0,     0,
       0,   139,   333,   335,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   319,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   228,     0,   367,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   229,   230,     0,   231,
       0,     0,     0,   319,   232,   139,   139,   139,   139,   139,
     139,     0,   233,     0,     0,     0,     0,     0,   234,     0,
       0,     0,     0,     0,   235,     0,     0,   319,   236,     0,
       0,   237,     0,     0,     0,     0,     0,     0,   325,   329,
       0,   238,     0,     0,     0,     0,     0,     0,   239,   240,
       0,     0,     0,   546,   547,     0,   241,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   242,   319,     0,     0,
       0,     0,     0,     0,     0,   243,   244,     0,   245,     0,
     246,     0,   247,     0,     0,   248,     0,     0,     0,   249,
     564,     0,   250,   487,     0,   251,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   583,     0,   585,     0,     0,     0,     0,
       0,     0,     0,     0,   595,     0,   600,   602,   604,   606,
     610,     0,   501,   504,     0,   226,     0,     0,     0,   511,
       0,     0,     0,     0,     0,     0,     0,     0,   272,   362,
     272,   975,     0,     0,     0,     0,     0,   272,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   682,   683,   684,
     686,   688,   689,     0,   690,   691,   692,   693,   694,     0,
     702,   703,   704,   705,   706,   707,   708,   709,   710,   711,
     712,   713,   714,   715,   716,   717,   718,   272,     0,     0,
       0,     0,     0,   319,   319,     0,     0,     0,     0,   319,
       0,     0,   319,   319,   319,   319,   728,   730,     0,     0,
     733,   734,   610,     0,   744,     0,     0,     0,     0,     0,
     747,     0,   106,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   756,     0,     0,   758,     0,     0,     0,   319,
       0,     0,     0,     0,     0,   224,     0,     0,     0,   769,
       0,   772,     0,     0,     0,   277,     0,     0,   777,   778,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   311,
       0,     0,     0,     0,   311,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,     0,     0,     0,     0,     0,
       0,     0,   747,     0,   328,   319,     0,   319,     0,     0,
       0,   803,   804,   226,     0,     0,   319,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   835,   835,   228,
       0,   106,     0,     0,     0,   356,     0,     0,     0,     0,
       0,     0,     0,   229,   230,     0,   231,   865,     0,     0,
       0,   232,     0,     0,     0,     0,     0,   879,     0,   233,
       0,     0,     0,     0,     0,   234,     0,   311,     0,     0,
       0,   235,     0,     0,     0,   236,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
       0,     0,     0,     0,     0,   239,   240,     0,     0,     0,
       0,     0,     0,   241,     0,     0,     0,     0,   429,     0,
     893,     0,     0,   242,     0,     0,     0,   898,     0,     0,
       0,     0,   243,   244,     0,   245,     0,   246,     0,   247,
       0,     0,   248,     0,     0,     0,   249,     0,     0,   250,
       0,     0,   251,     0,   485,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   430,     0,     0,     0,
     431,   756,   319,   272,   272,   272,   272,   272,   272,   272,
     272,     0,     0,     0,     0,     0,     0,     0,   319,     0,
     319,   939,   940,   427,   527,     0,   512,     0,   267,     0,
     428,   523,     0,     0,     0,     0,     0,     0,     0,     0,
     319,     0,   429,     0,     0,     0,     0,     0,   433,   434,
       0,   435,   436,   437,   311,   438,   439,   440,   968,   441,
     442,   443,   444,     0,   446,   447,   448,   449,   450,     0,
     452,     0,   319,   453,   454,   455,     0,     0,     0,     0,
       0,     0,     0,   583,   456,     0,     0,  1008,     0,  1010,
     835,   835,     0,   339,     0,   340,     0,     0,   342,   343,
     430,     0,     0,     0,   431,     0,     0,   106,     0,     0,
       0,     0,  1022,     0,  1024,     0,   226,     0,  1028,  1030,
       0,     0,   106,     0,  1034,  1036,     0,     0,     0,     0,
    1040,     0,   106,     0,     0,   432,     0,     0,  1045,   747,
    1048,     0,  1050,  1052,     0,   596,   319,     0,     0,     0,
       0,     0,   433,   434,     0,   435,   436,   437,     0,   438,
     439,   440,   947,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,     0,     0,   453,   454,   455,
       0,  1028,  1078,  1034,  1081,  1083,  1048,   319,   456,     0,
     744,     0,     0,     0,     0,     0,     0,   429,   976,     0,
       0,     0,     0,     0,   686,   728,     0,   272,   272,     0,
       0,     0,     0,     0,  1091,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   311,     0,     0,     0,  1106,     0,     0,     0,     0,
       0,   226,     0,     0,     0,     0,     0,   272,     0,     0,
    1115,  1117,  1119,     0,   319,   430,     0,   272,     0,   431,
     753,   272,     0,   272,     0,     0,   272,     0,     0,     0,
       0,     0,     0,     0,     0,   835,     0,     0,     0,     0,
       0,     0,     0,  1130,     0,  1131,     0,     0,     0,     0,
     432,     0,     0,     0,     0,  1139,     0,  1141,     0,     0,
       0,   744,     0,  1148,     0,     0,     0,   433,   434,     0,
     435,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,   444,     0,   446,   447,   448,   449,   450,     0,   452,
       0,     0,   453,   454,   455,     0,  1178,     0,     0,     0,
     106,     0,     0,   456,     0,     0,  1183,     0,     0,  1186,
       0,     0,     0,     0,     0,  1097,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   319,
     319,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1202,  1112,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1207,   835,   835,   835,   835,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1219,     0,  1221,     0,     0,     0,     0,  1226,
       0,     0,  1229,     0,     0,   253,     0,     0,     0,     0,
    1235,     0,  1239,   272,   272,  1241,   264,   265,   266,   272,
     272,     0,     0,   272,   272,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1219,  1221,     0,  1226,  1229,  1235,
    1239,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1278,     0,     0,     0,     0,  1279,  1280,     0,
    1281,     0,   744,     0,  1283,  1284,     0,     0,  1285,     0,
       0,     0,     0,  1286,     0,   429,     0,  1288,     0,     0,
       0,     0,     0,   272,   942,     0,  1291,   945,   272,   272,
     272,   272,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1279,  1301,  1302,  1283,  1303,  1304,  1305,  1306,     0,
     272,   272,     0,   272,     0,   504,     0,   397,     0,   400,
       0,     0,   504,     0,     0,   272,     0,     0,   272,     0,
     272,   420,   272,   430,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1321,
    1322,  1323,  1324,  1325,  1326,  1321,  1322,  1323,  1324,  1325,
    1326,     0,     0,     0,     0,   479,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1273,     0,     0,   523,     0,
       0,     0,     0,     0,   523,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,  1042,   442,   443,   444,
    1047,   446,   447,   448,   449,     0,     0,   452,     0,     0,
     453,   454,   455,     0,     0,     0,     0,     0,     0,     0,
       0,   456,     0,   498,   499,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   515,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   504,   504,     0,     0,   504,   504,
     504,     0,   504,     0,     0,     0,     0,     0,     0,     0,
       0,  1089,     0,  1090,   542,     0,  1092,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1107,     0,   885,     0,     5,     6,     7,     8,     9,    10,
      11,    12,     0,  1047,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,   253,    21,   570,     0,     0,
       0,     0,     0,     0,   291,     0,   196,     0,     0,     0,
      31,  -213,  -213,     0,     0,   197,    34,     0,     0,     0,
     293,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,   612,     0,    45,     0,    47,   400,    48,    49,     0,
      50,    51,     0,  1159,     0,     0,     0,     0,    54,  1164,
      56,     0,    58,     0,     0,     0,   671,    61,    62,   199,
       0,     0,     0,     0,     0,     0,     0,    69,   298,    71,
      72,    73,   299,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,  1194,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
     736,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,   523,   102,   523,  -213,     0,   762,   103,   523,
       0,   515,   523,     0,     0,     0,     0,     0,     0,     0,
    1234,     0,  1238,     0,   775,     0,     0,     0,     0,     0,
       0,     0,     0,   542,     0,     0,     0,     0,   515,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1269,     0,     0,     0,     0,     0,  1272,
       0,     0,     0,  1234,  1238,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   822,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   845,     0,  1287,     0,     0,     0,
    1289,     0,    -2,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,   106,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,  -213,    21,    22,    23,
      24,    25,    26,     0,  -213,    27,     0,    28,     0,    29,
      30,    31,  -213,  -213,  -213,    32,    33,    34,   -68,     0,
      35,    36,    37,     0,    38,  -350,     0,    39,    40,    41,
      42,    43,     0,    44,    45,    46,    47,   -68,    48,    49,
       0,    50,    51,    52,     0,  -350,   420,     0,    53,    54,
      55,    56,    57,    58,    59,  -350,   -68,    60,    61,    62,
      63,     0,    64,    65,    66,     0,    67,    68,    69,    70,
      71,    72,    73,    74,     0,    75,    76,     0,    77,    78,
      79,    80,    81,   427,    82,    83,   -68,    84,    85,     0,
     428,  -213,     0,    86,     0,     0,    87,     0,     0,     0,
       0,     0,   429,   271,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,    93,   860,     0,
       0,     0,    94,     0,     0,     0,   948,     0,     0,     0,
       0,    95,     0,   515,    96,    97,    98,    99,     0,     0,
     100,     0,   101,   957,   102,     0,  -213,     0,     0,   103,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,   977,     0,     0,   431,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   851,     0,     0,     0,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   420,     0,     0,
       0,     0,   433,   434,     0,   435,   436,   437,     0,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,     0,     0,   453,   454,   455,
       0,   362,     0,     0,     0,     0,     0,   400,   456,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   671,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     0,     5,
       6,     7,     8,     9,    10,    11,    12,   762,  -698,     0,
      13,    14,    15,    16,    17,  -698,    18,     0,    19,    20,
    -213,    21,    22,    23,    24,    25,    26,  -698,  -213,    27,
    -698,    28,   977,    29,    30,    31,  -213,  -213,  -213,    32,
      33,    34,   -68,     0,    35,    36,    37,     0,    38,  -350,
       0,    39,    40,    41,    42,    43,     0,    44,    45,    46,
      47,   -68,    48,    49,     0,    50,    51,    52,     0,  -350,
       0,     0,    53,    54,    55,    56,     0,    58,    59,  -350,
     -68,    60,    61,    62,    63,  -698,    64,    65,    66,  -698,
      67,    68,    69,    70,    71,    72,    73,    74,     0,    75,
      76,     0,    77,    78,    79,    80,    81,     0,    82,    83,
     -68,    84,    85,     0,     0,  -213,     0,    86,     0,   400,
    -698,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,  -698,    91,
    -698,  -698,  -698,  -698,  -698,  -698,  -698,     0,  -698,  -698,
    -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,
      98,    99,  -698,  -698,  -698,     0,   101,  -698,   102,     0,
    -213,     0,   344,  -698,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -213,    21,    22,    23,    24,
      25,    26,     0,  -213,    27,     0,    28,     0,    29,    30,
      31,  -213,  -213,  -213,    32,    33,    34,   -68,     0,    35,
      36,    37,     0,    38,  -350,     0,    39,    40,    41,    42,
      43,     0,    44,    45,    46,    47,   -68,    48,    49,     0,
      50,    51,    52,     0,  -350,     0,     0,    53,    54,    55,
      56,    57,    58,    59,  -350,   -68,    60,    61,    62,    63,
       0,    64,    65,    66,     0,    67,    68,    69,    70,    71,
      72,    73,    74,     0,    75,    76,     0,    77,    78,    79,
      80,    81,     0,    82,    83,   -68,    84,    85,     0,     0,
    -213,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,    99,     0,     0,   100,
       0,   101,   345,   102,     0,  -213,     0,     4,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -213,    21,    22,    23,    24,    25,    26,     0,  -213,    27,
       0,    28,     0,    29,    30,    31,  -213,  -213,  -213,    32,
      33,    34,   -68,     0,    35,    36,    37,     0,    38,  -350,
       0,    39,    40,    41,    42,    43,     0,    44,    45,    46,
      47,   -68,    48,    49,     0,    50,    51,    52,     0,  -350,
       0,     0,    53,    54,    55,    56,    57,    58,    59,  -350,
     -68,    60,    61,    62,    63,     0,    64,    65,    66,     0,
      67,    68,    69,    70,    71,    72,    73,    74,     0,    75,
      76,     0,    77,    78,    79,    80,    81,     0,    82,    83,
     -68,    84,    85,     0,     0,  -213,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,    99,     0,     0,   100,     0,   101,   560,   102,     0,
    -213,     0,   578,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -213,    21,    22,    23,    24,
      25,    26,     0,  -213,    27,     0,    28,     0,    29,    30,
      31,  -213,  -213,  -213,    32,    33,    34,   -68,     0,    35,
      36,    37,     0,    38,  -350,     0,    39,    40,    41,    42,
      43,     0,    44,    45,    46,    47,   -68,    48,    49,     0,
      50,    51,    52,     0,  -350,     0,     0,    53,    54,    55,
      56,    57,    58,    59,  -350,   -68,    60,    61,    62,    63,
       0,    64,    65,    66,     0,    67,    68,    69,    70,    71,
      72,    73,    74,     0,    75,    76,     0,    77,    78,    79,
      80,    81,     0,    82,    83,   -68,    84,    85,     0,     0,
    -213,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,    99,     0,     0,   100,
       0,   101,   579,   102,     0,  -213,     0,     4,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -213,    21,    22,    23,    24,    25,    26,     0,  -213,    27,
       0,    28,     0,    29,    30,    31,  -213,  -213,  -213,    32,
      33,    34,   -68,     0,    35,    36,    37,     0,    38,  -350,
       0,    39,    40,    41,    42,    43,     0,    44,    45,    46,
      47,   -68,    48,    49,     0,    50,    51,    52,     0,  -350,
       0,     0,    53,    54,    55,    56,    57,    58,    59,  -350,
     -68,    60,    61,    62,    63,     0,    64,    65,    66,     0,
      67,    68,    69,    70,    71,    72,    73,    74,     0,    75,
      76,     0,    77,    78,    79,    80,    81,     0,    82,    83,
     -68,    84,    85,     0,     0,  -213,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,    99,     0,     0,   100,     0,   101,   812,   102,     0,
    -213,     0,     4,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -213,    21,    22,    23,    24,
      25,    26,     0,  -213,    27,     0,    28,     0,    29,    30,
      31,  -213,  -213,  -213,    32,    33,    34,   -68,     0,    35,
      36,    37,     0,    38,  -350,     0,    39,    40,    41,    42,
      43,     0,    44,    45,    46,    47,   -68,    48,    49,     0,
      50,    51,    52,     0,  -350,     0,     0,    53,    54,    55,
      56,   355,    58,    59,  -350,   -68,    60,    61,    62,    63,
       0,    64,    65,    66,     0,    67,    68,    69,    70,    71,
      72,    73,    74,     0,    75,    76,     0,    77,    78,    79,
      80,    81,     0,    82,    83,   -68,    84,    85,     0,     0,
    -213,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,    99,     0,     0,   100,
       0,   101,     0,   102,     0,  -213,     0,     4,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -213,    21,    22,    23,    24,    25,    26,     0,  -213,    27,
       0,    28,     0,    29,    30,    31,  -213,  -213,  -213,    32,
      33,    34,   -68,     0,    35,    36,    37,     0,    38,  -350,
       0,    39,    40,    41,    42,    43,     0,    44,    45,    46,
      47,   -68,    48,    49,     0,    50,    51,    52,     0,  -350,
       0,     0,    53,    54,    55,    56,    57,    58,    59,  -350,
     -68,    60,    61,    62,    63,     0,    64,    65,    66,     0,
      67,    68,    69,    70,    71,    72,    73,    74,     0,    75,
      76,     0,    77,    78,    79,    80,    81,     0,    82,    83,
     -68,    84,    85,     0,     0,  -213,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,    99,     0,     0,   100,     0,   101,     0,   102,     0,
    -213,     0,     4,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -213,    21,    22,    23,    24,
      25,    26,     0,  -213,    27,     0,    28,     0,    29,    30,
      31,  -213,  -213,  -213,    32,    33,    34,   -68,     0,    35,
      36,    37,     0,    38,  -350,     0,    39,    40,    41,    42,
      43,     0,    44,    45,    46,    47,   -68,    48,    49,     0,
      50,    51,    52,     0,  -350,     0,     0,    53,    54,    55,
      56,     0,    58,    59,  -350,   -68,    60,    61,    62,    63,
       0,    64,    65,    66,     0,    67,    68,    69,    70,    71,
      72,    73,    74,     0,    75,    76,     0,    77,    78,    79,
      80,    81,     0,    82,    83,   -68,    84,    85,     0,     0,
    -213,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,    99,     0,     0,   100,
       0,   101,     0,   102,     0,  -213,     0,     4,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -213,    21,    22,    23,    24,    25,    26,     0,  -213,    27,
       0,    28,     0,    29,    30,    31,  -213,  -213,  -213,    32,
    1027,    34,   -68,     0,    35,    36,    37,     0,    38,  -350,
       0,    39,    40,    41,    42,    43,     0,    44,    45,    46,
      47,   -68,    48,    49,     0,    50,    51,    52,     0,  -350,
       0,     0,    53,    54,    55,    56,     0,    58,    59,  -350,
     -68,    60,    61,    62,    63,     0,    64,    65,    66,     0,
      67,    68,    69,    70,    71,    72,    73,    74,     0,    75,
      76,     0,    77,    78,    79,    80,    81,     0,    82,    83,
     -68,    84,    85,     0,     0,  -213,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,    99,     0,     0,   100,     0,   101,     0,   102,     0,
    -213,     0,     4,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -213,    21,    22,    23,    24,
      25,    26,     0,  -213,    27,     0,    28,     0,    29,    30,
      31,  -213,  -213,  -213,    32,  1033,    34,   -68,     0,    35,
      36,    37,     0,    38,  -350,     0,    39,    40,    41,    42,
      43,     0,    44,    45,    46,    47,   -68,    48,    49,     0,
      50,    51,    52,     0,  -350,     0,     0,    53,    54,    55,
      56,     0,    58,    59,  -350,   -68,    60,    61,    62,    63,
       0,    64,    65,    66,     0,    67,    68,    69,    70,    71,
      72,    73,    74,     0,    75,    76,     0,    77,    78,    79,
      80,    81,     0,    82,    83,   -68,    84,    85,     0,     0,
    -213,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,    99,     0,     0,   100,
       0,   101,     0,   102,     0,  -213,     0,     4,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -213,    21,    22,    23,    24,    25,    26,     0,  -213,    27,
       0,    28,     0,    29,    30,    31,  -213,  -213,  -213,    32,
    1218,    34,   -68,     0,    35,    36,    37,     0,    38,  -350,
       0,    39,    40,    41,    42,    43,     0,    44,    45,    46,
      47,   -68,    48,    49,     0,    50,    51,    52,     0,  -350,
       0,     0,    53,    54,    55,    56,     0,    58,    59,  -350,
     -68,    60,    61,    62,    63,     0,    64,    65,    66,     0,
      67,    68,    69,    70,    71,    72,    73,    74,     0,    75,
      76,     0,    77,    78,    79,    80,    81,     0,    82,    83,
     -68,    84,    85,     0,     0,  -213,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,    99,     0,     0,   100,     0,   101,     0,   102,     0,
    -213,     0,     4,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -213,    21,    22,    23,    24,
      25,    26,     0,  -213,    27,     0,    28,     0,    29,    30,
      31,  -213,  -213,  -213,    32,  1220,    34,   -68,     0,    35,
      36,    37,     0,    38,  -350,     0,    39,    40,    41,    42,
      43,     0,    44,    45,    46,    47,   -68,    48,    49,     0,
      50,    51,    52,     0,  -350,     0,     0,    53,    54,    55,
      56,     0,    58,    59,  -350,   -68,    60,    61,    62,    63,
       0,    64,    65,    66,     0,    67,    68,    69,    70,    71,
      72,    73,    74,     0,    75,    76,     0,    77,    78,    79,
      80,    81,     0,    82,    83,   -68,    84,    85,     0,     0,
    -213,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,    99,     0,     0,   100,
       0,   101,     0,   102,     0,  -213,     0,     4,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -213,    21,    22,    23,    24,    25,    26,     0,  -213,    27,
       0,    28,     0,    29,    30,    31,  -213,  -213,  -213,    32,
    1225,    34,   -68,     0,    35,    36,    37,     0,    38,  -350,
       0,    39,    40,    41,    42,    43,     0,    44,    45,    46,
      47,   -68,    48,    49,     0,    50,    51,    52,     0,  -350,
       0,     0,    53,    54,    55,    56,     0,    58,    59,  -350,
     -68,    60,    61,    62,    63,     0,    64,    65,    66,     0,
      67,    68,    69,    70,    71,    72,    73,    74,     0,    75,
      76,     0,    77,    78,    79,    80,    81,     0,    82,    83,
     -68,    84,    85,     0,     0,  -213,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,    99,     0,     0,   100,     0,   101,     0,   102,     0,
    -213,     0,     4,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -213,    21,    22,    23,    24,
      25,    26,     0,  -213,    27,     0,    28,     0,    29,    30,
      31,  -213,  -213,  -213,    32,  1228,    34,   -68,     0,    35,
      36,    37,     0,    38,  -350,     0,    39,    40,    41,    42,
      43,     0,    44,    45,    46,    47,   -68,    48,    49,     0,
      50,    51,    52,     0,  -350,     0,     0,    53,    54,    55,
      56,     0,    58,    59,  -350,   -68,    60,    61,    62,    63,
       0,    64,    65,    66,     0,    67,    68,    69,    70,    71,
      72,    73,    74,     0,    75,    76,     0,    77,    78,    79,
      80,    81,     0,    82,    83,   -68,    84,    85,     0,     0,
    -213,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,    99,     0,     0,   100,
       0,   101,     0,   102,     0,  -213,     0,     4,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -213,    21,    22,    23,    24,    25,    26,     0,  -213,    27,
       0,    28,     0,    29,    30,    31,  -213,  -213,  -213,    32,
    1233,    34,   -68,     0,    35,    36,    37,     0,    38,  -350,
       0,    39,    40,    41,    42,    43,     0,    44,    45,    46,
      47,   -68,    48,    49,     0,    50,    51,    52,     0,  -350,
       0,     0,    53,    54,    55,    56,     0,    58,    59,  -350,
     -68,    60,    61,    62,    63,     0,    64,    65,    66,     0,
      67,    68,    69,    70,    71,    72,    73,    74,     0,    75,
      76,     0,    77,    78,    79,    80,    81,     0,    82,    83,
     -68,    84,    85,     0,     0,  -213,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,    99,     0,     0,   100,     0,   101,     0,   102,     0,
    -213,     0,     4,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -213,    21,    22,    23,    24,
      25,    26,     0,  -213,    27,     0,    28,     0,    29,    30,
      31,  -213,  -213,  -213,    32,  1237,    34,   -68,     0,    35,
      36,    37,     0,    38,  -350,     0,    39,    40,    41,    42,
      43,     0,    44,    45,    46,    47,   -68,    48,    49,     0,
      50,    51,    52,     0,  -350,     0,     0,    53,    54,    55,
      56,     0,    58,    59,  -350,   -68,    60,    61,    62,    63,
       0,    64,    65,    66,     0,    67,    68,    69,    70,    71,
      72,    73,    74,     0,    75,    76,     0,    77,    78,    79,
      80,    81,     0,    82,    83,   -68,    84,    85,     0,     0,
    -213,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,    99,     0,     0,   100,
       0,   101,     0,   102,     0,  -213,     0,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,   633,     0,
      13,     0,     0,    16,    17,   635,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     640,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   200,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,   503,   458,   459,   460,   461,   462,     0,     0,   465,
     466,   467,   468,     0,   470,   471,     0,   823,   824,   825,
     826,   827,   653,     0,   654,     0,    94,     0,   655,   656,
     657,   658,   659,   660,   661,   828,   663,   664,    96,   829,
      98,   -92,   666,   667,   830,   669,   201,     0,   102,     0,
       0,     0,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,   633,     0,    13,     0,     0,    16,    17,
     635,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   640,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,   503,   458,   459,   460,
     461,   462,     0,     0,   465,   466,   467,   468,     0,   470,
     471,     0,   823,   824,   825,   826,   827,   653,     0,   654,
       0,    94,     0,   655,   656,   657,   658,   659,   660,   661,
     828,   663,   664,    96,   829,    98,     0,   666,   667,   830,
     669,   201,     0,   102,     0,     0,     0,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,     0,    16,    17,     0,    18,     0,   195,    20,
       0,    21,     0,     0,     0,     0,    26,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
      33,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,    44,    45,    46,
      47,     0,    48,    49,     0,    50,    51,    52,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,    72,    73,    74,     0,    75,
      76,     0,    77,     0,     0,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   101,     0,   102,     0,
       0,     0,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,   377,    23,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,    55,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
       0,    64,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,     0,    83,     0,    84,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,     0,     0,     0,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,  -742,     0,     0,     0,  -742,
       0,     0,    69,    70,    71,    72,    73,   200,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,  -392,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,  -392,     0,     0,   100,     0,   201,  -392,   102,     0,
       0,     0,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,   313,   314,
       0,    81,   349,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,   350,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,     0,     0,     0,     0,   103,     5,
       6,     7,     8,     9,   380,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,    19,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,  -213,  -213,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,    63,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   200,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,    82,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   201,     0,   102,     0,
    -213,     0,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,   737,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,   738,     0,
      36,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
       0,   739,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,     0,    83,     0,   740,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,     0,     0,     0,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   200,     0,    75,
       0,     0,    77,   313,   314,     0,    81,   349,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   201,     0,   102,   798,
       0,     0,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,   313,   314,
       0,    81,   349,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,   800,     0,     0,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   200,     0,    75,
       0,     0,    77,   313,   314,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,   315,     0,     0,   100,     0,   201,     0,   102,     0,
       0,     0,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,   313,   314,
       0,    81,     0,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,  -543,     0,     0,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,  -213,  -213,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   200,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   201,     0,   102,     0,
    -213,     0,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,   313,   314,
       0,    81,     0,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,     0,     0,  -543,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   200,     0,    75,
       0,     0,    77,   313,   314,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   201,     0,   102,   807,
       0,     0,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,   955,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,   313,   314,
       0,    81,     0,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,     0,     0,     0,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   200,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,  -400,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,  -400,     0,     0,   100,     0,   201,  -400,   102,     0,
       0,     0,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,  -385,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,  -385,     0,     0,   100,
       0,   201,  -385,   102,     0,     0,     0,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   200,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,  -393,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,  -393,     0,     0,   100,     0,   201,  -393,   102,     0,
       0,     0,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,   256,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,     0,    83,     0,   257,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,     0,     0,     0,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   200,     0,    75,
       0,     0,    77,   313,   314,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   201,     0,   102,     0,
       0,     0,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
    -742,     0,     0,     0,  -742,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,     0,     0,     0,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,   597,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   200,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,   598,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   201,     0,   102,     0,
       0,     0,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,   598,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,     0,     0,   607,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   200,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,  -421,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   201,     0,   102,  -421,
       0,     0,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,   271,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   101,     0,   102,     0,     0,     0,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   200,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,   598,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   201,     0,   102,     0,
       0,     0,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,     0,     0,   607,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   200,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   201,   796,   102,     0,
       0,     0,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,     0,     0,
       0,    81,  1009,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,     0,     0,     0,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   200,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   201,     0,   102,     0,
       0,  1046,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,     0,     0,  1051,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   200,     0,    75,
       0,     0,    77,     0,     0,     0,    81,  1206,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   201,     0,   102,     0,
       0,     0,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,     0,     0,     0,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,   291,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,   293,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,     0,     0,     0,     0,     0,
       0,     0,    69,   298,    71,    72,    73,   299,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   201,     0,   102,     0,
       0,     0,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,  1076,    34,     0,     0,     0,
      36,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,     0,     0,     0,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
    1079,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   200,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   201,     0,   102,     0,
       0,     0,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,  1262,    34,     0,     0,     0,
      36,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,     0,     0,     0,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
    1263,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   200,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   201,     0,   102,     0,
       0,     0,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,  1265,    34,     0,     0,     0,
      36,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,     0,     0,     0,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
    1266,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   200,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   201,     0,   102,     0,
       0,     0,     0,   103,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,  1267,    34,     0,     0,     0,
      36,     0,     0,    38,     0,     0,   198,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   199,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   200,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,     0,     0,     0,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
    1268,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   198,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   199,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   200,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     6,     7,     8,
       9,    10,    11,    12,     0,    95,     0,    13,    96,    97,
      98,    17,     0,     0,   100,   195,   201,     0,   102,     0,
     290,     0,     0,   103,     0,     0,     0,     0,   196,     0,
       0,     0,     0,  -213,  -213,     0,     0,   197,     0,     0,
     292,     0,     0,     0,   294,     0,     0,     0,   198,     0,
       0,    42,    43,     0,     0,     0,     0,    47,     0,    48,
      49,     0,     0,     0,     0,     0,     0,     0,   295,     0,
      54,   296,    56,    57,    58,     0,     0,     0,     0,    61,
       0,   199,     0,   297,     0,     0,     0,     0,     0,    69,
       0,    71,     0,    73,     0,     0,     0,     0,     0,     0,
       0,     0,   300,     0,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,    91,    92,    93,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     0,    96,    97,    98,     0,     0,
       0,   100,     0,   201,     0,   102,  -354,  -213,     0,     0,
     103,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,     0,    17,     0,     0,     0,   195,
       0,     0,     0,     0,   290,     0,     0,     0,     0,     0,
       0,     0,   196,     0,     0,     0,     0,  -213,  -213,     0,
       0,   197,     0,     0,   292,     0,     0,     0,   294,     0,
       0,     0,   198,     0,     0,    42,    43,     0,     0,     0,
       0,    47,     0,    48,    49,     0,     0,     0,     0,     0,
       0,     0,   295,     0,    54,   296,    56,    57,    58,     0,
       0,     0,     0,    61,     0,   199,     0,   297,     0,     0,
       0,     0,     0,    69,     0,    71,     0,    73,     0,     0,
       0,     0,     0,     0,     0,     0,   300,     0,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   631,     0,   632,     0,    89,    90,
      91,    92,    93,     0,     0,   633,     0,    94,     0,     0,
     634,   230,   635,   636,     0,     0,    95,     0,   637,    96,
      97,    98,     0,     0,     0,   100,   233,   201,   196,   102,
       0,  -213,   234,     0,   103,     0,     0,     0,   638,     0,
       0,     0,   236,     0,     0,   639,     0,   640,     0,     0,
       0,     0,     0,     0,     0,   641,     0,     0,     0,     0,
       0,     0,   239,   642,     0,     0,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     643,     0,     0,     0,     0,     0,     0,     0,     0,   243,
     244,     0,   644,     0,   246,     0,   645,     0,     0,   248,
       0,     0,     0,   646,     0,     0,   250,     0,     0,   647,
       0,     0,     0,     0,     0,     0,     0,     0,   503,   458,
     459,   460,   461,   462,     0,     0,   465,   466,   467,   468,
       0,   470,   471,     0,   648,   649,   650,   651,   652,   653,
       0,   654,     0,     0,     0,   655,   656,   657,   658,   659,
     660,   661,   662,   663,   664,     0,   665,     0,     0,   666,
     667,   668,   669,     0,     0,   670,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   195,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   196,     0,
       0,     0,    31,  -213,  -213,     0,     0,   197,    34,     0,
       0,     0,    36,     0,     0,    38,     0,     0,   198,     0,
       0,    42,    43,     0,     0,    45,     0,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
      62,   199,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,    73,   200,     0,    75,     0,     0,    77,
       0,     0,     0,    81,     0,     0,    83,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,    91,    92,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     0,     0,    97,    98,     0,     0,
       0,   100,     0,   201,     0,   102,     0,  -213,     5,     6,
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
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,     0,    91,     0,
       0,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,    98,
     195,     0,     0,    21,     0,   201,     0,   102,     0,  -213,
       0,    27,     0,   196,     0,     0,     0,    31,  -213,  -213,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,     0,   198,     0,     0,    42,    43,     0,     0,
      45,     0,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,    62,   199,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,    73,   200,
       0,    75,     0,     0,    77,     0,   695,     0,    81,     0,
       0,    83,     0,     0,    85,   633,     0,     0,     0,     0,
     988,   989,   635,   990,     0,     0,     0,     0,   991,     0,
       0,     0,     0,     0,     0,     0,   697,     0,     0,    89,
       0,    91,     0,     0,     0,     0,     0,     0,   992,     0,
       0,     0,   993,     0,     0,   994,     0,   640,     0,     0,
       0,     0,    98,     0,     0,   698,     0,     0,     0,     0,
     102,     0,  -213,   995,     0,     0,     0,     0,     0,     0,
     996,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     997,     0,     0,     0,     0,     0,     0,     0,     0,   998,
     999,     0,  1000,     0,  1001,     0,   699,     0,     0,     0,
       0,     0,   700,  1002,     0,     0,  1003,     0,     0,  1004,
       0,     0,     0,     0,     0,     0,     0,     0,   503,   458,
     459,   460,   461,   462,     0,     0,   465,   466,   467,   468,
       0,   470,   471,     0,   648,   649,   650,   651,   652,   653,
       0,   654,     0,     0,     0,   655,   656,   657,   658,   659,
     660,   661,   662,   663,   664,   228,   665,     0,     0,   666,
     667,   668,   669,  1005,   633,     0,     0,     0,     0,   229,
     230,   635,   231,     0,     0,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,   234,     0,     0,     0,     0,     0,   235,     0,     0,
       0,   236,     0,     0,   237,     0,   640,     0,     0,     0,
       0,     0,     0,     0,   238,     0,     0,     0,     0,     0,
       0,   239,   240,     0,     0,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   242,
       0,     0,     0,     0,     0,   427,     0,     0,   243,   244,
       0,   245,   428,   246,     0,   247,     0,     0,   248,     0,
       0,     0,   249,     0,   429,   250,     0,     0,   251,     0,
       0,     0,     0,     0,     0,     0,     0,   503,   458,   459,
     460,   461,   462,     0,     0,   465,   466,   467,   468,     0,
     470,   471,     0,   648,   649,   650,   651,   652,   653,     0,
     654,     0,     0,     0,   655,   656,   657,   658,   659,   660,
     661,   662,   663,   664,     0,   665,     0,     0,   666,   667,
     668,   669,   430,     0,     0,   427,   431,     0,     0,     0,
       0,     0,   428,     0,     0,   502,     0,     0,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   503,   458,
     459,   460,   461,   462,     0,     0,   465,   466,   467,   468,
       0,   470,   471,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,   430,   362,     0,   427,   431,     0,     0,     0,
     456,     0,   428,     0,     0,  1217,     0,     0,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   503,   458,
     459,   460,   461,   462,     0,     0,   465,   466,   467,   468,
       0,   470,   471,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,   430,   362,     0,   427,   431,     0,     0,     0,
     456,     0,   428,     0,     0,  1224,     0,     0,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   503,   458,
     459,   460,   461,   462,     0,     0,   465,   466,   467,   468,
       0,   470,   471,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,   430,   362,     0,   427,   431,     0,     0,     0,
     456,     0,   428,     0,     0,  1309,     0,     0,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   503,   458,
     459,   460,   461,   462,     0,     0,   465,   466,   467,   468,
       0,   470,   471,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,   430,   362,     0,   427,   431,     0,     0,     0,
     456,     0,   428,     0,     0,  1310,     0,     0,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   503,   458,
     459,   460,   461,   462,     0,     0,   465,   466,   467,   468,
       0,   470,   471,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,   430,   362,     0,   427,   431,     0,     0,     0,
     456,     0,   428,     0,     0,  1311,     0,     0,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   503,   458,
     459,   460,   461,   462,     0,     0,   465,   466,   467,   468,
       0,   470,   471,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,   430,   362,     0,   427,   431,     0,     0,     0,
     456,     0,   428,     0,     0,  1312,     0,     0,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   503,   458,
     459,   460,   461,   462,     0,     0,   465,   466,   467,   468,
       0,   470,   471,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,   430,   362,     0,   427,   431,     0,     0,     0,
     456,     0,   428,     0,     0,  1313,     0,     0,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   503,   458,
     459,   460,   461,   462,     0,     0,   465,   466,   467,   468,
       0,   470,   471,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,   430,   362,     0,     0,   431,     0,     0,     0,
     456,     0,     0,     0,     0,  1314,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   427,     0,     0,     0,     0,
       0,     0,   428,     0,     0,     0,     0,   432,   503,   458,
     459,   460,   461,   462,   429,   271,   465,   466,   467,   468,
       0,   470,   471,     0,   433,   434,     0,   435,   436,   437,
     981,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,     0,   362,     0,     0,     0,     0,     0,     0,
     456,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   427,     0,     0,
       0,     0,     0,     0,   428,     0,     0,     0,     0,     0,
       0,     0,     0,   851,     0,     0,   429,   432,     0,     0,
       0,   587,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   588,     0,   453,
     454,   455,     0,   362,     0,     0,     0,     0,     0,     0,
     456,     0,     0,     0,   430,     0,     0,     0,   431,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   427,
     819,     0,     0,     0,     0,     0,   428,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   429,   432,
       0,     0,     0,   820,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   821,
     589,   453,   454,   455,     0,     0,     0,     0,     0,     0,
       0,     0,   456,     0,     0,   427,   430,     0,     0,     0,
     431,     0,   428,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   429,   848,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     849,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   427,   430,   453,   454,   455,   431,     0,   428,     0,
       0,     0,     0,     0,   456,     0,     0,     0,     0,     0,
     429,   271,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   980,   432,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   427,   430,   453,
     454,   455,   431,   362,   428,     0,     0,     0,     0,     0,
     456,     0,     0,     0,     0,     0,   429,  1152,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   427,   430,   453,   454,   455,   431,   362,
     428,     0,     0,     0,     0,     0,   456,     0,     0,     0,
       0,     0,   429,   271,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   851,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   427,
     430,   453,   454,   455,   431,   362,   428,     0,     0,     0,
       0,     0,   456,     0,     0,     0,     0,     0,   429,   271,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   851,     0,     0,     0,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,   434,     0,   435,   436,   437,     0,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   427,   430,   453,   454,   455,
     431,   362,   428,     0,     0,     0,     0,     0,   456,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     842,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   427,   430,   453,   454,   455,   431,   362,   428,     0,
       0,     0,     0,     0,   456,     0,     0,     0,     0,     0,
     429,   911,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   213,     0,     0,   912,   432,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   427,   430,   453,
     454,   455,   431,     0,   428,     0,     0,     0,     0,     0,
     456,     0,     0,     0,     0,     0,   429,   913,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   914,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   427,   430,   453,   454,   455,   431,     0,
     428,     0,     0,     0,     0,     0,   456,     0,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
     430,   453,   454,   455,   431,     0,     0,     0,     0,     0,
       0,     0,   456,   946,     0,   427,     0,     0,     0,     0,
       0,     0,   428,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   429,   432,     0,     0,     0,  1125,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,   434,     0,   435,   436,   437,     0,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,  1126,     0,   453,   454,   455,
       0,   362,     0,     0,     0,     0,     0,     0,   456,     0,
       0,   427,   430,     0,     0,     0,   431,     0,   428,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     429,  1144,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   427,   430,   453,
     454,   455,   431,     0,   428,     0,     0,     0,     0,     0,
     456,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   427,   430,   453,   454,   455,   431,   362,
     428,     0,     0,     0,     0,     0,   456,     0,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   851,     0,     0,     0,   432,
       0,     0,     0,   427,     0,     0,     0,     0,     0,     0,
     428,     0,     0,     0,     0,     0,   433,   434,     0,   435,
     436,   437,   429,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
     430,   453,   454,   455,   431,     0,     0,     0,   427,     0,
    1162,     0,   456,     0,     0,   428,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
       0,   851,     0,     0,     0,   432,     0,     0,     0,     0,
     430,     0,     0,     0,   431,     0,     0,     0,     0,     0,
       0,     0,   433,   434,     0,   435,   436,   437,     0,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   432,     0,   453,   454,   455,
       0,     0,     0,     0,     0,   430,  1204,     0,   456,   431,
       0,     0,   433,   434,     0,   435,   436,   437,     0,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   427,     0,   453,   454,   455,
     432,   551,   428,     0,     0,     0,     0,     0,   456,     0,
       0,     0,     0,     0,   429,     0,     0,   433,   434,     0,
     435,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     427,   555,   453,   454,   455,     0,     0,   428,     0,     0,
       0,     0,     0,   456,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,   431,     0,     0,     0,
     427,   815,     0,     0,     0,   731,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,   432,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,     0,     0,
       0,   431,     0,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,   432,     0,     0,     0,   427,   430,     0,     0,
     456,   431,     0,   428,     0,     0,     0,     0,     0,   433,
     434,     0,   435,   436,   437,   429,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   432,     0,   453,   454,   455,     0,     0,     0,
       0,   797,     0,     0,     0,   456,     0,     0,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   816,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   427,   430,   453,   454,   455,   431,     0,   428,
       0,     0,     0,     0,     0,   456,     0,     0,     0,     0,
       0,   429,     0,     0,   944,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   872,   432,     0,
       0,     0,   427,     0,     0,     0,     0,     0,     0,   428,
       0,     0,     0,     0,     0,   433,   434,     0,   435,   436,
     437,   429,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,     0,   430,
     453,   454,   455,   431,     0,     0,     0,   427,     0,     0,
       0,   456,     0,     0,   428,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,   432,     0,     0,     0,     0,   430,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
       0,   433,   434,     0,   435,   436,   437,     0,   438,   439,
     440,     0,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   432,     0,   453,   454,   455,     0,
       0,     0,     0,     0,   430,     0,     0,   456,   431,     0,
       0,   433,   434,     0,   435,   436,   437,     0,   438,   439,
     440,     0,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   427,  1015,   453,   454,   455,   432,
       0,   428,     0,   958,     0,     0,     0,   456,     0,     0,
       0,     0,     0,   429,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   427,
       0,   453,   454,   455,     0,     0,   428,     0,   959,     0,
       0,     0,   456,     0,     0,     0,     0,     0,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   430,     0,     0,     0,   431,     0,     0,     0,   427,
    1122,     0,     0,     0,     0,     0,   428,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   429,     0,
       0,     0,     0,     0,     0,     0,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   430,     0,     0,     0,
     431,     0,     0,   433,   434,     0,   435,   436,   437,     0,
     438,   439,   440,     0,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,     0,     0,   453,   454,
     455,   432,     0,     0,     0,   427,   430,     0,     0,   456,
     431,     0,   428,     0,     0,     0,     0,     0,   433,   434,
       0,   435,   436,   437,   429,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   432,     0,   453,   454,   455,     0,     0,     0,     0,
    1070,     0,     0,     0,   456,     0,     0,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   427,   430,   453,   454,   455,   431,     0,   428,     0,
       0,     0,     0,     0,   456,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   213,     0,     0,     0,   432,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,   430,   453,
     454,   455,   431,     0,   427,     0,     0,     0,     0,     0,
     456,   428,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,  1138,   453,   454,   455,     0,     0,
     427,   430,     0,     0,     0,   431,   456,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
    1185,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1165,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,   434,     0,   435,   436,   437,     0,
     438,   439,   440,     0,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   427,   430,   453,   454,
     455,   431,     0,   428,     0,     0,     0,     0,     0,   456,
       0,     0,     0,     0,     0,   429,  1188,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   432,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,   428,     0,     0,     0,     0,     0,   433,
     434,     0,   435,   436,   437,   429,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,   430,   453,   454,   455,   431,     0,     0,
       0,   427,  1212,     0,     0,   456,     0,     0,   428,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,   432,     0,
       0,     0,     0,   430,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   432,     0,
     453,   454,   455,     0,     0,     0,     0,     0,   430,     0,
       0,   456,   431,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,     0,     0,
     453,   454,   455,   432,     0,     0,     0,   427,     0,  1190,
       0,   456,     0,     0,   428,     0,     0,     0,     0,     0,
     433,   434,     0,   435,   436,   437,   429,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,     0,   453,   454,   455,     0,     0,
       0,     0,   427,     0,     0,  1254,   456,     0,     0,   428,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,   431,     0,
       0,     0,     0,   427,     0,     0,     0,     0,     0,     0,
     428,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,   431,     0,     0,   433,   434,     0,   435,
     436,   437,  1261,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
       0,   453,   454,   455,   432,     0,     0,     0,     0,     0,
     430,     0,   456,     0,   431,     0,     0,     0,     0,     0,
       0,   433,   434,  1264,   435,   436,   437,     0,   438,   439,
     440,     0,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,     0,   432,   453,   454,   455,   427,
       0,     0,     0,     0,     0,     0,   428,   456,     0,     0,
       0,     0,   433,   434,     0,   435,   436,   437,   429,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,     0,     0,   453,   454,   455,
     427,     0,     0,     0,     0,     0,     0,   428,   456,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   430,     0,     0,     0,
     431,   427,     0,     0,     0,     0,     0,     0,   428,  1315,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,     0,     0,     0,     0,     0,   430,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,   433,   434,
    1316,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,     0,   432,   453,   454,   455,     0,     0,   430,     0,
       0,     0,   431,     0,   456,     0,     0,     0,     0,   433,
     434,  1317,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,   432,   453,   454,   455,   427,     0,     0,
       0,     0,     0,     0,   428,   456,     0,     0,     0,     0,
     433,   434,     0,   435,   436,   437,   429,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,     0,   453,   454,   455,   427,     0,
       0,     0,     0,     0,     0,   428,   456,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,   431,   427,
       0,     0,     0,     0,     0,     0,   428,  1318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   432,
       0,     0,     0,     0,     0,   430,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,   433,   434,  1319,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
     432,   453,   454,   455,     0,     0,   430,     0,     0,     0,
     431,     0,   456,     0,     0,     0,     0,   433,   434,  1320,
     435,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
       0,   432,   453,   454,   455,   427,     0,     0,     0,     0,
       0,     0,   428,   456,     0,     0,     0,     0,   433,   434,
       0,   435,   436,   437,   429,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,     0,     0,   453,   454,   455,   427,     0,     0,     0,
       0,     0,     0,   428,   456,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,     0,     0,
       0,     0,     0,   934,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,   432,   453,
     454,   455,     0,     0,     0,     0,     0,     0,     0,     0,
     456,     0,     0,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,     0,   357,
     453,   454,   455,    22,    23,     0,     0,     0,     0,     0,
       0,   456,   358,     0,    29,   359,     0,     0,     0,     0,
      32,     0,     0,     0,     0,     0,     0,    37,     0,     0,
       0,     0,   360,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   357,     0,     0,     0,    22,
      23,     0,     0,    53,     0,    55,     0,    57,   358,  1055,
      29,   359,  1056,     0,     0,   361,    32,    64,     0,     0,
       0,     0,     0,    37,     0,     0,     0,     0,   360,     0,
       0,     0,     0,     0,     0,     0,    80,     0,     0,    82,
       0,   357,    84,     0,     0,    22,    23,     0,     0,    53,
       0,    55,     0,    57,   358,  1055,    29,   359,  1056,     0,
       0,   361,    32,    64,     0,     0,     0,    88,     0,    37,
       0,     0,     0,     0,   360,     0,     0,     0,     0,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,    99,     0,     0,    53,     0,    55,  1057,    57,
       0,  1055,     0,     0,  1056,     0,     0,   361,     0,    64,
       0,     0,     0,    88,   357,     0,     0,     0,    22,    23,
       0,     0,     0,     0,     0,     0,     0,   358,    80,    29,
     359,    82,     0,     0,    84,    32,     0,     0,    99,     0,
       0,     0,    37,     0,  1133,  -350,     0,   360,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   -68,     0,    88,
       0,     0,     0,     0,     0,  -350,     0,     0,    53,     0,
      55,     0,     0,     0,    59,  -350,   -68,    60,     0,     0,
     361,   357,    64,     0,    99,    22,    23,     0,     0,     0,
    1213,     0,     0,     0,   358,     0,    29,   359,     0,     0,
       0,    80,    32,     0,    82,     0,     0,    84,     0,    37,
       0,     0,     0,     0,   360,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   357,     0,     0,
       0,    22,    23,     0,     0,    53,     0,    55,     0,   355,
     358,  1055,    29,   359,  1056,     0,     0,   361,    32,    64,
       0,     0,     0,     0,     0,    37,     0,    99,     0,     0,
     360,     0,   362,     0,     0,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,    53,     0,    55,     0,     0,     0,  1055,     0,   398,
    1056,   228,     0,   361,     0,    64,     0,     0,     0,    88,
       0,     0,     0,     0,     0,   229,   230,     0,   231,     0,
       0,     0,     0,   232,    80,     0,     0,    82,     0,     0,
      84,   233,     0,     0,    99,     0,     0,   234,     0,     0,
       0,     0,     0,   235,     0,     0,     0,   236,     0,     0,
     237,     0,     0,     0,     0,    88,   228,     0,     0,     0,
     238,     0,     0,     0,     0,     0,     0,   239,   240,     0,
     229,   230,     0,   231,     0,   241,     0,     0,   232,     0,
      99,     0,     0,     0,     0,   242,   233,     0,     0,     0,
       0,     0,   234,     0,   243,   244,     0,   245,   235,   246,
       0,   247,   236,     0,   248,   237,     0,     0,   249,     0,
       0,   250,     0,     0,   251,   238,     0,     0,     0,     0,
       0,     0,   239,   240,     0,     0,     0,     0,     0,     0,
     241,   228,     0,     0,     0,     0,     0,     0,    88,     0,
     242,     0,     0,     0,     0,   229,   230,     0,   231,   243,
     244,     0,   245,   232,   246,    23,   247,     0,     0,   248,
       0,   233,     0,   249,     0,     0,   250,   234,     0,   251,
       0,     0,     0,   235,     0,     0,     0,   236,     0,     0,
     237,     0,     0,     0,     0,     0,   228,     0,     0,     0,
     238,     0,     0,    88,     0,     0,     0,   239,   240,     0,
     229,   230,     0,   231,     0,   241,    55,     0,   232,     0,
       0,     0,     0,     0,     0,   242,   233,     0,    64,     0,
       0,     0,   234,     0,   243,   244,     0,   245,   235,   246,
       0,   247,   236,     0,   248,   237,     0,     0,   249,     0,
       0,   250,     0,    84,   251,   238,     0,     0,     0,     0,
       0,     0,   239,   240,     0,     0,     0,     0,     0,     0,
     241,  -325,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,  -325,  -325,     0,  -325,   243,
     244,     0,   245,  -325,   246,     0,   247,     0,     0,   248,
       0,  -325,     0,   249,   540,     0,   250,  -325,     0,   251,
       0,     0,     0,  -325,     0,     0,     0,  -325,     0,     0,
    -325,     0,     0,     0,     0,     0,   228,     0,     0,     0,
    -325,     0,     0,     0,     0,     0,     0,  -325,  -325,     0,
     229,   230,     0,   231,     0,  -325,     0,     0,   232,     0,
       0,     0,     0,     0,     0,  -325,   233,     0,     0,     0,
       0,     0,   234,     0,  -325,  -325,     0,  -325,   235,  -325,
       0,  -325,   236,     0,  -325,   237,     0,     0,  -325,     0,
       0,  -325,     0,     0,  -325,   238,     0,     0,     0,     0,
       0,     0,   239,   240,     0,     0,     0,     0,     0,     0,
     241,  -326,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,  -326,  -326,     0,  -326,   243,
     244,     0,   245,  -326,   246,     0,   247,     0,     0,   248,
       0,  -326,     0,   249,     0,     0,   250,  -326,     0,   251,
       0,     0,     0,  -326,     0,     0,     0,  -326,     0,     0,
    -326,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -326,     0,     0,     0,     0,     0,     0,  -326,  -326,     0,
       0,     0,     0,     0,     0,  -326,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -326,     0,     0,     0,     0,
       0,     0,     0,     0,  -326,  -326,     0,  -326,     0,  -326,
       0,  -326,     0,     0,  -326,     0,     0,     0,  -326,     0,
       0,  -326,     0,     0,  -326
};

static const yytype_int16 yycheck[] =
{
       2,   549,     2,   111,   181,   484,    61,    20,   152,     2,
     227,   165,    26,    61,   399,    61,   419,   553,   221,   627,
     263,   920,    66,    25,   624,   673,   587,   675,   878,    66,
     544,   601,    25,   603,   604,   821,   132,   509,   608,   609,
     576,   816,    86,   381,   287,   780,   263,     1,     1,    86,
     960,    65,   839,     8,     3,   786,    55,   404,   102,    32,
      32,   519,    27,    32,    32,   102,    32,   884,    55,   102,
     287,     1,    74,    40,    41,  1179,    78,    79,   120,    36,
      22,    74,   259,   147,   327,    78,    79,    47,   147,    24,
      35,    89,   120,     0,   120,    93,    47,    32,    32,   101,
      27,    24,    32,   105,   147,    40,    41,    42,   101,    32,
     327,   175,   105,    78,   173,    72,    47,    40,    41,    42,
      47,    74,    60,    92,    51,    90,    92,     3,   171,    32,
      60,     1,  1031,   175,     2,   387,   388,   170,   390,    90,
    1244,     3,   147,    92,   165,   962,    88,   175,    75,   175,
     115,    78,    27,    80,   103,    32,    89,    25,     1,    90,
      93,    47,    92,    90,   117,   110,   171,   767,  1018,   627,
     112,   125,    47,   172,   147,   105,    51,    47,   430,   431,
     118,    55,   109,    47,   119,   172,   120,   147,   118,    32,
     227,   538,   146,   147,   149,  1105,   119,   170,   170,   165,
      75,   170,   170,    78,   170,    80,    74,   174,  1058,  1059,
      78,    79,   214,    47,   750,    90,   146,    60,   171,   173,
     120,   214,   439,   954,   476,   477,   263,   103,   963,   960,
    1017,   474,   475,   101,   109,   805,   170,   105,  1013,   174,
     170,   103,   419,   120,   147,   514,   173,    46,    18,    92,
     287,   174,   590,    27,    74,   125,   120,   474,   475,   820,
     732,   147,   105,    24,   266,   123,    65,   170,     3,   271,
     784,    32,   541,   266,    27,   118,   146,   147,   271,    40,
      41,    42,  1132,   147,   380,    84,   120,    55,    53,   175,
     327,   399,   509,   170,    46,   165,   147,   117,   173,   118,
     165,    75,   141,   146,    78,   147,   171,    77,    73,  1173,
     149,   175,   170,   147,   165,   114,    90,  1167,    83,     2,
     572,   573,   165,  1222,   527,    78,    96,   170,   147,   109,
     802,   173,    84,   933,   488,   109,   553,    90,  1124,    31,
    1126,   175,    25,   113,    46,   347,   214,   347,   403,   147,
       3,   171,    55,   961,   347,  1086,   109,   165,   119,   576,
     362,    55,   114,    65,   172,   165,   174,    20,   140,   362,
     372,   165,   172,   171,  1105,   147,   390,   171,   125,   372,
      33,   395,    84,   385,  1248,  1249,  1250,  1251,  1252,  1253,
     593,    74,   385,    78,    79,    78,    79,    89,   266,   171,
      90,    93,    89,   271,   876,   152,    93,   154,   155,    62,
     157,   158,   114,   174,   147,   172,   147,   419,   101,   463,
     464,   147,   105,  1071,  1072,   469,   463,   464,   472,   473,
      78,    79,   469,   147,   147,   472,   473,   474,   475,    89,
     173,   920,   173,    93,  1005,   917,   147,   173,   984,   141,
     103,   147,   147,    11,   146,   125,   109,   149,    89,   173,
     173,   153,    93,   147,   165,  1035,  1036,    55,   482,  1039,
    1040,   147,   509,   147,  1044,  1045,   147,   173,   173,   347,
     147,   151,    55,    82,   539,    50,    85,   147,    53,   173,
      27,   539,   895,   539,   362,   880,   147,   173,   500,   173,
     502,   147,   173,   147,   372,     8,   173,   500,    73,   502,
      47,    76,    27,   173,    51,   732,   553,   385,    83,   147,
     172,   565,   173,   567,   147,   288,   289,   173,   565,   173,
     567,   214,    47,   750,     2,    55,    51,   165,    75,   576,
     147,    78,   165,    80,   109,    55,  1116,  1117,  1118,  1119,
     147,     2,  1031,    90,   172,   147,   174,    25,   165,   573,
      75,   147,  1123,    78,  1125,    80,   147,    55,   165,   165,
      55,   147,   109,   165,    25,    90,   172,   147,   580,   165,
     580,   172,   595,   266,   165,   802,   147,   580,   271,   165,
     842,   147,    89,   165,   109,   165,    93,   849,   815,   816,
     172,    55,   100,   855,   165,    55,    74,   141,   860,   165,
      78,    79,   146,   172,    55,   149,   868,    55,   172,   153,
     174,    55,   172,    74,   174,     3,    55,    78,    79,     3,
     170,  1179,   500,   101,   502,   140,   172,   105,   174,    17,
      55,   170,    20,   172,   146,   172,    12,    25,   165,   172,
     101,   174,   164,    19,   105,    55,   172,    35,   174,   876,
     912,   172,   914,   915,   347,    31,    32,    45,   172,   565,
     174,   567,   165,   165,    52,   165,   165,  1062,   165,   362,
      84,    47,   934,   935,    62,   165,   165,   841,   170,   372,
     165,   172,    70,   170,   172,   732,  1244,   173,   147,   146,
     917,   165,   385,   172,   748,   165,     8,     3,    65,    87,
     172,   748,   580,   750,   170,   125,   171,    90,   895,    90,
     125,    99,   149,    89,    68,   103,   770,    93,   980,   981,
     982,   125,   110,   770,   878,   172,   147,   881,   116,   125,
     173,   743,   125,  1222,    34,    34,   214,    65,   173,   125,
     173,   146,   754,   173,   120,   757,    21,   149,   124,   119,
     172,   754,   170,   214,   757,   802,   147,   984,   105,   172,
     149,   141,   149,   175,   149,   141,   142,   149,   144,   145,
     146,   149,   148,   149,   150,   149,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   266,   149,
     166,   167,   168,   271,   170,   172,   149,   149,   149,    31,
     149,   177,   165,   149,   149,   266,   149,   500,   165,   502,
     271,   173,    34,   878,    34,   165,   146,   882,   173,    47,
     147,   165,   846,   165,   165,   173,   891,   165,    34,   876,
     146,   171,   171,   170,    32,    32,   848,   172,   147,   175,
     165,   175,   854,     2,   175,   848,  1099,  1100,   165,   172,
     172,   854,   165,   173,   866,    34,   175,    89,   870,   173,
     175,    93,   165,   866,  1018,   175,    25,   870,   173,   347,
     917,   173,  1099,  1100,   165,   165,   754,   164,   164,   757,
     147,   110,   984,   575,   362,   813,   347,   580,   372,   262,
      24,   364,   975,   389,   372,   950,   525,   335,    31,   539,
     383,   362,   895,   891,  1058,   536,   301,   385,   882,   141,
    1244,   372,  1064,  1254,   146,    74,   148,   149,  1247,    78,
      79,   153,   934,   935,   385,   840,   679,   130,   160,   941,
     522,   943,   766,    -1,   946,    -1,   168,   984,   941,    -1,
     943,  1059,   101,   946,  1062,    -1,   105,   971,    -1,    -1,
      -1,    -1,    -1,  1018,  1141,    -1,    89,    -1,   970,    -1,
      93,  1148,    -1,    -1,    -1,    -1,    -1,   970,    -1,    -1,
     848,   983,    -1,    -1,    -1,    -1,   854,    -1,  1132,    -1,
     983,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   866,    -1,
      -1,    -1,   870,    -1,    -1,    -1,    -1,    -1,  1021,  1022,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,     2,
     153,    -1,   500,    -1,   502,    -1,   159,   160,    -1,    -1,
     163,  1043,    -1,   166,   167,   168,    -1,  1049,    -1,   500,
    1043,   502,    25,    -1,   177,    -1,  1049,    -1,    -1,  1167,
      -1,    -1,  1099,  1100,    -1,   214,    -1,    -1,    -1,    -1,
      -1,   754,    -1,   941,   757,   943,    -1,  1132,   946,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1093,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1093,    74,   970,  1280,  1281,    78,    79,  1284,  1285,  1286,
      -1,  1288,   580,    -1,    -1,   983,    -1,   266,    -1,    -1,
      -1,    -1,   271,    -1,    -1,    -1,    -1,    -1,   101,   580,
      -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1142,    -1,  1144,    -1,    -1,    -1,    -1,  1149,    -1,  1142,
    1152,  1144,    -1,    -1,    -1,    -1,  1149,    -1,  1160,  1152,
    1162,    -1,    -1,    -1,    -1,   848,    -1,  1160,    -1,  1162,
      -1,   854,    -1,    -1,    -1,  1043,    -1,    -1,    -1,    -1,
      -1,  1049,    -1,   866,    -1,    -1,    -1,   870,    -1,    -1,
      -1,  1193,    -1,    -1,    -1,    -1,    -1,  1199,   347,    -1,
    1193,  1203,  1204,    -1,    -1,    -1,  1199,    -1,    -1,    -1,
    1203,  1204,    -1,   362,    -1,  1217,    -1,    -1,    -1,    -1,
      -1,    -1,  1224,   372,  1217,  1093,    -1,    -1,    -1,    -1,
      -1,  1224,    -1,    -1,  1236,    -1,   385,    -1,  1240,    -1,
      -1,   214,    -1,  1236,    -1,    -1,    -1,  1240,    -1,    -1,
      -1,    -1,    -1,  1255,    44,  1255,    -1,    -1,   941,    -1,
     943,    -1,  1255,   946,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1142,    -1,  1144,    -1,    68,    -1,
      -1,  1149,    -1,    -1,  1152,    -1,   754,   970,    -1,   757,
      -1,    -1,  1160,   266,  1162,    -1,    -1,    -1,   271,    31,
     983,    -1,    -1,   754,    -1,    -1,   757,  1309,  1310,  1311,
    1312,  1313,  1314,    -1,    -1,    -1,  1309,  1310,  1311,  1312,
    1313,  1314,    -1,    -1,    -1,  1193,    -1,    -1,    -1,    -1,
      -1,  1199,    -1,    -1,    -1,  1203,  1204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1217,
       2,   500,    -1,   502,    -1,    -1,  1224,    89,    -1,    -1,
    1043,    93,    -1,    -1,    -1,    -1,  1049,    -1,  1236,    -1,
      -1,    -1,  1240,    25,   347,    -1,    -1,    -1,    -1,    -1,
     848,    -1,    -1,    -1,    -1,    -1,   854,  1255,    -1,   362,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   848,   866,   372,
      -1,    -1,   870,   854,    -1,    -1,    -1,    -1,    -1,   141,
    1093,    -1,   385,    -1,   146,   866,   148,   149,    -1,   870,
      -1,   153,    74,    -1,    -1,    -1,    78,    79,   160,    31,
      -1,   580,    -1,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,  1309,  1310,  1311,  1312,  1313,  1314,    -1,    -1,   101,
      -1,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,  1142,
      -1,  1144,    -1,    -1,    -1,    -1,  1149,    -1,    -1,  1152,
      -1,    -1,    -1,   941,    -1,   943,    -1,  1160,   946,  1162,
      -1,    -1,    -1,    -1,   274,    -1,   276,    89,    -1,    -1,
     941,    93,   943,   283,    -1,   946,    -1,    -1,    -1,    -1,
      -1,    -1,   970,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1193,    -1,    -1,    -1,    -1,   983,  1199,    -1,    -1,   970,
    1203,  1204,    -1,    -1,    -1,    -1,    -1,   500,    -1,   502,
      -1,    -1,   983,   323,  1217,    31,    -1,    -1,     2,   141,
     142,  1224,   144,   145,   146,    -1,   148,   149,    -1,    -1,
      -1,   153,    -1,  1236,    -1,    -1,    -1,  1240,   160,    -1,
      -1,    25,   214,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,  1255,    -1,    -1,  1043,    -1,    -1,    -1,    -1,
      -1,  1049,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1043,    89,    -1,    -1,    -1,    93,  1049,    -1,
      -1,    -1,    -1,    -1,    -1,   754,    -1,   580,   757,    -1,
      74,    -1,    -1,    -1,   266,    -1,    -1,    -1,    -1,   271,
      -1,    -1,    -1,    -1,    -1,  1093,  1309,  1310,  1311,  1312,
    1313,  1314,    -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,
      -1,   105,  1093,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,    -1,    -1,    -1,   153,    -1,    -1,
      -1,    -1,    -1,   159,   160,    -1,    -1,   163,    -1,    -1,
     166,   167,   168,    -1,  1142,    -1,  1144,    -1,    -1,    -1,
      -1,  1149,    -1,    -1,  1152,    -1,    -1,    -1,    -1,    -1,
      -1,  1142,  1160,  1144,  1162,   347,    -1,    -1,  1149,   848,
      -1,  1152,    -1,    -1,    -1,   854,    -1,    -1,    -1,  1160,
     362,  1162,    -1,    -1,    -1,    -1,    -1,   866,    31,    -1,
     372,   870,    -1,    -1,    -1,  1193,    -1,    -1,    -1,    -1,
      -1,  1199,    -1,   385,    -1,  1203,  1204,    -1,    -1,    -1,
      -1,    -1,  1193,    -1,    -1,    -1,    -1,    -1,  1199,  1217,
     214,    -1,  1203,  1204,    -1,    -1,  1224,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1217,    -1,  1236,    -1,
      -1,    -1,  1240,  1224,    -1,    -1,    89,    -1,    -1,    -1,
      93,   754,    -1,    -1,   757,  1236,    -1,  1255,    -1,  1240,
      -1,    -1,   941,    -1,   943,    -1,    -1,   946,    -1,    -1,
      -1,    -1,   266,    -1,  1255,    -1,    -1,   271,    -1,   599,
      -1,   601,   602,   603,   604,   605,   606,    -1,    -1,    -1,
      -1,   970,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,    -1,   145,   146,   983,   148,   149,    -1,    -1,    -1,
     153,  1309,  1310,  1311,  1312,  1313,  1314,   160,   500,    -1,
     502,    -1,    -1,   166,   167,   168,    -1,    -1,  1309,  1310,
    1311,  1312,  1313,  1314,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,    -1,    -1,    -1,   848,    -1,    -1,    -1,    -1,
      -1,   854,    -1,   347,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,   866,  1043,    26,    27,   870,   362,    -1,
    1049,    -1,    -1,    -1,    35,    -1,    37,    38,   372,    -1,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      -1,   385,    53,    -1,    55,    -1,    -1,    -1,   580,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    -1,  1093,    76,    -1,    78,    -1,    80,
      -1,    82,    83,    -1,    85,    -1,    -1,    88,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   941,    -1,
     943,    -1,    -1,   946,    -1,    -1,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1142,    -1,  1144,    -1,   970,    -1,    -1,
    1149,    -1,    -1,  1152,    -1,    -1,    -1,    -1,    -1,   140,
     983,  1160,    -1,  1162,   804,     3,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    -1,   165,    -1,   500,    25,   502,    -1,
     171,    -1,    -1,    -1,  1193,    33,    -1,    -1,    -1,    -1,
    1199,    39,    -1,   843,  1203,  1204,    -1,    45,    -1,    -1,
      -1,    49,    -1,   853,    52,    -1,    -1,   857,  1217,   859,
    1043,    -1,   862,    -1,    62,  1224,  1049,    -1,    -1,    89,
      -1,    69,    70,    93,    -1,    -1,    -1,  1236,    -1,    77,
      -1,  1240,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,   754,    -1,    -1,   757,  1255,    -1,    96,    97,
      -1,    99,    -1,   101,   124,   103,   580,    -1,   106,    -1,
    1093,    -1,   110,   111,    -1,   113,    -1,    -1,   116,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    31,   148,   149,
     150,    -1,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,    -1,
    1309,  1310,  1311,  1312,  1313,  1314,    -1,   177,    -1,  1142,
      -1,  1144,    -1,    -1,    -1,    -1,  1149,    -1,    -1,  1152,
      -1,    -1,    -1,    -1,   172,    -1,    -1,  1160,    -1,  1162,
      -1,    -1,    -1,    -1,    -1,    89,   848,    -1,    -1,    93,
      -1,    -1,   854,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   866,    -1,    -1,    -1,   870,    -1,
    1193,    -1,    -1,    -1,    -1,    -1,  1199,    -1,    -1,    -1,
    1203,  1204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1029,
    1030,    -1,    -1,    -1,  1217,  1035,  1036,   141,   142,  1039,
    1040,  1224,   146,    -1,   148,   149,    -1,    -1,    -1,   153,
      -1,    -1,    -1,  1236,    -1,    -1,   160,  1240,    -1,    -1,
      -1,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1255,    -1,    -1,    -1,    -1,    -1,    -1,   941,
     754,   943,    -1,   757,   946,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,    -1,    -1,   970,  1109,
      -1,    -1,    -1,    -1,  1114,  1115,  1116,  1117,    -1,    -1,
      -1,   983,    -1,    -1,    -1,    -1,  1309,  1310,  1311,  1312,
    1313,  1314,    -1,    -1,    -1,   169,  1136,  1137,   172,  1139,
     174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      -1,  1151,    -1,    -1,  1154,    -1,  1156,    -1,  1158,    -1,
      -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,   848,    -1,    -1,    -1,    33,    -1,
     854,  1043,    -1,    -1,    39,    -1,    -1,  1049,    -1,    -1,
      45,    -1,   866,    -1,    49,    -1,   870,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1093,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,   111,    -1,   113,    -1,
      22,   116,    -1,    -1,    26,    27,    -1,   941,    -1,   943,
      -1,    -1,   946,    35,    -1,    37,    38,    -1,    -1,    -1,
    1142,    43,  1144,    -1,    -1,    -1,    -1,  1149,    50,    -1,
    1152,    -1,    -1,    55,    -1,    -1,   970,    -1,  1160,    -1,
    1162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   983,
      -1,    -1,    -1,    -1,    76,    -1,    78,   172,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    90,    -1,
      -1,  1193,    -1,    -1,    -1,    -1,    -1,  1199,    -1,    -1,
      -1,  1203,  1204,    -1,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,  1217,    -1,    -1,    -1,    -1,
      -1,    -1,  1224,    -1,    -1,    -1,    -1,    -1,    -1,  1043,
      -1,    -1,    -1,    -1,  1236,  1049,    -1,    -1,  1240,    -1,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,  1255,    -1,    -1,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    46,    47,    -1,  1093,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    -1,
      -1,    61,    -1,    -1,    -1,    65,    66,    67,    68,    69,
      70,    -1,    -1,    -1,    74,    -1,    76,  1309,  1310,  1311,
    1312,  1313,  1314,    83,    -1,    -1,    86,    -1,    -1,    89,
      -1,    91,    -1,    -1,    94,    95,    -1,    97,  1142,    -1,
    1144,   101,   102,   103,    -1,  1149,    -1,    -1,  1152,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1160,    -1,  1162,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1193,
      -1,    -1,    -1,    -1,    -1,  1199,    -1,    -1,    -1,  1203,
    1204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1217,   174,    -1,    -1,    -1,    44,    -1,
    1224,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1236,    -1,    -1,    -1,  1240,   197,    -1,    -1,
     200,   201,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1255,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   227,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,
      -1,    -1,    -1,   263,    25,  1309,  1310,  1311,  1312,  1313,
    1314,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,   287,    49,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,   298,   299,
      -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,   313,   314,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,   327,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
     350,    -1,   113,   219,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   373,    -1,   375,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   384,    -1,   386,   387,   388,   389,
     390,    -1,   258,   259,    -1,   395,    -1,    -1,    -1,   265,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   274,   170,
     276,   172,    -1,    -1,    -1,    -1,    -1,   283,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   427,   428,   429,
     430,   431,   432,    -1,   434,   435,   436,   437,   438,    -1,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   323,    -1,    -1,
      -1,    -1,    -1,   463,   464,    -1,    -1,    -1,    -1,   469,
      -1,    -1,   472,   473,   474,   475,   476,   477,    -1,    -1,
     480,   481,   482,    -1,   484,    -1,    -1,    -1,    -1,    -1,
     490,    -1,     2,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   502,    -1,    -1,   505,    -1,    -1,    -1,   509,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,   519,
      -1,   521,    -1,    -1,    -1,   525,    -1,    -1,   528,   529,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   539,
      -1,    -1,    -1,    -1,   544,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   553,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   562,    -1,    74,   565,    -1,   567,    -1,    -1,
      -1,   571,   572,   573,    -1,    -1,   576,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   587,   588,     3,
      -1,   101,    -1,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    -1,    20,   607,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,   617,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    -1,   627,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    31,    -1,
     670,    -1,    -1,    87,    -1,    -1,    -1,   677,    -1,    -1,
      -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,   214,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,   731,   732,   599,   600,   601,   602,   603,   604,   605,
     606,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   748,    -1,
     750,   751,   752,    12,    13,    -1,   266,    -1,   172,    -1,
      19,   271,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     770,    -1,    31,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,   784,   148,   149,   150,   788,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,    -1,
     163,    -1,   802,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   813,   177,    -1,    -1,   817,    -1,   819,
     820,   821,    -1,   823,    -1,   825,    -1,    -1,   828,   829,
      89,    -1,    -1,    -1,    93,    -1,    -1,   347,    -1,    -1,
      -1,    -1,   842,    -1,   844,    -1,   846,    -1,   848,   849,
      -1,    -1,   362,    -1,   854,   855,    -1,    -1,    -1,    -1,
     860,    -1,   372,    -1,    -1,   124,    -1,    -1,   868,   869,
     870,    -1,   872,   873,    -1,   385,   876,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,   758,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,   911,   912,   913,   914,   915,   916,   917,   177,    -1,
     920,    -1,    -1,    -1,    -1,    -1,    -1,    31,   794,    -1,
      -1,    -1,    -1,    -1,   934,   935,    -1,   803,   804,    -1,
      -1,    -1,    -1,    -1,   944,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   961,    -1,    -1,    -1,   965,    -1,    -1,    -1,    -1,
      -1,   971,    -1,    -1,    -1,    -1,    -1,   843,    -1,    -1,
     980,   981,   982,    -1,   984,    89,    -1,   853,    -1,    93,
     500,   857,    -1,   859,    -1,    -1,   862,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1005,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1013,    -1,  1015,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,  1025,    -1,  1027,    -1,    -1,
      -1,  1031,    -1,  1033,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,    -1,   163,
      -1,    -1,   166,   167,   168,    -1,  1066,    -1,    -1,    -1,
     580,    -1,    -1,   177,    -1,    -1,  1076,    -1,    -1,  1079,
      -1,    -1,    -1,    -1,    -1,   951,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1099,
    1100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1111,   978,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1122,  1123,  1124,  1125,  1126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1142,    -1,  1144,    -1,    -1,    -1,    -1,  1149,
      -1,    -1,  1152,    -1,    -1,    28,    -1,    -1,    -1,    -1,
    1160,    -1,  1162,  1029,  1030,  1165,    39,    40,    41,  1035,
    1036,    -1,    -1,  1039,  1040,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1184,  1185,    -1,  1187,  1188,  1189,
    1190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1212,    -1,    -1,    -1,    -1,  1217,  1218,    -1,
    1220,    -1,  1222,    -1,  1224,  1225,    -1,    -1,  1228,    -1,
      -1,    -1,    -1,  1233,    -1,    31,    -1,  1237,    -1,    -1,
      -1,    -1,    -1,  1109,   754,    -1,  1246,   757,  1114,  1115,
    1116,  1117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1261,  1262,  1263,  1264,  1265,  1266,  1267,  1268,    -1,
    1136,  1137,    -1,  1139,    -1,  1141,    -1,   150,    -1,   152,
      -1,    -1,  1148,    -1,    -1,  1151,    -1,    -1,  1154,    -1,
    1156,   164,  1158,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1309,
    1310,  1311,  1312,  1313,  1314,  1315,  1316,  1317,  1318,  1319,
    1320,    -1,    -1,    -1,    -1,   198,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1201,    -1,    -1,   848,    -1,
      -1,    -1,    -1,    -1,   854,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   866,   153,   154,   155,
     870,   157,   158,   159,   160,    -1,    -1,   163,    -1,    -1,
     166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,   256,   257,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   267,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1280,  1281,    -1,    -1,  1284,  1285,
    1286,    -1,  1288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   941,    -1,   943,   307,    -1,   946,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     970,    -1,     1,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,   983,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,   358,    25,   360,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,   394,    -1,    62,    -1,    64,   399,    66,    67,    -1,
      69,    70,    -1,  1043,    -1,    -1,    -1,    -1,    77,  1049,
      79,    -1,    81,    -1,    -1,    -1,   419,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,  1093,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
     483,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,  1142,   172,  1144,   174,    -1,   510,   177,  1149,
      -1,   514,  1152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1160,    -1,  1162,    -1,   527,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   536,    -1,    -1,    -1,    -1,   541,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1193,    -1,    -1,    -1,    -1,    -1,  1199,
      -1,    -1,    -1,  1203,  1204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   586,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   597,    -1,  1236,    -1,    -1,    -1,
    1240,    -1,     0,     1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,  1255,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    71,    -1,    73,   679,    -1,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,    12,   112,   113,   114,   115,   116,    -1,
      19,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,    47,    -1,
      -1,    -1,   150,    -1,    -1,    -1,   759,    -1,    -1,    -1,
      -1,   159,    -1,   766,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,   776,   172,    -1,   174,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,   794,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   840,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,   170,    -1,    -1,    -1,    -1,    -1,   880,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   895,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,   950,    12,    -1,
      14,    15,    16,    17,    18,    19,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,   975,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,  1062,
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
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    12,    -1,
      14,    -1,    -1,    17,    18,    19,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
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
     164,   165,   166,   167,   168,   169,   170,    -1,   172,    -1,
      -1,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    12,    -1,    14,    -1,    -1,    17,    18,
      19,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,    -1,   138,
     139,    -1,   141,   142,   143,   144,   145,   146,    -1,   148,
      -1,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,   166,   167,   168,
     169,   170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    -1,    17,    18,    -1,    20,    -1,    22,    23,
      -1,    25,    -1,    -1,    -1,    -1,    30,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    61,    62,    63,
      64,    -1,    66,    67,    -1,    69,    70,    71,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    26,    27,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
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
     144,   145,    -1,   147,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,
      -1,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,
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
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,   112,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
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
      -1,   170,    -1,   172,   173,    -1,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
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
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
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
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,   173,    -1,    -1,    -1,   177,     3,
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
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
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
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,    -1,   175,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
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
     144,   145,    -1,   147,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,
      -1,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,   147,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,   171,   172,    -1,    -1,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
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
     144,   145,    -1,   147,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,
      -1,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      89,    -1,    -1,    -1,    93,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
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
      -1,   170,    -1,   172,    -1,    -1,   175,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
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
     144,   145,    -1,   147,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,   173,
      -1,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,   122,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
      -1,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
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
      -1,   170,    -1,   172,    -1,    -1,   175,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
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
     164,    -1,    -1,    -1,   168,    -1,   170,   171,   172,    -1,
      -1,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
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
      -1,   175,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
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
      -1,   170,    -1,   172,    -1,    -1,   175,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
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
      -1,   170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
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
     144,   145,    -1,    -1,    -1,    -1,   150,     4,     5,     6,
       7,     8,     9,    10,    -1,   159,    -1,    14,   162,   163,
     164,    18,    -1,    -1,   168,    22,   170,    -1,   172,    -1,
      27,    -1,    -1,   177,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    -1,    -1,    64,    -1,    66,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      77,    78,    79,    80,    81,    -1,    -1,    -1,    -1,    86,
      -1,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    98,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,   173,   174,    -1,    -1,
     177,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    18,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    -1,
      -1,    64,    -1,    66,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    77,    78,    79,    80,    81,    -1,
      -1,    -1,    -1,    86,    -1,    88,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    96,    -1,    98,    -1,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,     3,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    12,    -1,   150,    -1,    -1,
      17,    18,    19,    20,    -1,    -1,   159,    -1,    25,   162,
     163,   164,    -1,    -1,    -1,   168,    33,   170,    35,   172,
      -1,   174,    39,    -1,   177,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,    -1,   141,   142,   143,   144,   145,   146,
      -1,   148,    -1,    -1,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,   163,    -1,    -1,   166,
     167,   168,   169,    -1,    -1,   172,     3,     4,     5,     6,
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
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,    -1,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,     3,     4,
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
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,   143,    -1,
      -1,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,   164,
      22,    -1,    -1,    25,    -1,   170,    -1,   172,    -1,   174,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,     3,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    12,    -1,    -1,    -1,    -1,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,   141,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,
      -1,    -1,   164,    -1,    -1,    62,    -1,    -1,    -1,    -1,
     172,    -1,   174,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,    -1,
      -1,    -1,   109,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,    -1,   141,   142,   143,   144,   145,   146,
      -1,   148,    -1,    -1,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,     3,   163,    -1,    -1,   166,
     167,   168,   169,   170,    12,    -1,    -1,    -1,    -1,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    96,    97,
      -1,    99,    19,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    31,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,   143,   144,   145,   146,    -1,
     148,    -1,    -1,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,   163,    -1,    -1,   166,   167,
     168,   169,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    89,   170,    -1,    12,    93,    -1,    -1,    -1,
     177,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    89,   170,    -1,    12,    93,    -1,    -1,    -1,
     177,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    89,   170,    -1,    12,    93,    -1,    -1,    -1,
     177,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    89,   170,    -1,    12,    93,    -1,    -1,    -1,
     177,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    89,   170,    -1,    12,    93,    -1,    -1,    -1,
     177,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    89,   170,    -1,    12,    93,    -1,    -1,    -1,
     177,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    89,   170,    -1,    12,    93,    -1,    -1,    -1,
     177,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    89,   170,    -1,    -1,    93,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,    31,    32,   133,   134,   135,   136,
      -1,   138,   139,    -1,   141,   142,    -1,   144,   145,   146,
      47,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    31,   124,    -1,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    72,    -1,   166,
     167,   168,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,   124,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    72,
     165,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
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
     124,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      12,   165,   166,   167,   168,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      12,    13,    -1,    -1,    -1,   102,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,   124,    -1,    -1,    -1,    12,    89,    -1,    -1,
     177,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    31,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,   173,    -1,    -1,    -1,   177,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    12,    89,   166,   167,   168,    93,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    31,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    89,
     166,   167,   168,    93,    -1,    -1,    -1,    12,    -1,    -1,
      -1,   177,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,   177,    93,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    12,    13,   166,   167,   168,   124,
      -1,    19,    -1,   173,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    12,
      -1,   166,   167,   168,    -1,    -1,    19,    -1,   173,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    12,
      13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,   124,    -1,    -1,    -1,    12,    89,    -1,    -1,   177,
      93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    31,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,   177,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    89,   166,
     167,   168,    93,    -1,    12,    -1,    -1,    -1,    -1,    -1,
     177,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,   165,   166,   167,   168,    -1,    -1,
      12,    89,    -1,    -1,    -1,    93,   177,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    12,    89,   166,   167,
     168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    31,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    89,   166,   167,   168,    93,    -1,    -1,
      -1,    12,    13,    -1,    -1,   177,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   124,    -1,
     166,   167,   168,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,   177,    93,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
     166,   167,   168,   124,    -1,    -1,    -1,    12,    -1,   175,
      -1,   177,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    31,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    60,   177,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,   141,   142,    -1,   144,
     145,   146,   102,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,   124,    -1,    -1,    -1,    -1,    -1,
      89,    -1,   177,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   102,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,   124,   166,   167,   168,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,   177,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    31,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     102,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,   124,   166,   167,   168,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,   177,    -1,    -1,    -1,    -1,   141,
     142,   102,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,   124,   166,   167,   168,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,   177,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    31,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   102,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
     124,   166,   167,   168,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,   177,    -1,    -1,    -1,    -1,   141,   142,   102,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,   124,   166,   167,   168,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,   177,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    31,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,   167,   168,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,   124,   166,
     167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    22,
     166,   167,   168,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    35,    -1,    37,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    76,    -1,    78,    -1,    80,    35,    82,
      37,    38,    85,    -1,    -1,    88,    43,    90,    -1,    -1,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,    22,   115,    -1,    -1,    26,    27,    -1,    -1,    76,
      -1,    78,    -1,    80,    35,    82,    37,    38,    85,    -1,
      -1,    88,    43,    90,    -1,    -1,    -1,   140,    -1,    50,
      -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,   165,    -1,    -1,    76,    -1,    78,   171,    80,
      -1,    82,    -1,    -1,    85,    -1,    -1,    88,    -1,    90,
      -1,    -1,    -1,   140,    22,    -1,    -1,    -1,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,   109,    37,
      38,   112,    -1,    -1,   115,    43,    -1,    -1,   165,    -1,
      -1,    -1,    50,    -1,   171,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,    76,    -1,
      78,    -1,    -1,    -1,    82,    83,    84,    85,    -1,    -1,
      88,    22,    90,    -1,   165,    26,    27,    -1,    -1,    -1,
     171,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,
      -1,   109,    43,    -1,   112,    -1,    -1,   115,    -1,    50,
      -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,    76,    -1,    78,    -1,    80,
      35,    82,    37,    38,    85,    -1,    -1,    88,    43,    90,
      -1,    -1,    -1,    -1,    -1,    50,    -1,   165,    -1,    -1,
      55,    -1,   170,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    -1,    -1,    -1,    82,    -1,     1,
      85,     3,    -1,    88,    -1,    90,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,
      -1,    -1,    -1,    25,   109,    -1,    -1,   112,    -1,    -1,
     115,    33,    -1,    -1,   165,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    -1,    -1,   140,     3,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      17,    18,    -1,    20,    -1,    77,    -1,    -1,    25,    -1,
     165,    -1,    -1,    -1,    -1,    87,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    96,    97,    -1,    99,    45,   101,
      -1,   103,    49,    -1,   106,    52,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,     3,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,
      87,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,    96,
      97,    -1,    99,    25,   101,    27,   103,    -1,    -1,   106,
      -1,    33,    -1,   110,    -1,    -1,   113,    39,    -1,   116,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,
      62,    -1,    -1,   140,    -1,    -1,    -1,    69,    70,    -1,
      17,    18,    -1,    20,    -1,    77,    78,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    33,    -1,    90,    -1,
      -1,    -1,    39,    -1,    96,    97,    -1,    99,    45,   101,
      -1,   103,    49,    -1,   106,    52,    -1,    -1,   110,    -1,
      -1,   113,    -1,   115,   116,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,    96,
      97,    -1,    99,    25,   101,    -1,   103,    -1,    -1,   106,
      -1,    33,    -1,   110,   111,    -1,   113,    39,    -1,   116,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      17,    18,    -1,    20,    -1,    77,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    96,    97,    -1,    99,    45,   101,
      -1,   103,    49,    -1,   106,    52,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,    96,
      97,    -1,    99,    25,   101,    -1,   103,    -1,    -1,   106,
      -1,    33,    -1,   110,    -1,    -1,   113,    39,    -1,   116,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116
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
     249,   250,   251,   252,   253,   257,   258,   263,   264,   265,
     266,   269,   270,   275,   276,   278,   279,   284,   288,   289,
     291,   292,   314,   319,   320,   324,   325,   346,   347,   348,
     350,   352,   353,   354,   362,   363,   364,   365,   366,   367,
     369,   372,   373,   374,   375,   376,   377,   378,   379,   381,
     382,   383,   384,   385,   165,    22,    35,    44,    55,    88,
     101,   170,   239,   248,   276,   346,   353,   363,   364,   369,
     372,   374,   375,   120,   355,   356,     3,   218,   369,   355,
     109,   326,    90,   218,   186,   340,   369,   172,     3,    17,
      18,    20,    25,    33,    39,    45,    49,    52,    62,    69,
      70,    77,    87,    96,    97,    99,   101,   103,   106,   110,
     113,   116,   219,   220,   222,    11,    27,   115,   244,   369,
      82,    85,   202,   172,   220,   220,   220,   172,   220,   327,
     328,    32,   206,   225,   369,   261,   262,   369,   369,    18,
      77,    96,   113,   369,   369,   369,     8,   172,   229,   228,
      27,    33,    47,    49,    51,    75,    78,    90,    97,   101,
     109,   185,   224,   280,   281,   282,   305,   306,   307,   332,
     337,   369,   340,   107,   108,   165,   284,   285,   368,   369,
     371,   369,   225,   369,   369,   369,   100,   172,   186,   369,
     369,   188,   192,   206,   188,   206,   369,   371,     3,   369,
     369,   369,   369,   369,     1,   171,   184,   207,   340,   111,
     151,   341,   342,   371,   369,    80,   186,    22,    35,    38,
      55,    88,   170,   189,   190,   191,   202,   206,   194,     8,
     149,   196,   170,    46,    84,   114,   203,    26,   325,   369,
       8,   265,   369,   370,    24,    32,    40,    41,    42,   119,
     174,   241,   349,   351,    55,   146,   267,   220,     1,   193,
     220,   271,   274,   172,   300,    53,    73,    83,   309,    27,
      75,    78,    90,   109,   310,    27,    78,    90,   109,   308,
     220,   321,   322,   327,   164,   165,   369,    12,    19,    31,
      89,    93,   124,   141,   142,   144,   145,   146,   148,   149,
     150,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   166,   167,   168,   177,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   169,   296,   172,   174,    89,    93,   369,   220,
      40,    41,   174,    55,   172,   186,   165,   206,   324,   165,
     147,   165,   221,   343,   344,   345,   371,   170,   220,   220,
     102,   206,   102,   125,   206,   296,   203,   343,   165,   172,
     172,   206,   186,   111,   172,   220,   329,   330,     1,   146,
     336,    47,   147,   186,   225,   147,   225,    13,   172,   172,
     225,   343,   230,   230,    47,    90,   306,   307,   173,   147,
     111,   172,   220,   304,   146,   165,   369,   369,   123,   286,
     165,   170,   225,   172,   343,   165,   254,   254,   165,   171,
     171,   184,   147,   171,   369,   147,   173,   147,   173,     8,
     220,    40,    41,   174,   241,     3,   172,   197,     1,   171,
     207,   214,   215,   369,   209,   369,    65,    36,    72,   165,
     265,   267,   109,   236,   289,   369,   186,    78,   122,   242,
     369,   242,   369,   242,   369,   244,   369,   175,   242,   340,
     369,   380,   220,   340,   170,   171,   274,   125,   147,   171,
     173,   301,   303,   305,   310,    90,     1,   146,   334,   335,
      90,     1,     3,    12,    17,    19,    20,    25,    45,    52,
      54,    62,    70,    87,    99,   103,   110,   116,   141,   142,
     143,   144,   145,   146,   148,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   163,   166,   167,   168,   169,
     172,   220,   293,   294,   295,   296,   346,   125,   323,   147,
     165,   165,   369,   369,   369,   242,   369,   242,   369,   369,
     369,   369,   369,   369,   369,     3,    20,    33,    62,   103,
     109,   221,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   369,    68,
     371,   371,   371,   371,   371,   343,   343,   242,   369,   242,
     369,   102,   172,   369,   369,   340,   220,    27,    47,    90,
     115,   357,   360,   361,   369,   385,   327,   369,   125,   173,
     147,   125,   125,   186,    34,   186,   369,    34,   369,    65,
     173,   344,   220,   246,   247,   330,   147,   173,   224,   369,
     125,   331,   369,   328,   261,   220,   324,   369,   369,   173,
     304,   310,   282,   330,   146,   338,   339,   337,   287,   205,
       1,   259,   345,   173,    21,   255,   171,   173,   173,   341,
     173,   341,   172,   369,   369,   340,   196,   173,   198,   199,
     345,   171,   171,   147,   165,    13,   149,   147,   165,    13,
      36,    72,   220,   141,   142,   143,   144,   145,   159,   163,
     168,   208,   295,   296,   297,   369,   208,   210,   267,   170,
     237,   324,    47,   355,   119,   220,   172,   225,    32,    47,
     225,   120,   225,   358,    32,    47,   225,   358,   225,   358,
      47,   225,   358,   225,   225,   369,   175,   358,    47,   147,
     175,   358,   123,   147,   175,   165,   172,     1,   268,   369,
     272,   273,   147,   173,   336,     1,   224,   332,   333,   105,
     311,   172,   299,   369,   141,   149,   299,   299,   369,   322,
     172,   174,   165,   165,   165,   165,   165,   165,   173,   175,
     344,    32,    47,    32,    47,    47,   175,   172,    47,    90,
     147,   173,    17,    20,    25,    45,    52,    62,    70,    87,
      99,   110,   116,   346,    89,    89,   165,   371,   345,   369,
     369,    34,   186,    34,    34,   186,   102,   206,   220,   173,
     147,   173,   173,   329,   336,    68,   371,   220,   173,   173,
     339,   146,   283,   173,   337,   151,   298,   331,   369,   171,
      74,   117,   171,   260,   173,   172,   206,   220,   256,   344,
      47,    47,    47,   175,   147,   173,   214,   221,    17,    18,
      20,    25,    45,    49,    52,    70,    77,    87,    96,    97,
      99,   101,   110,   113,   116,   170,   213,   297,   369,   111,
     369,   208,   210,   147,   165,    13,   165,   170,   268,   321,
     327,   242,   369,   225,   369,    47,   340,    44,   369,   242,
     369,   172,   225,    44,   369,   242,   369,   225,   225,   242,
     369,   225,   186,   175,   242,   369,   175,   186,   369,   175,
     369,   175,   369,   344,   171,    82,    85,   171,   185,   193,
     227,   274,   193,   303,   311,    60,   118,   315,   301,   302,
     173,   294,   296,   173,   175,   173,    44,   242,   369,    44,
     242,   369,   242,   369,   344,   360,   336,   346,   346,   186,
     186,   369,   186,    34,   165,   165,   247,   206,   331,   172,
     172,   298,   331,   332,   311,   339,   369,   186,   239,   340,
     256,   146,   206,   173,   242,   369,   242,   369,   242,   369,
     199,   208,    13,    36,    72,    36,    72,   165,   165,   297,
     369,   369,   268,   171,   165,   165,   355,   355,   165,   369,
     173,   369,    32,   225,    32,   225,   359,   360,   369,    32,
     225,   358,    32,   225,   358,   225,   358,   225,   358,   186,
     175,   358,   175,   358,   186,   123,   173,   193,   227,   227,
     274,   315,     3,    92,   103,   316,   317,   318,   369,   290,
     173,   299,   299,   369,    32,    32,   369,    32,    32,   175,
     175,   173,   331,    34,   186,   343,   343,   298,   331,    32,
     225,   173,   369,   175,   175,   171,   111,   369,   208,   210,
     208,   210,    13,   171,   225,   225,   225,   102,    44,   369,
      44,   369,   147,   173,   102,    44,   369,   225,    44,   369,
     225,   225,   225,    44,   186,   369,   175,    44,   186,   369,
     175,   369,   165,   227,   277,   318,   118,   147,   125,   152,
     154,   155,   157,   158,    60,    32,   165,   205,   226,   312,
     313,   102,    44,    44,   102,    44,    44,    44,    44,   186,
     173,   173,   186,   206,   165,   165,   165,   165,   369,   369,
     369,   369,   360,   369,   369,   369,   369,   186,   369,   186,
     313,   369,   317,   318,   318,   318,   318,   318,   318,   316,
     184,   369,   369,   369,   369,   369,   369,   164,   164,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   369,   369,   369,   369,   369,   369
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
     250,   250,   251,   251,   252,   252,   252,   253,   253,   253,
     253,   254,   254,   255,   255,   255,   256,   256,   257,   258,
     258,   259,   259,   260,   260,   260,   261,   261,   261,   262,
     262,   263,   264,   264,   265,   266,   266,   266,   267,   267,
     268,   268,   268,   268,   268,   269,   269,   270,   271,   271,
     272,   271,   271,   273,   271,   274,   274,   275,   277,   276,
     278,   279,   279,   279,   280,   280,   281,   281,   282,   282,
     282,   283,   283,   284,   286,   285,   287,   285,   288,   290,
     289,   291,   291,   291,   291,   291,   292,   293,   293,   294,
     294,   294,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   297,
     297,   298,   298,   299,   299,   300,   300,   301,   301,   302,
     302,   303,   304,   304,   305,   305,   305,   305,   305,   305,
     306,   306,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   308,   308,   308,   308,   308,   308,   309,   309,   309,
     310,   310,   310,   310,   310,   310,   310,   311,   311,   312,
     312,   313,   313,   313,   314,   315,   315,   315,   315,   315,
     316,   316,   317,   317,   317,   317,   317,   317,   317,   318,
     318,   319,   320,   320,   321,   321,   322,   323,   323,   324,
     324,   324,   324,   324,   326,   325,   325,   327,   327,   328,
     328,   329,   329,   329,   330,   330,   330,   331,   331,   331,
     332,   333,   333,   333,   334,   334,   335,   335,   336,   336,
     336,   336,   337,   337,   338,   339,   339,   340,   340,   341,
     341,   342,   342,   343,   343,   344,   344,   345,   345,   346,
     346,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   348,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   350,   351,   351,   351,
     351,   351,   351,   351,   352,   353,   354,   354,   354,   354,
     354,   354,   354,   355,   355,   356,   357,   357,   358,   359,
     359,   360,   360,   360,   361,   361,   361,   361,   361,   361,
     362,   362,   362,   362,   362,   363,   363,   363,   363,   363,
     364,   365,   365,   365,   365,   365,   365,   366,   367,   368,
     368,   369,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   370,   370,
     371,   371,   371,   372,   372,   372,   372,   373,   373,   373,
     373,   373,   374,   374,   374,   375,   375,   375,   375,   375,
     375,   376,   376,   376,   376,   377,   377,   378,   378,   379,
     379,   379,   379,   379,   379,   379,   379,   379,   379,   379,
     379,   379,   380,   380,   381,   381,   381,   381,   381,   381,
     381,   381,   381,   381,   381,   381,   381,   381,   381,   381,
     381,   381,   381,   381,   381,   381,   381,   382,   382,   382,
     382,   382,   382,   382,   383,   383,   383,   383,   384,   384,
     384,   384,   385,   385,   385,   385,   385,   385,   385
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
       6,     3,     4,     7,     5,     3,     6,     2,     2,     3,
       3,     0,     2,     2,     3,     5,     1,     3,     3,     5,
       5,     0,     2,     3,     2,     4,     4,     3,     1,     1,
       3,     3,     5,     5,     2,     1,     1,     1,     0,     2,
       0,     2,     3,     3,     4,     3,     3,     3,     1,     2,
       0,     4,     2,     0,     5,     1,     3,     1,     0,     8,
       0,     1,     1,     1,     0,     1,     1,     3,     1,     3,
       1,     0,     2,     6,     0,     3,     0,     4,     1,     0,
       7,     4,     4,     6,     6,     4,     2,     1,     3,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     0,     3,     2,     3,     1,     3,     0,
       1,     1,     1,     1,     4,     5,     4,     5,     6,     6,
       0,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     0,     1,     1,     2,     1,     1,     1,     1,     1,
       0,     1,     1,     2,     1,     1,     1,     0,     1,     1,
       1,     1,     2,     1,     1,     0,     2,     2,     4,     4,
       1,     3,     3,     3,     3,     3,     3,     3,     2,     1,
       1,     3,     1,     2,     1,     3,     2,     0,     2,     1,
       2,     1,     1,     1,     0,     5,     3,     1,     3,     3,
       5,     1,     1,     3,     1,     2,     3,     0,     2,     2,
       1,     1,     1,     1,     2,     1,     0,     1,     0,     2,
       2,     1,     1,     1,     2,     0,     1,     1,     3,     1,
       1,     3,     3,     0,     1,     1,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     4,     4,     4,
       5,     2,     2,     1,     2,     1,     2,     1,     2,     1,
       2,     1,     1,     2,     6,     6,     4,     9,     9,     7,
       6,     6,     4,     9,     9,     7,     2,     2,     3,     4,
       6,     6,     9,     9,     6,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     4,     1,     3,     4,     1,
       3,     4,     3,     3,     1,     1,     2,     1,     2,     1,
       1,     2,     2,     2,     2,     2,     8,     8,     9,     9,
       4,     3,     3,     2,     2,     2,     1,     3,     4,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     4,     4,     4,     3,     3,     3,     3,     5,
       5,     3,     4,     3,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     4,     4,     5,
       4,     5,     3,     5,     3,     3,     3,     3,     3,     3,
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
#line 636 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6599 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 640 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6605 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 641 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6611 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 648 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6619 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 656 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6627 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 660 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6636 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 670 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6644 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 675 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6652 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 682 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6658 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 683 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6664 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 684 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6670 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 685 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6676 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6682 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 687 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6688 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 688 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6694 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: binary_implements_stmt  */
#line 690 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6700 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 691 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6706 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: TDEFER stmt  */
#line 693 "chpl.ypp"
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
#line 6723 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 705 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6729 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 706 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6735 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 708 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6748 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 717 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6761 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TLABEL ident_def stmt  */
#line 726 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6769 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 730 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6780 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: error TSEMI  */
#line 737 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6789 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: assignment_stmt  */
#line 744 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6795 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: if_stmt  */
#line 745 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6801 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: loop_stmt  */
#line 746 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6807 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: select_stmt  */
#line 747 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6813 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: manage_stmt  */
#line 748 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6819 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 749 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6825 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: throw_stmt  */
#line 750 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6831 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 752 "chpl.ypp"
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
#line 6849 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 766 "chpl.ypp"
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
#line 6868 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 781 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6880 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 789 "chpl.ypp"
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
#line 6896 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TLOCAL do_stmt  */
#line 801 "chpl.ypp"
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
#line 6911 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TON expr do_stmt  */
#line 812 "chpl.ypp"
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
#line 6927 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 824 "chpl.ypp"
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
#line 6943 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSERIAL do_stmt  */
#line 836 "chpl.ypp"
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
#line 6958 "bison-chpl-lib.cpp"
    break;

  case 44: /* tryable_stmt: TSYNC stmt  */
#line 847 "chpl.ypp"
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
#line 6975 "bison-chpl-lib.cpp"
    break;

  case 47: /* collect_attributes: %empty  */
#line 867 "chpl.ypp"
  {
    auto attributeGroup = context->buildAttributeGroup((yyloc));
    if (attributeGroup != nullptr) {
      context->resetAttributeGroupPartsState();
    }
    context->loopAttributes.push_back(std::move(attributeGroup));
  }
#line 6987 "bison-chpl-lib.cpp"
    break;

  case 53: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 892 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), true, (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6995 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace  */
#line 896 "chpl.ypp"
 {
  // this allows us to distinguish between @attribute() and @attribute
  // but should not be needed once `@unstable "msg"` is deprecated and gone
  auto lst = new MaybeNamedActualList();
  context->noteAttribute((yylsp[-1]), (yyvsp[-1].expr), false, (yyvsp[0].exprList), lst);
 }
#line 7006 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_stmt: attribute_decl_begin STRINGLITERAL  */
#line 903 "chpl.ypp"
 {
  // this allows us to maintain @unstable "message" without requiring parens
  // but should not be needed once `@unstable "msg"` is deprecated and gone
  auto lst = new MaybeNamedActualList();
  auto actual = makeMaybeNamedActual((yyvsp[0].expr));
  lst->push_back(actual);
  context->noteAttribute((yylsp[-1]), (yyvsp[-1].expr), false, context->makeList(), lst);
 }
#line 7019 "bison-chpl-lib.cpp"
    break;

  case 56: /* attribute_decl_begin: TATMARK TIDENT  */
#line 915 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 7028 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: %empty  */
#line 922 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 7036 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 926 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 7047 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP TRP  */
#line 936 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 7053 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 938 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 7061 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 945 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 7070 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 950 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 7080 "bison-chpl-lib.cpp"
    break;

  case 64: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 963 "chpl.ypp"
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
#line 7101 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 983 "chpl.ypp"
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
#line 7120 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 998 "chpl.ypp"
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
#line 7139 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1013 "chpl.ypp"
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
#line 7156 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1028 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 7162 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1029 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 7169 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1031 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 7176 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1036 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 7182 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1037 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 7189 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1043 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 7199 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1061 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 7207 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1065 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 7215 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR error TRCBR  */
#line 1069 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 7227 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt: block_stmt_body  */
#line 1080 "chpl.ypp"
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
#line 7246 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1098 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 7252 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1099 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7258 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1104 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 7266 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1108 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7274 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1112 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 7283 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1117 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 7291 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1121 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7299 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1125 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 7308 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1135 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7317 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1140 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7327 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1146 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7338 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1153 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 7347 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1158 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7358 "bison-chpl-lib.cpp"
    break;

  case 91: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1165 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7370 "bison-chpl-lib.cpp"
    break;

  case 92: /* opt_only_ls: %empty  */
#line 1175 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7376 "bison-chpl-lib.cpp"
    break;

  case 93: /* opt_only_ls: renames_ls  */
#line 1176 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7382 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1181 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7391 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1186 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7403 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1194 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7416 "bison-chpl-lib.cpp"
    break;

  case 97: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1203 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7430 "bison-chpl-lib.cpp"
    break;

  case 98: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1213 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7442 "bison-chpl-lib.cpp"
    break;

  case 99: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1221 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7455 "bison-chpl-lib.cpp"
    break;

  case 100: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1230 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7469 "bison-chpl-lib.cpp"
    break;

  case 101: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1243 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7478 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_expr: expr  */
#line 1279 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 7486 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_expr: expr TDOT all_op_name  */
#line 1283 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7495 "bison-chpl-lib.cpp"
    break;

  case 121: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1288 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7504 "bison-chpl-lib.cpp"
    break;

  case 122: /* import_expr: expr TAS ident_use  */
#line 1293 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 7514 "bison-chpl-lib.cpp"
    break;

  case 123: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1299 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 7524 "bison-chpl-lib.cpp"
    break;

  case 124: /* import_ls: import_expr  */
#line 1307 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7530 "bison-chpl-lib.cpp"
    break;

  case 125: /* import_ls: import_ls TCOMMA import_expr  */
#line 1308 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7536 "bison-chpl-lib.cpp"
    break;

  case 126: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1313 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7546 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1322 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7554 "bison-chpl-lib.cpp"
    break;

  case 128: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1326 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7562 "bison-chpl-lib.cpp"
    break;

  case 129: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1330 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7570 "bison-chpl-lib.cpp"
    break;

  case 130: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1334 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7578 "bison-chpl-lib.cpp"
    break;

  case 131: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1338 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7586 "bison-chpl-lib.cpp"
    break;

  case 132: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1342 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7594 "bison-chpl-lib.cpp"
    break;

  case 133: /* opt_label_ident: %empty  */
#line 1350 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7600 "bison-chpl-lib.cpp"
    break;

  case 134: /* opt_label_ident: TIDENT  */
#line 1351 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7606 "bison-chpl-lib.cpp"
    break;

  case 142: /* ident_use: TIDENT  */
#line 1376 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7612 "bison-chpl-lib.cpp"
    break;

  case 143: /* ident_use: TTHIS  */
#line 1377 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7618 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TBOOL  */
#line 1404 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7624 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TENUM  */
#line 1405 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7630 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TINT  */
#line 1406 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7636 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TUINT  */
#line 1407 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7642 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TREAL  */
#line 1408 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7648 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TIMAG  */
#line 1409 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7654 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TCOMPLEX  */
#line 1410 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7660 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TBYTES  */
#line 1411 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7666 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TSTRING  */
#line 1412 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7672 "bison-chpl-lib.cpp"
    break;

  case 172: /* scalar_type: TLOCALE  */
#line 1413 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7678 "bison-chpl-lib.cpp"
    break;

  case 173: /* scalar_type: TNOTHING  */
#line 1414 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7684 "bison-chpl-lib.cpp"
    break;

  case 174: /* scalar_type: TVOID  */
#line 1415 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7690 "bison-chpl-lib.cpp"
    break;

  case 179: /* do_stmt: TDO stmt  */
#line 1429 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7696 "bison-chpl-lib.cpp"
    break;

  case 180: /* do_stmt: block_stmt  */
#line 1430 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7702 "bison-chpl-lib.cpp"
    break;

  case 181: /* return_stmt: TRETURN TSEMI  */
#line 1435 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7713 "bison-chpl-lib.cpp"
    break;

  case 182: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1442 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7724 "bison-chpl-lib.cpp"
    break;

  case 183: /* class_level_stmt: TSEMI  */
#line 1452 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7732 "bison-chpl-lib.cpp"
    break;

  case 184: /* class_level_stmt: inner_class_level_stmt  */
#line 1456 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7742 "bison-chpl-lib.cpp"
    break;

  case 185: /* $@1: %empty  */
#line 1461 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7749 "bison-chpl-lib.cpp"
    break;

  case 186: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1463 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7758 "bison-chpl-lib.cpp"
    break;

  case 187: /* $@2: %empty  */
#line 1467 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7765 "bison-chpl-lib.cpp"
    break;

  case 188: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1469 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7774 "bison-chpl-lib.cpp"
    break;

  case 197: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1488 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7783 "bison-chpl-lib.cpp"
    break;

  case 198: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1493 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7792 "bison-chpl-lib.cpp"
    break;

  case 199: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1498 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7801 "bison-chpl-lib.cpp"
    break;

  case 200: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1503 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7809 "bison-chpl-lib.cpp"
    break;

  case 201: /* forwarding_decl_start: TFORWARDING  */
#line 1510 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7818 "bison-chpl-lib.cpp"
    break;

  case 202: /* extern_or_export: TEXTERN  */
#line 1517 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7824 "bison-chpl-lib.cpp"
    break;

  case 203: /* extern_or_export: TEXPORT  */
#line 1518 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7830 "bison-chpl-lib.cpp"
    break;

  case 204: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1523 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7840 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1533 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7849 "bison-chpl-lib.cpp"
    break;

  case 206: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1539 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7860 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1546 "chpl.ypp"
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
#line 7882 "bison-chpl-lib.cpp"
    break;

  case 208: /* $@3: %empty  */
#line 1564 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7890 "bison-chpl-lib.cpp"
    break;

  case 209: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1568 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7899 "bison-chpl-lib.cpp"
    break;

  case 210: /* $@4: %empty  */
#line 1573 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7908 "bison-chpl-lib.cpp"
    break;

  case 211: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1578 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7917 "bison-chpl-lib.cpp"
    break;

  case 212: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1586 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7925 "bison-chpl-lib.cpp"
    break;

  case 213: /* no_loop_attributes: %empty  */
#line 1591 "chpl.ypp"
                    { context->loopAttributes.push_back(nullptr); }
#line 7931 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1594 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7937 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1595 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7943 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1600 "chpl.ypp"
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
#line 7966 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1619 "chpl.ypp"
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
#line 7984 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1633 "chpl.ypp"
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
#line 8002 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1647 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8010 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1651 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8018 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1655 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8026 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: TFOR expr TIN expr do_stmt  */
#line 1659 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8034 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: TFOR expr TIN zippered_iterator do_stmt  */
#line 1663 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8042 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: TFOR expr do_stmt  */
#line 1667 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8050 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: TFOR zippered_iterator do_stmt  */
#line 1671 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8058 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1675 "chpl.ypp"
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
#line 8082 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: TFORALL expr TIN expr do_stmt  */
#line 1695 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8090 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1699 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8098 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1703 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8106 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1707 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8114 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: TFORALL expr do_stmt  */
#line 1711 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8122 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TFORALL expr forall_intent_clause do_stmt  */
#line 1715 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8130 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TFORALL zippered_iterator do_stmt  */
#line 1719 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8138 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1723 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8146 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: TFOREACH expr TIN expr do_stmt  */
#line 1727 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8154 "bison-chpl-lib.cpp"
    break;

  case 236: /* loop_stmt_base: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1731 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8162 "bison-chpl-lib.cpp"
    break;

  case 237: /* loop_stmt_base: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1735 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8170 "bison-chpl-lib.cpp"
    break;

  case 238: /* loop_stmt_base: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1739 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8178 "bison-chpl-lib.cpp"
    break;

  case 239: /* loop_stmt_base: TFOREACH expr do_stmt  */
#line 1743 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8186 "bison-chpl-lib.cpp"
    break;

  case 240: /* loop_stmt_base: TFOREACH expr forall_intent_clause do_stmt  */
#line 1747 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8194 "bison-chpl-lib.cpp"
    break;

  case 241: /* loop_stmt_base: TFOREACH zippered_iterator do_stmt  */
#line 1751 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8202 "bison-chpl-lib.cpp"
    break;

  case 242: /* loop_stmt_base: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1755 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8210 "bison-chpl-lib.cpp"
    break;

  case 243: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1759 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8218 "bison-chpl-lib.cpp"
    break;

  case 244: /* loop_stmt_base: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1763 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8226 "bison-chpl-lib.cpp"
    break;

  case 245: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1767 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8234 "bison-chpl-lib.cpp"
    break;

  case 246: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1771 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8242 "bison-chpl-lib.cpp"
    break;

  case 247: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1775 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8250 "bison-chpl-lib.cpp"
    break;

  case 248: /* loop_stmt_base: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1779 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8258 "bison-chpl-lib.cpp"
    break;

  case 249: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1783 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8267 "bison-chpl-lib.cpp"
    break;

  case 250: /* loop_stmt_base: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1788 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8276 "bison-chpl-lib.cpp"
    break;

  case 251: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1796 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 8286 "bison-chpl-lib.cpp"
    break;

  case 252: /* if_stmt: TIF expr TTHEN stmt  */
#line 1805 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8294 "bison-chpl-lib.cpp"
    break;

  case 253: /* if_stmt: TIF expr block_stmt  */
#line 1809 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8302 "bison-chpl-lib.cpp"
    break;

  case 254: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1813 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8310 "bison-chpl-lib.cpp"
    break;

  case 255: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1817 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8318 "bison-chpl-lib.cpp"
    break;

  case 256: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1821 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8326 "bison-chpl-lib.cpp"
    break;

  case 257: /* if_stmt: TIF ifvar block_stmt  */
#line 1825 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8334 "bison-chpl-lib.cpp"
    break;

  case 258: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1829 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8342 "bison-chpl-lib.cpp"
    break;

  case 259: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1833 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8350 "bison-chpl-lib.cpp"
    break;

  case 260: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1837 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8359 "bison-chpl-lib.cpp"
    break;

  case 261: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1842 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8368 "bison-chpl-lib.cpp"
    break;

  case 262: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1847 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8377 "bison-chpl-lib.cpp"
    break;

  case 263: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1852 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8386 "bison-chpl-lib.cpp"
    break;

  case 264: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1860 "chpl.ypp"
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
#line 8405 "bison-chpl-lib.cpp"
    break;

  case 265: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1875 "chpl.ypp"
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
#line 8424 "bison-chpl-lib.cpp"
    break;

  case 266: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1893 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8432 "bison-chpl-lib.cpp"
    break;

  case 267: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1897 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8440 "bison-chpl-lib.cpp"
    break;

  case 268: /* ifc_formal_ls: ifc_formal  */
#line 1903 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8446 "bison-chpl-lib.cpp"
    break;

  case 269: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1904 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8452 "bison-chpl-lib.cpp"
    break;

  case 270: /* ifc_formal: ident_def  */
#line 1909 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8458 "bison-chpl-lib.cpp"
    break;

  case 283: /* implements_type_ident: implements_type_error_ident  */
#line 1927 "chpl.ypp"
  {
    context->syntax((yyloc), "type '%s' cannot implement an interface.", (yyvsp[0].uniqueStr).c_str());
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8467 "bison-chpl-lib.cpp"
    break;

  case 290: /* unary_implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1953 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8475 "bison-chpl-lib.cpp"
    break;

  case 291: /* unary_implements_stmt: TIMPLEMENTS ident_def TSEMI  */
#line 1957 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 8483 "bison-chpl-lib.cpp"
    break;

  case 292: /* binary_implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1964 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 8491 "bison-chpl-lib.cpp"
    break;

  case 293: /* binary_implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1968 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8499 "bison-chpl-lib.cpp"
    break;

  case 294: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1975 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8507 "bison-chpl-lib.cpp"
    break;

  case 295: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1979 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 8515 "bison-chpl-lib.cpp"
    break;

  case 296: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1983 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8523 "bison-chpl-lib.cpp"
    break;

  case 297: /* try_stmt: TTRY tryable_stmt  */
#line 1990 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8531 "bison-chpl-lib.cpp"
    break;

  case 298: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1994 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8539 "bison-chpl-lib.cpp"
    break;

  case 299: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1998 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8547 "bison-chpl-lib.cpp"
    break;

  case 300: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 2002 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8555 "bison-chpl-lib.cpp"
    break;

  case 301: /* catch_expr_ls: %empty  */
#line 2008 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8561 "bison-chpl-lib.cpp"
    break;

  case 302: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2009 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8567 "bison-chpl-lib.cpp"
    break;

  case 303: /* catch_expr: TCATCH block_stmt  */
#line 2014 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8575 "bison-chpl-lib.cpp"
    break;

  case 304: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2018 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8583 "bison-chpl-lib.cpp"
    break;

  case 305: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2022 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8591 "bison-chpl-lib.cpp"
    break;

  case 306: /* catch_expr_inner: ident_def  */
#line 2029 "chpl.ypp"
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
#line 8608 "bison-chpl-lib.cpp"
    break;

  case 307: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2042 "chpl.ypp"
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
#line 8625 "bison-chpl-lib.cpp"
    break;

  case 308: /* throw_stmt: TTHROW expr TSEMI  */
#line 2058 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8635 "bison-chpl-lib.cpp"
    break;

  case 309: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2066 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8643 "bison-chpl-lib.cpp"
    break;

  case 310: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2070 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8653 "bison-chpl-lib.cpp"
    break;

  case 311: /* when_stmt_ls: %empty  */
#line 2078 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8659 "bison-chpl-lib.cpp"
    break;

  case 312: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2079 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8665 "bison-chpl-lib.cpp"
    break;

  case 313: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2084 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8673 "bison-chpl-lib.cpp"
    break;

  case 314: /* when_stmt: TOTHERWISE stmt  */
#line 2088 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8682 "bison-chpl-lib.cpp"
    break;

  case 315: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2093 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 8692 "bison-chpl-lib.cpp"
    break;

  case 316: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2102 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8701 "bison-chpl-lib.cpp"
    break;

  case 317: /* manager_expr: expr TAS ident_def  */
#line 2107 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8709 "bison-chpl-lib.cpp"
    break;

  case 318: /* manager_expr: expr  */
#line 2111 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8717 "bison-chpl-lib.cpp"
    break;

  case 319: /* manager_expr_ls: manager_expr  */
#line 2117 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8723 "bison-chpl-lib.cpp"
    break;

  case 320: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2118 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8729 "bison-chpl-lib.cpp"
    break;

  case 321: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2123 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8737 "bison-chpl-lib.cpp"
    break;

  case 322: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2132 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8746 "bison-chpl-lib.cpp"
    break;

  case 323: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2137 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8757 "bison-chpl-lib.cpp"
    break;

  case 324: /* class_start: class_tag ident_def  */
#line 2148 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8765 "bison-chpl-lib.cpp"
    break;

  case 325: /* class_tag: TCLASS  */
#line 2154 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8771 "bison-chpl-lib.cpp"
    break;

  case 326: /* class_tag: TRECORD  */
#line 2155 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8777 "bison-chpl-lib.cpp"
    break;

  case 327: /* class_tag: TUNION  */
#line 2156 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8783 "bison-chpl-lib.cpp"
    break;

  case 328: /* opt_inherit: %empty  */
#line 2160 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8789 "bison-chpl-lib.cpp"
    break;

  case 329: /* opt_inherit: TCOLON expr_ls  */
#line 2161 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8795 "bison-chpl-lib.cpp"
    break;

  case 330: /* class_level_stmt_ls: %empty  */
#line 2165 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8804 "bison-chpl-lib.cpp"
    break;

  case 331: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2170 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8812 "bison-chpl-lib.cpp"
    break;

  case 332: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2174 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8820 "bison-chpl-lib.cpp"
    break;

  case 333: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2178 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8828 "bison-chpl-lib.cpp"
    break;

  case 334: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2182 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8836 "bison-chpl-lib.cpp"
    break;

  case 335: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2189 "chpl.ypp"
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
#line 8857 "bison-chpl-lib.cpp"
    break;

  case 336: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2206 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8871 "bison-chpl-lib.cpp"
    break;

  case 337: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2219 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8879 "bison-chpl-lib.cpp"
    break;

  case 338: /* enum_ls: enum_item  */
#line 2226 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8888 "bison-chpl-lib.cpp"
    break;

  case 339: /* enum_ls: enum_ls TCOMMA  */
#line 2231 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8898 "bison-chpl-lib.cpp"
    break;

  case 340: /* $@5: %empty  */
#line 2237 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8907 "bison-chpl-lib.cpp"
    break;

  case 341: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2242 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8916 "bison-chpl-lib.cpp"
    break;

  case 342: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2247 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8925 "bison-chpl-lib.cpp"
    break;

  case 343: /* $@6: %empty  */
#line 2252 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8934 "bison-chpl-lib.cpp"
    break;

  case 344: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2257 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8943 "bison-chpl-lib.cpp"
    break;

  case 345: /* enum_item: ident_def  */
#line 2267 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8954 "bison-chpl-lib.cpp"
    break;

  case 346: /* enum_item: ident_def TASSIGN expr  */
#line 2274 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8967 "bison-chpl-lib.cpp"
    break;

  case 347: /* lambda_decl_start: TLAMBDA  */
#line 2286 "chpl.ypp"
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
#line 8983 "bison-chpl-lib.cpp"
    break;

  case 348: /* $@7: %empty  */
#line 2303 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8993 "bison-chpl-lib.cpp"
    break;

  case 349: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2309 "chpl.ypp"
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
#line 9015 "bison-chpl-lib.cpp"
    break;

  case 351: /* linkage_spec: linkage_spec_empty  */
#line 2332 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 9021 "bison-chpl-lib.cpp"
    break;

  case 352: /* linkage_spec: TINLINE  */
#line 2333 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 9028 "bison-chpl-lib.cpp"
    break;

  case 353: /* linkage_spec: TOVERRIDE  */
#line 2335 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 9035 "bison-chpl-lib.cpp"
    break;

  case 354: /* opt_fn_type_formal_ls: %empty  */
#line 2340 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 9041 "bison-chpl-lib.cpp"
    break;

  case 355: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2341 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9047 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_type_formal_ls: fn_type_formal  */
#line 2345 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9053 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2346 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9059 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_type_formal: named_formal  */
#line 2351 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9065 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2354 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 9071 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_type_formal: formal_type  */
#line 2356 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 9077 "bison-chpl-lib.cpp"
    break;

  case 361: /* opt_fn_type_ret_type: %empty  */
#line 2360 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 9083 "bison-chpl-lib.cpp"
    break;

  case 362: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2361 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9089 "bison-chpl-lib.cpp"
    break;

  case 363: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2374 "chpl.ypp"
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
#line 9108 "bison-chpl-lib.cpp"
    break;

  case 364: /* $@8: %empty  */
#line 2392 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 9118 "bison-chpl-lib.cpp"
    break;

  case 365: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2398 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 9131 "bison-chpl-lib.cpp"
    break;

  case 366: /* $@9: %empty  */
#line 2407 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 9141 "bison-chpl-lib.cpp"
    break;

  case 367: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2413 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 9154 "bison-chpl-lib.cpp"
    break;

  case 368: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2425 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 9162 "bison-chpl-lib.cpp"
    break;

  case 369: /* $@10: %empty  */
#line 2434 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();

    // May never have been built if there was a syntax error.
    if ((yyvsp[-4].functionParts).errorExpr == nullptr) {
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name->name());
    }
  }
#line 9176 "bison-chpl-lib.cpp"
    break;

  case 370: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2444 "chpl.ypp"
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
#line 9199 "bison-chpl-lib.cpp"
    break;

  case 371: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2466 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9211 "bison-chpl-lib.cpp"
    break;

  case 372: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2474 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9223 "bison-chpl-lib.cpp"
    break;

  case 373: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2482 "chpl.ypp"
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
#line 9238 "bison-chpl-lib.cpp"
    break;

  case 374: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2493 "chpl.ypp"
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
#line 9253 "bison-chpl-lib.cpp"
    break;

  case 375: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2504 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 9263 "bison-chpl-lib.cpp"
    break;

  case 376: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2513 "chpl.ypp"
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
#line 9279 "bison-chpl-lib.cpp"
    break;

  case 378: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2528 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 9285 "bison-chpl-lib.cpp"
    break;

  case 381: /* fn_ident: ident_def TBANG  */
#line 2535 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 9295 "bison-chpl-lib.cpp"
    break;

  case 421: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2592 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 9301 "bison-chpl-lib.cpp"
    break;

  case 422: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2593 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9307 "bison-chpl-lib.cpp"
    break;

  case 423: /* opt_formal_ls: %empty  */
#line 2597 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 9313 "bison-chpl-lib.cpp"
    break;

  case 424: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2598 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9319 "bison-chpl-lib.cpp"
    break;

  case 425: /* req_formal_ls: TLP TRP  */
#line 2602 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 9325 "bison-chpl-lib.cpp"
    break;

  case 426: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2603 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9331 "bison-chpl-lib.cpp"
    break;

  case 427: /* formal_ls_inner: formal  */
#line 2607 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9337 "bison-chpl-lib.cpp"
    break;

  case 428: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2608 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9343 "bison-chpl-lib.cpp"
    break;

  case 429: /* formal_ls: %empty  */
#line 2612 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 9349 "bison-chpl-lib.cpp"
    break;

  case 430: /* formal_ls: formal_ls_inner  */
#line 2613 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9355 "bison-chpl-lib.cpp"
    break;

  case 434: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2627 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9363 "bison-chpl-lib.cpp"
    break;

  case 435: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2632 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 9371 "bison-chpl-lib.cpp"
    break;

  case 436: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2637 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9379 "bison-chpl-lib.cpp"
    break;

  case 437: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2642 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 9387 "bison-chpl-lib.cpp"
    break;

  case 438: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2647 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9395 "bison-chpl-lib.cpp"
    break;

  case 439: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2652 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 9403 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_formal_intent_tag: %empty  */
#line 2658 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 9412 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_formal_intent_tag: required_intent_tag  */
#line 2663 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 9421 "bison-chpl-lib.cpp"
    break;

  case 442: /* required_intent_tag: TIN  */
#line 2670 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 9427 "bison-chpl-lib.cpp"
    break;

  case 443: /* required_intent_tag: TINOUT  */
#line 2671 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 9433 "bison-chpl-lib.cpp"
    break;

  case 444: /* required_intent_tag: TOUT  */
#line 2672 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 9439 "bison-chpl-lib.cpp"
    break;

  case 445: /* required_intent_tag: TCONST TIN  */
#line 2673 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 9445 "bison-chpl-lib.cpp"
    break;

  case 446: /* required_intent_tag: TCONST TREF  */
#line 2674 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9451 "bison-chpl-lib.cpp"
    break;

  case 447: /* required_intent_tag: TCONST  */
#line 2675 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9457 "bison-chpl-lib.cpp"
    break;

  case 448: /* required_intent_tag: TPARAM  */
#line 2676 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9463 "bison-chpl-lib.cpp"
    break;

  case 449: /* required_intent_tag: TREF  */
#line 2677 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9469 "bison-chpl-lib.cpp"
    break;

  case 450: /* required_intent_tag: TTYPE  */
#line 2678 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9475 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_this_intent_tag: %empty  */
#line 2682 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9481 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_this_intent_tag: TPARAM  */
#line 2683 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9487 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_this_intent_tag: TREF  */
#line 2684 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9493 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_this_intent_tag: TCONST TREF  */
#line 2685 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9499 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_this_intent_tag: TCONST  */
#line 2686 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9505 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_this_intent_tag: TTYPE  */
#line 2687 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9511 "bison-chpl-lib.cpp"
    break;

  case 457: /* proc_iter_or_op: TPROC  */
#line 2691 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9517 "bison-chpl-lib.cpp"
    break;

  case 458: /* proc_iter_or_op: TITER  */
#line 2692 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9523 "bison-chpl-lib.cpp"
    break;

  case 459: /* proc_iter_or_op: TOPERATOR  */
#line 2693 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9529 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_ret_tag: %empty  */
#line 2697 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9535 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_ret_tag: TOUT  */
#line 2698 "chpl.ypp"
                    { (yyval.returnTag) = Function::OUT; }
#line 9541 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_ret_tag: TCONST  */
#line 2699 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 9547 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_ret_tag: TCONST TREF  */
#line 2700 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 9553 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_ret_tag: TREF  */
#line 2701 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 9559 "bison-chpl-lib.cpp"
    break;

  case 465: /* opt_ret_tag: TPARAM  */
#line 2702 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 9565 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_ret_tag: TTYPE  */
#line 2703 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 9571 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_throws_error: %empty  */
#line 2707 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9577 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_throws_error: TTHROWS  */
#line 2708 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9583 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_function_body_stmt: TSEMI  */
#line 2711 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9589 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_function_body_stmt: function_body_stmt  */
#line 2712 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9595 "bison-chpl-lib.cpp"
    break;

  case 471: /* function_body_stmt: block_stmt_body  */
#line 2716 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9601 "bison-chpl-lib.cpp"
    break;

  case 472: /* function_body_stmt: TDO toplevel_stmt  */
#line 2717 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 9607 "bison-chpl-lib.cpp"
    break;

  case 473: /* function_body_stmt: return_stmt  */
#line 2718 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt), &((yylsp[0]))); }
#line 9613 "bison-chpl-lib.cpp"
    break;

  case 474: /* query_expr: TQUERIEDIDENT  */
#line 2722 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9619 "bison-chpl-lib.cpp"
    break;

  case 475: /* opt_lifetime_where: %empty  */
#line 2727 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9625 "bison-chpl-lib.cpp"
    break;

  case 476: /* opt_lifetime_where: TWHERE expr  */
#line 2729 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9631 "bison-chpl-lib.cpp"
    break;

  case 477: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2731 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9637 "bison-chpl-lib.cpp"
    break;

  case 478: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2733 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9643 "bison-chpl-lib.cpp"
    break;

  case 479: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2735 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9649 "bison-chpl-lib.cpp"
    break;

  case 480: /* lifetime_components_expr: lifetime_expr  */
#line 2740 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9655 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2742 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9661 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2747 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9667 "bison-chpl-lib.cpp"
    break;

  case 483: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2749 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9673 "bison-chpl-lib.cpp"
    break;

  case 484: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2751 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9679 "bison-chpl-lib.cpp"
    break;

  case 485: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2753 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9685 "bison-chpl-lib.cpp"
    break;

  case 486: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2755 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9691 "bison-chpl-lib.cpp"
    break;

  case 487: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2757 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9697 "bison-chpl-lib.cpp"
    break;

  case 488: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2759 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9703 "bison-chpl-lib.cpp"
    break;

  case 489: /* lifetime_ident: TIDENT  */
#line 2763 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9709 "bison-chpl-lib.cpp"
    break;

  case 490: /* lifetime_ident: TTHIS  */
#line 2764 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9715 "bison-chpl-lib.cpp"
    break;

  case 491: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2769 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9724 "bison-chpl-lib.cpp"
    break;

  case 492: /* type_alias_decl_stmt_start: TTYPE  */
#line 2778 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9732 "bison-chpl-lib.cpp"
    break;

  case 493: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2782 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9741 "bison-chpl-lib.cpp"
    break;

  case 494: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2790 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9749 "bison-chpl-lib.cpp"
    break;

  case 495: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2794 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9757 "bison-chpl-lib.cpp"
    break;

  case 496: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2801 "chpl.ypp"
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
#line 9779 "bison-chpl-lib.cpp"
    break;

  case 497: /* opt_init_type: %empty  */
#line 2821 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9785 "bison-chpl-lib.cpp"
    break;

  case 498: /* opt_init_type: TASSIGN expr  */
#line 2823 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9791 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_type: TPARAM  */
#line 2827 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9797 "bison-chpl-lib.cpp"
    break;

  case 500: /* var_decl_type: TCONST TREF  */
#line 2828 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9803 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_type: TREF  */
#line 2829 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9809 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_type: TCONST  */
#line 2830 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9815 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_type: TVAR  */
#line 2831 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9821 "bison-chpl-lib.cpp"
    break;

  case 504: /* $@11: %empty  */
#line 2836 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9830 "bison-chpl-lib.cpp"
    break;

  case 505: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2840 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9839 "bison-chpl-lib.cpp"
    break;

  case 506: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2845 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9848 "bison-chpl-lib.cpp"
    break;

  case 507: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2853 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9856 "bison-chpl-lib.cpp"
    break;

  case 508: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2857 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9864 "bison-chpl-lib.cpp"
    break;

  case 509: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2864 "chpl.ypp"
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
#line 9886 "bison-chpl-lib.cpp"
    break;

  case 510: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2882 "chpl.ypp"
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
#line 9906 "bison-chpl-lib.cpp"
    break;

  case 511: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2901 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9914 "bison-chpl-lib.cpp"
    break;

  case 512: /* tuple_var_decl_component: ident_def  */
#line 2905 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9922 "bison-chpl-lib.cpp"
    break;

  case 513: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2909 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9930 "bison-chpl-lib.cpp"
    break;

  case 514: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2916 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9936 "bison-chpl-lib.cpp"
    break;

  case 515: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2918 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9942 "bison-chpl-lib.cpp"
    break;

  case 516: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2920 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9948 "bison-chpl-lib.cpp"
    break;

  case 517: /* opt_init_expr: %empty  */
#line 2926 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9954 "bison-chpl-lib.cpp"
    break;

  case 518: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2927 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9960 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2928 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9966 "bison-chpl-lib.cpp"
    break;

  case 520: /* formal_or_ret_type_expr: expr  */
#line 2932 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9972 "bison-chpl-lib.cpp"
    break;

  case 521: /* ret_type: formal_or_ret_type_expr  */
#line 2936 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9978 "bison-chpl-lib.cpp"
    break;

  case 522: /* ret_type: reserved_type_ident_use  */
#line 2937 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9984 "bison-chpl-lib.cpp"
    break;

  case 523: /* ret_type: error  */
#line 2938 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9990 "bison-chpl-lib.cpp"
    break;

  case 524: /* colon_ret_type: TCOLON ret_type  */
#line 2942 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9996 "bison-chpl-lib.cpp"
    break;

  case 525: /* colon_ret_type: error  */
#line 2943 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 10002 "bison-chpl-lib.cpp"
    break;

  case 526: /* opt_ret_type: %empty  */
#line 2947 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 10008 "bison-chpl-lib.cpp"
    break;

  case 528: /* opt_type: %empty  */
#line 2952 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 10014 "bison-chpl-lib.cpp"
    break;

  case 529: /* opt_type: TCOLON expr  */
#line 2953 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10020 "bison-chpl-lib.cpp"
    break;

  case 530: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2954 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10026 "bison-chpl-lib.cpp"
    break;

  case 531: /* opt_type: error  */
#line 2955 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 10032 "bison-chpl-lib.cpp"
    break;

  case 532: /* formal_type: formal_or_ret_type_expr  */
#line 2959 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 10038 "bison-chpl-lib.cpp"
    break;

  case 533: /* formal_type: reserved_type_ident_use  */
#line 2960 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10044 "bison-chpl-lib.cpp"
    break;

  case 534: /* colon_formal_type: TCOLON formal_type  */
#line 2964 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10050 "bison-chpl-lib.cpp"
    break;

  case 535: /* opt_colon_formal_type: %empty  */
#line 2968 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 10056 "bison-chpl-lib.cpp"
    break;

  case 536: /* opt_colon_formal_type: colon_formal_type  */
#line 2969 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 10062 "bison-chpl-lib.cpp"
    break;

  case 537: /* expr_ls: expr  */
#line 2975 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10068 "bison-chpl-lib.cpp"
    break;

  case 538: /* expr_ls: expr_ls TCOMMA expr  */
#line 2976 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10074 "bison-chpl-lib.cpp"
    break;

  case 539: /* tuple_component: TUNDERSCORE  */
#line 2980 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10080 "bison-chpl-lib.cpp"
    break;

  case 540: /* tuple_component: opt_try_expr  */
#line 2981 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 10086 "bison-chpl-lib.cpp"
    break;

  case 541: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2986 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 10094 "bison-chpl-lib.cpp"
    break;

  case 542: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2990 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10102 "bison-chpl-lib.cpp"
    break;

  case 543: /* opt_actual_ls: %empty  */
#line 2996 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 10108 "bison-chpl-lib.cpp"
    break;

  case 544: /* opt_actual_ls: actual_ls  */
#line 2997 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 10114 "bison-chpl-lib.cpp"
    break;

  case 545: /* actual_ls: actual_expr  */
#line 3002 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10123 "bison-chpl-lib.cpp"
    break;

  case 546: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3007 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10133 "bison-chpl-lib.cpp"
    break;

  case 547: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3015 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10139 "bison-chpl-lib.cpp"
    break;

  case 548: /* actual_expr: opt_try_expr  */
#line 3016 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10145 "bison-chpl-lib.cpp"
    break;

  case 549: /* ident_expr: ident_use  */
#line 3020 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10151 "bison-chpl-lib.cpp"
    break;

  case 550: /* ident_expr: scalar_type  */
#line 3021 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10157 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TSINGLE expr  */
#line 3034 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10163 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3036 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10169 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3038 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10175 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3040 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10181 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 3042 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 10191 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TATOMIC expr  */
#line 3048 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10197 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TSYNC expr  */
#line 3050 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10203 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TOWNED  */
#line 3053 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10209 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TOWNED expr  */
#line 3055 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10215 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TUNMANAGED  */
#line 3057 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10221 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3059 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10227 "bison-chpl-lib.cpp"
    break;

  case 567: /* sub_type_level_expr: TSHARED  */
#line 3061 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10233 "bison-chpl-lib.cpp"
    break;

  case 568: /* sub_type_level_expr: TSHARED expr  */
#line 3063 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10239 "bison-chpl-lib.cpp"
    break;

  case 569: /* sub_type_level_expr: TBORROWED  */
#line 3065 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10245 "bison-chpl-lib.cpp"
    break;

  case 570: /* sub_type_level_expr: TBORROWED expr  */
#line 3067 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10251 "bison-chpl-lib.cpp"
    break;

  case 571: /* sub_type_level_expr: TCLASS  */
#line 3069 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10257 "bison-chpl-lib.cpp"
    break;

  case 572: /* sub_type_level_expr: TRECORD  */
#line 3071 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10263 "bison-chpl-lib.cpp"
    break;

  case 573: /* for_expr: no_loop_attributes for_expr_base  */
#line 3075 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 10269 "bison-chpl-lib.cpp"
    break;

  case 574: /* for_expr_base: TFOR expr TIN expr TDO expr  */
#line 3080 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10283 "bison-chpl-lib.cpp"
    break;

  case 575: /* for_expr_base: TFOR expr TIN zippered_iterator TDO expr  */
#line 3090 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10297 "bison-chpl-lib.cpp"
    break;

  case 576: /* for_expr_base: TFOR expr TDO expr  */
#line 3100 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10310 "bison-chpl-lib.cpp"
    break;

  case 577: /* for_expr_base: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 3109 "chpl.ypp"
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
#line 10329 "bison-chpl-lib.cpp"
    break;

  case 578: /* for_expr_base: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3124 "chpl.ypp"
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
#line 10348 "bison-chpl-lib.cpp"
    break;

  case 579: /* for_expr_base: TFOR expr TDO TIF expr TTHEN expr  */
#line 3139 "chpl.ypp"
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
#line 10367 "bison-chpl-lib.cpp"
    break;

  case 580: /* for_expr_base: TFORALL expr TIN expr TDO expr  */
#line 3154 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10381 "bison-chpl-lib.cpp"
    break;

  case 581: /* for_expr_base: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3164 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10395 "bison-chpl-lib.cpp"
    break;

  case 582: /* for_expr_base: TFORALL expr TDO expr  */
#line 3174 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10408 "bison-chpl-lib.cpp"
    break;

  case 583: /* for_expr_base: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3183 "chpl.ypp"
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
#line 10427 "bison-chpl-lib.cpp"
    break;

  case 584: /* for_expr_base: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3198 "chpl.ypp"
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
#line 10446 "bison-chpl-lib.cpp"
    break;

  case 585: /* for_expr_base: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3213 "chpl.ypp"
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
#line 10465 "bison-chpl-lib.cpp"
    break;

  case 586: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3230 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 10471 "bison-chpl-lib.cpp"
    break;

  case 587: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3235 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 10479 "bison-chpl-lib.cpp"
    break;

  case 588: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3239 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 10487 "bison-chpl-lib.cpp"
    break;

  case 589: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3243 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10495 "bison-chpl-lib.cpp"
    break;

  case 590: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3247 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10503 "bison-chpl-lib.cpp"
    break;

  case 591: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3251 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10511 "bison-chpl-lib.cpp"
    break;

  case 592: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3255 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10519 "bison-chpl-lib.cpp"
    break;

  case 593: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3259 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10527 "bison-chpl-lib.cpp"
    break;

  case 594: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3266 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10541 "bison-chpl-lib.cpp"
    break;

  case 595: /* nil_expr: TNIL  */
#line 3283 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10547 "bison-chpl-lib.cpp"
    break;

  case 603: /* opt_task_intent_ls: %empty  */
#line 3301 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10553 "bison-chpl-lib.cpp"
    break;

  case 604: /* opt_task_intent_ls: task_intent_clause  */
#line 3302 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10559 "bison-chpl-lib.cpp"
    break;

  case 605: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3307 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10569 "bison-chpl-lib.cpp"
    break;

  case 606: /* task_intent_ls: intent_expr  */
#line 3315 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10575 "bison-chpl-lib.cpp"
    break;

  case 607: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3316 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10581 "bison-chpl-lib.cpp"
    break;

  case 608: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3321 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10591 "bison-chpl-lib.cpp"
    break;

  case 609: /* forall_intent_ls: intent_expr  */
#line 3329 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10597 "bison-chpl-lib.cpp"
    break;

  case 610: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3330 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10603 "bison-chpl-lib.cpp"
    break;

  case 611: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3335 "chpl.ypp"
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
#line 10621 "bison-chpl-lib.cpp"
    break;

  case 612: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3349 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10629 "bison-chpl-lib.cpp"
    break;

  case 613: /* intent_expr: expr TREDUCE ident_expr  */
#line 3353 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10637 "bison-chpl-lib.cpp"
    break;

  case 614: /* task_var_prefix: TCONST  */
#line 3359 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10643 "bison-chpl-lib.cpp"
    break;

  case 615: /* task_var_prefix: TIN  */
#line 3360 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10649 "bison-chpl-lib.cpp"
    break;

  case 616: /* task_var_prefix: TCONST TIN  */
#line 3361 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10655 "bison-chpl-lib.cpp"
    break;

  case 617: /* task_var_prefix: TREF  */
#line 3362 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10661 "bison-chpl-lib.cpp"
    break;

  case 618: /* task_var_prefix: TCONST TREF  */
#line 3363 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10667 "bison-chpl-lib.cpp"
    break;

  case 619: /* task_var_prefix: TVAR  */
#line 3364 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10673 "bison-chpl-lib.cpp"
    break;

  case 620: /* new_maybe_decorated: TNEW  */
#line 3369 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10679 "bison-chpl-lib.cpp"
    break;

  case 621: /* new_maybe_decorated: TNEW TOWNED  */
#line 3371 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10685 "bison-chpl-lib.cpp"
    break;

  case 622: /* new_maybe_decorated: TNEW TSHARED  */
#line 3373 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10691 "bison-chpl-lib.cpp"
    break;

  case 623: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3375 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10697 "bison-chpl-lib.cpp"
    break;

  case 624: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3377 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10703 "bison-chpl-lib.cpp"
    break;

  case 625: /* new_expr: new_maybe_decorated expr  */
#line 3383 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10711 "bison-chpl-lib.cpp"
    break;

  case 626: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3390 "chpl.ypp"
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
#line 10727 "bison-chpl-lib.cpp"
    break;

  case 627: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3402 "chpl.ypp"
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
#line 10743 "bison-chpl-lib.cpp"
    break;

  case 628: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3414 "chpl.ypp"
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
#line 10760 "bison-chpl-lib.cpp"
    break;

  case 629: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3427 "chpl.ypp"
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
#line 10778 "bison-chpl-lib.cpp"
    break;

  case 630: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3444 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10786 "bison-chpl-lib.cpp"
    break;

  case 631: /* range_literal_expr: expr TDOTDOT expr  */
#line 3451 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10795 "bison-chpl-lib.cpp"
    break;

  case 632: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3456 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10804 "bison-chpl-lib.cpp"
    break;

  case 633: /* range_literal_expr: expr TDOTDOT  */
#line 3461 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10813 "bison-chpl-lib.cpp"
    break;

  case 634: /* range_literal_expr: TDOTDOT expr  */
#line 3466 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10823 "bison-chpl-lib.cpp"
    break;

  case 635: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3472 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10833 "bison-chpl-lib.cpp"
    break;

  case 636: /* range_literal_expr: TDOTDOT  */
#line 3478 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10843 "bison-chpl-lib.cpp"
    break;

  case 637: /* cast_expr: expr TCOLON expr  */
#line 3508 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10851 "bison-chpl-lib.cpp"
    break;

  case 638: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3515 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10859 "bison-chpl-lib.cpp"
    break;

  case 639: /* super_expr: fn_expr  */
#line 3521 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10865 "bison-chpl-lib.cpp"
    break;

  case 643: /* expr: sub_type_level_expr TQUESTION  */
#line 3530 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10871 "bison-chpl-lib.cpp"
    break;

  case 644: /* expr: TQUESTION  */
#line 3532 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10877 "bison-chpl-lib.cpp"
    break;

  case 648: /* expr: fn_type  */
#line 3537 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10883 "bison-chpl-lib.cpp"
    break;

  case 658: /* opt_expr: %empty  */
#line 3551 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10889 "bison-chpl-lib.cpp"
    break;

  case 659: /* opt_expr: expr  */
#line 3552 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10895 "bison-chpl-lib.cpp"
    break;

  case 660: /* opt_try_expr: TTRY expr  */
#line 3556 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10901 "bison-chpl-lib.cpp"
    break;

  case 661: /* opt_try_expr: TTRYBANG expr  */
#line 3557 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10907 "bison-chpl-lib.cpp"
    break;

  case 662: /* opt_try_expr: super_expr  */
#line 3558 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10913 "bison-chpl-lib.cpp"
    break;

  case 668: /* call_base_expr: expr TBANG  */
#line 3575 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10921 "bison-chpl-lib.cpp"
    break;

  case 669: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3578 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10927 "bison-chpl-lib.cpp"
    break;

  case 672: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3585 "chpl.ypp"
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
#line 10943 "bison-chpl-lib.cpp"
    break;

  case 673: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3597 "chpl.ypp"
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
#line 10959 "bison-chpl-lib.cpp"
    break;

  case 674: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3609 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10967 "bison-chpl-lib.cpp"
    break;

  case 675: /* dot_expr: expr TDOT ident_use  */
#line 3616 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10973 "bison-chpl-lib.cpp"
    break;

  case 676: /* dot_expr: expr TDOT TTYPE  */
#line 3618 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10979 "bison-chpl-lib.cpp"
    break;

  case 677: /* dot_expr: expr TDOT TDOMAIN  */
#line 3620 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10985 "bison-chpl-lib.cpp"
    break;

  case 678: /* dot_expr: expr TDOT TLOCALE  */
#line 3622 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10991 "bison-chpl-lib.cpp"
    break;

  case 679: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3624 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 11001 "bison-chpl-lib.cpp"
    break;

  case 680: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3630 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 11011 "bison-chpl-lib.cpp"
    break;

  case 681: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3642 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 11017 "bison-chpl-lib.cpp"
    break;

  case 682: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3644 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 11025 "bison-chpl-lib.cpp"
    break;

  case 683: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3648 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11033 "bison-chpl-lib.cpp"
    break;

  case 684: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3652 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11041 "bison-chpl-lib.cpp"
    break;

  case 685: /* bool_literal: TFALSE  */
#line 3658 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 11047 "bison-chpl-lib.cpp"
    break;

  case 686: /* bool_literal: TTRUE  */
#line 3659 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 11053 "bison-chpl-lib.cpp"
    break;

  case 687: /* str_bytes_literal: STRINGLITERAL  */
#line 3663 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11059 "bison-chpl-lib.cpp"
    break;

  case 688: /* str_bytes_literal: BYTESLITERAL  */
#line 3664 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11065 "bison-chpl-lib.cpp"
    break;

  case 691: /* literal_expr: INTLITERAL  */
#line 3670 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 11071 "bison-chpl-lib.cpp"
    break;

  case 692: /* literal_expr: REALLITERAL  */
#line 3671 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 11077 "bison-chpl-lib.cpp"
    break;

  case 693: /* literal_expr: IMAGLITERAL  */
#line 3672 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 11083 "bison-chpl-lib.cpp"
    break;

  case 694: /* literal_expr: CSTRINGLITERAL  */
#line 3673 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 11089 "bison-chpl-lib.cpp"
    break;

  case 695: /* literal_expr: TNONE  */
#line 3674 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 11095 "bison-chpl-lib.cpp"
    break;

  case 696: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3676 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11104 "bison-chpl-lib.cpp"
    break;

  case 697: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3681 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11113 "bison-chpl-lib.cpp"
    break;

  case 698: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3686 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 11122 "bison-chpl-lib.cpp"
    break;

  case 699: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3691 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 11131 "bison-chpl-lib.cpp"
    break;

  case 700: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3696 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 11140 "bison-chpl-lib.cpp"
    break;

  case 701: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3701 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 11149 "bison-chpl-lib.cpp"
    break;

  case 702: /* assoc_expr_ls: expr TALIAS expr  */
#line 3710 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 11158 "bison-chpl-lib.cpp"
    break;

  case 703: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3715 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 11168 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TPLUS expr  */
#line 3723 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11174 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TMINUS expr  */
#line 3724 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11180 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TSTAR expr  */
#line 3725 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11186 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TDIVIDE expr  */
#line 3726 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11192 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3727 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11198 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3728 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11204 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TMOD expr  */
#line 3729 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11210 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TEQUAL expr  */
#line 3730 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11216 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3731 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11222 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3732 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11228 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3733 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11234 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TLESS expr  */
#line 3734 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11240 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TGREATER expr  */
#line 3735 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11246 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TBAND expr  */
#line 3736 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11252 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr TBOR expr  */
#line 3737 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11258 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr TBXOR expr  */
#line 3738 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11264 "bison-chpl-lib.cpp"
    break;

  case 720: /* binary_op_expr: expr TAND expr  */
#line 3739 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11270 "bison-chpl-lib.cpp"
    break;

  case 721: /* binary_op_expr: expr TOR expr  */
#line 3740 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11276 "bison-chpl-lib.cpp"
    break;

  case 722: /* binary_op_expr: expr TEXP expr  */
#line 3741 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11282 "bison-chpl-lib.cpp"
    break;

  case 723: /* binary_op_expr: expr TBY expr  */
#line 3742 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11288 "bison-chpl-lib.cpp"
    break;

  case 724: /* binary_op_expr: expr TALIGN expr  */
#line 3743 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11294 "bison-chpl-lib.cpp"
    break;

  case 725: /* binary_op_expr: expr THASH expr  */
#line 3744 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11300 "bison-chpl-lib.cpp"
    break;

  case 726: /* binary_op_expr: expr TDMAPPED expr  */
#line 3745 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11306 "bison-chpl-lib.cpp"
    break;

  case 727: /* unary_op_expr: TPLUS expr  */
#line 3749 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11312 "bison-chpl-lib.cpp"
    break;

  case 728: /* unary_op_expr: TMINUS expr  */
#line 3750 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11318 "bison-chpl-lib.cpp"
    break;

  case 729: /* unary_op_expr: TMINUSMINUS expr  */
#line 3751 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11324 "bison-chpl-lib.cpp"
    break;

  case 730: /* unary_op_expr: TPLUSPLUS expr  */
#line 3752 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11330 "bison-chpl-lib.cpp"
    break;

  case 731: /* unary_op_expr: TBANG expr  */
#line 3753 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11336 "bison-chpl-lib.cpp"
    break;

  case 732: /* unary_op_expr: expr TBANG  */
#line 3754 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 11344 "bison-chpl-lib.cpp"
    break;

  case 733: /* unary_op_expr: TBNOT expr  */
#line 3757 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11350 "bison-chpl-lib.cpp"
    break;

  case 734: /* reduce_expr: expr TREDUCE expr  */
#line 3762 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11358 "bison-chpl-lib.cpp"
    break;

  case 735: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3766 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11366 "bison-chpl-lib.cpp"
    break;

  case 736: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3770 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11374 "bison-chpl-lib.cpp"
    break;

  case 737: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3774 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11382 "bison-chpl-lib.cpp"
    break;

  case 738: /* scan_expr: expr TSCAN expr  */
#line 3781 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11390 "bison-chpl-lib.cpp"
    break;

  case 739: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3785 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11398 "bison-chpl-lib.cpp"
    break;

  case 740: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3789 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11406 "bison-chpl-lib.cpp"
    break;

  case 741: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3793 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11414 "bison-chpl-lib.cpp"
    break;


#line 11418 "bison-chpl-lib.cpp"

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
