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
  YYSYMBOL_loop_stmt = 239,                /* loop_stmt  */
  YYSYMBOL_zippered_iterator = 240,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 241,                  /* if_stmt  */
  YYSYMBOL_ifvar = 242,                    /* ifvar  */
  YYSYMBOL_interface_stmt = 243,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 244,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 245,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 246,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 247, /* implements_type_error_ident  */
  YYSYMBOL_implements_stmt = 248,          /* implements_stmt  */
  YYSYMBOL_ifc_constraint = 249,           /* ifc_constraint  */
  YYSYMBOL_try_stmt = 250,                 /* try_stmt  */
  YYSYMBOL_catch_expr_ls = 251,            /* catch_expr_ls  */
  YYSYMBOL_catch_expr = 252,               /* catch_expr  */
  YYSYMBOL_catch_expr_inner = 253,         /* catch_expr_inner  */
  YYSYMBOL_throw_stmt = 254,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 255,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 256,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 257,                /* when_stmt  */
  YYSYMBOL_manager_expr = 258,             /* manager_expr  */
  YYSYMBOL_manager_expr_ls = 259,          /* manager_expr_ls  */
  YYSYMBOL_manage_stmt = 260,              /* manage_stmt  */
  YYSYMBOL_class_decl_stmt = 261,          /* class_decl_stmt  */
  YYSYMBOL_class_start = 262,              /* class_start  */
  YYSYMBOL_class_tag = 263,                /* class_tag  */
  YYSYMBOL_opt_inherit = 264,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 265,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 266,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header_lcbr = 267,         /* enum_header_lcbr  */
  YYSYMBOL_enum_ls = 268,                  /* enum_ls  */
  YYSYMBOL_269_5 = 269,                    /* $@5  */
  YYSYMBOL_270_6 = 270,                    /* $@6  */
  YYSYMBOL_enum_item = 271,                /* enum_item  */
  YYSYMBOL_lambda_decl_start = 272,        /* lambda_decl_start  */
  YYSYMBOL_lambda_decl_expr = 273,         /* lambda_decl_expr  */
  YYSYMBOL_274_7 = 274,                    /* $@7  */
  YYSYMBOL_linkage_spec_empty = 275,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 276,             /* linkage_spec  */
  YYSYMBOL_opt_fn_type_formal_ls = 277,    /* opt_fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal_ls = 278,        /* fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal = 279,           /* fn_type_formal  */
  YYSYMBOL_opt_fn_type_ret_type = 280,     /* opt_fn_type_ret_type  */
  YYSYMBOL_fn_type = 281,                  /* fn_type  */
  YYSYMBOL_fn_expr = 282,                  /* fn_expr  */
  YYSYMBOL_283_8 = 283,                    /* $@8  */
  YYSYMBOL_284_9 = 284,                    /* $@9  */
  YYSYMBOL_fn_decl_stmt_complete = 285,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 286,             /* fn_decl_stmt  */
  YYSYMBOL_287_10 = 287,                   /* $@10  */
  YYSYMBOL_fn_decl_stmt_inner = 288,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 289,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 290,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 291,                 /* fn_ident  */
  YYSYMBOL_op_ident = 292,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 293,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 294,              /* all_op_name  */
  YYSYMBOL_formal_var_arg_expr = 295,      /* formal_var_arg_expr  */
  YYSYMBOL_opt_formal_ls = 296,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 297,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 298,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 299,                /* formal_ls  */
  YYSYMBOL_formal = 300,                   /* formal  */
  YYSYMBOL_formal_ident_def = 301,         /* formal_ident_def  */
  YYSYMBOL_named_formal = 302,             /* named_formal  */
  YYSYMBOL_opt_formal_intent_tag = 303,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 304,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 305,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 306,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 307,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 308,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 309,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 310,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 311,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 312,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 313, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 314,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 315,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 316,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 317, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 318, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 319, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 320,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 321,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 322,            /* var_decl_stmt  */
  YYSYMBOL_323_11 = 323,                   /* $@11  */
  YYSYMBOL_var_decl_stmt_inner_ls = 324,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 325,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 326, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 327, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 328,            /* opt_init_expr  */
  YYSYMBOL_formal_or_ret_type_expr = 329,  /* formal_or_ret_type_expr  */
  YYSYMBOL_ret_type = 330,                 /* ret_type  */
  YYSYMBOL_colon_ret_type = 331,           /* colon_ret_type  */
  YYSYMBOL_opt_ret_type = 332,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 333,                 /* opt_type  */
  YYSYMBOL_formal_type = 334,              /* formal_type  */
  YYSYMBOL_colon_formal_type = 335,        /* colon_formal_type  */
  YYSYMBOL_opt_colon_formal_type = 336,    /* opt_colon_formal_type  */
  YYSYMBOL_expr_ls = 337,                  /* expr_ls  */
  YYSYMBOL_tuple_component = 338,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 339,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 340,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 341,                /* actual_ls  */
  YYSYMBOL_actual_expr = 342,              /* actual_expr  */
  YYSYMBOL_ident_expr = 343,               /* ident_expr  */
  YYSYMBOL_sub_type_level_expr = 344,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 345,                 /* for_expr  */
  YYSYMBOL_bracket_loop_expr = 346,        /* bracket_loop_expr  */
  YYSYMBOL_cond_expr = 347,                /* cond_expr  */
  YYSYMBOL_nil_expr = 348,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 349,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 350,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 351,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 352,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 353,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 354,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 355,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 356,          /* task_var_prefix  */
  YYSYMBOL_new_maybe_decorated = 357,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 358,                 /* new_expr  */
  YYSYMBOL_let_expr = 359,                 /* let_expr  */
  YYSYMBOL_range_literal_expr = 360,       /* range_literal_expr  */
  YYSYMBOL_cast_expr = 361,                /* cast_expr  */
  YYSYMBOL_tuple_expand_expr = 362,        /* tuple_expand_expr  */
  YYSYMBOL_super_expr = 363,               /* super_expr  */
  YYSYMBOL_expr = 364,                     /* expr  */
  YYSYMBOL_opt_expr = 365,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 366,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 367,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 368,           /* call_base_expr  */
  YYSYMBOL_call_expr = 369,                /* call_expr  */
  YYSYMBOL_dot_expr = 370,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 371,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 372,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 373,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 374,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 375,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 376,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 377,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 378,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 379,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 380       /* reduce_scan_op_expr  */
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

#line 504 "bison-chpl-lib.cpp"

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
#define YYLAST   19216

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  200
/* YYNRULES -- Number of rules.  */
#define YYNRULES  742
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1313

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
     798,   809,   821,   833,   844,   860,   861,   865,   875,   876,
     877,   881,   885,   886,   890,   894,   901,   913,   921,   924,
     934,   936,   943,   948,   957,   961,   981,   996,  1011,  1027,
    1028,  1030,  1035,  1036,  1041,  1059,  1063,  1067,  1078,  1097,
    1098,  1102,  1106,  1110,  1115,  1119,  1123,  1133,  1138,  1144,
    1151,  1156,  1163,  1174,  1175,  1179,  1184,  1192,  1201,  1211,
    1219,  1228,  1241,  1249,  1250,  1251,  1252,  1253,  1254,  1255,
    1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,  1264,  1265,
    1277,  1281,  1286,  1291,  1297,  1306,  1307,  1311,  1320,  1324,
    1328,  1332,  1336,  1340,  1349,  1350,  1354,  1355,  1356,  1357,
    1358,  1362,  1363,  1375,  1376,  1381,  1382,  1383,  1384,  1385,
    1386,  1387,  1388,  1389,  1390,  1391,  1392,  1393,  1394,  1395,
    1396,  1397,  1398,  1399,  1403,  1404,  1405,  1406,  1407,  1408,
    1409,  1410,  1411,  1412,  1413,  1414,  1421,  1422,  1423,  1424,
    1428,  1429,  1433,  1440,  1450,  1454,  1460,  1460,  1466,  1466,
    1475,  1476,  1477,  1478,  1479,  1480,  1481,  1485,  1490,  1495,
    1500,  1507,  1515,  1516,  1520,  1529,  1535,  1543,  1562,  1561,
    1571,  1570,  1583,  1590,  1609,  1623,  1637,  1641,  1645,  1649,
    1653,  1657,  1661,  1665,  1685,  1689,  1693,  1697,  1701,  1705,
    1709,  1713,  1717,  1721,  1725,  1729,  1733,  1737,  1741,  1745,
    1749,  1753,  1757,  1761,  1765,  1769,  1773,  1778,  1786,  1795,
    1799,  1803,  1807,  1811,  1815,  1819,  1823,  1827,  1832,  1837,
    1842,  1850,  1865,  1883,  1887,  1894,  1895,  1900,  1905,  1906,
    1907,  1908,  1909,  1910,  1911,  1912,  1913,  1914,  1915,  1916,
    1917,  1926,  1927,  1928,  1929,  1938,  1939,  1943,  1947,  1951,
    1958,  1962,  1966,  1973,  1977,  1981,  1985,  1992,  1993,  1997,
    2001,  2005,  2012,  2025,  2041,  2049,  2053,  2062,  2063,  2067,
    2071,  2076,  2084,  2089,  2093,  2100,  2101,  2105,  2114,  2119,
    2130,  2137,  2138,  2139,  2143,  2144,  2148,  2152,  2156,  2160,
    2164,  2171,  2188,  2201,  2208,  2213,  2220,  2219,  2229,  2235,
    2234,  2249,  2256,  2268,  2286,  2283,  2311,  2315,  2316,  2318,
    2323,  2324,  2328,  2329,  2333,  2336,  2338,  2343,  2344,  2355,
    2375,  2374,  2390,  2389,  2407,  2417,  2414,  2448,  2456,  2464,
    2475,  2486,  2495,  2510,  2511,  2515,  2516,  2517,  2526,  2527,
    2528,  2529,  2530,  2531,  2532,  2533,  2534,  2535,  2536,  2537,
    2538,  2539,  2540,  2541,  2542,  2543,  2544,  2545,  2546,  2547,
    2548,  2549,  2550,  2554,  2555,  2556,  2557,  2558,  2559,  2560,
    2561,  2562,  2563,  2564,  2565,  2570,  2571,  2575,  2576,  2580,
    2581,  2585,  2586,  2590,  2591,  2595,  2596,  2600,  2604,  2605,
    2609,  2613,  2618,  2623,  2628,  2633,  2641,  2645,  2653,  2654,
    2655,  2656,  2657,  2658,  2659,  2660,  2661,  2665,  2666,  2667,
    2668,  2669,  2670,  2674,  2675,  2676,  2680,  2681,  2682,  2683,
    2684,  2685,  2686,  2690,  2691,  2694,  2695,  2699,  2700,  2701,
    2705,  2710,  2711,  2713,  2715,  2717,  2722,  2724,  2729,  2731,
    2733,  2735,  2737,  2739,  2741,  2746,  2747,  2751,  2760,  2764,
    2772,  2776,  2783,  2804,  2805,  2810,  2811,  2812,  2813,  2814,
    2819,  2818,  2827,  2835,  2839,  2846,  2864,  2883,  2887,  2891,
    2898,  2900,  2902,  2909,  2910,  2911,  2915,  2919,  2920,  2921,
    2925,  2926,  2930,  2931,  2935,  2936,  2937,  2938,  2942,  2943,
    2947,  2951,  2952,  2958,  2959,  2963,  2964,  2968,  2972,  2979,
    2980,  2984,  2989,  2998,  2999,  3003,  3004,  3011,  3012,  3013,
    3014,  3015,  3016,  3018,  3020,  3022,  3024,  3030,  3032,  3035,
    3037,  3039,  3041,  3043,  3045,  3047,  3049,  3051,  3053,  3058,
    3069,  3080,  3090,  3106,  3122,  3138,  3147,  3156,  3164,  3178,
    3192,  3209,  3213,  3217,  3221,  3225,  3229,  3233,  3240,  3258,
    3266,  3267,  3268,  3269,  3270,  3271,  3272,  3276,  3277,  3281,
    3290,  3291,  3295,  3304,  3305,  3309,  3323,  3327,  3334,  3335,
    3336,  3337,  3338,  3339,  3343,  3345,  3347,  3349,  3351,  3357,
    3364,  3376,  3388,  3401,  3418,  3425,  3430,  3435,  3440,  3446,
    3452,  3482,  3489,  3496,  3497,  3501,  3503,  3504,  3506,  3508,
    3509,  3510,  3511,  3514,  3515,  3516,  3517,  3518,  3519,  3520,
    3521,  3522,  3526,  3527,  3531,  3532,  3533,  3537,  3538,  3539,
    3540,  3549,  3550,  3553,  3554,  3555,  3559,  3571,  3583,  3590,
    3592,  3594,  3596,  3598,  3604,  3617,  3618,  3622,  3626,  3633,
    3634,  3638,  3639,  3643,  3644,  3645,  3646,  3647,  3648,  3649,
    3650,  3655,  3660,  3664,  3668,  3672,  3680,  3685,  3694,  3695,
    3696,  3697,  3698,  3699,  3700,  3701,  3702,  3703,  3704,  3705,
    3706,  3707,  3708,  3709,  3710,  3711,  3712,  3713,  3714,  3715,
    3716,  3720,  3721,  3722,  3723,  3724,  3725,  3728,  3732,  3736,
    3740,  3744,  3751,  3755,  3759,  3763,  3771,  3772,  3773,  3774,
    3775,  3776,  3777
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

#define YYPACT_NINF (-1131)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-743)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1131,    77,  4131, -1131,   -37,   120, -1131, -1131, -1131, -1131,
   -1131, -1131, -1131, 11483,   148,   189,   147, 13884,   244, 18970,
     148, 11483,   310,   263,   287,   189,  5533, 11483,  5533,   215,
   19025, -1131,   388,   402,  9033,  9908,  9908, -1131,  9208,   422,
     464,   306, -1131,   434, 19025, 19025, 19025, -1131,  3709, 10083,
     444, 11483, 11483,   212, -1131,   465,   481, 11483, -1131, 13884,
   -1131, 11483,   521,   389,   229,   286, 13232,   514, 19100, -1131,
   11483,  8508, 11483, 10083, 13884, 11483,   470,   524,   404,  5533,
     527, 11483,   535,  7458,  7458, -1131,   536, -1131, 13884, -1131,
     541,  9208,  9383, -1131,   596, 11483, -1131, 11483, -1131, -1131,
   13712, 11483, -1131, 11483, -1131, -1131, -1131,  4483,  7808,  9558,
   11483, -1131,  5183, -1131, -1131, -1131, -1131,  1998,   469, -1131,
      62,   435, -1131,    43, -1131, -1131, -1131, -1131, -1131, -1131,
   -1131, -1131, -1131, -1131, -1131, -1131, -1131,  7633, -1131, 10258,
   -1131, -1131, -1131, -1131, -1131,   549, -1131, -1131, -1131, -1131,
   -1131, -1131, -1131, -1131,   466, 19025, -1131,  3125,   452,   242,
   -1131,   180, -1131, -1131, -1131,   300,   233, -1131, -1131, 19025,
    3709, -1131,   451,   472, -1131, -1131, -1131,   487,   488, 11483,
     493,   496, -1131, -1131, -1131, 18332,  1349,   246,   497,   498,
   -1131, -1131,   415, -1131, -1131, -1131, -1131, -1131,   173, -1131,
   -1131, -1131, 11483, 11483, 11483, 19025, -1131, 11483, 11483, 10433,
     574,   447, -1131, -1131, -1131, -1131, 18332,   453, -1131, -1131,
     485,  5533, -1131, -1131,   500,   152,   501, 15752, -1131,    44,
   -1131,   503, -1131,    16, 18332,   547,  9383, -1131, -1131, -1131,
   -1131, -1131, -1131, -1131, -1131, -1131, -1131, -1131, -1131, -1131,
   -1131, -1131, -1131, -1131, -1131, -1131, -1131, -1131, -1131, -1131,
   -1131, -1131,   502, -1131, -1131, 19025,   505,    72, 15264,    15,
    2623,    15, 15112, 19025, 19025,   130, 14372, -1131, -1131,   589,
    9383,   506,   462,  5533,  1906,    63,    18, -1131,  5533, -1131,
   -1131, 15828, -1131,    70, 16416,   257, -1131,   507,   508, -1131,
   15828,   152,   257, -1131,  9383,   958,   958,    65,    -5, -1131,
       0, -1131, -1131, -1131, -1131, 11483, 11483, -1131,   533, -1131,
     509,   528, -1131, -1131,  3313,   539, -1131, -1131, 18332,   254,
   11483, 11483, -1131,   -20, -1131, -1131, 18332,   522, 16461, -1131,
   15828,   152, 18332,   519,  9383, -1131, 18332, 16568, -1131, -1131,
   -1131, -1131,   152,    72, 15828,   529, -1131,   240,   240,   650,
     257,   257,   250, -1131, -1131,  4658,   112, -1131, 11483,   -11,
     114, -1131, 14027,   -14,    -2, 16614,   -24,   650,   678, -1131,
   -1131, 19025, -1131, -1131,  4483, -1131,    67, -1131,   589, -1131,
   -1131, -1131,   690,   523,  4833, 11483, -1131, 11483,   631, -1131,
   -1131, 15356,   158,   466, 18332,   591, 19025, 11483,   534, -1131,
     530, 18753,   573,   127, -1131,    74,   300, -1131, -1131, -1131,
   -1131,   609, -1131, -1131, -1131, -1131,   139,   618, -1131, -1131,
   -1131, 13542,   588,   319, -1131,   323,   463, -1131,   569, 11483,
   11483, 11483,  9908,  9908, 11483,   473, 11483, 11483, 11483, 11483,
   11483,   253, 13712, 11483, 11483, 11483, 11483, 11483, 11483, 11483,
   11483, 11483, 11483, 11483, 11483, 11483, 11483, 11483, 11483,   651,
   -1131, -1131, -1131, -1131, -1131,  9383,  9383, -1131, -1131, -1131,
   -1131,  9383, -1131, -1131,  9383,  9383,  9383,  9383,  9908,  9908,
   15904, 15980, 16655,   551,     9, 19025,  7983, -1131, -1131, -1131,
    9908,    72,  3709, -1131, 11483, -1131, 11483,   600,   557,   584,
   -1131, -1131, -1131,   685, 11483, -1131,  5708,  9908, -1131,   562,
   -1131,    72,  5883,  9908, -1131,    72, -1131,    72,  9908, -1131,
      72,   608,   610,  5533,   702,  5533, -1131,   707, 11483,   677,
     580,  9383, 19025, -1131, -1131, -1131,  1906, -1131, -1131,   181,
   -1131, 11658,   629, 11483,  3709, -1131, -1131, 11483, -1131, 18840,
   11483, 11483, -1131,   582, -1131, -1131, -1131, -1131, 18895, -1131,
     300, 13406, -1131,  1906, -1131,   613, 11658, -1131, 18332, 18332,
   -1131,   501, -1131,    41, -1131,  9383,   587, -1131,   741,   741,
   -1131, -1131, -1131, -1131, -1131, -1131, 10608, -1131, 16771,  8158,
   -1131,  8333, -1131, 18332,  5533,   590,  9908, 10783,  4308,   592,
   11483, 10958, -1131, -1131,  9033,  9908,  9908, -1131,   614,  8683,
   -1131,   296, -1131,  5008, -1131,   335, 16812,   347, 15448, 19025,
    7283,  7283, -1131,   466,   594, -1131,    44, -1131,   172,   619,
    1351, -1131, -1131, 11483,   165, -1131, -1131,   193, -1131, -1131,
     146, -1131, -1131,  3927, -1131,   667, -1131,   602,   626, -1131,
     627, -1131,   632,   633,   634,   637, -1131,   641,   644,   648,
     652,   655,   656,   657, -1131, -1131, -1131, -1131, -1131, -1131,
   -1131, -1131, -1131, -1131, -1131, -1131, -1131, -1131, -1131, -1131,
   -1131, -1131, -1131, -1131, -1131, -1131, 11483,   659,   662,   602,
   -1131,   602, -1131, 11483, -1131, 19025, -1131, -1131,  3725,  3725,
     425, -1131,   425, -1131,   425, 13103,   754,  1089,   691,   152,
     240, -1131,   474, -1131, -1131, -1131, -1131, -1131,   650, 13277,
     425,   561,   561,  3725,   561,   561,   738,   240, 13277,  1980,
     738,   257,   257,   240,   650,   643,   647,   658,   663,   664,
     665,   642,   603, -1131,   425, -1131,   425, 11833,  9908, 12008,
    9908, 11483,  9383,  9908, 14027,   676,    92, -1131, -1131, -1131,
     194, -1131,  1074, 18377,   271,   148, 16857, -1131,   366, 18332,
   16964,  9383, -1131,  9383, 11483,   201,  9208, 18332,    79, 16056,
    7983, -1131,  9208, 18332,    48, 15524, -1131, -1131,    15, 15600,
   -1131, 11483, 11483,   786,  5533,   788, 17009,  5533, 16132, 19025,
   -1131,   210, -1131,   222, -1131,   224,  1906,    63, -1131, 18332,
    8858, -1131, 18332, -1131, -1131, -1131, 19025, 17049, 17125, -1131,
     613,   688, -1131,   238, 11658, -1131,   251, -1131, 11483, -1131,
     653,   155,   679, -1131,  3590, -1131, -1131, -1131, -1131, -1131,
   -1131, -1131, -1131,  5533,   149, 16208, -1131, -1131, 18332,  5533,
   18332, -1131, 17201, 15676, 15188,   101, -1131, -1131,   249, -1131,
   -1131, -1131, -1131, 11483, -1131,    66, 14132, 11483, -1131, 11133,
    7283,  7283, -1131, 11483,   315, 11483,   437,   450, 11483,  9733,
     492,   367, -1131, -1131, -1131, 17241,   703,   670,   683, -1131,
   19025,  3709, -1131,  9383,   692, 18520, 18332, 19025,   469,   838,
   -1131,   667, -1131, -1131, -1131, -1131, -1131,    26,   283, -1131,
   17287, -1131, 14291, -1131, -1131, 18332, -1131,   681,   674, -1131,
   -1131, -1131, -1131, -1131, -1131, -1131, -1131, 11483,   824, 17393,
   11483,   829, 17439,   265,   687, 17479,  9383, -1131, -1131,  7983,
   -1131, -1131, -1131, -1131, -1131, -1131, -1131, -1131, -1131, -1131,
   -1131, -1131,    51,  9908,  9908,    72,    72, -1131, -1131, -1131,
   -1131, 15828, -1131, 14452,  6058, -1131,  6233, -1131,   285, -1131,
   14532,  6408, -1131,    72,  6583, -1131,    72, -1131,    72, -1131,
      72, 18332, 18332,  5533, -1131,  5533, 11483, -1131,  5533,   835,
     706,   712, 19025,   501, -1131, -1131,   629, -1131, -1131, -1131,
     709,   720,   251, 11483,   667,   613, -1131, 11483, -1131, -1131,
   18332, -1131,  6758, 11483, -1131, -1131, -1131, 19025, -1131,   732,
     501, -1131,  6933,   719,  7108,   722, -1131, 11483,  9908,  9908,
    9908,  5533,  9383, -1131, -1131, -1131, -1131, -1131,   474, -1131,
   -1131, -1131, -1131, -1131, -1131, -1131, -1131, -1131, -1131, -1131,
   -1131, -1131, -1131,  7283, -1131, -1131, 17599,   186, 16300,   369,
     734,  7283, -1131, 11483, -1131, -1131, 18538,   372,   376,   312,
   -1131, -1131, -1131, -1131, 18632, 18705, -1131, -1131, 18753, -1131,
      26,   134, 11483, -1131,   761,   728, -1131,   602,   602, -1131,
   -1131, 17559, 12183, 12358, 17675, 12533, 12708, -1131, 12883, 13058,
     317, -1131,   629,   322,   334, -1131, -1131, -1131,  5533,  9208,
   18332,  9208, 18332,  7983, -1131,  5533,  9208, 18332, -1131,  9208,
   18332, -1131, -1131, -1131, -1131, -1131, 18332,   875,  5533, -1131,
   -1131, -1131, -1131, -1131,  9383,  9383, -1131, -1131, -1131, -1131,
     251, 18332,  5533, -1131,    73,   737, 11483, -1131,  9208, -1131,
   18332,  5533,  9208, -1131, 18332,  5533, 18332,    72, 15828,    15,
   15600,   164, 16376, -1131,   150, 11308,  7283,  7283,  7283,  7283,
   -1131, -1131, -1131, 17760, 18332, 18614, -1131, -1131, -1131,   746,
   18705, -1131, -1131, -1131, -1131, -1131,    97, -1131,    13, -1131,
     410, 17836,    85, -1131, -1131, -1131, 11483, 11483, 11483, 11483,
   11483, 11483, 11483, 11483, -1131, -1131, 17009, 14612, 14692, -1131,
   17009, 14772, 14852,  5533, -1131,   739,   744, -1131, -1131,   547,
   -1131,   501, 18332, 14932, -1131, 15012, -1131,  5533,  5533, -1131,
   -1131, 18332,   377,   758,   380,   765, 11483, -1131, -1131, -1131,
      82, -1131, 11483,   134,    97,    97,    97,    97,    97,    97,
     134,  5358, -1131, -1131, -1131, -1131, -1131, 17912, 17988, 18064,
   18140, 18216, 18292,  5533,  5533,  5533,  5533, -1131,   755,   762,
   -1131,  5533,  5533, -1131, -1131, -1131, -1131, 18332, -1131, 18332,
   -1131, -1131, -1131, -1131, -1131, -1131, -1131,   784, -1131, 11483,
   11483, 11483, 11483, 11483, 11483, 17009, 17009, 17009, 17009, -1131,
   -1131, 17009, 17009
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   143,   470,   685,   686,   687,
     681,   682,   688,     0,   597,   134,   164,   565,   171,   567,
     597,     0,   170,   500,   498,   134,     0,     0,     0,   285,
     165,   203,   202,   679,     0,     0,     0,   201,     0,   169,
      69,   286,   348,   166,     0,     0,     0,   343,     0,     0,
     173,     0,     0,   614,   589,   689,   174,     0,   349,   559,
     495,     0,     0,     0,   188,   186,   436,   168,   568,   497,
       0,     0,     0,     0,   563,     0,     0,   172,     0,     0,
     144,     0,   680,     0,     0,   488,   167,   323,   561,   499,
     175,     0,     0,   738,     0,     0,   740,     0,   741,   742,
     630,     0,   739,   736,   638,   184,   737,     0,     0,     0,
       0,     4,     0,     5,    10,    11,    12,    47,    51,    52,
      58,     0,    45,    72,    13,    78,    14,    15,    16,    17,
      29,   545,   546,    23,    46,   185,   195,     0,   204,   652,
     196,    18,    31,    30,    20,     0,   280,    19,   648,    22,
      35,    32,    33,   194,   324,     0,   192,     0,     0,   645,
     347,     0,   642,   190,   364,   456,   447,   640,   193,     0,
       0,   191,   657,   636,   635,   639,   549,   547,     0,     0,
     646,   647,   651,   650,   649,     0,   548,     0,   658,   659,
     660,   683,   684,   641,   551,   550,   643,   644,     0,    28,
     567,   165,     0,     0,     0,     0,   568,     0,     0,     0,
       0,   645,   657,   547,   646,   647,   557,   548,   658,   659,
       0,     0,   598,   135,     0,   566,     0,   597,   489,     0,
     496,     0,    21,     0,   533,     0,   539,   141,   145,   157,
     151,   150,   159,   138,   149,   160,   146,   161,   136,   162,
     155,   148,   156,   154,   152,   153,   137,   139,   147,   158,
     163,   142,     0,   140,   212,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    71,    70,    72,
     539,     0,     0,     0,     0,     0,     0,   503,     0,   181,
      40,     0,   315,     0,   314,   723,   618,   615,   616,   617,
       0,   560,   724,     7,   539,   346,   346,   443,   178,   438,
     179,   439,   440,   444,   445,   177,   176,   446,   436,   529,
       0,   351,   352,   354,     0,   437,   528,   356,   516,     0,
       0,     0,   182,   642,   633,   656,   634,     0,     0,    43,
       0,   564,   552,     0,   539,    44,   558,     0,   293,   297,
     294,   297,   562,     0,     0,     0,    57,   725,   727,   628,
     722,   721,     0,    75,    79,     0,     0,   535,     0,     0,
       0,   536,   581,     0,     0,   533,     0,   629,     0,     6,
     321,     0,   202,   322,     0,    49,     0,     9,    72,    50,
      53,    56,     0,    55,     0,     0,    73,     0,     0,   500,
     200,     0,   681,   324,   653,   208,     0,     0,     0,   320,
       0,     0,   341,     0,   334,   436,   456,   454,   455,   453,
     372,   458,   457,   461,   460,   462,     0,   451,   448,   449,
     452,     0,   493,     0,   490,     0,   637,    34,   619,     0,
       0,     0,     0,     0,     0,   726,     0,     0,     0,     0,
       0,     0,   627,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   403,
     410,   411,   412,   407,   409,     0,     0,   405,   408,   406,
     404,     0,   414,   413,     0,     0,   539,   539,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    36,    24,    37,
       0,     0,     0,    25,     0,    38,     0,   545,     0,   540,
     541,   544,   333,     0,     0,   222,     0,     0,   221,     0,
     230,     0,     0,     0,   228,     0,   238,     0,     0,   236,
       0,     0,     0,     0,   254,     0,   403,   250,     0,     0,
       0,     0,     0,   264,    26,   507,     0,   508,   510,     0,
     527,     0,   513,     0,     0,   180,    39,     0,   317,     0,
       0,     0,    41,     0,   189,   187,   441,   442,     0,   437,
     456,   436,   429,     0,   428,   531,     0,   127,   654,   655,
     362,     0,   183,     0,    42,     0,     0,   304,   295,   296,
     215,   214,    27,    77,    76,    80,     0,   690,     0,     0,
     675,     0,   677,   582,     0,     0,     0,     0,     0,     0,
       0,     0,   694,     8,     0,     0,     0,    48,    58,     0,
      54,     0,    66,     0,   125,     0,   120,     0,    87,     0,
       0,    93,   197,   324,     0,   210,     0,   207,   291,   325,
       0,   332,   338,     0,   336,   331,   421,     0,   423,   427,
       0,   459,   521,     0,   523,   463,   450,   419,   141,   399,
     145,   397,   151,   150,   149,   146,   401,   161,   162,   148,
     152,   137,   147,   163,   396,   378,   381,   379,   380,   402,
     391,   382,   395,   387,   385,   398,   386,   384,   389,   394,
     383,   388,   392,   393,   390,   400,     0,   375,     0,   419,
     376,   419,   373,     0,   492,     0,   487,   502,   718,   717,
     720,   729,   728,   733,   732,   714,   711,   712,   713,   631,
     701,   143,     0,   671,   672,   144,   670,   669,   625,   705,
     716,   710,   708,   719,   709,   707,   699,   704,   706,   715,
     698,   702,   703,   700,   626,     0,     0,     0,     0,     0,
       0,     0,     0,   731,   730,   735,   734,     0,     0,     0,
       0,     0,     0,     0,   692,   291,   608,   609,   611,   613,
       0,   600,     0,     0,     0,   597,   597,   218,     0,   534,
       0,     0,   554,     0,     0,     0,     0,   571,     0,     0,
       0,   231,     0,   577,     0,     0,   229,   239,     0,     0,
     237,     0,     0,   253,     0,   249,     0,     0,     0,     0,
     553,     0,   267,     0,   265,     0,   511,     0,   526,   525,
       0,   505,   624,   504,   316,   313,     0,     0,     0,   668,
     531,   357,   353,     0,     0,   532,   513,   355,     0,   361,
       0,     0,     0,   555,     0,   298,   691,   632,   676,   537,
     678,   538,   246,     0,     0,     0,   693,   244,   583,     0,
     696,   695,     0,     0,     0,     0,    59,    60,     0,    62,
      64,    68,    67,     0,   102,     0,     0,     0,    95,     0,
       0,    93,    65,   396,   378,   381,   379,   380,   389,   388,
     390,     0,   415,   416,    82,    81,    94,     0,     0,   326,
       0,     0,   288,     0,     0,   346,   342,     0,     0,   436,
     422,   463,   519,   518,   517,   520,   464,   471,   436,   371,
       0,   377,     0,   367,   368,   494,   491,     0,     0,   133,
     131,   132,   130,   129,   128,   666,   667,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   610,   612,     0,
     599,   164,   171,   170,   169,   166,   173,   174,   168,   172,
     167,   175,     0,     0,     0,     0,     0,   501,   213,   543,
     542,     0,   248,     0,     0,   220,     0,   219,     0,   603,
       0,     0,   226,     0,     0,   224,     0,   234,     0,   232,
       0,   262,   261,     0,   256,     0,     0,   252,     0,   258,
       0,   290,     0,     0,   509,   512,   513,   514,   515,   312,
       0,     0,   513,     0,   463,   531,   530,   417,   432,   430,
     363,   306,     0,     0,   305,   308,   556,     0,   299,   302,
       0,   247,     0,     0,     0,     0,   245,     0,     0,     0,
       0,     0,     0,    61,   126,   123,   103,   115,   109,   108,
     107,   117,   104,   118,   113,   106,   114,   112,   110,   111,
     105,   116,   119,     0,   122,   121,    90,    89,    88,     0,
       0,     0,   198,     0,   199,   326,   346,     0,     0,     0,
     319,   188,   186,   318,   346,   346,   327,   337,     0,   424,
     471,     0,     0,   365,   426,     0,   374,   419,   419,   673,
     674,     0,     0,     0,     0,     0,     0,   290,     0,     0,
       0,   601,   513,   657,   657,   217,   216,   223,     0,     0,
     570,     0,   569,     0,   602,     0,     0,   576,   227,     0,
     575,   225,   235,   233,   255,   251,   588,   257,     0,    74,
     287,   266,   263,   506,   539,   539,   433,   431,   358,   359,
     513,   418,     0,   310,     0,     0,     0,   300,     0,   242,
     585,     0,     0,   240,   584,     0,   697,     0,     0,     0,
       0,     0,     0,    63,     0,     0,     0,    93,     0,    93,
      96,    99,    85,    84,    83,   346,   205,   211,   209,   292,
     346,   329,   328,   340,   344,   485,     0,   486,   473,   476,
       0,   472,     0,   420,   369,   370,     0,     0,     0,     0,
       0,     0,     0,     0,   292,   605,   574,     0,     0,   604,
     580,     0,     0,     0,   260,     0,     0,   435,   434,   311,
     309,     0,   303,     0,   243,     0,   241,     0,     0,   124,
      92,    91,     0,     0,     0,     0,     0,   206,   289,   330,
       0,   484,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   465,   467,   469,   366,   466,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,   620,   621,
     301,     0,     0,    98,   101,    97,   100,    86,   345,   475,
     477,   478,   481,   482,   483,   479,   480,   474,   468,     0,
       0,     0,     0,     0,     0,   573,   572,   579,   578,   622,
     623,   587,   586
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1131, -1131, -1131,     3,   -63,  2536, -1131,    59,   815, -1131,
   -1131, -1131,  -147,  -116, -1131,   324, -1131, -1131,  -107, -1131,
   -1131,    36,   661, -1131,  -574,  2678,   550,  -603, -1131,  -857,
   -1131, -1131, -1131,    68, -1131, -1131, -1131,   918, -1131,  3222,
    -206, -1131, -1131,  -466,  2750, -1130,  -871, -1131, -1131,   -27,
   -1131, -1131, -1131, -1131, -1131, -1131, -1131, -1131,   559,  1135,
   -1131,   855, -1131, -1131,   -54,  2313, -1131, -1131, -1131, -1131,
     598, -1131,   -75, -1131, -1131, -1131, -1131,   396, -1131, -1131,
   -1131,  -120, -1131,  -368,  -856, -1131, -1131, -1131, -1131, -1131,
    -396, -1131,   605, -1131, -1131, -1131, -1131, -1131,   383, -1131,
    2356, -1131, -1131, -1131, -1131,   552, -1131, -1131, -1131, -1131,
      33,  -405,  -135,  -835,  -896,  -618, -1131,    38, -1131,    55,
     391,   -53,   649,   -50, -1131, -1131,  -367,  -844, -1131,  -285,
   -1131,  -124,  -287,  -279,  -704, -1131, -1131,    75,   272, -1131,
    -212,   839, -1131,  -164,   424,   163,  -427,  -805,  -625, -1131,
   -1131, -1131,  -613,  -544, -1131,  -790,   -16,    50, -1131,  -271,
    -523,  -556,   531, -1131, -1131, -1131, -1131,   932, -1131,   -19,
   -1131, -1131,  -139, -1131,  -770, -1131, -1131,  1249,  1313, -1131,
   -1131, -1131, -1131,   -13, -1131,    21,  1338, -1131,  1873,  2075,
   -1131, -1131, -1131, -1131, -1131, -1131, -1131, -1131, -1131,  -484
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   364,   112,   805,   114,   115,   116,   386,
     387,   117,   118,   119,   120,   393,   620,   868,   869,   121,
     122,   123,   398,   124,   125,   126,   365,   896,   627,   897,
     127,   128,  1064,   624,   625,   129,   130,   224,   261,   285,
     131,   263,   132,   319,   518,   133,   134,   306,   305,   135,
     136,   137,   138,   139,   140,   636,   900,   141,   142,   267,
     143,   275,   144,   813,   814,   210,   146,   147,   148,   149,
     588,   845,  1030,   150,   151,   841,  1025,   292,   293,   152,
     153,   154,   155,   408,   905,   156,   157,   413,   907,   908,
     414,   158,   211,  1250,   160,   161,   320,   321,   322,  1014,
     162,   334,   581,   838,   163,   164,  1202,   165,   166,   698,
     699,   892,   893,   894,  1018,   919,   416,   647,  1095,   648,
     575,   649,   324,   569,   431,   420,   426,   917,  1265,  1266,
     167,  1093,  1198,  1199,  1200,   168,   169,   433,   434,   704,
     170,   171,   229,   286,   287,   548,   549,   821,   326,   915,
     654,   655,   552,   327,   835,   836,   366,   369,   370,   508,
     509,   510,   212,   173,   174,   175,   176,   213,   178,   221,
     222,   770,   525,   978,   771,   772,   179,   214,   215,   182,
     183,   184,   335,   185,   405,   511,   217,   187,   218,   219,
     190,   191,   192,   193,   376,   194,   195,   196,   197,   198
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     216,   226,   390,   318,   225,   111,   435,   839,   227,   540,
     411,   233,   774,   323,   234,   642,   325,   502,   811,   403,
     979,   268,   270,   272,  1070,   276,   700,   891,   914,   842,
     507,  1019,   837,   563,  1086,   634,   291,   911,   294,   295,
    1012,  1065,   840,  1076,   300,   606,   301,   288,   302,   650,
    -285,   485,   550,   328,   329,  -286,   763,   234,   336,   338,
     340,   341,   342,   870,   550,   553,   346,  1090,   347,   721,
     391,    24,  1264,   586,   507,   352,   279,     3,   354,   336,
     981,   923,   357,   924,   358,   818,  1091,   359,   360,   395,
     361,    21,   337,   374,   234,   336,   375,   377,   507,    28,
    1195,   307,   288,   580,   288,   288,   519,   614,   615,    36,
    -524,   974,   566,   355,  1261,  -307,  1146,  1261,   519,   815,
    1264,   309,    60,   611,   401,   311,   404,   396,   199,   371,
     521,  1252,   527,   530,    69,   519,   599,  1195,   507,   947,
     652,   538,   348,   350,  1092,   607,   833,   550,  1040,   312,
    -360,   612,   313,   388,    62,   567,   607,   397,  -307,    89,
    1253,   604,   600,   504,   314,   554,   438,   236,   519,   725,
    1149,  -522,   280,   608,    71,  -268,  -524,    71,  -524,  1111,
     380,   505,   948,   317,   764,   384,    91,   913,  -524,   490,
     491,   492,   223,   494,   346,   234,   375,   551,  -524,  -522,
    1197,  1143,  -269,   831,  1006,  1086,  -524,  1147,   501,   551,
    -524,   392,  -307,  1191,  1192,   751,   752,   557,   384,  1185,
     504,   519,  1176,   336,  -524,  1150,  1196,   970,  -524,  1022,
     296,  -522,   533,   417,   605,   609,  1182,  1197,  -524,   943,
     384,   616,   384,   384,  -522,   727,   383,   646,   504,   384,
    1262,  -524,   384,   418,  1227,   384,   721,  -522,  1177,   596,
     427,   601,   488,   419,  -524,   898,   489,   336,   220,   519,
      87,   441,  1023,   722,   644,   -71,  1041,  1069,   564,   565,
     507,   507,   633,   597,   519,   653,   723,   602,   441,   297,
    1016,   336,   551,   445,   -71,   390,   701,  1071,   645,  -275,
     384,   451,   342,   346,  -522,  -339,   774,  1215,   298,  -522,
     307,   428,  -335,   -71,  1086,   724,  -524,   578,   579,  1249,
    1243,  1239,  1245,   429,  1032,   299,  1024,   421,   816,   442,
     309,   336,   -70,   443,   311,   507,  -335,   902,   778,  1237,
     909,   949,   430,   -71,   903,  1228,   442,   826,   504,  1112,
     443,   -70,   318,  1219,   817,   598,   725,   783,   312,   603,
     964,   313,   726,    62,   489,  -274,   910,   950,   595,  1002,
     -70,   816,   228,   314,   972,   422,   820,   230,   423,   507,
    1079,   445,   626,  1001,   628,   816,   449,   236,  1148,   451,
     424,   639,   317,   454,   234,  1003,  1042,  1004,   445,   264,
     -70,   504,  1017,   449,  -740,   450,   451,  -594,  -740,   425,
     454,  1015,   783,   507,  -664,   199,  -664,   461,   486,   577,
     487,   593,  1043,  1110,   901,   467,   708,   709,   710,   712,
     714,   715,  1123,   716,   717,   718,   719,   720,  1107,   728,
     729,   730,   731,   732,   733,   734,   735,   736,   737,   738,
     739,   740,   741,   742,   743,   744,  -425,  -283,  1124,   783,
    1174,   199,   336,   336,   783,   774,   705,   871,   336,  -607,
     554,   336,   336,   336,   336,   754,   756,  -273,   280,  1204,
    1205,  -606,   873,   773,   706,  1189,   870,   776,   707,  -270,
    1214,   779,  1251,   780,   877,  -607,   746,   747,   785,  -277,
     874,   234,   748,   787,   789,   749,   750,  -606,   318,   793,
     795,  1087,   878,   554,  1071,   799,  1071,   700,   323,   705,
    -281,   325,   806,   554,  1071,   808,  -741,  1071,   336,   303,
    -741,   967,  1072,   172,  1180,  1254,  -278,  1187,   819,  -742,
     822,  1188,  1283,  -742,   294,  1285,   277,   827,   828,   278,
    1291,  1292,  1293,  1294,  1295,  1296,   507,   172,   328,   172,
     307,   304,  1255,   328,  1256,  1257,   445,  1258,  1259,  -272,
     343,   449,   336,  1242,   451,  1244,   344,   507,   454,  -276,
     309,  -737,  -282,   779,   311,  -737,   336,  -665,   336,  -665,
    -284,  -271,   441,   855,   779,   858,  -279,   860,   862,   356,
     865,   863,   864,   234,   406,   394,   336,   159,   312,    94,
     172,   313,   407,   378,   172,   172,  -591,   895,   895,  -664,
     371,  -664,   371,   314,   415,  -661,   595,  -661,  1263,   495,
     906,   159,   384,   159,   542,  -663,   436,  -663,   172,   774,
     328,    42,   317,   172,  -346,  -662,   927,  -662,   928,   849,
     442,   851,  -590,   437,   443,   983,   986,   496,  -595,   988,
     990,  -596,  -593,  -592,  -346,   498,   506,    58,   503,  1045,
     727,   384,   512,   396,  -346,   571,  1263,   514,   541,   560,
     561,   441,   570,   920,   159,   576,   613,   582,   159,   159,
     925,   585,  1193,   618,   592,   619,   629,   507,   643,   651,
     635,   641,   445,   446,   640,   447,   448,   449,   656,   450,
     451,   452,   159,   703,   454,  1033,  1035,   159,   451,   745,
     460,   461,   441,   762,   464,   781,   609,   465,   466,   467,
     782,   783,   784,   801,   790,   802,   804,  1078,   468,   442,
     507,   807,   809,   443,   787,   939,   793,   942,   806,   336,
     945,   858,   172,   810,   820,   829,   965,   966,  1085,   834,
     843,  1088,   844,   392,   899,   853,   504,   859,   336,   441,
     336,   971,   916,   973,   918,  -143,  -164,   773,   936,   980,
     442,  -171,  -170,  -169,   443,   441,  -166,  1098,   991,   992,
    -173,   445,   446,  -174,   447,   448,   449,  -168,   450,   451,
     921,  -172,   969,   454,  -144,  -167,  -175,   336,   929,   460,
     461,   922,   930,   464,   172,   935,   465,   466,   467,   172,
     993,   328,   995,   931,  1021,  1020,   159,   442,   932,   933,
     934,   443,   445,   446,  1013,  1074,   507,   449,   538,   450,
     451,  1008,  1084,   442,   454,   538,   318,   443,   946,  1100,
    1071,   461,  1026,  1075,  1099,   318,  1102,   465,   466,   467,
     626,  1105,  1108,  1080,  1066,   307,  1068,   895,   895,  1138,
     357,  1139,   358,  1225,  1226,   360,   361,  1140,  1156,   445,
     446,  1144,   447,   448,   449,   309,   450,   451,   159,   311,
     336,   454,  1145,   159,  1161,   445,   172,  1165,   461,  1181,
     449,  1203,   450,   451,   465,   466,   467,   454,   909,  1223,
    1231,  1248,  1278,   312,   461,   172,   313,  1279,    62,  1309,
     465,   466,   467,  1284,  1101,   172,  1310,  1104,   314,  1085,
    1286,  1253,   385,   336,   177,  1173,   773,  1190,   507,   507,
     539,  1044,   866,   231,   623,   617,   353,   317,  1141,   589,
     712,   754,  1155,   824,   832,  1097,  1094,   637,   177,   830,
     177,  1120,   702,  1122,  1089,  1288,  1194,   568,  1127,   390,
     159,  1130,   390,  1297,  1290,  1077,   400,   926,   823,  1005,
     380,     0,     0,  1136,    23,    24,     0,     0,     0,   159,
       0,     0,     0,   381,     0,    31,   382,     0,     0,   159,
     328,    37,     0,     0,  1151,     0,     0,  1154,    42,     0,
     234,   177,     0,  1084,     0,   177,   177,     0,     0,  1160,
       0,  1164,     0,     0,  1166,  1168,  1170,  1172,     0,   336,
     983,   986,  1033,  1035,    58,     0,    60,     0,  1085,   177,
       0,     0,     0,     0,   177,     0,   383,   172,    69,     0,
     895,     0,     0,   172,     0,     0,     0,     0,  1183,     0,
    1184,     0,     0,     0,   172,     0,   172,    85,     0,     0,
      87,     0,     0,    89,   390,     0,     0,   721,     0,  1201,
       0,     0,   538,   538,     0,     0,   538,   538,     0,  1120,
    1122,   951,  1127,  1130,   952,  1160,  1164,     0,   538,   953,
     538,     0,     0,     0,     0,  1216,  1217,     0,  1218,   201,
     773,     0,  1220,  1221,     0,     0,  1222,     0,     0,   954,
     441,   159,  1084,     0,     0,     0,   955,   159,     0,     0,
       0,   336,   336,     0,     0,   172,   956,     0,   159,   172,
     159,     0,     0,  1232,   957,  1233,     0,     0,     0,  1235,
       0,     0,     0,   177,   172,     0,     0,     0,     0,     0,
       0,   958,  1241,   895,   895,   895,   895,     0,     0,     0,
     269,   271,     0,   959,     0,     0,     0,   725,   442,     0,
       0,     0,   443,     0,   960,     0,     0,     0,     0,     0,
     961,     0,     0,  1216,  1267,  1268,  1220,  1269,  1270,  1271,
    1272,     0,     0,     0,     0,     0,     0,     0,     0,   159,
       0,     0,     0,   159,     0,   177,     0,     0,     0,     0,
     177,     0,     0,     0,     0,     0,     0,     0,   159,     0,
     445,   446,     0,  1287,   448,   449,     0,   450,   451,  1289,
       0,     0,   454,     0,   373,     0,     0,     0,     0,   461,
       0,   180,     0,     0,     0,   465,   466,   467,     0,     0,
    1305,  1306,  1307,  1308,  1298,     0,     0,     0,  1311,  1312,
       0,     0,     0,     0,     0,   180,     0,   180,     0,     0,
       0,     0,     0,     0,     0,     0,  1305,  1306,  1307,  1308,
    1311,  1312,     0,     0,     0,     0,     0,   177,     0,     0,
       0,     0,     0,   962,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   181,   177,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   177,     0,   180,     0,
       0,     0,   180,   180,     0,   172,     0,     0,   172,   181,
     186,   181,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   904,     0,     0,     0,   180,     0,     0,     0,
       0,   180,     0,     0,   186,     0,   186,     0,     0,     0,
       0,     0,     0,  -326,     0,     0,     0,  -326,  -326,     0,
       0,     0,     0,     0,   172,     0,  -326,     0,  -326,  -326,
     172,     0,   181,     0,  -326,     0,   181,   181,     0,     0,
       0,  -326,     0,     0,  -326,     0,     0,     0,     0,   159,
       0,     0,   159,     0,     0,     0,     0,   186,     0,     0,
     181,   186,   186,     0,  -326,   181,     0,  -326,     0,  -326,
       0,  -326,     0,  -326,  -326,     0,  -326,     0,     0,  -326,
       0,  -326,     0,     0,     0,   186,     0,     0,   177,     0,
     186,     0,     0,     0,   177,     0,     0,     0,   159,     0,
    -326,     0,     0,  -326,   159,   177,  -326,   177,     0,     0,
     180,     0,     0,     0,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,     0,
       0,  -326,     0,     0,  1113,  1114,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   172,     0,   172,     0,     0,
       0,     0,   172,     0,     0,   172,  -326,     0,   484,     0,
       0,  -661,  -326,  -661,   172,     0,   172,     0,     0,   172,
       0,     0,   180,     0,   181,     0,   177,   180,     0,     0,
     177,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   172,     0,   177,     0,     0,     0,   186,
       0,     0,     0,   172,     0,   172,     0,     0,     0,     0,
       0,     0,   172,     0,     0,     0,     0,   711,   713,   159,
       0,   159,     0,     0,     0,     0,   159,     0,     0,   159,
       0,     0,     0,     0,     0,     0,   181,     0,   159,     0,
     159,   181,     0,   159,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   180,     0,     0,     0,     0,     0,
       0,   186,     0,   753,   755,     0,   186,   159,     0,     0,
       0,     0,     0,   180,     0,   775,     0,   159,     0,   159,
       0,     0,     0,   180,     0,     0,   159,     0,     0,   172,
       0,     0,   788,     0,     0,     0,   172,     0,   794,     0,
       0,     0,     0,   798,     0,     0,     0,     0,     0,   172,
       0,     0,     0,     0,     0,     0,     0,     0,   181,     0,
       0,     0,     0,   172,     0,     0,     0,     0,     0,     0,
       0,     0,   172,     0,     0,     0,   172,   181,     0,     0,
       0,     0,     0,   186,     0,     0,     0,   181,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   186,   159,     0,     0,     0,     0,     0,     0,
     159,     0,   186,     0,     0,     0,   177,     0,     0,   177,
       0,   854,     0,   159,     0,     0,     0,     0,     0,     0,
     269,   373,     0,     0,   172,     0,     0,   159,     0,     0,
       0,     0,     0,     0,     0,   180,   159,     0,   172,   172,
     159,   180,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   180,     0,   180,   177,     0,     0,     0,     0,
       0,   177,   172,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   172,   172,   172,   172,     0,     0,
       0,     0,   172,   172,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   159,   181,
       0,     0,     0,     0,     0,   181,     0,     0,     0,     0,
       0,     0,   159,   159,     0,     0,   181,     0,   181,     0,
       0,     0,     0,   180,   186,     0,     0,   180,     0,     0,
     186,     0,     0,     0,     0,     0,   159,     0,     0,     0,
       0,   186,   180,   186,     0,   188,     0,     0,   159,   159,
     159,   159,     0,     0,     0,     0,   159,   159,     0,     0,
       0,     0,     0,   938,     0,   941,     0,     0,   944,   188,
       0,   188,     0,     0,     0,     0,   177,     0,   177,   237,
       0,     0,     0,   177,     0,     0,   177,   181,     0,     0,
       0,   181,     0,   238,   239,   177,   240,   177,     0,     0,
     177,   241,     0,     0,     0,     0,   181,     0,     0,   242,
       0,     0,   186,     0,     0,   243,   186,     0,     0,     0,
       0,   244,   188,     0,   177,   245,   188,   188,   246,     0,
       0,   186,     0,     0,   177,     0,   177,     0,   247,     0,
       0,     0,     0,   177,     0,   248,   249,     0,     0,     0,
     188,     0,     0,   250,     0,   188,     0,     0,     0,     0,
       0,     0,     0,   251,     0,     0,     0,     0,     0,     0,
       0,     0,   252,   253,     0,   254,     0,   255,     0,   256,
       0,   441,   257,     0,     0,     0,   258,   545,     0,   259,
     380,     0,   260,     0,    23,    24,     0,     0,     0,     0,
       0,     0,     0,   381,     0,    31,   382,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,     0,    42,     0,
     177,  -346,     0,   180,     0,     0,   180,   177,     0,     0,
       0,     0,     0,   -69,     0,     0,     0,     0,     0,   442,
     177,  -346,     0,   443,    58,     0,    60,   189,   546,     0,
      64,  -346,   -69,    65,   177,     0,   383,     0,    69,     0,
       0,     0,     0,   177,   188,     0,     0,   177,   711,   753,
       0,   189,   180,   189,   444,     0,     0,    85,   180,     0,
      87,     0,     0,    89,     0,     0,     0,   181,     0,     0,
     181,   445,   446,     0,   447,   448,   449,     0,   450,   451,
     452,     0,   453,   454,   455,   456,     0,   458,   459,   460,
     461,   462,   186,   464,     0,   186,   465,   466,   467,     0,
       0,     0,     0,     0,   189,   177,   188,   468,   189,   189,
       0,   188,     0,   105,     0,     0,   181,     0,   384,   177,
     177,     0,   181,  1167,  1169,  1171,     0,     0,     0,     0,
       0,     0,   189,     0,     0,     0,     0,   189,     0,     0,
       0,   186,     0,   177,     0,     0,     0,   186,     0,     0,
       0,     0,     0,     0,     0,   177,   177,   177,   177,     0,
       0,     0,     0,   177,   177,     0,     0,     0,     0,     0,
       0,     0,     0,   180,     0,   180,     0,     0,     0,     0,
     180,     0,     0,   180,     0,     0,     0,     0,   188,     0,
       0,     0,   180,     0,   180,     0,     0,   180,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   188,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   188,     0,     0,
       0,   180,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   180,     0,   180,     0,     0,     0,   181,     0,   181,
     180,     0,     0,     0,   181,     0,   189,   181,     0,     0,
       0,     0,     0,     0,     0,     0,   181,     0,   181,     0,
       0,   181,   186,     0,   186,   145,     0,     0,     0,   186,
       0,     0,   186,     0,     0,     0,     0,     0,     0,     0,
       0,   186,     0,   186,     0,   181,   186,     0,     0,   145,
       0,   145,     0,     0,     0,   181,     0,   181,     0,     0,
       0,     0,     0,     0,   181,     0,     0,     0,   189,     0,
     186,     0,     0,   189,     0,     0,     0,   180,     0,     0,
     186,     0,   186,     0,   180,     0,     0,     0,     0,   186,
       0,     0,     0,     0,     0,     0,     0,   180,     0,   188,
       0,     0,   145,     0,     0,   188,     0,     0,     0,     0,
       0,   180,     0,     0,     0,     0,   188,     0,   188,     0,
     180,     0,     0,     0,   180,     0,     0,     0,     0,     0,
     145,     0,     0,     0,     0,   145,     0,   333,     0,     0,
       0,   181,     0,     0,     0,     0,     0,     0,   181,     0,
     189,     0,     0,     0,     0,     0,     0,     0,   333,     0,
       0,   181,     0,     0,     0,     0,   186,     0,     0,   189,
       0,     0,     0,   186,   333,   181,     0,     0,     0,   189,
       0,     0,   180,     0,   181,     0,   186,   188,   181,     0,
       0,   188,     0,     0,     0,     0,   180,   180,     0,     0,
     186,     0,     0,     0,     0,     0,   188,     0,     0,   186,
       0,     0,     0,   186,     0,     0,     0,     0,     0,     0,
     180,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   180,   180,   180,   180,     0,     0,     0,     0,
     180,   180,     0,     0,   145,     0,   181,     0,   113,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     181,   181,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   186,   232,     0,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   181,   186,   186,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   181,   181,   181,   181,
       0,   189,   333,     0,   181,   181,   145,   189,     0,   186,
       0,   145,     0,     0,     0,     0,     0,     0,   189,     0,
     189,   186,   186,   186,   186,   345,     0,     0,     0,   186,
     186,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   439,   333,     0,     0,     0,
       0,     0,   440,   113,     0,     0,     0,     0,   379,     0,
       0,     0,     0,     0,   441,   522,     0,     0,     0,     0,
     333,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,     0,     0,     0,     0,     0,     0,   188,   145,   189,
     188,     0,     0,   189,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   145,   189,     0,
     333,     0,     0,     0,     0,     0,     0,   145,     0,     0,
       0,     0,   442,     0,     0,     0,   443,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   188,   289,     0,     0,
       0,     0,   188,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   519,     0,     0,     0,   444,     0,     0,
       0,   289,     0,     0,     0,     0,     0,   497,     0,     0,
       0,   349,   351,     0,   445,   446,     0,   447,   448,   449,
       0,   450,   451,   452,     0,   453,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,     0,     0,   465,
     466,   467,     0,   384,     0,   389,     0,     0,     0,   290,
     468,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   544,
       0,     0,     0,   339,   555,     0,     0,     0,     0,   145,
       0,   333,   333,     0,     0,   145,     0,   333,     0,     0,
     333,   333,   333,   333,     0,     0,   145,   188,   145,   188,
       0,     0,     0,     0,   188,     0,     0,   188,     0,     0,
       0,     0,     0,     0,     0,     0,   188,     0,   188,     0,
       0,   188,     0,     0,     0,     0,     0,     0,     0,   189,
       0,     0,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   188,     0,   333,     0,     0,
       0,   113,     0,     0,   499,   188,     0,   188,     0,     0,
       0,     0,     0,     0,   188,     0,     0,   145,     0,     0,
     113,   145,     0,     0,     0,     0,     0,     0,   189,     0,
     113,     0,     0,     0,   189,     0,   145,     0,     0,     0,
       0,   333,     0,     0,     0,   289,   289,   289,   289,   289,
     289,     0,     0,   534,   537,   333,     0,   333,     0,     0,
     543,     0,     0,     0,     0,     0,     0,     0,     0,   289,
       0,   289,     0,     0,     0,   333,     0,     0,   289,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,     0,     0,     0,     0,     0,     0,   188,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,     0,     0,     0,     0,     0,   515,   289,   520,
     524,   526,   529,     0,     0,   188,     0,     0,     0,     0,
       0,   289,   289,     0,   188,     0,     0,     0,   188,     0,
       0,   556,     0,   558,     0,     0,     0,     0,     0,   189,
     562,   189,   555,     0,     0,     0,   189,     0,   555,   189,
       0,     0,     0,     0,     0,     0,     0,     0,   189,   803,
     189,     0,     0,   189,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     584,     0,     0,     0,     0,     0,   188,   189,     0,     0,
       0,     0,     0,   590,   591,     0,     0,   189,     0,   189,
     188,   188,     0,     0,     0,     0,   189,   145,   333,     0,
     145,     0,     0,     0,     0,     0,   410,     0,   237,     0,
       0,     0,     0,     0,   188,     0,     0,   333,     0,   333,
     852,     0,   238,   239,   857,   240,   188,   188,   188,   188,
     241,     0,     0,     0,   188,   188,     0,     0,   242,   113,
       0,     0,     0,     0,   243,     0,   145,     0,     0,     0,
     244,     0,   145,     0,   245,     0,   333,   246,     0,   289,
       0,     0,     0,     0,     0,     0,     0,   247,     0,     0,
       0,     0,     0,   189,   248,   249,     0,     0,     0,   289,
     189,     0,   250,   289,     0,   289,     0,     0,   289,     0,
       0,     0,   251,   189,     0,     0,     0,     0,     0,     0,
       0,   252,   253,     0,   254,     0,   255,   189,   256,     0,
       0,   257,     0,     0,     0,   258,   189,     0,   259,     0,
     189,   260,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   777,   262,     0,     0,     0,     0,     0,     0,   333,
       0,     0,     0,     0,     0,    94,   281,   282,   283,     0,
       0,   791,     0,     0,     0,   796,     0,   797,     0,     0,
     800,     0,     0,     0,     0,     0,     0,   145,     0,   145,
       0,     0,     0,     0,   145,     0,     0,   145,   189,     0,
       0,     0,   333,     0,     0,     0,   145,     0,   145,     0,
       0,   145,   189,   189,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,   239,     0,   240,     0,   145,   189,     0,   241,     0,
     994,     0,     0,   997,     0,   145,   242,   145,   189,   189,
     189,   189,   243,     0,   145,     0,   189,   189,   244,     0,
       0,     0,   245,     0,     0,   246,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   247,     0,   409,     0,   412,
       0,     0,   248,   249,     0,     0,     0,     0,     0,  1031,
     250,   432,     0,     0,     0,  1036,     0,     0,   333,     0,
     251,     0,     0,     0,     0,     0,     0,     0,     0,   252,
     253,     0,   254,     0,   255,     0,   256,     0,     0,   257,
       0,     0,     0,   258,   572,     0,   259,   493,     0,   260,
       0,   145,     0,     0,     0,     0,     0,     0,   145,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   145,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   145,   289,   289,     0,     0,
       0,     0,   289,   289,   145,     0,   289,   289,   145,     0,
       0,     0,     0,     0,     0,   573,   999,   513,     0,     0,
       0,     0,     0,     0,     0,   531,   532,     0,     0,     0,
     333,   333,     0,     0,     0,     0,   547,     0,     0,     0,
     555,     0,   555,     0,     0,     0,     0,   555,     0,     0,
     555,     0,  1028,     0,     0,     0,     0,     0,     0,  1134,
       0,  1135,     0,     0,  1137,     0,   145,     0,   975,   977,
       0,   289,   289,     0,   982,   985,   574,     0,   987,   989,
     145,   145,     0,     0,     0,     0,     0,     0,  1153,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1159,     0,
    1163,     0,     0,     0,   145,     0,     0,   857,     0,     0,
       0,     0,     0,     0,     0,     0,   145,   145,   145,   145,
       0,     0,     0,   237,   145,   145,     0,     0,     0,     0,
       0,     0,     0,   262,     0,     0,     0,   238,   239,     0,
     240,     0,     0,     0,   524,   241,     0,     0,     0,     0,
       0,     0,     0,   242,     0,     0,     0,     0,   638,   243,
       0,     0,     0,   412,     0,   244,     0,     0,     0,   245,
       0,     0,   246,   289,   289,     0,     0,     0,     0,   289,
       0,   537,   247,   697,     0,     0,     0,     0,   537,   248,
     249,   289,     0,     0,   289,     0,   289,   250,   289,     0,
       0,     0,     0,     0,  1224,     0,     0,   251,     0,     0,
       0,  1142,     0,     0,     0,     0,   252,   253,  1229,   254,
       0,   255,     0,   256,     0,     0,   257,  1234,     0,     0,
     258,  1236,     0,   259,     0,     0,   260,     0,  1157,     0,
       0,     0,   237,     0,     0,  1115,  1116,   765,     0,     0,
       0,  1117,     0,     0,     0,     0,   238,   239,     0,   240,
       0,     0,     0,  1128,   241,     0,  1131,     0,  1132,     0,
    1133,     0,   242,     0,     0,     0,     0,     0,   243,     0,
       0,     0,     0,     0,   244,     0,   441,     0,   245,  1277,
     384,   246,  1027,     0,   812,     0,     0,     0,   547,     0,
       0,   247,     0,  1159,  1163,     0,     0,     0,   248,   249,
       0,   825,     0,     0,     0,     0,   250,     0,     0,     0,
     574,     0,     0,     0,     0,   547,   251,   113,     0,     0,
       0,     0,     0,     0,     0,   252,   253,     0,   254,     0,
     255,     0,   256,     0,   442,   257,     0,     0,   443,   258,
       0,     0,   259,     0,     0,   260,     0,     0,     0,     0,
       0,     0,   289,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   289,   289,   289,   289,   444,
       0,   882,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   445,   446,     0,   447,
     448,   449,     0,   450,   451,   452,     0,   453,   454,   455,
     456,   284,   458,   459,   460,   461,   462,   463,   464,     0,
       0,   465,   466,   467,     0,   537,   537,     0,     0,   537,
     537,     0,   468,     0,  1230,     0,     0,     0,     0,  1280,
       0,   537,     0,   537,     0,     0,     0,   975,   977,   982,
     985,     0,     0,     0,     0,     0,     0,   432,   912,     0,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   200,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
     308,     0,   201,     0,     0,     0,    33,   202,   203,     0,
       0,   204,    39,     0,     0,     0,   310,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   206,     0,     0,     0,     0,
       0,     0,     0,    74,   315,    76,    77,    78,   316,     0,
      80,  1000,     0,    82,     0,     0,     0,    86,   547,     0,
      88,     0,     0,    90,     0,     0,     0,     0,  1009,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1029,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,     0,     0,     0,   106,     0,   208,     0,   108,
       0,   209,     0,     0,   110,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,   412,
       0,    -2,     4,     0,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,   697,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -69,     0,    40,
      41,    42,     0,    43,  -346,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,    52,   -69,    53,    54,     0,
      55,    56,    57,     0,  -346,     0,     0,    58,    59,    60,
      61,    62,    63,    64,  -346,   -69,    65,    66,    67,    68,
       0,    69,    70,    71,   812,    72,    73,    74,    75,    76,
      77,    78,    79,     0,    80,    81,     0,    82,    83,    84,
      85,    86,     0,    87,    88,   -69,    89,    90,     0,  1029,
      91,     0,    92,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,   105,     0,     0,   106,
       0,   107,     0,   108,     0,   109,     0,     0,   110,     4,
     412,     5,     6,     7,     8,     9,    10,    11,    12,     0,
    -692,     0,    13,    14,    15,    16,    17,  -692,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,  -692,
      28,    29,  -692,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -69,     0,    40,    41,    42,     0,
      43,  -346,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -69,    53,    54,     0,    55,    56,    57,
       0,  -346,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -346,   -69,    65,    66,    67,    68,  -692,    69,    70,
      71,  -692,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,   -69,    89,    90,     0,     0,    91,     0,    92,
       0,     0,  -692,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
    -692,    97,  -692,  -692,  -692,  -692,  -692,  -692,  -692,     0,
    -692,  -692,  -692,  -692,  -692,  -692,  -692,  -692,  -692,  -692,
    -692,  -692,   104,   105,  -692,  -692,  -692,     0,   107,  -692,
     108,     0,   109,     0,   362,  -692,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -69,
       0,    40,    41,    42,     0,    43,  -346,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -69,    53,
      54,     0,    55,    56,    57,     0,  -346,     0,     0,    58,
      59,    60,    61,    62,    63,    64,  -346,   -69,    65,    66,
      67,    68,     0,    69,    70,    71,     0,    72,    73,    74,
      75,    76,    77,    78,    79,     0,    80,    81,     0,    82,
      83,    84,    85,    86,     0,    87,    88,   -69,    89,    90,
       0,     0,    91,     0,    92,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,   105,     0,
       0,   106,     0,   107,   363,   108,     0,   109,     0,     4,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -69,     0,    40,    41,    42,     0,
      43,  -346,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -69,    53,    54,     0,    55,    56,    57,
       0,  -346,     0,     0,    58,    59,    60,    61,    62,    63,
      64,  -346,   -69,    65,    66,    67,    68,     0,    69,    70,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,   -69,    89,    90,     0,     0,    91,     0,    92,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   105,     0,     0,   106,     0,   107,   594,
     108,     0,   109,     0,   621,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -69,
       0,    40,    41,    42,     0,    43,  -346,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -69,    53,
      54,     0,    55,    56,    57,     0,  -346,     0,     0,    58,
      59,    60,    61,    62,    63,    64,  -346,   -69,    65,    66,
      67,    68,     0,    69,    70,    71,     0,    72,    73,    74,
      75,    76,    77,    78,    79,     0,    80,    81,     0,    82,
      83,    84,    85,    86,     0,    87,    88,   -69,    89,    90,
       0,     0,    91,     0,    92,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,   105,     0,
       0,   106,     0,   107,   622,   108,     0,   109,     0,     4,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -69,     0,    40,    41,    42,     0,
      43,  -346,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -69,    53,    54,     0,    55,    56,    57,
       0,  -346,     0,     0,    58,    59,    60,    61,    62,    63,
      64,  -346,   -69,    65,    66,    67,    68,     0,    69,    70,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,   -69,    89,    90,     0,     0,    91,     0,    92,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   105,     0,     0,   106,     0,   107,   872,
     108,     0,   109,     0,     4,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -69,
       0,    40,    41,    42,     0,    43,  -346,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -69,    53,
      54,     0,    55,    56,    57,     0,  -346,     0,     0,    58,
      59,    60,    61,   378,    63,    64,  -346,   -69,    65,    66,
      67,    68,     0,    69,    70,    71,     0,    72,    73,    74,
      75,    76,    77,    78,    79,     0,    80,    81,     0,    82,
      83,    84,    85,    86,     0,    87,    88,   -69,    89,    90,
       0,     0,    91,     0,    92,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,   105,     0,
       0,   106,     0,   107,     0,   108,     0,   109,     0,     4,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -69,     0,    40,    41,    42,     0,
      43,  -346,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -69,    53,    54,     0,    55,    56,    57,
       0,  -346,     0,     0,    58,    59,    60,    61,    62,    63,
      64,  -346,   -69,    65,    66,    67,    68,     0,    69,    70,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,   -69,    89,    90,     0,     0,    91,     0,    92,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   105,     0,     0,   106,     0,   107,     0,
     108,     0,   109,     0,     4,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -69,
       0,    40,    41,    42,     0,    43,  -346,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -69,    53,
      54,     0,    55,    56,    57,     0,  -346,     0,     0,    58,
      59,    60,    61,     0,    63,    64,  -346,   -69,    65,    66,
      67,    68,     0,    69,    70,    71,     0,    72,    73,    74,
      75,    76,    77,    78,    79,     0,    80,    81,     0,    82,
      83,    84,    85,    86,     0,    87,    88,   -69,    89,    90,
       0,     0,    91,     0,    92,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,   105,     0,
       0,   106,     0,   107,     0,   108,     0,   109,     0,     4,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,   786,    39,   -69,     0,    40,    41,    42,     0,
      43,  -346,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -69,    53,    54,     0,    55,    56,    57,
       0,  -346,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -346,   -69,    65,    66,    67,    68,     0,    69,    70,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,   -69,    89,    90,     0,     0,    91,     0,    92,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   105,     0,     0,   106,     0,   107,     0,
     108,     0,   109,     0,     4,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,   792,    39,   -69,
       0,    40,    41,    42,     0,    43,  -346,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -69,    53,
      54,     0,    55,    56,    57,     0,  -346,     0,     0,    58,
      59,    60,    61,     0,    63,    64,  -346,   -69,    65,    66,
      67,    68,     0,    69,    70,    71,     0,    72,    73,    74,
      75,    76,    77,    78,    79,     0,    80,    81,     0,    82,
      83,    84,    85,    86,     0,    87,    88,   -69,    89,    90,
       0,     0,    91,     0,    92,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,   105,     0,
       0,   106,     0,   107,     0,   108,     0,   109,     0,     4,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,  1119,    39,   -69,     0,    40,    41,    42,     0,
      43,  -346,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -69,    53,    54,     0,    55,    56,    57,
       0,  -346,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -346,   -69,    65,    66,    67,    68,     0,    69,    70,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,   -69,    89,    90,     0,     0,    91,     0,    92,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   105,     0,     0,   106,     0,   107,     0,
     108,     0,   109,     0,     4,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,  1121,    39,   -69,
       0,    40,    41,    42,     0,    43,  -346,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -69,    53,
      54,     0,    55,    56,    57,     0,  -346,     0,     0,    58,
      59,    60,    61,     0,    63,    64,  -346,   -69,    65,    66,
      67,    68,     0,    69,    70,    71,     0,    72,    73,    74,
      75,    76,    77,    78,    79,     0,    80,    81,     0,    82,
      83,    84,    85,    86,     0,    87,    88,   -69,    89,    90,
       0,     0,    91,     0,    92,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,   105,     0,
       0,   106,     0,   107,     0,   108,     0,   109,     0,     4,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,  1126,    39,   -69,     0,    40,    41,    42,     0,
      43,  -346,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -69,    53,    54,     0,    55,    56,    57,
       0,  -346,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -346,   -69,    65,    66,    67,    68,     0,    69,    70,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,   -69,    89,    90,     0,     0,    91,     0,    92,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   105,     0,     0,   106,     0,   107,     0,
     108,     0,   109,     0,     4,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,  1129,    39,   -69,
       0,    40,    41,    42,     0,    43,  -346,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -69,    53,
      54,     0,    55,    56,    57,     0,  -346,     0,     0,    58,
      59,    60,    61,     0,    63,    64,  -346,   -69,    65,    66,
      67,    68,     0,    69,    70,    71,     0,    72,    73,    74,
      75,    76,    77,    78,    79,     0,    80,    81,     0,    82,
      83,    84,    85,    86,     0,    87,    88,   -69,    89,    90,
       0,     0,    91,     0,    92,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,   105,     0,
       0,   106,     0,   107,     0,   108,     0,   109,     0,     4,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
    1152,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -69,     0,    40,    41,    42,     0,
      43,  -346,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -69,    53,    54,     0,    55,    56,    57,
       0,  -346,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -346,   -69,    65,    66,    67,    68,     0,    69,    70,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,   -69,    89,    90,     0,     0,    91,     0,    92,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   105,     0,     0,   106,     0,   107,     0,
     108,     0,   109,     0,     4,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,  1158,    39,   -69,
       0,    40,    41,    42,     0,    43,  -346,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -69,    53,
      54,     0,    55,    56,    57,     0,  -346,     0,     0,    58,
      59,    60,    61,     0,    63,    64,  -346,   -69,    65,    66,
      67,    68,     0,    69,    70,    71,     0,    72,    73,    74,
      75,    76,    77,    78,    79,     0,    80,    81,     0,    82,
      83,    84,    85,    86,     0,    87,    88,   -69,    89,    90,
       0,     0,    91,     0,    92,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,   105,     0,
       0,   106,     0,   107,     0,   108,     0,   109,     0,     4,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,  1162,    39,   -69,     0,    40,    41,    42,     0,
      43,  -346,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -69,    53,    54,     0,    55,    56,    57,
       0,  -346,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -346,   -69,    65,    66,    67,    68,     0,    69,    70,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,   -69,    89,    90,     0,     0,    91,     0,    92,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   105,     0,     0,   106,     0,   107,     0,
     108,     0,   109,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,   659,     0,    13,     0,     0,
      16,    17,   661,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
       0,     0,    41,     0,     0,    43,     0,   666,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,   536,   470,
     471,   472,   473,   474,     0,     0,   477,   478,   479,   480,
       0,   482,   483,     0,   883,   884,   885,   886,   887,   679,
       0,   680,     0,   100,     0,   681,   682,   683,   684,   685,
     686,   687,   888,   689,   690,   102,   889,   104,     0,   692,
     693,   890,   695,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,     0,    16,    17,     0,    18,     0,
     200,    20,    21,    22,     0,     0,     0,     0,    27,     0,
      28,    29,     0,   201,     0,     0,     0,    33,    34,    35,
      36,     0,    38,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,    49,
      50,    51,    52,     0,    53,    54,     0,    55,    56,    57,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,    91,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   107,     0,
     108,     0,   109,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,   399,
      24,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,    60,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,    69,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,    89,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,   330,   331,     0,    86,   367,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,   368,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
     766,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
     767,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,   768,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,   769,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,   330,   331,     0,    86,   367,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,   848,   209,     0,     0,   110,     5,     6,     7,     8,
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
     330,   331,     0,    86,   367,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,   850,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,   330,   331,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   332,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
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
     330,   331,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,   867,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,  1007,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,   330,   331,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,   265,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,   266,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,   273,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,   274,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
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
     330,   331,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
     266,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,   372,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,  -736,     0,     0,     0,  -736,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
     266,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,   288,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   107,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,   402,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
      19,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,    68,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
      87,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
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
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,   372,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,   846,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
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
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,   856,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,   861,     0,   110,     5,     6,     7,     8,
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
       0,     0,     0,    86,  1067,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,  1240,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
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
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,   308,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,   310,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,   315,    76,    77,    78,   316,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   937,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   940,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,  1207,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,  1208,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,  1210,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,  1211,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,  1212,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,  1213,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,   441,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   442,     0,     0,     0,   443,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     6,     7,     8,     9,
      10,    11,    12,     0,   445,   446,    13,   447,   448,   449,
      17,   450,   451,   452,   200,   453,   454,   455,   456,   307,
     458,   459,   460,   461,   462,     0,   464,   201,     0,   465,
     466,   467,   202,   203,     0,     0,   204,     0,     0,   309,
     468,     0,     0,   311,     0,     0,     0,   205,     0,     0,
      47,    48,     0,     0,     0,     0,    52,     0,    53,    54,
       0,     0,     0,     0,     0,     0,     0,   312,   441,    59,
     313,    61,    62,    63,     0,     0,     0,     0,    66,     0,
     206,     0,   314,     0,     0,     0,     0,     0,    74,     0,
      76,     0,    78,     0,     0,     0,     0,     0,     0,     0,
       0,   317,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   442,     0,     0,     0,
     443,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   208,     0,   108,  -350,   209,     0,     0,   110,
       6,     7,     8,     9,    10,    11,    12,     0,   445,   446,
      13,   447,   448,   449,    17,   450,   451,   452,   200,     0,
     454,   455,   456,   307,   458,   459,   460,   461,     0,     0,
     464,   201,     0,   465,   466,   467,   202,   203,     0,     0,
     204,     0,     0,   309,   468,     0,     0,   311,     0,     0,
       0,   205,     0,     0,    47,    48,     0,     0,     0,     0,
      52,     0,    53,    54,     0,     0,     0,     0,     0,     0,
       0,   312,     0,    59,   313,    61,    62,    63,     0,     0,
       0,     0,    66,     0,   206,     0,   314,     0,     0,     0,
       0,     0,    74,     0,    76,     0,    78,     0,     0,     0,
       0,     0,     0,     0,     0,   317,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   657,     0,   658,     0,    95,    96,    97,
      98,    99,     0,     0,   659,     0,   100,     0,     0,   660,
     239,   661,   662,     0,     0,   101,     0,   663,   102,   103,
     104,     0,     0,     0,   106,   242,   208,   201,   108,     0,
     209,   243,     0,   110,     0,     0,     0,   664,     0,     0,
       0,   245,     0,     0,   665,     0,   666,     0,     0,     0,
       0,     0,     0,     0,   667,     0,     0,     0,     0,     0,
       0,   248,   668,     0,     0,     0,     0,     0,     0,   250,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   669,
       0,     0,     0,     0,     0,     0,     0,     0,   252,   253,
       0,   670,     0,   255,     0,   671,     0,     0,   257,     0,
       0,     0,   672,     0,     0,   259,     0,     0,   673,     0,
       0,     0,     0,     0,     0,     0,     0,   536,   470,   471,
     472,   473,   474,     0,     0,   477,   478,   479,   480,     0,
     482,   483,     0,   674,   675,   676,   677,   678,   679,     0,
     680,     0,     0,     0,   681,   682,   683,   684,   685,   686,
     687,   688,   689,   690,     0,   691,     0,     0,   692,   693,
     694,   695,     0,     0,   696,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   200,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   201,     0,     0,
       0,    33,   202,   203,     0,     0,   204,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   205,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     206,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   207,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,     0,   103,   104,     0,     0,     0,
     106,     0,   208,     0,   108,     0,   209,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   200,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   201,
       0,     0,     0,    33,   202,   203,     0,     0,   204,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   205,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,    67,   206,     0,     0,     0,     0,     0,     0,     0,
      74,    75,    76,    77,    78,   207,     0,    80,     0,     0,
      82,     0,     0,     0,    86,     0,     0,    88,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,    97,     0,     0,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,   104,   200,
       0,     0,    22,     0,   208,     0,   108,     0,   209,     0,
      29,     0,   201,     0,     0,     0,    33,   202,   203,     0,
       0,   204,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   206,     0,     0,     0,     0,
       0,     0,     0,    74,    75,    76,    77,    78,   207,     0,
      80,     0,     0,    82,     0,   721,     0,    86,     0,     0,
      88,     0,     0,    90,   659,     0,     0,     0,     0,  1046,
    1047,   661,  1048,     0,     0,     0,     0,  1049,     0,     0,
       0,     0,     0,     0,     0,   723,     0,     0,    95,     0,
      97,     0,     0,     0,     0,     0,     0,  1050,     0,     0,
       0,  1051,     0,     0,  1052,     0,   666,     0,     0,     0,
       0,   104,     0,     0,   724,     0,     0,     0,     0,   108,
       0,   209,  1053,     0,     0,     0,     0,     0,     0,  1054,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1055,
       0,     0,     0,     0,     0,     0,     0,     0,  1056,  1057,
       0,  1058,     0,  1059,     0,   725,     0,     0,     0,     0,
       0,   726,  1060,     0,     0,  1061,     0,     0,  1062,     0,
       0,     0,     0,     0,     0,     0,     0,   536,   470,   471,
     472,   473,   474,     0,     0,   477,   478,   479,   480,     0,
     482,   483,     0,   674,   675,   676,   677,   678,   679,     0,
     680,     0,     0,     0,   681,   682,   683,   684,   685,   686,
     687,   688,   689,   690,   237,   691,     0,     0,   692,   693,
     694,   695,  1063,   659,     0,     0,     0,     0,   238,   239,
     661,   240,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,   242,     0,     0,     0,     0,     0,
     243,     0,     0,     0,     0,     0,   244,     0,     0,     0,
     245,     0,     0,   246,     0,   666,     0,     0,     0,     0,
       0,     0,     0,   247,     0,     0,     0,     0,     0,     0,
     248,   249,     0,     0,     0,     0,     0,     0,   250,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   251,     0,
       0,     0,     0,     0,   439,     0,     0,   252,   253,     0,
     254,   440,   255,     0,   256,     0,     0,   257,     0,     0,
       0,   258,     0,   441,   259,     0,     0,   260,     0,     0,
       0,     0,     0,     0,     0,     0,   536,   470,   471,   472,
     473,   474,     0,     0,   477,   478,   479,   480,     0,   482,
     483,     0,   674,   675,   676,   677,   678,   679,     0,   680,
       0,     0,     0,   681,   682,   683,   684,   685,   686,   687,
     688,   689,   690,     0,   691,     0,     0,   692,   693,   694,
     695,   442,     0,     0,   439,   443,     0,     0,     0,     0,
       0,   440,     0,     0,   535,     0,     0,     0,     0,     0,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   444,   536,   470,   471,
     472,   473,   474,     0,     0,   477,   478,   479,   480,     0,
     482,   483,     0,   445,   446,     0,   447,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,   465,   466,
     467,   442,   384,     0,   439,   443,     0,     0,     0,   468,
       0,   440,     0,     0,  1118,     0,     0,     0,     0,     0,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   444,   536,   470,   471,
     472,   473,   474,     0,     0,   477,   478,   479,   480,     0,
     482,   483,     0,   445,   446,     0,   447,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,   465,   466,
     467,   442,   384,     0,   439,   443,     0,     0,     0,   468,
       0,   440,     0,     0,  1125,     0,     0,     0,     0,     0,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   444,   536,   470,   471,
     472,   473,   474,     0,     0,   477,   478,   479,   480,     0,
     482,   483,     0,   445,   446,     0,   447,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,   465,   466,
     467,   442,   384,     0,   439,   443,     0,     0,     0,   468,
       0,   440,     0,     0,  1273,     0,     0,     0,     0,     0,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   444,   536,   470,   471,
     472,   473,   474,     0,     0,   477,   478,   479,   480,     0,
     482,   483,     0,   445,   446,     0,   447,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,   465,   466,
     467,   442,   384,     0,   439,   443,     0,     0,     0,   468,
       0,   440,     0,     0,  1274,     0,     0,     0,     0,     0,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   444,   536,   470,   471,
     472,   473,   474,     0,     0,   477,   478,   479,   480,     0,
     482,   483,     0,   445,   446,     0,   447,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,   465,   466,
     467,   442,   384,     0,   439,   443,     0,     0,     0,   468,
       0,   440,     0,     0,  1275,     0,     0,     0,     0,     0,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   444,   536,   470,   471,
     472,   473,   474,     0,     0,   477,   478,   479,   480,     0,
     482,   483,     0,   445,   446,     0,   447,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,   465,   466,
     467,   442,   384,     0,   439,   443,     0,     0,     0,   468,
       0,   440,     0,     0,  1276,     0,     0,     0,     0,     0,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   444,   536,   470,   471,
     472,   473,   474,     0,     0,   477,   478,   479,   480,     0,
     482,   483,     0,   445,   446,     0,   447,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,   465,   466,
     467,   442,   384,     0,   439,   443,     0,     0,     0,   468,
       0,   440,     0,     0,  1281,     0,     0,     0,     0,     0,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   444,   536,   470,   471,
     472,   473,   474,     0,     0,   477,   478,   479,   480,     0,
     482,   483,     0,   445,   446,     0,   447,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,   465,   466,
     467,   442,   384,     0,     0,   443,     0,     0,     0,   468,
       0,     0,     0,     0,  1282,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,     0,     0,     0,     0,     0,
       0,   440,     0,     0,     0,     0,   444,   536,   470,   471,
     472,   473,   474,   441,   288,   477,   478,   479,   480,     0,
     482,   483,     0,   445,   446,     0,   447,   448,   449,   528,
     450,   451,   452,     0,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,   465,   466,
     467,     0,   384,     0,     0,     0,     0,     0,     0,   468,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     439,   442,     0,     0,     0,   443,     0,   440,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
     288,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   519,     0,     0,  1039,   444,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   445,   446,     0,   447,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   439,   442,   465,   466,
     467,   443,   384,   440,     0,     0,     0,     0,     0,   468,
       0,     0,     0,     0,     0,   441,   516,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   519,     0,
       0,   517,   444,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   445,
     446,     0,   447,   448,   449,     0,   450,   451,   452,     0,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,     0,   442,   465,   466,   467,   443,   384,     0,
       0,     0,     0,     0,     0,   468,     0,     0,   439,     0,
       0,     0,     0,     0,     0,   440,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   441,   444,     0,
       0,     0,   630,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   445,   446,     0,   447,   448,
     449,     0,   450,   451,   452,     0,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   631,     0,
     465,   466,   467,     0,   384,     0,     0,     0,     0,     0,
       0,   468,     0,     0,     0,   442,     0,     0,     0,   443,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     439,   879,     0,     0,     0,     0,     0,   440,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
     444,     0,     0,     0,   880,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   445,   446,     0,
     447,   448,   449,     0,   450,   451,   452,     0,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     881,   632,   465,   466,   467,     0,     0,     0,     0,     0,
       0,     0,     0,   468,     0,     0,   439,   442,     0,     0,
       0,   443,     0,   440,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   441,   984,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   444,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   445,
     446,     0,   447,   448,   449,     0,   450,   451,   452,     0,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   439,   442,   465,   466,   467,   443,     0,   440,
       0,     0,     0,     0,     0,   468,     0,     0,     0,     0,
       0,   441,   288,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   519,     0,     0,     0,   444,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   445,   446,     0,   447,   448,
     449,     0,   450,   451,   452,     0,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   439,   442,
     465,   466,   467,   443,   384,   440,     0,     0,     0,     0,
       0,   468,     0,     0,     0,     0,     0,   441,   288,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     519,     0,     0,  1038,   444,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   445,   446,     0,   447,   448,   449,     0,   450,   451,
     452,     0,   453,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   439,   442,   465,   466,   467,   443,
     384,   440,     0,     0,     0,     0,     0,   468,     0,     0,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   500,
     444,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   445,   446,     0,
     447,   448,   449,     0,   450,   451,   452,     0,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     439,   442,   465,   466,   467,   443,   384,   440,     0,     0,
       0,     0,     0,   468,     0,     0,     0,     0,     0,   441,
     288,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   220,     0,     0,     0,   444,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   445,   446,     0,   447,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   439,   442,   465,   466,
     467,   443,     0,   440,     0,     0,     0,     0,     0,   468,
       0,     0,     0,     0,     0,   441,   757,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   758,   444,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   445,
     446,     0,   447,   448,   449,     0,   450,   451,   452,     0,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   439,   442,   465,   466,   467,   443,   384,   440,
       0,     0,     0,     0,     0,   468,     0,     0,     0,     0,
       0,   441,   759,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   760,   444,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   445,   446,     0,   447,   448,
     449,     0,   450,   451,   452,     0,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   439,   442,
     465,   466,   467,   443,     0,   440,     0,     0,     0,     0,
       0,   468,     0,     0,     0,     0,     0,   441,   976,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   444,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   445,   446,     0,   447,   448,   449,     0,   450,   451,
     452,     0,   453,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   439,   442,   465,   466,   467,   443,
       0,   440,     0,     0,     0,     0,     0,   468,     0,     0,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   445,   446,     0,
     447,   448,   449,     0,   450,   451,   452,     0,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     439,   442,   465,   466,   467,   443,   384,   440,     0,     0,
       0,     0,     0,   468,   998,     0,     0,     0,     0,   441,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   445,   446,     0,   447,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,   442,   465,   466,
     467,   443,   384,     0,     0,     0,     0,     0,     0,   468,
       0,     0,   439,     0,     0,     0,     0,     0,     0,   440,
       0,     0,     0,     0,     0,     0,     0,     0,   519,     0,
       0,   441,   444,     0,     0,     0,  1178,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   445,
     446,     0,   447,   448,   449,     0,   450,   451,   452,     0,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,  1179,     0,   465,   466,   467,     0,     0,     0,
       0,     0,     0,  1034,     0,   468,     0,     0,   439,   442,
       0,     0,     0,   443,     0,   440,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   441,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   444,     0,     0,     0,   439,   559,
       0,     0,     0,     0,     0,   440,     0,     0,     0,     0,
       0,   445,   446,     0,   447,   448,   449,   441,   450,   451,
     452,     0,   453,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,     0,   442,   465,   466,   467,   443,
       0,     0,     0,   439,     0,     0,     0,   468,     0,     0,
     440,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   441,     0,     0,     0,   519,     0,     0,     0,
     444,     0,     0,     0,     0,   442,     0,     0,     0,   443,
       0,     0,     0,     0,     0,     0,     0,   445,   446,     0,
     447,   448,   449,     0,   450,   451,   452,     0,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     444,     0,   465,   466,   467,     0,     0,     0,     0,     0,
     442,  1238,     0,   468,   443,     0,     0,   445,   446,     0,
     447,   448,   449,     0,   450,   451,   452,     0,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     439,     0,   465,   466,   467,   444,     0,   440,     0,     0,
       0,     0,     0,   468,     0,     0,     0,     0,     0,   441,
       0,     0,   445,   446,     0,   447,   448,   449,     0,   450,
     451,   452,     0,   453,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,     0,   439,   465,   466,   467,
       0,   583,     0,   440,     0,     0,     0,     0,   468,     0,
       0,     0,     0,     0,     0,   441,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   442,     0,     0,
       0,   443,     0,     0,     0,     0,     0,   439,     0,     0,
       0,     0,     0,     0,   440,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   441,     0,     0,     0,
       0,     0,   444,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,   443,     0,   445,
     446,     0,   447,   448,   449,     0,   450,   451,   452,     0,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,     0,   587,   465,   466,   467,   610,   444,     0,
       0,     0,     0,     0,   442,   468,     0,     0,   443,     0,
       0,     0,     0,     0,     0,   445,   446,   761,   447,   448,
     449,     0,   450,   451,   452,     0,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,     0,   444,
     465,   466,   467,   439,     0,     0,     0,     0,     0,     0,
     440,   468,     0,     0,     0,     0,   445,   446,     0,   447,
     448,   449,   441,   450,   451,   452,     0,   453,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,     0,
       0,   465,   466,   467,   439,   875,     0,     0,     0,     0,
       0,   440,   468,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     442,     0,     0,     0,   443,     0,     0,     0,     0,   439,
       0,     0,     0,     0,     0,     0,   440,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   441,     0,
       0,     0,     0,     0,     0,   444,     0,     0,     0,     0,
       0,   442,     0,     0,     0,   443,     0,     0,     0,     0,
       0,     0,   445,   446,     0,   447,   448,   449,     0,   450,
     451,   452,     0,   453,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,     0,   444,   465,   466,   467,
       0,     0,     0,     0,   847,     0,   442,     0,   468,     0,
     443,     0,     0,   445,   446,     0,   447,   448,   449,     0,
     450,   876,   452,     0,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   439,   220,   465,   466,
     467,   444,     0,   440,     0,     0,     0,     0,     0,   468,
       0,     0,     0,     0,     0,   441,     0,     0,   445,   446,
       0,   447,   448,   449,     0,   450,   451,   452,     0,   453,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   439,     0,   465,   466,   467,     0,     0,   440,     0,
       0,     0,     0,     0,   468,     0,     0,     0,     0,     0,
     441,     0,     0,   996,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,   443,     0,     0,
       0,   439,     0,     0,     0,     0,     0,     0,   440,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     441,     0,     0,     0,     0,     0,     0,     0,   444,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   442,     0,
       0,     0,   443,     0,     0,   445,   446,     0,   447,   448,
     449,     0,   450,   451,   452,     0,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,     0,   968,
     465,   466,   467,   444,     0,     0,     0,   439,   442,     0,
       0,   468,   443,     0,   440,     0,     0,     0,     0,     0,
     445,   446,     0,   447,   448,   449,   441,   450,   451,   452,
       0,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   444,     0,   465,   466,   467,     0,     0,
       0,     0,     0,     0,     0,     0,   468,     0,     0,     0,
     445,   446,     0,   447,   448,   449,     0,   450,   451,   452,
       0,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   439,   442,   465,   466,   467,   443,     0,
     440,     0,  1010,     0,     0,     0,   468,     0,     0,     0,
       0,     0,   441,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   444,
       0,     0,     0,   439,  1073,     0,     0,     0,     0,     0,
     440,     0,     0,     0,     0,     0,   445,   446,     0,   447,
     448,   449,   441,   450,   451,   452,     0,   453,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,     0,
     442,   465,   466,   467,   443,     0,     0,     0,  1011,   439,
       0,     0,   468,     0,     0,     0,   440,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   441,     0,
       0,     0,     0,     0,  1037,   444,     0,     0,     0,     0,
     442,     0,     0,     0,   443,     0,     0,     0,     0,     0,
       0,     0,   445,   446,     0,   447,   448,   449,     0,   450,
     451,   452,     0,   453,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   444,     0,   465,   466,   467,
       0,     0,     0,     0,     0,     0,   442,     0,   468,     0,
     443,     0,   445,   446,     0,   447,   448,   449,     0,   450,
     451,   452,     0,   453,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   439,     0,   465,   466,   467,
       0,   444,   440,     0,     0,     0,     0,     0,   468,     0,
       0,     0,     0,     0,   441,  1103,     0,     0,   445,   446,
       0,   447,   448,   449,     0,   450,   451,   452,     0,   453,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   439,     0,   465,   466,   467,     0,     0,   440,     0,
    1096,     0,     0,     0,   468,     0,     0,     0,     0,     0,
     441,  1106,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   442,     0,     0,     0,   443,     0,     0,     0,
       0,   439,     0,     0,     0,     0,     0,     0,   440,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     441,     0,     0,     0,     0,     0,     0,   444,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   442,     0,
       0,     0,   443,     0,   445,   446,     0,   447,   448,   449,
       0,   450,   451,   452,     0,   453,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,     0,     0,   465,
     466,   467,     0,   444,     0,     0,     0,     0,   442,     0,
     468,   439,   443,     0,     0,     0,     0,     0,   440,     0,
     445,   446,     0,   447,   448,   449,     0,   450,   451,   452,
     441,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   444,     0,   465,   466,   467,     0,     0,
       0,   439,  1175,     0,     0,     0,   468,     0,   440,     0,
     445,   446,     0,   447,   448,   449,     0,   450,   451,   452,
     441,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,     0,     0,   465,   466,   467,   442,     0,
       0,     0,   443,     0,  1109,     0,   468,     0,     0,     0,
       0,  1206,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   444,     0,     0,     0,   439,   442,     0,
       0,     0,   443,     0,   440,     0,     0,     0,     0,     0,
     445,   446,     0,   447,   448,   449,   441,   450,   451,   452,
       0,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   444,     0,   465,   466,   467,     0,     0,
       0,     0,     0,     0,     0,     0,   468,     0,     0,     0,
     445,   446,     0,   447,   448,   449,     0,   450,   451,   452,
       0,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,     0,   442,   465,   466,   467,   443,     0,
       0,     0,   439,  1246,     0,     0,   468,  1209,     0,   440,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   441,     0,     0,     0,     0,     0,     0,     0,   444,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   445,   446,     0,   447,
     448,   449,     0,   450,   451,   452,     0,   453,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,     0,
       0,   465,   466,   467,     0,     0,     0,     0,   439,   442,
       0,     0,   468,   443,     0,   440,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   441,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   444,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1260,     0,     0,     0,
       0,   445,   446,     0,   447,   448,   449,     0,   450,   451,
     452,     0,   453,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   439,   442,   465,   466,   467,   443,
       0,   440,     0,     0,     0,     0,     0,   468,     0,     0,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   445,   446,     0,
     447,   448,   449,     0,   450,   451,   452,     0,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     439,   442,   465,   466,   467,   443,     0,   440,     0,     0,
       0,     0,     0,   468,  1299,     0,     0,     0,     0,   441,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   445,   446,     0,   447,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   439,   442,   465,   466,
     467,   443,     0,   440,     0,     0,     0,     0,     0,   468,
    1300,     0,     0,     0,     0,   441,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   444,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   445,
     446,     0,   447,   448,   449,     0,   450,   451,   452,     0,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   439,   442,   465,   466,   467,   443,     0,   440,
       0,     0,     0,     0,     0,   468,  1301,     0,     0,     0,
       0,   441,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   444,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   445,   446,     0,   447,   448,
     449,     0,   450,   451,   452,     0,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   439,   442,
     465,   466,   467,   443,     0,   440,     0,     0,     0,     0,
       0,   468,  1302,     0,     0,     0,     0,   441,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   444,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   445,   446,     0,   447,   448,   449,     0,   450,   451,
     452,     0,   453,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   439,   442,   465,   466,   467,   443,
       0,   440,     0,     0,     0,     0,     0,   468,  1303,     0,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,   439,     0,     0,     0,     0,     0,
       0,   440,     0,     0,     0,     0,     0,   445,   446,     0,
     447,   448,   449,   441,   450,   451,   452,     0,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
       0,   442,   465,   466,   467,   443,     0,     0,     0,   439,
       0,     0,     0,   468,  1304,     0,   440,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   441,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,   442,     0,     0,     0,   443,     0,     0,     0,     0,
       0,     0,     0,   445,   446,     0,   447,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   444,     0,   465,   466,
     467,     0,     0,     0,     0,     0,   963,     0,     0,   468,
     443,     0,     0,   445,   446,     0,   447,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,   465,   466,
     467,   444,     0,     0,     0,     0,     0,     0,     0,   468,
       0,     0,     0,     0,     0,     0,     0,     0,   445,   446,
       0,   447,   448,   449,     0,   450,   451,   452,     0,   453,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,     0,   380,   465,   466,   467,    23,    24,     0,     0,
       0,     0,     0,     0,   468,   381,     0,    31,   382,     0,
     380,     0,     0,    37,    23,    24,     0,     0,     0,     0,
      42,     0,     0,   381,     0,    31,   382,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,     0,    42,     0,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,  1081,     0,     0,  1082,     0,     0,   383,     0,
      69,     0,     0,     0,    58,     0,    60,     0,    62,     0,
    1081,     0,     0,  1082,     0,     0,   383,     0,    69,    85,
       0,     0,    87,     0,     0,    89,   380,     0,     0,     0,
      23,    24,     0,     0,     0,     0,     0,    85,     0,   381,
      87,    31,   382,    89,   380,     0,     0,    37,    23,    24,
      94,     0,     0,     0,    42,     0,     0,   381,     0,    31,
     382,     0,     0,     0,     0,    37,     0,     0,    94,     0,
       0,     0,    42,     0,     0,   105,     0,     0,     0,     0,
      58,  1083,    60,     0,    62,     0,  1081,     0,     0,  1082,
       0,     0,   383,   105,    69,     0,     0,     0,    58,  1186,
      60,     0,   378,     0,  1081,     0,     0,  1082,     0,     0,
     383,     0,    69,    85,     0,     0,    87,   380,     0,    89,
       0,    23,    24,     0,     0,     0,     0,     0,     0,     0,
     381,    85,    31,   382,    87,     0,     0,    89,    37,     0,
       0,     0,     0,     0,    94,    42,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,   239,    94,   240,     0,     0,     0,     0,   241,   105,
       0,    58,     0,    60,     0,  1247,   242,  1081,     0,     0,
    1082,     0,   243,   383,     0,    69,     0,   105,   244,     0,
       0,     0,   245,     0,     0,   246,     0,     0,     0,     0,
       0,     0,     0,     0,    85,   247,     0,    87,     0,     0,
      89,     0,   248,   249,     0,     0,     0,     0,     0,     0,
     250,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     251,     0,     0,   237,     0,    94,     0,     0,     0,   252,
     253,     0,   254,     0,   255,     0,   256,   238,   239,   257,
     240,     0,     0,   258,     0,   241,   259,    24,     0,   260,
     105,     0,     0,   242,     0,     0,     0,     0,     0,   243,
       0,     0,     0,     0,     0,   244,     0,     0,     0,   245,
       0,     0,   246,    94,     0,     0,     0,     0,   237,     0,
       0,     0,   247,     0,     0,     0,     0,     0,     0,   248,
     249,     0,   238,   239,     0,   240,     0,   250,    60,     0,
     241,     0,     0,     0,     0,     0,     0,   251,   242,     0,
      69,     0,     0,     0,   243,     0,   252,   253,     0,   254,
     244,   255,     0,   256,   245,     0,   257,   246,     0,     0,
     258,     0,     0,   259,     0,    89,   260,   247,     0,     0,
       0,     0,     0,     0,   248,   249,     0,     0,     0,     0,
       0,     0,   250,  -321,     0,     0,     0,     0,     0,     0,
       0,     0,   251,     0,     0,     0,     0,  -321,  -321,     0,
    -321,   252,   253,     0,   254,  -321,   255,     0,   256,     0,
       0,   257,     0,  -321,     0,   258,   572,     0,   259,  -321,
       0,   260,     0,     0,     0,  -321,     0,     0,     0,  -321,
       0,     0,  -321,     0,     0,     0,     0,     0,   237,     0,
       0,     0,  -321,     0,     0,     0,     0,     0,     0,  -321,
    -321,     0,   238,   239,     0,   240,     0,  -321,     0,     0,
     241,     0,     0,     0,     0,     0,     0,  -321,   242,     0,
       0,     0,     0,     0,   243,     0,  -321,  -321,     0,  -321,
     244,  -321,     0,  -321,   245,     0,  -321,   246,     0,     0,
    -321,     0,     0,  -321,     0,     0,  -321,   247,     0,     0,
       0,     0,     0,     0,   248,   249,     0,     0,     0,     0,
       0,     0,   250,  -322,     0,     0,     0,     0,     0,     0,
       0,     0,   251,     0,     0,     0,     0,  -322,  -322,     0,
    -322,   252,   253,     0,   254,  -322,   255,     0,   256,     0,
       0,   257,     0,  -322,     0,   258,     0,     0,   259,  -322,
       0,   260,     0,     0,     0,  -322,     0,     0,     0,  -322,
       0,     0,  -322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -322,     0,     0,     0,     0,     0,     0,  -322,
    -322,     0,     0,     0,     0,     0,     0,  -322,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -322,     0,     0,
       0,     0,     0,     0,     0,     0,  -322,  -322,     0,  -322,
       0,  -322,     0,  -322,     0,     0,  -322,     0,     0,     0,
    -322,     0,     0,  -322,     0,     0,  -322
};

static const yytype_int16 yycheck[] =
{
      13,    20,   118,    66,    17,     2,   170,   581,    21,   280,
     157,    27,   496,    66,    27,   411,    66,   229,   541,   139,
     790,    34,    35,    36,   881,    38,   431,   630,   653,   585,
     236,   836,   576,   304,   905,   403,    49,   650,    51,    52,
     830,   876,     1,   899,    57,    47,    59,    32,    61,   416,
      55,   186,     1,    66,    70,    55,    47,    70,    71,    72,
      73,    74,    75,   619,     1,    47,    79,   911,    81,     3,
       8,    27,  1202,   344,   280,    88,    40,     0,    91,    92,
      32,   699,    95,   701,    97,   551,    60,   100,   101,    46,
     103,    24,    71,   109,   107,   108,   109,   110,   304,    32,
       3,    27,    32,   123,    32,    32,   120,    40,    41,    42,
      47,    32,    47,    92,    32,    74,  1012,    32,   120,   546,
    1250,    47,    78,   147,   137,    51,   139,    84,   165,   108,
     269,   118,   271,   272,    90,   120,   147,     3,   344,    47,
       1,   276,    83,    84,   118,   147,   573,     1,    47,    75,
     170,   175,    78,   117,    80,    90,   147,   114,   117,   115,
     147,   175,   173,   147,    90,   147,   179,   172,   120,   103,
    1014,    32,   172,   175,    92,    55,   125,    92,    32,   949,
      22,   165,    90,   109,   175,   170,   119,   653,   125,   202,
     203,   204,     3,   209,   207,   208,   209,   146,   147,    60,
     103,  1006,    55,   570,   817,  1076,    60,  1012,   227,   146,
     147,   149,   171,  1084,  1085,   486,   487,   147,   170,  1075,
     147,   120,    36,   236,   173,  1015,    92,   783,   165,    74,
      18,    92,   102,    53,   373,   374,  1071,   103,    92,   762,
     170,   174,   170,   170,   105,   451,    88,   173,   147,   170,
     165,   105,   170,    73,  1150,   170,     3,   118,    72,   147,
      27,   147,    89,    83,   118,   633,    93,   280,   120,   120,
     112,    31,   117,    20,   147,    46,   175,   880,   305,   306,
     486,   487,   402,   171,   120,   146,    33,   173,    31,    77,
     834,   304,   146,   141,    65,   411,   431,   147,   171,    55,
     170,   149,   315,   316,   165,   140,   790,  1112,    96,   170,
      27,    78,   147,    84,  1185,    62,   170,   330,   331,  1190,
    1177,   171,  1179,    90,   175,   113,   171,    27,   147,    89,
      47,   344,    46,    93,    51,   541,   171,   165,   502,   175,
     147,   147,   109,   114,   172,  1150,    89,   559,   147,   962,
      93,    65,   415,  1123,   173,   368,   103,   147,    75,   372,
      89,    78,   109,    80,    93,    55,   173,   173,   365,   147,
      84,   147,   109,    90,   173,    75,   125,    90,    78,   585,
     903,   141,   395,   173,   397,   147,   146,   172,  1013,   149,
      90,   407,   109,   153,   407,   173,   147,   173,   141,    11,
     114,   147,   151,   146,    89,   148,   149,   165,    93,   109,
     153,   173,   147,   619,   172,   165,   174,   160,   172,   165,
     174,   171,   173,   946,   636,   168,   439,   440,   441,   442,
     443,   444,   147,   446,   447,   448,   449,   450,   173,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   173,    55,   173,   147,
    1063,   165,   475,   476,   147,   949,   147,   171,   481,   147,
     147,   484,   485,   486,   487,   488,   489,    55,   172,  1097,
    1098,   147,   147,   496,   165,   173,  1042,   500,   165,    55,
     173,   504,  1196,   506,   147,   173,   475,   476,   514,    55,
     165,   514,   481,   516,   517,   484,   485,   173,   571,   522,
     523,   907,   165,   147,   147,   528,   147,   922,   571,   147,
      55,   571,   535,   147,   147,   538,    89,   147,   541,     8,
      93,   165,   165,     2,   165,   125,    55,   165,   551,    89,
     553,   165,   165,    93,   557,   165,    82,   560,   561,    85,
    1254,  1255,  1256,  1257,  1258,  1259,   762,    26,   571,    28,
      27,   172,   152,   576,   154,   155,   141,   157,   158,    55,
     100,   146,   585,  1176,   149,  1178,   172,   783,   153,    55,
      47,    89,    55,   596,    51,    93,   599,   172,   601,   174,
      55,    55,    31,   606,   607,   608,    55,   610,   611,     3,
     616,   614,   615,   616,    55,   170,   619,     2,    75,   140,
      79,    78,   146,    80,    83,    84,   165,   630,   631,   172,
     599,   174,   601,    90,   172,   172,   623,   174,  1202,    55,
     643,    26,   170,    28,   172,   172,   164,   174,   107,  1123,
     653,    50,   109,   112,    53,   172,   172,   174,   174,   599,
      89,   601,   165,   165,    93,   794,   795,   172,   165,   798,
     799,   165,   165,   165,    73,   165,   119,    76,   165,   875,
     876,   170,   170,    84,    83,   147,  1250,   172,   172,   172,
     172,    31,   173,   696,    79,   146,     8,   165,    83,    84,
     703,   172,  1088,     3,   165,   172,    65,   903,   125,    90,
     109,   171,   141,   142,   170,   144,   145,   146,    90,   148,
     149,   150,   107,   125,   153,   854,   855,   112,   149,    68,
     159,   160,    31,   172,   163,   125,   865,   166,   167,   168,
     173,   147,    47,   125,   172,   125,    34,   901,   177,    89,
     946,    34,    65,    93,   757,   758,   759,   760,   761,   762,
     763,   764,   221,   173,   125,   173,   775,   776,   905,   146,
     173,   908,    21,   149,   170,   175,   147,   175,   781,    31,
     783,   784,   105,   786,   172,   149,   149,   790,   175,   792,
      89,   149,   149,   149,    93,    31,   149,   922,   801,   802,
     149,   141,   142,   149,   144,   145,   146,   149,   148,   149,
     141,   149,   781,   153,   149,   149,   149,   820,   165,   159,
     160,   149,   165,   163,   283,   173,   166,   167,   168,   288,
      34,   834,    34,   165,   171,   838,   221,    89,   165,   165,
     165,    93,   141,   142,   146,   165,  1042,   146,   973,   148,
     149,   820,   905,    89,   153,   980,   909,    93,   172,   175,
     147,   160,   173,   170,   173,   918,    32,   166,   167,   168,
     873,    32,   175,   171,   877,    27,   879,   880,   881,    34,
     883,   165,   885,  1144,  1145,   888,   889,   165,   146,   141,
     142,   172,   144,   145,   146,    47,   148,   149,   283,    51,
     903,   153,   172,   288,   175,   141,   365,   175,   160,   165,
     146,   173,   148,   149,   166,   167,   168,   153,   147,    34,
     173,   165,   173,    75,   160,   384,    78,   173,    80,   164,
     166,   167,   168,   165,   937,   394,   164,   940,    90,  1076,
     165,   147,   117,   946,     2,  1042,   949,  1084,  1144,  1145,
     279,   873,   618,    25,   394,   386,    91,   109,  1002,   351,
     963,   964,  1027,   557,   571,   922,   918,   405,    26,   568,
      28,   974,   431,   976,   909,  1250,  1090,   318,   981,  1085,
     365,   984,  1088,  1260,  1253,   900,   137,   705,   554,   816,
      22,    -1,    -1,   996,    26,    27,    -1,    -1,    -1,   384,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,   394,
    1013,    43,    -1,    -1,  1017,    -1,    -1,  1023,    50,    -1,
    1023,    79,    -1,  1076,    -1,    83,    84,    -1,    -1,  1032,
      -1,  1034,    -1,    -1,  1037,  1038,  1039,  1040,    -1,  1042,
    1169,  1170,  1171,  1172,    76,    -1,    78,    -1,  1185,   107,
      -1,    -1,    -1,    -1,   112,    -1,    88,   516,    90,    -1,
    1063,    -1,    -1,   522,    -1,    -1,    -1,    -1,  1071,    -1,
    1073,    -1,    -1,    -1,   533,    -1,   535,   109,    -1,    -1,
     112,    -1,    -1,   115,  1190,    -1,    -1,     3,    -1,  1092,
      -1,    -1,  1217,  1218,    -1,    -1,  1221,  1222,    -1,  1102,
    1103,    17,  1105,  1106,    20,  1108,  1109,    -1,  1233,    25,
    1235,    -1,    -1,    -1,    -1,  1118,  1119,    -1,  1121,    35,
    1123,    -1,  1125,  1126,    -1,    -1,  1129,    -1,    -1,    45,
      31,   516,  1185,    -1,    -1,    -1,    52,   522,    -1,    -1,
      -1,  1144,  1145,    -1,    -1,   604,    62,    -1,   533,   608,
     535,    -1,    -1,  1156,    70,  1158,    -1,    -1,    -1,  1162,
      -1,    -1,    -1,   221,   623,    -1,    -1,    -1,    -1,    -1,
      -1,    87,  1175,  1176,  1177,  1178,  1179,    -1,    -1,    -1,
      35,    36,    -1,    99,    -1,    -1,    -1,   103,    89,    -1,
      -1,    -1,    93,    -1,   110,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,  1206,  1207,  1208,  1209,  1210,  1211,  1212,
    1213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   604,
      -1,    -1,    -1,   608,    -1,   283,    -1,    -1,    -1,    -1,
     288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   623,    -1,
     141,   142,    -1,  1246,   145,   146,    -1,   148,   149,  1252,
      -1,    -1,   153,    -1,   109,    -1,    -1,    -1,    -1,   160,
      -1,     2,    -1,    -1,    -1,   166,   167,   168,    -1,    -1,
    1273,  1274,  1275,  1276,  1261,    -1,    -1,    -1,  1281,  1282,
      -1,    -1,    -1,    -1,    -1,    26,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1299,  1300,  1301,  1302,
    1303,  1304,    -1,    -1,    -1,    -1,    -1,   365,    -1,    -1,
      -1,    -1,    -1,   772,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     2,   384,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   394,    -1,    79,    -1,
      -1,    -1,    83,    84,    -1,   804,    -1,    -1,   807,    26,
       2,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,    -1,    -1,   107,    -1,    -1,    -1,
      -1,   112,    -1,    -1,    26,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,
      -1,    -1,    -1,    -1,   853,    -1,    35,    -1,    37,    38,
     859,    -1,    79,    -1,    43,    -1,    83,    84,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,   804,
      -1,    -1,   807,    -1,    -1,    -1,    -1,    79,    -1,    -1,
     107,    83,    84,    -1,    73,   112,    -1,    76,    -1,    78,
      -1,    80,    -1,    82,    83,    -1,    85,    -1,    -1,    88,
      -1,    90,    -1,    -1,    -1,   107,    -1,    -1,   516,    -1,
     112,    -1,    -1,    -1,   522,    -1,    -1,    -1,   853,    -1,
     109,    -1,    -1,   112,   859,   533,   115,   535,    -1,    -1,
     221,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,    -1,
      -1,   140,    -1,    -1,   963,   964,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   974,    -1,   976,    -1,    -1,
      -1,    -1,   981,    -1,    -1,   984,   165,    -1,   169,    -1,
      -1,   172,   171,   174,   993,    -1,   995,    -1,    -1,   998,
      -1,    -1,   283,    -1,   221,    -1,   604,   288,    -1,    -1,
     608,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1022,    -1,   623,    -1,    -1,    -1,   221,
      -1,    -1,    -1,  1032,    -1,  1034,    -1,    -1,    -1,    -1,
      -1,    -1,  1041,    -1,    -1,    -1,    -1,   442,   443,   974,
      -1,   976,    -1,    -1,    -1,    -1,   981,    -1,    -1,   984,
      -1,    -1,    -1,    -1,    -1,    -1,   283,    -1,   993,    -1,
     995,   288,    -1,   998,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   365,    -1,    -1,    -1,    -1,    -1,
      -1,   283,    -1,   488,   489,    -1,   288,  1022,    -1,    -1,
      -1,    -1,    -1,   384,    -1,   500,    -1,  1032,    -1,  1034,
      -1,    -1,    -1,   394,    -1,    -1,  1041,    -1,    -1,  1118,
      -1,    -1,   517,    -1,    -1,    -1,  1125,    -1,   523,    -1,
      -1,    -1,    -1,   528,    -1,    -1,    -1,    -1,    -1,  1138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   365,    -1,
      -1,    -1,    -1,  1152,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1161,    -1,    -1,    -1,  1165,   384,    -1,    -1,
      -1,    -1,    -1,   365,    -1,    -1,    -1,   394,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   384,  1118,    -1,    -1,    -1,    -1,    -1,    -1,
    1125,    -1,   394,    -1,    -1,    -1,   804,    -1,    -1,   807,
      -1,   606,    -1,  1138,    -1,    -1,    -1,    -1,    -1,    -1,
     615,   616,    -1,    -1,  1223,    -1,    -1,  1152,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   516,  1161,    -1,  1237,  1238,
    1165,   522,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   533,    -1,   535,   853,    -1,    -1,    -1,    -1,
      -1,   859,  1261,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1273,  1274,  1275,  1276,    -1,    -1,
      -1,    -1,  1281,  1282,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1223,   516,
      -1,    -1,    -1,    -1,    -1,   522,    -1,    -1,    -1,    -1,
      -1,    -1,  1237,  1238,    -1,    -1,   533,    -1,   535,    -1,
      -1,    -1,    -1,   604,   516,    -1,    -1,   608,    -1,    -1,
     522,    -1,    -1,    -1,    -1,    -1,  1261,    -1,    -1,    -1,
      -1,   533,   623,   535,    -1,     2,    -1,    -1,  1273,  1274,
    1275,  1276,    -1,    -1,    -1,    -1,  1281,  1282,    -1,    -1,
      -1,    -1,    -1,   758,    -1,   760,    -1,    -1,   763,    26,
      -1,    28,    -1,    -1,    -1,    -1,   974,    -1,   976,     3,
      -1,    -1,    -1,   981,    -1,    -1,   984,   604,    -1,    -1,
      -1,   608,    -1,    17,    18,   993,    20,   995,    -1,    -1,
     998,    25,    -1,    -1,    -1,    -1,   623,    -1,    -1,    33,
      -1,    -1,   604,    -1,    -1,    39,   608,    -1,    -1,    -1,
      -1,    45,    79,    -1,  1022,    49,    83,    84,    52,    -1,
      -1,   623,    -1,    -1,  1032,    -1,  1034,    -1,    62,    -1,
      -1,    -1,    -1,  1041,    -1,    69,    70,    -1,    -1,    -1,
     107,    -1,    -1,    77,    -1,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,
      -1,    31,   106,    -1,    -1,    -1,   110,   111,    -1,   113,
      22,    -1,   116,    -1,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,
    1118,    53,    -1,   804,    -1,    -1,   807,  1125,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,    -1,    89,
    1138,    73,    -1,    93,    76,    -1,    78,     2,   172,    -1,
      82,    83,    84,    85,  1152,    -1,    88,    -1,    90,    -1,
      -1,    -1,    -1,  1161,   221,    -1,    -1,  1165,   963,   964,
      -1,    26,   853,    28,   124,    -1,    -1,   109,   859,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,   804,    -1,    -1,
     807,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   804,   163,    -1,   807,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    79,  1223,   283,   177,    83,    84,
      -1,   288,    -1,   165,    -1,    -1,   853,    -1,   170,  1237,
    1238,    -1,   859,  1038,  1039,  1040,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      -1,   853,    -1,  1261,    -1,    -1,    -1,   859,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1273,  1274,  1275,  1276,    -1,
      -1,    -1,    -1,  1281,  1282,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   974,    -1,   976,    -1,    -1,    -1,    -1,
     981,    -1,    -1,   984,    -1,    -1,    -1,    -1,   365,    -1,
      -1,    -1,   993,    -1,   995,    -1,    -1,   998,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   384,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,    -1,    -1,
      -1,  1022,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1032,    -1,  1034,    -1,    -1,    -1,   974,    -1,   976,
    1041,    -1,    -1,    -1,   981,    -1,   221,   984,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   993,    -1,   995,    -1,
      -1,   998,   974,    -1,   976,     2,    -1,    -1,    -1,   981,
      -1,    -1,   984,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   993,    -1,   995,    -1,  1022,   998,    -1,    -1,    26,
      -1,    28,    -1,    -1,    -1,  1032,    -1,  1034,    -1,    -1,
      -1,    -1,    -1,    -1,  1041,    -1,    -1,    -1,   283,    -1,
    1022,    -1,    -1,   288,    -1,    -1,    -1,  1118,    -1,    -1,
    1032,    -1,  1034,    -1,  1125,    -1,    -1,    -1,    -1,  1041,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1138,    -1,   516,
      -1,    -1,    79,    -1,    -1,   522,    -1,    -1,    -1,    -1,
      -1,  1152,    -1,    -1,    -1,    -1,   533,    -1,   535,    -1,
    1161,    -1,    -1,    -1,  1165,    -1,    -1,    -1,    -1,    -1,
     107,    -1,    -1,    -1,    -1,   112,    -1,    71,    -1,    -1,
      -1,  1118,    -1,    -1,    -1,    -1,    -1,    -1,  1125,    -1,
     365,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,
      -1,  1138,    -1,    -1,    -1,    -1,  1118,    -1,    -1,   384,
      -1,    -1,    -1,  1125,   108,  1152,    -1,    -1,    -1,   394,
      -1,    -1,  1223,    -1,  1161,    -1,  1138,   604,  1165,    -1,
      -1,   608,    -1,    -1,    -1,    -1,  1237,  1238,    -1,    -1,
    1152,    -1,    -1,    -1,    -1,    -1,   623,    -1,    -1,  1161,
      -1,    -1,    -1,  1165,    -1,    -1,    -1,    -1,    -1,    -1,
    1261,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1273,  1274,  1275,  1276,    -1,    -1,    -1,    -1,
    1281,  1282,    -1,    -1,   221,    -1,  1223,    -1,     2,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1237,  1238,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1223,    26,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1261,  1237,  1238,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1273,  1274,  1275,  1276,
      -1,   516,   236,    -1,  1281,  1282,   283,   522,    -1,  1261,
      -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,   533,    -1,
     535,  1273,  1274,  1275,  1276,    79,    -1,    -1,    -1,  1281,
    1282,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,   280,    -1,    -1,    -1,
      -1,    -1,    19,   107,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
     304,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    -1,    -1,    -1,    -1,    -1,    -1,   804,   365,   604,
     807,    -1,    -1,   608,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   384,   623,    -1,
     344,    -1,    -1,    -1,    -1,    -1,    -1,   394,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   853,    49,    -1,    -1,
      -1,    -1,   859,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    73,    -1,    -1,    -1,    -1,    -1,   221,    -1,    -1,
      -1,    83,    84,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    -1,   170,    -1,   117,    -1,    -1,    -1,    49,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   283,
      -1,    -1,    -1,    73,   288,    -1,    -1,    -1,    -1,   516,
      -1,   475,   476,    -1,    -1,   522,    -1,   481,    -1,    -1,
     484,   485,   486,   487,    -1,    -1,   533,   974,   535,   976,
      -1,    -1,    -1,    -1,   981,    -1,    -1,   984,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   993,    -1,   995,    -1,
      -1,   998,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   804,
      -1,    -1,   807,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1022,    -1,   541,    -1,    -1,
      -1,   365,    -1,    -1,   226,  1032,    -1,  1034,    -1,    -1,
      -1,    -1,    -1,    -1,  1041,    -1,    -1,   604,    -1,    -1,
     384,   608,    -1,    -1,    -1,    -1,    -1,    -1,   853,    -1,
     394,    -1,    -1,    -1,   859,    -1,   623,    -1,    -1,    -1,
      -1,   585,    -1,    -1,    -1,   267,   268,   269,   270,   271,
     272,    -1,    -1,   275,   276,   599,    -1,   601,    -1,    -1,
     282,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,
      -1,   293,    -1,    -1,    -1,   619,    -1,    -1,   300,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1118,    -1,    -1,    -1,    -1,    -1,    -1,  1125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1138,    -1,    -1,    -1,    -1,    -1,   267,   340,   269,
     270,   271,   272,    -1,    -1,  1152,    -1,    -1,    -1,    -1,
      -1,   353,   354,    -1,  1161,    -1,    -1,    -1,  1165,    -1,
      -1,   291,    -1,   293,    -1,    -1,    -1,    -1,    -1,   974,
     300,   976,   516,    -1,    -1,    -1,   981,    -1,   522,   984,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   993,   533,
     995,    -1,    -1,   998,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     340,    -1,    -1,    -1,    -1,    -1,  1223,  1022,    -1,    -1,
      -1,    -1,    -1,   353,   354,    -1,    -1,  1032,    -1,  1034,
    1237,  1238,    -1,    -1,    -1,    -1,  1041,   804,   762,    -1,
     807,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,  1261,    -1,    -1,   781,    -1,   783,
     604,    -1,    17,    18,   608,    20,  1273,  1274,  1275,  1276,
      25,    -1,    -1,    -1,  1281,  1282,    -1,    -1,    33,   623,
      -1,    -1,    -1,    -1,    39,    -1,   853,    -1,    -1,    -1,
      45,    -1,   859,    -1,    49,    -1,   820,    52,    -1,   501,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,
      -1,    -1,    -1,  1118,    69,    70,    -1,    -1,    -1,   521,
    1125,    -1,    77,   525,    -1,   527,    -1,    -1,   530,    -1,
      -1,    -1,    87,  1138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,    -1,   101,  1152,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,  1161,    -1,   113,    -1,
    1165,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   501,    30,    -1,    -1,    -1,    -1,    -1,    -1,   903,
      -1,    -1,    -1,    -1,    -1,   140,    44,    45,    46,    -1,
      -1,   521,    -1,    -1,    -1,   525,    -1,   527,    -1,    -1,
     530,    -1,    -1,    -1,    -1,    -1,    -1,   974,    -1,   976,
      -1,    -1,    -1,    -1,   981,    -1,    -1,   984,  1223,    -1,
      -1,    -1,   946,    -1,    -1,    -1,   993,    -1,   995,    -1,
      -1,   998,  1237,  1238,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    18,    -1,    20,    -1,  1022,  1261,    -1,    25,    -1,
     804,    -1,    -1,   807,    -1,  1032,    33,  1034,  1273,  1274,
    1275,  1276,    39,    -1,  1041,    -1,  1281,  1282,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    -1,   155,    -1,   157,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,   853,
      77,   169,    -1,    -1,    -1,   859,    -1,    -1,  1042,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,   111,    -1,   113,   205,    -1,   116,
      -1,  1118,    -1,    -1,    -1,    -1,    -1,    -1,  1125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1152,   788,   789,    -1,    -1,
      -1,    -1,   794,   795,  1161,    -1,   798,   799,  1165,    -1,
      -1,    -1,    -1,    -1,    -1,   172,   808,   265,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   273,   274,    -1,    -1,    -1,
    1144,  1145,    -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,
     974,    -1,   976,    -1,    -1,    -1,    -1,   981,    -1,    -1,
     984,    -1,   844,    -1,    -1,    -1,    -1,    -1,    -1,   993,
      -1,   995,    -1,    -1,   998,    -1,  1223,    -1,   788,   789,
      -1,   863,   864,    -1,   794,   795,   324,    -1,   798,   799,
    1237,  1238,    -1,    -1,    -1,    -1,    -1,    -1,  1022,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1032,    -1,
    1034,    -1,    -1,    -1,  1261,    -1,    -1,  1041,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1273,  1274,  1275,  1276,
      -1,    -1,    -1,     3,  1281,  1282,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   381,    -1,    -1,    -1,    17,    18,    -1,
      20,    -1,    -1,    -1,   864,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,   406,    39,
      -1,    -1,    -1,   411,    -1,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,   965,   966,    -1,    -1,    -1,    -1,   971,
      -1,   973,    62,   431,    -1,    -1,    -1,    -1,   980,    69,
      70,   983,    -1,    -1,   986,    -1,   988,    77,   990,    -1,
      -1,    -1,    -1,    -1,  1138,    -1,    -1,    87,    -1,    -1,
      -1,  1003,    -1,    -1,    -1,    -1,    96,    97,  1152,    99,
      -1,   101,    -1,   103,    -1,    -1,   106,  1161,    -1,    -1,
     110,  1165,    -1,   113,    -1,    -1,   116,    -1,  1030,    -1,
      -1,    -1,     3,    -1,    -1,   965,   966,   495,    -1,    -1,
      -1,   971,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,
      -1,    -1,    -1,   983,    25,    -1,   986,    -1,   988,    -1,
     990,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    31,    -1,    49,  1223,
     170,    52,   172,    -1,   542,    -1,    -1,    -1,   546,    -1,
      -1,    62,    -1,  1237,  1238,    -1,    -1,    -1,    69,    70,
      -1,   559,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
     568,    -1,    -1,    -1,    -1,   573,    87,  1261,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,
     101,    -1,   103,    -1,    89,   106,    -1,    -1,    93,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,  1154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1167,  1168,  1169,  1170,   124,
      -1,   629,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   172,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    -1,  1217,  1218,    -1,    -1,  1221,
    1222,    -1,   177,    -1,  1154,    -1,    -1,    -1,    -1,  1231,
      -1,  1233,    -1,  1235,    -1,    -1,    -1,  1167,  1168,  1169,
    1170,    -1,    -1,    -1,    -1,    -1,    -1,   705,     1,    -1,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,   809,    -1,   106,    -1,    -1,    -1,   110,   816,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,   826,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   844,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   900,    -1,    -1,    -1,    -1,    -1,    -1,   907,
      -1,     0,     1,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,   922,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,  1002,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,  1027,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     1,
    1088,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      12,    -1,    14,    15,    16,    17,    18,    19,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,    -1,   170,   171,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,   171,   172,    -1,   174,    -1,     1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,   171,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,   171,   172,    -1,   174,    -1,     1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,   171,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    12,    -1,    14,    -1,    -1,
      17,    18,    19,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,    -1,   141,   142,   143,   144,   145,   146,
      -1,   148,    -1,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,   166,
     167,   168,   169,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    -1,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    -1,    -1,    -1,    -1,    30,    -1,
      32,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      42,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    61,
      62,    63,    64,    -1,    66,    67,    -1,    69,    70,    71,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,   119,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    26,
      27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      47,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,   173,   174,    -1,    -1,   177,     3,     4,     5,     6,
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
     107,   108,    -1,   110,   111,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,   173,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
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
     107,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,   173,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
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
     107,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
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
     122,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,   175,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    89,    -1,    -1,    -1,    93,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
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
     122,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
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
     112,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,   175,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,   171,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,   175,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,   175,    -1,   177,     3,     4,     5,     6,
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
      -1,    -1,    -1,   110,   111,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    31,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     4,     5,     6,     7,
       8,     9,    10,    -1,   141,   142,    14,   144,   145,   146,
      18,   148,   149,   150,    22,   152,   153,   154,   155,    27,
     157,   158,   159,   160,   161,    -1,   163,    35,    -1,   166,
     167,   168,    40,    41,    -1,    -1,    44,    -1,    -1,    47,
     177,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    -1,    -1,    64,    -1,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    31,    77,
      78,    79,    80,    81,    -1,    -1,    -1,    -1,    86,    -1,
      88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      98,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,   173,   174,    -1,    -1,   177,
       4,     5,     6,     7,     8,     9,    10,    -1,   141,   142,
      14,   144,   145,   146,    18,   148,   149,   150,    22,    -1,
     153,   154,   155,    27,   157,   158,   159,   160,    -1,    -1,
     163,    35,    -1,   166,   167,   168,    40,    41,    -1,    -1,
      44,    -1,    -1,    47,   177,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,
      64,    -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    78,    79,    80,    81,    -1,    -1,
      -1,    -1,    86,    -1,    88,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    98,    -1,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    12,    -1,   150,    -1,    -1,    17,
      18,    19,    20,    -1,    -1,   159,    -1,    25,   162,   163,
     164,    -1,    -1,    -1,   168,    33,   170,    35,   172,    -1,
     174,    39,    -1,   177,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,   143,   144,   145,   146,    -1,
     148,    -1,    -1,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,   163,    -1,    -1,   166,   167,
     168,   169,    -1,    -1,   172,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,    -1,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,     3,     4,     5,
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
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,   143,    -1,    -1,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,   164,    22,
      -1,    -1,    25,    -1,   170,    -1,   172,    -1,   174,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,     3,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    12,    -1,    -1,    -1,    -1,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,   141,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,
      -1,   164,    -1,    -1,    62,    -1,    -1,    -1,    -1,   172,
      -1,   174,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
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
     162,   163,    -1,    89,   166,   167,   168,    93,   170,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,   124,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    72,    -1,
     166,   167,   168,    -1,   170,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
     124,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      72,   165,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    12,    89,    -1,    -1,
      -1,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    12,    89,   166,   167,   168,    93,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    12,    89,
     166,   167,   168,    93,   170,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    47,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    12,    89,   166,   167,   168,    93,
     170,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
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
     168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    47,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
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
      12,    89,   166,   167,   168,    93,   170,    19,    -1,    -1,
      -1,    -1,    -1,   177,   102,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    89,   166,   167,
     168,    93,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    31,   124,    -1,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    72,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,   177,    -1,    -1,    12,    89,
      -1,    -1,    -1,    93,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    12,    13,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    31,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    89,   166,   167,   168,    93,
      -1,    -1,    -1,    12,    -1,    -1,    -1,   177,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,   120,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      89,   175,    -1,   177,    93,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      12,    -1,   166,   167,   168,   124,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    12,   166,   167,   168,
      -1,   170,    -1,    19,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,   165,   166,   167,   168,   123,   124,    -1,
      -1,    -1,    -1,    -1,    89,   177,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   102,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,   124,
     166,   167,   168,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,   177,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    31,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    12,    13,    -1,    -1,    -1,    -1,
      -1,    19,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,   124,   166,   167,   168,
      -1,    -1,    -1,    -1,   173,    -1,    89,    -1,   177,    -1,
      93,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    12,   120,   166,   167,
     168,   124,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    12,    -1,   166,   167,   168,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,   165,
     166,   167,   168,   124,    -1,    -1,    -1,    12,    89,    -1,
      -1,   177,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    31,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    12,    89,   166,   167,   168,    93,    -1,
      19,    -1,   173,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    31,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      89,   166,   167,   168,    93,    -1,    -1,    -1,   173,    12,
      -1,    -1,   177,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,   123,   124,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   124,    -1,   166,   167,   168,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,   177,    -1,
      93,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    12,    -1,   166,   167,   168,
      -1,   124,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    12,    -1,   166,   167,   168,    -1,    -1,    19,    -1,
     173,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,
     177,    12,    93,    -1,    -1,    -1,    -1,    -1,    19,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      31,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      -1,    12,    13,    -1,    -1,    -1,   177,    -1,    19,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      31,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    89,    -1,
      -1,    -1,    93,    -1,   175,    -1,   177,    -1,    -1,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    12,    89,    -1,
      -1,    -1,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    31,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    89,   166,   167,   168,    93,    -1,
      -1,    -1,    12,    13,    -1,    -1,   177,   102,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    -1,    -1,    -1,    -1,    12,    89,
      -1,    -1,   177,    93,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,
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
     124,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    31,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    89,   166,   167,   168,    93,    -1,    -1,    -1,    12,
      -1,    -1,    -1,   177,   102,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,   177,
      93,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    22,   166,   167,   168,    26,    27,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    35,    -1,    37,    38,    -1,
      22,    -1,    -1,    43,    26,    27,    -1,    -1,    -1,    -1,
      50,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    85,    -1,    -1,    88,    -1,
      90,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    85,    -1,    -1,    88,    -1,    90,   109,
      -1,    -1,   112,    -1,    -1,   115,    22,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,   109,    -1,    35,
     112,    37,    38,   115,    22,    -1,    -1,    43,    26,    27,
     140,    -1,    -1,    -1,    50,    -1,    -1,    35,    -1,    37,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,   140,    -1,
      -1,    -1,    50,    -1,    -1,   165,    -1,    -1,    -1,    -1,
      76,   171,    78,    -1,    80,    -1,    82,    -1,    -1,    85,
      -1,    -1,    88,   165,    90,    -1,    -1,    -1,    76,   171,
      78,    -1,    80,    -1,    82,    -1,    -1,    85,    -1,    -1,
      88,    -1,    90,   109,    -1,    -1,   112,    22,    -1,   115,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,   109,    37,    38,   112,    -1,    -1,   115,    43,    -1,
      -1,    -1,    -1,    -1,   140,    50,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    18,   140,    20,    -1,    -1,    -1,    -1,    25,   165,
      -1,    76,    -1,    78,    -1,   171,    33,    82,    -1,    -1,
      85,    -1,    39,    88,    -1,    90,    -1,   165,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    62,    -1,   112,    -1,    -1,
     115,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,     3,    -1,   140,    -1,    -1,    -1,    96,
      97,    -1,    99,    -1,   101,    -1,   103,    17,    18,   106,
      20,    -1,    -1,   110,    -1,    25,   113,    27,    -1,   116,
     165,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,   140,    -1,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    17,    18,    -1,    20,    -1,    77,    78,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    87,    33,    -1,
      90,    -1,    -1,    -1,    39,    -1,    96,    97,    -1,    99,
      45,   101,    -1,   103,    49,    -1,   106,    52,    -1,    -1,
     110,    -1,    -1,   113,    -1,   115,   116,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    17,    18,    -1,
      20,    96,    97,    -1,    99,    25,   101,    -1,   103,    -1,
      -1,   106,    -1,    33,    -1,   110,   111,    -1,   113,    39,
      -1,   116,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    17,    18,    -1,    20,    -1,    77,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    87,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    96,    97,    -1,    99,
      45,   101,    -1,   103,    49,    -1,   106,    52,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    17,    18,    -1,
      20,    96,    97,    -1,    99,    25,   101,    -1,   103,    -1,
      -1,   106,    -1,    33,    -1,   110,    -1,    -1,   113,    39,
      -1,   116,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,
      -1,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116
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
     177,   184,   185,   186,   187,   188,   189,   192,   193,   194,
     195,   200,   201,   202,   204,   205,   206,   211,   212,   216,
     217,   221,   223,   226,   227,   230,   231,   232,   233,   234,
     235,   238,   239,   241,   243,   246,   247,   248,   249,   250,
     254,   255,   260,   261,   262,   263,   266,   267,   272,   273,
     275,   276,   281,   285,   286,   288,   289,   311,   316,   317,
     321,   322,   343,   344,   345,   346,   347,   348,   349,   357,
     358,   359,   360,   361,   362,   364,   367,   368,   369,   370,
     371,   372,   373,   374,   376,   377,   378,   379,   380,   165,
      22,    35,    40,    41,    44,    55,    88,   101,   170,   174,
     246,   273,   343,   348,   358,   359,   364,   367,   369,   370,
     120,   350,   351,     3,   218,   364,   350,   364,   109,   323,
      90,   218,   186,   337,   364,   186,   172,     3,    17,    18,
      20,    25,    33,    39,    45,    49,    52,    62,    69,    70,
      77,    87,    96,    97,    99,   101,   103,   106,   110,   113,
     116,   219,   220,   222,    11,    78,   122,   240,   364,   240,
     364,   240,   364,    27,   115,   242,   364,    82,    85,   202,
     172,   220,   220,   220,   172,   220,   324,   325,    32,   206,
     225,   364,   258,   259,   364,   364,    18,    77,    96,   113,
     364,   364,   364,     8,   172,   229,   228,    27,    33,    47,
      49,    51,    75,    78,    90,    97,   101,   109,   185,   224,
     277,   278,   279,   302,   303,   304,   329,   334,   364,   337,
     107,   108,   165,   281,   282,   363,   364,   366,   364,   225,
     364,   364,   364,   100,   172,   186,   364,   364,   188,   206,
     188,   206,   364,   242,   364,   366,     3,   364,   364,   364,
     364,   364,     1,   171,   184,   207,   337,   111,   151,   338,
     339,   366,   175,   240,   337,   364,   375,   364,    80,   186,
      22,    35,    38,    88,   170,   189,   190,   191,   202,   206,
     194,     8,   149,   196,   170,    46,    84,   114,   203,    26,
     322,   364,     8,   262,   364,   365,    55,   146,   264,   220,
       1,   193,   220,   268,   271,   172,   297,    53,    73,    83,
     306,    27,    75,    78,    90,   109,   307,    27,    78,    90,
     109,   305,   220,   318,   319,   324,   164,   165,   364,    12,
      19,    31,    89,    93,   124,   141,   142,   144,   145,   146,
     148,   149,   150,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   166,   167,   168,   177,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   169,   293,   172,   174,    89,    93,
     364,   364,   364,   220,   337,    55,   172,   186,   165,   206,
      47,   350,   321,   165,   147,   165,   119,   221,   340,   341,
     342,   366,   170,   220,   172,   225,    32,    47,   225,   120,
     225,   353,    32,    47,   225,   353,   225,   353,    47,   225,
     353,   220,   220,   102,   206,   102,   125,   206,   293,   203,
     340,   172,   172,   206,   186,   111,   172,   220,   326,   327,
       1,   146,   333,    47,   147,   186,   225,   147,   225,    13,
     172,   172,   225,   340,   230,   230,    47,    90,   303,   304,
     173,   147,   111,   172,   220,   301,   146,   165,   364,   364,
     123,   283,   165,   170,   225,   172,   340,   165,   251,   251,
     225,   225,   165,   171,   171,   184,   147,   171,   364,   147,
     173,   147,   173,   364,   175,   353,    47,   147,   175,   353,
     123,   147,   175,     8,    40,    41,   174,   239,     3,   172,
     197,     1,   171,   207,   214,   215,   364,   209,   364,    65,
      36,    72,   165,   262,   264,   109,   236,   286,   220,   337,
     170,   171,   271,   125,   147,   171,   173,   298,   300,   302,
     307,    90,     1,   146,   331,   332,    90,     1,     3,    12,
      17,    19,    20,    25,    45,    52,    54,    62,    70,    87,
      99,   103,   110,   116,   141,   142,   143,   144,   145,   146,
     148,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   163,   166,   167,   168,   169,   172,   220,   290,   291,
     292,   293,   343,   125,   320,   147,   165,   165,   364,   364,
     364,   240,   364,   240,   364,   364,   364,   364,   364,   364,
     364,     3,    20,    33,    62,   103,   109,   221,   364,   364,
     364,   364,   364,   364,   364,   364,   364,   364,   364,   364,
     364,   364,   364,   364,   364,    68,   366,   366,   366,   366,
     366,   340,   340,   240,   364,   240,   364,    32,    47,    32,
      47,   102,   172,    47,   175,   220,    27,    47,    90,   115,
     352,   355,   356,   364,   380,   240,   364,   225,   324,   364,
     364,   125,   173,   147,    47,   337,    44,   364,   240,   364,
     172,   225,    44,   364,   240,   364,   225,   225,   240,   364,
     225,   125,   125,   186,    34,   186,   364,    34,   364,    65,
     173,   341,   220,   244,   245,   327,   147,   173,   224,   364,
     125,   328,   364,   325,   258,   220,   321,   364,   364,   173,
     301,   307,   279,   327,   146,   335,   336,   334,   284,   205,
       1,   256,   342,   173,    21,   252,   171,   173,   173,   338,
     173,   338,   186,   175,   240,   364,   175,   186,   364,   175,
     364,   175,   364,   364,   364,   337,   196,   173,   198,   199,
     342,   171,   171,   147,   165,    13,   149,   147,   165,    13,
      36,    72,   220,   141,   142,   143,   144,   145,   159,   163,
     168,   208,   292,   293,   294,   364,   208,   210,   264,   170,
     237,   321,   165,   172,     1,   265,   364,   269,   270,   147,
     173,   333,     1,   224,   329,   330,   105,   308,   172,   296,
     364,   141,   149,   296,   296,   364,   319,   172,   174,   165,
     165,   165,   165,   165,   165,   173,   175,    44,   240,   364,
      44,   240,   364,   341,   240,   364,   172,    47,    90,   147,
     173,    17,    20,    25,    45,    52,    62,    70,    87,    99,
     110,   116,   343,    89,    89,   350,   350,   165,   165,   366,
     342,   364,   173,   364,    32,   225,    32,   225,   354,   355,
     364,    32,   225,   353,    32,   225,   353,   225,   353,   225,
     353,   364,   364,    34,   186,    34,    34,   186,   102,   206,
     220,   173,   147,   173,   173,   326,   333,    68,   366,   220,
     173,   173,   336,   146,   280,   173,   334,   151,   295,   328,
     364,   171,    74,   117,   171,   257,   173,   172,   206,   220,
     253,   186,   175,   353,   175,   353,   186,   123,    47,    47,
      47,   175,   147,   173,   214,   221,    17,    18,    20,    25,
      45,    49,    52,    70,    77,    87,    96,    97,    99,   101,
     110,   113,   116,   170,   213,   294,   364,   111,   364,   208,
     210,   147,   165,    13,   165,   170,   265,   318,   324,   341,
     171,    82,    85,   171,   185,   193,   227,   271,   193,   300,
     308,    60,   118,   312,   298,   299,   173,   291,   293,   173,
     175,   364,    32,    32,   364,    32,    32,   173,   175,   175,
     341,   355,   333,   343,   343,   225,   225,   225,   102,    44,
     364,    44,   364,   147,   173,   102,    44,   364,   225,    44,
     364,   225,   225,   225,   186,   186,   364,   186,    34,   165,
     165,   245,   206,   328,   172,   172,   295,   328,   329,   308,
     336,   364,    32,   186,   337,   253,   146,   206,    44,   186,
     364,   175,    44,   186,   364,   175,   364,   240,   364,   240,
     364,   240,   364,   199,   208,    13,    36,    72,    36,    72,
     165,   165,   294,   364,   364,   265,   171,   165,   165,   173,
     193,   227,   227,   271,   312,     3,    92,   103,   313,   314,
     315,   364,   287,   173,   296,   296,   102,    44,    44,   102,
      44,    44,    44,    44,   173,   328,   364,   364,   364,   355,
     364,   364,   364,    34,   186,   340,   340,   295,   328,   186,
     225,   173,   364,   364,   186,   364,   186,   175,   175,   171,
     111,   364,   208,   210,   208,   210,    13,   171,   165,   227,
     274,   315,   118,   147,   125,   152,   154,   155,   157,   158,
      60,    32,   165,   205,   226,   309,   310,   364,   364,   364,
     364,   364,   364,   102,   102,   102,   102,   186,   173,   173,
     206,   102,   102,   165,   165,   165,   165,   364,   310,   364,
     314,   315,   315,   315,   315,   315,   315,   313,   184,   102,
     102,   102,   102,   102,   102,   364,   364,   364,   364,   164,
     164,   364,   364
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   181,   182,   183,   183,   184,   184,   185,   185,   186,
     186,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   189,   189,   190,   191,   191,
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
     230,   230,   230,   230,   230,   230,   230,   231,   231,   231,
     231,   232,   233,   233,   234,   235,   235,   235,   236,   235,
     237,   235,   238,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   240,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   242,   242,   243,   243,   244,   244,   245,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   247,   247,   247,   247,   247,   247,   248,   248,   248,
     249,   249,   249,   250,   250,   250,   250,   251,   251,   252,
     252,   252,   253,   253,   254,   255,   255,   256,   256,   257,
     257,   257,   258,   258,   258,   259,   259,   260,   261,   261,
     262,   263,   263,   263,   264,   264,   265,   265,   265,   265,
     265,   266,   266,   267,   268,   268,   269,   268,   268,   270,
     268,   271,   271,   272,   274,   273,   275,   276,   276,   276,
     277,   277,   278,   278,   279,   279,   279,   280,   280,   281,
     283,   282,   284,   282,   285,   287,   286,   288,   288,   288,
     288,   288,   289,   290,   290,   291,   291,   291,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   293,   293,   293,   293,   293,   293,   293,
     293,   293,   293,   293,   293,   294,   294,   295,   295,   296,
     296,   297,   297,   298,   298,   299,   299,   300,   301,   301,
     302,   302,   302,   302,   302,   302,   303,   303,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   305,   305,   305,
     305,   305,   305,   306,   306,   306,   307,   307,   307,   307,
     307,   307,   307,   308,   308,   309,   309,   310,   310,   310,
     311,   312,   312,   312,   312,   312,   313,   313,   314,   314,
     314,   314,   314,   314,   314,   315,   315,   316,   317,   317,
     318,   318,   319,   320,   320,   321,   321,   321,   321,   321,
     323,   322,   322,   324,   324,   325,   325,   326,   326,   326,
     327,   327,   327,   328,   328,   328,   329,   330,   330,   330,
     331,   331,   332,   332,   333,   333,   333,   333,   334,   334,
     335,   336,   336,   337,   337,   338,   338,   339,   339,   340,
     340,   341,   341,   342,   342,   343,   343,   344,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   344,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   346,   346,   346,   346,   346,   346,   346,   347,   348,
     349,   349,   349,   349,   349,   349,   349,   350,   350,   351,
     352,   352,   353,   354,   354,   355,   355,   355,   356,   356,
     356,   356,   356,   356,   357,   357,   357,   357,   357,   358,
     358,   358,   358,   358,   359,   360,   360,   360,   360,   360,
     360,   361,   362,   363,   363,   364,   364,   364,   364,   364,
     364,   364,   364,   364,   364,   364,   364,   364,   364,   364,
     364,   364,   365,   365,   366,   366,   366,   367,   367,   367,
     367,   368,   368,   368,   368,   368,   369,   369,   369,   370,
     370,   370,   370,   370,   370,   371,   371,   371,   371,   372,
     372,   373,   373,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   375,   375,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   377,   377,   377,   377,   377,   377,   377,   378,   378,
     378,   378,   379,   379,   379,   379,   380,   380,   380,   380,
     380,   380,   380
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     1,     3,     3,     3,     3,     2,     1,
       1,     1,     1,     1,     2,     1,     3,     3,     3,     3,
       2,     3,     3,     2,     2,     1,     1,     0,     2,     1,
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
       1,     1,     1,     1,     1,     1,     1,     3,     5,     5,
       2,     1,     1,     1,     1,     6,     7,     3,     0,     6,
       0,     6,     2,     5,     3,     3,     6,     6,     4,     5,
       5,     3,     3,     6,     5,     6,     5,     6,     3,     4,
       3,     4,     5,     6,     5,     6,     3,     4,     3,     4,
       6,     7,     6,     7,     4,     5,     4,     5,     4,     4,
       3,     6,     5,     4,     3,     6,     5,     6,     5,     8,
       7,     4,     4,     6,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     6,     4,     7,
       5,     3,     6,     2,     2,     3,     3,     0,     2,     2,
       3,     5,     1,     3,     3,     5,     5,     0,     2,     3,
       2,     3,     4,     3,     1,     1,     3,     3,     5,     5,
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
       2,     1,     2,     1,     2,     1,     2,     1,     1,     6,
       6,     4,     9,     9,     7,     6,     6,     4,     9,     9,
       7,     2,     3,     4,     6,     6,     9,     9,     6,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     1,     4,
       1,     3,     4,     1,     3,     4,     3,     3,     1,     1,
       2,     1,     2,     1,     1,     2,     2,     2,     2,     2,
       8,     8,     9,     9,     4,     3,     3,     2,     2,     2,
       1,     3,     4,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     2,     2,     1,     1,     4,     4,     4,     3,
       3,     3,     3,     5,     5,     3,     4,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     4,     3,     4,     3,     4,     3,     5,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       1,     1,     1
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
#line 6240 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 639 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6246 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 640 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6252 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 647 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6260 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 655 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6268 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 659 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6277 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 669 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6285 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 674 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6293 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 681 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6299 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 682 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6305 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 683 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6311 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 684 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6317 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 685 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6323 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6329 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 687 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6335 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 688 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6341 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 689 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6347 "bison-chpl-lib.cpp"
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
#line 6364 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 703 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6370 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 704 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6376 "bison-chpl-lib.cpp"
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
#line 6389 "bison-chpl-lib.cpp"
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
#line 6402 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TLABEL ident_def stmt  */
#line 724 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6410 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 728 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6421 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: error TSEMI  */
#line 735 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6430 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: assignment_stmt  */
#line 742 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6436 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: if_stmt  */
#line 743 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6442 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: loop_stmt  */
#line 744 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6448 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: select_stmt  */
#line 745 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6454 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: manage_stmt  */
#line 746 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6460 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 747 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6466 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: throw_stmt  */
#line 748 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6472 "bison-chpl-lib.cpp"
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
#line 6490 "bison-chpl-lib.cpp"
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
#line 6509 "bison-chpl-lib.cpp"
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
#line 6521 "bison-chpl-lib.cpp"
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
#line 6537 "bison-chpl-lib.cpp"
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
#line 6552 "bison-chpl-lib.cpp"
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
#line 6568 "bison-chpl-lib.cpp"
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
#line 6584 "bison-chpl-lib.cpp"
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
#line 6599 "bison-chpl-lib.cpp"
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
#line 6616 "bison-chpl-lib.cpp"
    break;

  case 47: /* collect_attributes: %empty  */
#line 865 "chpl.ypp"
  {
    auto attributeGroup = context->buildAttributeGroup((yyloc));
    if (attributeGroup != nullptr) {
      context->resetAttributeGroupPartsState();
    }
    context->loopAttributes = attributeGroup.release();
  }
#line 6628 "bison-chpl-lib.cpp"
    break;

  case 48: /* attribute_receiver: collect_attributes loop_stmt  */
#line 875 "chpl.ypp"
                               { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6634 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 891 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), true, (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6642 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace  */
#line 895 "chpl.ypp"
 {
  // this allows us to distinguish between @attribute() and @attribute
  // but should not be needed once `@unstable "msg"` is deprecated and gone
  auto lst = new MaybeNamedActualList();
  context->noteAttribute((yylsp[-1]), (yyvsp[-1].expr), false, (yyvsp[0].exprList), lst);
 }
#line 6653 "bison-chpl-lib.cpp"
    break;

  case 56: /* attribute_decl_stmt: attribute_decl_begin STRINGLITERAL  */
#line 902 "chpl.ypp"
 {
  // this allows us to maintain @unstable "message" without requiring parens
  // but should not be needed once `@unstable "msg"` is deprecated and gone
  auto lst = new MaybeNamedActualList();
  auto actual = makeMaybeNamedActual((yyvsp[0].expr));
  lst->push_back(actual);
  context->noteAttribute((yylsp[-1]), (yyvsp[-1].expr), false, context->makeList(), lst);
 }
#line 6666 "bison-chpl-lib.cpp"
    break;

  case 57: /* attribute_decl_begin: TATMARK TIDENT  */
#line 914 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6675 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_decl_toolspace: %empty  */
#line 921 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6683 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 925 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6694 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: TLP TRP  */
#line 935 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6700 "bison-chpl-lib.cpp"
    break;

  case 61: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 937 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6708 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actual  */
#line 944 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6717 "bison-chpl-lib.cpp"
    break;

  case 63: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 949 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6727 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 962 "chpl.ypp"
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
#line 6748 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 982 "chpl.ypp"
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
#line 6767 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 997 "chpl.ypp"
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
#line 6786 "bison-chpl-lib.cpp"
    break;

  case 68: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1012 "chpl.ypp"
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
#line 6803 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: %empty  */
#line 1027 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6809 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPUBLIC  */
#line 1028 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6816 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_access_control: TPRIVATE  */
#line 1030 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6823 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: %empty  */
#line 1035 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6829 "bison-chpl-lib.cpp"
    break;

  case 73: /* opt_prototype: TPROTOTYPE  */
#line 1036 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6836 "bison-chpl-lib.cpp"
    break;

  case 74: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1042 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6846 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR TRCBR  */
#line 1060 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6854 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1064 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6862 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt_body: TLCBR error TRCBR  */
#line 1068 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6874 "bison-chpl-lib.cpp"
    break;

  case 78: /* block_stmt: block_stmt_body  */
#line 1079 "chpl.ypp"
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
#line 6893 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: toplevel_stmt  */
#line 1097 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6899 "bison-chpl-lib.cpp"
    break;

  case 80: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1098 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6905 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: expr  */
#line 1103 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6913 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: all_op_name  */
#line 1107 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6921 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: expr TAS expr  */
#line 1111 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6930 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA expr  */
#line 1116 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6938 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1120 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6946 "bison-chpl-lib.cpp"
    break;

  case 86: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1124 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6955 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr  */
#line 1134 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6964 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: expr TAS expr  */
#line 1139 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6974 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1145 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6985 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1152 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6994 "bison-chpl-lib.cpp"
    break;

  case 91: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1157 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7005 "bison-chpl-lib.cpp"
    break;

  case 92: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1164 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7017 "bison-chpl-lib.cpp"
    break;

  case 93: /* opt_only_ls: %empty  */
#line 1174 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7023 "bison-chpl-lib.cpp"
    break;

  case 94: /* opt_only_ls: renames_ls  */
#line 1175 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7029 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1180 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7038 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1185 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7050 "bison-chpl-lib.cpp"
    break;

  case 97: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1193 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7063 "bison-chpl-lib.cpp"
    break;

  case 98: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1202 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7077 "bison-chpl-lib.cpp"
    break;

  case 99: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1212 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7089 "bison-chpl-lib.cpp"
    break;

  case 100: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1220 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7102 "bison-chpl-lib.cpp"
    break;

  case 101: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1229 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7116 "bison-chpl-lib.cpp"
    break;

  case 102: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1242 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7125 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_expr: expr  */
#line 1278 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 7133 "bison-chpl-lib.cpp"
    break;

  case 121: /* import_expr: expr TDOT all_op_name  */
#line 1282 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7142 "bison-chpl-lib.cpp"
    break;

  case 122: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1287 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7151 "bison-chpl-lib.cpp"
    break;

  case 123: /* import_expr: expr TAS ident_use  */
#line 1292 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 7161 "bison-chpl-lib.cpp"
    break;

  case 124: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1298 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 7171 "bison-chpl-lib.cpp"
    break;

  case 125: /* import_ls: import_expr  */
#line 1306 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7177 "bison-chpl-lib.cpp"
    break;

  case 126: /* import_ls: import_ls TCOMMA import_expr  */
#line 1307 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7183 "bison-chpl-lib.cpp"
    break;

  case 127: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1312 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7193 "bison-chpl-lib.cpp"
    break;

  case 128: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1321 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7201 "bison-chpl-lib.cpp"
    break;

  case 129: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1325 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7209 "bison-chpl-lib.cpp"
    break;

  case 130: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1329 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7217 "bison-chpl-lib.cpp"
    break;

  case 131: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1333 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7225 "bison-chpl-lib.cpp"
    break;

  case 132: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1337 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7233 "bison-chpl-lib.cpp"
    break;

  case 133: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1341 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7241 "bison-chpl-lib.cpp"
    break;

  case 134: /* opt_label_ident: %empty  */
#line 1349 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7247 "bison-chpl-lib.cpp"
    break;

  case 135: /* opt_label_ident: TIDENT  */
#line 1350 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7253 "bison-chpl-lib.cpp"
    break;

  case 143: /* ident_use: TIDENT  */
#line 1375 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7259 "bison-chpl-lib.cpp"
    break;

  case 144: /* ident_use: TTHIS  */
#line 1376 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7265 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TBOOL  */
#line 1403 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7271 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TENUM  */
#line 1404 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7277 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TINT  */
#line 1405 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7283 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TUINT  */
#line 1406 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7289 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TREAL  */
#line 1407 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7295 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TIMAG  */
#line 1408 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7301 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TCOMPLEX  */
#line 1409 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7307 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TBYTES  */
#line 1410 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7313 "bison-chpl-lib.cpp"
    break;

  case 172: /* scalar_type: TSTRING  */
#line 1411 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7319 "bison-chpl-lib.cpp"
    break;

  case 173: /* scalar_type: TLOCALE  */
#line 1412 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7325 "bison-chpl-lib.cpp"
    break;

  case 174: /* scalar_type: TNOTHING  */
#line 1413 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7331 "bison-chpl-lib.cpp"
    break;

  case 175: /* scalar_type: TVOID  */
#line 1414 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7337 "bison-chpl-lib.cpp"
    break;

  case 180: /* do_stmt: TDO stmt  */
#line 1428 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7343 "bison-chpl-lib.cpp"
    break;

  case 181: /* do_stmt: block_stmt  */
#line 1429 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7349 "bison-chpl-lib.cpp"
    break;

  case 182: /* return_stmt: TRETURN TSEMI  */
#line 1434 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7360 "bison-chpl-lib.cpp"
    break;

  case 183: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1441 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7371 "bison-chpl-lib.cpp"
    break;

  case 184: /* class_level_stmt: TSEMI  */
#line 1451 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7379 "bison-chpl-lib.cpp"
    break;

  case 185: /* class_level_stmt: inner_class_level_stmt  */
#line 1455 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7389 "bison-chpl-lib.cpp"
    break;

  case 186: /* $@1: %empty  */
#line 1460 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7396 "bison-chpl-lib.cpp"
    break;

  case 187: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1462 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7405 "bison-chpl-lib.cpp"
    break;

  case 188: /* $@2: %empty  */
#line 1466 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7412 "bison-chpl-lib.cpp"
    break;

  case 189: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1468 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7421 "bison-chpl-lib.cpp"
    break;

  case 197: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1486 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7430 "bison-chpl-lib.cpp"
    break;

  case 198: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1491 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7439 "bison-chpl-lib.cpp"
    break;

  case 199: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1496 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7448 "bison-chpl-lib.cpp"
    break;

  case 200: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1501 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7456 "bison-chpl-lib.cpp"
    break;

  case 201: /* forwarding_decl_start: TFORWARDING  */
#line 1508 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7465 "bison-chpl-lib.cpp"
    break;

  case 202: /* extern_or_export: TEXTERN  */
#line 1515 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7471 "bison-chpl-lib.cpp"
    break;

  case 203: /* extern_or_export: TEXPORT  */
#line 1516 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7477 "bison-chpl-lib.cpp"
    break;

  case 204: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1521 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7487 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1531 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7496 "bison-chpl-lib.cpp"
    break;

  case 206: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1537 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7507 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1544 "chpl.ypp"
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
#line 7529 "bison-chpl-lib.cpp"
    break;

  case 208: /* $@3: %empty  */
#line 1562 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7537 "bison-chpl-lib.cpp"
    break;

  case 209: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1566 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7546 "bison-chpl-lib.cpp"
    break;

  case 210: /* $@4: %empty  */
#line 1571 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7555 "bison-chpl-lib.cpp"
    break;

  case 211: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1576 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7564 "bison-chpl-lib.cpp"
    break;

  case 212: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1584 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7572 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1591 "chpl.ypp"
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
                               toOwned(context->loopAttributes));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7595 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TWHILE expr do_stmt  */
#line 1610 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto body = context->consumeToBlock((yylsp[0]), exprLst);
    auto node = While::build(BUILDER, LOC((yylsp[-2])), toOwned((yyvsp[-1].expr)),
                             blockStyle,
                             std::move(body),
                             toOwned(context->loopAttributes));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7613 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1624 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto body = context->consumeToBlock((yylsp[0]), exprLst);
    auto node = While::build(BUILDER, LOC((yylsp[-2])), toOwned((yyvsp[-1].expr)),
                             blockStyle,
                             std::move(body),
                             toOwned(context->loopAttributes));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7631 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1638 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7639 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1642 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7647 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1646 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7655 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1650 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7663 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1654 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7671 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt: TFOR expr do_stmt  */
#line 1658 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7679 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1662 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7687 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1666 "chpl.ypp"
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
                           toOwned(context->loopAttributes));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    context->resetAttributeGroupPartsState();
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7711 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1686 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7719 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1690 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7727 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1694 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7735 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1698 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7743 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt: TFORALL expr do_stmt  */
#line 1702 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7751 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1706 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7759 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1710 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7767 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1714 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7775 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1718 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7783 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1722 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7791 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1726 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7799 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1730 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7807 "bison-chpl-lib.cpp"
    break;

  case 236: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1734 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7815 "bison-chpl-lib.cpp"
    break;

  case 237: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1738 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7823 "bison-chpl-lib.cpp"
    break;

  case 238: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1742 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7831 "bison-chpl-lib.cpp"
    break;

  case 239: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1746 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7839 "bison-chpl-lib.cpp"
    break;

  case 240: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1750 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7847 "bison-chpl-lib.cpp"
    break;

  case 241: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1754 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7855 "bison-chpl-lib.cpp"
    break;

  case 242: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1758 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7863 "bison-chpl-lib.cpp"
    break;

  case 243: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1762 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7871 "bison-chpl-lib.cpp"
    break;

  case 244: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1766 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7879 "bison-chpl-lib.cpp"
    break;

  case 245: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1770 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7887 "bison-chpl-lib.cpp"
    break;

  case 246: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1774 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7896 "bison-chpl-lib.cpp"
    break;

  case 247: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1779 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7905 "bison-chpl-lib.cpp"
    break;

  case 248: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1787 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7915 "bison-chpl-lib.cpp"
    break;

  case 249: /* if_stmt: TIF expr TTHEN stmt  */
#line 1796 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7923 "bison-chpl-lib.cpp"
    break;

  case 250: /* if_stmt: TIF expr block_stmt  */
#line 1800 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7931 "bison-chpl-lib.cpp"
    break;

  case 251: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1804 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7939 "bison-chpl-lib.cpp"
    break;

  case 252: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1808 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7947 "bison-chpl-lib.cpp"
    break;

  case 253: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1812 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7955 "bison-chpl-lib.cpp"
    break;

  case 254: /* if_stmt: TIF ifvar block_stmt  */
#line 1816 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7963 "bison-chpl-lib.cpp"
    break;

  case 255: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1820 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7971 "bison-chpl-lib.cpp"
    break;

  case 256: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1824 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7979 "bison-chpl-lib.cpp"
    break;

  case 257: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1828 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7988 "bison-chpl-lib.cpp"
    break;

  case 258: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1833 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7997 "bison-chpl-lib.cpp"
    break;

  case 259: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1838 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8006 "bison-chpl-lib.cpp"
    break;

  case 260: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1843 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8015 "bison-chpl-lib.cpp"
    break;

  case 261: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1851 "chpl.ypp"
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
#line 8034 "bison-chpl-lib.cpp"
    break;

  case 262: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1866 "chpl.ypp"
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
#line 8053 "bison-chpl-lib.cpp"
    break;

  case 263: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1884 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8061 "bison-chpl-lib.cpp"
    break;

  case 264: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1888 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8069 "bison-chpl-lib.cpp"
    break;

  case 265: /* ifc_formal_ls: ifc_formal  */
#line 1894 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8075 "bison-chpl-lib.cpp"
    break;

  case 266: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1895 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8081 "bison-chpl-lib.cpp"
    break;

  case 267: /* ifc_formal: ident_def  */
#line 1900 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8087 "bison-chpl-lib.cpp"
    break;

  case 280: /* implements_type_ident: implements_type_error_ident  */
#line 1918 "chpl.ypp"
  {
    context->syntax((yyloc), "type '%s' cannot implement an interface.", (yyvsp[0].uniqueStr).c_str());
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8096 "bison-chpl-lib.cpp"
    break;

  case 287: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1944 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8104 "bison-chpl-lib.cpp"
    break;

  case 288: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1948 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 8112 "bison-chpl-lib.cpp"
    break;

  case 289: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1952 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8120 "bison-chpl-lib.cpp"
    break;

  case 290: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1959 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8128 "bison-chpl-lib.cpp"
    break;

  case 291: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1963 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 8136 "bison-chpl-lib.cpp"
    break;

  case 292: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1967 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8144 "bison-chpl-lib.cpp"
    break;

  case 293: /* try_stmt: TTRY tryable_stmt  */
#line 1974 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8152 "bison-chpl-lib.cpp"
    break;

  case 294: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1978 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8160 "bison-chpl-lib.cpp"
    break;

  case 295: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1982 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8168 "bison-chpl-lib.cpp"
    break;

  case 296: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1986 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8176 "bison-chpl-lib.cpp"
    break;

  case 297: /* catch_expr_ls: %empty  */
#line 1992 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8182 "bison-chpl-lib.cpp"
    break;

  case 298: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1993 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8188 "bison-chpl-lib.cpp"
    break;

  case 299: /* catch_expr: TCATCH block_stmt  */
#line 1998 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8196 "bison-chpl-lib.cpp"
    break;

  case 300: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2002 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8204 "bison-chpl-lib.cpp"
    break;

  case 301: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2006 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8212 "bison-chpl-lib.cpp"
    break;

  case 302: /* catch_expr_inner: ident_def  */
#line 2013 "chpl.ypp"
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
#line 8229 "bison-chpl-lib.cpp"
    break;

  case 303: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2026 "chpl.ypp"
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
#line 8246 "bison-chpl-lib.cpp"
    break;

  case 304: /* throw_stmt: TTHROW expr TSEMI  */
#line 2042 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8256 "bison-chpl-lib.cpp"
    break;

  case 305: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2050 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8264 "bison-chpl-lib.cpp"
    break;

  case 306: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2054 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8274 "bison-chpl-lib.cpp"
    break;

  case 307: /* when_stmt_ls: %empty  */
#line 2062 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8280 "bison-chpl-lib.cpp"
    break;

  case 308: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2063 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8286 "bison-chpl-lib.cpp"
    break;

  case 309: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2068 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8294 "bison-chpl-lib.cpp"
    break;

  case 310: /* when_stmt: TOTHERWISE stmt  */
#line 2072 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8303 "bison-chpl-lib.cpp"
    break;

  case 311: /* when_stmt: TOTHERWISE TDO stmt  */
#line 2077 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8312 "bison-chpl-lib.cpp"
    break;

  case 312: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2085 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8321 "bison-chpl-lib.cpp"
    break;

  case 313: /* manager_expr: expr TAS ident_def  */
#line 2090 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8329 "bison-chpl-lib.cpp"
    break;

  case 314: /* manager_expr: expr  */
#line 2094 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8337 "bison-chpl-lib.cpp"
    break;

  case 315: /* manager_expr_ls: manager_expr  */
#line 2100 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8343 "bison-chpl-lib.cpp"
    break;

  case 316: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2101 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8349 "bison-chpl-lib.cpp"
    break;

  case 317: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2106 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8357 "bison-chpl-lib.cpp"
    break;

  case 318: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2115 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8366 "bison-chpl-lib.cpp"
    break;

  case 319: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2120 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8377 "bison-chpl-lib.cpp"
    break;

  case 320: /* class_start: class_tag ident_def  */
#line 2131 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8385 "bison-chpl-lib.cpp"
    break;

  case 321: /* class_tag: TCLASS  */
#line 2137 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8391 "bison-chpl-lib.cpp"
    break;

  case 322: /* class_tag: TRECORD  */
#line 2138 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8397 "bison-chpl-lib.cpp"
    break;

  case 323: /* class_tag: TUNION  */
#line 2139 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8403 "bison-chpl-lib.cpp"
    break;

  case 324: /* opt_inherit: %empty  */
#line 2143 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8409 "bison-chpl-lib.cpp"
    break;

  case 325: /* opt_inherit: TCOLON expr_ls  */
#line 2144 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8415 "bison-chpl-lib.cpp"
    break;

  case 326: /* class_level_stmt_ls: %empty  */
#line 2148 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8424 "bison-chpl-lib.cpp"
    break;

  case 327: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2153 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8432 "bison-chpl-lib.cpp"
    break;

  case 328: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2157 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8440 "bison-chpl-lib.cpp"
    break;

  case 329: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2161 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8448 "bison-chpl-lib.cpp"
    break;

  case 330: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2165 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8456 "bison-chpl-lib.cpp"
    break;

  case 331: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2172 "chpl.ypp"
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
#line 8477 "bison-chpl-lib.cpp"
    break;

  case 332: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2189 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8491 "bison-chpl-lib.cpp"
    break;

  case 333: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2202 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8499 "bison-chpl-lib.cpp"
    break;

  case 334: /* enum_ls: enum_item  */
#line 2209 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8508 "bison-chpl-lib.cpp"
    break;

  case 335: /* enum_ls: enum_ls TCOMMA  */
#line 2214 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8518 "bison-chpl-lib.cpp"
    break;

  case 336: /* $@5: %empty  */
#line 2220 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8527 "bison-chpl-lib.cpp"
    break;

  case 337: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2225 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8536 "bison-chpl-lib.cpp"
    break;

  case 338: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2230 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8545 "bison-chpl-lib.cpp"
    break;

  case 339: /* $@6: %empty  */
#line 2235 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8554 "bison-chpl-lib.cpp"
    break;

  case 340: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2240 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8563 "bison-chpl-lib.cpp"
    break;

  case 341: /* enum_item: ident_def  */
#line 2250 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8574 "bison-chpl-lib.cpp"
    break;

  case 342: /* enum_item: ident_def TASSIGN expr  */
#line 2257 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8587 "bison-chpl-lib.cpp"
    break;

  case 343: /* lambda_decl_start: TLAMBDA  */
#line 2269 "chpl.ypp"
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
#line 8603 "bison-chpl-lib.cpp"
    break;

  case 344: /* $@7: %empty  */
#line 2286 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8613 "bison-chpl-lib.cpp"
    break;

  case 345: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2292 "chpl.ypp"
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
#line 8635 "bison-chpl-lib.cpp"
    break;

  case 347: /* linkage_spec: linkage_spec_empty  */
#line 2315 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8641 "bison-chpl-lib.cpp"
    break;

  case 348: /* linkage_spec: TINLINE  */
#line 2316 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8648 "bison-chpl-lib.cpp"
    break;

  case 349: /* linkage_spec: TOVERRIDE  */
#line 2318 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8655 "bison-chpl-lib.cpp"
    break;

  case 350: /* opt_fn_type_formal_ls: %empty  */
#line 2323 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8661 "bison-chpl-lib.cpp"
    break;

  case 351: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2324 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8667 "bison-chpl-lib.cpp"
    break;

  case 352: /* fn_type_formal_ls: fn_type_formal  */
#line 2328 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8673 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2329 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8679 "bison-chpl-lib.cpp"
    break;

  case 354: /* fn_type_formal: named_formal  */
#line 2334 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8685 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2337 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8691 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_type_formal: formal_type  */
#line 2339 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8697 "bison-chpl-lib.cpp"
    break;

  case 357: /* opt_fn_type_ret_type: %empty  */
#line 2343 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8703 "bison-chpl-lib.cpp"
    break;

  case 358: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2344 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8709 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2357 "chpl.ypp"
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
#line 8728 "bison-chpl-lib.cpp"
    break;

  case 360: /* $@8: %empty  */
#line 2375 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8738 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2381 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8751 "bison-chpl-lib.cpp"
    break;

  case 362: /* $@9: %empty  */
#line 2390 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8761 "bison-chpl-lib.cpp"
    break;

  case 363: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2396 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8774 "bison-chpl-lib.cpp"
    break;

  case 364: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2408 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8782 "bison-chpl-lib.cpp"
    break;

  case 365: /* $@10: %empty  */
#line 2417 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();

    // May never have been built if there was a syntax error.
    if ((yyvsp[-4].functionParts).errorExpr == nullptr) {
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name->name());
    }
  }
#line 8796 "bison-chpl-lib.cpp"
    break;

  case 366: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2427 "chpl.ypp"
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
#line 8819 "bison-chpl-lib.cpp"
    break;

  case 367: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2449 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8831 "bison-chpl-lib.cpp"
    break;

  case 368: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2457 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8843 "bison-chpl-lib.cpp"
    break;

  case 369: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2465 "chpl.ypp"
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
#line 8858 "bison-chpl-lib.cpp"
    break;

  case 370: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2476 "chpl.ypp"
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
#line 8873 "bison-chpl-lib.cpp"
    break;

  case 371: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2487 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8883 "bison-chpl-lib.cpp"
    break;

  case 372: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2496 "chpl.ypp"
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
#line 8899 "bison-chpl-lib.cpp"
    break;

  case 374: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2511 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8905 "bison-chpl-lib.cpp"
    break;

  case 377: /* fn_ident: ident_def TBANG  */
#line 2518 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8915 "bison-chpl-lib.cpp"
    break;

  case 417: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2575 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8921 "bison-chpl-lib.cpp"
    break;

  case 418: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2576 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8927 "bison-chpl-lib.cpp"
    break;

  case 419: /* opt_formal_ls: %empty  */
#line 2580 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8933 "bison-chpl-lib.cpp"
    break;

  case 420: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2581 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8939 "bison-chpl-lib.cpp"
    break;

  case 421: /* req_formal_ls: TLP TRP  */
#line 2585 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8945 "bison-chpl-lib.cpp"
    break;

  case 422: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2586 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8951 "bison-chpl-lib.cpp"
    break;

  case 423: /* formal_ls_inner: formal  */
#line 2590 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8957 "bison-chpl-lib.cpp"
    break;

  case 424: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2591 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8963 "bison-chpl-lib.cpp"
    break;

  case 425: /* formal_ls: %empty  */
#line 2595 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8969 "bison-chpl-lib.cpp"
    break;

  case 426: /* formal_ls: formal_ls_inner  */
#line 2596 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8975 "bison-chpl-lib.cpp"
    break;

  case 430: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2610 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8983 "bison-chpl-lib.cpp"
    break;

  case 431: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2615 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8991 "bison-chpl-lib.cpp"
    break;

  case 432: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2620 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8999 "bison-chpl-lib.cpp"
    break;

  case 433: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2625 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 9007 "bison-chpl-lib.cpp"
    break;

  case 434: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2630 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9015 "bison-chpl-lib.cpp"
    break;

  case 435: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2635 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 9023 "bison-chpl-lib.cpp"
    break;

  case 436: /* opt_formal_intent_tag: %empty  */
#line 2641 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 9032 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_formal_intent_tag: required_intent_tag  */
#line 2646 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 9041 "bison-chpl-lib.cpp"
    break;

  case 438: /* required_intent_tag: TIN  */
#line 2653 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 9047 "bison-chpl-lib.cpp"
    break;

  case 439: /* required_intent_tag: TINOUT  */
#line 2654 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 9053 "bison-chpl-lib.cpp"
    break;

  case 440: /* required_intent_tag: TOUT  */
#line 2655 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 9059 "bison-chpl-lib.cpp"
    break;

  case 441: /* required_intent_tag: TCONST TIN  */
#line 2656 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 9065 "bison-chpl-lib.cpp"
    break;

  case 442: /* required_intent_tag: TCONST TREF  */
#line 2657 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9071 "bison-chpl-lib.cpp"
    break;

  case 443: /* required_intent_tag: TCONST  */
#line 2658 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9077 "bison-chpl-lib.cpp"
    break;

  case 444: /* required_intent_tag: TPARAM  */
#line 2659 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9083 "bison-chpl-lib.cpp"
    break;

  case 445: /* required_intent_tag: TREF  */
#line 2660 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9089 "bison-chpl-lib.cpp"
    break;

  case 446: /* required_intent_tag: TTYPE  */
#line 2661 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9095 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_this_intent_tag: %empty  */
#line 2665 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9101 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_this_intent_tag: TPARAM  */
#line 2666 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9107 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_this_intent_tag: TREF  */
#line 2667 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9113 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_this_intent_tag: TCONST TREF  */
#line 2668 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9119 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_this_intent_tag: TCONST  */
#line 2669 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9125 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_this_intent_tag: TTYPE  */
#line 2670 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9131 "bison-chpl-lib.cpp"
    break;

  case 453: /* proc_iter_or_op: TPROC  */
#line 2674 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9137 "bison-chpl-lib.cpp"
    break;

  case 454: /* proc_iter_or_op: TITER  */
#line 2675 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9143 "bison-chpl-lib.cpp"
    break;

  case 455: /* proc_iter_or_op: TOPERATOR  */
#line 2676 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9149 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_ret_tag: %empty  */
#line 2680 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9155 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_ret_tag: TOUT  */
#line 2681 "chpl.ypp"
                    { (yyval.returnTag) = Function::OUT; }
#line 9161 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_ret_tag: TCONST  */
#line 2682 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 9167 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_ret_tag: TCONST TREF  */
#line 2683 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 9173 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_ret_tag: TREF  */
#line 2684 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 9179 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_ret_tag: TPARAM  */
#line 2685 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 9185 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_ret_tag: TTYPE  */
#line 2686 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 9191 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_throws_error: %empty  */
#line 2690 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9197 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_throws_error: TTHROWS  */
#line 2691 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9203 "bison-chpl-lib.cpp"
    break;

  case 465: /* opt_function_body_stmt: TSEMI  */
#line 2694 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9209 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_function_body_stmt: function_body_stmt  */
#line 2695 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9215 "bison-chpl-lib.cpp"
    break;

  case 467: /* function_body_stmt: block_stmt_body  */
#line 2699 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9221 "bison-chpl-lib.cpp"
    break;

  case 468: /* function_body_stmt: TDO toplevel_stmt  */
#line 2700 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 9227 "bison-chpl-lib.cpp"
    break;

  case 469: /* function_body_stmt: return_stmt  */
#line 2701 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt), &((yylsp[0]))); }
#line 9233 "bison-chpl-lib.cpp"
    break;

  case 470: /* query_expr: TQUERIEDIDENT  */
#line 2705 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9239 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_lifetime_where: %empty  */
#line 2710 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9245 "bison-chpl-lib.cpp"
    break;

  case 472: /* opt_lifetime_where: TWHERE expr  */
#line 2712 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9251 "bison-chpl-lib.cpp"
    break;

  case 473: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2714 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9257 "bison-chpl-lib.cpp"
    break;

  case 474: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2716 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9263 "bison-chpl-lib.cpp"
    break;

  case 475: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2718 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9269 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_components_expr: lifetime_expr  */
#line 2723 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9275 "bison-chpl-lib.cpp"
    break;

  case 477: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2725 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9281 "bison-chpl-lib.cpp"
    break;

  case 478: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2730 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9287 "bison-chpl-lib.cpp"
    break;

  case 479: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2732 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9293 "bison-chpl-lib.cpp"
    break;

  case 480: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2734 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9299 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2736 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9305 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2738 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9311 "bison-chpl-lib.cpp"
    break;

  case 483: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2740 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9317 "bison-chpl-lib.cpp"
    break;

  case 484: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2742 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9323 "bison-chpl-lib.cpp"
    break;

  case 485: /* lifetime_ident: TIDENT  */
#line 2746 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9329 "bison-chpl-lib.cpp"
    break;

  case 486: /* lifetime_ident: TTHIS  */
#line 2747 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9335 "bison-chpl-lib.cpp"
    break;

  case 487: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2752 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9344 "bison-chpl-lib.cpp"
    break;

  case 488: /* type_alias_decl_stmt_start: TTYPE  */
#line 2761 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9352 "bison-chpl-lib.cpp"
    break;

  case 489: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2765 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9361 "bison-chpl-lib.cpp"
    break;

  case 490: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2773 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9369 "bison-chpl-lib.cpp"
    break;

  case 491: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2777 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9377 "bison-chpl-lib.cpp"
    break;

  case 492: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2784 "chpl.ypp"
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
#line 9399 "bison-chpl-lib.cpp"
    break;

  case 493: /* opt_init_type: %empty  */
#line 2804 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9405 "bison-chpl-lib.cpp"
    break;

  case 494: /* opt_init_type: TASSIGN expr  */
#line 2806 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9411 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_type: TPARAM  */
#line 2810 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9417 "bison-chpl-lib.cpp"
    break;

  case 496: /* var_decl_type: TCONST TREF  */
#line 2811 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9423 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_type: TREF  */
#line 2812 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9429 "bison-chpl-lib.cpp"
    break;

  case 498: /* var_decl_type: TCONST  */
#line 2813 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9435 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_type: TVAR  */
#line 2814 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9441 "bison-chpl-lib.cpp"
    break;

  case 500: /* $@11: %empty  */
#line 2819 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9450 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2823 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9459 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2828 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9468 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2836 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9476 "bison-chpl-lib.cpp"
    break;

  case 504: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2840 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9484 "bison-chpl-lib.cpp"
    break;

  case 505: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2847 "chpl.ypp"
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
#line 9506 "bison-chpl-lib.cpp"
    break;

  case 506: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2865 "chpl.ypp"
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
#line 9526 "bison-chpl-lib.cpp"
    break;

  case 507: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2884 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9534 "bison-chpl-lib.cpp"
    break;

  case 508: /* tuple_var_decl_component: ident_def  */
#line 2888 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9542 "bison-chpl-lib.cpp"
    break;

  case 509: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2892 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9550 "bison-chpl-lib.cpp"
    break;

  case 510: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2899 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9556 "bison-chpl-lib.cpp"
    break;

  case 511: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2901 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9562 "bison-chpl-lib.cpp"
    break;

  case 512: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2903 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9568 "bison-chpl-lib.cpp"
    break;

  case 513: /* opt_init_expr: %empty  */
#line 2909 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9574 "bison-chpl-lib.cpp"
    break;

  case 514: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2910 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9580 "bison-chpl-lib.cpp"
    break;

  case 515: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2911 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9586 "bison-chpl-lib.cpp"
    break;

  case 516: /* formal_or_ret_type_expr: expr  */
#line 2915 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9592 "bison-chpl-lib.cpp"
    break;

  case 517: /* ret_type: formal_or_ret_type_expr  */
#line 2919 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9598 "bison-chpl-lib.cpp"
    break;

  case 518: /* ret_type: reserved_type_ident_use  */
#line 2920 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9604 "bison-chpl-lib.cpp"
    break;

  case 519: /* ret_type: error  */
#line 2921 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9610 "bison-chpl-lib.cpp"
    break;

  case 520: /* colon_ret_type: TCOLON ret_type  */
#line 2925 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9616 "bison-chpl-lib.cpp"
    break;

  case 521: /* colon_ret_type: error  */
#line 2926 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9622 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_ret_type: %empty  */
#line 2930 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9628 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_type: %empty  */
#line 2935 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9634 "bison-chpl-lib.cpp"
    break;

  case 525: /* opt_type: TCOLON expr  */
#line 2936 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9640 "bison-chpl-lib.cpp"
    break;

  case 526: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2937 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9646 "bison-chpl-lib.cpp"
    break;

  case 527: /* opt_type: error  */
#line 2938 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9652 "bison-chpl-lib.cpp"
    break;

  case 528: /* formal_type: formal_or_ret_type_expr  */
#line 2942 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9658 "bison-chpl-lib.cpp"
    break;

  case 529: /* formal_type: reserved_type_ident_use  */
#line 2943 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9664 "bison-chpl-lib.cpp"
    break;

  case 530: /* colon_formal_type: TCOLON formal_type  */
#line 2947 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9670 "bison-chpl-lib.cpp"
    break;

  case 531: /* opt_colon_formal_type: %empty  */
#line 2951 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9676 "bison-chpl-lib.cpp"
    break;

  case 532: /* opt_colon_formal_type: colon_formal_type  */
#line 2952 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9682 "bison-chpl-lib.cpp"
    break;

  case 533: /* expr_ls: expr  */
#line 2958 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9688 "bison-chpl-lib.cpp"
    break;

  case 534: /* expr_ls: expr_ls TCOMMA expr  */
#line 2959 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9694 "bison-chpl-lib.cpp"
    break;

  case 535: /* tuple_component: TUNDERSCORE  */
#line 2963 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9700 "bison-chpl-lib.cpp"
    break;

  case 536: /* tuple_component: opt_try_expr  */
#line 2964 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9706 "bison-chpl-lib.cpp"
    break;

  case 537: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2969 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9714 "bison-chpl-lib.cpp"
    break;

  case 538: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2973 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9722 "bison-chpl-lib.cpp"
    break;

  case 539: /* opt_actual_ls: %empty  */
#line 2979 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9728 "bison-chpl-lib.cpp"
    break;

  case 540: /* opt_actual_ls: actual_ls  */
#line 2980 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9734 "bison-chpl-lib.cpp"
    break;

  case 541: /* actual_ls: actual_expr  */
#line 2985 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9743 "bison-chpl-lib.cpp"
    break;

  case 542: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2990 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9753 "bison-chpl-lib.cpp"
    break;

  case 543: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2998 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9759 "bison-chpl-lib.cpp"
    break;

  case 544: /* actual_expr: opt_try_expr  */
#line 2999 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9765 "bison-chpl-lib.cpp"
    break;

  case 545: /* ident_expr: ident_use  */
#line 3003 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9771 "bison-chpl-lib.cpp"
    break;

  case 546: /* ident_expr: scalar_type  */
#line 3004 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9777 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TSINGLE expr  */
#line 3017 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9783 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3019 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9789 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3021 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9795 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3023 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9801 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 3025 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9811 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TATOMIC expr  */
#line 3031 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9817 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TSYNC expr  */
#line 3033 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9823 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TOWNED  */
#line 3036 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9829 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TOWNED expr  */
#line 3038 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9835 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TUNMANAGED  */
#line 3040 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9841 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3042 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9847 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TSHARED  */
#line 3044 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9853 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TSHARED expr  */
#line 3046 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9859 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TBORROWED  */
#line 3048 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9865 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TBORROWED expr  */
#line 3050 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9871 "bison-chpl-lib.cpp"
    break;

  case 567: /* sub_type_level_expr: TCLASS  */
#line 3052 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9877 "bison-chpl-lib.cpp"
    break;

  case 568: /* sub_type_level_expr: TRECORD  */
#line 3054 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9883 "bison-chpl-lib.cpp"
    break;

  case 569: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 3059 "chpl.ypp"
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
#line 9898 "bison-chpl-lib.cpp"
    break;

  case 570: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 3070 "chpl.ypp"
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
#line 9913 "bison-chpl-lib.cpp"
    break;

  case 571: /* for_expr: TFOR expr TDO expr  */
#line 3081 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->buildAttributeGroup((yyloc))).release();
                    context->resetAttributeGroupPartsState();
  }
#line 9927 "bison-chpl-lib.cpp"
    break;

  case 572: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 3091 "chpl.ypp"
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
#line 9947 "bison-chpl-lib.cpp"
    break;

  case 573: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3107 "chpl.ypp"
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
#line 9967 "bison-chpl-lib.cpp"
    break;

  case 574: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 3123 "chpl.ypp"
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
#line 9987 "bison-chpl-lib.cpp"
    break;

  case 575: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 3139 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10000 "bison-chpl-lib.cpp"
    break;

  case 576: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3148 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10013 "bison-chpl-lib.cpp"
    break;

  case 577: /* for_expr: TFORALL expr TDO expr  */
#line 3157 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10025 "bison-chpl-lib.cpp"
    break;

  case 578: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3165 "chpl.ypp"
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
#line 10043 "bison-chpl-lib.cpp"
    break;

  case 579: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3179 "chpl.ypp"
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
#line 10061 "bison-chpl-lib.cpp"
    break;

  case 580: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3193 "chpl.ypp"
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
#line 10079 "bison-chpl-lib.cpp"
    break;

  case 581: /* bracket_loop_expr: TLSBR TRSBR  */
#line 3210 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 10087 "bison-chpl-lib.cpp"
    break;

  case 582: /* bracket_loop_expr: TLSBR TRSBR expr  */
#line 3214 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 10095 "bison-chpl-lib.cpp"
    break;

  case 583: /* bracket_loop_expr: TLSBR expr_ls TRSBR expr  */
#line 3218 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10103 "bison-chpl-lib.cpp"
    break;

  case 584: /* bracket_loop_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3222 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10111 "bison-chpl-lib.cpp"
    break;

  case 585: /* bracket_loop_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3226 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10119 "bison-chpl-lib.cpp"
    break;

  case 586: /* bracket_loop_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3230 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10127 "bison-chpl-lib.cpp"
    break;

  case 587: /* bracket_loop_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3234 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10135 "bison-chpl-lib.cpp"
    break;

  case 588: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3241 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10149 "bison-chpl-lib.cpp"
    break;

  case 589: /* nil_expr: TNIL  */
#line 3258 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10155 "bison-chpl-lib.cpp"
    break;

  case 597: /* opt_task_intent_ls: %empty  */
#line 3276 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10161 "bison-chpl-lib.cpp"
    break;

  case 598: /* opt_task_intent_ls: task_intent_clause  */
#line 3277 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10167 "bison-chpl-lib.cpp"
    break;

  case 599: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3282 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10177 "bison-chpl-lib.cpp"
    break;

  case 600: /* task_intent_ls: intent_expr  */
#line 3290 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10183 "bison-chpl-lib.cpp"
    break;

  case 601: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3291 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10189 "bison-chpl-lib.cpp"
    break;

  case 602: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3296 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10199 "bison-chpl-lib.cpp"
    break;

  case 603: /* forall_intent_ls: intent_expr  */
#line 3304 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10205 "bison-chpl-lib.cpp"
    break;

  case 604: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3305 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10211 "bison-chpl-lib.cpp"
    break;

  case 605: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3310 "chpl.ypp"
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
#line 10229 "bison-chpl-lib.cpp"
    break;

  case 606: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3324 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10237 "bison-chpl-lib.cpp"
    break;

  case 607: /* intent_expr: expr TREDUCE ident_expr  */
#line 3328 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10245 "bison-chpl-lib.cpp"
    break;

  case 608: /* task_var_prefix: TCONST  */
#line 3334 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10251 "bison-chpl-lib.cpp"
    break;

  case 609: /* task_var_prefix: TIN  */
#line 3335 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10257 "bison-chpl-lib.cpp"
    break;

  case 610: /* task_var_prefix: TCONST TIN  */
#line 3336 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10263 "bison-chpl-lib.cpp"
    break;

  case 611: /* task_var_prefix: TREF  */
#line 3337 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10269 "bison-chpl-lib.cpp"
    break;

  case 612: /* task_var_prefix: TCONST TREF  */
#line 3338 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10275 "bison-chpl-lib.cpp"
    break;

  case 613: /* task_var_prefix: TVAR  */
#line 3339 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10281 "bison-chpl-lib.cpp"
    break;

  case 614: /* new_maybe_decorated: TNEW  */
#line 3344 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10287 "bison-chpl-lib.cpp"
    break;

  case 615: /* new_maybe_decorated: TNEW TOWNED  */
#line 3346 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10293 "bison-chpl-lib.cpp"
    break;

  case 616: /* new_maybe_decorated: TNEW TSHARED  */
#line 3348 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10299 "bison-chpl-lib.cpp"
    break;

  case 617: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3350 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10305 "bison-chpl-lib.cpp"
    break;

  case 618: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3352 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10311 "bison-chpl-lib.cpp"
    break;

  case 619: /* new_expr: new_maybe_decorated expr  */
#line 3358 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10319 "bison-chpl-lib.cpp"
    break;

  case 620: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3365 "chpl.ypp"
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
#line 10335 "bison-chpl-lib.cpp"
    break;

  case 621: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3377 "chpl.ypp"
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
#line 10351 "bison-chpl-lib.cpp"
    break;

  case 622: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
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
    (yyval.expr) = context->buildNewExpr((yyloc), New::OWNED, call.release());
  }
#line 10368 "bison-chpl-lib.cpp"
    break;

  case 623: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3402 "chpl.ypp"
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
#line 10386 "bison-chpl-lib.cpp"
    break;

  case 624: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3419 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10394 "bison-chpl-lib.cpp"
    break;

  case 625: /* range_literal_expr: expr TDOTDOT expr  */
#line 3426 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10403 "bison-chpl-lib.cpp"
    break;

  case 626: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3431 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10412 "bison-chpl-lib.cpp"
    break;

  case 627: /* range_literal_expr: expr TDOTDOT  */
#line 3436 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10421 "bison-chpl-lib.cpp"
    break;

  case 628: /* range_literal_expr: TDOTDOT expr  */
#line 3441 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10431 "bison-chpl-lib.cpp"
    break;

  case 629: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3447 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10441 "bison-chpl-lib.cpp"
    break;

  case 630: /* range_literal_expr: TDOTDOT  */
#line 3453 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10451 "bison-chpl-lib.cpp"
    break;

  case 631: /* cast_expr: expr TCOLON expr  */
#line 3483 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10459 "bison-chpl-lib.cpp"
    break;

  case 632: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3490 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10467 "bison-chpl-lib.cpp"
    break;

  case 633: /* super_expr: fn_expr  */
#line 3496 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10473 "bison-chpl-lib.cpp"
    break;

  case 637: /* expr: sub_type_level_expr TQUESTION  */
#line 3505 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10479 "bison-chpl-lib.cpp"
    break;

  case 638: /* expr: TQUESTION  */
#line 3507 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10485 "bison-chpl-lib.cpp"
    break;

  case 642: /* expr: fn_type  */
#line 3512 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10491 "bison-chpl-lib.cpp"
    break;

  case 652: /* opt_expr: %empty  */
#line 3526 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10497 "bison-chpl-lib.cpp"
    break;

  case 653: /* opt_expr: expr  */
#line 3527 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10503 "bison-chpl-lib.cpp"
    break;

  case 654: /* opt_try_expr: TTRY expr  */
#line 3531 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10509 "bison-chpl-lib.cpp"
    break;

  case 655: /* opt_try_expr: TTRYBANG expr  */
#line 3532 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10515 "bison-chpl-lib.cpp"
    break;

  case 656: /* opt_try_expr: super_expr  */
#line 3533 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10521 "bison-chpl-lib.cpp"
    break;

  case 662: /* call_base_expr: expr TBANG  */
#line 3550 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10529 "bison-chpl-lib.cpp"
    break;

  case 663: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3553 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10535 "bison-chpl-lib.cpp"
    break;

  case 666: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3560 "chpl.ypp"
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
#line 10551 "bison-chpl-lib.cpp"
    break;

  case 667: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3572 "chpl.ypp"
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
#line 10567 "bison-chpl-lib.cpp"
    break;

  case 668: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3584 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10575 "bison-chpl-lib.cpp"
    break;

  case 669: /* dot_expr: expr TDOT ident_use  */
#line 3591 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10581 "bison-chpl-lib.cpp"
    break;

  case 670: /* dot_expr: expr TDOT TTYPE  */
#line 3593 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10587 "bison-chpl-lib.cpp"
    break;

  case 671: /* dot_expr: expr TDOT TDOMAIN  */
#line 3595 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10593 "bison-chpl-lib.cpp"
    break;

  case 672: /* dot_expr: expr TDOT TLOCALE  */
#line 3597 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10599 "bison-chpl-lib.cpp"
    break;

  case 673: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3599 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10609 "bison-chpl-lib.cpp"
    break;

  case 674: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3605 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10619 "bison-chpl-lib.cpp"
    break;

  case 675: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3617 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10625 "bison-chpl-lib.cpp"
    break;

  case 676: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3619 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10633 "bison-chpl-lib.cpp"
    break;

  case 677: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3623 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10641 "bison-chpl-lib.cpp"
    break;

  case 678: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3627 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10649 "bison-chpl-lib.cpp"
    break;

  case 679: /* bool_literal: TFALSE  */
#line 3633 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10655 "bison-chpl-lib.cpp"
    break;

  case 680: /* bool_literal: TTRUE  */
#line 3634 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10661 "bison-chpl-lib.cpp"
    break;

  case 681: /* str_bytes_literal: STRINGLITERAL  */
#line 3638 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10667 "bison-chpl-lib.cpp"
    break;

  case 682: /* str_bytes_literal: BYTESLITERAL  */
#line 3639 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10673 "bison-chpl-lib.cpp"
    break;

  case 685: /* literal_expr: INTLITERAL  */
#line 3645 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10679 "bison-chpl-lib.cpp"
    break;

  case 686: /* literal_expr: REALLITERAL  */
#line 3646 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10685 "bison-chpl-lib.cpp"
    break;

  case 687: /* literal_expr: IMAGLITERAL  */
#line 3647 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10691 "bison-chpl-lib.cpp"
    break;

  case 688: /* literal_expr: CSTRINGLITERAL  */
#line 3648 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10697 "bison-chpl-lib.cpp"
    break;

  case 689: /* literal_expr: TNONE  */
#line 3649 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10703 "bison-chpl-lib.cpp"
    break;

  case 690: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3651 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10712 "bison-chpl-lib.cpp"
    break;

  case 691: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3656 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10721 "bison-chpl-lib.cpp"
    break;

  case 692: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3661 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10729 "bison-chpl-lib.cpp"
    break;

  case 693: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3665 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
  }
#line 10737 "bison-chpl-lib.cpp"
    break;

  case 694: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3669 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
  }
#line 10745 "bison-chpl-lib.cpp"
    break;

  case 695: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3673 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
  }
#line 10753 "bison-chpl-lib.cpp"
    break;

  case 696: /* assoc_expr_ls: expr TALIAS expr  */
#line 3681 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10762 "bison-chpl-lib.cpp"
    break;

  case 697: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3686 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10772 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TPLUS expr  */
#line 3694 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10778 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TMINUS expr  */
#line 3695 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10784 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TSTAR expr  */
#line 3696 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10790 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TDIVIDE expr  */
#line 3697 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10796 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3698 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10802 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3699 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10808 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TMOD expr  */
#line 3700 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10814 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TEQUAL expr  */
#line 3701 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10820 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3702 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10826 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3703 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10832 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3704 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10838 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TLESS expr  */
#line 3705 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10844 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TGREATER expr  */
#line 3706 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10850 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TBAND expr  */
#line 3707 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10856 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TBOR expr  */
#line 3708 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10862 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TBXOR expr  */
#line 3709 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10868 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TAND expr  */
#line 3710 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10874 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TOR expr  */
#line 3711 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10880 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TEXP expr  */
#line 3712 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10886 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TBY expr  */
#line 3713 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10892 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr TALIGN expr  */
#line 3714 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10898 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr THASH expr  */
#line 3715 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10904 "bison-chpl-lib.cpp"
    break;

  case 720: /* binary_op_expr: expr TDMAPPED expr  */
#line 3716 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10910 "bison-chpl-lib.cpp"
    break;

  case 721: /* unary_op_expr: TPLUS expr  */
#line 3720 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10916 "bison-chpl-lib.cpp"
    break;

  case 722: /* unary_op_expr: TMINUS expr  */
#line 3721 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10922 "bison-chpl-lib.cpp"
    break;

  case 723: /* unary_op_expr: TMINUSMINUS expr  */
#line 3722 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10928 "bison-chpl-lib.cpp"
    break;

  case 724: /* unary_op_expr: TPLUSPLUS expr  */
#line 3723 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10934 "bison-chpl-lib.cpp"
    break;

  case 725: /* unary_op_expr: TBANG expr  */
#line 3724 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10940 "bison-chpl-lib.cpp"
    break;

  case 726: /* unary_op_expr: expr TBANG  */
#line 3725 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10948 "bison-chpl-lib.cpp"
    break;

  case 727: /* unary_op_expr: TBNOT expr  */
#line 3728 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10954 "bison-chpl-lib.cpp"
    break;

  case 728: /* reduce_expr: expr TREDUCE expr  */
#line 3733 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10962 "bison-chpl-lib.cpp"
    break;

  case 729: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3737 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10970 "bison-chpl-lib.cpp"
    break;

  case 730: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3741 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10978 "bison-chpl-lib.cpp"
    break;

  case 731: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3745 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10986 "bison-chpl-lib.cpp"
    break;

  case 732: /* scan_expr: expr TSCAN expr  */
#line 3752 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10994 "bison-chpl-lib.cpp"
    break;

  case 733: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3756 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11002 "bison-chpl-lib.cpp"
    break;

  case 734: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3760 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11010 "bison-chpl-lib.cpp"
    break;

  case 735: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3764 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11018 "bison-chpl-lib.cpp"
    break;


#line 11022 "bison-chpl-lib.cpp"

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
