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
  YYSYMBOL_TINIT = 54,                     /* TINIT  */
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
  YYSYMBOL_TPROCLP = 87,                   /* TPROCLP  */
  YYSYMBOL_TREAL = 88,                     /* TREAL  */
  YYSYMBOL_TRECORD = 89,                   /* TRECORD  */
  YYSYMBOL_TREDUCE = 90,                   /* TREDUCE  */
  YYSYMBOL_TREF = 91,                      /* TREF  */
  YYSYMBOL_TREQUIRE = 92,                  /* TREQUIRE  */
  YYSYMBOL_TRETURN = 93,                   /* TRETURN  */
  YYSYMBOL_TSCAN = 94,                     /* TSCAN  */
  YYSYMBOL_TSELECT = 95,                   /* TSELECT  */
  YYSYMBOL_TSERIAL = 96,                   /* TSERIAL  */
  YYSYMBOL_TSHARED = 97,                   /* TSHARED  */
  YYSYMBOL_TSINGLE = 98,                   /* TSINGLE  */
  YYSYMBOL_TSPARSE = 99,                   /* TSPARSE  */
  YYSYMBOL_TSTRING = 100,                  /* TSTRING  */
  YYSYMBOL_TSUBDOMAIN = 101,               /* TSUBDOMAIN  */
  YYSYMBOL_TSYNC = 102,                    /* TSYNC  */
  YYSYMBOL_TTHEN = 103,                    /* TTHEN  */
  YYSYMBOL_TTHIS = 104,                    /* TTHIS  */
  YYSYMBOL_TTHROW = 105,                   /* TTHROW  */
  YYSYMBOL_TTHROWS = 106,                  /* TTHROWS  */
  YYSYMBOL_TTRUE = 107,                    /* TTRUE  */
  YYSYMBOL_TTRY = 108,                     /* TTRY  */
  YYSYMBOL_TTRYBANG = 109,                 /* TTRYBANG  */
  YYSYMBOL_TTYPE = 110,                    /* TTYPE  */
  YYSYMBOL_TUINT = 111,                    /* TUINT  */
  YYSYMBOL_TUNDERSCORE = 112,              /* TUNDERSCORE  */
  YYSYMBOL_TUNION = 113,                   /* TUNION  */
  YYSYMBOL_TUNMANAGED = 114,               /* TUNMANAGED  */
  YYSYMBOL_TUSE = 115,                     /* TUSE  */
  YYSYMBOL_TVAR = 116,                     /* TVAR  */
  YYSYMBOL_TVOID = 117,                    /* TVOID  */
  YYSYMBOL_TWHEN = 118,                    /* TWHEN  */
  YYSYMBOL_TWHERE = 119,                   /* TWHERE  */
  YYSYMBOL_TWHILE = 120,                   /* TWHILE  */
  YYSYMBOL_TWITH = 121,                    /* TWITH  */
  YYSYMBOL_TYIELD = 122,                   /* TYIELD  */
  YYSYMBOL_TZIP = 123,                     /* TZIP  */
  YYSYMBOL_TALIAS = 124,                   /* TALIAS  */
  YYSYMBOL_TAND = 125,                     /* TAND  */
  YYSYMBOL_TASSIGN = 126,                  /* TASSIGN  */
  YYSYMBOL_TASSIGNBAND = 127,              /* TASSIGNBAND  */
  YYSYMBOL_TASSIGNBOR = 128,               /* TASSIGNBOR  */
  YYSYMBOL_TASSIGNBXOR = 129,              /* TASSIGNBXOR  */
  YYSYMBOL_TASSIGNDIVIDE = 130,            /* TASSIGNDIVIDE  */
  YYSYMBOL_TASSIGNEXP = 131,               /* TASSIGNEXP  */
  YYSYMBOL_TASSIGNLAND = 132,              /* TASSIGNLAND  */
  YYSYMBOL_TASSIGNLOR = 133,               /* TASSIGNLOR  */
  YYSYMBOL_TASSIGNMINUS = 134,             /* TASSIGNMINUS  */
  YYSYMBOL_TASSIGNMOD = 135,               /* TASSIGNMOD  */
  YYSYMBOL_TASSIGNMULTIPLY = 136,          /* TASSIGNMULTIPLY  */
  YYSYMBOL_TASSIGNPLUS = 137,              /* TASSIGNPLUS  */
  YYSYMBOL_TASSIGNREDUCE = 138,            /* TASSIGNREDUCE  */
  YYSYMBOL_TASSIGNSL = 139,                /* TASSIGNSL  */
  YYSYMBOL_TASSIGNSR = 140,                /* TASSIGNSR  */
  YYSYMBOL_TATMARK = 141,                  /* TATMARK  */
  YYSYMBOL_TBANG = 142,                    /* TBANG  */
  YYSYMBOL_TBAND = 143,                    /* TBAND  */
  YYSYMBOL_TBNOT = 144,                    /* TBNOT  */
  YYSYMBOL_TBOR = 145,                     /* TBOR  */
  YYSYMBOL_TBXOR = 146,                    /* TBXOR  */
  YYSYMBOL_TCOLON = 147,                   /* TCOLON  */
  YYSYMBOL_TCOMMA = 148,                   /* TCOMMA  */
  YYSYMBOL_TDIVIDE = 149,                  /* TDIVIDE  */
  YYSYMBOL_TDOT = 150,                     /* TDOT  */
  YYSYMBOL_TDOTDOT = 151,                  /* TDOTDOT  */
  YYSYMBOL_TDOTDOTDOT = 152,               /* TDOTDOTDOT  */
  YYSYMBOL_TEQUAL = 153,                   /* TEQUAL  */
  YYSYMBOL_TEXP = 154,                     /* TEXP  */
  YYSYMBOL_TGREATER = 155,                 /* TGREATER  */
  YYSYMBOL_TGREATEREQUAL = 156,            /* TGREATEREQUAL  */
  YYSYMBOL_THASH = 157,                    /* THASH  */
  YYSYMBOL_TLESS = 158,                    /* TLESS  */
  YYSYMBOL_TLESSEQUAL = 159,               /* TLESSEQUAL  */
  YYSYMBOL_TMINUS = 160,                   /* TMINUS  */
  YYSYMBOL_TMOD = 161,                     /* TMOD  */
  YYSYMBOL_TNOTEQUAL = 162,                /* TNOTEQUAL  */
  YYSYMBOL_TOR = 163,                      /* TOR  */
  YYSYMBOL_TPLUS = 164,                    /* TPLUS  */
  YYSYMBOL_TQUESTION = 165,                /* TQUESTION  */
  YYSYMBOL_TSEMI = 166,                    /* TSEMI  */
  YYSYMBOL_TSHIFTLEFT = 167,               /* TSHIFTLEFT  */
  YYSYMBOL_TSHIFTRIGHT = 168,              /* TSHIFTRIGHT  */
  YYSYMBOL_TSTAR = 169,                    /* TSTAR  */
  YYSYMBOL_TSWAP = 170,                    /* TSWAP  */
  YYSYMBOL_TLCBR = 171,                    /* TLCBR  */
  YYSYMBOL_TRCBR = 172,                    /* TRCBR  */
  YYSYMBOL_TLP = 173,                      /* TLP  */
  YYSYMBOL_TRP = 174,                      /* TRP  */
  YYSYMBOL_TLSBR = 175,                    /* TLSBR  */
  YYSYMBOL_TRSBR = 176,                    /* TRSBR  */
  YYSYMBOL_TNOELSE = 177,                  /* TNOELSE  */
  YYSYMBOL_TDOTDOTOPENHIGH = 178,          /* TDOTDOTOPENHIGH  */
  YYSYMBOL_TUPLUS = 179,                   /* TUPLUS  */
  YYSYMBOL_TUMINUS = 180,                  /* TUMINUS  */
  YYSYMBOL_TLNOT = 181,                    /* TLNOT  */
  YYSYMBOL_YYACCEPT = 182,                 /* $accept  */
  YYSYMBOL_program = 183,                  /* program  */
  YYSYMBOL_toplevel_stmt_ls = 184,         /* toplevel_stmt_ls  */
  YYSYMBOL_toplevel_stmt = 185,            /* toplevel_stmt  */
  YYSYMBOL_pragma_ls = 186,                /* pragma_ls  */
  YYSYMBOL_stmt = 187,                     /* stmt  */
  YYSYMBOL_stmt_base = 188,                /* stmt_base  */
  YYSYMBOL_tryable_stmt = 189,             /* tryable_stmt  */
  YYSYMBOL_decl_base = 190,                /* decl_base  */
  YYSYMBOL_collect_attributes = 191,       /* collect_attributes  */
  YYSYMBOL_attribute_receiver = 192,       /* attribute_receiver  */
  YYSYMBOL_attribute_group = 193,          /* attribute_group  */
  YYSYMBOL_attribute_decl_stmt_ls = 194,   /* attribute_decl_stmt_ls  */
  YYSYMBOL_attribute_decl_stmt = 195,      /* attribute_decl_stmt  */
  YYSYMBOL_attribute_decl_begin = 196,     /* attribute_decl_begin  */
  YYSYMBOL_opt_attribute_decl_toolspace = 197, /* opt_attribute_decl_toolspace  */
  YYSYMBOL_opt_attribute_actuals = 198,    /* opt_attribute_actuals  */
  YYSYMBOL_attribute_actuals_ls = 199,     /* attribute_actuals_ls  */
  YYSYMBOL_attribute_actual = 200,         /* attribute_actual  */
  YYSYMBOL_module_decl_start = 201,        /* module_decl_start  */
  YYSYMBOL_module_decl_stmt = 202,         /* module_decl_stmt  */
  YYSYMBOL_opt_access_control = 203,       /* opt_access_control  */
  YYSYMBOL_opt_prototype = 204,            /* opt_prototype  */
  YYSYMBOL_include_module_stmt = 205,      /* include_module_stmt  */
  YYSYMBOL_block_stmt_body = 206,          /* block_stmt_body  */
  YYSYMBOL_block_stmt = 207,               /* block_stmt  */
  YYSYMBOL_stmt_ls = 208,                  /* stmt_ls  */
  YYSYMBOL_renames_ls = 209,               /* renames_ls  */
  YYSYMBOL_use_renames_ls = 210,           /* use_renames_ls  */
  YYSYMBOL_opt_only_ls = 211,              /* opt_only_ls  */
  YYSYMBOL_use_stmt = 212,                 /* use_stmt  */
  YYSYMBOL_import_stmt = 213,              /* import_stmt  */
  YYSYMBOL_import_internal_type_ident = 214, /* import_internal_type_ident  */
  YYSYMBOL_import_expr = 215,              /* import_expr  */
  YYSYMBOL_import_ls = 216,                /* import_ls  */
  YYSYMBOL_require_stmt = 217,             /* require_stmt  */
  YYSYMBOL_assignment_stmt = 218,          /* assignment_stmt  */
  YYSYMBOL_opt_label_ident = 219,          /* opt_label_ident  */
  YYSYMBOL_reserved_word_ident = 220,      /* reserved_word_ident  */
  YYSYMBOL_ident_def = 221,                /* ident_def  */
  YYSYMBOL_ident_use = 222,                /* ident_use  */
  YYSYMBOL_internal_type_ident_def = 223,  /* internal_type_ident_def  */
  YYSYMBOL_scalar_type = 224,              /* scalar_type  */
  YYSYMBOL_reserved_type_ident_use = 225,  /* reserved_type_ident_use  */
  YYSYMBOL_do_stmt = 226,                  /* do_stmt  */
  YYSYMBOL_return_stmt = 227,              /* return_stmt  */
  YYSYMBOL_class_level_stmt = 228,         /* class_level_stmt  */
  YYSYMBOL_229_1 = 229,                    /* $@1  */
  YYSYMBOL_230_2 = 230,                    /* $@2  */
  YYSYMBOL_inner_class_level_stmt = 231,   /* inner_class_level_stmt  */
  YYSYMBOL_forwarding_decl_stmt = 232,     /* forwarding_decl_stmt  */
  YYSYMBOL_forwarding_decl_start = 233,    /* forwarding_decl_start  */
  YYSYMBOL_extern_or_export = 234,         /* extern_or_export  */
  YYSYMBOL_extern_export_decl_stmt_start = 235, /* extern_export_decl_stmt_start  */
  YYSYMBOL_extern_export_decl_stmt = 236,  /* extern_export_decl_stmt  */
  YYSYMBOL_237_3 = 237,                    /* $@3  */
  YYSYMBOL_238_4 = 238,                    /* $@4  */
  YYSYMBOL_extern_block_stmt = 239,        /* extern_block_stmt  */
  YYSYMBOL_no_loop_attributes = 240,       /* no_loop_attributes  */
  YYSYMBOL_loop_stmt = 241,                /* loop_stmt  */
  YYSYMBOL_for_loop_kw = 242,              /* for_loop_kw  */
  YYSYMBOL_loop_stmt_base = 243,           /* loop_stmt_base  */
  YYSYMBOL_zippered_iterator = 244,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 245,                  /* if_stmt  */
  YYSYMBOL_ifvar = 246,                    /* ifvar  */
  YYSYMBOL_interface_stmt = 247,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 248,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 249,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 250,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 251, /* implements_type_error_ident  */
  YYSYMBOL_implements_stmt = 252,          /* implements_stmt  */
  YYSYMBOL_ifc_constraint = 253,           /* ifc_constraint  */
  YYSYMBOL_try_stmt = 254,                 /* try_stmt  */
  YYSYMBOL_catch_expr_ls = 255,            /* catch_expr_ls  */
  YYSYMBOL_catch_expr = 256,               /* catch_expr  */
  YYSYMBOL_catch_expr_inner = 257,         /* catch_expr_inner  */
  YYSYMBOL_throw_stmt = 258,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 259,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 260,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 261,                /* when_stmt  */
  YYSYMBOL_manager_expr = 262,             /* manager_expr  */
  YYSYMBOL_manager_expr_ls = 263,          /* manager_expr_ls  */
  YYSYMBOL_manage_stmt = 264,              /* manage_stmt  */
  YYSYMBOL_class_decl_stmt = 265,          /* class_decl_stmt  */
  YYSYMBOL_class_start = 266,              /* class_start  */
  YYSYMBOL_class_tag = 267,                /* class_tag  */
  YYSYMBOL_opt_inherit = 268,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 269,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 270,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header_lcbr = 271,         /* enum_header_lcbr  */
  YYSYMBOL_enum_ls = 272,                  /* enum_ls  */
  YYSYMBOL_273_5 = 273,                    /* $@5  */
  YYSYMBOL_274_6 = 274,                    /* $@6  */
  YYSYMBOL_enum_item = 275,                /* enum_item  */
  YYSYMBOL_lambda_decl_start = 276,        /* lambda_decl_start  */
  YYSYMBOL_lambda_decl_expr = 277,         /* lambda_decl_expr  */
  YYSYMBOL_278_7 = 278,                    /* $@7  */
  YYSYMBOL_linkage_spec_empty = 279,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 280,             /* linkage_spec  */
  YYSYMBOL_opt_fn_type_formal_ls = 281,    /* opt_fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal_ls = 282,        /* fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal = 283,           /* fn_type_formal  */
  YYSYMBOL_opt_fn_type_ret_type = 284,     /* opt_fn_type_ret_type  */
  YYSYMBOL_fn_type = 285,                  /* fn_type  */
  YYSYMBOL_fn_expr = 286,                  /* fn_expr  */
  YYSYMBOL_287_8 = 287,                    /* $@8  */
  YYSYMBOL_288_9 = 288,                    /* $@9  */
  YYSYMBOL_fn_decl_stmt_complete = 289,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 290,             /* fn_decl_stmt  */
  YYSYMBOL_291_10 = 291,                   /* $@10  */
  YYSYMBOL_fn_decl_stmt_inner = 292,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 293,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 294,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 295,                 /* fn_ident  */
  YYSYMBOL_op_ident = 296,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 297,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 298,              /* all_op_name  */
  YYSYMBOL_formal_var_arg_expr = 299,      /* formal_var_arg_expr  */
  YYSYMBOL_opt_formal_ls = 300,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 301,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 302,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 303,                /* formal_ls  */
  YYSYMBOL_formal = 304,                   /* formal  */
  YYSYMBOL_formal_ident_def = 305,         /* formal_ident_def  */
  YYSYMBOL_named_formal = 306,             /* named_formal  */
  YYSYMBOL_opt_formal_intent_tag = 307,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 308,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 309,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 310,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 311,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 312,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 313,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 314,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 315,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 316,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 317, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 318,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 319,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 320,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 321, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 322, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 323, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 324,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 325,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 326,            /* var_decl_stmt  */
  YYSYMBOL_327_11 = 327,                   /* $@11  */
  YYSYMBOL_var_decl_stmt_inner_ls = 328,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 329,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 330, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 331, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 332,            /* opt_init_expr  */
  YYSYMBOL_formal_or_ret_type_expr = 333,  /* formal_or_ret_type_expr  */
  YYSYMBOL_ret_type = 334,                 /* ret_type  */
  YYSYMBOL_colon_ret_type = 335,           /* colon_ret_type  */
  YYSYMBOL_opt_ret_type = 336,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 337,                 /* opt_type  */
  YYSYMBOL_formal_type = 338,              /* formal_type  */
  YYSYMBOL_colon_formal_type = 339,        /* colon_formal_type  */
  YYSYMBOL_opt_colon_formal_type = 340,    /* opt_colon_formal_type  */
  YYSYMBOL_expr_ls = 341,                  /* expr_ls  */
  YYSYMBOL_tuple_component = 342,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 343,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 344,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 345,                /* actual_ls  */
  YYSYMBOL_actual_expr = 346,              /* actual_expr  */
  YYSYMBOL_ident_expr = 347,               /* ident_expr  */
  YYSYMBOL_sub_type_level_expr = 348,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 349,                 /* for_expr  */
  YYSYMBOL_for_expr_base = 350,            /* for_expr_base  */
  YYSYMBOL_bracket_loop_expr = 351,        /* bracket_loop_expr  */
  YYSYMBOL_bracket_loop_expr_base = 352,   /* bracket_loop_expr_base  */
  YYSYMBOL_cond_expr = 353,                /* cond_expr  */
  YYSYMBOL_nil_expr = 354,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 355,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 356,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 357,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 358,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 359,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 360,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 361,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 362,          /* task_var_prefix  */
  YYSYMBOL_new_maybe_decorated = 363,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 364,                 /* new_expr  */
  YYSYMBOL_let_expr = 365,                 /* let_expr  */
  YYSYMBOL_range_literal_expr = 366,       /* range_literal_expr  */
  YYSYMBOL_cast_expr = 367,                /* cast_expr  */
  YYSYMBOL_tuple_expand_expr = 368,        /* tuple_expand_expr  */
  YYSYMBOL_super_expr = 369,               /* super_expr  */
  YYSYMBOL_expr = 370,                     /* expr  */
  YYSYMBOL_opt_expr = 371,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 372,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 373,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 374,           /* call_base_expr  */
  YYSYMBOL_call_expr = 375,                /* call_expr  */
  YYSYMBOL_dot_expr = 376,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 377,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 378,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 379,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 380,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 381,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 382,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 383,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 384,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 385,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 386       /* reduce_scan_op_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 325 "chpl.ypp"

  // include the definition of methods for ParserContext
  #include "ParserContextImpl.h"
  // include helper macros for use in writing productions
  #include "parser-help.h"
  // include implementation of error logging hook
  #include "parser-yyerror.h"

#line 510 "bison-chpl-lib.cpp"

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
#define YYLAST   18460

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  182
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  205
/* YYNRULES -- Number of rules.  */
#define YYNRULES  735
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1264

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   436


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
     175,   176,   177,   178,   179,   180,   181
};

#if YYCHPL_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   634,   634,   638,   639,   644,   645,   653,   657,   667,
     672,   679,   680,   681,   682,   683,   684,   685,   686,   687,
     688,   689,   702,   703,   704,   712,   721,   730,   734,   741,
     749,   750,   751,   752,   753,   754,   755,   756,   758,   773,
     781,   793,   804,   816,   828,   839,   855,   856,   860,   870,
     871,   875,   879,   880,   884,   891,   899,   902,   912,   914,
     919,   925,   930,   939,   943,   963,   978,   993,  1009,  1010,
    1012,  1017,  1018,  1023,  1041,  1045,  1049,  1060,  1079,  1080,
    1084,  1088,  1092,  1097,  1101,  1105,  1115,  1120,  1126,  1133,
    1138,  1145,  1156,  1157,  1161,  1166,  1174,  1183,  1193,  1201,
    1210,  1223,  1231,  1232,  1233,  1234,  1235,  1236,  1237,  1238,
    1239,  1240,  1241,  1242,  1243,  1244,  1245,  1246,  1247,  1259,
    1263,  1268,  1273,  1279,  1288,  1289,  1293,  1302,  1306,  1310,
    1314,  1318,  1322,  1331,  1332,  1333,  1337,  1338,  1339,  1340,
    1341,  1345,  1346,  1347,  1359,  1360,  1361,  1366,  1367,  1368,
    1369,  1370,  1371,  1372,  1373,  1374,  1375,  1376,  1377,  1378,
    1379,  1380,  1381,  1382,  1383,  1384,  1388,  1389,  1390,  1391,
    1392,  1393,  1394,  1395,  1396,  1397,  1398,  1399,  1406,  1407,
    1408,  1409,  1413,  1414,  1418,  1425,  1435,  1439,  1445,  1445,
    1451,  1451,  1460,  1461,  1462,  1463,  1464,  1465,  1466,  1470,
    1475,  1480,  1485,  1492,  1500,  1501,  1505,  1514,  1520,  1528,
    1547,  1546,  1556,  1555,  1568,  1574,  1577,  1578,  1587,  1588,
    1589,  1593,  1612,  1626,  1640,  1644,  1648,  1652,  1672,  1676,
    1680,  1684,  1688,  1692,  1696,  1700,  1704,  1708,  1712,  1716,
    1720,  1724,  1728,  1733,  1741,  1750,  1754,  1758,  1762,  1766,
    1770,  1774,  1778,  1782,  1787,  1792,  1797,  1805,  1820,  1838,
    1842,  1849,  1850,  1855,  1860,  1861,  1862,  1863,  1864,  1865,
    1866,  1867,  1868,  1869,  1870,  1871,  1876,  1881,  1882,  1883,
    1884,  1893,  1894,  1898,  1902,  1906,  1913,  1917,  1921,  1928,
    1932,  1936,  1940,  1947,  1948,  1952,  1956,  1960,  1967,  1980,
    1996,  2004,  2008,  2017,  2018,  2022,  2026,  2031,  2040,  2045,
    2049,  2056,  2057,  2061,  2070,  2075,  2086,  2093,  2094,  2095,
    2099,  2100,  2104,  2108,  2112,  2116,  2120,  2128,  2146,  2159,
    2166,  2171,  2178,  2177,  2187,  2193,  2192,  2207,  2214,  2226,
    2244,  2241,  2269,  2273,  2274,  2276,  2281,  2282,  2286,  2287,
    2291,  2294,  2296,  2301,  2302,  2313,  2333,  2332,  2348,  2347,
    2365,  2375,  2372,  2394,  2402,  2410,  2421,  2432,  2441,  2456,
    2457,  2461,  2462,  2463,  2472,  2473,  2474,  2475,  2476,  2477,
    2478,  2479,  2480,  2481,  2482,  2483,  2484,  2485,  2486,  2487,
    2488,  2489,  2490,  2491,  2492,  2493,  2494,  2495,  2496,  2500,
    2501,  2502,  2503,  2504,  2505,  2506,  2507,  2508,  2509,  2510,
    2511,  2516,  2517,  2521,  2522,  2526,  2527,  2531,  2532,  2536,
    2537,  2541,  2542,  2546,  2550,  2551,  2555,  2559,  2564,  2569,
    2574,  2579,  2587,  2591,  2599,  2600,  2601,  2602,  2603,  2604,
    2605,  2606,  2607,  2611,  2612,  2613,  2614,  2615,  2616,  2620,
    2621,  2622,  2626,  2627,  2628,  2629,  2630,  2631,  2632,  2636,
    2637,  2640,  2641,  2645,  2646,  2650,  2655,  2656,  2658,  2660,
    2662,  2667,  2669,  2674,  2676,  2678,  2680,  2682,  2684,  2686,
    2691,  2692,  2693,  2697,  2706,  2710,  2718,  2722,  2729,  2750,
    2751,  2756,  2757,  2758,  2759,  2760,  2765,  2764,  2773,  2778,
    2786,  2790,  2797,  2816,  2835,  2839,  2843,  2850,  2852,  2854,
    2861,  2862,  2863,  2867,  2871,  2872,  2873,  2877,  2878,  2882,
    2883,  2887,  2888,  2889,  2890,  2894,  2895,  2899,  2903,  2904,
    2910,  2911,  2915,  2916,  2920,  2924,  2931,  2932,  2936,  2941,
    2950,  2951,  2955,  2956,  2963,  2964,  2965,  2966,  2967,  2968,
    2970,  2972,  2974,  2976,  2982,  2984,  2987,  2989,  2991,  2993,
    2995,  2997,  2999,  3001,  3003,  3005,  3010,  3014,  3018,  3022,
    3026,  3035,  3044,  3056,  3060,  3064,  3068,  3072,  3076,  3080,
    3084,  3091,  3109,  3117,  3118,  3119,  3120,  3121,  3122,  3123,
    3127,  3128,  3132,  3139,  3140,  3144,  3153,  3154,  3158,  3172,
    3176,  3183,  3184,  3185,  3186,  3187,  3188,  3192,  3194,  3196,
    3198,  3200,  3206,  3213,  3225,  3237,  3250,  3267,  3274,  3279,
    3284,  3289,  3295,  3301,  3331,  3338,  3345,  3346,  3350,  3352,
    3353,  3355,  3357,  3358,  3359,  3360,  3363,  3364,  3365,  3366,
    3367,  3368,  3369,  3370,  3371,  3375,  3376,  3380,  3381,  3382,
    3386,  3387,  3388,  3389,  3398,  3399,  3402,  3403,  3404,  3408,
    3420,  3432,  3439,  3441,  3443,  3445,  3447,  3449,  3458,  3459,
    3463,  3467,  3474,  3475,  3479,  3480,  3484,  3485,  3486,  3487,
    3488,  3489,  3490,  3491,  3496,  3501,  3506,  3511,  3516,  3525,
    3530,  3539,  3540,  3541,  3542,  3543,  3544,  3545,  3546,  3547,
    3548,  3549,  3550,  3551,  3552,  3553,  3554,  3555,  3556,  3557,
    3558,  3559,  3560,  3561,  3565,  3566,  3567,  3568,  3569,  3570,
    3573,  3577,  3581,  3585,  3589,  3596,  3600,  3604,  3608,  3616,
    3617,  3618,  3619,  3620,  3621,  3622
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
  "TINIT", "TINITEQUALS", "TIMPLEMENTS", "TINTERFACE", "TLABEL", "TLAMBDA",
  "TLET", "TLIFETIME", "TLOCAL", "TLOCALE", "TMANAGE", "TMINUSMINUS",
  "TMODULE", "TNEW", "TNIL", "TNOINIT", "TNONE", "TNOTHING", "TON",
  "TONLY", "TOPERATOR", "TOTHERWISE", "TOUT", "TOVERRIDE", "TOWNED",
  "TPARAM", "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC",
  "TPROTOTYPE", "TPUBLIC", "TPROCLP", "TREAL", "TRECORD", "TREDUCE",
  "TREF", "TREQUIRE", "TRETURN", "TSCAN", "TSELECT", "TSERIAL", "TSHARED",
  "TSINGLE", "TSPARSE", "TSTRING", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS",
  "TTHROW", "TTHROWS", "TTRUE", "TTRY", "TTRYBANG", "TTYPE", "TUINT",
  "TUNDERSCORE", "TUNION", "TUNMANAGED", "TUSE", "TVAR", "TVOID", "TWHEN",
  "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND",
  "TASSIGN", "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE",
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
  "extern_block_stmt", "no_loop_attributes", "loop_stmt", "for_loop_kw",
  "loop_stmt_base", "zippered_iterator", "if_stmt", "ifvar",
  "interface_stmt", "ifc_formal_ls", "ifc_formal", "implements_type_ident",
  "implements_type_error_ident", "implements_stmt", "ifc_constraint",
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

#define YYPACT_NINF (-871)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-736)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -871,    88,  3916,  -871,   -38,   120,  -871,  -871,  -871,  -871,
    -871,  -871,  -871, 12188,    64,   241,   154, 14048,   235, 18231,
      64,   240,   166,    46,   241,  5324, 12188,   138, 18287,  -871,
     305,   288,  -871,  9900,   324,   273,   219,  -871,   346,   321,
   18287, 18287, 18287,  -871, 14272, 10780,   385, 12188, 12188,   216,
    -871,   388,   420, 12188,  -871, 14048,  -871, 12188,   492,   296,
     311,   428, 13392,   483, 18343,  -871, 12188,  8492, 12188, 10780,
   14048, 12188,   475,   541,   440,  5324,   560, 12188,   565,  6908,
    6908,  -871,   570,  -871, 14048,  -871,   574, 10076,  -871,   635,
   12188,  -871, 12188,  -871,  -871, 13875, 12188,  -871, 10252,  -871,
    -871,  -871,  4268,  7612, 12188,  -871,  4972,  -871,  -871,  -871,
    -871,   709,   516,  -871,   491,   488,  -871,   134,  -871,  -871,
    -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,
    -871,  7084,  -871,  7260,  -871,  -871,    27,  -871,  -871,  -871,
     606,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,   525,
   18287,  -871, 17975,   501,   397,  -871,   262,  -871,  -871,  -871,
     250,   159,  -871,  -871, 18287,  1843,  -871,   499,   517,  -871,
    -871,  -871,   519,   523, 12188,   526,   530,  -871,  -871,  -871,
   17546,  1844,   353,   534,   535,  -871,  -871,   367,  -871,  -871,
    -871,  -871,  -871,   484,  -871,  -871,  -871, 12188,  -871, 18287,
    -871, 12188, 12188,   118,   625,   413,  -871,  -871,  -871,  -871,
   17546,   416,  -871,  -871,   510,  5324,  -871,  -871,  -871,   536,
     339,   533,  -871,   326,  -871,   539,  -871,   -68, 17546,  8668,
    -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,
    -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,
    -871,  -871,  -871,  -871,  -871,  -871,   538,  -871,  -871, 18287,
   18287,   -33, 14617,  -871,  -871,   601,  8668,   540,   537,   425,
    5324,  2796,    98,    -5,  -871,  5324,  -871,  -871, 15599,  -871,
      16, 16005,   220,  -871,   544,   545,  -871, 15599,   339,   220,
    -871,  8668,   977,   977,    70,     8,  -871,    59,  -871,  -871,
    -871,  -871,  7788,  7788,  -871,   597,  -871,   546,   563,  -871,
    -871,  2894,   566,  -871,  -871, 17546,   -65, 12188, 12188,  -871,
     -17,  -871,  -871, 17546,   548, 16046,  -871, 15599,   339, 17546,
     552,  8668,  -871, 17546, 16122,  -871,  -871,  -871,  -871,  -871,
     339,   555,  -871,   136,   136,   500,   220,   220,    91,  -871,
    -871,  4444,   176,  -871, 12188,   114,   149,  -871,   500,   704,
    -871,  -871, 18287,  -871,  -871,  4268,  -871,    52,  -871,   601,
    -871,  -871,   723,   557,  4620, 12188,  -871, 12188,   662,  -871,
    -871, 15270,    35,   525, 17546,   613, 12188,  5324,   653,  -871,
    -871,  9900, 10428, 10956,  -871,  -871,  -871, 18287, 12188,   562,
    -871,   569, 18063,   608,   239,  -871,    55,   250,  -871,  -871,
    -871,  -871,   646,  -871,  -871,  -871,  -871,    49,   647,  -871,
    -871,  -871, 13704,   617,   -15,  -871,   579,   286,   452,  -871,
     598, 12188, 12188, 12188, 10956, 10956, 12188,   461, 12188, 12188,
   12188, 12188, 12188,   373, 13875, 12188, 12188, 12188, 12188, 12188,
   12188, 12188, 12188, 12188, 12188, 12188, 12188, 12188, 12188, 12188,
   12188,   680,  -871,  -871,  -871,  -871,  -871, 10076, 10076,  -871,
    -871,  -871,  -871, 10076,  -871,  -871, 10076, 10076,  8668,  8844,
   10956, 10956, 16167,   577,  -871, 11132, 12188, 18287,  7964,  -871,
    -871,  -871, 14272,  -871, 12188,  -871,   627,   583,   612,  -871,
    -871,  -871,   638,   639,  5324,   727,  5324,  -871,   733, 12188,
     702,   595,  -871, 10076, 18287,  -871,  -871,  -871,  2796,  -871,
    -871,   160,  -871, 12364,   644, 12188, 14272,  -871,  -871, 12188,
    -871, 18119, 12188, 12188,  -871,   599,  -871,  -871,  -871,  -871,
   18175,  -871,   250, 13567,  -871,  2796,  -871,   624, 12364,  -871,
   17546, 17546,  -871,   533,  -871,    39,  -871, 10076,   600,  -871,
     751,   751,  -871,  -871,  -871,  -871, 11308,  -871, 16207,  8140,
    -871,  8316,  -871,  -871, 10956, 10956,  -871,   491,  9020,  -871,
     405,  -871,  4796,  -871,   291, 16328,   332, 15362, 18287,  6732,
    6556,  -871,   525,   605,  -871,   326,  -871, 15684,   657, 18287,
      26, 15599,   607, 14166,   -49,   161, 16374,     2,    23, 15102,
     272,   630,  2154,  -871,  -871, 12188,   171,  -871,  -871,   184,
    -871,  -871,    65,  -871,  -871,  6380,  -871,   673,  -871,   609,
     631,  -871,   634,  -871,   637,   640,   641,   649,   651,  -871,
     654,   655,   658,   659,   664,   667,   671,  -871,  -871,  -871,
    -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,
    -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871, 12188,
     643,   674,   609,  -871,   609,  -871, 12188,  -871, 18287,  -871,
    -871,  -871, 17637, 17637,   481,  -871,   481,  -871,   481,  2354,
    1438,  1165,  1406,   339,   136,  -871,   464,  -871,  -871,  -871,
    -871,  -871,   500, 17780,   481,   304,   304, 17637,   304,   304,
    1132,   136, 17780, 17740,  1132,   220,   220,   136,   500,   623,
     661,   663,   665,   666,   668,   656,   636,  -871,   481,  -871,
     481, 12188, 10076,    85, 15760,   650,   213,  -871,  -871,  -871,
     191,  -871,   793, 17587,   493,   355, 17546, 10076,  -871, 10076,
   12188, 12188,   802,  5324,   803, 16414,  5324,  3195, 18287,  -871,
     204,  -871,   242,  -871,   247,  2796,    98,  -871, 17546,  9196,
    -871, 17546,  -871,  -871,  -871, 18287, 16495, 16535,  -871,   624,
     692,  -871,   255, 12364,  -871,   260,  -871, 12188,  -871,   669,
      45,   670,  -871,  3199,  -871,  -871,  -871,  -871,  -871,  -871,
    -871,   207, 15178,  -871,  -871,   313,  -871,  -871,  -871,  -871,
   12188,  -871,   139, 14375, 12188,  -871, 11484,  6732,  6556,  -871,
    9372,   515,  9548,   518,   520,  9724,  7436,   521,   375,  -871,
    -871,  -871, 16581,   694,   677,   678,  -871, 18287, 14272, 10956,
      26, 12188,   801,  -871,  -871, 12188, 17546,   679,  5324,   675,
   10956, 11660,  4092,   683, 12188, 11836,  -871,  -871,    26,  5500,
   10956,  -871,    26,  -871, 10076,   681,  1582, 17546, 18287,   516,
     648,  -871,   673,  -871,  -871,  -871,  -871,  -871,    34,   208,
    -871, 16688,  -871, 14535,  -871,  -871, 17546,  -871,   689,   690,
    -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,   327, 10956,
   14166, 12540, 10956, 10076,  -871,  -871,  7964,  -871,  -871,  -871,
    -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,    44,
   10956, 10956,  -871,  -871,  -871, 17546, 17546,  5324,  -871,  5324,
   12188,  -871,  5324,   806,   688,   699, 18287,   533,  -871,  -871,
     644,  -871,  -871,  -871,   695,   698,   260, 12188,   673,   624,
    -871, 10604,  -871,  -871, 17546,  -871,  5324, 12188,  -871,  -871,
    -871, 18287,  -871,   720,   533, 10956,  5324, 10956, 10076,  -871,
    -871,  -871,  -871,  -871,   464,  -871,  -871,  -871,  -871,  -871,
    -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  6732,
    -871,  -871, 16734,   234, 15852,   387,   706,  6732,  -871, 12188,
    -871,  -871,  3373,   402,   403,    64, 16779,  -871, 16855, 12188,
     337,  7964,  -871,  5324,   -10, 15805,  -871,  -871, 17546,  5324,
   17546,  -871, 16940,  -871,  9900, 17546,    28, 15438,  -871,   338,
    -871,  -871,  -871,  -871, 17924,  2287,  -871,  -871, 18063,  -871,
      34,   314, 12188,  -871,   725,   700,  -871,   609,   609,  -871,
    -871,  -871,   701, 17016, 12188,   844, 17101,   344,  -871,   644,
     345,   348,  -871,  -871, 17546,   848,  5324,  -871,  -871,  -871,
    -871,  -871,  8668,  8668,  -871,  -871,  -871,  -871,   260, 17546,
    -871,    81,    29,   705, 12188,  -871,    61, 15959,    23, 15523,
    -871,   284, 12012,  6732,  6556,  6732,  6556,  -871,  -871,  -871,
   17061, 17546, 17731,  -871,  -871,  -871,    26,    26,  -871, 15599,
    -871,   351,  -871,  -871,  5676,   707,  5852,   708,  -871, 12188,
   14697,  6028,  -871,    26,  6204,  -871,    26,   719,  2287,  -871,
    -871,  -871,  -871,  -871,  -871,   238,  -871,   123,  -871,   406,
   17222,    24,  -871,  -871,  -871, 12716, 12892, 17263, 13068, 13244,
    -871,  -871,  5324,  -871,   712,   714,  -871,  -871,  5324,  -871,
     533, 17546,  5324,  5324,  -871,  -871, 17546,   418,   724,   419,
     728, 12188,  -871,  -871,  -871,  -871,  7964,  -871,  9900,  -871,
   17546,  5324,  9900,  -871, 17546,  5324, 17546,  5324,  9900, 17546,
    -871,  9900, 17546,  -871,  -871,  -871,    33,  -871, 12188,   314,
     238,   238,   238,   238,   238,   238,   314,  5148,  -871,  -871,
    -871,  -871, 12188, 12188, 12188, 12188, 12188,  -871,   726,   734,
     657,  -871,  -871,  -871,  -871,  -871, 17546,  -871, 14777,  -871,
   14857,  -871, 16414, 14937, 15017,  -871, 17546,  -871,  -871,  -871,
    -871,  -871,  -871,  -871,   741,  -871, 17308, 17384, 17425, 17470,
    -871,  -871,  5324,  5324,  5324,  5324, 12188, 12188, 12188, 12188,
   16414, 16414, 16414, 16414
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   144,   465,   678,   679,   680,
     674,   675,   681,   215,   590,   133,   166,   562,   173,   564,
     590,   172,   496,   494,   133,     0,   215,   281,   167,   205,
     204,   672,   203,   215,   171,    68,   282,   344,   168,   145,
       0,     0,     0,   339,     0,   215,   175,   215,   215,   607,
     582,   682,   176,   215,   345,   556,   491,   215,     0,     0,
     190,   188,   432,   170,   565,   493,   215,   215,   215,   215,
     560,   215,     0,   174,     0,     0,   146,   215,   673,   215,
     215,   484,   169,   319,   558,   495,   177,   215,   731,     0,
     215,   733,   215,   734,   735,   623,   215,   732,   215,   631,
     186,   730,     0,   215,   215,     4,     0,     5,    10,    11,
      12,    48,    51,    52,    56,     0,    46,    71,    13,    77,
      14,    15,    16,    17,    30,   542,   543,    23,    47,   187,
     197,   215,   206,   645,   198,    18,     0,    32,    31,    20,
       0,   276,    19,   641,    22,    36,    33,    34,   196,   320,
       0,   194,     0,     0,   638,   343,     0,   635,   192,   360,
     452,   443,   633,   195,     0,     0,   193,   650,   629,   628,
     632,   546,   544,     0,   215,   639,   640,   644,   643,   642,
       0,   545,     0,   651,   652,   653,   676,   677,   634,   548,
     547,   636,   637,     0,    29,   564,   167,   215,   145,     0,
     565,   215,   215,     0,     0,   638,   650,   544,   639,   640,
     554,   545,   651,   652,     0,     0,   591,   134,   135,     0,
     563,     0,   485,     0,   492,     0,    21,     0,   530,   215,
     141,   147,   159,   153,   152,   161,   138,   151,   162,   148,
     142,   163,   136,   164,   157,   150,   158,   156,   154,   155,
     137,   139,   149,   160,   165,   143,     0,   140,   214,     0,
       0,     0,     0,    70,    69,    71,   215,     0,     0,     0,
       0,     0,     0,     0,   500,     0,   183,    41,     0,   311,
       0,   310,   716,   611,   608,   609,   610,     0,   557,   717,
       7,   215,   342,   342,   439,   180,   434,   181,   435,   436,
     440,   441,   179,   178,   442,   432,   526,     0,   347,   348,
     350,     0,   433,   525,   352,   513,     0,   215,   215,   184,
     635,   626,   649,   627,     0,     0,    44,     0,   561,   549,
       0,   215,    45,   555,     0,   289,    48,   293,   290,   293,
     559,     0,    55,   718,   720,   621,   715,   714,     0,    74,
      78,     0,     0,   532,   215,     0,     0,   533,   622,     0,
       6,   317,     0,   204,   318,     0,    49,     0,     9,    71,
      50,    53,     0,    60,     0,   215,    72,   215,     0,   496,
     202,     0,   674,   320,   646,   210,   215,     0,   218,   219,
     220,   215,   215,   215,   217,   566,   573,     0,   215,     0,
     316,     0,     0,   337,     0,   330,   432,   452,   450,   451,
     449,   368,   454,   453,   457,   456,   458,     0,   447,   444,
     445,   448,     0,   489,     0,   486,     0,     0,   630,    35,
     612,   215,   215,   215,   215,   215,   215,   719,   215,   215,
     215,   215,   215,     0,   620,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   399,   406,   407,   408,   403,   405,   215,   215,   401,
     404,   402,   400,   215,   410,   409,   215,   215,   215,   215,
     215,   215,     0,     0,   218,   215,   215,     0,   215,    37,
      25,    38,     0,    26,   215,    39,   542,     0,   537,   538,
     541,   329,     0,     0,     0,   250,     0,   399,   246,   215,
       0,     0,    24,   215,     0,   260,    27,   504,     0,   505,
     507,     0,   524,   215,   510,   215,     0,   182,    40,   215,
     313,     0,   215,   215,    42,     0,   191,   189,   437,   438,
       0,   433,   452,   432,   425,     0,   424,   528,   215,   126,
     647,   648,   358,     0,   185,     0,    43,   215,     0,   300,
     291,   292,    28,    76,    75,    79,   215,   683,     0,   215,
     668,   215,   670,     8,   215,   215,   216,    56,   215,    54,
       0,    65,     0,   124,     0,   119,     0,    86,     0,   215,
     215,   199,   320,     0,   212,     0,   209,   590,     0,     0,
       0,     0,     0,   574,     0,     0,   530,     0,     0,     0,
     287,   321,     0,   328,   334,   215,   332,   327,   417,     0,
     419,   423,     0,   455,   518,     0,   520,   459,   446,   415,
     141,   395,   147,   393,   153,   152,   151,   148,   142,   397,
     163,   164,   150,   154,   137,   149,   165,   392,   374,   377,
     375,   376,   398,   387,   378,   391,   383,   381,   394,   382,
     380,   385,   390,   379,   384,   388,   389,   386,   396,   215,
     371,     0,   415,   372,   415,   369,   215,   488,     0,   483,
     499,   498,   711,   710,   713,   722,   721,   726,   725,   707,
     704,   705,   706,   624,   694,   144,     0,   664,   665,   146,
     663,   662,   618,   698,   709,   703,   701,   712,   702,   700,
     692,   697,   699,   708,   691,   695,   696,   693,   619,     0,
       0,     0,     0,     0,     0,     0,     0,   724,   723,   728,
     727,   215,   215,     0,     0,   287,   601,   602,   604,   606,
       0,   593,     0,     0,     0,     0,   531,   215,   551,   215,
     215,   215,   249,     0,   245,     0,     0,     0,     0,   550,
       0,   263,     0,   261,     0,   508,     0,   523,   522,   215,
     502,   617,   501,   312,   309,     0,     0,     0,   661,   528,
     353,   349,     0,   215,   529,   510,   351,   215,   357,     0,
       0,     0,   552,     0,   294,   684,   625,   669,   534,   671,
     535,     0,     0,    57,    58,     0,    61,    63,    67,    66,
     215,   101,     0,     0,   215,    94,   215,   215,   215,    64,
     215,   374,   215,   375,   376,   215,   215,   386,     0,   411,
     412,    81,    80,    93,     0,     0,   322,     0,     0,   215,
       0,   215,     0,   223,   222,   215,   575,     0,     0,     0,
     215,   215,     0,     0,   215,   215,   687,   234,     0,     0,
     215,   232,     0,   284,   215,     0,   342,   338,     0,     0,
     432,   418,   459,   516,   515,   514,   517,   460,   466,   432,
     367,     0,   373,     0,   363,   364,   490,   487,     0,     0,
     132,   130,   131,   129,   128,   127,   659,   660,     0,   215,
     685,   215,   215,   215,   603,   605,   215,   592,   166,   173,
     172,   171,   168,   175,   176,   170,   174,   169,   177,     0,
     215,   215,   497,   540,   539,   258,   257,     0,   252,     0,
     215,   248,     0,   254,     0,   286,     0,     0,   506,   509,
     510,   511,   512,   308,     0,     0,   510,   215,   459,   528,
     527,   215,   428,   426,   359,   302,     0,   215,   301,   304,
     553,     0,   295,   298,     0,   215,     0,   215,   215,    59,
     125,   122,   102,   114,   108,   107,   106,   116,   103,   117,
     112,   105,   113,   111,   109,   110,   104,   115,   118,   215,
     121,   120,    89,    88,    87,     0,     0,   215,   200,   215,
     201,   322,   342,     0,     0,   590,   590,   226,     0,   215,
       0,   215,   242,     0,     0,     0,   686,   240,   576,     0,
     689,   688,     0,   235,   215,   569,     0,     0,   233,     0,
     315,   190,   188,   314,   342,   342,   323,   333,     0,   420,
     466,     0,   215,   361,   422,     0,   370,   415,   415,   666,
     667,   286,     0,     0,   215,     0,     0,     0,   594,   510,
     650,   650,   251,   247,   581,   253,     0,    73,   283,   262,
     259,   503,   215,   215,   429,   427,   354,   355,   510,   414,
     306,     0,     0,     0,   215,   296,     0,     0,     0,     0,
      62,     0,   215,   215,   215,   215,   215,    95,    98,    84,
      83,    82,   342,   207,   213,   211,     0,     0,   221,     0,
     244,     0,   596,   243,     0,     0,     0,     0,   241,   215,
       0,     0,   230,     0,     0,   228,     0,   288,   342,   325,
     324,   336,   340,   480,   481,     0,   482,   468,   471,     0,
     467,     0,   416,   365,   366,   215,   215,     0,   215,   215,
     288,   598,     0,   256,     0,     0,   431,   430,     0,   305,
       0,   299,     0,     0,   123,    91,    90,     0,     0,     0,
       0,   215,   208,   225,   224,   227,   215,   595,   215,   238,
     578,     0,   215,   236,   577,     0,   690,     0,   215,   568,
     231,   215,   567,   229,   285,   326,     0,   479,   215,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   461,   463,
     362,   462,   215,   215,   215,   215,   215,   255,   613,   614,
     307,   297,    97,   100,    96,    99,    85,   597,     0,   239,
       0,   237,   572,     0,     0,   341,   470,   472,   473,   476,
     477,   478,   474,   475,   469,   464,     0,     0,     0,     0,
     615,   616,     0,     0,     0,     0,   215,   215,   215,   215,
     580,   579,   571,   570
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -871,  -871,  -871,    -1,   -59,   845,  -871,   411,   789,  -871,
    -871,   576,  -147,  -104,  -871,   328,  -871,  -871,   -62,  -871,
    -871,    51,   642,  -871,  -538,  2582,   542,  -559,  -871,  -791,
    -871,  -871,  -871,   102,  -871,  -871,  -871,   884,  -871,  2293,
    -212,  -871,  -871,  -452,   295,  -871,  -828,  -871,  -871,   378,
    -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,    10,  -871,
    -162,   550,  -372,  -871,   522,  -871,  -871,   -21,  1413,  -871,
    -871,  -871,  -871,   582,  -871,   -39,  -871,  -871,  -871,  -871,
     394,  -871,  -871,  -871,  -101,  -871,  -352,  -792,  -871,  -871,
    -871,  -871,  -871,  -398,  -871,    50,  -871,  -871,  -871,  -871,
    -871,   381,  -871,  1599,  -871,  -871,  -871,  -871,   543,  -871,
    -871,  -871,  -871,    53,  -404,  -175,  -774,  -850,  -648,  -871,
      54,  -871,    62,   395,   -48,   645,   -46,  -871,  -871,  -354,
    -769,  -871,  -256,  -871,   -99,  -262,  -253,  -695,  -871,  -871,
     110,   270,  -871,  -204,   818,  -871,  -154,   426,   188,  -427,
    -757,  -596,  -871,  -871,  -871,  -597,  -511,  -871,  -733,   -19,
      82,  -871,  -257,  -493,  -535,    -2,  -871,  -871,  -871,  -871,
    -871,  -871,   173,  -871,   -18,  -871,  -871,  -469,  -871,  -870,
    -871,  -871,   329,   527,  -871,  -871,  -871,  -871,  2656,  -871,
     -54,   972,  -871,  1288,  1445,  -871,  -871,  -871,  -871,  -871,
    -871,  -871,  -871,  -871,  -478
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   350,   106,   107,   108,   109,   110,   367,
     368,   111,   112,   113,   114,   373,   579,   805,   806,   115,
     116,   117,   378,   118,   119,   120,   351,   833,   586,   834,
     121,   122,   990,   583,   584,   123,   124,   219,   255,   272,
     125,   257,   126,   306,   861,   127,   128,   293,   292,   129,
     130,   131,   132,   133,   134,   595,   837,   135,   203,   137,
     393,   394,   604,   138,   261,   139,   762,   763,   204,   141,
     142,   143,   144,   560,   794,   964,   145,   146,   790,   959,
     279,   280,   147,   148,   149,   150,   399,   866,   151,   152,
     404,   868,   869,   405,   153,   205,  1196,   155,   156,   307,
     308,   309,   948,   157,   321,   553,   787,   158,   159,  1141,
     160,   161,   671,   672,   829,   830,   831,   952,   880,   407,
     619,  1045,   620,   547,   621,   311,   541,   422,   411,   417,
     878,  1210,  1211,   162,  1043,  1137,  1138,  1139,   163,   164,
     424,   425,   677,   165,   166,   223,   273,   274,   520,   521,
     770,   313,   876,   626,   627,   524,   314,   784,   785,   352,
     355,   356,   497,   498,   499,   206,   168,   169,   395,   170,
     396,   171,   207,   173,   215,   216,   740,   853,  1111,   741,
     742,   174,   208,   209,   177,   178,   179,   322,   180,   385,
     500,   211,   182,   212,   213,   185,   186,   187,   188,   607,
     189,   190,   191,   192,   193
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   105,   221,   305,   614,   402,   477,   227,   371,   511,
     744,   427,   136,   324,   310,   788,   312,   496,   673,   492,
     760,   608,   791,   167,   884,   872,   885,   996,   953,   875,
     828,   593,   383,   341,   535,   136,  1058,   786,  1036,   991,
     789,   486,   525,   807,  1002,   522,   946,   316,   275,   357,
     624,   386,   154,   622,   496,   275,  1207,   361,   275,   387,
    1121,   275,   685,   687,  -281,  1207,   522,   388,   389,   390,
     504,   767,   847,   167,   558,   154,   386,   167,   167,   496,
     494,  -519,   294,   494,   387,   136,   265,   509,     3,   136,
     136,   764,   388,   389,   390,  1041,  1074,  -521,   495,   522,
     167,   549,   296,  1040,   167,   386,   298,   552,   727,   729,
    -519,   847,   136,  1158,  -303,  -282,   136,   538,   782,   496,
     956,   388,   389,   390,   364,   154,  -521,   848,   194,   154,
     154,   299,   899,   678,   300,   849,    58,   224,   365,   858,
     862,  1112,   695,   526,   847,  -521,   301,   391,    83,   847,
     855,   679,   154,  1042,  -356,  -519,   154,  -303,   484,   389,
     390,   539,   369,   957,   529,   304,  1114,   433,  -519,   940,
    -521,  -521,   391,   874,  1036,   172,  -264,   494,   856,  1077,
     375,   229,   847,  1071,  -521,   214,   418,   365,   780,  1075,
    1208,   523,  -521,   198,   365,   365,   625,   365,   172,   365,
     365,   391,   392,   608,   365,   575,  1129,  1130,   850,  1102,
    -265,  -303,   523,   167,   924,  -519,  1078,   958,  -521,   376,
    -519,   725,   726,  1099,  -521,   136,   434,   574,  1156,   618,
     435,   701,   266,   851,   283,   294,  -521,  1162,   419,   898,
     835,  1133,  1198,   699,   217,   523,  -521,   674,   172,   377,
     420,   433,   172,   172,   965,   296,   392,   194,   995,   298,
     904,   900,   569,   563,  -521,   154,   496,   496,   167,   421,
    1093,  1199,   950,   167,  1036,   172,   222,   412,   437,   172,
     136,   592,   847,   441,   299,   136,   443,   300,   570,    58,
     446,  -271,  1134,   485,   284,   218,  -270,   571,   371,   301,
    1195,   496,  1151,  1168,   905,  1170,  1227,  1094,   765,   851,
     434,   229,  -335,   285,   435,   408,   258,  1133,   304,  -331,
     154,  1157,  1059,   572,   566,   154,   413,   775,   847,   414,
     286,   175,   870,   862,   766,   433,   409,   852,   745,   906,
     277,   415,  1136,  -331,  -279,   496,   410,   305,   567,   167,
     565,  1076,   749,    23,   175,   494,   263,   -70,   871,   264,
     416,   136,   437,   167,   326,   907,   496,   441,  1134,   442,
     443,  1029,   167,   605,   446,   136,   695,   -70,   935,   611,
    -269,   453,  -421,   966,   136,   167,   769,   616,   172,   459,
     936,   838,   266,   696,   434,   765,   -70,   136,   435,  1143,
    1144,   154,  -266,   765,   175,    56,   697,  1135,   175,   175,
    1057,   617,   951,   720,   721,   154,   937,    65,  1136,   722,
     675,   938,   723,   724,   154,   267,   -70,   198,   744,   949,
    1091,   175,   997,   807,   526,   175,   698,   154,   863,   810,
    1197,  -273,    85,   172,  -277,   864,   437,   438,   172,   439,
     440,   441,   681,   442,   443,   444,  1164,   811,   446,  -736,
    -736,   968,  -736,  -736,   452,   453,   733,  1005,   456,   291,
    1037,   457,   458,   459,   -69,   749,  -274,   699,  1014,   673,
     814,   437,   460,   700,   305,   494,   749,   969,  1026,   443,
     335,   338,   749,  -600,   -69,   310,  -599,   312,   815,  1176,
     290,  1051,   167,   526,   167,  1238,  1239,  1240,  1241,  1242,
    1243,  1110,  1127,   -69,   136,   357,   136,   357,  1150,  -600,
     496,   922,  -599,   997,   172,  1177,   478,  1052,   479,   176,
    1055,   433,  1200,   744,  1167,   997,  1169,   496,   172,  -268,
    -658,   998,  -658,   -69,   175,  1115,  1117,   172,   685,   727,
     678,   526,   176,  1097,   154,   801,   154,  1123,  1126,  1201,
     172,  1202,  1203,  -587,  1204,  1205,   997,   997,  1104,  1105,
    -657,   194,  -657,   528,   480,   530,   330,   808,   481,   840,
     167,   565,   534,   921,  1222,  1224,  -657,   481,  -657,  -654,
     434,  -654,   136,  1086,   435,  1088,   365,  -272,   514,   175,
     971,   701,   176,  1209,   175,  -733,   176,   176,  -734,  -733,
    -735,  -730,  -734,   331,  -735,  -730,  -278,  1115,  1117,  1123,
    1126,  -280,   556,   437,   294,  -656,  -267,  -656,   441,   176,
    -275,   443,   154,   176,  -655,   446,  -655,   888,   342,   889,
    1131,   372,   437,   438,   296,   439,   440,   441,   298,   442,
     443,   798,   496,   800,   446,   336,   336,    89,  1209,   374,
     452,   453,   397,    37,   456,  -584,  -342,   457,   458,   459,
     536,   537,   398,   299,   406,   294,   300,   172,   359,   172,
     175,   487,   428,   488,  1004,  -583,   376,  -342,   301,   429,
      54,   496,  -588,   923,   175,   296,  -589,  -342,   744,   298,
    -586,  -585,   490,   175,   365,   493,   512,   304,  1048,   501,
     513,   543,   573,   548,   554,   942,   175,   532,   533,  1035,
     542,   562,  1038,   594,   299,   557,   577,   300,   588,    58,
     578,   361,   599,   612,   615,    22,    23,   623,   628,   301,
     919,   613,   176,   676,   362,   680,    29,   363,   443,   719,
     732,   167,    32,   747,   167,   172,   496,   748,   304,    37,
     749,   753,  -342,   136,   750,   751,   136,   756,   758,   759,
     769,   783,   793,   778,   792,   -68,   836,   841,   494,   877,
     845,  -144,   879,  -342,  -166,   882,    54,  -173,    56,   890,
    -172,  -171,    60,  -342,   -68,    61,   695,   176,   364,  -168,
      65,  -145,   176,   154,  -175,  -176,   154,  1034,  -170,  -174,
     908,   305,   897,   909,  -146,  1154,  1155,  -169,   910,    81,
     305,  -177,    83,   903,   883,    85,  1010,   891,   196,   892,
     896,   893,   894,   175,   895,   175,   927,   929,   911,   947,
    1066,   955,   997,  1000,   960,   912,   167,   198,  1009,  1001,
     167,  1013,  1011,  1030,  1067,  1035,   913,   167,   136,  1019,
     496,   496,   136,  1049,   914,  1068,  1050,  1084,  1072,   136,
     226,  1073,  1098,   870,  1142,   100,  1148,  1145,   176,  1160,
     365,   915,  1152,  1181,  1185,  1194,  1218,  1128,  1219,  1199,
    1223,  1250,   176,   916,  1225,   843,   844,   699,   154,  1251,
     366,   176,   154,   857,   917,   803,  1090,   510,   225,   154,
     918,   175,   970,   600,   176,  1069,   582,   576,  1060,  1061,
     332,   561,  1083,   773,   781,   167,   172,   167,   596,   172,
     167,   371,  1039,  1044,   371,   779,  1047,   136,  1082,   136,
    1235,  1132,   136,  1034,  1244,   509,  1237,  1003,   887,   380,
     540,   360,   772,   939,   167,  1035,     0,     0,     0,     0,
       0,     0,     0,     0,   167,     0,  1081,     0,     0,     0,
       0,     0,     0,     0,   181,     0,   136,   154,     0,   154,
       0,     0,   154,     0,     0,     0,     0,  1106,  1107,     0,
       0,     0,     0,     0,     0,     0,     0,   181,     0,   361,
       0,     0,     0,    22,    23,     0,   154,     0,     0,     0,
       0,   167,   362,     0,    29,   363,   154,   167,     0,     0,
      32,   172,     0,   136,   371,   172,     0,    37,     0,   136,
       0,   176,   172,   176,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1034,     0,     0,     0,   181,     0,     0,
       0,   181,   181,   509,    54,   509,    56,     0,   509,   509,
     489,     0,     0,   154,   167,     0,   364,     0,    65,   154,
       0,     0,     0,     0,   181,     0,   136,     0,   181,     0,
       0,     0,   175,     0,     0,   175,     0,    81,     0,     0,
      83,     0,     0,    85,     0,     0,     0,     0,     0,     0,
     172,     0,   172,     0,     0,   172,     0,     0,     0,   176,
       0,     0,   167,     0,   167,   516,   154,     0,     0,   167,
     527,     0,   167,     0,   136,     0,   136,     0,     0,   172,
       0,   136,     0,     0,   136,  1007,     0,     0,     0,   172,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     167,     0,     0,  1023,     0,     0,   167,  1028,     0,     0,
     167,   167,   136,   433,   154,     0,   154,     0,   136,     0,
       0,   154,   136,   136,   154,     0,     0,   175,     0,   167,
       0,   175,     0,   167,     0,   167,   172,   181,   175,     0,
       0,   136,   172,     0,     0,   136,   433,   136,     0,     0,
       0,     0,   154,     0,     0,   167,  1245,     0,   154,     0,
       0,     0,   154,   154,     0,     0,     0,   136,     0,     0,
       0,     0,   434,     0,     0,     0,   435,     0,     0,     0,
       0,   154,   598,     0,     0,   154,     0,   154,     0,   172,
       0,     0,   181,     0,     0,     0,     0,   181,     0,     0,
     167,   167,   167,   167,     0,   434,   175,   154,   175,   435,
       0,   175,   136,   136,   136,   136,     0,     0,     0,     0,
       0,     0,     0,     0,   437,   438,     0,   439,   440,   441,
     176,   442,   443,   176,     0,   175,   446,   172,     0,   172,
     183,     0,     0,   453,   172,   175,     0,   172,     0,   457,
     458,   459,   154,   154,   154,   154,     0,   437,   438,     0,
       0,   440,   441,   183,   442,   443,     0,     0,     0,   446,
       0,  1122,  1125,   181,     0,   172,   453,     0,     0,     0,
       0,   172,   457,   458,   459,   172,   172,   181,     0,     0,
       0,     0,   175,     0,     0,     0,   181,     0,   175,   752,
       0,   754,     0,     0,   172,     0,     0,     0,   172,   181,
     172,     0,     0,   183,     0,     0,     0,   183,   183,     0,
       0,     0,     0,     0,     0,   176,     0,  1159,     0,   176,
     172,     0,     0,  1122,  1125,     0,   176,     0,     0,     0,
     183,     0,     0,     0,   183,   175,     0,     0,     0,     0,
       0,  1173,  1174,     0,  1175,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   140,     0,     0,  1190,     0,
       0,  1193,     0,     0,     0,   172,   172,   172,   172,     0,
       0,     0,     0,     0,     0,     0,     0,   433,   140,     0,
       0,     0,     0,   175,     0,   175,     0,   184,     0,     0,
     175,     0,     0,   175,   176,     0,   176,     0,     0,   176,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
     184,     0,     0,     0,     0,     0,   181,     0,   181,     0,
       0,   175,     0,   176,     0,     0,     0,   175,   140,     0,
       0,   175,   175,   176,     0,     0,   434,     0,     0,     0,
     435,     0,     0,   183,     0,     0,     0,     0,     0,     0,
     175,     0,     0,     0,   175,   140,   175,     0,     0,   140,
     184,     0,     0,     0,   184,   184,     0,     0,   434,     0,
       0,     0,   435,     0,     0,     0,   175,     0,     0,     0,
     176,     0,     0,     0,     0,     0,   176,   184,   437,   438,
       0,   184,     0,   441,   181,   442,   443,     0,   183,     0,
     446,     0,     0,   183,     0,     0,     0,   453,     0,     0,
       0,     0,     0,   457,   458,   459,     0,     0,     0,     0,
     437,   175,   175,   175,   175,   441,     0,   442,   443,     0,
       0,     0,   446,   176,     0,     0,     0,     0,   928,   453,
       0,   931,     0,     0,   361,   457,   458,   459,    22,    23,
       0,     0,     0,     0,     0,     0,     0,   362,     0,    29,
     363,     0,     0,     0,     0,    32,     0,     0,   140,     0,
       0,     0,    37,     0,     0,     0,     0,     0,     0,   183,
       0,   176,     0,   176,     0,     0,     0,     0,   176,     0,
       0,   176,     0,   183,     0,     0,     0,     0,     0,    54,
     184,    56,   183,    58,     0,  1031,   320,     0,  1032,     0,
       0,   364,     0,    65,     0,   183,     0,     0,     0,   176,
       0,     0,     0,   140,     0,   176,   320,     0,   140,   176,
     176,     0,    81,  1012,     0,    83,     0,  1017,    85,     0,
       0,     0,   320,     0,   527,     0,     0,     0,   176,     0,
       0,     0,   176,     0,   176,   184,     0,     0,     0,     0,
     184,     0,     0,    89,     0,   181,     0,     0,   181,     0,
       0,     0,     0,     0,   176,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,  1033,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   140,     0,     0,     0,     0,     0,
       0,     0,  1062,     0,  1063,     0,     0,  1065,   140,   176,
     176,   176,   176,     0,     0,     0,     0,   140,     0,     0,
       0,     0,   183,     0,   183,     0,   184,     0,     0,     0,
     140,  1080,     0,     0,     0,     0,     0,     0,     0,     0,
     184,  1017,     0,     0,     0,     0,     0,     0,     0,   184,
     181,     0,     0,     0,   181,     0,     0,     0,   320,     0,
       0,   181,   184,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   426,     0,   230,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1113,     0,
     231,   232,     0,   233,  1118,   320,     0,     0,   234,     0,
     183,     0,     0,     0,     0,     0,   235,     0,     0,     0,
       0,     0,   236,     0,     0,     0,     0,     0,   237,     0,
     320,     0,   238,     0,     0,   239,     0,   240,     0,   181,
       0,   181,     0,     0,   181,     0,   241,     0,     0,     0,
       0,  1153,     0,   242,   243,     0,     0,   140,     0,   140,
       0,   244,     0,     0,     0,     0,     0,     0,   181,     0,
     320,   245,     0,     0,     0,     0,     0,     0,   181,     0,
     246,   247,     0,   248,     0,   249,     0,   250,     0,   184,
     251,   184,     0,     0,   252,     0,     0,   253,     0,  1179,
     254,  1183,     0,     0,     0,     0,   527,     0,     0,   527,
     461,   462,   463,   464,   465,   466,   467,   468,   469,   470,
     471,   472,   473,   474,   475,   181,     0,     0,     0,     0,
       0,   181,     0,     0,     0,   140,     0,  1217,     0,     0,
       0,     0,     0,  1220,     0,     0,     0,  1179,  1183,     0,
       0,     0,     0,     0,   476,     0,   271,  -654,     0,  -654,
       0,     0,     0,     0,     0,     0,  1229,   184,     0,     0,
    1231,     0,   754,     0,     0,     0,     0,     0,   181,     0,
       0,   183,     0,     0,   183,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   320,   320,     0,     0,
       0,     0,   320,     0,     0,   320,   320,   320,   320,     0,
       0,     0,     0,     0,     0,     0,   181,     0,   181,     0,
       0,     0,     0,   181,     0,     0,   181,   754,   754,   754,
     754,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   320,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   181,     0,     0,     0,     0,     0,
     181,     0,     0,     0,   181,   181,   183,     0,     0,     0,
     183,     0,     0,     0,     0,     0,     0,   183,     0,     0,
       0,     0,     0,   181,     0,   865,   320,   181,     0,   181,
       0,     0,     0,     0,     0,     0,   140,     0,   320,   140,
     320,     0,     0,     0,     0,     0,  -322,   320,     0,   181,
    -322,  -322,     0,     0,     0,     0,     0,     0,     0,  -322,
       0,  -322,  -322,     0,     0,     0,     0,  -322,   184,     0,
       0,   184,     0,     0,  -322,     0,     0,  -322,     0,     0,
       0,     0,     0,     0,     0,   183,     0,   183,     0,     0,
     183,     0,     0,     0,   181,   181,   181,   181,  -322,     0,
       0,  -322,     0,  -322,     0,  -322,     0,  -322,  -322,     0,
    -322,     0,     0,  -322,   183,  -322,     0,     0,     0,     0,
       0,     0,     0,     0,   183,     0,     0,     0,     0,     0,
       0,   140,     0,     0,  -322,   140,     0,  -322,     0,     0,
    -322,     0,   140,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   184,     0,  -322,     0,   184,     0,     0,
       0,   183,     0,     0,   184,     0,     0,   183,     0,   361,
       0,     0,     0,    22,    23,     0,     0,     0,     0,     0,
    -322,   256,   362,     0,    29,   363,  -322,     0,     0,     0,
      32,   320,     0,   268,   269,   270,     0,    37,     0,     0,
     140,     0,   140,     0,     0,   140,   320,     0,   320,     0,
       0,     0,     0,     0,   183,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,     0,    56,     0,   320,   140,
    1031,     0,   184,  1032,   184,     0,   364,   184,    65,   140,
       0,     0,     0,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    81,     0,     0,
      83,   184,   183,    85,   183,     0,     0,     0,     0,   183,
       0,   184,   183,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   140,     0,    89,     0,
       0,     0,   140,     0,     0,     0,     0,     0,     0,     0,
     183,     0,     0,   400,   434,   403,   183,     0,   435,     0,
     183,   183,     0,   100,     0,     0,     0,   423,   184,     0,
       0,     0,     0,   320,   184,     0,     0,     0,     0,   183,
       0,     0,     0,   183,     0,   183,     0,     0,     0,   140,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   483,     0,     0,   183,   437,   438,     0,   439,
     440,   441,   320,   442,   443,   444,     0,   445,   446,   447,
     448,   184,   450,   451,   452,   453,   454,     0,   456,     0,
       0,   457,   458,   459,     0,     0,     0,   140,     0,   140,
       0,     0,   460,     0,   140,     0,     0,   140,     0,     0,
     183,   183,   183,   183,     0,     0,     0,     0,     0,     0,
       0,     0,   502,   503,     0,     0,     0,     0,     0,   184,
       0,   184,     0,     0,   519,   140,   184,   320,     0,   184,
       0,   140,     0,     0,     0,   140,   140,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   140,     0,     0,   184,   140,     0,
     140,     0,     0,   184,   546,     0,     0,   184,   184,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     140,     0,     0,     0,     0,     0,   184,   276,     0,     0,
     184,     0,   184,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   276,   184,     0,     0,   256,     0,     0,     0,     0,
       0,   337,   339,     0,     0,   140,   140,   140,   140,   210,
       0,   320,   320,   220,     0,     0,     0,     0,     0,     0,
       0,     0,   228,     0,     0,     0,     0,     0,     0,   262,
     610,     0,     0,   370,     0,   403,     0,   184,   184,   184,
     184,   278,     0,   281,   282,     0,     0,     0,     0,   287,
       0,   288,     0,   289,     0,   670,     0,     0,   315,     0,
       0,     0,   228,   323,   325,   327,   328,   329,     0,     0,
       0,   333,     0,   334,     0,     0,     0,     0,     0,     0,
     340,     0,     0,   323,     0,     0,   343,     0,   344,     0,
       0,   345,   346,     0,   347,     0,     0,     0,   228,   323,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     735,     0,     0,     0,     0,     0,     0,   381,     0,   384,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   230,
       0,     0,     0,   491,     0,     0,     0,   761,     0,     0,
       0,   519,     0,   231,   232,     0,   233,     0,     0,     0,
       0,   234,     0,     0,   774,     0,     0,     0,     0,   235,
     430,     0,     0,   546,     0,   236,     0,     0,   519,     0,
       0,   237,     0,   505,   508,   238,     0,     0,   239,     0,
     240,   515,     0,   482,     0,     0,     0,   333,   228,   241,
     276,     0,   276,     0,     0,     0,   242,   243,     0,   276,
       0,     0,     0,     0,   244,     0,     0,     0,     0,     0,
       0,   819,     0,     0,   245,   323,     0,     0,     0,     0,
       0,     0,   842,   246,   247,     0,   248,   230,   249,     0,
     250,     0,     0,   251,     0,     0,     0,   252,   517,   276,
     253,   231,   232,   254,   233,     0,     0,     0,     0,   234,
       0,     0,   323,     0,     0,     0,     0,   235,     0,     0,
       0,     0,     0,   236,     0,     0,     0,     0,     0,   237,
       0,     0,     0,   238,     0,     0,   239,   323,   240,     0,
       0,     0,     0,     0,     0,     0,     0,   241,   329,   333,
       0,     0,     0,     0,   242,   243,     0,     0,     0,   518,
       0,   423,   244,   550,   551,     0,     0,     0,     0,     0,
       0,     0,   245,     0,     0,     0,     0,   323,     0,     0,
       0,   246,   247,     0,   248,     0,   249,     0,   250,     0,
       0,   251,     0,     0,     0,   252,   544,     0,   253,     0,
     568,   254,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   585,     0,   587,     0,     0,     0,     0,     0,     0,
       0,     0,   597,     0,     0,     0,     0,   601,   606,   609,
       0,   934,     0,     0,   228,     0,     0,     0,   519,     0,
       0,     0,     0,     0,     0,     0,     0,   545,   943,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   963,   682,   683,   684,
     686,   688,   689,     0,   690,   691,   692,   693,   694,     0,
     702,   703,   704,   705,   706,   707,   708,   709,   710,   711,
     712,   713,   714,   715,   716,   717,   718,     0,     0,     0,
       0,     0,     0,   323,   323,     0,     0,     0,     0,   323,
     423,     0,   323,   323,   323,   323,   728,   730,     0,     0,
       0,   606,   734,     0,   743,     0,     0,     0,     0,     0,
     746,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   403,   755,     0,     0,   757,     0,     0,     0,   323,
       0,     0,     0,     0,     0,     0,   670,     0,     0,   768,
       0,   771,   276,   276,     0,   281,     0,     0,   776,   777,
     276,   276,     0,     0,     0,     0,     0,     0,     0,   315,
       0,     0,   230,     0,   315,     0,     0,   431,     0,     0,
       0,     0,     0,   323,   432,     0,   231,   232,     0,   233,
       0,     0,   746,     0,   234,   323,   433,   323,     0,   761,
     228,   802,   235,     0,   323,     0,     0,     0,   236,     0,
       0,     0,     0,     0,   237,   832,   832,     0,   238,     0,
       0,   239,     0,   240,   963,     0,     0,     0,     0,   846,
       0,     0,   241,     0,     0,     0,     0,     0,     0,   242,
     243,   867,     0,     0,     0,     0,     0,   244,     0,     0,
       0,   315,     0,     0,     0,   434,     0,   245,     0,   435,
       0,     0,     0,     0,     0,     0,   246,   247,   932,   248,
       0,   249,     0,   250,     0,     0,   251,     0,     0,     0,
     252,     0,     0,   253,     0,     0,   254,     0,     0,     0,
     436,     0,     0,     0,     0,   881,     0,     0,     0,     0,
       0,   403,   886,     0,     0,     0,     0,   437,   438,   933,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
       0,     0,   457,   458,   459,     0,   365,     0,     0,     0,
     365,     0,   961,   460,     0,   962,     0,     0,     0,     0,
       0,     0,     0,     0,   276,     0,     0,   755,   323,     0,
       0,     0,     0,     0,     0,   361,     0,     0,     0,    22,
      23,     0,     0,   323,     0,   323,   925,   926,   362,     0,
      29,   363,     0,     0,     0,     0,    32,     0,     0,     0,
       0,     0,   276,    37,     0,   323,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   315,
     276,     0,     0,   954,   276,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,  1031,     0,     0,  1032,
       0,     0,   364,     0,    65,     0,   585,     0,     0,     0,
     992,     0,   994,   832,   832,     0,   343,     0,   344,     0,
       0,   346,   347,    81,     0,     0,    83,     0,     0,    85,
       0,     0,     0,     0,     0,  1006,     0,  1008,     0,     0,
       0,   228,     0,     0,     0,     0,  1015,   746,  1018,     0,
    1020,  1022,     0,     0,    89,  1025,  1027,     0,     0,  1070,
     323,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,  1103,  1085,     0,     0,     0,
       0,     0,     0,     0,     0,  1053,  1018,  1025,  1056,   323,
       0,     0,   743,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   686,   728,     0,     0,
       0,     0,     0,     0,     0,     0,  1064,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   315,     0,     0,     0,  1079,   276,   276,
       0,     0,     0,   228,     0,     0,     0,     0,     0,     0,
       0,  1087,     0,  1089,   323,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   832,     0,     0,     0,     0,
       0,     0,     0,  1100,     0,  1101,     0,     0,     0,     0,
       0,     0,     0,     0,   276,  1109,     0,   743,     0,     0,
     276,   276,     0,     0,     0,     0,     0,     0,     0,     0,
    1120,     0,     0,     0,     0,     0,     0,     0,   276,   276,
       0,   276,     0,     0,     0,     0,     0,     0,  1140,     0,
       0,     0,   508,     0,     0,   276,     0,     0,   276,     0,
    1147,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   323,   323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1161,     0,  1221,     0,     0,     0,     0,     0,  1166,   832,
     832,   832,   832,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1180,     0,  1184,     0,     0,  1186,     0,  1189,     0,     0,
    1192,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1180,  1184,     0,  1189,  1192,     0,     0,     0,     0,
     508,     0,   508,     0,     0,   508,   508,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1226,     0,     0,
       0,     0,   743,     0,  1228,     0,     0,     0,  1230,     0,
       0,     0,     0,  1232,  1233,     0,     0,  1234,     0,     0,
       0,     0,     0,     0,  1236,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1246,  1247,
    1232,  1248,  1249,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1260,  1261,
    1262,  1263,  1260,  1261,  1262,  1263,    -2,     4,     0,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -215,    21,    22,    23,    24,    25,    26,     0,  -215,    27,
       0,    28,     0,    29,    30,    31,  -215,  -215,  -215,    32,
      33,    34,   -68,     0,    35,    36,    37,     0,    38,  -342,
      39,     0,    40,    41,    42,    43,    44,     0,    45,    46,
      47,    48,   -68,    49,    50,     0,    51,    52,    53,     0,
    -342,     0,     0,    54,    55,    56,    57,    58,    59,    60,
    -342,   -68,    61,    62,    63,    64,     0,    65,    66,    67,
       0,    68,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,     0,    83,
      84,   -68,    85,    86,     0,     0,  -215,     0,    87,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,     0,   103,
       0,  -215,     0,     4,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,  -685,     0,    13,    14,    15,    16,
      17,  -685,    18,     0,    19,    20,  -215,    21,    22,    23,
      24,    25,    26,  -685,  -215,    27,  -685,    28,     0,    29,
      30,    31,  -215,  -215,  -215,    32,    33,    34,   -68,     0,
      35,    36,    37,     0,    38,  -342,    39,     0,    40,    41,
      42,    43,    44,     0,    45,    46,    47,    48,   -68,    49,
      50,     0,    51,    52,    53,     0,  -342,     0,     0,    54,
      55,    56,    57,     0,    59,    60,  -342,   -68,    61,    62,
      63,    64,  -685,    65,    66,    67,  -685,    68,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,     0,    83,    84,   -68,    85,    86,
       0,     0,  -215,     0,    87,     0,     0,  -685,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,  -685,    92,  -685,  -685,  -685,
    -685,  -685,  -685,  -685,     0,  -685,  -685,  -685,  -685,  -685,
    -685,  -685,  -685,  -685,  -685,  -685,  -685,    99,   100,  -685,
    -685,  -685,     0,   102,  -685,   103,     0,  -215,     0,   348,
    -685,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,  -215,    21,    22,    23,    24,    25,    26,     0,
    -215,    27,     0,    28,     0,    29,    30,    31,  -215,  -215,
    -215,    32,    33,    34,   -68,     0,    35,    36,    37,     0,
      38,  -342,    39,     0,    40,    41,    42,    43,    44,     0,
      45,    46,    47,    48,   -68,    49,    50,     0,    51,    52,
      53,     0,  -342,     0,     0,    54,    55,    56,    57,    58,
      59,    60,  -342,   -68,    61,    62,    63,    64,     0,    65,
      66,    67,     0,    68,    69,    70,    71,    72,    73,    74,
      75,     0,    76,    77,     0,    78,    79,    80,    81,    82,
       0,    83,    84,   -68,    85,    86,     0,     0,  -215,     0,
      87,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
     349,   103,     0,  -215,     0,     4,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -215,    21,
      22,    23,    24,    25,    26,     0,  -215,    27,     0,    28,
       0,    29,    30,    31,  -215,  -215,  -215,    32,    33,    34,
     -68,     0,    35,    36,    37,     0,    38,  -342,    39,     0,
      40,    41,    42,    43,    44,     0,    45,    46,    47,    48,
     -68,    49,    50,     0,    51,    52,    53,     0,  -342,     0,
       0,    54,    55,    56,    57,    58,    59,    60,  -342,   -68,
      61,    62,    63,    64,     0,    65,    66,    67,     0,    68,
      69,    70,    71,    72,    73,    74,    75,     0,    76,    77,
       0,    78,    79,    80,    81,    82,     0,    83,    84,   -68,
      85,    86,     0,     0,  -215,     0,    87,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     100,     0,     0,   101,     0,   102,   564,   103,     0,  -215,
       0,   580,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -215,    21,    22,    23,    24,    25,
      26,     0,  -215,    27,     0,    28,     0,    29,    30,    31,
    -215,  -215,  -215,    32,    33,    34,   -68,     0,    35,    36,
      37,     0,    38,  -342,    39,     0,    40,    41,    42,    43,
      44,     0,    45,    46,    47,    48,   -68,    49,    50,     0,
      51,    52,    53,     0,  -342,     0,     0,    54,    55,    56,
      57,    58,    59,    60,  -342,   -68,    61,    62,    63,    64,
       0,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,     0,    83,    84,   -68,    85,    86,     0,     0,
    -215,     0,    87,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   100,     0,     0,   101,
       0,   102,   581,   103,     0,  -215,     0,     4,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -215,    21,    22,    23,    24,    25,    26,     0,  -215,    27,
       0,    28,     0,    29,    30,    31,  -215,  -215,  -215,    32,
      33,    34,   -68,     0,    35,    36,    37,     0,    38,  -342,
      39,     0,    40,    41,    42,    43,    44,     0,    45,    46,
      47,    48,   -68,    49,    50,     0,    51,    52,    53,     0,
    -342,     0,     0,    54,    55,    56,    57,    58,    59,    60,
    -342,   -68,    61,    62,    63,    64,     0,    65,    66,    67,
       0,    68,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,     0,    83,
      84,   -68,    85,    86,     0,     0,  -215,     0,    87,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,   809,   103,
       0,  -215,     0,     4,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,  -215,    21,    22,    23,
      24,    25,    26,     0,  -215,    27,     0,    28,     0,    29,
      30,    31,  -215,  -215,  -215,    32,    33,    34,   -68,     0,
      35,    36,    37,     0,    38,  -342,    39,     0,    40,    41,
      42,    43,    44,     0,    45,    46,    47,    48,   -68,    49,
      50,     0,    51,    52,    53,     0,  -342,     0,     0,    54,
      55,    56,    57,   359,    59,    60,  -342,   -68,    61,    62,
      63,    64,     0,    65,    66,    67,     0,    68,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,     0,    83,    84,   -68,    85,    86,
       0,     0,  -215,     0,    87,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,     0,   103,     0,  -215,     0,     4,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,  -215,    21,    22,    23,    24,    25,    26,     0,
    -215,    27,     0,    28,     0,    29,    30,    31,  -215,  -215,
    -215,    32,    33,    34,   -68,     0,    35,    36,    37,     0,
      38,  -342,    39,     0,    40,    41,    42,    43,    44,     0,
      45,    46,    47,    48,   -68,    49,    50,     0,    51,    52,
      53,     0,  -342,     0,     0,    54,    55,    56,    57,    58,
      59,    60,  -342,   -68,    61,    62,    63,    64,     0,    65,
      66,    67,     0,    68,    69,    70,    71,    72,    73,    74,
      75,     0,    76,    77,     0,    78,    79,    80,    81,    82,
       0,    83,    84,   -68,    85,    86,     0,     0,  -215,     0,
      87,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
       0,   103,     0,  -215,     0,     4,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -215,    21,
      22,    23,    24,    25,    26,     0,  -215,    27,     0,    28,
       0,    29,    30,    31,  -215,  -215,  -215,    32,    33,    34,
     -68,     0,    35,    36,    37,     0,    38,  -342,    39,     0,
      40,    41,    42,    43,    44,     0,    45,    46,    47,    48,
     -68,    49,    50,     0,    51,    52,    53,     0,  -342,     0,
       0,    54,    55,    56,    57,     0,    59,    60,  -342,   -68,
      61,    62,    63,    64,     0,    65,    66,    67,     0,    68,
      69,    70,    71,    72,    73,    74,    75,     0,    76,    77,
       0,    78,    79,    80,    81,    82,     0,    83,    84,   -68,
      85,    86,     0,     0,  -215,     0,    87,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     100,     0,     0,   101,     0,   102,     0,   103,     0,  -215,
       0,     4,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -215,    21,    22,    23,    24,    25,
      26,     0,  -215,    27,     0,    28,     0,    29,    30,    31,
    -215,  -215,  -215,    32,  1024,    34,   -68,     0,    35,    36,
      37,     0,    38,  -342,    39,     0,    40,    41,    42,    43,
      44,     0,    45,    46,    47,    48,   -68,    49,    50,     0,
      51,    52,    53,     0,  -342,     0,     0,    54,    55,    56,
      57,     0,    59,    60,  -342,   -68,    61,    62,    63,    64,
       0,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,     0,    83,    84,   -68,    85,    86,     0,     0,
    -215,     0,    87,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   100,     0,     0,   101,
       0,   102,     0,   103,     0,  -215,     0,     4,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -215,    21,    22,    23,    24,    25,    26,     0,  -215,    27,
       0,    28,     0,    29,    30,    31,  -215,  -215,  -215,    32,
    1178,    34,   -68,     0,    35,    36,    37,     0,    38,  -342,
      39,     0,    40,    41,    42,    43,    44,     0,    45,    46,
      47,    48,   -68,    49,    50,     0,    51,    52,    53,     0,
    -342,     0,     0,    54,    55,    56,    57,     0,    59,    60,
    -342,   -68,    61,    62,    63,    64,     0,    65,    66,    67,
       0,    68,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,     0,    83,
      84,   -68,    85,    86,     0,     0,  -215,     0,    87,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,     0,   103,
       0,  -215,     0,     4,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,  -215,    21,    22,    23,
      24,    25,    26,     0,  -215,    27,     0,    28,     0,    29,
      30,    31,  -215,  -215,  -215,    32,  1182,    34,   -68,     0,
      35,    36,    37,     0,    38,  -342,    39,     0,    40,    41,
      42,    43,    44,     0,    45,    46,    47,    48,   -68,    49,
      50,     0,    51,    52,    53,     0,  -342,     0,     0,    54,
      55,    56,    57,     0,    59,    60,  -342,   -68,    61,    62,
      63,    64,     0,    65,    66,    67,     0,    68,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,     0,    83,    84,   -68,    85,    86,
       0,     0,  -215,     0,    87,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,     0,   103,     0,  -215,     0,     4,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,  -215,    21,    22,    23,    24,    25,    26,     0,
    -215,    27,     0,    28,     0,    29,    30,    31,  -215,  -215,
    -215,    32,  1188,    34,   -68,     0,    35,    36,    37,     0,
      38,  -342,    39,     0,    40,    41,    42,    43,    44,     0,
      45,    46,    47,    48,   -68,    49,    50,     0,    51,    52,
      53,     0,  -342,     0,     0,    54,    55,    56,    57,     0,
      59,    60,  -342,   -68,    61,    62,    63,    64,     0,    65,
      66,    67,     0,    68,    69,    70,    71,    72,    73,    74,
      75,     0,    76,    77,     0,    78,    79,    80,    81,    82,
       0,    83,    84,   -68,    85,    86,     0,     0,  -215,     0,
      87,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
       0,   103,     0,  -215,     0,     4,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -215,    21,
      22,    23,    24,    25,    26,     0,  -215,    27,     0,    28,
       0,    29,    30,    31,  -215,  -215,  -215,    32,  1191,    34,
     -68,     0,    35,    36,    37,     0,    38,  -342,    39,     0,
      40,    41,    42,    43,    44,     0,    45,    46,    47,    48,
     -68,    49,    50,     0,    51,    52,    53,     0,  -342,     0,
       0,    54,    55,    56,    57,     0,    59,    60,  -342,   -68,
      61,    62,    63,    64,     0,    65,    66,    67,     0,    68,
      69,    70,    71,    72,    73,    74,    75,     0,    76,    77,
       0,    78,    79,    80,    81,    82,     0,    83,    84,   -68,
      85,    86,     0,     0,  -215,     0,    87,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     100,     0,     0,   101,     0,   102,     0,   103,     0,  -215,
       0,   873,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,   295,     0,   196,     0,     0,     0,    31,
    -215,  -215,  -215,     0,   197,    34,     0,     0,     0,   297,
       0,     0,    38,     0,   198,     0,   199,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   200,
       0,     0,     0,     0,     0,     0,     0,    70,   302,    72,
      73,    74,   303,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,     0,  -215,     0,     0,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,   631,     0,
      13,     0,     0,    16,    17,   633,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,   639,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   201,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,   507,   462,   463,   464,   465,   466,     0,     0,
     469,   470,   471,   472,     0,   474,   475,     0,   820,   821,
     822,   823,   824,   652,     0,   653,     0,    95,     0,   654,
     655,   656,   657,   658,   659,   660,   825,   662,   663,    97,
     826,    99,   -92,   665,   666,   827,   668,   202,     0,   103,
       0,     0,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,   631,     0,    13,     0,     0,    16,
      17,   633,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   198,   639,   199,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   200,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   201,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,   507,   462,
     463,   464,   465,   466,     0,     0,   469,   470,   471,   472,
       0,   474,   475,     0,   820,   821,   822,   823,   824,   652,
       0,   653,     0,    95,     0,   654,   655,   656,   657,   658,
     659,   660,   825,   662,   663,    97,   826,    99,     0,   665,
     666,   827,   668,   202,     0,   103,     0,     0,     0,     0,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,     0,    16,    17,     0,    18,     0,
     195,    20,     0,    21,     0,     0,     0,     0,    26,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,    33,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   198,     0,   199,     0,     0,    43,    44,     0,
      45,    46,    47,    48,     0,    49,    50,     0,    51,    52,
      53,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   200,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,    73,    74,
      75,     0,    76,    77,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   102,
       0,   103,     0,     0,     0,     0,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
     379,    23,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   198,     0,
     199,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,    56,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,    65,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   201,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
      85,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   202,     0,   103,     0,     0,
       0,     0,   104,     5,     6,     7,     8,     9,   382,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,    19,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
    -215,  -215,  -215,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   198,     0,   199,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,    64,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   201,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,    83,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,     0,  -215,     0,     0,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,  -729,     0,     0,     0,
    -729,     0,     0,    70,    71,    72,    73,    74,   201,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,  -384,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,  -384,     0,     0,   101,     0,   202,  -384,   103,
       0,     0,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   198,     0,   199,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   200,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   201,     0,    76,     0,     0,    78,
     317,   318,     0,    82,   353,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,   354,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   202,     0,   103,     0,     0,     0,     0,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,  -215,  -215,
    -215,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   198,     0,   199,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   200,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     201,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,     0,  -215,     0,     0,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,   736,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,   737,     0,    36,     0,     0,    38,     0,   198,     0,
     199,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,   738,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   201,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
     739,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   202,     0,   103,     0,     0,
       0,     0,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   198,     0,   199,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   200,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   201,     0,    76,     0,     0,    78,   317,   318,
       0,    82,   353,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,   797,     0,     0,     0,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   201,     0,
      76,     0,     0,    78,   317,   318,     0,    82,   353,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   202,     0,   103,
     799,     0,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   198,     0,   199,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   200,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   201,     0,    76,     0,     0,    78,
     317,   318,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   319,     0,
       0,   101,     0,   202,     0,   103,     0,     0,     0,     0,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   198,     0,   199,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   200,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     201,     0,    76,     0,     0,    78,   317,   318,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,  -536,     0,     0,     0,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   198,     0,
     199,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   201,     0,    76,     0,
       0,    78,   317,   318,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   202,     0,   103,     0,     0,
    -536,     0,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   198,     0,   199,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   200,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   201,     0,    76,     0,     0,    78,   317,   318,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,   804,     0,     0,     0,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,   941,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   201,     0,
      76,     0,     0,    78,   317,   318,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   202,     0,   103,
       0,     0,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   198,     0,   199,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   200,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   201,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
    -392,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,  -392,     0,
       0,   101,     0,   202,  -392,   103,     0,     0,     0,     0,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   198,     0,   199,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   200,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     201,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,  -377,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,  -377,     0,     0,   101,     0,   202,
    -377,   103,     0,     0,     0,     0,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   198,     0,
     199,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   201,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,  -385,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
    -385,     0,     0,   101,     0,   202,  -385,   103,     0,     0,
       0,     0,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,   259,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   198,     0,   199,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   200,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   201,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,   260,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,     0,     0,     0,     0,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   201,     0,
      76,     0,     0,    78,   317,   318,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   202,     0,   103,
       0,     0,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   198,     0,   199,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   200,  -729,     0,     0,     0,  -729,     0,     0,    70,
      71,    72,    73,    74,   201,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   202,     0,   103,     0,     0,     0,     0,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   198,     0,   199,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   200,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     201,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,   602,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,     0,     0,   603,     0,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   198,     0,
     199,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   201,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,  -413,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   202,     0,   103,  -413,     0,
       0,     0,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,   275,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   198,     0,   199,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   200,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   201,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   102,     0,   103,     0,     0,     0,     0,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   201,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,   602,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   202,     0,   103,
       0,     0,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   198,     0,   199,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   200,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   201,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   202,     0,   103,     0,     0,   603,     0,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   198,     0,   199,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   200,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     201,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
     795,   103,     0,     0,     0,     0,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   198,     0,
     199,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   201,     0,    76,     0,
       0,    78,     0,     0,     0,    82,   993,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   202,     0,   103,     0,     0,
       0,     0,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   198,     0,   199,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   200,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   201,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,     0,     0,  1016,     0,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   201,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   202,     0,   103,
       0,     0,  1021,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   198,     0,   199,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   200,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   201,     0,    76,     0,     0,    78,
       0,     0,     0,    82,  1165,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   202,     0,   103,     0,     0,     0,     0,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   198,     0,   199,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   200,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     201,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,     0,     0,     0,     0,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,   295,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,     0,     0,   297,     0,     0,    38,     0,   198,     0,
     199,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,     0,     0,     0,     0,     0,
       0,    70,   302,    72,    73,    74,   303,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   202,     0,   103,     0,     0,
       0,     0,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,  1054,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   198,     0,   199,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   200,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   201,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,     0,     0,     0,     0,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
    1212,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   201,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   202,     0,   103,
       0,     0,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,  1213,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   198,     0,   199,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   200,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   201,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   202,     0,   103,     0,     0,     0,     0,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,  1215,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   198,     0,   199,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   200,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     201,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,     0,     0,     0,     0,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,  1216,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   198,     0,
     199,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   201,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     6,     7,     8,     9,
      10,    11,    12,     0,    96,     0,    13,    97,    98,    99,
      17,     0,     0,   101,   195,   202,     0,   103,     0,   294,
       0,     0,   104,     0,     0,     0,     0,   196,     0,     0,
       0,     0,  -215,  -215,  -215,     0,   197,     0,     0,   296,
       0,     0,     0,   298,     0,     0,     0,     0,   199,     0,
       0,    43,    44,     0,     0,     0,     0,    48,     0,    49,
      50,     0,     0,     0,     0,     0,     0,     0,   299,     0,
      55,   300,    57,    58,    59,     0,     0,     0,     0,    62,
       0,   200,     0,   301,     0,     0,     0,     0,     0,    70,
       0,    72,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,   304,     0,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   202,     0,   103,  -346,  -215,     0,     0,
     104,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,     0,    17,     0,     0,     0,   195,
       0,     0,     0,     0,   294,     0,     0,     0,     0,     0,
       0,     0,   196,     0,     0,     0,     0,  -215,  -215,  -215,
       0,   197,     0,     0,   296,     0,     0,     0,   298,     0,
       0,     0,     0,   199,     0,     0,    43,    44,     0,     0,
       0,     0,    48,     0,    49,    50,     0,     0,     0,     0,
       0,     0,     0,   299,     0,    55,   300,    57,    58,    59,
       0,     0,     0,     0,    62,     0,   200,     0,   301,     0,
       0,     0,     0,     0,    70,     0,    72,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,   304,     0,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   629,     0,   630,     0,    90,
      91,    92,    93,    94,     0,     0,   631,     0,    95,     0,
       0,   632,   232,   633,   634,     0,     0,    96,     0,   635,
      97,    98,    99,     0,     0,     0,   101,   235,   202,   196,
     103,     0,  -215,   236,     0,   104,     0,     0,     0,   636,
       0,     0,     0,   238,     0,     0,   637,     0,   638,   639,
       0,     0,     0,     0,     0,     0,     0,   640,     0,     0,
       0,     0,     0,     0,   242,   641,     0,     0,     0,     0,
       0,     0,   244,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   642,     0,     0,     0,     0,     0,     0,     0,
       0,   246,   247,     0,   643,     0,   249,     0,   644,     0,
       0,   251,     0,     0,     0,   645,     0,     0,   253,     0,
       0,   646,     0,     0,     0,     0,     0,     0,     0,     0,
     507,   462,   463,   464,   465,   466,     0,     0,   469,   470,
     471,   472,     0,   474,   475,     0,   647,   648,   649,   650,
     651,   652,     0,   653,     0,     0,     0,   654,   655,   656,
     657,   658,   659,   660,   661,   662,   663,     0,   664,     0,
       0,   665,   666,   667,   668,     0,     0,   669,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,  -215,  -215,  -215,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   198,
       0,   199,     0,     0,    43,    44,     0,     0,    46,     0,
      48,     0,    49,    50,     0,    51,    52,     0,     0,     0,
       0,     0,     0,    55,     0,    57,     0,    59,     0,     0,
       0,     0,    62,    63,   200,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   201,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,     0,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,     0,
    -215,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,  -215,  -215,
    -215,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   198,     0,   199,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   200,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     201,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
      90,    21,    92,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,  -215,  -215,  -215,     0,
     197,    34,     0,    99,     0,    36,     0,     0,    38,   202,
     198,   103,   199,  -215,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   201,     0,
      76,     0,     0,    78,     0,   230,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,   231,
     232,     0,   233,     0,     0,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,   235,     0,     0,    90,     0,
      92,   236,     0,     0,     0,     0,     0,   237,     0,     0,
       0,   238,     0,     0,   239,     0,   240,     0,     0,     0,
       0,    99,     0,     0,     0,   241,     0,     0,     0,   103,
       0,  -215,   242,   243,     0,     0,     0,     0,     0,     0,
     244,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,     0,     0,     0,     0,     0,     0,     0,     0,   246,
     247,     0,   248,     0,   249,     0,   250,     0,   695,   251,
       0,     0,     0,   252,     0,     0,   253,   631,     0,   254,
       0,     0,   972,   973,   633,   974,     0,     0,     0,     0,
     975,     0,     0,     0,     0,     0,     0,     0,   697,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     976,     0,     0,     0,   977,     0,     0,   978,     0,   198,
     639,     0,     0,     0,     0,     0,     0,     0,   698,     0,
       0,     0,     0,     0,     0,   271,   979,     0,     0,     0,
       0,     0,     0,   980,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   981,     0,     0,     0,     0,     0,     0,
       0,     0,   982,   983,     0,   984,     0,   985,     0,   699,
       0,     0,     0,     0,     0,   700,   986,     0,     0,   987,
       0,     0,   988,     0,     0,     0,     0,     0,     0,     0,
       0,   507,   462,   463,   464,   465,   466,     0,     0,   469,
     470,   471,   472,     0,   474,   475,     0,   647,   648,   649,
     650,   651,   652,     0,   653,     0,     0,     0,   654,   655,
     656,   657,   658,   659,   660,   661,   662,   663,   230,   664,
       0,     0,   665,   666,   667,   668,   989,   631,     0,     0,
       0,     0,   231,   232,   633,   233,     0,     0,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,   235,     0,
       0,     0,     0,     0,   236,     0,     0,     0,     0,     0,
     237,     0,     0,     0,   238,     0,     0,   239,     0,   240,
     639,     0,     0,     0,     0,     0,     0,     0,   241,     0,
       0,     0,     0,     0,     0,   242,   243,     0,     0,     0,
       0,     0,     0,   244,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   245,     0,     0,     0,     0,     0,   431,
       0,     0,   246,   247,     0,   248,   432,   249,     0,   250,
       0,     0,   251,     0,     0,     0,   252,     0,   433,   253,
       0,     0,   254,     0,     0,     0,     0,     0,     0,     0,
       0,   507,   462,   463,   464,   465,   466,     0,     0,   469,
     470,   471,   472,     0,   474,   475,     0,   647,   648,   649,
     650,   651,   652,     0,   653,     0,     0,     0,   654,   655,
     656,   657,   658,   659,   660,   661,   662,   663,     0,   664,
       0,     0,   665,   666,   667,   668,     0,   434,     0,   431,
       0,   435,     0,     0,     0,     0,   432,     0,     0,     0,
     506,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,   507,   462,   463,   464,   465,   466,     0,
       0,   469,   470,   471,   472,     0,   474,   475,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,     0,   457,   458,   459,   434,   365,   431,
       0,   435,     0,     0,     0,   460,   432,     0,     0,     0,
    1187,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,   507,   462,   463,   464,   465,   466,     0,
       0,   469,   470,   471,   472,     0,   474,   475,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,     0,   457,   458,   459,   434,   365,   431,
       0,   435,     0,     0,     0,   460,   432,     0,     0,     0,
    1252,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,   507,   462,   463,   464,   465,   466,     0,
       0,   469,   470,   471,   472,     0,   474,   475,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,     0,   457,   458,   459,   434,   365,   431,
       0,   435,     0,     0,     0,   460,   432,     0,     0,     0,
    1253,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,   507,   462,   463,   464,   465,   466,     0,
       0,   469,   470,   471,   472,     0,   474,   475,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,     0,   457,   458,   459,   434,   365,   431,
       0,   435,     0,     0,     0,   460,   432,     0,     0,     0,
    1254,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,   507,   462,   463,   464,   465,   466,     0,
       0,   469,   470,   471,   472,     0,   474,   475,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,     0,   457,   458,   459,   434,   365,     0,
       0,   435,     0,     0,   431,   460,     0,     0,     0,     0,
    1255,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,   859,     0,     0,     0,     0,     0,
       0,     0,   436,   507,   462,   463,   464,   465,   466,   860,
       0,   469,   470,   471,   472,     0,   474,   475,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,     0,   457,   458,   459,     0,   365,     0,
     431,     0,   434,     0,     0,   460,   435,   432,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
     275,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   847,     0,   967,     0,   436,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,     0,   434,   457,
     458,   459,   435,   365,     0,     0,     0,     0,     0,     0,
     460,     0,   431,     0,     0,     0,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   847,
       0,   433,     0,   436,     0,     0,   589,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   590,     0,   457,   458,   459,     0,   365,
       0,     0,     0,     0,     0,     0,   460,     0,     0,     0,
     434,     0,     0,     0,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   431,   816,     0,     0,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,   436,     0,     0,   817,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   818,   591,   457,   458,   459,
       0,     0,     0,     0,     0,     0,     0,     0,   460,     0,
     431,     0,   434,     0,     0,     0,   435,   432,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
    1124,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,     0,   434,   457,
     458,   459,   435,     0,     0,   431,     0,     0,     0,     0,
     460,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,   275,     0,     0,     0,   847,
       0,     0,     0,   436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,     0,   457,   458,   459,     0,   365,
       0,   431,     0,   434,     0,     0,   460,   435,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,   275,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   847,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,     0,   434,
     457,   458,   459,   435,   365,     0,   431,     0,     0,     0,
       0,   460,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,   839,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,     0,     0,   457,   458,   459,     0,
     365,     0,   431,     0,   434,     0,     0,   460,   435,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,   901,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   214,     0,   902,     0,   436,
       0,     0,     0,     0,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,   432,     0,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   433,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,     0,
     434,   457,   458,   459,   435,     0,     0,     0,     0,     0,
       0,     0,   460,     0,   431,     0,     0,     0,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,   436,     0,     0,  1095,     0,
       0,     0,     0,     0,     0,   434,     0,     0,     0,   435,
       0,     0,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,  1096,   847,   457,   458,   459,
     436,     0,     0,     0,     0,     0,     0,     0,   460,     0,
       0,     0,   434,     0,     0,     0,   435,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
       0,   431,   457,   458,   459,     0,     0,   436,   432,     0,
       0,  1116,     0,   460,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   431,   531,   457,
     458,   459,     0,     0,   432,     0,     0,     0,     0,     0,
     460,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
       0,     0,     0,   435,     0,     0,     0,     0,   431,     0,
       0,     0,     0,     0,     0,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   433,     0,     0,
     847,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   434,     0,     0,     0,   435,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,     0,     0,   457,   458,   459,     0,
     436,     0,     0,     0,   431,  1163,   434,   460,     0,     0,
     435,   432,     0,     0,     0,     0,     0,   437,   438,     0,
     439,   440,   441,   433,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
       0,   436,   457,   458,   459,     0,     0,     0,     0,   431,
       0,     0,     0,   460,     0,     0,   432,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   433,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,   434,   457,   458,   459,   435,   555,     0,   431,
       0,     0,     0,     0,   460,     0,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,   436,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   434,     0,     0,
       0,   435,     0,     0,   437,   438,     0,   439,   440,   441,
     731,   442,   443,   444,     0,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,     0,   559,   457,
     458,   459,   436,     0,     0,     0,     0,   434,     0,     0,
     460,   435,     0,     0,     0,     0,     0,     0,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   436,     0,   457,   458,   459,     0,     0,     0,
     431,   812,     0,     0,     0,   460,     0,   432,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,   433,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,     0,   457,   458,   459,     0,     0,     0,
       0,   796,     0,     0,     0,   460,   431,     0,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,     0,
       0,     0,   435,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,     0,     0,   930,     0,
       0,     0,     0,   436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,     0,     0,     0,   435,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   813,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,     0,   457,   458,   459,   854,   436,
       0,     0,     0,     0,   434,     0,   460,   431,   435,     0,
       0,     0,     0,     0,   432,     0,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   433,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   436,
       0,   457,   458,   459,     0,     0,     0,   431,     0,     0,
       0,     0,   460,     0,   432,     0,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   433,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,     0,
       0,   457,   458,   459,     0,   434,     0,     0,     0,   435,
       0,     0,   460,   431,   999,     0,     0,     0,     0,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,   434,     0,     0,     0,   435,
       0,     0,     0,     0,     0,     0,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     436,     0,   457,   458,   459,     0,     0,     0,     0,   944,
       0,   434,     0,   460,     0,   435,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     431,     0,   457,   458,   459,     0,   436,   432,     0,   945,
       0,     0,     0,   460,     0,     0,     0,     0,     0,   433,
       0,     0,     0,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   431,  1092,   457,   458,
     459,     0,     0,   432,     0,     0,     0,     0,     0,   460,
       0,     0,     0,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,     0,     0,   436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,     0,     0,     0,   435,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,     0,   457,   458,   459,     0,   436,
       0,     0,  1046,     0,     0,     0,   460,   431,     0,   434,
       0,     0,     0,   435,   432,     0,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   433,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,     0,
     214,   457,   458,   459,   436,     0,     0,     0,     0,     0,
       0,     0,   460,     0,     0,     0,     0,     0,     0,     0,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,     0,   434,   457,   458,   459,   435,
       0,     0,   431,     0,     0,     0,     0,   460,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
       0,  1108,   457,   458,   459,     0,     0,     0,   431,     0,
     434,     0,     0,   460,   435,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1119,   436,     0,     0,     0,     0,
       0,     0,     0,   431,  1171,     0,     0,     0,     0,     0,
     432,     0,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   433,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,     0,   434,   457,   458,   459,
     435,     0,     0,   431,     0,     0,     0,     0,   460,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,  1149,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   434,     0,     0,     0,   435,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,     0,   457,   458,   459,   436,     0,     0,     0,
       0,   434,  1146,     0,   460,   435,     0,     0,     0,     0,
       0,     0,     0,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   436,     0,   457,   458,
     459,     0,     0,     0,   431,     0,     0,     0,     0,   460,
       0,   432,     0,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   433,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,     0,     0,   457,   458,
     459,     0,     0,     0,     0,   431,     0,     0,     0,   460,
       0,     0,   432,  1206,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,     0,     0,     0,   435,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,   432,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,   436,     0,     0,
       0,     0,     0,   434,     0,     0,     0,   435,     0,     0,
       0,     0,     0,     0,   437,   438,  1214,   439,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,     0,   436,   457,
     458,   459,     0,     0,     0,     0,   431,     0,   434,     0,
     460,     0,   435,   432,     0,   437,   438,     0,   439,   440,
     441,  1256,   442,   443,   444,   433,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,     0,     0,
     457,   458,   459,   436,     0,     0,     0,   431,     0,     0,
       0,   460,     0,     0,   432,     0,     0,     0,     0,     0,
     437,   438,     0,   439,   440,   441,   433,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,   434,   457,   458,   459,   435,     0,
       0,     0,   431,     0,     0,     0,   460,  1257,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,   436,
       0,     0,     0,     0,     0,   434,     0,     0,     0,   435,
       0,     0,     0,     0,     0,     0,   437,   438,  1258,   439,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,     0,
     436,   457,   458,   459,     0,     0,     0,     0,   431,     0,
     434,     0,   460,     0,   435,   432,     0,   437,   438,     0,
     439,   440,   441,  1259,   442,   443,   444,   433,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
       0,     0,   457,   458,   459,   436,     0,     0,     0,   431,
       0,     0,     0,   460,     0,     0,   432,     0,     0,     0,
       0,     0,   437,   438,     0,   439,   440,   441,   433,   442,
     443,   444,     0,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,     0,   434,   457,   458,   459,
     435,     0,     0,     0,     0,     0,     0,     0,   460,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,   436,     0,     0,     0,     0,     0,   920,     0,     0,
       0,   435,     0,     0,     0,     0,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,   436,   457,   458,   459,     0,     0,     0,     0,
       0,     0,     0,     0,   460,     0,     0,   434,     0,   437,
     438,   435,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,   361,   457,   458,   459,    22,    23,     0,
       0,     0,   436,     0,     0,   460,   362,     0,    29,   363,
       0,   433,     0,     0,    32,     0,     0,     0,     0,   437,
     438,    37,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,     0,   450,   451,   452,   453,   454,
     455,   456,     0,     0,   457,   458,   459,     0,    54,     0,
      56,   433,    58,     0,  1031,   460,     0,  1032,     0,     0,
     364,     0,    65,     0,     0,     0,     0,     0,     0,     0,
     434,     0,     0,     0,   435,     0,     0,     0,     0,     0,
       0,    81,     0,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
     434,     0,    89,     0,   435,     0,     0,     0,     0,     0,
       0,     0,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,   448,   100,   450,   451,
     452,   453,   454,  1172,   456,     0,     0,   457,   458,   459,
       0,     0,     0,     0,     0,     0,     0,     0,   460,     0,
       0,     0,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,     0,  -736,   446,   447,   448,     0,   450,   451,
     452,   453,  -736,     0,   456,     0,   361,   457,   458,   459,
      22,    23,     0,     0,     0,     0,     0,     0,   460,   362,
       0,    29,   363,     0,     0,     0,     0,    32,     0,     0,
       0,     0,     0,     0,    37,     0,   401,     0,   230,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   231,   232,     0,   233,     0,     0,     0,     0,
     234,    54,     0,    56,     0,   359,     0,  1031,   235,     0,
    1032,     0,     0,   364,   236,    65,     0,     0,     0,     0,
     237,     0,     0,     0,   238,     0,     0,   239,     0,   240,
       0,     0,     0,     0,    81,     0,     0,    83,   241,     0,
      85,     0,     0,     0,     0,   242,   243,     0,     0,     0,
       0,     0,     0,   244,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   245,     0,    89,   230,     0,     0,     0,
       0,     0,   246,   247,     0,   248,     0,   249,     0,   250,
     231,   232,   251,   233,     0,     0,   252,     0,   234,   253,
     100,     0,   254,     0,     0,     0,   235,     0,     0,     0,
       0,     0,   236,     0,     0,     0,     0,     0,   237,     0,
       0,     0,   238,     0,     0,   239,    89,   240,     0,     0,
       0,     0,   230,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,   242,   243,     0,   231,   232,     0,   233,
       0,   244,     0,     0,   234,     0,    23,     0,     0,     0,
       0,   245,   235,     0,     0,     0,     0,     0,   236,     0,
     246,   247,     0,   248,   237,   249,     0,   250,   238,     0,
     251,   239,     0,   240,   252,     0,     0,   253,   230,     0,
     254,     0,   241,     0,     0,     0,     0,     0,     0,   242,
     243,     0,   231,   232,     0,   233,     0,   244,    56,     0,
     234,     0,     0,     0,    89,     0,     0,   245,   235,     0,
      65,     0,     0,     0,   236,     0,   246,   247,     0,   248,
     237,   249,     0,   250,   238,     0,   251,   239,     0,   240,
     252,     0,     0,   253,  -317,    85,   254,     0,   241,     0,
       0,     0,     0,     0,     0,   242,   243,     0,  -317,  -317,
       0,  -317,     0,   244,     0,     0,  -317,     0,     0,     0,
       0,     0,     0,   245,  -317,     0,     0,     0,     0,     0,
    -317,     0,   246,   247,     0,   248,  -317,   249,     0,   250,
    -317,     0,   251,  -317,     0,  -317,   252,   544,     0,   253,
     230,     0,   254,     0,  -317,     0,     0,     0,     0,     0,
       0,  -317,  -317,     0,   231,   232,     0,   233,     0,  -317,
       0,     0,   234,     0,     0,     0,     0,     0,     0,  -317,
     235,     0,     0,     0,     0,     0,   236,     0,  -317,  -317,
       0,  -317,   237,  -317,     0,  -317,   238,     0,  -317,   239,
       0,   240,  -317,     0,     0,  -317,  -318,     0,  -317,     0,
     241,     0,     0,     0,     0,     0,     0,   242,   243,     0,
    -318,  -318,     0,  -318,     0,   244,     0,     0,  -318,     0,
       0,     0,     0,     0,     0,   245,  -318,     0,     0,     0,
       0,     0,  -318,     0,   246,   247,     0,   248,  -318,   249,
       0,   250,  -318,     0,   251,  -318,     0,  -318,   252,     0,
       0,   253,     0,     0,   254,     0,  -318,     0,     0,     0,
       0,     0,     0,  -318,  -318,     0,     0,     0,     0,     0,
       0,  -318,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -318,     0,     0,     0,     0,     0,     0,     0,     0,
    -318,  -318,     0,  -318,     0,  -318,     0,  -318,     0,     0,
    -318,     0,     0,     0,  -318,     0,     0,  -318,     0,     0,
    -318
};

static const yytype_int16 yycheck[] =
{
       2,     2,    20,    62,   402,   152,   181,    26,   112,   266,
     488,   165,     2,    67,    62,   553,    62,   229,   422,   223,
     513,   393,   557,    25,   672,   622,   674,   818,   785,   625,
     589,   383,   133,    87,   291,    25,   906,   548,   866,   813,
       1,   203,    47,   578,   836,     1,   779,    66,    32,   103,
       1,    24,     2,   407,   266,    32,    32,    22,    32,    32,
      32,    32,   434,   435,    56,    32,     1,    40,    41,    42,
     103,   523,   121,    75,   331,    25,    24,    79,    80,   291,
     148,    32,    27,   148,    32,    75,    35,   262,     0,    79,
      80,   518,    40,    41,    42,    61,   946,    32,   166,     1,
     102,   166,    47,   872,   106,    24,    51,   124,   480,   481,
      61,   121,   102,    32,    75,    56,   106,    47,   545,   331,
      75,    40,    41,    42,    89,    75,    61,   176,   166,    79,
      80,    76,    47,   148,    79,   604,    81,    91,   171,   608,
     609,  1011,     3,   148,   121,    47,    91,   120,   113,   121,
     148,   166,   102,   119,   171,   106,   106,   118,    40,    41,
      42,    91,   111,   118,   148,   110,   176,    31,   119,   766,
     126,   106,   120,   625,  1002,     2,    56,   148,   176,   948,
      46,   173,   121,   940,   119,   121,    27,   171,   542,   946,
     166,   147,   148,    54,   171,   171,   147,   171,    25,   171,
     171,   120,   175,   575,   171,   367,  1034,  1035,    47,  1001,
      56,   172,   147,   215,   749,   166,   949,   172,   174,    85,
     171,   478,   479,   997,   126,   215,    90,   175,  1078,   174,
      94,   443,   173,   148,    18,    27,   171,   176,    79,   732,
     592,     3,   119,   104,     3,   147,   148,   422,    75,   115,
      91,    31,    79,    80,    47,    47,   175,   166,   817,    51,
      47,   176,   148,   172,   166,   215,   478,   479,   270,   110,
      36,   148,   783,   275,  1102,   102,   110,    27,   142,   106,
     270,   382,   121,   147,    76,   275,   150,    79,   174,    81,
     154,    56,    54,   175,    78,    54,    56,   148,   402,    91,
    1128,   513,  1059,  1094,    91,  1096,  1176,    73,   148,   148,
      90,   173,   141,    97,    94,    53,    11,     3,   110,   148,
     270,  1078,   919,   174,   148,   275,    76,   531,   121,    79,
     114,     2,   148,   802,   174,    31,    74,   176,   492,   148,
      45,    91,   104,   172,    56,   557,    84,   406,   172,   351,
     351,   947,   148,    27,    25,   148,    83,    46,   174,    86,
     110,   351,   142,   365,    69,   174,   578,   147,    54,   149,
     150,   864,   374,   392,   154,   365,     3,    66,   174,   398,
      56,   161,   174,   176,   374,   387,   126,   148,   215,   169,
     148,   595,   173,    20,    90,   148,    85,   387,    94,  1047,
    1048,   351,    56,   148,    75,    79,    33,    93,    79,    80,
     903,   172,   152,   467,   468,   365,   174,    91,   104,   473,
     422,   174,   476,   477,   374,   104,   115,    54,   906,   174,
     989,   102,   148,   968,   148,   106,    63,   387,   166,   148,
    1135,    56,   116,   270,    56,   173,   142,   143,   275,   145,
     146,   147,   166,   149,   150,   151,   172,   166,   154,   155,
     156,   148,   158,   159,   160,   161,   485,   839,   164,   173,
     868,   167,   168,   169,    46,   148,    56,   104,   850,   883,
     148,   142,   178,   110,   543,   148,   148,   174,   860,   150,
      79,    80,   148,   148,    66,   543,   148,   543,   166,   148,
       8,   174,   504,   148,   506,  1200,  1201,  1202,  1203,  1204,
    1205,   174,   174,    85,   504,   569,   506,   571,   174,   174,
     732,   166,   174,   148,   351,   174,   173,   899,   175,     2,
     902,    31,   126,  1011,  1093,   148,  1095,   749,   365,    56,
     173,   166,   175,   115,   215,  1014,  1015,   374,   920,   921,
     148,   148,    25,   166,   504,   574,   506,  1026,  1027,   153,
     387,   155,   156,   166,   158,   159,   148,   148,   166,   166,
     173,   166,   175,   278,    90,   280,   101,   172,    94,   597,
     582,   582,   287,    90,   166,   166,   173,    94,   175,   173,
      90,   175,   582,   965,    94,   967,   171,    56,   173,   270,
     812,   813,    75,  1141,   275,    90,    79,    80,    90,    94,
      90,    90,    94,   173,    94,    94,    56,  1086,  1087,  1088,
    1089,    56,   327,   142,    27,   173,    56,   175,   147,   102,
      56,   150,   582,   106,   173,   154,   175,   173,     3,   175,
    1038,   150,   142,   143,    47,   145,   146,   147,    51,   149,
     150,   569,   864,   571,   154,    79,    80,   141,  1196,   171,
     160,   161,    56,    50,   164,   166,    53,   167,   168,   169,
     292,   293,   147,    76,   173,    27,    79,   504,    81,   506,
     351,    56,   165,   173,   838,   166,    85,    74,    91,   166,
      77,   903,   166,   747,   365,    47,   166,    84,  1176,    51,
     166,   166,   166,   374,   171,   166,   166,   110,   883,   171,
     173,   148,     8,   147,   166,   769,   387,   173,   173,   866,
     174,   166,   869,   110,    76,   173,     3,    79,    66,    81,
     173,    22,    79,   171,   126,    26,    27,    91,    91,    91,
     742,   172,   215,   126,    35,   166,    37,    38,   150,    69,
     173,   753,    43,   126,   756,   582,   968,   174,   110,    50,
     148,    34,    53,   753,   126,   126,   756,    34,    66,   174,
     126,   147,    21,   174,   174,    66,   171,   120,   148,   106,
     173,   150,   173,    74,   150,   142,    77,   150,    79,   166,
     150,   150,    83,    84,    85,    86,     3,   270,    89,   150,
      91,   150,   275,   753,   150,   150,   756,   866,   150,   150,
      17,   870,   176,    20,   150,  1072,  1073,   150,    25,   110,
     879,   150,   113,   173,   150,   116,   845,   166,    35,   166,
     174,   166,   166,   504,   166,   506,    34,    34,    45,   147,
      34,   172,   148,   166,   174,    52,   848,    54,    47,   171,
     852,   176,   173,   172,   166,  1002,    63,   859,   848,   176,
    1072,  1073,   852,   174,    71,   166,   176,   147,   173,   859,
      25,   173,   166,   148,   174,   166,    32,   176,   351,   174,
     171,    88,    34,   176,   176,   166,   174,  1034,   174,   148,
     166,   165,   365,   100,   166,   600,   601,   104,   848,   165,
     111,   374,   852,   608,   111,   577,   968,   265,    24,   859,
     117,   582,   810,   391,   387,   936,   374,   367,   920,   921,
      75,   339,   961,   529,   543,   927,   753,   929,   385,   756,
     932,  1035,   870,   879,  1038,   540,   883,   927,   957,   929,
    1196,  1040,   932,  1002,  1206,  1120,  1199,   837,   678,   131,
     305,   106,   526,   765,   956,  1102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   966,    -1,   956,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     2,    -1,   966,   927,    -1,   929,
      -1,    -1,   932,    -1,    -1,    -1,    -1,  1005,  1006,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    22,
      -1,    -1,    -1,    26,    27,    -1,   956,    -1,    -1,    -1,
      -1,  1013,    35,    -1,    37,    38,   966,  1019,    -1,    -1,
      43,   848,    -1,  1013,  1128,   852,    -1,    50,    -1,  1019,
      -1,   504,   859,   506,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1102,    -1,    -1,    -1,    75,    -1,    -1,
      -1,    79,    80,  1228,    77,  1230,    79,    -1,  1233,  1234,
     215,    -1,    -1,  1013,  1066,    -1,    89,    -1,    91,  1019,
      -1,    -1,    -1,    -1,   102,    -1,  1066,    -1,   106,    -1,
      -1,    -1,   753,    -1,    -1,   756,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
     927,    -1,   929,    -1,    -1,   932,    -1,    -1,    -1,   582,
      -1,    -1,  1114,    -1,  1116,   270,  1066,    -1,    -1,  1121,
     275,    -1,  1124,    -1,  1114,    -1,  1116,    -1,    -1,   956,
      -1,  1121,    -1,    -1,  1124,   840,    -1,    -1,    -1,   966,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1152,    -1,    -1,   858,    -1,    -1,  1158,   862,    -1,    -1,
    1162,  1163,  1152,    31,  1114,    -1,  1116,    -1,  1158,    -1,
      -1,  1121,  1162,  1163,  1124,    -1,    -1,   848,    -1,  1181,
      -1,   852,    -1,  1185,    -1,  1187,  1013,   215,   859,    -1,
      -1,  1181,  1019,    -1,    -1,  1185,    31,  1187,    -1,    -1,
      -1,    -1,  1152,    -1,    -1,  1207,  1207,    -1,  1158,    -1,
      -1,    -1,  1162,  1163,    -1,    -1,    -1,  1207,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,
      -1,  1181,   387,    -1,    -1,  1185,    -1,  1187,    -1,  1066,
      -1,    -1,   270,    -1,    -1,    -1,    -1,   275,    -1,    -1,
    1252,  1253,  1254,  1255,    -1,    90,   927,  1207,   929,    94,
      -1,   932,  1252,  1253,  1254,  1255,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
     753,   149,   150,   756,    -1,   956,   154,  1114,    -1,  1116,
       2,    -1,    -1,   161,  1121,   966,    -1,  1124,    -1,   167,
     168,   169,  1252,  1253,  1254,  1255,    -1,   142,   143,    -1,
      -1,   146,   147,    25,   149,   150,    -1,    -1,    -1,   154,
      -1,  1026,  1027,   351,    -1,  1152,   161,    -1,    -1,    -1,
      -1,  1158,   167,   168,   169,  1162,  1163,   365,    -1,    -1,
      -1,    -1,  1013,    -1,    -1,    -1,   374,    -1,  1019,   504,
      -1,   506,    -1,    -1,  1181,    -1,    -1,    -1,  1185,   387,
    1187,    -1,    -1,    75,    -1,    -1,    -1,    79,    80,    -1,
      -1,    -1,    -1,    -1,    -1,   848,    -1,  1082,    -1,   852,
    1207,    -1,    -1,  1088,  1089,    -1,   859,    -1,    -1,    -1,
     102,    -1,    -1,    -1,   106,  1066,    -1,    -1,    -1,    -1,
      -1,  1106,  1107,    -1,  1109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,  1123,    -1,
      -1,  1126,    -1,    -1,    -1,  1252,  1253,  1254,  1255,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    25,    -1,
      -1,    -1,    -1,  1114,    -1,  1116,    -1,     2,    -1,    -1,
    1121,    -1,    -1,  1124,   927,    -1,   929,    -1,    -1,   932,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      25,    -1,    -1,    -1,    -1,    -1,   504,    -1,   506,    -1,
      -1,  1152,    -1,   956,    -1,    -1,    -1,  1158,    75,    -1,
      -1,  1162,  1163,   966,    -1,    -1,    90,    -1,    -1,    -1,
      94,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,
    1181,    -1,    -1,    -1,  1185,   102,  1187,    -1,    -1,   106,
      75,    -1,    -1,    -1,    79,    80,    -1,    -1,    90,    -1,
      -1,    -1,    94,    -1,    -1,    -1,  1207,    -1,    -1,    -1,
    1013,    -1,    -1,    -1,    -1,    -1,  1019,   102,   142,   143,
      -1,   106,    -1,   147,   582,   149,   150,    -1,   270,    -1,
     154,    -1,    -1,   275,    -1,    -1,    -1,   161,    -1,    -1,
      -1,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,
     142,  1252,  1253,  1254,  1255,   147,    -1,   149,   150,    -1,
      -1,    -1,   154,  1066,    -1,    -1,    -1,    -1,   753,   161,
      -1,   756,    -1,    -1,    22,   167,   168,   169,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    37,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,   215,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,   351,
      -1,  1114,    -1,  1116,    -1,    -1,    -1,    -1,  1121,    -1,
      -1,  1124,    -1,   365,    -1,    -1,    -1,    -1,    -1,    77,
     215,    79,   374,    81,    -1,    83,    67,    -1,    86,    -1,
      -1,    89,    -1,    91,    -1,   387,    -1,    -1,    -1,  1152,
      -1,    -1,    -1,   270,    -1,  1158,    87,    -1,   275,  1162,
    1163,    -1,   110,   848,    -1,   113,    -1,   852,   116,    -1,
      -1,    -1,   103,    -1,   859,    -1,    -1,    -1,  1181,    -1,
      -1,    -1,  1185,    -1,  1187,   270,    -1,    -1,    -1,    -1,
     275,    -1,    -1,   141,    -1,   753,    -1,    -1,   756,    -1,
      -1,    -1,    -1,    -1,  1207,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
      -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   351,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   927,    -1,   929,    -1,    -1,   932,   365,  1252,
    1253,  1254,  1255,    -1,    -1,    -1,    -1,   374,    -1,    -1,
      -1,    -1,   504,    -1,   506,    -1,   351,    -1,    -1,    -1,
     387,   956,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     365,   966,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   374,
     848,    -1,    -1,    -1,   852,    -1,    -1,    -1,   229,    -1,
      -1,   859,   387,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1013,    -1,
      17,    18,    -1,    20,  1019,   266,    -1,    -1,    25,    -1,
     582,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
     291,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,   927,
      -1,   929,    -1,    -1,   932,    -1,    63,    -1,    -1,    -1,
      -1,  1066,    -1,    70,    71,    -1,    -1,   504,    -1,   506,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,   956,    -1,
     331,    88,    -1,    -1,    -1,    -1,    -1,    -1,   966,    -1,
      97,    98,    -1,   100,    -1,   102,    -1,   104,    -1,   504,
     107,   506,    -1,    -1,   111,    -1,    -1,   114,    -1,  1114,
     117,  1116,    -1,    -1,    -1,    -1,  1121,    -1,    -1,  1124,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,  1013,    -1,    -1,    -1,    -1,
      -1,  1019,    -1,    -1,    -1,   582,    -1,  1152,    -1,    -1,
      -1,    -1,    -1,  1158,    -1,    -1,    -1,  1162,  1163,    -1,
      -1,    -1,    -1,    -1,   170,    -1,   173,   173,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,  1181,   582,    -1,    -1,
    1185,    -1,  1187,    -1,    -1,    -1,    -1,    -1,  1066,    -1,
      -1,   753,    -1,    -1,   756,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   467,   468,    -1,    -1,
      -1,    -1,   473,    -1,    -1,   476,   477,   478,   479,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1114,    -1,  1116,    -1,
      -1,    -1,    -1,  1121,    -1,    -1,  1124,  1252,  1253,  1254,
    1255,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   513,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1152,    -1,    -1,    -1,    -1,    -1,
    1158,    -1,    -1,    -1,  1162,  1163,   848,    -1,    -1,    -1,
     852,    -1,    -1,    -1,    -1,    -1,    -1,   859,    -1,    -1,
      -1,    -1,    -1,  1181,    -1,     1,   557,  1185,    -1,  1187,
      -1,    -1,    -1,    -1,    -1,    -1,   753,    -1,   569,   756,
     571,    -1,    -1,    -1,    -1,    -1,    22,   578,    -1,  1207,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    37,    38,    -1,    -1,    -1,    -1,    43,   753,    -1,
      -1,   756,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   927,    -1,   929,    -1,    -1,
     932,    -1,    -1,    -1,  1252,  1253,  1254,  1255,    74,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    83,    84,    -1,
      86,    -1,    -1,    89,   956,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   966,    -1,    -1,    -1,    -1,    -1,
      -1,   848,    -1,    -1,   110,   852,    -1,   113,    -1,    -1,
     116,    -1,   859,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   848,    -1,   141,    -1,   852,    -1,    -1,
      -1,  1013,    -1,    -1,   859,    -1,    -1,  1019,    -1,    22,
      -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,
     166,    28,    35,    -1,    37,    38,   172,    -1,    -1,    -1,
      43,   732,    -1,    40,    41,    42,    -1,    50,    -1,    -1,
     927,    -1,   929,    -1,    -1,   932,   747,    -1,   749,    -1,
      -1,    -1,    -1,    -1,  1066,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,   769,   956,
      83,    -1,   927,    86,   929,    -1,    89,   932,    91,   966,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
     113,   956,  1114,   116,  1116,    -1,    -1,    -1,    -1,  1121,
      -1,   966,  1124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1013,    -1,   141,    -1,
      -1,    -1,  1019,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1152,    -1,    -1,   150,    90,   152,  1158,    -1,    94,    -1,
    1162,  1163,    -1,   166,    -1,    -1,    -1,   164,  1013,    -1,
      -1,    -1,    -1,   864,  1019,    -1,    -1,    -1,    -1,  1181,
      -1,    -1,    -1,  1185,    -1,  1187,    -1,    -1,    -1,  1066,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   199,    -1,    -1,  1207,   142,   143,    -1,   145,
     146,   147,   903,   149,   150,   151,    -1,   153,   154,   155,
     156,  1066,   158,   159,   160,   161,   162,    -1,   164,    -1,
      -1,   167,   168,   169,    -1,    -1,    -1,  1114,    -1,  1116,
      -1,    -1,   178,    -1,  1121,    -1,    -1,  1124,    -1,    -1,
    1252,  1253,  1254,  1255,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   259,   260,    -1,    -1,    -1,    -1,    -1,  1114,
      -1,  1116,    -1,    -1,   271,  1152,  1121,   968,    -1,  1124,
      -1,  1158,    -1,    -1,    -1,  1162,  1163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1181,    -1,    -1,  1152,  1185,    -1,
    1187,    -1,    -1,  1158,   311,    -1,    -1,  1162,  1163,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1207,    -1,    -1,    -1,    -1,    -1,  1181,    45,    -1,    -1,
    1185,    -1,  1187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,  1207,    -1,    -1,   362,    -1,    -1,    -1,    -1,
      -1,    79,    80,    -1,    -1,  1252,  1253,  1254,  1255,    13,
      -1,  1072,  1073,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    33,
     397,    -1,    -1,   111,    -1,   402,    -1,  1252,  1253,  1254,
    1255,    45,    -1,    47,    48,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    57,    -1,   422,    -1,    -1,    62,    -1,
      -1,    -1,    66,    67,    68,    69,    70,    71,    -1,    -1,
      -1,    75,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    87,    -1,    -1,    90,    -1,    92,    -1,
      -1,    95,    96,    -1,    98,    -1,    -1,    -1,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     487,    -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    -1,    -1,   221,    -1,    -1,    -1,   514,    -1,    -1,
      -1,   518,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,   531,    -1,    -1,    -1,    -1,    33,
     174,    -1,    -1,   540,    -1,    39,    -1,    -1,   545,    -1,
      -1,    45,    -1,   261,   262,    49,    -1,    -1,    52,    -1,
      54,   269,    -1,   197,    -1,    -1,    -1,   201,   202,    63,
     278,    -1,   280,    -1,    -1,    -1,    70,    71,    -1,   287,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,   588,    -1,    -1,    88,   229,    -1,    -1,    -1,    -1,
      -1,    -1,   599,    97,    98,    -1,   100,     3,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,   112,   327,
     114,    17,    18,   117,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,   266,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,   291,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,   302,   303,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,   173,
      -1,   678,    78,   317,   318,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    -1,   331,    -1,    -1,
      -1,    97,    98,    -1,   100,    -1,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,   112,    -1,   114,    -1,
     354,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   375,    -1,   377,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   386,    -1,    -1,    -1,    -1,   391,   392,   393,
      -1,   758,    -1,    -1,   398,    -1,    -1,    -1,   765,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,   775,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   793,   431,   432,   433,
     434,   435,   436,    -1,   438,   439,   440,   441,   442,    -1,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,    -1,    -1,    -1,
      -1,    -1,    -1,   467,   468,    -1,    -1,    -1,    -1,   473,
     837,    -1,   476,   477,   478,   479,   480,   481,    -1,    -1,
      -1,   485,   486,    -1,   488,    -1,    -1,    -1,    -1,    -1,
     494,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   868,   506,    -1,    -1,   509,    -1,    -1,    -1,   513,
      -1,    -1,    -1,    -1,    -1,    -1,   883,    -1,    -1,   523,
      -1,   525,   600,   601,    -1,   529,    -1,    -1,   532,   533,
     608,   609,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   543,
      -1,    -1,     3,    -1,   548,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   557,    19,    -1,    17,    18,    -1,    20,
      -1,    -1,   566,    -1,    25,   569,    31,   571,    -1,   936,
     574,   575,    33,    -1,   578,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,   589,   590,    -1,    49,    -1,
      -1,    52,    -1,    54,   961,    -1,    -1,    -1,    -1,   603,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,   615,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,   625,    -1,    -1,    -1,    90,    -1,    88,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,   103,   100,
      -1,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,   669,    -1,    -1,    -1,    -1,
      -1,  1038,   676,    -1,    -1,    -1,    -1,   142,   143,   757,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,    -1,   171,    -1,    -1,    -1,
     171,    -1,   173,   178,    -1,   793,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   802,    -1,    -1,   731,   732,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,   747,    -1,   749,   750,   751,    35,    -1,
      37,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,
      -1,    -1,   840,    50,    -1,   769,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   783,
     858,    -1,    -1,   787,   862,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    86,
      -1,    -1,    89,    -1,    91,    -1,   810,    -1,    -1,    -1,
     814,    -1,   816,   817,   818,    -1,   820,    -1,   822,    -1,
      -1,   825,   826,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,   839,    -1,   841,    -1,    -1,
      -1,   845,    -1,    -1,    -1,    -1,   850,   851,   852,    -1,
     854,   855,    -1,    -1,   141,   859,   860,    -1,    -1,   937,
     864,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,   172,   964,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   899,   900,   901,   902,   903,
      -1,    -1,   906,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   920,   921,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   930,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   947,    -1,    -1,    -1,   951,  1026,  1027,
      -1,    -1,    -1,   957,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   965,    -1,   967,   968,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   989,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   997,    -1,   999,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1082,  1009,    -1,  1011,    -1,    -1,
    1088,  1089,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1024,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1106,  1107,
      -1,  1109,    -1,    -1,    -1,    -1,    -1,    -1,  1042,    -1,
      -1,    -1,  1120,    -1,    -1,  1123,    -1,    -1,  1126,    -1,
    1054,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1072,  1073,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1084,    -1,  1160,    -1,    -1,    -1,    -1,    -1,  1092,  1093,
    1094,  1095,  1096,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1114,    -1,  1116,    -1,    -1,  1119,    -1,  1121,    -1,    -1,
    1124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1145,  1146,    -1,  1148,  1149,    -1,    -1,    -1,    -1,
    1228,    -1,  1230,    -1,    -1,  1233,  1234,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1171,    -1,    -1,
      -1,    -1,  1176,    -1,  1178,    -1,    -1,    -1,  1182,    -1,
      -1,    -1,    -1,  1187,  1188,    -1,    -1,  1191,    -1,    -1,
      -1,    -1,    -1,    -1,  1198,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1212,  1213,
    1214,  1215,  1216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1252,  1253,
    1254,  1255,  1256,  1257,  1258,  1259,     0,     1,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,    -1,
     104,   105,    -1,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,    -1,    -1,   120,    -1,   122,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,   166,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,   175,    -1,     1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    12,    -1,    14,    15,    16,    17,
      18,    19,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,
      78,    79,    80,    -1,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,   105,    -1,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
      -1,    -1,   120,    -1,   122,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,    -1,   171,   172,   173,    -1,   175,    -1,     1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      92,    93,    -1,    95,    96,    97,    98,    99,   100,   101,
     102,    -1,   104,   105,    -1,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,    -1,    -1,   120,    -1,
     122,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,   166,    -1,    -1,   169,    -1,   171,
     172,   173,    -1,   175,    -1,     1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,    -1,   104,   105,
      -1,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,    -1,    -1,   120,    -1,   122,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
     166,    -1,    -1,   169,    -1,   171,   172,   173,    -1,   175,
      -1,     1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    71,    72,    -1,    74,    -1,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    91,    92,    93,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,   105,    -1,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,
      -1,   171,   172,   173,    -1,   175,    -1,     1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,    -1,
     104,   105,    -1,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,    -1,    -1,   120,    -1,   122,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,   166,    -1,    -1,   169,    -1,   171,   172,   173,
      -1,   175,    -1,     1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,   105,    -1,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
      -1,    -1,   120,    -1,   122,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,     1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      92,    93,    -1,    95,    96,    97,    98,    99,   100,   101,
     102,    -1,   104,   105,    -1,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,    -1,    -1,   120,    -1,
     122,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,   166,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,   175,    -1,     1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    -1,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,    -1,   104,   105,
      -1,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,    -1,    -1,   120,    -1,   122,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
     166,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,
      -1,     1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    71,    72,    -1,    74,    -1,    -1,    77,    78,    79,
      80,    -1,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    91,    92,    93,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,   105,    -1,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,   175,    -1,     1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    -1,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,    -1,
     104,   105,    -1,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,    -1,    -1,   120,    -1,   122,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,   166,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,   175,    -1,     1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,
      78,    79,    80,    -1,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,   105,    -1,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
      -1,    -1,   120,    -1,   122,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,     1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      92,    93,    -1,    95,    96,    97,    98,    99,   100,   101,
     102,    -1,   104,   105,    -1,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,    -1,    -1,   120,    -1,
     122,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,   166,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,   175,    -1,     1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    -1,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,    -1,   104,   105,
      -1,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,    -1,    -1,   120,    -1,   122,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
     166,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,
      -1,     1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    42,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,   175,    -1,    -1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    12,    -1,
      14,    -1,    -1,    17,    18,    19,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    55,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,    -1,    -1,
     134,   135,   136,   137,    -1,   139,   140,    -1,   142,   143,
     144,   145,   146,   147,    -1,   149,    -1,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,    -1,   173,
      -1,    -1,    -1,    -1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    12,    -1,    14,    -1,    -1,    17,
      18,    19,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    55,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,    -1,    -1,   134,   135,   136,   137,
      -1,   139,   140,    -1,   142,   143,   144,   145,   146,   147,
      -1,   149,    -1,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,   167,
     168,   169,   170,   171,    -1,   173,    -1,    -1,    -1,    -1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    -1,    17,    18,    -1,    20,    -1,
      22,    23,    -1,    25,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,
      62,    63,    64,    65,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,   101,
     102,    -1,   104,   105,    -1,   107,    -1,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,    -1,    -1,    -1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      26,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,
      -1,    -1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    42,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,   113,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,   175,    -1,    -1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    -1,
      94,    -1,    -1,    97,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,   148,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,   166,    -1,    -1,   169,    -1,   171,   172,   173,
      -1,    -1,    -1,    -1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
     108,   109,    -1,   111,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,    -1,    -1,    -1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      42,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,   175,    -1,    -1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    47,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,
      -1,    -1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,   102,    -1,   104,    -1,    -1,   107,   108,   109,
      -1,   111,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,   174,    -1,    -1,    -1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,   108,   109,    -1,   111,   112,    -1,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
     174,    -1,    -1,    -1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
     108,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,    -1,    -1,    -1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,   108,   109,    -1,   111,
      -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,   174,    -1,    -1,    -1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   102,    -1,   104,    -1,
      -1,   107,   108,   109,    -1,   111,    -1,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,
     176,    -1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,   102,    -1,   104,    -1,    -1,   107,   108,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,   174,    -1,    -1,    -1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    69,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,   108,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,    -1,    -1,    -1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
     148,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,
      -1,   169,    -1,   171,   172,   173,    -1,    -1,    -1,    -1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,   148,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,   166,    -1,    -1,   169,    -1,   171,
     172,   173,    -1,    -1,    -1,    -1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,   148,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
     166,    -1,    -1,   169,    -1,   171,   172,   173,    -1,    -1,
      -1,    -1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,    -1,   114,    -1,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,    -1,    -1,    -1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,   108,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,    -1,    -1,    -1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    -1,    94,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,    -1,    -1,    -1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,   123,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,    -1,   176,    -1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,   148,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,   174,    -1,
      -1,    -1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,    -1,    -1,    -1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,   123,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,    -1,    -1,    -1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,    -1,   176,    -1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
     172,   173,    -1,    -1,    -1,    -1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,   112,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,
      -1,    -1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,    -1,   176,    -1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,    -1,   176,    -1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,    -1,    -1,    -1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,    -1,    -1,    -1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,
      -1,    -1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,    -1,    -1,    -1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,    -1,    -1,    -1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,    -1,    -1,    -1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,    -1,    -1,    -1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,     4,     5,     6,     7,
       8,     9,    10,    -1,   160,    -1,    14,   163,   164,   165,
      18,    -1,    -1,   169,    22,   171,    -1,   173,    -1,    27,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    42,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    -1,    -1,    65,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      78,    79,    80,    81,    82,    -1,    -1,    -1,    -1,    87,
      -1,    89,    -1,    91,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    99,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,   174,   175,    -1,    -1,
     178,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    18,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    42,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      -1,    -1,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    79,    80,    81,    82,
      -1,    -1,    -1,    -1,    87,    -1,    89,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    99,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,     3,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    12,    -1,   151,    -1,
      -1,    17,    18,    19,    20,    -1,    -1,   160,    -1,    25,
     163,   164,   165,    -1,    -1,    -1,   169,    33,   171,    35,
     173,    -1,   175,    39,    -1,   178,    -1,    -1,    -1,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    -1,   100,    -1,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,   128,   129,   130,   131,    -1,    -1,   134,   135,
     136,   137,    -1,   139,   140,    -1,   142,   143,   144,   145,
     146,   147,    -1,   149,    -1,    -1,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,   164,    -1,
      -1,   167,   168,   169,   170,    -1,    -1,   173,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    42,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,    -1,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
     175,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      42,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
     142,    25,   144,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    42,    -1,
      44,    45,    -1,   165,    -1,    49,    -1,    -1,    52,   171,
      54,   173,    56,   175,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,    -1,     3,    -1,   111,    -1,    -1,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,   142,    -1,
     144,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    63,    -1,    -1,    -1,   173,
      -1,   175,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    -1,   100,    -1,   102,    -1,   104,    -1,     3,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,    12,    -1,   117,
      -1,    -1,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,   173,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    -1,   100,    -1,   102,    -1,   104,
      -1,    -1,    -1,    -1,    -1,   110,   111,    -1,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,   129,   130,   131,    -1,    -1,   134,
     135,   136,   137,    -1,   139,   140,    -1,   142,   143,   144,
     145,   146,   147,    -1,   149,    -1,    -1,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,     3,   164,
      -1,    -1,   167,   168,   169,   170,   171,    12,    -1,    -1,
      -1,    -1,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    97,    98,    -1,   100,    19,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    31,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,   129,   130,   131,    -1,    -1,   134,
     135,   136,   137,    -1,   139,   140,    -1,   142,   143,   144,
     145,   146,   147,    -1,   149,    -1,    -1,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,    -1,   164,
      -1,    -1,   167,   168,   169,   170,    -1,    90,    -1,    12,
      -1,    94,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,   131,    -1,
      -1,   134,   135,   136,   137,    -1,   139,   140,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    90,   171,    12,
      -1,    94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,   131,    -1,
      -1,   134,   135,   136,   137,    -1,   139,   140,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    90,   171,    12,
      -1,    94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,   131,    -1,
      -1,   134,   135,   136,   137,    -1,   139,   140,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    90,   171,    12,
      -1,    94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,   131,    -1,
      -1,   134,   135,   136,   137,    -1,   139,   140,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    90,   171,    12,
      -1,    94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,   131,    -1,
      -1,   134,   135,   136,   137,    -1,   139,   140,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    90,   171,    -1,
      -1,    94,    -1,    -1,    12,   178,    -1,    -1,    -1,    -1,
     103,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,   131,    47,
      -1,   134,   135,   136,   137,    -1,   139,   140,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,   171,    -1,
      12,    -1,    90,    -1,    -1,   178,    94,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    47,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    90,   167,
     168,   169,    94,   171,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    31,    -1,   125,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    73,    -1,   167,   168,   169,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,   125,    -1,    -1,    36,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    73,   166,   167,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      12,    -1,    90,    -1,    -1,    -1,    94,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    90,   167,
     168,   169,    94,    -1,    -1,    12,    -1,    -1,    -1,    -1,
     178,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,   121,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,   171,
      -1,    12,    -1,    90,    -1,    -1,   178,    94,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    90,
     167,   168,   169,    94,   171,    -1,    12,    -1,    -1,    -1,
      -1,   178,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    -1,
     171,    -1,    12,    -1,    90,    -1,    -1,   178,    94,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    47,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    31,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      90,   167,   168,   169,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,   125,    -1,    -1,    36,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    73,   121,   167,   168,   169,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    94,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    12,   167,   168,   169,    -1,    -1,   125,    19,    -1,
      -1,   176,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    12,    13,   167,
     168,   169,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
     121,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    -1,
     125,    -1,    -1,    -1,    12,   176,    90,   178,    -1,    -1,
      94,    19,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    31,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,   125,   167,   168,   169,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,   178,    -1,    -1,    19,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    31,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    90,   167,   168,   169,    94,   171,    -1,    12,
      -1,    -1,    -1,    -1,   178,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,   142,   143,    -1,   145,   146,   147,
     103,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,   166,   167,
     168,   169,   125,    -1,    -1,    -1,    -1,    90,    -1,    -1,
     178,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   125,    -1,   167,   168,   169,    -1,    -1,    -1,
      12,    13,    -1,    -1,    -1,   178,    -1,    19,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    31,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,   174,    -1,    -1,    -1,   178,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    94,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,   124,   125,
      -1,    -1,    -1,    -1,    90,    -1,   178,    12,    94,    -1,
      -1,    -1,    -1,    -1,    19,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    31,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   125,
      -1,   167,   168,   169,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,   178,    -1,    19,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    31,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,   178,    12,    13,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     125,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,   174,
      -1,    90,    -1,   178,    -1,    94,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      12,    -1,   167,   168,   169,    -1,   125,    19,    -1,   174,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    12,    13,   167,   168,
     169,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,   125,
      -1,    -1,   174,    -1,    -1,    -1,   178,    12,    -1,    90,
      -1,    -1,    -1,    94,    19,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    31,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
     121,   167,   168,   169,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    90,   167,   168,   169,    94,
      -1,    -1,    12,    -1,    -1,    -1,    -1,   178,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,   166,   167,   168,   169,    -1,    -1,    -1,    12,    -1,
      90,    -1,    -1,   178,    94,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,
      19,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    31,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    90,   167,   168,   169,
      94,    -1,    -1,    12,    -1,    -1,    -1,    -1,   178,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    94,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,   125,    -1,    -1,    -1,
      -1,    90,   176,    -1,   178,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   125,    -1,   167,   168,
     169,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,   178,
      -1,    19,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    31,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,   178,
      -1,    -1,    19,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   103,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,   125,   167,
     168,   169,    -1,    -1,    -1,    -1,    12,    -1,    90,    -1,
     178,    -1,    94,    19,    -1,   142,   143,    -1,   145,   146,
     147,   103,   149,   150,   151,    31,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,   125,    -1,    -1,    -1,    12,    -1,    -1,
      -1,   178,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    31,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    90,   167,   168,   169,    94,    -1,
      -1,    -1,    12,    -1,    -1,    -1,   178,   103,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   103,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
     125,   167,   168,   169,    -1,    -1,    -1,    -1,    12,    -1,
      90,    -1,   178,    -1,    94,    19,    -1,   142,   143,    -1,
     145,   146,   147,   103,   149,   150,   151,    31,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,   125,    -1,    -1,    -1,    12,
      -1,    -1,    -1,   178,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    31,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    90,   167,   168,   169,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,   125,   167,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    90,    -1,   142,
     143,    94,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    22,   167,   168,   169,    26,    27,    -1,
      -1,    -1,   125,    -1,    -1,   178,    35,    -1,    37,    38,
      -1,    31,    -1,    -1,    43,    -1,    -1,    -1,    -1,   142,
     143,    50,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,    -1,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,    77,    -1,
      79,    31,    81,    -1,    83,   178,    -1,    86,    -1,    -1,
      89,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      90,    -1,   141,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   166,   158,   159,
     160,   161,   162,   172,   164,    -1,    -1,   167,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,    -1,   158,   159,
     160,   161,   162,    -1,   164,    -1,    22,   167,   168,   169,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,   178,    35,
      -1,    37,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    -1,     1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    77,    -1,    79,    -1,    81,    -1,    83,    33,    -1,
      86,    -1,    -1,    89,    39,    91,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    -1,    -1,    -1,   110,    -1,    -1,   113,    63,    -1,
     116,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,   141,     3,    -1,    -1,    -1,
      -1,    -1,    97,    98,    -1,   100,    -1,   102,    -1,   104,
      17,    18,   107,    20,    -1,    -1,   111,    -1,    25,   114,
     166,    -1,   117,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,   141,    54,    -1,    -1,
      -1,    -1,     3,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    17,    18,    -1,    20,
      -1,    78,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      -1,    88,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      97,    98,    -1,   100,    45,   102,    -1,   104,    49,    -1,
     107,    52,    -1,    54,   111,    -1,    -1,   114,     3,    -1,
     117,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    17,    18,    -1,    20,    -1,    78,    79,    -1,
      25,    -1,    -1,    -1,   141,    -1,    -1,    88,    33,    -1,
      91,    -1,    -1,    -1,    39,    -1,    97,    98,    -1,   100,
      45,   102,    -1,   104,    49,    -1,   107,    52,    -1,    54,
     111,    -1,    -1,   114,     3,   116,   117,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    17,    18,
      -1,    20,    -1,    78,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    97,    98,    -1,   100,    45,   102,    -1,   104,
      49,    -1,   107,    52,    -1,    54,   111,   112,    -1,   114,
       3,    -1,   117,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    17,    18,    -1,    20,    -1,    78,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    97,    98,
      -1,   100,    45,   102,    -1,   104,    49,    -1,   107,    52,
      -1,    54,   111,    -1,    -1,   114,     3,    -1,   117,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      17,    18,    -1,    20,    -1,    78,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    97,    98,    -1,   100,    45,   102,
      -1,   104,    49,    -1,   107,    52,    -1,    54,   111,    -1,
      -1,   114,    -1,    -1,   117,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    -1,   100,    -1,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
     117
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   183,   184,     0,     1,     3,     4,     5,     6,     7,
       8,     9,    10,    14,    15,    16,    17,    18,    20,    22,
      23,    25,    26,    27,    28,    29,    30,    33,    35,    37,
      38,    39,    43,    44,    45,    48,    49,    50,    52,    54,
      56,    57,    58,    59,    60,    62,    63,    64,    65,    67,
      68,    70,    71,    72,    77,    78,    79,    80,    81,    82,
      83,    86,    87,    88,    89,    91,    92,    93,    95,    96,
      97,    98,    99,   100,   101,   102,   104,   105,   107,   108,
     109,   110,   111,   113,   114,   116,   117,   122,   125,   141,
     142,   143,   144,   145,   146,   151,   160,   163,   164,   165,
     166,   169,   171,   173,   178,   185,   186,   187,   188,   189,
     190,   193,   194,   195,   196,   201,   202,   203,   205,   206,
     207,   212,   213,   217,   218,   222,   224,   227,   228,   231,
     232,   233,   234,   235,   236,   239,   240,   241,   245,   247,
     250,   251,   252,   253,   254,   258,   259,   264,   265,   266,
     267,   270,   271,   276,   277,   279,   280,   285,   289,   290,
     292,   293,   315,   320,   321,   325,   326,   347,   348,   349,
     351,   353,   354,   355,   363,   364,   365,   366,   367,   368,
     370,   373,   374,   375,   376,   377,   378,   379,   380,   382,
     383,   384,   385,   386,   166,    22,    35,    44,    54,    56,
      89,   102,   171,   240,   250,   277,   347,   354,   364,   365,
     370,   373,   375,   376,   121,   356,   357,     3,    54,   219,
     370,   356,   110,   327,    91,   219,   187,   341,   370,   173,
       3,    17,    18,    20,    25,    33,    39,    45,    49,    52,
      54,    63,    70,    71,    78,    88,    97,    98,   100,   102,
     104,   107,   111,   114,   117,   220,   221,   223,    11,    27,
     116,   246,   370,    83,    86,   203,   173,   104,   221,   221,
     221,   173,   221,   328,   329,    32,   207,   226,   370,   262,
     263,   370,   370,    18,    78,    97,   114,   370,   370,   370,
       8,   173,   230,   229,    27,    33,    47,    49,    51,    76,
      79,    91,    98,   102,   110,   186,   225,   281,   282,   283,
     306,   307,   308,   333,   338,   370,   341,   108,   109,   166,
     285,   286,   369,   370,   372,   370,   226,   370,   370,   370,
     101,   173,   187,   370,   370,   189,   193,   207,   189,   207,
     370,   372,     3,   370,   370,   370,   370,   370,     1,   172,
     185,   208,   341,   112,   152,   342,   343,   372,   370,    81,
     187,    22,    35,    38,    89,   171,   190,   191,   192,   203,
     207,   195,   150,   197,   171,    46,    85,   115,   204,    26,
     326,   370,     8,   266,   370,   371,    24,    32,    40,    41,
      42,   120,   175,   242,   243,   350,   352,    56,   147,   268,
     221,     1,   194,   221,   272,   275,   173,   301,    53,    74,
      84,   310,    27,    76,    79,    91,   110,   311,    27,    79,
      91,   110,   309,   221,   322,   323,     1,   328,   165,   166,
     370,    12,    19,    31,    90,    94,   125,   142,   143,   145,
     146,   147,   149,   150,   151,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   167,   168,   169,
     178,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   170,   297,   173,   175,
      90,    94,   370,   221,    40,   175,   242,    56,   173,   187,
     166,   207,   325,   166,   148,   166,   222,   344,   345,   346,
     372,   171,   221,   221,   103,   207,   103,   126,   207,   297,
     204,   344,   166,   173,   173,   207,   187,   112,   173,   221,
     330,   331,     1,   147,   337,    47,   148,   187,   226,   148,
     226,    13,   173,   173,   226,   344,   231,   231,    47,    91,
     307,   308,   174,   148,   112,   173,   221,   305,   147,   166,
     370,   370,   124,   287,   166,   171,   226,   173,   344,   166,
     255,   255,   166,   172,   172,   185,   148,   172,   370,   148,
     174,   148,   174,     8,   175,   242,   243,     3,   173,   198,
       1,   172,   208,   215,   216,   370,   210,   370,    66,    36,
      73,   166,   266,   268,   110,   237,   290,   370,   187,    79,
     246,   370,   123,   176,   244,   341,   370,   381,   244,   370,
     221,   341,   171,   172,   275,   126,   148,   172,   174,   302,
     304,   306,   311,    91,     1,   147,   335,   336,    91,     1,
       3,    12,    17,    19,    20,    25,    45,    52,    54,    55,
      63,    71,    88,   100,   104,   111,   117,   142,   143,   144,
     145,   146,   147,   149,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   164,   167,   168,   169,   170,   173,
     221,   294,   295,   296,   297,   347,   126,   324,   148,   166,
     166,   166,   370,   370,   370,   244,   370,   244,   370,   370,
     370,   370,   370,   370,   370,     3,    20,    33,    63,   104,
     110,   222,   370,   370,   370,   370,   370,   370,   370,   370,
     370,   370,   370,   370,   370,   370,   370,   370,   370,    69,
     372,   372,   372,   372,   372,   344,   344,   244,   370,   244,
     370,   103,   173,   341,   370,   221,    27,    47,    91,   116,
     358,   361,   362,   370,   386,   328,   370,   126,   174,   148,
     126,   126,   187,    34,   187,   370,    34,   370,    66,   174,
     345,   221,   248,   249,   331,   148,   174,   225,   370,   126,
     332,   370,   329,   262,   221,   325,   370,   370,   174,   305,
     311,   283,   331,   147,   339,   340,   338,   288,   206,     1,
     260,   346,   174,    21,   256,   172,   174,   174,   342,   174,
     342,   341,   370,   197,   174,   199,   200,   346,   172,   172,
     148,   166,    13,   150,   148,   166,    13,    36,    73,   221,
     142,   143,   144,   145,   146,   160,   164,   169,   209,   296,
     297,   298,   370,   209,   211,   268,   171,   238,   325,    47,
     356,   120,   221,   226,   226,   173,   370,   121,   176,   359,
      47,   148,   176,   359,   124,   148,   176,   226,   359,    32,
      47,   226,   359,   166,   173,     1,   269,   370,   273,   274,
     148,   174,   337,     1,   225,   333,   334,   106,   312,   173,
     300,   370,   142,   150,   300,   300,   370,   323,   173,   175,
     166,   166,   166,   166,   166,   166,   174,   176,   345,    47,
     176,    32,    47,   173,    47,    91,   148,   174,    17,    20,
      25,    45,    52,    63,    71,    88,   100,   111,   117,   347,
      90,    90,   166,   372,   346,   370,   370,    34,   187,    34,
      34,   187,   103,   207,   221,   174,   148,   174,   174,   330,
     337,    69,   372,   221,   174,   174,   340,   147,   284,   174,
     338,   152,   299,   332,   370,   172,    75,   118,   172,   261,
     174,   173,   207,   221,   257,    47,   176,    47,   148,   174,
     215,   222,    17,    18,    20,    25,    45,    49,    52,    71,
      78,    88,    97,    98,   100,   102,   111,   114,   117,   171,
     214,   298,   370,   112,   370,   209,   211,   148,   166,    13,
     166,   171,   269,   322,   328,   244,   370,   226,   370,    47,
     341,   173,   187,   176,   244,   370,   176,   187,   370,   176,
     370,   176,   370,   226,    44,   370,   244,   370,   226,   345,
     172,    83,    86,   172,   186,   194,   228,   275,   194,   304,
     312,    61,   119,   316,   302,   303,   174,   295,   297,   174,
     176,   174,   244,   370,    44,   244,   370,   345,   361,   337,
     347,   347,   187,   187,   370,   187,    34,   166,   166,   249,
     207,   332,   173,   173,   299,   332,   333,   312,   340,   370,
     187,   240,   341,   257,   147,   207,   244,   370,   244,   370,
     200,   209,    13,    36,    73,    36,    73,   166,   166,   298,
     370,   370,   269,   172,   166,   166,   356,   356,   166,   370,
     174,   360,   361,   187,   176,   359,   176,   359,   187,   124,
     370,    32,   226,   359,    32,   226,   359,   174,   194,   228,
     228,   275,   316,     3,    54,    93,   104,   317,   318,   319,
     370,   291,   174,   300,   300,   176,   176,   370,    32,    32,
     174,   332,    34,   187,   344,   344,   299,   332,    32,   226,
     174,   370,   176,   176,   172,   112,   370,   209,   211,   209,
     211,    13,   172,   226,   226,   226,   148,   174,    44,   187,
     370,   176,    44,   187,   370,   176,   370,   103,    44,   370,
     226,    44,   370,   226,   166,   228,   278,   319,   119,   148,
     126,   153,   155,   156,   158,   159,    61,    32,   166,   206,
     313,   314,    44,    44,   103,    44,    44,   187,   174,   174,
     187,   207,   166,   166,   166,   166,   370,   361,   370,   187,
     370,   187,   370,   370,   370,   314,   370,   318,   319,   319,
     319,   319,   319,   319,   317,   185,   370,   370,   370,   370,
     165,   165,   103,   103,   103,   103,   103,   103,   103,   103,
     370,   370,   370,   370
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   182,   183,   184,   184,   185,   185,   186,   186,   187,
     187,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   190,   190,   191,   192,
     192,   193,   194,   194,   195,   196,   197,   197,   198,   198,
     198,   199,   199,   200,   201,   202,   202,   202,   203,   203,
     203,   204,   204,   205,   206,   206,   206,   207,   208,   208,
     209,   209,   209,   209,   209,   209,   210,   210,   210,   210,
     210,   210,   211,   211,   212,   212,   212,   212,   212,   212,
     212,   213,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   215,
     215,   215,   215,   215,   216,   216,   217,   218,   218,   218,
     218,   218,   218,   219,   219,   219,   220,   220,   220,   220,
     220,   221,   221,   221,   222,   222,   222,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   225,   225,
     225,   225,   226,   226,   227,   227,   228,   228,   229,   228,
     230,   228,   231,   231,   231,   231,   231,   231,   231,   232,
     232,   232,   232,   233,   234,   234,   235,   236,   236,   236,
     237,   236,   238,   236,   239,   240,   241,   241,   242,   242,
     242,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   244,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   246,   246,   247,
     247,   248,   248,   249,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   250,   250,   251,   251,   251,
     251,   251,   251,   252,   252,   252,   253,   253,   253,   254,
     254,   254,   254,   255,   255,   256,   256,   256,   257,   257,
     258,   259,   259,   260,   260,   261,   261,   261,   262,   262,
     262,   263,   263,   264,   265,   265,   266,   267,   267,   267,
     268,   268,   269,   269,   269,   269,   269,   270,   270,   271,
     272,   272,   273,   272,   272,   274,   272,   275,   275,   276,
     278,   277,   279,   280,   280,   280,   281,   281,   282,   282,
     283,   283,   283,   284,   284,   285,   287,   286,   288,   286,
     289,   291,   290,   292,   292,   292,   292,   292,   293,   294,
     294,   295,   295,   295,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   297,   297,
     297,   298,   298,   299,   299,   300,   300,   301,   301,   302,
     302,   303,   303,   304,   305,   305,   306,   306,   306,   306,
     306,   306,   307,   307,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   309,   309,   309,   309,   309,   309,   310,
     310,   310,   311,   311,   311,   311,   311,   311,   311,   312,
     312,   313,   313,   314,   314,   315,   316,   316,   316,   316,
     316,   317,   317,   318,   318,   318,   318,   318,   318,   318,
     319,   319,   319,   320,   321,   321,   322,   322,   323,   324,
     324,   325,   325,   325,   325,   325,   327,   326,   326,   326,
     328,   328,   329,   329,   330,   330,   330,   331,   331,   331,
     332,   332,   332,   333,   334,   334,   334,   335,   335,   336,
     336,   337,   337,   337,   337,   338,   338,   339,   340,   340,
     341,   341,   342,   342,   343,   343,   344,   344,   345,   345,
     346,   346,   347,   347,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   349,   350,   350,   350,
     350,   350,   350,   351,   352,   352,   352,   352,   352,   352,
     352,   353,   354,   355,   355,   355,   355,   355,   355,   355,
     356,   356,   357,   358,   358,   359,   360,   360,   361,   361,
     361,   362,   362,   362,   362,   362,   362,   363,   363,   363,
     363,   363,   364,   364,   364,   364,   364,   365,   366,   366,
     366,   366,   366,   366,   367,   368,   369,   369,   370,   370,
     370,   370,   370,   370,   370,   370,   370,   370,   370,   370,
     370,   370,   370,   370,   370,   371,   371,   372,   372,   372,
     373,   373,   373,   373,   374,   374,   374,   374,   374,   375,
     375,   375,   376,   376,   376,   376,   376,   376,   377,   377,
     377,   377,   378,   378,   379,   379,   380,   380,   380,   380,
     380,   380,   380,   380,   380,   380,   380,   380,   380,   381,
     381,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   383,   383,   383,   383,   383,   383,
     383,   384,   384,   384,   384,   385,   385,   385,   385,   386,
     386,   386,   386,   386,   386,   386
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     1,     3,     3,     3,     3,     3,     2,
       1,     1,     1,     1,     1,     2,     1,     3,     3,     3,
       3,     2,     3,     3,     2,     2,     1,     1,     0,     1,
       1,     1,     1,     2,     3,     2,     0,     3,     2,     3,
       0,     1,     3,     1,     4,     3,     4,     4,     0,     1,
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     2,     3,     1,     1,     0,     3,
       0,     3,     1,     1,     1,     1,     1,     1,     1,     3,
       5,     5,     2,     1,     1,     1,     1,     6,     7,     3,
       0,     6,     0,     6,     2,     0,     3,     2,     1,     1,
       1,     5,     3,     3,     6,     6,     4,     6,     5,     6,
       5,     6,     3,     4,     3,     4,     6,     7,     6,     7,
       4,     5,     4,     5,     4,     4,     3,     6,     5,     4,
       3,     6,     5,     6,     5,     8,     7,     4,     4,     6,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     6,     4,     7,     5,     3,     6,     2,
       2,     3,     3,     0,     2,     2,     3,     5,     1,     3,
       3,     5,     5,     0,     2,     3,     2,     4,     4,     3,
       1,     1,     3,     3,     5,     5,     2,     1,     1,     1,
       0,     2,     0,     2,     3,     3,     4,     3,     3,     3,
       1,     2,     0,     4,     2,     0,     5,     1,     3,     1,
       0,     8,     0,     1,     1,     1,     0,     1,     1,     3,
       1,     3,     1,     0,     2,     6,     0,     3,     0,     4,
       1,     0,     7,     4,     4,     6,     6,     4,     2,     1,
       3,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     0,     3,     2,     3,     1,
       3,     0,     1,     1,     1,     1,     4,     5,     4,     5,
       6,     6,     0,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     0,     1,     1,     2,     1,     1,     1,     0,
       1,     1,     1,     1,     2,     1,     0,     2,     2,     4,
       4,     1,     3,     3,     3,     3,     3,     3,     3,     2,
       1,     1,     1,     3,     1,     2,     1,     3,     2,     0,
       2,     1,     2,     1,     1,     1,     0,     5,     3,     3,
       1,     3,     3,     5,     1,     1,     3,     1,     2,     3,
       0,     2,     2,     1,     1,     1,     1,     2,     1,     0,
       1,     0,     2,     2,     1,     1,     1,     2,     0,     1,
       1,     3,     1,     1,     3,     3,     0,     1,     1,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       4,     4,     4,     5,     2,     2,     1,     2,     1,     2,
       1,     2,     1,     2,     1,     1,     2,     6,     6,     4,
       9,     9,     7,     2,     2,     3,     4,     6,     6,     9,
       9,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     4,     1,     3,     4,     1,     3,     4,     3,
       3,     1,     1,     2,     1,     2,     1,     1,     2,     2,
       2,     2,     2,     8,     8,     9,     9,     4,     3,     3,
       2,     2,     2,     1,     3,     4,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     2,     2,     1,     1,     4,
       4,     4,     3,     3,     3,     3,     5,     5,     3,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     4,     4,     5,     4,     5,     3,
       5,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1
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
#line 634 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6080 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 638 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6086 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 639 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6092 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 646 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6100 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 654 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6108 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 658 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6117 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 668 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6125 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 673 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6133 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 680 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6139 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 681 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6145 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 682 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6151 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 683 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6157 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 684 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6163 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 685 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6169 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6175 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 687 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6181 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 688 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6187 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: TDEFER stmt  */
#line 690 "chpl.ypp"
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
#line 6204 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 702 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6210 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 703 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6216 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: TINIT TTHIS TSEMI  */
#line 705 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr));
    auto node = Init::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6228 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 713 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6241 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 722 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6254 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TLABEL ident_def stmt  */
#line 731 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6262 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 735 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6273 "bison-chpl-lib.cpp"
    break;

  case 29: /* stmt_base: error TSEMI  */
#line 742 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6282 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: assignment_stmt  */
#line 749 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6288 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: if_stmt  */
#line 750 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6294 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: loop_stmt  */
#line 751 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6300 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: select_stmt  */
#line 752 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6306 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: manage_stmt  */
#line 753 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6312 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 754 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6318 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: throw_stmt  */
#line 755 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6324 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 757 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6330 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 759 "chpl.ypp"
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
#line 6349 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 774 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6361 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 782 "chpl.ypp"
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
#line 6377 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TLOCAL do_stmt  */
#line 794 "chpl.ypp"
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
#line 6392 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TON expr do_stmt  */
#line 805 "chpl.ypp"
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
#line 6408 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 817 "chpl.ypp"
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
#line 6424 "bison-chpl-lib.cpp"
    break;

  case 44: /* tryable_stmt: TSERIAL do_stmt  */
#line 829 "chpl.ypp"
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
#line 6439 "bison-chpl-lib.cpp"
    break;

  case 45: /* tryable_stmt: TSYNC stmt  */
#line 840 "chpl.ypp"
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
#line 6456 "bison-chpl-lib.cpp"
    break;

  case 48: /* collect_attributes: %empty  */
#line 860 "chpl.ypp"
  {
    auto attributeGroup = context->buildAttributeGroup((yyloc));
    if (attributeGroup != nullptr) {
      context->resetAttributeGroupPartsState();
    }
    context->loopAttributes.push_back(std::move(attributeGroup));
  }
#line 6468 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 885 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6476 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATMARK TIDENT  */
#line 892 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6485 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 899 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6493 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 903 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6504 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 913 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6510 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 915 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6518 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 919 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6526 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 926 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6535 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 931 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6545 "bison-chpl-lib.cpp"
    break;

  case 64: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 944 "chpl.ypp"
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
#line 6566 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 964 "chpl.ypp"
    {
      context->clearCommentsBefore((yylsp[-1]));

      ModuleParts parts = (yyvsp[-2].moduleParts);
      ParserExprList* body = context->makeList();
      context->appendList(body, context->gatherComments((yylsp[0])));
      auto mod = Module::build(BUILDER, LOC((yyloc)), toOwned(parts.attributeGroup),
                               parts.visibility,
                               parts.name,
                               parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 6585 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 979 "chpl.ypp"
    {
      context->clearCommentsBefore((yylsp[-2]));

      ModuleParts parts = (yyvsp[-3].moduleParts);
      ParserExprList* body = (yyvsp[-1].exprList);
      context->appendList(body, context->gatherComments((yylsp[0])));
      auto mod = Module::build(BUILDER, LOC((yyloc)), toOwned(parts.attributeGroup),
                               parts.visibility,
                               parts.name,
                               parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 6604 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 994 "chpl.ypp"
    {
      ModuleParts parts = (yyvsp[-3].moduleParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      ParserExprList* body = context->makeList(std::move(err));
      auto mod = Module::build(BUILDER, LOC((yyloc)), toOwned(parts.attributeGroup),
                               parts.visibility,
                               parts.name,
                               parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 6621 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1009 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6627 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1010 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6634 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1012 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6641 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1017 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6647 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1018 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6654 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1024 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6664 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1042 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6672 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1046 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6680 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR error TRCBR  */
#line 1050 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6692 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt: block_stmt_body  */
#line 1061 "chpl.ypp"
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
#line 6711 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1079 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6717 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1080 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6723 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1085 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6731 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1089 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6739 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1093 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6748 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1098 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6756 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1102 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6764 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1106 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6773 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1116 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6782 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1121 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6792 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1127 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6803 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1134 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6812 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1139 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6823 "bison-chpl-lib.cpp"
    break;

  case 91: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1146 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6835 "bison-chpl-lib.cpp"
    break;

  case 92: /* opt_only_ls: %empty  */
#line 1156 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6841 "bison-chpl-lib.cpp"
    break;

  case 93: /* opt_only_ls: renames_ls  */
#line 1157 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6847 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1162 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6856 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1167 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6868 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1175 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6881 "bison-chpl-lib.cpp"
    break;

  case 97: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1184 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6895 "bison-chpl-lib.cpp"
    break;

  case 98: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1194 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6907 "bison-chpl-lib.cpp"
    break;

  case 99: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1202 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6920 "bison-chpl-lib.cpp"
    break;

  case 100: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1211 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6934 "bison-chpl-lib.cpp"
    break;

  case 101: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1224 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6943 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_expr: expr  */
#line 1260 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6951 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_expr: expr TDOT all_op_name  */
#line 1264 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6960 "bison-chpl-lib.cpp"
    break;

  case 121: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1269 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6969 "bison-chpl-lib.cpp"
    break;

  case 122: /* import_expr: expr TAS ident_use  */
#line 1274 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6979 "bison-chpl-lib.cpp"
    break;

  case 123: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1280 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6989 "bison-chpl-lib.cpp"
    break;

  case 124: /* import_ls: import_expr  */
#line 1288 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6995 "bison-chpl-lib.cpp"
    break;

  case 125: /* import_ls: import_ls TCOMMA import_expr  */
#line 1289 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7001 "bison-chpl-lib.cpp"
    break;

  case 126: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1294 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7011 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1303 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7019 "bison-chpl-lib.cpp"
    break;

  case 128: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1307 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7027 "bison-chpl-lib.cpp"
    break;

  case 129: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1311 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7035 "bison-chpl-lib.cpp"
    break;

  case 130: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1315 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7043 "bison-chpl-lib.cpp"
    break;

  case 131: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1319 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7051 "bison-chpl-lib.cpp"
    break;

  case 132: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1323 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7059 "bison-chpl-lib.cpp"
    break;

  case 133: /* opt_label_ident: %empty  */
#line 1331 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7065 "bison-chpl-lib.cpp"
    break;

  case 134: /* opt_label_ident: TIDENT  */
#line 1332 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7071 "bison-chpl-lib.cpp"
    break;

  case 135: /* opt_label_ident: TINIT  */
#line 1333 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7077 "bison-chpl-lib.cpp"
    break;

  case 144: /* ident_use: TIDENT  */
#line 1359 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7083 "bison-chpl-lib.cpp"
    break;

  case 145: /* ident_use: TINIT  */
#line 1360 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7089 "bison-chpl-lib.cpp"
    break;

  case 146: /* ident_use: TTHIS  */
#line 1361 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7095 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TBOOL  */
#line 1388 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7101 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TENUM  */
#line 1389 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7107 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TINT  */
#line 1390 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7113 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TUINT  */
#line 1391 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7119 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TREAL  */
#line 1392 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7125 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TIMAG  */
#line 1393 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7131 "bison-chpl-lib.cpp"
    break;

  case 172: /* scalar_type: TCOMPLEX  */
#line 1394 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7137 "bison-chpl-lib.cpp"
    break;

  case 173: /* scalar_type: TBYTES  */
#line 1395 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7143 "bison-chpl-lib.cpp"
    break;

  case 174: /* scalar_type: TSTRING  */
#line 1396 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7149 "bison-chpl-lib.cpp"
    break;

  case 175: /* scalar_type: TLOCALE  */
#line 1397 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7155 "bison-chpl-lib.cpp"
    break;

  case 176: /* scalar_type: TNOTHING  */
#line 1398 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7161 "bison-chpl-lib.cpp"
    break;

  case 177: /* scalar_type: TVOID  */
#line 1399 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7167 "bison-chpl-lib.cpp"
    break;

  case 182: /* do_stmt: TDO stmt  */
#line 1413 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7173 "bison-chpl-lib.cpp"
    break;

  case 183: /* do_stmt: block_stmt  */
#line 1414 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7179 "bison-chpl-lib.cpp"
    break;

  case 184: /* return_stmt: TRETURN TSEMI  */
#line 1419 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7190 "bison-chpl-lib.cpp"
    break;

  case 185: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1426 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7201 "bison-chpl-lib.cpp"
    break;

  case 186: /* class_level_stmt: TSEMI  */
#line 1436 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7209 "bison-chpl-lib.cpp"
    break;

  case 187: /* class_level_stmt: inner_class_level_stmt  */
#line 1440 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7219 "bison-chpl-lib.cpp"
    break;

  case 188: /* $@1: %empty  */
#line 1445 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7226 "bison-chpl-lib.cpp"
    break;

  case 189: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1447 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7235 "bison-chpl-lib.cpp"
    break;

  case 190: /* $@2: %empty  */
#line 1451 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7242 "bison-chpl-lib.cpp"
    break;

  case 191: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1453 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7251 "bison-chpl-lib.cpp"
    break;

  case 199: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1471 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7260 "bison-chpl-lib.cpp"
    break;

  case 200: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1476 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7269 "bison-chpl-lib.cpp"
    break;

  case 201: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1481 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7278 "bison-chpl-lib.cpp"
    break;

  case 202: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1486 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7286 "bison-chpl-lib.cpp"
    break;

  case 203: /* forwarding_decl_start: TFORWARDING  */
#line 1493 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7295 "bison-chpl-lib.cpp"
    break;

  case 204: /* extern_or_export: TEXTERN  */
#line 1500 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7301 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_or_export: TEXPORT  */
#line 1501 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7307 "bison-chpl-lib.cpp"
    break;

  case 206: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1506 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7317 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1516 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7326 "bison-chpl-lib.cpp"
    break;

  case 208: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1522 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7337 "bison-chpl-lib.cpp"
    break;

  case 209: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1529 "chpl.ypp"
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
#line 7359 "bison-chpl-lib.cpp"
    break;

  case 210: /* $@3: %empty  */
#line 1547 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7367 "bison-chpl-lib.cpp"
    break;

  case 211: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1551 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7376 "bison-chpl-lib.cpp"
    break;

  case 212: /* $@4: %empty  */
#line 1556 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7385 "bison-chpl-lib.cpp"
    break;

  case 213: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1561 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7394 "bison-chpl-lib.cpp"
    break;

  case 214: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1569 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7402 "bison-chpl-lib.cpp"
    break;

  case 215: /* no_loop_attributes: %empty  */
#line 1574 "chpl.ypp"
                    { context->loopAttributes.push_back(nullptr); }
#line 7408 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1577 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7414 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1578 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7420 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1594 "chpl.ypp"
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
#line 7443 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1613 "chpl.ypp"
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
#line 7461 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1627 "chpl.ypp"
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
#line 7479 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1641 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7487 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1645 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7495 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1649 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7503 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
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
                           context->popLoopAttributeGroup());
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    context->resetAttributeGroupPartsState();
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7527 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1673 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7535 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: for_loop_kw expr TIN expr forall_intent_clause do_stmt  */
#line 1677 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7543 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1681 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7551 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1685 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7559 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1689 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7567 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: for_loop_kw expr forall_intent_clause do_stmt  */
#line 1693 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7575 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1697 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7583 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: for_loop_kw zippered_iterator forall_intent_clause do_stmt  */
#line 1701 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7591 "bison-chpl-lib.cpp"
    break;

  case 236: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1705 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7599 "bison-chpl-lib.cpp"
    break;

  case 237: /* loop_stmt_base: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1709 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7607 "bison-chpl-lib.cpp"
    break;

  case 238: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1713 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7615 "bison-chpl-lib.cpp"
    break;

  case 239: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1717 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7623 "bison-chpl-lib.cpp"
    break;

  case 240: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1721 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7631 "bison-chpl-lib.cpp"
    break;

  case 241: /* loop_stmt_base: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1725 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7639 "bison-chpl-lib.cpp"
    break;

  case 242: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1729 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7648 "bison-chpl-lib.cpp"
    break;

  case 243: /* loop_stmt_base: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1734 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7657 "bison-chpl-lib.cpp"
    break;

  case 244: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1742 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7667 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF expr TTHEN stmt  */
#line 1751 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7675 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF expr block_stmt  */
#line 1755 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7683 "bison-chpl-lib.cpp"
    break;

  case 247: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1759 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7691 "bison-chpl-lib.cpp"
    break;

  case 248: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1763 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7699 "bison-chpl-lib.cpp"
    break;

  case 249: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1767 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7707 "bison-chpl-lib.cpp"
    break;

  case 250: /* if_stmt: TIF ifvar block_stmt  */
#line 1771 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7715 "bison-chpl-lib.cpp"
    break;

  case 251: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1775 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7723 "bison-chpl-lib.cpp"
    break;

  case 252: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1779 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7731 "bison-chpl-lib.cpp"
    break;

  case 253: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1783 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7740 "bison-chpl-lib.cpp"
    break;

  case 254: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1788 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7749 "bison-chpl-lib.cpp"
    break;

  case 255: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1793 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7758 "bison-chpl-lib.cpp"
    break;

  case 256: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1798 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7767 "bison-chpl-lib.cpp"
    break;

  case 257: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1806 "chpl.ypp"
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
#line 7786 "bison-chpl-lib.cpp"
    break;

  case 258: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1821 "chpl.ypp"
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
#line 7805 "bison-chpl-lib.cpp"
    break;

  case 259: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1839 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7813 "bison-chpl-lib.cpp"
    break;

  case 260: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1843 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7821 "bison-chpl-lib.cpp"
    break;

  case 261: /* ifc_formal_ls: ifc_formal  */
#line 1849 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7827 "bison-chpl-lib.cpp"
    break;

  case 262: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1850 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7833 "bison-chpl-lib.cpp"
    break;

  case 263: /* ifc_formal: ident_def  */
#line 1855 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7839 "bison-chpl-lib.cpp"
    break;

  case 283: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1899 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7847 "bison-chpl-lib.cpp"
    break;

  case 284: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1903 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7855 "bison-chpl-lib.cpp"
    break;

  case 285: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1907 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7863 "bison-chpl-lib.cpp"
    break;

  case 286: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1914 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7871 "bison-chpl-lib.cpp"
    break;

  case 287: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1918 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7879 "bison-chpl-lib.cpp"
    break;

  case 288: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1922 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7887 "bison-chpl-lib.cpp"
    break;

  case 289: /* try_stmt: TTRY tryable_stmt  */
#line 1929 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7895 "bison-chpl-lib.cpp"
    break;

  case 290: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1933 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7903 "bison-chpl-lib.cpp"
    break;

  case 291: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1937 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7911 "bison-chpl-lib.cpp"
    break;

  case 292: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1941 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7919 "bison-chpl-lib.cpp"
    break;

  case 293: /* catch_expr_ls: %empty  */
#line 1947 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7925 "bison-chpl-lib.cpp"
    break;

  case 294: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1948 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7931 "bison-chpl-lib.cpp"
    break;

  case 295: /* catch_expr: TCATCH block_stmt  */
#line 1953 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 7939 "bison-chpl-lib.cpp"
    break;

  case 296: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1957 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 7947 "bison-chpl-lib.cpp"
    break;

  case 297: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1961 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 7955 "bison-chpl-lib.cpp"
    break;

  case 298: /* catch_expr_inner: ident_def  */
#line 1968 "chpl.ypp"
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
#line 7972 "bison-chpl-lib.cpp"
    break;

  case 299: /* catch_expr_inner: ident_def TCOLON expr  */
#line 1981 "chpl.ypp"
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
#line 7989 "bison-chpl-lib.cpp"
    break;

  case 300: /* throw_stmt: TTHROW expr TSEMI  */
#line 1997 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7999 "bison-chpl-lib.cpp"
    break;

  case 301: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2005 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8007 "bison-chpl-lib.cpp"
    break;

  case 302: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2009 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8017 "bison-chpl-lib.cpp"
    break;

  case 303: /* when_stmt_ls: %empty  */
#line 2017 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8023 "bison-chpl-lib.cpp"
    break;

  case 304: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2018 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8029 "bison-chpl-lib.cpp"
    break;

  case 305: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2023 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8037 "bison-chpl-lib.cpp"
    break;

  case 306: /* when_stmt: TOTHERWISE stmt  */
#line 2027 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8046 "bison-chpl-lib.cpp"
    break;

  case 307: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2032 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 8056 "bison-chpl-lib.cpp"
    break;

  case 308: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2041 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8065 "bison-chpl-lib.cpp"
    break;

  case 309: /* manager_expr: expr TAS ident_def  */
#line 2046 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8073 "bison-chpl-lib.cpp"
    break;

  case 310: /* manager_expr: expr  */
#line 2050 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8081 "bison-chpl-lib.cpp"
    break;

  case 311: /* manager_expr_ls: manager_expr  */
#line 2056 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8087 "bison-chpl-lib.cpp"
    break;

  case 312: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2057 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8093 "bison-chpl-lib.cpp"
    break;

  case 313: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2062 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8101 "bison-chpl-lib.cpp"
    break;

  case 314: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2071 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8110 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2076 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8121 "bison-chpl-lib.cpp"
    break;

  case 316: /* class_start: class_tag ident_def  */
#line 2087 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8129 "bison-chpl-lib.cpp"
    break;

  case 317: /* class_tag: TCLASS  */
#line 2093 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8135 "bison-chpl-lib.cpp"
    break;

  case 318: /* class_tag: TRECORD  */
#line 2094 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8141 "bison-chpl-lib.cpp"
    break;

  case 319: /* class_tag: TUNION  */
#line 2095 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8147 "bison-chpl-lib.cpp"
    break;

  case 320: /* opt_inherit: %empty  */
#line 2099 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8153 "bison-chpl-lib.cpp"
    break;

  case 321: /* opt_inherit: TCOLON expr_ls  */
#line 2100 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8159 "bison-chpl-lib.cpp"
    break;

  case 322: /* class_level_stmt_ls: %empty  */
#line 2104 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8168 "bison-chpl-lib.cpp"
    break;

  case 323: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2109 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8176 "bison-chpl-lib.cpp"
    break;

  case 324: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2113 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8184 "bison-chpl-lib.cpp"
    break;

  case 325: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2117 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8192 "bison-chpl-lib.cpp"
    break;

  case 326: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2121 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8200 "bison-chpl-lib.cpp"
    break;

  case 327: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2129 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      ParserExprList* list = (yyvsp[-1].exprList);
      // get any comments after the last element but before the closing brace
      context->appendList(list, context->gatherComments((yylsp[0])));

      auto decl = Enum::build(BUILDER, LOC((yyloc)), toOwned(parts.attributeGroup),
                              parts.visibility,
                              parts.name,
                              context->consumeList(list));
      BUILDER->noteDeclNameLocation(decl.get(), LOC(parts.locName));
      CommentsAndStmt cs = {parts.comments, decl.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8222 "bison-chpl-lib.cpp"
    break;

  case 328: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2147 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8236 "bison-chpl-lib.cpp"
    break;

  case 329: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2160 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yylsp[-1]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8244 "bison-chpl-lib.cpp"
    break;

  case 330: /* enum_ls: enum_item  */
#line 2167 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8253 "bison-chpl-lib.cpp"
    break;

  case 331: /* enum_ls: enum_ls TCOMMA  */
#line 2172 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8263 "bison-chpl-lib.cpp"
    break;

  case 332: /* $@5: %empty  */
#line 2178 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8272 "bison-chpl-lib.cpp"
    break;

  case 333: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2183 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8281 "bison-chpl-lib.cpp"
    break;

  case 334: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2188 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8290 "bison-chpl-lib.cpp"
    break;

  case 335: /* $@6: %empty  */
#line 2193 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8299 "bison-chpl-lib.cpp"
    break;

  case 336: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2198 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8308 "bison-chpl-lib.cpp"
    break;

  case 337: /* enum_item: ident_def  */
#line 2208 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8319 "bison-chpl-lib.cpp"
    break;

  case 338: /* enum_item: ident_def TASSIGN expr  */
#line 2215 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8332 "bison-chpl-lib.cpp"
    break;

  case 339: /* lambda_decl_start: TLAMBDA  */
#line 2227 "chpl.ypp"
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
#line 8348 "bison-chpl-lib.cpp"
    break;

  case 340: /* $@7: %empty  */
#line 2244 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8358 "bison-chpl-lib.cpp"
    break;

  case 341: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2250 "chpl.ypp"
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
#line 8380 "bison-chpl-lib.cpp"
    break;

  case 343: /* linkage_spec: linkage_spec_empty  */
#line 2273 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8386 "bison-chpl-lib.cpp"
    break;

  case 344: /* linkage_spec: TINLINE  */
#line 2274 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8393 "bison-chpl-lib.cpp"
    break;

  case 345: /* linkage_spec: TOVERRIDE  */
#line 2276 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8400 "bison-chpl-lib.cpp"
    break;

  case 346: /* opt_fn_type_formal_ls: %empty  */
#line 2281 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8406 "bison-chpl-lib.cpp"
    break;

  case 347: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2282 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8412 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_type_formal_ls: fn_type_formal  */
#line 2286 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8418 "bison-chpl-lib.cpp"
    break;

  case 349: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2287 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8424 "bison-chpl-lib.cpp"
    break;

  case 350: /* fn_type_formal: named_formal  */
#line 2292 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8430 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2295 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8436 "bison-chpl-lib.cpp"
    break;

  case 352: /* fn_type_formal: formal_type  */
#line 2297 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8442 "bison-chpl-lib.cpp"
    break;

  case 353: /* opt_fn_type_ret_type: %empty  */
#line 2301 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8448 "bison-chpl-lib.cpp"
    break;

  case 354: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2302 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8454 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2315 "chpl.ypp"
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
#line 8473 "bison-chpl-lib.cpp"
    break;

  case 356: /* $@8: %empty  */
#line 2333 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8483 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2339 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8496 "bison-chpl-lib.cpp"
    break;

  case 358: /* $@9: %empty  */
#line 2348 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8506 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2354 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8519 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2366 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8527 "bison-chpl-lib.cpp"
    break;

  case 361: /* $@10: %empty  */
#line 2375 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8535 "bison-chpl-lib.cpp"
    break;

  case 362: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2379 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-6].functionParts);
    fp.returnIntent = (yyvsp[-5].returnTag);
    fp.returnType = (yyvsp[-4].expr);
    fp.throws = ((yyvsp[-3].throwsTag) == ThrowsTag_THROWS);
    WhereAndLifetime wl = (yyvsp[-2].lifetimeAndWhere);
    fp.body = (yyvsp[0].exprList);
    fp.lifetime = wl.lifetime;
    fp.where = wl.where;
    context->exitScopeForFunctionDecl(fp);
    (yyval.functionParts) = fp;
  }
#line 8552 "bison-chpl-lib.cpp"
    break;

  case 363: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2395 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8564 "bison-chpl-lib.cpp"
    break;

  case 364: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2403 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8576 "bison-chpl-lib.cpp"
    break;

  case 365: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2411 "chpl.ypp"
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
#line 8591 "bison-chpl-lib.cpp"
    break;

  case 366: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2422 "chpl.ypp"
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
#line 8606 "bison-chpl-lib.cpp"
    break;

  case 367: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2433 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8616 "bison-chpl-lib.cpp"
    break;

  case 368: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2442 "chpl.ypp"
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
#line 8632 "bison-chpl-lib.cpp"
    break;

  case 370: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2457 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8638 "bison-chpl-lib.cpp"
    break;

  case 373: /* fn_ident: ident_def TBANG  */
#line 2464 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8648 "bison-chpl-lib.cpp"
    break;

  case 413: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2521 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8654 "bison-chpl-lib.cpp"
    break;

  case 414: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2522 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8660 "bison-chpl-lib.cpp"
    break;

  case 415: /* opt_formal_ls: %empty  */
#line 2526 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8666 "bison-chpl-lib.cpp"
    break;

  case 416: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2527 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8672 "bison-chpl-lib.cpp"
    break;

  case 417: /* req_formal_ls: TLP TRP  */
#line 2531 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8678 "bison-chpl-lib.cpp"
    break;

  case 418: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2532 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8684 "bison-chpl-lib.cpp"
    break;

  case 419: /* formal_ls_inner: formal  */
#line 2536 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8690 "bison-chpl-lib.cpp"
    break;

  case 420: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2537 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8696 "bison-chpl-lib.cpp"
    break;

  case 421: /* formal_ls: %empty  */
#line 2541 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8702 "bison-chpl-lib.cpp"
    break;

  case 422: /* formal_ls: formal_ls_inner  */
#line 2542 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8708 "bison-chpl-lib.cpp"
    break;

  case 426: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2556 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8716 "bison-chpl-lib.cpp"
    break;

  case 427: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2561 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8724 "bison-chpl-lib.cpp"
    break;

  case 428: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2566 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8732 "bison-chpl-lib.cpp"
    break;

  case 429: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2571 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8740 "bison-chpl-lib.cpp"
    break;

  case 430: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2576 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8748 "bison-chpl-lib.cpp"
    break;

  case 431: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2581 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8756 "bison-chpl-lib.cpp"
    break;

  case 432: /* opt_formal_intent_tag: %empty  */
#line 2587 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 8765 "bison-chpl-lib.cpp"
    break;

  case 433: /* opt_formal_intent_tag: required_intent_tag  */
#line 2592 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8774 "bison-chpl-lib.cpp"
    break;

  case 434: /* required_intent_tag: TIN  */
#line 2599 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 8780 "bison-chpl-lib.cpp"
    break;

  case 435: /* required_intent_tag: TINOUT  */
#line 2600 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 8786 "bison-chpl-lib.cpp"
    break;

  case 436: /* required_intent_tag: TOUT  */
#line 2601 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 8792 "bison-chpl-lib.cpp"
    break;

  case 437: /* required_intent_tag: TCONST TIN  */
#line 2602 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 8798 "bison-chpl-lib.cpp"
    break;

  case 438: /* required_intent_tag: TCONST TREF  */
#line 2603 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 8804 "bison-chpl-lib.cpp"
    break;

  case 439: /* required_intent_tag: TCONST  */
#line 2604 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 8810 "bison-chpl-lib.cpp"
    break;

  case 440: /* required_intent_tag: TPARAM  */
#line 2605 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 8816 "bison-chpl-lib.cpp"
    break;

  case 441: /* required_intent_tag: TREF  */
#line 2606 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 8822 "bison-chpl-lib.cpp"
    break;

  case 442: /* required_intent_tag: TTYPE  */
#line 2607 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 8828 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_this_intent_tag: %empty  */
#line 2611 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 8834 "bison-chpl-lib.cpp"
    break;

  case 444: /* opt_this_intent_tag: TPARAM  */
#line 2612 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 8840 "bison-chpl-lib.cpp"
    break;

  case 445: /* opt_this_intent_tag: TREF  */
#line 2613 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 8846 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_this_intent_tag: TCONST TREF  */
#line 2614 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 8852 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_this_intent_tag: TCONST  */
#line 2615 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 8858 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_this_intent_tag: TTYPE  */
#line 2616 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 8864 "bison-chpl-lib.cpp"
    break;

  case 449: /* proc_iter_or_op: TPROC  */
#line 2620 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8870 "bison-chpl-lib.cpp"
    break;

  case 450: /* proc_iter_or_op: TITER  */
#line 2621 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8876 "bison-chpl-lib.cpp"
    break;

  case 451: /* proc_iter_or_op: TOPERATOR  */
#line 2622 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8882 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: %empty  */
#line 2626 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 8888 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_ret_tag: TOUT  */
#line 2627 "chpl.ypp"
                    { (yyval.returnTag) = Function::OUT; }
#line 8894 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_ret_tag: TCONST  */
#line 2628 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 8900 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_ret_tag: TCONST TREF  */
#line 2629 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 8906 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_ret_tag: TREF  */
#line 2630 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 8912 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_ret_tag: TPARAM  */
#line 2631 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 8918 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_ret_tag: TTYPE  */
#line 2632 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 8924 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_throws_error: %empty  */
#line 2636 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8930 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_throws_error: TTHROWS  */
#line 2637 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8936 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_function_body_stmt: TSEMI  */
#line 2640 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8942 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_function_body_stmt: function_body_stmt  */
#line 2641 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8948 "bison-chpl-lib.cpp"
    break;

  case 463: /* function_body_stmt: block_stmt_body  */
#line 2645 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8954 "bison-chpl-lib.cpp"
    break;

  case 464: /* function_body_stmt: TDO toplevel_stmt  */
#line 2646 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 8960 "bison-chpl-lib.cpp"
    break;

  case 465: /* query_expr: TQUERIEDIDENT  */
#line 2650 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 8966 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_lifetime_where: %empty  */
#line 2655 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 8972 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_lifetime_where: TWHERE expr  */
#line 2657 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 8978 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2659 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 8984 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2661 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 8990 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2663 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 8996 "bison-chpl-lib.cpp"
    break;

  case 471: /* lifetime_components_expr: lifetime_expr  */
#line 2668 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9002 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2670 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9008 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2675 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9014 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2677 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9020 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2679 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9026 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2681 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9032 "bison-chpl-lib.cpp"
    break;

  case 477: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2683 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9038 "bison-chpl-lib.cpp"
    break;

  case 478: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2685 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9044 "bison-chpl-lib.cpp"
    break;

  case 479: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2687 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9050 "bison-chpl-lib.cpp"
    break;

  case 480: /* lifetime_ident: TIDENT  */
#line 2691 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9056 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_ident: TINIT  */
#line 2692 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9062 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_ident: TTHIS  */
#line 2693 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9068 "bison-chpl-lib.cpp"
    break;

  case 483: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2698 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9077 "bison-chpl-lib.cpp"
    break;

  case 484: /* type_alias_decl_stmt_start: TTYPE  */
#line 2707 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9085 "bison-chpl-lib.cpp"
    break;

  case 485: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2711 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9094 "bison-chpl-lib.cpp"
    break;

  case 486: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2719 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9102 "bison-chpl-lib.cpp"
    break;

  case 487: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2723 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9110 "bison-chpl-lib.cpp"
    break;

  case 488: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2730 "chpl.ypp"
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
#line 9132 "bison-chpl-lib.cpp"
    break;

  case 489: /* opt_init_type: %empty  */
#line 2750 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9138 "bison-chpl-lib.cpp"
    break;

  case 490: /* opt_init_type: TASSIGN expr  */
#line 2752 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9144 "bison-chpl-lib.cpp"
    break;

  case 491: /* var_decl_type: TPARAM  */
#line 2756 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9150 "bison-chpl-lib.cpp"
    break;

  case 492: /* var_decl_type: TCONST TREF  */
#line 2757 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9156 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_type: TREF  */
#line 2758 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9162 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_type: TCONST  */
#line 2759 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9168 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_type: TVAR  */
#line 2760 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9174 "bison-chpl-lib.cpp"
    break;

  case 496: /* $@11: %empty  */
#line 2765 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9183 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2769 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9192 "bison-chpl-lib.cpp"
    break;

  case 498: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2774 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9201 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_stmt: var_decl_type error TSEMI  */
#line 2779 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = { .comments=nullptr, .stmt=ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release() };
    context->resetDeclStateOnError();
  }
#line 9210 "bison-chpl-lib.cpp"
    break;

  case 500: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2787 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9218 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2791 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9226 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2798 "chpl.ypp"
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
      BUILDER->noteDeclNameLocation(varDecl.get(), LOC((yylsp[-2])));
      // gather the comments like a statement does
      (yyval.commentsAndStmt) = STMT((yylsp[-2]), varDecl.release());
      context->clearComments();
    }
#line 9249 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2817 "chpl.ypp"
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
#line 9269 "bison-chpl-lib.cpp"
    break;

  case 504: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2836 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9277 "bison-chpl-lib.cpp"
    break;

  case 505: /* tuple_var_decl_component: ident_def  */
#line 2840 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9285 "bison-chpl-lib.cpp"
    break;

  case 506: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2844 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9293 "bison-chpl-lib.cpp"
    break;

  case 507: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2851 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9299 "bison-chpl-lib.cpp"
    break;

  case 508: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2853 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9305 "bison-chpl-lib.cpp"
    break;

  case 509: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2855 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9311 "bison-chpl-lib.cpp"
    break;

  case 510: /* opt_init_expr: %empty  */
#line 2861 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9317 "bison-chpl-lib.cpp"
    break;

  case 511: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2862 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9323 "bison-chpl-lib.cpp"
    break;

  case 512: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2863 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9329 "bison-chpl-lib.cpp"
    break;

  case 513: /* formal_or_ret_type_expr: expr  */
#line 2867 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9335 "bison-chpl-lib.cpp"
    break;

  case 514: /* ret_type: formal_or_ret_type_expr  */
#line 2871 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9341 "bison-chpl-lib.cpp"
    break;

  case 515: /* ret_type: reserved_type_ident_use  */
#line 2872 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9347 "bison-chpl-lib.cpp"
    break;

  case 516: /* ret_type: error  */
#line 2873 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9353 "bison-chpl-lib.cpp"
    break;

  case 517: /* colon_ret_type: TCOLON ret_type  */
#line 2877 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9359 "bison-chpl-lib.cpp"
    break;

  case 518: /* colon_ret_type: error  */
#line 2878 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9365 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_ret_type: %empty  */
#line 2882 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9371 "bison-chpl-lib.cpp"
    break;

  case 521: /* opt_type: %empty  */
#line 2887 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9377 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_type: TCOLON expr  */
#line 2888 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9383 "bison-chpl-lib.cpp"
    break;

  case 523: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2889 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9389 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_type: error  */
#line 2890 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9395 "bison-chpl-lib.cpp"
    break;

  case 525: /* formal_type: formal_or_ret_type_expr  */
#line 2894 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9401 "bison-chpl-lib.cpp"
    break;

  case 526: /* formal_type: reserved_type_ident_use  */
#line 2895 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9407 "bison-chpl-lib.cpp"
    break;

  case 527: /* colon_formal_type: TCOLON formal_type  */
#line 2899 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9413 "bison-chpl-lib.cpp"
    break;

  case 528: /* opt_colon_formal_type: %empty  */
#line 2903 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9419 "bison-chpl-lib.cpp"
    break;

  case 529: /* opt_colon_formal_type: colon_formal_type  */
#line 2904 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9425 "bison-chpl-lib.cpp"
    break;

  case 530: /* expr_ls: expr  */
#line 2910 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9431 "bison-chpl-lib.cpp"
    break;

  case 531: /* expr_ls: expr_ls TCOMMA expr  */
#line 2911 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9437 "bison-chpl-lib.cpp"
    break;

  case 532: /* tuple_component: TUNDERSCORE  */
#line 2915 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9443 "bison-chpl-lib.cpp"
    break;

  case 533: /* tuple_component: opt_try_expr  */
#line 2916 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9449 "bison-chpl-lib.cpp"
    break;

  case 534: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2921 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9457 "bison-chpl-lib.cpp"
    break;

  case 535: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2925 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9465 "bison-chpl-lib.cpp"
    break;

  case 536: /* opt_actual_ls: %empty  */
#line 2931 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9471 "bison-chpl-lib.cpp"
    break;

  case 537: /* opt_actual_ls: actual_ls  */
#line 2932 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9477 "bison-chpl-lib.cpp"
    break;

  case 538: /* actual_ls: actual_expr  */
#line 2937 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9486 "bison-chpl-lib.cpp"
    break;

  case 539: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2942 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9496 "bison-chpl-lib.cpp"
    break;

  case 540: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2950 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9502 "bison-chpl-lib.cpp"
    break;

  case 541: /* actual_expr: opt_try_expr  */
#line 2951 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9508 "bison-chpl-lib.cpp"
    break;

  case 542: /* ident_expr: ident_use  */
#line 2955 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9514 "bison-chpl-lib.cpp"
    break;

  case 543: /* ident_expr: scalar_type  */
#line 2956 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9520 "bison-chpl-lib.cpp"
    break;

  case 549: /* sub_type_level_expr: TSINGLE expr  */
#line 2969 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9526 "bison-chpl-lib.cpp"
    break;

  case 550: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2971 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9532 "bison-chpl-lib.cpp"
    break;

  case 551: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2973 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9538 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2975 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9544 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2977 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9554 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TATOMIC expr  */
#line 2983 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9560 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TSYNC expr  */
#line 2985 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9566 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TOWNED  */
#line 2988 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9572 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TOWNED expr  */
#line 2990 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9578 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TUNMANAGED  */
#line 2992 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9584 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2994 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9590 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TSHARED  */
#line 2996 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9596 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TSHARED expr  */
#line 2998 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9602 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TBORROWED  */
#line 3000 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9608 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TBORROWED expr  */
#line 3002 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9614 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TCLASS  */
#line 3004 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9620 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TRECORD  */
#line 3006 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9626 "bison-chpl-lib.cpp"
    break;

  case 566: /* for_expr: no_loop_attributes for_expr_base  */
#line 3010 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9632 "bison-chpl-lib.cpp"
    break;

  case 567: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3015 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9640 "bison-chpl-lib.cpp"
    break;

  case 568: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3019 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9648 "bison-chpl-lib.cpp"
    break;

  case 569: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3023 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yyloc), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9656 "bison-chpl-lib.cpp"
    break;

  case 570: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3027 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9669 "bison-chpl-lib.cpp"
    break;

  case 571: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3036 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9682 "bison-chpl-lib.cpp"
    break;

  case 572: /* for_expr_base: for_loop_kw expr TDO TIF expr TTHEN expr  */
#line 3045 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yyloc), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9695 "bison-chpl-lib.cpp"
    break;

  case 573: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3056 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9701 "bison-chpl-lib.cpp"
    break;

  case 574: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3061 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9709 "bison-chpl-lib.cpp"
    break;

  case 575: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3065 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9717 "bison-chpl-lib.cpp"
    break;

  case 576: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3069 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9725 "bison-chpl-lib.cpp"
    break;

  case 577: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3073 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9733 "bison-chpl-lib.cpp"
    break;

  case 578: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3077 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9741 "bison-chpl-lib.cpp"
    break;

  case 579: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3081 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9749 "bison-chpl-lib.cpp"
    break;

  case 580: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3085 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9757 "bison-chpl-lib.cpp"
    break;

  case 581: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3092 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9771 "bison-chpl-lib.cpp"
    break;

  case 582: /* nil_expr: TNIL  */
#line 3109 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9777 "bison-chpl-lib.cpp"
    break;

  case 590: /* opt_task_intent_ls: %empty  */
#line 3127 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9783 "bison-chpl-lib.cpp"
    break;

  case 591: /* opt_task_intent_ls: task_intent_clause  */
#line 3128 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9789 "bison-chpl-lib.cpp"
    break;

  case 592: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3133 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9797 "bison-chpl-lib.cpp"
    break;

  case 593: /* task_intent_ls: intent_expr  */
#line 3139 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9803 "bison-chpl-lib.cpp"
    break;

  case 594: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3140 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9809 "bison-chpl-lib.cpp"
    break;

  case 595: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3145 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 9819 "bison-chpl-lib.cpp"
    break;

  case 596: /* forall_intent_ls: intent_expr  */
#line 3153 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9825 "bison-chpl-lib.cpp"
    break;

  case 597: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3154 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9831 "bison-chpl-lib.cpp"
    break;

  case 598: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3159 "chpl.ypp"
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
#line 9849 "bison-chpl-lib.cpp"
    break;

  case 599: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3173 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 9857 "bison-chpl-lib.cpp"
    break;

  case 600: /* intent_expr: expr TREDUCE ident_expr  */
#line 3177 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9865 "bison-chpl-lib.cpp"
    break;

  case 601: /* task_var_prefix: TCONST  */
#line 3183 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 9871 "bison-chpl-lib.cpp"
    break;

  case 602: /* task_var_prefix: TIN  */
#line 3184 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 9877 "bison-chpl-lib.cpp"
    break;

  case 603: /* task_var_prefix: TCONST TIN  */
#line 3185 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 9883 "bison-chpl-lib.cpp"
    break;

  case 604: /* task_var_prefix: TREF  */
#line 3186 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 9889 "bison-chpl-lib.cpp"
    break;

  case 605: /* task_var_prefix: TCONST TREF  */
#line 3187 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 9895 "bison-chpl-lib.cpp"
    break;

  case 606: /* task_var_prefix: TVAR  */
#line 3188 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 9901 "bison-chpl-lib.cpp"
    break;

  case 607: /* new_maybe_decorated: TNEW  */
#line 3193 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 9907 "bison-chpl-lib.cpp"
    break;

  case 608: /* new_maybe_decorated: TNEW TOWNED  */
#line 3195 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 9913 "bison-chpl-lib.cpp"
    break;

  case 609: /* new_maybe_decorated: TNEW TSHARED  */
#line 3197 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 9919 "bison-chpl-lib.cpp"
    break;

  case 610: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3199 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 9925 "bison-chpl-lib.cpp"
    break;

  case 611: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3201 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 9931 "bison-chpl-lib.cpp"
    break;

  case 612: /* new_expr: new_maybe_decorated expr  */
#line 3207 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 9939 "bison-chpl-lib.cpp"
    break;

  case 613: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3214 "chpl.ypp"
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
#line 9955 "bison-chpl-lib.cpp"
    break;

  case 614: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3226 "chpl.ypp"
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
#line 9971 "bison-chpl-lib.cpp"
    break;

  case 615: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3238 "chpl.ypp"
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
#line 9988 "bison-chpl-lib.cpp"
    break;

  case 616: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3251 "chpl.ypp"
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
#line 10006 "bison-chpl-lib.cpp"
    break;

  case 617: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3268 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10014 "bison-chpl-lib.cpp"
    break;

  case 618: /* range_literal_expr: expr TDOTDOT expr  */
#line 3275 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10023 "bison-chpl-lib.cpp"
    break;

  case 619: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3280 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10032 "bison-chpl-lib.cpp"
    break;

  case 620: /* range_literal_expr: expr TDOTDOT  */
#line 3285 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10041 "bison-chpl-lib.cpp"
    break;

  case 621: /* range_literal_expr: TDOTDOT expr  */
#line 3290 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10051 "bison-chpl-lib.cpp"
    break;

  case 622: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3296 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10061 "bison-chpl-lib.cpp"
    break;

  case 623: /* range_literal_expr: TDOTDOT  */
#line 3302 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10071 "bison-chpl-lib.cpp"
    break;

  case 624: /* cast_expr: expr TCOLON expr  */
#line 3332 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10079 "bison-chpl-lib.cpp"
    break;

  case 625: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3339 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10087 "bison-chpl-lib.cpp"
    break;

  case 626: /* super_expr: fn_expr  */
#line 3345 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10093 "bison-chpl-lib.cpp"
    break;

  case 630: /* expr: sub_type_level_expr TQUESTION  */
#line 3354 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10099 "bison-chpl-lib.cpp"
    break;

  case 631: /* expr: TQUESTION  */
#line 3356 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10105 "bison-chpl-lib.cpp"
    break;

  case 635: /* expr: fn_type  */
#line 3361 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10111 "bison-chpl-lib.cpp"
    break;

  case 645: /* opt_expr: %empty  */
#line 3375 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10117 "bison-chpl-lib.cpp"
    break;

  case 646: /* opt_expr: expr  */
#line 3376 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10123 "bison-chpl-lib.cpp"
    break;

  case 647: /* opt_try_expr: TTRY expr  */
#line 3380 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10129 "bison-chpl-lib.cpp"
    break;

  case 648: /* opt_try_expr: TTRYBANG expr  */
#line 3381 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10135 "bison-chpl-lib.cpp"
    break;

  case 649: /* opt_try_expr: super_expr  */
#line 3382 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10141 "bison-chpl-lib.cpp"
    break;

  case 655: /* call_base_expr: expr TBANG  */
#line 3399 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10149 "bison-chpl-lib.cpp"
    break;

  case 656: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3402 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10155 "bison-chpl-lib.cpp"
    break;

  case 659: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3409 "chpl.ypp"
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
#line 10171 "bison-chpl-lib.cpp"
    break;

  case 660: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3421 "chpl.ypp"
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
#line 10187 "bison-chpl-lib.cpp"
    break;

  case 661: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3433 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10195 "bison-chpl-lib.cpp"
    break;

  case 662: /* dot_expr: expr TDOT ident_use  */
#line 3440 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10201 "bison-chpl-lib.cpp"
    break;

  case 663: /* dot_expr: expr TDOT TTYPE  */
#line 3442 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10207 "bison-chpl-lib.cpp"
    break;

  case 664: /* dot_expr: expr TDOT TDOMAIN  */
#line 3444 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10213 "bison-chpl-lib.cpp"
    break;

  case 665: /* dot_expr: expr TDOT TLOCALE  */
#line 3446 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10219 "bison-chpl-lib.cpp"
    break;

  case 666: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3448 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10225 "bison-chpl-lib.cpp"
    break;

  case 667: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3450 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10231 "bison-chpl-lib.cpp"
    break;

  case 668: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3458 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10237 "bison-chpl-lib.cpp"
    break;

  case 669: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3460 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10245 "bison-chpl-lib.cpp"
    break;

  case 670: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3464 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10253 "bison-chpl-lib.cpp"
    break;

  case 671: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3468 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10261 "bison-chpl-lib.cpp"
    break;

  case 672: /* bool_literal: TFALSE  */
#line 3474 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10267 "bison-chpl-lib.cpp"
    break;

  case 673: /* bool_literal: TTRUE  */
#line 3475 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10273 "bison-chpl-lib.cpp"
    break;

  case 674: /* str_bytes_literal: STRINGLITERAL  */
#line 3479 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10279 "bison-chpl-lib.cpp"
    break;

  case 675: /* str_bytes_literal: BYTESLITERAL  */
#line 3480 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10285 "bison-chpl-lib.cpp"
    break;

  case 678: /* literal_expr: INTLITERAL  */
#line 3486 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10291 "bison-chpl-lib.cpp"
    break;

  case 679: /* literal_expr: REALLITERAL  */
#line 3487 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10297 "bison-chpl-lib.cpp"
    break;

  case 680: /* literal_expr: IMAGLITERAL  */
#line 3488 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10303 "bison-chpl-lib.cpp"
    break;

  case 681: /* literal_expr: CSTRINGLITERAL  */
#line 3489 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10309 "bison-chpl-lib.cpp"
    break;

  case 682: /* literal_expr: TNONE  */
#line 3490 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10315 "bison-chpl-lib.cpp"
    break;

  case 683: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3492 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10324 "bison-chpl-lib.cpp"
    break;

  case 684: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3497 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10333 "bison-chpl-lib.cpp"
    break;

  case 685: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3502 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10342 "bison-chpl-lib.cpp"
    break;

  case 686: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3507 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10351 "bison-chpl-lib.cpp"
    break;

  case 687: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3512 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10360 "bison-chpl-lib.cpp"
    break;

  case 688: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3517 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10369 "bison-chpl-lib.cpp"
    break;

  case 689: /* assoc_expr_ls: expr TALIAS expr  */
#line 3526 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10378 "bison-chpl-lib.cpp"
    break;

  case 690: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3531 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10388 "bison-chpl-lib.cpp"
    break;

  case 691: /* binary_op_expr: expr TPLUS expr  */
#line 3539 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10394 "bison-chpl-lib.cpp"
    break;

  case 692: /* binary_op_expr: expr TMINUS expr  */
#line 3540 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10400 "bison-chpl-lib.cpp"
    break;

  case 693: /* binary_op_expr: expr TSTAR expr  */
#line 3541 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10406 "bison-chpl-lib.cpp"
    break;

  case 694: /* binary_op_expr: expr TDIVIDE expr  */
#line 3542 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10412 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3543 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10418 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3544 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10424 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TMOD expr  */
#line 3545 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10430 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TEQUAL expr  */
#line 3546 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10436 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3547 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10442 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3548 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10448 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3549 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10454 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TLESS expr  */
#line 3550 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10460 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TGREATER expr  */
#line 3551 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10466 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TBAND expr  */
#line 3552 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10472 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TBOR expr  */
#line 3553 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10478 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TBXOR expr  */
#line 3554 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10484 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TAND expr  */
#line 3555 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10490 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TOR expr  */
#line 3556 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10496 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TEXP expr  */
#line 3557 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10502 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TBY expr  */
#line 3558 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10508 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TALIGN expr  */
#line 3559 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10514 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr THASH expr  */
#line 3560 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10520 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TDMAPPED expr  */
#line 3561 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10526 "bison-chpl-lib.cpp"
    break;

  case 714: /* unary_op_expr: TPLUS expr  */
#line 3565 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10532 "bison-chpl-lib.cpp"
    break;

  case 715: /* unary_op_expr: TMINUS expr  */
#line 3566 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10538 "bison-chpl-lib.cpp"
    break;

  case 716: /* unary_op_expr: TMINUSMINUS expr  */
#line 3567 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10544 "bison-chpl-lib.cpp"
    break;

  case 717: /* unary_op_expr: TPLUSPLUS expr  */
#line 3568 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10550 "bison-chpl-lib.cpp"
    break;

  case 718: /* unary_op_expr: TBANG expr  */
#line 3569 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10556 "bison-chpl-lib.cpp"
    break;

  case 719: /* unary_op_expr: expr TBANG  */
#line 3570 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10564 "bison-chpl-lib.cpp"
    break;

  case 720: /* unary_op_expr: TBNOT expr  */
#line 3573 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10570 "bison-chpl-lib.cpp"
    break;

  case 721: /* reduce_expr: expr TREDUCE expr  */
#line 3578 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10578 "bison-chpl-lib.cpp"
    break;

  case 722: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3582 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10586 "bison-chpl-lib.cpp"
    break;

  case 723: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3586 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10594 "bison-chpl-lib.cpp"
    break;

  case 724: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3590 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10602 "bison-chpl-lib.cpp"
    break;

  case 725: /* scan_expr: expr TSCAN expr  */
#line 3597 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10610 "bison-chpl-lib.cpp"
    break;

  case 726: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3601 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10618 "bison-chpl-lib.cpp"
    break;

  case 727: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3605 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10626 "bison-chpl-lib.cpp"
    break;

  case 728: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3609 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10634 "bison-chpl-lib.cpp"
    break;


#line 10638 "bison-chpl-lib.cpp"

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
