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
  YYSYMBOL_enum_header = 271,              /* enum_header  */
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
#line 326 "chpl.ypp"

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
#define YYLAST   18247

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  182
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  205
/* YYNRULES -- Number of rules.  */
#define YYNRULES  735
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1265

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
       0,   635,   635,   639,   640,   645,   646,   654,   658,   668,
     673,   680,   681,   682,   683,   684,   685,   686,   687,   688,
     689,   690,   708,   709,   710,   718,   727,   736,   740,   747,
     755,   756,   757,   758,   759,   760,   761,   762,   764,   784,
     792,   809,   825,   842,   859,   875,   896,   897,   901,   911,
     912,   916,   920,   921,   925,   932,   940,   943,   953,   955,
     960,   966,   971,   980,   984,  1005,  1022,  1039,  1057,  1058,
    1060,  1065,  1066,  1071,  1089,  1094,  1099,  1111,  1130,  1131,
    1135,  1139,  1143,  1148,  1152,  1156,  1166,  1171,  1177,  1184,
    1189,  1196,  1207,  1208,  1212,  1217,  1225,  1234,  1244,  1252,
    1261,  1274,  1282,  1283,  1284,  1285,  1286,  1287,  1288,  1289,
    1290,  1291,  1292,  1293,  1294,  1295,  1296,  1297,  1298,  1310,
    1314,  1319,  1324,  1330,  1339,  1340,  1344,  1353,  1357,  1361,
    1365,  1369,  1373,  1382,  1383,  1384,  1388,  1389,  1390,  1391,
    1392,  1396,  1397,  1398,  1410,  1411,  1412,  1417,  1418,  1419,
    1420,  1421,  1422,  1423,  1424,  1425,  1426,  1427,  1428,  1429,
    1430,  1431,  1432,  1433,  1434,  1435,  1439,  1440,  1441,  1442,
    1443,  1444,  1445,  1446,  1447,  1448,  1449,  1450,  1457,  1458,
    1459,  1460,  1464,  1465,  1469,  1476,  1486,  1490,  1496,  1496,
    1502,  1502,  1511,  1512,  1513,  1514,  1515,  1516,  1517,  1521,
    1526,  1531,  1536,  1543,  1551,  1552,  1556,  1565,  1572,  1581,
    1600,  1599,  1609,  1608,  1621,  1627,  1630,  1631,  1640,  1641,
    1642,  1646,  1669,  1688,  1707,  1712,  1717,  1722,  1747,  1752,
    1757,  1762,  1767,  1772,  1777,  1782,  1787,  1792,  1797,  1802,
    1807,  1812,  1817,  1823,  1832,  1841,  1845,  1849,  1853,  1857,
    1861,  1865,  1869,  1873,  1878,  1883,  1888,  1896,  1911,  1929,
    1933,  1940,  1941,  1946,  1951,  1952,  1953,  1954,  1955,  1956,
    1957,  1958,  1959,  1960,  1961,  1962,  1967,  1972,  1973,  1974,
    1975,  1984,  1985,  1989,  1993,  1997,  2004,  2008,  2012,  2019,
    2023,  2027,  2031,  2038,  2039,  2043,  2047,  2051,  2058,  2071,
    2087,  2095,  2100,  2110,  2111,  2115,  2119,  2124,  2133,  2138,
    2142,  2149,  2150,  2154,  2164,  2170,  2182,  2189,  2190,  2191,
    2195,  2196,  2200,  2204,  2208,  2212,  2216,  2224,  2243,  2256,
    2263,  2268,  2275,  2274,  2284,  2290,  2289,  2304,  2311,  2323,
    2341,  2338,  2366,  2370,  2371,  2373,  2378,  2379,  2383,  2384,
    2388,  2391,  2393,  2398,  2399,  2410,  2430,  2429,  2445,  2444,
    2462,  2472,  2469,  2496,  2504,  2512,  2523,  2534,  2543,  2558,
    2559,  2563,  2564,  2565,  2574,  2575,  2576,  2577,  2578,  2579,
    2580,  2581,  2582,  2583,  2584,  2585,  2586,  2587,  2588,  2589,
    2590,  2591,  2592,  2593,  2594,  2595,  2596,  2597,  2598,  2602,
    2603,  2604,  2605,  2606,  2607,  2608,  2609,  2610,  2611,  2612,
    2613,  2618,  2619,  2623,  2624,  2628,  2629,  2633,  2634,  2638,
    2639,  2643,  2644,  2648,  2652,  2653,  2657,  2661,  2666,  2671,
    2676,  2681,  2689,  2693,  2701,  2702,  2703,  2704,  2705,  2706,
    2707,  2708,  2709,  2713,  2714,  2715,  2716,  2717,  2718,  2722,
    2723,  2724,  2728,  2729,  2730,  2731,  2732,  2733,  2734,  2738,
    2739,  2742,  2743,  2747,  2748,  2752,  2757,  2758,  2760,  2762,
    2764,  2769,  2771,  2776,  2778,  2780,  2782,  2784,  2786,  2788,
    2793,  2794,  2795,  2799,  2808,  2812,  2820,  2824,  2831,  2852,
    2853,  2858,  2859,  2860,  2861,  2862,  2867,  2866,  2875,  2880,
    2888,  2892,  2899,  2918,  2937,  2941,  2945,  2952,  2954,  2956,
    2963,  2964,  2965,  2969,  2973,  2974,  2975,  2979,  2980,  2984,
    2985,  2989,  2990,  2991,  2992,  2996,  2997,  3001,  3005,  3006,
    3012,  3013,  3017,  3018,  3022,  3026,  3033,  3034,  3038,  3043,
    3052,  3053,  3057,  3058,  3065,  3066,  3067,  3068,  3069,  3070,
    3072,  3074,  3076,  3078,  3084,  3086,  3089,  3091,  3093,  3095,
    3097,  3099,  3101,  3103,  3105,  3107,  3112,  3116,  3120,  3124,
    3128,  3137,  3146,  3158,  3162,  3166,  3170,  3174,  3178,  3182,
    3186,  3193,  3211,  3219,  3220,  3221,  3222,  3223,  3224,  3225,
    3229,  3230,  3234,  3241,  3242,  3246,  3255,  3256,  3260,  3275,
    3279,  3286,  3287,  3288,  3289,  3290,  3291,  3295,  3297,  3299,
    3301,  3303,  3309,  3316,  3328,  3340,  3353,  3370,  3377,  3382,
    3387,  3392,  3398,  3404,  3434,  3441,  3448,  3449,  3453,  3455,
    3456,  3458,  3460,  3461,  3462,  3463,  3466,  3467,  3468,  3469,
    3470,  3471,  3472,  3473,  3474,  3478,  3479,  3483,  3484,  3485,
    3489,  3490,  3491,  3492,  3501,  3502,  3505,  3506,  3507,  3511,
    3523,  3535,  3542,  3544,  3546,  3548,  3550,  3552,  3561,  3567,
    3571,  3575,  3582,  3583,  3587,  3588,  3592,  3593,  3594,  3595,
    3596,  3597,  3598,  3599,  3604,  3609,  3614,  3619,  3624,  3633,
    3638,  3647,  3648,  3649,  3650,  3651,  3652,  3653,  3654,  3655,
    3656,  3657,  3658,  3659,  3660,  3661,  3662,  3663,  3664,  3665,
    3666,  3667,  3668,  3669,  3673,  3674,  3675,  3676,  3677,  3678,
    3681,  3685,  3689,  3693,  3697,  3704,  3708,  3712,  3716,  3724,
    3725,  3726,  3727,  3728,  3729,  3730
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
  "enum_header", "enum_ls", "$@5", "$@6", "enum_item", "lambda_decl_start",
  "lambda_decl_expr", "$@7", "linkage_spec_empty", "linkage_spec",
  "opt_fn_type_formal_ls", "fn_type_formal_ls", "fn_type_formal",
  "opt_fn_type_ret_type", "fn_type", "fn_expr", "$@8", "$@9",
  "fn_decl_stmt_complete", "fn_decl_stmt", "$@10", "fn_decl_stmt_inner",
  "fn_decl_stmt_start", "fn_decl_receiver_expr", "fn_ident", "op_ident",
  "assignop_ident", "all_op_name", "formal_var_arg_expr", "opt_formal_ls",
  "req_formal_ls", "formal_ls_inner", "formal_ls", "formal",
  "formal_ident_def", "named_formal", "opt_formal_intent_tag",
  "required_intent_tag", "opt_this_intent_tag", "proc_iter_or_op",
  "opt_ret_tag", "opt_throws_error", "opt_function_body_stmt",
  "function_body_stmt", "query_expr", "opt_lifetime_where",
  "lifetime_components_expr", "lifetime_expr", "lifetime_ident",
  "type_alias_decl_stmt", "type_alias_decl_stmt_start",
  "type_alias_decl_stmt_inner_ls", "type_alias_decl_stmt_inner",
  "opt_init_type", "var_decl_type", "var_decl_stmt", "$@11",
  "var_decl_stmt_inner_ls", "var_decl_stmt_inner",
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

#define YYPACT_NINF (-900)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-736)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -900,   102,  3856,  -900,   -67,    98,  -900,  -900,  -900,  -900,
    -900,  -900,  -900, 12128,    68,    61,   111, 13988,   203, 18018,
      68,   222,   179,   223,    61,  5264, 12128,   121, 18074,  -900,
     317,   315,  -900,  9840,   408,   283,   247,  -900,   420,   389,
   18074, 18074, 18074,  -900,  3518, 10720,   431, 12128, 12128,   173,
    -900,   444,   454, 12128,  -900, 13988,  -900, 12128,   513,   361,
     259,   307, 13332,   495, 18130,  -900, 12128,  8432, 12128, 10720,
   13988, 12128,   478,   550,   409,  5264,   553, 12128,   555,  6848,
    6848,  -900,   572,  -900, 13988,  -900,   573, 10016,  -900,   589,
   12128,  -900, 12128,  -900,  -900, 13815, 12128,  -900, 10192,  -900,
    -900,  -900,  4208,  7552, 12128,  -900,  4912,  -900,  -900,  -900,
    -900, 17618,   458,  -900,   481,   462,  -900,   309,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  7024,  -900,  7200,  -900,  -900,    99,  -900,  -900,  -900,
     583,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,   498,
   18074,  -900,   470,   473,   242,  -900,    40,  -900,  -900,  -900,
     335,   227,  -900,  -900, 18074,  1787,  -900,   483,   482,  -900,
    -900,  -900,   484,   488, 12128,   489,   492,  -900,  -900,  -900,
   17429,  2673,   234,   493,   497,  -900,  -900,   237,  -900,  -900,
    -900,  -900,  -900,   494,  -900,  -900,  -900, 12128,  -900, 18074,
    -900, 12128, 12128,    46,   612,   340,  -900,  -900,  -900,  -900,
   17429,   370,  -900,  -900,   499,  5264,  -900,  -900,  -900,   509,
     142,   490,  -900,   263,  -900,   511,  -900,   181, 17429,  8608,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900, 18074,
   18074,   -34, 14451,  -900,  -900,   594,  8608,   514,   510,   371,
    5264,  2187,    51,    82,  -900,  5264,  -900,  -900, 15433,  -900,
      21, 15873,   828,  -900,   515,   516,  -900, 15433,   142,   828,
    -900,  8608,  1816,  1816,    64,     2,  -900,     7,  -900,  -900,
    -900,  -900,  7728,  7728,  -900,   605,  -900,   523,   539,  -900,
    -900,  3124,   535,  -900,  -900, 17429,   232, 12128, 12128,  -900,
     -28,  -900,  -900, 17429,   529, 15994,  -900, 15433,   142, 17429,
     517,  8608,  -900, 17429, 16034,  -900,  -900,  -900,  -900,  -900,
     142,   532,  -900,   166,   166,   676,   828,   828,   -69,  -900,
    -900,  4384,    89,  -900, 12128,   551,   -39,   528,   676,   693,
    -900,  -900, 18074,  -900,  -900,  4208,  -900,   106,  -900,   594,
    -900,  -900,   701,   537,  4560, 12128,  -900, 12128,   639,  -900,
    -900, 15104,    45,   498, 17429,   382, 12128,  5264,   633,  -900,
    -900,  9840, 10368, 10896,  -900,  -900,  -900, 18074, 12128,   542,
    -900, 17794,   534,   335,  -900,  -900,  -900,  -900,   627,  -900,
    -900,  -900,  -900,   144,   629,  -900,  -900,  -900, 13644,   597,
     305,  -900,   559,   330,   383,  -900,   576, 12128, 12128, 12128,
   10896, 10896, 12128,   384, 12128, 12128, 12128, 12128, 12128,   485,
   13815, 12128, 12128, 12128, 12128, 12128, 12128, 12128, 12128, 12128,
   12128, 12128, 12128, 12128, 12128, 12128, 12128,   658,  -900,  -900,
    -900,  -900,  -900, 10016, 10016,  -900,  -900,  -900,  -900, 10016,
    -900,  -900, 10016, 10016,  8608,  8784, 10896, 10896, 16079,   556,
    -900, 11072, 12128, 18074,  7904,  -900,  -900,  -900,  3518,  -900,
   12128,  -900,   606,   560,   590,  -900,  -900,   611,   613,  5264,
     707,  5264,  -900,   709, 12128,   678,   574,  -900, 10016, 18074,
    -900,  -900,  -900,  2187,  -900,  -900,    16,  -900, 12304,   621,
   12128,  3518,  -900,  -900, 12128,  -900, 17906, 12128, 12128,  -900,
     577,  -900,  -900,  -900,  -900, 17962,  -900,   335, 13507,  -900,
    2187,  -900,   603, 12304,  -900, 17429, 17429,  -900,   490,  -900,
      43,  -900, 10016,   579,  -900,   733,   733,  -900,  -900,  -900,
    -900, 11248,  -900, 16155,  8080,  8256,  -900,  -900,  -900, 10896,
   10896,  -900,   481,  8960,  -900,   296,  -900,  4736,  -900,   337,
   16196,   350, 15196, 18074,  6672,  6496,  -900,   498,   585,  -900,
     263,  -900, 15518,   637, 18074,    15, 15433,   586, 14106,   -56,
     219, 16242,    76,    29, 14936,   127,   614,  1914,   588, 17850,
     638,   105,  -900,  -900,   128,  -900,  -900,    47,  -900,  -900,
    6320,  -900,   657,  -900,   595,   617,  -900,   622,  -900,   625,
     628,   630,   632,   635,  -900,   636,   640,   642,   643,   648,
     650,   654,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900, 12128,   645,   660,   595,  -900,   595,
    -900, 12128,  -900, 18074,  -900,  -900,  -900, 17595, 17595,   424,
    -900,   424,  -900,   424, 17632,   634,  1240,   524,   142,   166,
    -900,   418,  -900,  -900,  -900,  -900,  -900,   676,  3578,   424,
    1521,  1521, 17595,  1521,  1521,   189,   166,  3578,  2606,   189,
     828,   828,   166,   676,   623,   646,   649,   651,   663,   668,
     664,   665,  -900,   424,  -900,   424, 12128, 10016,    60, 15594,
     598,    66,  -900,  -900,  -900,   138,  -900,  2443, 17474,   508,
     356, 17429, 10016,  -900, 10016, 12128, 12128,   745,  5264,   774,
   16349,  5264, 15635, 18074,  -900,   174,  -900,   185,  -900,   212,
    2187,    51,  -900, 17429,  9136,  -900, 17429,  -900,  -900,  -900,
   18074, 16395, 16435,  -900,   603,   669,  -900,   216, 12304,  -900,
     250,  -900, 12128,  -900,   652,    -1,   673,  -900,  3247,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,   220, 15012,  -900,
    -900,   251,  -900,  -900,  -900,  -900, 12128,  -900,    79, 14209,
   12128,  -900, 11424,  6672,  6496,  -900,  9312,   518,  9488,   526,
     527,  9664,  7376,   536,   364,  -900,  -900,  -900, 16588,   691,
     682,   679,  -900, 18074,  3518, 10896,    15, 12128,   802,  -900,
    -900, 12128, 17429,   683,  5264,   675, 10896, 11600,  4032,   681,
   12128, 11776,  -900,  -900,    15,  5440, 10896,  -900,    15,  -900,
   10016,   680,  1388,  -900,  -900, 12128,   147,  -900,   718,  -900,
     657,  -900,  -900,  -900,  -900,  -900,    33,   543,  -900, 16628,
    -900, 14369,  -900,  -900, 17429,  -900,   686,   687,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,   253, 10896, 14106, 12480,
   10896, 10016,  -900,  -900,  7904,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,    37, 10896, 10896,
    -900,  -900,  -900, 17429, 17429,  5264,  -900,  5264, 12128,  -900,
    5264,   821,   696,   698, 18074,   490,  -900,  -900,   621,  -900,
    -900,  -900,   700,   702,   250, 12128,   657,   603,  -900, 10544,
    -900,  -900, 17429,  -900,  5264, 12128,  -900,  -900,  -900, 18074,
    -900,   719,   490, 10896,  5264, 10896, 10016,  -900,  -900,  -900,
    -900,  -900,   418,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  6672,  -900,  -900,
   16668,    83, 15788,   369,   705,  6672,  -900, 12128,  -900,  -900,
    1484,   372,   381,    68, 16749,  -900, 16789, 12128,   255,  7904,
    -900,  5264,   -50, 15680,  -900,  -900, 17429,  5264, 17429,  -900,
   16835,  -900,  9840, 17429,    30, 15272,  -900,   270,  -900,  -900,
    -900,  -900,  2226,  2328,  -900, 17429, 18074,   458,  -900,    33,
      56, 12128,  -900,   728,   703,  -900,   595,   595,  -900,  -900,
    -900,   704, 16942, 12128,   846, 16988,   275,  -900,   621,   289,
     310,  -900,  -900, 17429,   848,  5264,  -900,  -900,  -900,  -900,
    -900,  8608,  8608,  -900,  -900,  -900,  -900,   250, 17429,  -900,
     207,    24,   711, 12128,  -900,   -49, 15833,    29, 15357,  -900,
     151, 11952,  6672,  6496,  6672,  6496,  -900,  -900,  -900, 17029,
   17429,  2016,  -900,  -900,  -900,    15,    15,  -900, 15433,  -900,
     321,  -900,  -900,  5616,   712,  5792,   713,  -900, 12128, 14531,
    5968,  -900,    15,  6144,  -900,    15,   717,  2328,  -900,  -900,
    -900, 17850,  -900,  -900,  -900,   119,  -900,   -27,  -900,   324,
   17109,    63,  -900,  -900,  -900, 12656, 12832, 17150, 13008, 13184,
    -900,  -900,  5264,  -900,   716,   720,  -900,  -900,  5264,  -900,
     490, 17429,  5264,  5264,  -900,  -900, 17429,   388,   721,   401,
     725, 12128,  -900,  -900,  -900,  -900,  7904,  -900,  9840,  -900,
   17429,  5264,  9840,  -900, 17429,  5264, 17429,  5264,  9840, 17429,
    -900,  9840, 17429,  -900,  -900,  -900,  -900,    36,  -900, 12128,
      56,   119,   119,   119,   119,   119,   119,    56,  5088,  -900,
    -900,  -900,  -900, 12128, 12128, 12128, 12128, 12128,  -900,   727,
     731,   637,  -900,  -900,  -900,  -900,  -900, 17429,  -900, 14611,
    -900, 14691,  -900, 16349, 14771, 14851,  -900, 17429,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,   749,  -900, 17226, 17267, 17312,
   17388,  -900,  -900,  5264,  5264,  5264,  5264, 12128, 12128, 12128,
   12128, 16349, 16349, 16349, 16349
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
     137,   139,   149,   160,   165,   143,   329,   140,   214,     0,
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
     316,     0,   432,   452,   450,   451,   449,   368,   454,   453,
     457,   456,   458,     0,   447,   444,   445,   448,     0,   489,
       0,   486,     0,     0,   630,    35,   612,   215,   215,   215,
     215,   215,   215,   719,   215,   215,   215,   215,   215,     0,
     620,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   399,   406,   407,
     408,   403,   405,   215,   215,   401,   404,   402,   400,   215,
     410,   409,   215,   215,   215,   215,   215,   215,     0,     0,
     218,   215,   215,     0,   215,    37,    25,    38,     0,    26,
     215,    39,   542,     0,   537,   538,   541,     0,     0,     0,
     250,     0,   399,   246,   215,     0,     0,    24,   215,     0,
     260,    27,   504,     0,   505,   507,     0,   524,   215,   510,
     215,     0,   182,    40,   215,   313,     0,   215,   215,    42,
       0,   191,   189,   437,   438,     0,   433,   452,   432,   425,
       0,   424,   528,   215,   126,   647,   648,   358,     0,   185,
       0,    43,   215,     0,   300,   291,   292,    28,    76,    75,
      79,   215,   683,     0,   215,   215,   670,   668,     8,   215,
     215,   216,    56,   215,    54,     0,    65,     0,   124,     0,
     119,     0,    86,     0,   215,   215,   199,   320,     0,   212,
       0,   209,   590,     0,     0,     0,     0,     0,   574,     0,
       0,   530,     0,     0,     0,   287,   321,     0,     0,     0,
     337,     0,   330,   417,     0,   419,   423,     0,   455,   518,
       0,   520,   459,   446,   415,   141,   395,   147,   393,   153,
     152,   151,   148,   142,   397,   163,   164,   150,   154,   137,
     149,   165,   392,   374,   377,   375,   376,   398,   387,   378,
     391,   383,   381,   394,   382,   380,   385,   390,   379,   384,
     388,   389,   386,   396,   215,   371,     0,   415,   372,   415,
     369,   215,   488,     0,   483,   499,   498,   711,   710,   713,
     722,   721,   726,   725,   707,   704,   705,   706,   624,   694,
     144,     0,   664,   665,   146,   663,   662,   618,   698,   709,
     703,   701,   712,   702,   700,   692,   697,   699,   708,   691,
     695,   696,   693,   619,     0,     0,     0,     0,     0,     0,
       0,     0,   724,   723,   728,   727,   215,   215,     0,     0,
     287,   601,   602,   604,   606,     0,   593,     0,     0,     0,
       0,   531,   215,   551,   215,   215,   215,   249,     0,   245,
       0,     0,     0,     0,   550,     0,   263,     0,   261,     0,
     508,     0,   523,   522,   215,   502,   617,   501,   312,   309,
       0,     0,     0,   661,   528,   353,   349,     0,   215,   529,
     510,   351,   215,   357,     0,     0,     0,   552,     0,   294,
     684,   625,   669,   534,   533,   671,   535,     0,     0,    57,
      58,     0,    61,    63,    67,    66,   215,   101,     0,     0,
     215,    94,   215,   215,   215,    64,   215,   374,   215,   375,
     376,   215,   215,   386,     0,   411,   412,    81,    80,    93,
       0,     0,   322,     0,     0,   215,     0,   215,     0,   223,
     222,   215,   575,     0,     0,     0,   215,   215,     0,     0,
     215,   215,   687,   234,     0,     0,   215,   232,     0,   284,
     215,     0,   342,   328,   334,   215,   332,   327,   432,   418,
     459,   516,   515,   514,   517,   460,   466,   432,   367,     0,
     373,     0,   363,   364,   490,   487,     0,     0,   132,   130,
     131,   129,   128,   127,   659,   660,     0,   215,   685,   215,
     215,   215,   603,   605,   215,   592,   166,   173,   172,   171,
     168,   175,   176,   170,   174,   169,   177,     0,   215,   215,
     497,   540,   539,   258,   257,     0,   252,     0,   215,   248,
       0,   254,     0,   286,     0,     0,   506,   509,   510,   511,
     512,   308,     0,     0,   510,   215,   459,   528,   527,   215,
     428,   426,   359,   302,     0,   215,   301,   304,   553,     0,
     295,   298,     0,   215,     0,   215,   215,    59,   125,   122,
     102,   114,   108,   107,   106,   116,   103,   117,   112,   105,
     113,   111,   109,   110,   104,   115,   118,   215,   121,   120,
      89,    88,    87,     0,     0,   215,   200,   215,   201,   322,
     342,     0,     0,   590,   590,   226,     0,   215,     0,   215,
     242,     0,     0,     0,   686,   240,   576,     0,   689,   688,
       0,   235,   215,   569,     0,     0,   233,     0,   315,   190,
     188,   314,   342,   342,   323,   338,     0,     0,   420,   466,
       0,   215,   361,   422,     0,   370,   415,   415,   666,   667,
     286,     0,     0,   215,     0,     0,     0,   594,   510,   650,
     650,   251,   247,   581,   253,     0,    73,   283,   262,   259,
     503,   215,   215,   429,   427,   354,   355,   510,   414,   306,
       0,     0,     0,   215,   296,     0,     0,     0,     0,    62,
       0,   215,   215,   215,   215,   215,    95,    98,    84,    83,
      82,   342,   207,   213,   211,     0,     0,   221,     0,   244,
       0,   596,   243,     0,     0,     0,     0,   241,   215,     0,
       0,   230,     0,     0,   228,     0,   288,   342,   325,   324,
     333,     0,   340,   480,   481,     0,   482,   468,   471,     0,
     467,     0,   416,   365,   366,   215,   215,     0,   215,   215,
     288,   598,     0,   256,     0,     0,   431,   430,     0,   305,
       0,   299,     0,     0,   123,    91,    90,     0,     0,     0,
       0,   215,   208,   225,   224,   227,   215,   595,   215,   238,
     578,     0,   215,   236,   577,     0,   690,     0,   215,   568,
     231,   215,   567,   229,   285,   326,   336,     0,   479,   215,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   461,
     463,   362,   462,   215,   215,   215,   215,   215,   255,   613,
     614,   307,   297,    97,   100,    96,    99,    85,   597,     0,
     239,     0,   237,   572,     0,     0,   341,   470,   472,   473,
     476,   477,   478,   474,   475,   469,   464,     0,     0,     0,
       0,   615,   616,     0,     0,     0,     0,   215,   215,   215,
     215,   580,   579,   571,   570
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -900,  -900,  -900,     0,   -57,  1675,  -900,   375,   782,  -900,
    -900,   428,  -395,  -103,  -900,   326,  -900,  -900,   -61,  -900,
    -900,    14,   641,  -900,  -537,  1858,   533,  -568,  -900,  -784,
    -900,  -900,  -900,   103,  -900,  -900,  -900,   886,  -900,  2694,
     -87,  -900,  -900,  -458,   -23,  -900,  -830,  -900,  -900,   276,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,    10,  -900,
    -153,   545,  -249,  -900,   522,  -900,  -900,   -19,  1422,  -900,
    -900,  -900,  -900,   580,  -900,   -38,  -900,  -900,  -900,  -900,
     396,  -900,  -900,  -900,  -100,  -900,  -341,  -793,  -900,  -900,
    -900,  -900,  -900,  -590,  -900,    26,  -900,  -900,  -900,  -900,
    -900,   386,  -900,   -33,  -900,  -900,  -900,  -900,   541,  -900,
    -900,  -900,  -900,    48,  -408,  -178,  -773,  -899,  -642,  -900,
      50,  -900,    74,   397,   -47,   626,   -44,  -900,  -900,  -363,
    -787,  -900,  -261,  -900,   -95,  -260,  -252,  -302,  -900,  -900,
     116,   277,  -900,  -209,   823,  -900,  -158,   434,   197,  -278,
    -751,  -599,  -900,  -900,  -900,  -593,  -506,  -900,  -731,    -9,
      71,  -900,  -265,  -495,  -532,    -2,  -900,  -900,  -900,  -900,
    -900,  -900,   163,  -900,   -16,  -900,  -900,  -523,  -900,  -873,
    -900,  -900,   368,   521,  -900,  -900,  -900,  -900,  2595,  -900,
     -12,   859,  -900,  1217,  1370,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -476
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   350,   106,   107,   108,   109,   110,   367,
     368,   111,   112,   113,   114,   373,   574,   801,   802,   115,
     116,   117,   378,   118,   119,   120,   351,   829,   581,   830,
     121,   122,   988,   578,   579,   123,   124,   219,   255,   272,
     125,   257,   126,   306,   857,   127,   128,   293,   292,   129,
     130,   131,   132,   133,   134,   590,   833,   135,   203,   137,
     393,   394,   599,   138,   261,   139,   757,   758,   204,   141,
     142,   143,   144,   555,   789,   962,   145,   146,   785,   957,
     279,   280,   147,   148,   149,   150,   399,   862,   151,   152,
     611,  1036,  1037,   612,   153,   205,  1197,   155,   156,   307,
     308,   309,   946,   157,   321,   548,   782,   158,   159,  1141,
     160,   161,   666,   667,   825,   826,   827,   950,   878,   403,
     614,  1044,   615,   542,   616,   311,   536,   418,   407,   413,
     876,  1211,  1212,   162,  1042,  1137,  1138,  1139,   163,   164,
     420,   421,   672,   165,   166,   223,   273,   274,   515,   516,
     765,   313,   874,   621,   622,   519,   314,   779,   780,   352,
     355,   356,   493,   494,   495,   206,   168,   169,   395,   170,
     396,   171,   207,   173,   215,   216,   735,   849,  1110,   736,
     737,   174,   208,   209,   177,   178,   179,   322,   180,   385,
     496,   211,   182,   212,   213,   185,   186,   187,   188,   602,
     189,   190,   191,   192,   193
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   506,   105,   473,   221,   305,   609,   423,   739,   371,
     668,   783,   136,   755,   488,   310,   824,   227,   312,   864,
     786,   873,   277,   167,   870,   882,   530,   883,   154,   951,
     994,  1057,  1034,   383,   320,   136,   989,   781,   517,  1000,
     617,   803,   588,   944,   784,  1073,   326,   275,   517,   265,
     482,   154,   517,   275,   320,   324,   275,   316,  -281,  1133,
     762,   275,  1120,  -282,   217,   843,   553,   361,  1208,   499,
     320,   843,   843,   167,   954,   341,   845,   167,   167,  -521,
     854,   858,   690,  1039,   504,   136,   480,   389,   390,   136,
     136,   357,  1199,   404,  1040,  1208,   547,   194,  -521,   194,
     167,   154,     3,   558,   167,   154,   154,   897,  -521,   565,
    1134,   533,   136,   902,   405,   218,   136,   955,  -303,  1092,
     844,  1200,  1133,   386,   406,   369,  1113,  1162,   154,   520,
     386,   387,   154,   198,   364,   566,  1111,   365,   387,   388,
     389,   390,   492,  -356,   603,   619,   388,   389,   390,  1135,
     843,   843,  1041,  -521,  -264,   534,  1093,   903,    83,  1076,
    1136,  -303,   872,  -521,   760,   172,  -521,  -265,   938,   524,
    1034,   956,   490,  1134,   775,   229,  -519,  -521,  1156,   492,
     266,   680,   682,   694,   518,  -521,   365,  1070,   172,   214,
     761,   283,   365,  1074,   518,   365,   320,   429,   518,  -521,
     365,   365,  1128,  1129,   492,  -519,  1101,   365,   847,   720,
     721,  -521,   922,   167,   570,  -303,  1077,  -521,  -521,   391,
     429,   481,  1098,  1136,   851,   136,   391,   722,   724,  1209,
     521,   386,   896,   320,   365,   759,   898,   561,   172,  1158,
     669,   154,   172,   172,   492,   993,   831,   388,   389,   390,
    -519,   284,   852,   866,   414,   523,   430,   525,   320,  -271,
     431,   562,   777,  -519,   529,   172,   846,   963,   167,   172,
     285,  1034,   948,   167,   392,   858,   868,   867,  -270,   430,
     136,   569,   587,   431,   433,   136,   904,   286,  -335,   222,
      23,   620,   439,   859,   229,  -331,   154,  1195,   320,   995,
     860,   154,   869,  1228,   551,   -70,   415,  1151,   433,  1168,
    -519,  1170,   905,   437,   224,  -519,   439,   770,   416,  -331,
     442,   603,   744,  1164,  1058,   -70,  1157,   391,   258,   490,
     740,   433,   434,   934,   435,   436,   437,   417,   438,   439,
     843,   843,    56,   442,   -70,   305,  1075,   491,   933,   167,
     449,   560,   696,   -69,    65,   375,   453,   454,   455,   935,
     760,   136,   408,   167,   760,  1027,   263,   847,   490,   264,
     175,  -279,   167,   -69,   -70,   136,   764,   154,   172,    85,
     490,   834,   392,   600,   136,   167,   936,   492,   492,   606,
     947,   154,   -69,   175,   376,   848,   964,   136,   544,   966,
     154,   744,   949,   490,  1143,  1144,  1056,   474,  -587,   475,
    -658,   409,  -658,   154,   410,  -657,   670,  -657,   744,  1090,
     266,   492,   -69,   744,   377,   967,   411,  1050,   739,  1109,
     320,   320,    37,   172,   803,  -342,   320,  -600,   172,   320,
     320,   320,   320,   175,  1126,   412,  1130,   175,   175,  1150,
    1201,   715,   716,   673,   335,   338,  -342,   717,  -599,    54,
     718,   719,   194,  -600,  -269,   492,  -342,  1033,   804,  1176,
     175,   674,   728,   668,   175,   320,  -266,  1202,   521,  1203,
    1204,   305,  1205,  1206,  -599,   806,   492,  -273,   690,  1114,
    1116,   310,   589,   267,   312,  1177,   676,   167,   810,   167,
    -277,  1122,  1125,   807,   521,   691,   371,   336,   336,   136,
    -274,   136,   995,  -657,   172,  -657,   811,   995,   692,   320,
     673,   290,   920,   176,  1167,   154,  1169,   154,   172,   521,
     996,   320,   320,   739,   291,  1096,   995,   172,  1103,   198,
     320,  1196,   365,  -654,   509,  -654,   176,  1104,   693,   995,
     172,  -268,   794,   794,  1223,   429,  -656,  -655,  -656,  -655,
     797,   294,  1114,  1116,  1122,  1125,   433,  1225,   531,   532,
     294,   437,   839,   840,   439,   167,   836,   560,   442,   330,
     853,   296,   331,   175,   476,   298,  1003,   136,   477,   694,
     296,   886,   342,   887,   298,   695,   176,  1012,   919,    89,
     176,   176,   477,   154,  1210,  1033,  -272,  1024,  -733,  -278,
     299,  -280,  -733,   300,   430,    58,  -734,  -735,   431,   299,
    -734,  -735,   300,   176,    58,   301,  -730,   176,  -267,  -275,
    -730,   372,   294,   374,   301,   793,   796,  1127,   175,   397,
     492,   401,  1131,   175,   304,   398,   402,   424,  1051,  -584,
    -583,  1054,   296,   304,   425,  -588,   298,   492,  -589,  -586,
    1210,   365,   172,  -585,   172,   429,   433,   434,   483,   680,
     722,   437,   484,   438,   439,   486,  1002,   489,   442,   376,
     507,   299,   543,   508,   300,   449,   359,   538,   527,   528,
     552,   453,   454,   455,   320,   549,   301,   537,   557,   564,
     739,   568,   567,  1047,   572,   583,  1033,   429,   613,   320,
     573,   320,   594,   607,  1085,   304,  1087,  -421,   618,   175,
     623,   969,   696,   671,   430,   675,   439,   714,   431,   727,
     921,   320,   742,   175,   743,   917,   176,   745,   744,   746,
     172,   748,   175,   751,   753,   294,   167,   764,   754,   167,
     778,   773,   940,   787,   788,   175,   832,   837,   136,   841,
     863,   136,   490,   875,   865,   296,   430,  -144,   877,   298,
     431,   901,  -166,   492,   154,  -173,   433,   154,  -172,   925,
    -171,   437,  -168,   438,   439,  -145,  -175,   880,   442,   888,
    -176,   176,  -170,  -174,   299,   449,   176,   300,  -146,    58,
    -169,   453,   454,   455,  -177,  1032,  1154,  1155,   927,   301,
     881,   305,   889,  1005,   492,   890,   945,   891,   433,   434,
     305,   435,   436,   437,   953,   438,   439,   320,   304,   892,
     442,  1021,  1008,  1198,   893,  1026,   448,   449,   894,   995,
     452,   895,   167,   453,   454,   455,   167,   958,   998,  1007,
     999,  1011,  1028,   167,   136,  1065,  1009,  1017,   136,   429,
    1048,   181,  1066,  1049,  1067,   136,  1083,   175,   320,   175,
     154,  1097,   176,  1071,   154,  1072,   868,  1142,  1148,   492,
    1145,   154,  1152,  1194,   181,  1160,   176,  1224,  1181,  1185,
    1219,  1226,  1251,   366,  1220,   176,  1252,  1200,   799,  1239,
    1240,  1241,  1242,  1243,  1244,  1089,   505,   577,   176,   968,
     225,   172,   571,   595,   172,  1068,  1059,  1060,   430,   556,
     768,  1082,   431,   167,   776,   167,   591,  1043,   167,  1046,
     371,   535,   774,   320,   181,   136,  1236,   136,   181,   181,
     136,   504,  1038,  1032,  1132,   175,  1081,  1245,  1238,  1001,
     885,   154,   167,   154,   380,   767,   154,   937,     0,     0,
       0,   181,   167,     0,  1080,   181,     0,     0,     0,     0,
     433,     0,     0,     0,   136,   437,     0,   438,   439,     0,
     154,     0,   442,     0,   492,   492,     0,  1105,  1106,   449,
     154,     0,     0,     0,     0,     0,     0,   455,     0,     0,
       0,  1121,  1124,     0,     0,     0,     0,   172,     0,   167,
       0,   172,     0,     0,     0,   167,     0,     0,   172,     0,
     176,   136,   176,     0,   371,     0,     0,   136,   371,     0,
       0,     0,     0,     0,     0,     0,     0,   154,   320,   320,
       0,     0,     0,   154,  1032,     0,     0,     0,     0,     0,
       0,   504,     0,   504,     0,     0,   504,   504,  1159,     0,
       0,     0,     0,   167,  1121,  1124,     0,     0,     0,     0,
       0,     0,     0,     0,   181,   136,     0,     0,     0,     0,
       0,     0,  1173,  1174,     0,  1175,     0,     0,   172,     0,
     172,   154,     0,   172,     0,     0,     0,     0,   176,  1190,
       0,     0,  1193,     0,     0,     0,     0,     0,     0,     0,
       0,   167,     0,   167,     0,     0,   175,   172,   167,   175,
       0,   167,     0,   136,     0,   136,     0,   172,     0,   181,
     136,     0,     0,   136,   181,     0,     0,     0,     0,   154,
       0,   154,     0,     0,     0,     0,   154,     0,     0,   154,
     167,     0,     0,     0,     0,     0,   167,     0,     0,     0,
     167,   167,   136,     0,     0,     0,     0,     0,   136,     0,
       0,     0,   136,   136,   172,     0,     0,     0,   154,   167,
     172,     0,     0,   167,   154,   167,     0,     0,   154,   154,
       0,   136,     0,     0,     0,   136,     0,   136,     0,     0,
       0,     0,     0,     0,     0,     0,   167,   154,  1246,     0,
     181,   154,   175,   154,     0,     0,   175,     0,   136,   183,
       0,     0,     0,   175,   181,     0,     0,     0,   172,     0,
       0,     0,     0,   181,   154,     0,     0,     0,     0,     0,
       0,     0,   183,     0,     0,     0,   181,     0,     0,     0,
       0,   167,   167,   167,   167,     0,     0,     0,     0,     0,
       0,     0,     0,   136,   136,   136,   136,     0,     0,   176,
       0,   429,   176,     0,     0,     0,   172,     0,   172,   154,
     154,   154,   154,   172,     0,     0,   172,     0,     0,     0,
       0,     0,   183,   175,     0,   175,   183,   183,   175,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   172,     0,     0,     0,   183,
       0,   172,   175,   183,     0,   172,   172,     0,     0,     0,
     430,     0,   175,     0,   431,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   172,     0,     0,     0,   172,     0,
     172,     0,     0,     0,     0,     0,     0,     0,   181,     0,
     181,     0,     0,     0,     0,   176,     0,     0,     0,   176,
       0,   172,   184,     0,     0,     0,   176,     0,     0,   175,
       0,     0,   433,   434,     0,   175,   436,   437,     0,   438,
     439,     0,     0,     0,   442,   184,     0,     0,     0,     0,
       0,   449,     0,     0,     0,     0,     0,   453,   454,   455,
     361,     0,     0,     0,    22,    23,   172,   172,   172,   172,
       0,     0,     0,   362,   140,    29,   363,     0,     0,     0,
       0,    32,   183,   175,     0,     0,   181,     0,    37,     0,
       0,     0,     0,     0,     0,   184,   176,   140,   176,   184,
     184,   176,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,  1029,   184,     0,  1030,   176,   184,   364,     0,    65,
       0,   175,     0,   175,     0,   176,     0,   183,   175,     0,
       0,   175,   183,     0,     0,     0,     0,   140,    81,     0,
       0,    83,     0,     0,    85,     0,   361,     0,     0,     0,
      22,    23,     0,     0,     0,     0,     0,     0,     0,   362,
     175,    29,   363,     0,   140,     0,   175,    32,   140,    89,
     175,   175,   176,     0,    37,     0,     0,     0,   176,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   175,
       0,     0,   429,   175,   100,   175,     0,     0,     0,     0,
    1031,    54,     0,    56,     0,    58,     0,  1029,   183,     0,
    1030,     0,     0,   364,     0,    65,   175,     0,     0,     0,
       0,     0,   183,     0,     0,   184,   176,     0,     0,     0,
       0,   183,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,   183,     0,     0,   181,     0,     0,
     181,   430,     0,     0,     0,   431,     0,     0,     0,     0,
       0,   175,   175,   175,   175,    89,     0,     0,     0,     0,
       0,     0,     0,     0,   176,     0,   176,   140,     0,     0,
     184,   176,     0,     0,   176,   184,     0,     0,     0,     0,
     100,     0,     0,     0,     0,     0,  1102,     0,     0,     0,
       0,     0,     0,   433,   434,     0,   435,   436,   437,     0,
     438,   439,   440,   176,     0,   442,     0,     0,     0,   176,
       0,   448,   449,   176,   176,   452,     0,     0,   453,   454,
     455,     0,   140,     0,     0,     0,     0,   140,     0,   456,
     226,     0,   176,   181,     0,     0,   176,   181,   176,     0,
       0,     0,     0,     0,   181,     0,   183,     0,   183,     0,
       0,   184,     0,     0,     0,     0,     0,     0,     0,   176,
       0,     0,     0,     0,     0,   184,     0,     0,     0,     0,
       0,     0,     0,     0,   184,     0,     0,     0,     0,     0,
     332,     0,     0,     0,     0,     0,     0,   184,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   140,   176,   176,   176,   176,     0,     0,
       0,   360,     0,     0,   181,     0,   181,   140,   422,   181,
     230,     0,     0,     0,   183,     0,   140,     0,     0,     0,
       0,     0,     0,     0,   231,   232,     0,   233,     0,   140,
       0,     0,   234,   181,     0,     0,     0,     0,     0,     0,
     235,     0,     0,   181,     0,     0,   236,     0,     0,     0,
       0,     0,   237,     0,     0,     0,   238,     0,   361,   239,
       0,   240,    22,    23,     0,     0,     0,     0,     0,     0,
     241,   362,     0,    29,   363,     0,     0,   242,   243,    32,
       0,     0,     0,     0,     0,   244,    37,     0,     0,   184,
     181,   184,     0,     0,     0,   245,   181,     0,     0,     0,
       0,     0,     0,     0,   246,   247,     0,   248,     0,   249,
     485,   250,     0,    54,   251,    56,     0,     0,   252,     0,
       0,   253,     0,   276,   254,   364,     0,    65,     0,     0,
       0,     0,     0,     0,     0,   861,     0,     0,     0,     0,
       0,   140,     0,   140,   181,     0,    81,   276,     0,    83,
       0,     0,    85,     0,     0,     0,  -322,   337,   339,     0,
    -322,  -322,     0,     0,     0,   511,     0,   184,     0,  -322,
     522,  -322,  -322,     0,     0,     0,     0,  -322,     0,     0,
     271,     0,     0,     0,  -322,   183,     0,  -322,   183,   370,
       0,     0,   181,     0,   181,     0,     0,     0,     0,   181,
       0,     0,   181,     0,     0,     0,     0,     0,  -322,     0,
       0,  -322,     0,  -322,     0,  -322,     0,  -322,  -322,   140,
    -322,     0,     0,  -322,     0,  -322,     0,     0,     0,     0,
       0,   181,     0,     0,     0,     0,     0,   181,     0,     0,
       0,   181,   181,     0,  -322,     0,     0,  -322,     0,     0,
    -322,     0,     0,     0,     0,     0,     0,     0,   361,     0,
     181,     0,    22,    23,   181,     0,   181,     0,     0,     0,
       0,   362,     0,    29,   363,  -322,     0,     0,     0,    32,
       0,   183,   593,     0,     0,   183,    37,   181,     0,     0,
       0,     0,   183,     0,     0,     0,     0,     0,     0,   487,
    -322,     0,     0,     0,     0,     0,  -322,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,  1029,
       0,     0,  1030,     0,     0,   364,     0,    65,     0,     0,
       0,     0,   181,   181,   181,   181,     0,     0,   184,   500,
     503,   184,     0,     0,     0,     0,    81,   510,     0,    83,
       0,     0,    85,     0,     0,     0,   276,     0,   276,     0,
       0,     0,   183,     0,   183,   276,     0,   183,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     140,   183,     0,   140,   747,     0,   749,     0,     0,     0,
       0,   183,   100,     0,     0,   276,     0,     0,  1172,     0,
     230,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   231,   232,     0,   233,     0,     0,
       0,     0,   234,     0,   184,     0,     0,     0,   184,     0,
     235,     0,     0,     0,     0,   184,   236,     0,   183,     0,
       0,     0,   237,     0,   183,     0,   238,     0,     0,   239,
       0,   240,     0,     0,     0,     0,     0,     0,   361,     0,
     241,     0,    22,    23,     0,     0,     0,   242,   243,     0,
       0,   362,     0,    29,   363,   244,   140,     0,     0,    32,
     140,     0,     0,     0,     0,   245,    37,   140,     0,     0,
       0,     0,   183,     0,   246,   247,     0,   248,     0,   249,
       0,   250,     0,     0,   251,   184,     0,   184,   252,   512,
     184,   253,     0,    54,   254,    56,     0,   359,     0,  1029,
       0,     0,  1030,     0,     0,   364,     0,    65,     0,     0,
       0,     0,     0,     0,   184,     0,     0,     0,     0,     0,
     183,     0,   183,     0,   184,     0,    81,   183,     0,    83,
     183,     0,    85,     0,     0,     0,     0,   140,     0,   140,
     361,     0,   140,     0,    22,    23,     0,     0,     0,     0,
     513,     0,     0,   362,     0,    29,   363,    89,     0,   183,
       0,    32,     0,     0,     0,   183,   140,     0,    37,   183,
     183,   184,     0,     0,     0,     0,   140,   184,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,   183,     0,
       0,     0,   183,     0,   183,    54,     0,    56,     0,     0,
       0,  1029,     0,     0,  1030,     0,     0,   364,     0,    65,
       0,     0,     0,   926,     0,   183,   929,     0,     0,     0,
       0,     0,     0,   140,     0,   184,     0,     0,    81,   140,
       0,    83,     0,     0,    85,     0,   690,     0,     0,     0,
       0,     0,     0,   276,   276,     0,     0,     0,     0,     0,
     906,   276,   276,   907,     0,     0,     0,     0,   908,    89,
     183,   183,   183,   183,     0,     0,     0,     0,   196,     0,
       0,     0,     0,   184,     0,   184,     0,   140,   909,     0,
     184,     0,     0,   184,   100,   910,     0,   198,     0,     0,
       0,     0,     0,     0,     0,     0,   911,     0,     0,     0,
       0,     0,     0,     0,   912,     0,     0,     0,     0,  1010,
       0,     0,   184,  1015,     0,     0,     0,     0,   184,     0,
     522,   913,   184,   184,     0,   140,     0,   140,     0,     0,
       0,     0,   140,   914,     0,   140,     0,   694,     0,     0,
       0,   184,     0,     0,   915,   184,     0,   184,     0,     0,
     916,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   140,     0,     0,     0,   184,     0,
     140,     0,     0,     0,   140,   140,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1061,     0,  1062,   140,     0,  1064,     0,   140,   210,   140,
     931,     0,   220,     0,     0,     0,     0,     0,     0,     0,
       0,   228,     0,   184,   184,   184,   184,     0,   262,  1079,
     140,     0,     0,     0,     0,     0,     0,   429,     0,  1015,
     278,     0,   281,   282,     0,     0,   960,     0,   287,     0,
     288,     0,   289,     0,     0,     0,   276,   315,     0,     0,
       0,   228,   323,   325,   327,   328,   329,     0,     0,     0,
     333,     0,   334,     0,     0,   140,   140,   140,   140,   340,
       0,     0,   323,     0,     0,   343,  1112,   344,     0,     0,
     345,   346,  1117,   347,   276,     0,   430,   228,   323,   358,
     431,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   276,     0,     0,     0,   276,     0,     0,     0,
       0,     0,   256,     0,     0,     0,   381,     0,   384,     0,
       0,   432,     0,     0,   268,   269,   270,     0,     0,     0,
    1153,     0,     0,     0,     0,     0,     0,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,     0,   446,   447,   448,   449,   450,   426,
     452,     0,     0,   453,   454,   455,     0,     0,     0,     0,
       0,     0,     0,     0,   456,     0,     0,     0,  1179,     0,
    1183,     0,   478,  1069,     0,   522,   333,   228,   522,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,     0,     0,     0,     0,     0,     0,
    1084,     0,     0,     0,   323,     0,     0,  1218,     0,     0,
       0,     0,     0,  1221,     0,     0,     0,  1179,  1183,     0,
       0,     0,     0,   472,   400,     0,  -654,     0,  -654,     0,
       0,     0,     0,     0,     0,     0,  1230,     0,   419,     0,
    1232,   323,   749,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   276,   276,     0,     0,   323,     0,     0,     0,
       0,     0,     0,   479,     0,     0,     0,   329,   333,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   545,   546,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,     0,   749,   749,
     749,   749,     0,     0,     0,     0,     0,     0,     0,   276,
       0,     0,     0,     0,     0,   276,   276,     0,     0,   563,
       0,     0,     0,   497,   498,     0,     0,     0,     0,     0,
       0,     0,     0,   276,   276,   514,   276,     0,     0,     0,
     580,     0,   582,     0,     0,     0,     0,   503,     0,     0,
     276,   592,     0,   276,     0,     0,   596,   601,   604,     0,
       0,     0,     0,   228,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   541,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1222,     0,
       0,     0,   677,   678,   679,   681,   683,   684,     0,   685,
     686,   687,   688,   689,     0,   697,   698,   699,   700,   701,
     702,   703,   704,   705,   706,   707,   708,   709,   710,   711,
     712,   713,     0,     0,     0,     0,   256,     0,   323,   323,
       0,     0,     0,     0,   323,     0,     0,   323,   323,   323,
     323,   723,   725,     0,     0,     0,   601,   729,     0,   738,
       0,     0,     0,     0,     0,   741,     0,   503,     0,   503,
       0,   605,   503,   503,     0,   610,   750,     0,     0,   752,
       0,     0,     0,   323,     0,     0,     0,     0,     0,     0,
       0,     0,   665,   763,     0,   766,     0,     0,     0,   281,
       0,     0,   771,   772,     0,     0,     0,   230,     0,     0,
       0,     0,     0,   315,     0,     0,     0,     0,   315,     0,
       0,   231,   232,     0,   233,     0,     0,   323,     0,   234,
       0,     0,     0,     0,     0,     0,   741,   235,     0,   323,
     323,     0,     0,   236,   228,   798,     0,     0,   323,   237,
       0,     0,     0,   238,     0,     0,   239,   730,   240,   828,
     828,     0,     0,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,   842,   242,   243,     0,     0,     0,     0,
       0,     0,   244,   756,     0,     0,     0,   514,     0,     0,
       0,     0,   245,     0,     0,   315,     0,     0,     0,     0,
     769,   246,   247,     0,   248,     0,   249,     0,   250,   541,
       0,   251,     0,     0,   514,   252,   539,     0,   253,     0,
       0,   254,     0,     0,     0,     0,     0,     0,     0,     0,
     230,     0,     0,     0,     0,     0,     0,     0,     0,   879,
       0,     0,     0,     0,   231,   232,   884,   233,     0,     0,
       0,     0,   234,     0,     0,     0,     0,   815,     0,     0,
     235,     0,     0,     0,     0,     0,   236,     0,   838,     0,
       0,     0,   237,     0,     0,     0,   238,   540,     0,   239,
       0,   240,     0,   610,     0,     0,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,   242,   243,     0,
       0,   750,   323,     0,     0,   244,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   245,     0,   323,     0,   323,
     923,   924,     0,     0,   246,   247,     0,   248,     0,   249,
       0,   250,     0,     0,   251,     0,     0,     0,   252,   323,
       0,   253,     0,     0,   254,     0,     0,   419,     0,     0,
       0,     0,     0,   315,     0,     0,     0,   952,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   580,     0,     0,     0,   990,     0,   992,   828,   828,
       0,   343,     0,   344,     0,     0,   346,   347,   365,     0,
     959,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1004,     0,  1006,     0,     0,     0,   228,     0,     0,     0,
       0,  1013,   741,  1016,     0,  1018,  1020,   932,     0,     0,
    1023,  1025,     0,     0,   514,   323,     0,     0,     0,     0,
    1035,     0,     0,     0,   941,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   961,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1052,  1016,  1023,  1055,   323,     0,     0,   738,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   681,   723,     0,     0,     0,     0,     0,
       0,   230,     0,  1063,     0,     0,     0,   419,     0,     0,
       0,     0,     0,     0,     0,   231,   232,     0,   233,     0,
     315,     0,     0,   234,  1078,     0,     0,     0,     0,     0,
     228,   235,     0,     0,     0,     0,     0,   236,  1086,     0,
    1088,   323,     0,   237,     0,     0,     0,   238,     0,     0,
     239,     0,   240,     0,     0,   665,     0,     0,     0,     0,
       0,   241,   828,     0,     0,     0,     0,     0,   242,   243,
    1099,     0,  1100,     0,     0,     0,   244,     0,     0,     0,
       0,     0,  1108,     0,   738,     0,   245,     0,     0,   429,
       0,     0,     0,     0,     0,   246,   247,  1119,   248,     0,
     249,     0,   250,     0,     0,   251,     0,     0,   756,   252,
       0,     0,   253,     0,     0,   254,  1140,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1147,     0,
       0,     0,     0,   961,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,   323,   430,     0,
       0,     0,   431,     0,     0,     0,     0,     0,  1161,     0,
       0,     0,     0,     0,     0,     0,  1166,   828,   828,   828,
     828,   271,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1180,     0,
    1184,     0,     0,  1186,     0,  1189,     0,     0,  1192,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
     610,     0,   442,   443,   444,     0,   446,   447,   448,   449,
    1180,  1184,   452,  1189,  1192,   453,   454,   455,     0,     0,
       0,     0,     0,     0,     0,     0,   456,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1227,     0,     0,     0,
       0,   738,     0,  1229,     0,     0,     0,  1231,     0,     0,
       0,     0,  1233,  1234,     0,     0,  1235,     0,     0,     0,
       0,     0,     0,     0,  1237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1247,  1248,
    1233,  1249,  1250,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   610,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1261,  1262,
    1263,  1264,  1261,  1262,  1263,  1264,    -2,     4,     0,     5,
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
     100,     0,     0,   101,     0,   102,   559,   103,     0,  -215,
       0,   575,   104,     5,     6,     7,     8,     9,    10,    11,
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
       0,   102,   576,   103,     0,  -215,     0,     4,   104,     5,
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
      98,    99,   100,     0,     0,   101,     0,   102,   805,   103,
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
    -215,  -215,  -215,    32,  1022,    34,   -68,     0,    35,    36,
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
       0,   871,   104,     5,     6,     7,     8,     9,    10,    11,
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
       6,     7,     8,     9,    10,    11,    12,     0,   626,     0,
      13,     0,     0,    16,    17,   628,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,   634,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   201,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,   502,   458,   459,   460,   461,   462,     0,     0,
     465,   466,   467,   468,     0,   470,   471,     0,   816,   817,
     818,   819,   820,   647,     0,   648,     0,    95,     0,   649,
     650,   651,   652,   653,   654,   655,   821,   657,   658,    97,
     822,    99,   -92,   660,   661,   823,   663,   202,     0,   103,
       0,     0,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,   626,     0,    13,     0,     0,    16,
      17,   628,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   198,   634,   199,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   200,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   201,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,   502,   458,
     459,   460,   461,   462,     0,     0,   465,   466,   467,   468,
       0,   470,   471,     0,   816,   817,   818,   819,   820,   647,
       0,   648,     0,    95,     0,   649,   650,   651,   652,   653,
     654,   655,   821,   657,   658,    97,   822,    99,     0,   660,
     661,   823,   663,   202,     0,   103,     0,     0,     0,     0,
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
       0,   731,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,   732,     0,    36,     0,     0,    38,     0,   198,     0,
     199,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,   733,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   201,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
     734,    86,     0,     0,     0,     0,     0,     0,     0,    88,
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
       0,   202,     0,   103,   792,     0,     0,     0,   104,     5,
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
     795,     0,     0,     0,   104,     5,     6,     7,     8,     9,
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
       0,   202,     0,   103,   800,     0,     0,     0,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,   939,    51,    52,     0,     0,
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
       0,   597,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,     0,     0,   598,     0,   104,     5,     6,     7,
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
      84,     0,     0,    86,     0,     0,     0,     0,     0,   597,
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
       0,   101,     0,   202,     0,   103,     0,     0,   598,     0,
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
     790,   103,     0,     0,     0,     0,   104,     5,     6,     7,
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
       0,    78,     0,     0,     0,    82,   991,     0,    84,     0,
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
       0,   202,     0,   103,     0,     0,  1014,     0,   104,     5,
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
       0,     0,  1019,     0,   104,     5,     6,     7,     8,     9,
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
       0,     0,     0,     0,  1053,    34,     0,     0,     0,    36,
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
    1213,    34,     0,     0,     0,    36,     0,     0,    38,     0,
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
       0,    31,     0,     0,     0,     0,  1214,    34,     0,     0,
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
       0,     0,  1216,    34,     0,     0,     0,    36,     0,     0,
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
       0,     0,     0,    31,     0,     0,     0,     0,  1217,    34,
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
       0,     0,     0,     0,     0,   624,     0,   625,     0,    90,
      91,    92,    93,    94,     0,     0,   626,     0,    95,     0,
       0,   627,   232,   628,   629,     0,     0,    96,     0,   630,
      97,    98,    99,     0,     0,     0,   101,   235,   202,   196,
     103,     0,  -215,   236,     0,   104,     0,     0,     0,   631,
       0,     0,     0,   238,     0,     0,   632,     0,   633,   634,
       0,     0,     0,     0,     0,     0,     0,   635,     0,     0,
       0,     0,     0,     0,   242,   636,     0,     0,     0,     0,
       0,     0,   244,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   637,     0,     0,     0,     0,     0,     0,     0,
       0,   246,   247,     0,   638,     0,   249,     0,   639,     0,
       0,   251,     0,     0,     0,   640,     0,     0,   253,     0,
       0,   641,     0,     0,     0,     0,     0,     0,     0,     0,
     502,   458,   459,   460,   461,   462,     0,     0,   465,   466,
     467,   468,     0,   470,   471,     0,   642,   643,   644,   645,
     646,   647,     0,   648,     0,     0,     0,   649,   650,   651,
     652,   653,   654,   655,   656,   657,   658,     0,   659,     0,
       0,   660,   661,   662,   663,     0,     0,   664,     5,     6,
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
      76,     0,   690,    78,     0,     0,     0,    82,     0,     0,
      84,   626,     0,    86,     0,     0,   970,   971,   628,   972,
       0,     0,     0,     0,   973,     0,     0,     0,     0,     0,
       0,     0,   692,     0,     0,     0,     0,     0,    90,     0,
      92,     0,     0,     0,   974,     0,     0,     0,   975,     0,
       0,   976,     0,   198,   634,     0,     0,     0,     0,     0,
       0,    99,   693,     0,     0,     0,     0,     0,     0,   103,
     977,  -215,     0,     0,     0,     0,     0,   978,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   979,     0,     0,
       0,     0,     0,     0,     0,     0,   980,   981,     0,   982,
       0,   983,     0,   694,     0,     0,     0,     0,     0,   695,
     984,     0,     0,   985,     0,     0,   986,     0,     0,     0,
       0,     0,     0,     0,     0,   502,   458,   459,   460,   461,
     462,     0,     0,   465,   466,   467,   468,     0,   470,   471,
       0,   642,   643,   644,   645,   646,   647,     0,   648,     0,
       0,     0,   649,   650,   651,   652,   653,   654,   655,   656,
     657,   658,   230,   659,     0,     0,   660,   661,   662,   663,
     987,   626,     0,     0,     0,     0,   231,   232,   628,   233,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   235,     0,     0,     0,     0,     0,   236,     0,
       0,     0,     0,     0,   237,     0,     0,     0,   238,     0,
       0,   239,     0,   240,   634,     0,     0,     0,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,   242,
     243,     0,     0,     0,     0,     0,     0,   244,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   245,     0,     0,
       0,     0,     0,   427,     0,     0,   246,   247,     0,   248,
     428,   249,     0,   250,     0,     0,   251,     0,     0,     0,
     252,     0,   429,   253,     0,     0,   254,     0,     0,     0,
       0,     0,     0,     0,     0,   502,   458,   459,   460,   461,
     462,     0,     0,   465,   466,   467,   468,     0,   470,   471,
       0,   642,   643,   644,   645,   646,   647,     0,   648,     0,
       0,     0,   649,   650,   651,   652,   653,   654,   655,   656,
     657,   658,     0,   659,     0,     0,   660,   661,   662,   663,
       0,   430,     0,   427,     0,   431,     0,     0,     0,     0,
     428,     0,     0,     0,   501,     0,     0,     0,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   432,   502,   458,   459,
     460,   461,   462,     0,     0,   465,   466,   467,   468,     0,
     470,   471,     0,   433,   434,     0,   435,   436,   437,     0,
     438,   439,   440,     0,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,     0,     0,   453,   454,
     455,   430,   365,   427,     0,   431,     0,     0,     0,   456,
     428,     0,     0,     0,  1187,     0,     0,     0,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   432,   502,   458,   459,
     460,   461,   462,     0,     0,   465,   466,   467,   468,     0,
     470,   471,     0,   433,   434,     0,   435,   436,   437,     0,
     438,   439,   440,     0,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,     0,     0,   453,   454,
     455,   430,   365,   427,     0,   431,     0,     0,     0,   456,
     428,     0,     0,     0,  1253,     0,     0,     0,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   432,   502,   458,   459,
     460,   461,   462,     0,     0,   465,   466,   467,   468,     0,
     470,   471,     0,   433,   434,     0,   435,   436,   437,     0,
     438,   439,   440,     0,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,     0,     0,   453,   454,
     455,   430,   365,   427,     0,   431,     0,     0,     0,   456,
     428,     0,     0,     0,  1254,     0,     0,     0,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   432,   502,   458,   459,
     460,   461,   462,     0,     0,   465,   466,   467,   468,     0,
     470,   471,     0,   433,   434,     0,   435,   436,   437,     0,
     438,   439,   440,     0,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,     0,     0,   453,   454,
     455,   430,   365,   427,     0,   431,     0,     0,     0,   456,
     428,     0,     0,     0,  1255,     0,     0,     0,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   432,   502,   458,   459,
     460,   461,   462,     0,     0,   465,   466,   467,   468,     0,
     470,   471,     0,   433,   434,     0,   435,   436,   437,     0,
     438,   439,   440,     0,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,     0,     0,   453,   454,
     455,   430,   365,     0,     0,   431,     0,     0,   427,   456,
       0,     0,     0,     0,  1256,   428,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,   855,     0,
       0,     0,     0,     0,     0,     0,   432,   502,   458,   459,
     460,   461,   462,   856,     0,   465,   466,   467,   468,     0,
     470,   471,     0,   433,   434,     0,   435,   436,   437,     0,
     438,   439,   440,     0,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,     0,     0,   453,   454,
     455,     0,   365,     0,   427,     0,   430,     0,     0,   456,
     431,   428,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   429,   275,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   843,     0,   965,
       0,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,     0,   430,   453,   454,   455,   431,   365,     0,     0,
       0,     0,     0,     0,   456,     0,   427,     0,     0,     0,
       0,     0,     0,   428,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   843,     0,   429,     0,   432,     0,     0,
     584,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   585,     0,   453,
     454,   455,     0,   365,     0,     0,     0,     0,     0,     0,
     456,     0,     0,     0,   430,     0,     0,     0,   431,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   427,   812,
       0,     0,     0,     0,     0,   428,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,   432,
       0,     0,   813,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   814,
     586,   453,   454,   455,     0,     0,     0,     0,     0,     0,
       0,     0,   456,     0,   427,     0,   430,     0,     0,     0,
     431,   428,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   429,  1123,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,     0,   430,   453,   454,   455,   431,     0,     0,   427,
       0,     0,     0,     0,   456,     0,   428,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   429,   275,
       0,     0,     0,   843,     0,     0,     0,   432,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,     0,   365,     0,   427,     0,   430,     0,     0,
     456,   431,   428,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   429,   275,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   843,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,   430,   453,   454,   455,   431,   365,     0,
     427,     0,     0,     0,     0,   456,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,   835,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,     0,     0,
     453,   454,   455,     0,   365,     0,   427,     0,   430,     0,
       0,   456,   431,   428,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   429,   899,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   214,
       0,   900,     0,   432,     0,     0,     0,   427,     0,     0,
       0,     0,     0,     0,   428,     0,     0,     0,     0,     0,
     433,   434,     0,   435,   436,   437,   429,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,   430,   453,   454,   455,   431,     0,
       0,     0,   427,     0,     0,     0,   456,     0,     0,   428,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,   432,
       0,     0,     0,     0,     0,   430,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,   433,   434,   930,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
     432,   453,   454,   455,     0,     0,     0,     0,     0,     0,
     430,     0,   456,     0,   431,     0,     0,   433,   434,     0,
     435,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     427,   843,   453,   454,   455,   432,   365,   428,     0,     0,
       0,     0,     0,   456,     0,     0,     0,     0,     0,   429,
       0,     0,   433,   434,  1094,   435,   436,   437,     0,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   427,     0,   453,   454,   455,
       0,     0,   428,     0,     0,     0,  1115,     0,   456,     0,
       0,  1095,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,   431,     0,     0,   427,   526,     0,     0,     0,
       0,     0,   428,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,   431,     0,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,   843,   453,   454,   455,   432,     0,
       0,     0,     0,   430,     0,     0,   456,   431,     0,     0,
       0,     0,     0,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   432,     0,
     453,   454,   455,     0,     0,     0,   427,     0,     0,  1163,
       0,   456,     0,   428,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,   429,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,     0,     0,
     453,   454,   455,     0,     0,     0,   427,     0,     0,     0,
       0,   456,     0,   428,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,   431,     0,
       0,   427,     0,     0,     0,     0,     0,     0,   428,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,   432,
       0,     0,     0,     0,   430,     0,     0,     0,   431,     0,
       0,     0,     0,     0,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   432,
       0,   453,   454,   455,     0,   550,     0,   427,     0,   430,
       0,     0,   456,   431,   428,     0,   433,   434,     0,   435,
     436,   437,   726,   438,   439,   440,   429,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
     554,   453,   454,   455,   432,     0,     0,     0,   427,   808,
       0,     0,   456,     0,     0,   428,     0,     0,     0,     0,
       0,   433,   434,     0,   435,   436,   437,   429,   438,   439,
     440,     0,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,     0,   430,   453,   454,   455,   431,
       0,     0,     0,     0,   427,     0,     0,   456,     0,     0,
       0,   428,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
     432,     0,     0,     0,     0,     0,   430,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,   433,   434,     0,
     435,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
       0,   432,   453,   454,   455,     0,     0,     0,     0,   791,
       0,     0,   430,   456,     0,     0,   431,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   809,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   427,     0,   453,   454,   455,   850,   432,   428,     0,
       0,     0,     0,     0,   456,     0,     0,     0,     0,     0,
     429,     0,     0,   928,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   427,     0,   453,
     454,   455,     0,     0,   428,     0,     0,     0,     0,     0,
     456,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,   431,     0,     0,     0,   427,     0,     0,
       0,     0,     0,     0,   428,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   430,     0,     0,     0,   431,
       0,   433,   434,     0,   435,   436,   437,     0,   438,   439,
     440,     0,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,     0,     0,   453,   454,   455,     0,
     432,     0,     0,     0,     0,   430,     0,   456,     0,   431,
       0,     0,     0,     0,     0,     0,     0,   433,   434,     0,
     435,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     432,     0,   453,   454,   455,     0,     0,     0,     0,   942,
       0,     0,     0,   456,     0,     0,     0,   433,   434,     0,
     435,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     427,   997,   453,   454,   455,     0,     0,   428,     0,   943,
       0,     0,     0,   456,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     427,     0,     0,     0,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,     0,
     427,  1091,   431,     0,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,   430,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   432,     0,   453,   454,   455,   430,     0,
       0,   427,   431,     0,     0,     0,   456,     0,   428,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
     429,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   432,     0,   453,   454,   455,     0,     0,
       0,   427,  1045,     0,     0,     0,   456,     0,   428,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
     429,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,     0,   453,   454,   455,     0,   430,
       0,     0,     0,   431,     0,     0,   456,   427,     0,     0,
       0,     0,     0,     0,   428,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
     214,     0,     0,     0,   432,     0,     0,     0,     0,   430,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
       0,   433,   434,     0,   435,   436,   437,     0,   438,   439,
     440,     0,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   432,     0,   453,   454,   455,     0,
       0,     0,     0,     0,     0,   430,     0,   456,     0,   431,
       0,   433,   434,     0,   435,   436,   437,     0,   438,   439,
     440,     0,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   427,  1107,   453,   454,   455,  1118,
     432,   428,     0,     0,     0,     0,     0,   456,     0,     0,
       0,     0,     0,   429,     0,     0,     0,   433,   434,     0,
     435,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     427,     0,   453,   454,   455,     0,     0,   428,     0,     0,
       0,     0,     0,   456,     0,     0,     0,     0,     0,   429,
    1149,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,   431,     0,     0,     0,
       0,   427,  1171,     0,     0,     0,     0,     0,   428,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,   432,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,   431,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,     0,   432,     0,     0,     0,     0,  1146,   430,
     456,   427,     0,   431,     0,     0,     0,     0,   428,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
     429,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,   432,   453,   454,   455,     0,     0,
       0,     0,   427,     0,     0,     0,   456,     0,     0,   428,
    1207,   433,   434,     0,   435,   436,   437,     0,   438,   439,
     440,   429,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,     0,     0,   453,   454,   455,   430,
       0,     0,     0,   431,     0,     0,     0,   456,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   432,     0,     0,     0,   427,     0,
     430,     0,     0,     0,   431,   428,     0,     0,     0,     0,
       0,   433,   434,  1215,   435,   436,   437,   429,   438,   439,
     440,     0,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,     0,   432,   453,   454,   455,   427,
       0,     0,     0,     0,     0,     0,   428,   456,     0,     0,
       0,     0,   433,   434,     0,   435,   436,   437,   429,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,     0,   430,   453,   454,   455,
     431,     0,     0,     0,   427,     0,     0,     0,   456,  1257,
       0,   428,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
       0,   432,     0,     0,     0,     0,     0,   430,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,   433,   434,
    1258,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,     0,   432,   453,   454,   455,     0,     0,     0,     0,
     427,     0,   430,     0,   456,     0,   431,   428,     0,   433,
     434,     0,   435,   436,   437,  1259,   438,   439,   440,   429,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,     0,   453,   454,   455,   432,     0,     0,
       0,   427,     0,     0,     0,   456,     0,     0,   428,     0,
       0,     0,     0,     0,   433,   434,     0,   435,   436,   437,
     429,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,   430,   453,
     454,   455,   431,     0,     0,     0,   427,     0,     0,     0,
     456,  1260,     0,   428,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,   430,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,   432,   453,   454,   455,     0,     0,
       0,     0,     0,     0,   918,     0,   456,     0,   431,     0,
       0,   433,   434,     0,   435,   436,   437,     0,   438,   439,
     440,     0,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,     0,     0,   453,   454,   455,   432,
       0,     0,     0,     0,     0,     0,     0,   456,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,   429,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
     361,   453,   454,   455,    22,    23,     0,     0,     0,     0,
       0,     0,   456,   362,     0,    29,   363,     0,     0,     0,
       0,    32,     0,   429,     0,     0,     0,     0,    37,     0,
       0,  -342,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   -68,   430,     0,     0,     0,   431,
       0,     0,  -342,     0,     0,    54,     0,    56,     0,     0,
       0,    60,  -342,   -68,    61,     0,     0,   364,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     432,     0,   430,     0,     0,     0,   431,     0,    81,     0,
       0,    83,     0,     0,    85,     0,     0,   433,   434,     0,
     435,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,   444,     0,   446,   447,   448,   449,   450,   451,   452,
       0,     0,   453,   454,   455,     0,     0,     0,     0,     0,
       0,     0,     0,   456,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,   100,   441,   442,   443,   444,   365,
     446,   447,   448,   449,   450,   608,   452,   230,     0,   453,
     454,   455,     0,     0,     0,     0,     0,     0,     0,     0,
     456,   231,   232,     0,   233,     0,     0,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,   235,     0,     0,
       0,     0,     0,   236,     0,     0,     0,     0,     0,   237,
       0,     0,     0,   238,     0,     0,   239,     0,   240,     0,
       0,     0,     0,   230,     0,     0,     0,   241,     0,     0,
       0,     0,     0,     0,   242,   243,     0,   231,   232,     0,
     233,     0,   244,     0,     0,   234,     0,     0,     0,     0,
       0,     0,   245,   235,     0,     0,     0,     0,     0,   236,
       0,   246,   247,     0,   248,   237,   249,     0,   250,   238,
       0,   251,   239,     0,   240,   252,     0,     0,   253,   230,
       0,   254,     0,   241,     0,     0,     0,     0,     0,     0,
     242,   243,     0,   231,   232,     0,   233,     0,   244,     0,
       0,   234,     0,    23,     0,    89,     0,     0,   245,   235,
       0,     0,     0,     0,     0,   236,     0,   246,   247,     0,
     248,   237,   249,     0,   250,   238,     0,   251,   239,     0,
     240,   252,     0,     0,   253,   230,     0,   254,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,     0,   231,
     232,     0,   233,     0,   244,    56,     0,   234,     0,     0,
       0,    89,     0,     0,   245,   235,     0,    65,     0,     0,
       0,   236,     0,   246,   247,     0,   248,   237,   249,     0,
     250,   238,     0,   251,   239,     0,   240,   252,     0,     0,
     253,  -317,    85,   254,     0,   241,     0,     0,     0,     0,
       0,     0,   242,   243,     0,  -317,  -317,     0,  -317,     0,
     244,     0,     0,  -317,     0,     0,     0,     0,     0,     0,
     245,  -317,     0,     0,     0,     0,     0,  -317,     0,   246,
     247,     0,   248,  -317,   249,     0,   250,  -317,     0,   251,
    -317,     0,  -317,   252,   539,     0,   253,   230,     0,   254,
       0,  -317,     0,     0,     0,     0,     0,     0,  -317,  -317,
       0,   231,   232,     0,   233,     0,  -317,     0,     0,   234,
       0,     0,     0,     0,     0,     0,  -317,   235,     0,     0,
       0,     0,     0,   236,     0,  -317,  -317,     0,  -317,   237,
    -317,     0,  -317,   238,     0,  -317,   239,     0,   240,  -317,
       0,     0,  -317,  -318,     0,  -317,     0,   241,     0,     0,
       0,     0,     0,     0,   242,   243,     0,  -318,  -318,     0,
    -318,     0,   244,     0,     0,  -318,     0,     0,     0,     0,
       0,     0,   245,  -318,     0,     0,     0,     0,     0,  -318,
       0,   246,   247,     0,   248,  -318,   249,     0,   250,  -318,
       0,   251,  -318,     0,  -318,   252,     0,     0,   253,     0,
       0,   254,     0,  -318,     0,     0,     0,     0,     0,     0,
    -318,  -318,     0,     0,     0,     0,     0,     0,  -318,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -318,     0,
       0,     0,     0,     0,     0,     0,     0,  -318,  -318,     0,
    -318,     0,  -318,     0,  -318,     0,     0,  -318,     0,     0,
       0,  -318,     0,     0,  -318,     0,     0,  -318
};

static const yytype_int16 yycheck[] =
{
       2,   266,     2,   181,    20,    62,   401,   165,   484,   112,
     418,   548,     2,   508,   223,    62,   584,    26,    62,   609,
     552,   620,    45,    25,   617,   667,   291,   669,     2,   780,
     814,   904,   862,   133,    67,    25,   809,   543,     1,   832,
     403,   573,   383,   774,     1,   944,    69,    32,     1,    35,
     203,    25,     1,    32,    87,    67,    32,    66,    56,     3,
     518,    32,    32,    56,     3,   121,   331,    22,    32,   103,
     103,   121,   121,    75,    75,    87,   599,    79,    80,    32,
     603,   604,     3,   870,   262,    75,    40,    41,    42,    79,
      80,   103,   119,    53,    61,    32,   124,   166,    47,   166,
     102,    75,     0,   172,   106,    79,    80,    47,    61,   148,
      54,    47,   102,    47,    74,    54,   106,   118,    75,    36,
     176,   148,     3,    24,    84,   111,   176,   176,   102,    47,
      24,    32,   106,    54,    89,   174,  1009,   171,    32,    40,
      41,    42,   229,   171,   393,     1,    40,    41,    42,    93,
     121,   121,   119,   106,    56,    91,    73,    91,   113,   946,
     104,   118,   620,   126,   148,     2,   119,    56,   761,   148,
    1000,   172,   148,    54,   537,   173,    32,   126,  1077,   266,
     173,   430,   431,   104,   147,   148,   171,   938,    25,   121,
     174,    18,   171,   944,   147,   171,   229,    31,   147,   148,
     171,   171,  1032,  1033,   291,    61,   999,   171,   148,   474,
     475,   174,   744,   215,   367,   172,   947,   166,   171,   120,
      31,   175,   995,   104,   148,   215,   120,   476,   477,   166,
     148,    24,   727,   266,   171,   513,   176,   148,    75,    32,
     418,   215,    79,    80,   331,   813,   587,    40,    41,    42,
     106,    78,   176,   148,    27,   278,    90,   280,   291,    56,
      94,   172,   540,   119,   287,   102,    47,    47,   270,   106,
      97,  1101,   778,   275,   175,   798,   148,   172,    56,    90,
     270,   175,   382,    94,   142,   275,   148,   114,   141,   110,
      27,   147,   150,   166,   173,   148,   270,  1127,   331,   148,
     173,   275,   174,  1176,   327,    46,    79,  1058,   142,  1093,
     166,  1095,   174,   147,    91,   171,   150,   526,    91,   172,
     154,   570,   148,   172,   917,    66,  1077,   120,    11,   148,
     488,   142,   143,   148,   145,   146,   147,   110,   149,   150,
     121,   121,    79,   154,    85,   402,   945,   166,   174,   351,
     161,   351,   439,    46,    91,    46,   167,   168,   169,   174,
     148,   351,    27,   365,   148,   860,    83,   148,   148,    86,
       2,    56,   374,    66,   115,   365,   126,   351,   215,   116,
     148,   590,   175,   392,   374,   387,   174,   474,   475,   398,
     174,   365,    85,    25,    85,   176,   176,   387,   166,   148,
     374,   148,   152,   148,  1046,  1047,   901,   173,   166,   175,
     173,    76,   175,   387,    79,   173,   418,   175,   148,   987,
     173,   508,   115,   148,   115,   174,    91,   174,   904,   174,
     463,   464,    50,   270,   966,    53,   469,   148,   275,   472,
     473,   474,   475,    75,   174,   110,  1036,    79,    80,   174,
     126,   463,   464,   148,    79,    80,    74,   469,   148,    77,
     472,   473,   166,   174,    56,   552,    84,   862,   172,   148,
     102,   166,   481,   881,   106,   508,    56,   153,   148,   155,
     156,   538,   158,   159,   174,   148,   573,    56,     3,  1012,
    1013,   538,   110,   104,   538,   174,   166,   499,   148,   501,
      56,  1024,  1025,   166,   148,    20,   609,    79,    80,   499,
      56,   501,   148,   173,   351,   175,   166,   148,    33,   552,
     148,     8,   166,     2,  1092,   499,  1094,   501,   365,   148,
     166,   564,   565,  1009,   173,   166,   148,   374,   166,    54,
     573,  1131,   171,   173,   173,   175,    25,   166,    63,   148,
     387,    56,   564,   565,   166,    31,   173,   173,   175,   175,
     569,    27,  1085,  1086,  1087,  1088,   142,   166,   292,   293,
      27,   147,   595,   596,   150,   577,   592,   577,   154,   101,
     603,    47,   173,   215,    90,    51,   835,   577,    94,   104,
      47,   173,     3,   175,    51,   110,    75,   846,    90,   141,
      79,    80,    94,   577,  1141,  1000,    56,   856,    90,    56,
      76,    56,    94,    79,    90,    81,    90,    90,    94,    76,
      94,    94,    79,   102,    81,    91,    90,   106,    56,    56,
      94,   150,    27,   171,    91,   564,   565,  1032,   270,    56,
     727,   171,  1037,   275,   110,   147,   173,   165,   897,   166,
     166,   900,    47,   110,   166,   166,    51,   744,   166,   166,
    1197,   171,   499,   166,   501,    31,   142,   143,    56,   918,
     919,   147,   173,   149,   150,   166,   834,   166,   154,    85,
     166,    76,   147,   173,    79,   161,    81,   148,   173,   173,
     173,   167,   168,   169,   727,   166,    91,   174,   166,   148,
    1176,     8,   174,   881,     3,    66,  1101,    31,   174,   742,
     173,   744,    79,   171,   963,   110,   965,   174,    91,   351,
      91,   808,   809,   126,    90,   166,   150,    69,    94,   173,
     742,   764,   126,   365,   174,   737,   215,   126,   148,   126,
     577,    34,   374,    34,    66,    27,   748,   126,   174,   751,
     147,   174,   764,   174,    21,   387,   171,   120,   748,   173,
     172,   751,   148,   106,   126,    47,    90,   150,   173,    51,
      94,   173,   150,   860,   748,   150,   142,   751,   150,    34,
     150,   147,   150,   149,   150,   150,   150,   142,   154,   166,
     150,   270,   150,   150,    76,   161,   275,    79,   150,    81,
     150,   167,   168,   169,   150,   862,  1071,  1072,    34,    91,
     150,   868,   166,   836,   901,   166,   147,   166,   142,   143,
     877,   145,   146,   147,   172,   149,   150,   860,   110,   166,
     154,   854,   841,  1135,   166,   858,   160,   161,   174,   148,
     164,   176,   844,   167,   168,   169,   848,   174,   166,    47,
     171,   176,   172,   855,   844,    34,   173,   176,   848,    31,
     174,     2,   166,   176,   166,   855,   147,   499,   901,   501,
     844,   166,   351,   173,   848,   173,   148,   174,    32,   966,
     176,   855,    34,   166,    25,   174,   365,   166,   176,   176,
     174,   166,   165,   111,   174,   374,   165,   148,   572,  1201,
    1202,  1203,  1204,  1205,  1206,   966,   265,   374,   387,   806,
      24,   748,   367,   391,   751,   934,   918,   919,    90,   339,
     524,   959,    94,   925,   538,   927,   385,   877,   930,   881,
    1033,   305,   535,   966,    75,   925,  1197,   927,    79,    80,
     930,  1119,   868,  1000,  1039,   577,   955,  1207,  1200,   833,
     673,   925,   954,   927,   131,   521,   930,   760,    -1,    -1,
      -1,   102,   964,    -1,   954,   106,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,   964,   147,    -1,   149,   150,    -1,
     954,    -1,   154,    -1,  1071,  1072,    -1,  1003,  1004,   161,
     964,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,
      -1,  1024,  1025,    -1,    -1,    -1,    -1,   844,    -1,  1011,
      -1,   848,    -1,    -1,    -1,  1017,    -1,    -1,   855,    -1,
     499,  1011,   501,    -1,  1127,    -1,    -1,  1017,  1131,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1011,  1071,  1072,
      -1,    -1,    -1,  1017,  1101,    -1,    -1,    -1,    -1,    -1,
      -1,  1229,    -1,  1231,    -1,    -1,  1234,  1235,  1081,    -1,
      -1,    -1,    -1,  1065,  1087,  1088,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   215,  1065,    -1,    -1,    -1,    -1,
      -1,    -1,  1105,  1106,    -1,  1108,    -1,    -1,   925,    -1,
     927,  1065,    -1,   930,    -1,    -1,    -1,    -1,   577,  1122,
      -1,    -1,  1125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1113,    -1,  1115,    -1,    -1,   748,   954,  1120,   751,
      -1,  1123,    -1,  1113,    -1,  1115,    -1,   964,    -1,   270,
    1120,    -1,    -1,  1123,   275,    -1,    -1,    -1,    -1,  1113,
      -1,  1115,    -1,    -1,    -1,    -1,  1120,    -1,    -1,  1123,
    1152,    -1,    -1,    -1,    -1,    -1,  1158,    -1,    -1,    -1,
    1162,  1163,  1152,    -1,    -1,    -1,    -1,    -1,  1158,    -1,
      -1,    -1,  1162,  1163,  1011,    -1,    -1,    -1,  1152,  1181,
    1017,    -1,    -1,  1185,  1158,  1187,    -1,    -1,  1162,  1163,
      -1,  1181,    -1,    -1,    -1,  1185,    -1,  1187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1208,  1181,  1208,    -1,
     351,  1185,   844,  1187,    -1,    -1,   848,    -1,  1208,     2,
      -1,    -1,    -1,   855,   365,    -1,    -1,    -1,  1065,    -1,
      -1,    -1,    -1,   374,  1208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,   387,    -1,    -1,    -1,
      -1,  1253,  1254,  1255,  1256,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1253,  1254,  1255,  1256,    -1,    -1,   748,
      -1,    31,   751,    -1,    -1,    -1,  1113,    -1,  1115,  1253,
    1254,  1255,  1256,  1120,    -1,    -1,  1123,    -1,    -1,    -1,
      -1,    -1,    75,   925,    -1,   927,    79,    80,   930,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1152,    -1,    -1,    -1,   102,
      -1,  1158,   954,   106,    -1,  1162,  1163,    -1,    -1,    -1,
      90,    -1,   964,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1181,    -1,    -1,    -1,  1185,    -1,
    1187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   499,    -1,
     501,    -1,    -1,    -1,    -1,   844,    -1,    -1,    -1,   848,
      -1,  1208,     2,    -1,    -1,    -1,   855,    -1,    -1,  1011,
      -1,    -1,   142,   143,    -1,  1017,   146,   147,    -1,   149,
     150,    -1,    -1,    -1,   154,    25,    -1,    -1,    -1,    -1,
      -1,   161,    -1,    -1,    -1,    -1,    -1,   167,   168,   169,
      22,    -1,    -1,    -1,    26,    27,  1253,  1254,  1255,  1256,
      -1,    -1,    -1,    35,     2,    37,    38,    -1,    -1,    -1,
      -1,    43,   215,  1065,    -1,    -1,   577,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    75,   925,    25,   927,    79,
      80,   930,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    83,   102,    -1,    86,   954,   106,    89,    -1,    91,
      -1,  1113,    -1,  1115,    -1,   964,    -1,   270,  1120,    -1,
      -1,  1123,   275,    -1,    -1,    -1,    -1,    75,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    22,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
    1152,    37,    38,    -1,   102,    -1,  1158,    43,   106,   141,
    1162,  1163,  1011,    -1,    50,    -1,    -1,    -1,  1017,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1181,
      -1,    -1,    31,  1185,   166,  1187,    -1,    -1,    -1,    -1,
     172,    77,    -1,    79,    -1,    81,    -1,    83,   351,    -1,
      86,    -1,    -1,    89,    -1,    91,  1208,    -1,    -1,    -1,
      -1,    -1,   365,    -1,    -1,   215,  1065,    -1,    -1,    -1,
      -1,   374,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,   387,    -1,    -1,   748,    -1,    -1,
     751,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,  1253,  1254,  1255,  1256,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1113,    -1,  1115,   215,    -1,    -1,
     270,  1120,    -1,    -1,  1123,   275,    -1,    -1,    -1,    -1,
     166,    -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,  1152,    -1,   154,    -1,    -1,    -1,  1158,
      -1,   160,   161,  1162,  1163,   164,    -1,    -1,   167,   168,
     169,    -1,   270,    -1,    -1,    -1,    -1,   275,    -1,   178,
      25,    -1,  1181,   844,    -1,    -1,  1185,   848,  1187,    -1,
      -1,    -1,    -1,    -1,   855,    -1,   499,    -1,   501,    -1,
      -1,   351,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1208,
      -1,    -1,    -1,    -1,    -1,   365,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   374,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,   387,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   351,  1253,  1254,  1255,  1256,    -1,    -1,
      -1,   106,    -1,    -1,   925,    -1,   927,   365,     1,   930,
       3,    -1,    -1,    -1,   577,    -1,   374,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,   387,
      -1,    -1,    25,   954,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,   964,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    22,    52,
      -1,    54,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    35,    -1,    37,    38,    -1,    -1,    70,    71,    43,
      -1,    -1,    -1,    -1,    -1,    78,    50,    -1,    -1,   499,
    1011,   501,    -1,    -1,    -1,    88,  1017,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    -1,   100,    -1,   102,
     215,   104,    -1,    77,   107,    79,    -1,    -1,   111,    -1,
      -1,   114,    -1,    45,   117,    89,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,    -1,    -1,    -1,
      -1,   499,    -1,   501,  1065,    -1,   110,    69,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    22,    79,    80,    -1,
      26,    27,    -1,    -1,    -1,   270,    -1,   577,    -1,    35,
     275,    37,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
     173,    -1,    -1,    -1,    50,   748,    -1,    53,   751,   111,
      -1,    -1,  1113,    -1,  1115,    -1,    -1,    -1,    -1,  1120,
      -1,    -1,  1123,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    83,    84,   577,
      86,    -1,    -1,    89,    -1,    91,    -1,    -1,    -1,    -1,
      -1,  1152,    -1,    -1,    -1,    -1,    -1,  1158,    -1,    -1,
      -1,  1162,  1163,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
    1181,    -1,    26,    27,  1185,    -1,  1187,    -1,    -1,    -1,
      -1,    35,    -1,    37,    38,   141,    -1,    -1,    -1,    43,
      -1,   844,   387,    -1,    -1,   848,    50,  1208,    -1,    -1,
      -1,    -1,   855,    -1,    -1,    -1,    -1,    -1,    -1,   221,
     166,    -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    83,
      -1,    -1,    86,    -1,    -1,    89,    -1,    91,    -1,    -1,
      -1,    -1,  1253,  1254,  1255,  1256,    -1,    -1,   748,   261,
     262,   751,    -1,    -1,    -1,    -1,   110,   269,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,   278,    -1,   280,    -1,
      -1,    -1,   925,    -1,   927,   287,    -1,   930,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     748,   954,    -1,   751,   499,    -1,   501,    -1,    -1,    -1,
      -1,   964,   166,    -1,    -1,   327,    -1,    -1,   172,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,   844,    -1,    -1,    -1,   848,    -1,
      33,    -1,    -1,    -1,    -1,   855,    39,    -1,  1011,    -1,
      -1,    -1,    45,    -1,  1017,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      63,    -1,    26,    27,    -1,    -1,    -1,    70,    71,    -1,
      -1,    35,    -1,    37,    38,    78,   844,    -1,    -1,    43,
     848,    -1,    -1,    -1,    -1,    88,    50,   855,    -1,    -1,
      -1,    -1,  1065,    -1,    97,    98,    -1,   100,    -1,   102,
      -1,   104,    -1,    -1,   107,   925,    -1,   927,   111,   112,
     930,   114,    -1,    77,   117,    79,    -1,    81,    -1,    83,
      -1,    -1,    86,    -1,    -1,    89,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,   954,    -1,    -1,    -1,    -1,    -1,
    1113,    -1,  1115,    -1,   964,    -1,   110,  1120,    -1,   113,
    1123,    -1,   116,    -1,    -1,    -1,    -1,   925,    -1,   927,
      22,    -1,   930,    -1,    26,    27,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    35,    -1,    37,    38,   141,    -1,  1152,
      -1,    43,    -1,    -1,    -1,  1158,   954,    -1,    50,  1162,
    1163,  1011,    -1,    -1,    -1,    -1,   964,  1017,    -1,    -1,
      -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,  1181,    -1,
      -1,    -1,  1185,    -1,  1187,    77,    -1,    79,    -1,    -1,
      -1,    83,    -1,    -1,    86,    -1,    -1,    89,    -1,    91,
      -1,    -1,    -1,   748,    -1,  1208,   751,    -1,    -1,    -1,
      -1,    -1,    -1,  1011,    -1,  1065,    -1,    -1,   110,  1017,
      -1,   113,    -1,    -1,   116,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,   595,   596,    -1,    -1,    -1,    -1,    -1,
      17,   603,   604,    20,    -1,    -1,    -1,    -1,    25,   141,
    1253,  1254,  1255,  1256,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,  1113,    -1,  1115,    -1,  1065,    45,    -1,
    1120,    -1,    -1,  1123,   166,    52,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,   844,
      -1,    -1,  1152,   848,    -1,    -1,    -1,    -1,  1158,    -1,
     855,    88,  1162,  1163,    -1,  1113,    -1,  1115,    -1,    -1,
      -1,    -1,  1120,   100,    -1,  1123,    -1,   104,    -1,    -1,
      -1,  1181,    -1,    -1,   111,  1185,    -1,  1187,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1152,    -1,    -1,    -1,  1208,    -1,
    1158,    -1,    -1,    -1,  1162,  1163,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     925,    -1,   927,  1181,    -1,   930,    -1,  1185,    13,  1187,
     752,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,  1253,  1254,  1255,  1256,    -1,    33,   954,
    1208,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,   964,
      45,    -1,    47,    48,    -1,    -1,   788,    -1,    53,    -1,
      55,    -1,    57,    -1,    -1,    -1,   798,    62,    -1,    -1,
      -1,    66,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      75,    -1,    77,    -1,    -1,  1253,  1254,  1255,  1256,    84,
      -1,    -1,    87,    -1,    -1,    90,  1011,    92,    -1,    -1,
      95,    96,  1017,    98,   836,    -1,    90,   102,   103,   104,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   854,    -1,    -1,    -1,   858,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,   131,    -1,   133,    -1,
      -1,   125,    -1,    -1,    40,    41,    42,    -1,    -1,    -1,
    1065,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,    -1,   158,   159,   160,   161,   162,   174,
     164,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,  1113,    -1,
    1115,    -1,   197,   935,    -1,  1120,   201,   202,  1123,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,    -1,    -1,    -1,    -1,    -1,    -1,
     962,    -1,    -1,    -1,   229,    -1,    -1,  1152,    -1,    -1,
      -1,    -1,    -1,  1158,    -1,    -1,    -1,  1162,  1163,    -1,
      -1,    -1,    -1,   170,   150,    -1,   173,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1181,    -1,   164,    -1,
    1185,   266,  1187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1024,  1025,    -1,    -1,   291,    -1,    -1,    -1,
      -1,    -1,    -1,   199,    -1,    -1,    -1,   302,   303,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   317,   318,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   331,    -1,  1253,  1254,
    1255,  1256,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1081,
      -1,    -1,    -1,    -1,    -1,  1087,  1088,    -1,    -1,   354,
      -1,    -1,    -1,   259,   260,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1105,  1106,   271,  1108,    -1,    -1,    -1,
     375,    -1,   377,    -1,    -1,    -1,    -1,  1119,    -1,    -1,
    1122,   386,    -1,  1125,    -1,    -1,   391,   392,   393,    -1,
      -1,    -1,    -1,   398,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   311,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1160,    -1,
      -1,    -1,   427,   428,   429,   430,   431,   432,    -1,   434,
     435,   436,   437,   438,    -1,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,    -1,    -1,    -1,    -1,   362,    -1,   463,   464,
      -1,    -1,    -1,    -1,   469,    -1,    -1,   472,   473,   474,
     475,   476,   477,    -1,    -1,    -1,   481,   482,    -1,   484,
      -1,    -1,    -1,    -1,    -1,   490,    -1,  1229,    -1,  1231,
      -1,   397,  1234,  1235,    -1,   401,   501,    -1,    -1,   504,
      -1,    -1,    -1,   508,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   418,   518,    -1,   520,    -1,    -1,    -1,   524,
      -1,    -1,   527,   528,    -1,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,   538,    -1,    -1,    -1,    -1,   543,    -1,
      -1,    17,    18,    -1,    20,    -1,    -1,   552,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,   561,    33,    -1,   564,
     565,    -1,    -1,    39,   569,   570,    -1,    -1,   573,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,   483,    54,   584,
     585,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,   598,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,   509,    -1,    -1,    -1,   513,    -1,    -1,
      -1,    -1,    88,    -1,    -1,   620,    -1,    -1,    -1,    -1,
     526,    97,    98,    -1,   100,    -1,   102,    -1,   104,   535,
      -1,   107,    -1,    -1,   540,   111,   112,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   664,
      -1,    -1,    -1,    -1,    17,    18,   671,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,   583,    -1,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    39,    -1,   594,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    49,   173,    -1,    52,
      -1,    54,    -1,   609,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,   726,   727,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,   742,    -1,   744,
     745,   746,    -1,    -1,    97,    98,    -1,   100,    -1,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,   764,
      -1,   114,    -1,    -1,   117,    -1,    -1,   673,    -1,    -1,
      -1,    -1,    -1,   778,    -1,    -1,    -1,   782,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   806,    -1,    -1,    -1,   810,    -1,   812,   813,   814,
      -1,   816,    -1,   818,    -1,    -1,   821,   822,   171,    -1,
     173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     835,    -1,   837,    -1,    -1,    -1,   841,    -1,    -1,    -1,
      -1,   846,   847,   848,    -1,   850,   851,   753,    -1,    -1,
     855,   856,    -1,    -1,   760,   860,    -1,    -1,    -1,    -1,
     865,    -1,    -1,    -1,   770,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   788,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   897,   898,   899,   900,   901,    -1,    -1,   904,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   918,   919,    -1,    -1,    -1,    -1,    -1,
      -1,     3,    -1,   928,    -1,    -1,    -1,   833,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,
     945,    -1,    -1,    25,   949,    -1,    -1,    -1,    -1,    -1,
     955,    33,    -1,    -1,    -1,    -1,    -1,    39,   963,    -1,
     965,   966,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    -1,   881,    -1,    -1,    -1,    -1,
      -1,    63,   987,    -1,    -1,    -1,    -1,    -1,    70,    71,
     995,    -1,   997,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    -1,  1007,    -1,  1009,    -1,    88,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    97,    98,  1022,   100,    -1,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,   934,   111,
      -1,    -1,   114,    -1,    -1,   117,  1041,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1053,    -1,
      -1,    -1,    -1,   959,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1071,  1072,    90,    -1,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,  1083,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1091,  1092,  1093,  1094,
    1095,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1113,    -1,
    1115,    -1,    -1,  1118,    -1,  1120,    -1,    -1,  1123,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
    1036,    -1,   154,   155,   156,    -1,   158,   159,   160,   161,
    1145,  1146,   164,  1148,  1149,   167,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1171,    -1,    -1,    -1,
      -1,  1176,    -1,  1178,    -1,    -1,    -1,  1182,    -1,    -1,
      -1,    -1,  1187,  1188,    -1,    -1,  1191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1199,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1213,  1214,
    1215,  1216,  1217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1131,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1253,  1254,
    1255,  1256,  1257,  1258,  1259,  1260,     0,     1,    -1,     3,
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
     104,    -1,     3,   107,    -1,    -1,    -1,   111,    -1,    -1,
     114,    12,    -1,   117,    -1,    -1,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,   142,    -1,
     144,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    63,    -1,    -1,    -1,    -1,    -1,    -1,   173,
      71,   175,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    -1,   100,
      -1,   102,    -1,   104,    -1,    -1,    -1,    -1,    -1,   110,
     111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,
     131,    -1,    -1,   134,   135,   136,   137,    -1,   139,   140,
      -1,   142,   143,   144,   145,   146,   147,    -1,   149,    -1,
      -1,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,     3,   164,    -1,    -1,   167,   168,   169,   170,
     171,    12,    -1,    -1,    -1,    -1,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    97,    98,    -1,   100,
      19,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,    31,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,
     131,    -1,    -1,   134,   135,   136,   137,    -1,   139,   140,
      -1,   142,   143,   144,   145,   146,   147,    -1,   149,    -1,
      -1,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,    -1,   164,    -1,    -1,   167,   168,   169,   170,
      -1,    90,    -1,    12,    -1,    94,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,   131,    -1,    -1,   134,   135,   136,   137,    -1,
     139,   140,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    90,   171,    12,    -1,    94,    -1,    -1,    -1,   178,
      19,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,   131,    -1,    -1,   134,   135,   136,   137,    -1,
     139,   140,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    90,   171,    12,    -1,    94,    -1,    -1,    -1,   178,
      19,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,   131,    -1,    -1,   134,   135,   136,   137,    -1,
     139,   140,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    90,   171,    12,    -1,    94,    -1,    -1,    -1,   178,
      19,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,   131,    -1,    -1,   134,   135,   136,   137,    -1,
     139,   140,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    90,   171,    12,    -1,    94,    -1,    -1,    -1,   178,
      19,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,   131,    -1,    -1,   134,   135,   136,   137,    -1,
     139,   140,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    90,   171,    -1,    -1,    94,    -1,    -1,    12,   178,
      -1,    -1,    -1,    -1,   103,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,   131,    47,    -1,   134,   135,   136,   137,    -1,
     139,   140,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    -1,   171,    -1,    12,    -1,    90,    -1,    -1,   178,
      94,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    47,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    90,   167,   168,   169,    94,   171,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    31,    -1,   125,    -1,    -1,
      36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    73,    -1,   167,
     168,   169,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,   125,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    73,
     166,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    12,    -1,    90,    -1,    -1,    -1,
      94,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    90,   167,   168,   169,    94,    -1,    -1,    12,
      -1,    -1,    -1,    -1,   178,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,   121,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,   171,    -1,    12,    -1,    90,    -1,    -1,
     178,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    90,   167,   168,   169,    94,   171,    -1,
      12,    -1,    -1,    -1,    -1,   178,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,   171,    -1,    12,    -1,    90,    -1,
      -1,   178,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    47,    -1,   125,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    31,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    90,   167,   168,   169,    94,    -1,
      -1,    -1,    12,    -1,    -1,    -1,   178,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   103,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
     125,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,   178,    -1,    94,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      12,   121,   167,   168,   169,   125,   171,    19,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,   142,   143,    36,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    12,    -1,   167,   168,   169,
      -1,    -1,    19,    -1,    -1,    -1,   176,    -1,   178,    -1,
      -1,    73,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    94,    -1,    -1,    12,    13,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,   121,   167,   168,   169,   125,    -1,
      -1,    -1,    -1,    90,    -1,    -1,   178,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   125,    -1,
     167,   168,   169,    -1,    -1,    -1,    12,    -1,    -1,   176,
      -1,   178,    -1,    19,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    31,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,   178,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   125,
      -1,   167,   168,   169,    -1,   171,    -1,    12,    -1,    90,
      -1,    -1,   178,    94,    19,    -1,   142,   143,    -1,   145,
     146,   147,   103,   149,   150,   151,    31,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
     166,   167,   168,   169,   125,    -1,    -1,    -1,    12,    13,
      -1,    -1,   178,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    31,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    90,   167,   168,   169,    94,
      -1,    -1,    -1,    -1,    12,    -1,    -1,   178,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,   125,   167,   168,   169,    -1,    -1,    -1,    -1,   174,
      -1,    -1,    90,   178,    -1,    -1,    94,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    12,    -1,   167,   168,   169,   124,   125,    19,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    34,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    12,    -1,   167,
     168,   169,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    -1,
     125,    -1,    -1,    -1,    -1,    90,    -1,   178,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     125,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,   174,
      -1,    -1,    -1,   178,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      12,    13,   167,   168,   169,    -1,    -1,    19,    -1,   174,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      12,    13,    94,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   125,    -1,   167,   168,   169,    90,    -1,
      -1,    12,    94,    -1,    -1,    -1,   178,    -1,    19,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      31,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   125,    -1,   167,   168,   169,    -1,    -1,
      -1,    12,   174,    -1,    -1,    -1,   178,    -1,    19,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      31,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,   178,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
     121,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   125,    -1,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,   178,    -1,    94,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    12,   166,   167,   168,   169,   124,
     125,    19,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      12,    -1,   167,   168,   169,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    94,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,   125,    -1,    -1,    -1,    -1,   176,    90,
     178,    12,    -1,    94,    -1,    -1,    -1,    -1,    19,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      31,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,   125,   167,   168,   169,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,   178,    -1,    -1,    19,
      61,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    31,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    90,
      -1,    -1,    -1,    94,    -1,    -1,    -1,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    12,    -1,
      90,    -1,    -1,    -1,    94,    19,    -1,    -1,    -1,    -1,
      -1,   142,   143,   103,   145,   146,   147,    31,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,   125,   167,   168,   169,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,   178,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    31,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    90,   167,   168,   169,
      94,    -1,    -1,    -1,    12,    -1,    -1,    -1,   178,   103,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     103,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,   125,   167,   168,   169,    -1,    -1,    -1,    -1,
      12,    -1,    90,    -1,   178,    -1,    94,    19,    -1,   142,
     143,    -1,   145,   146,   147,   103,   149,   150,   151,    31,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,   125,    -1,    -1,
      -1,    12,    -1,    -1,    -1,   178,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      31,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    90,   167,
     168,   169,    94,    -1,    -1,    -1,    12,    -1,    -1,    -1,
     178,   103,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,   125,   167,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,   178,    -1,    94,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    31,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      22,   167,   168,   169,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    35,    -1,    37,    38,    -1,    -1,    -1,
      -1,    43,    -1,    31,    -1,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    90,    -1,    -1,    -1,    94,
      -1,    -1,    74,    -1,    -1,    77,    -1,    79,    -1,    -1,
      -1,    83,    84,    85,    86,    -1,    -1,    89,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    90,    -1,    -1,    -1,    94,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,    -1,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   178,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,   166,   153,   154,   155,   156,   171,
     158,   159,   160,   161,   162,     1,   164,     3,    -1,   167,
     168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      -1,    -1,    -1,     3,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    17,    18,    -1,
      20,    -1,    78,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    97,    98,    -1,   100,    45,   102,    -1,   104,    49,
      -1,   107,    52,    -1,    54,   111,    -1,    -1,   114,     3,
      -1,   117,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    17,    18,    -1,    20,    -1,    78,    -1,
      -1,    25,    -1,    27,    -1,   141,    -1,    -1,    88,    33,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    97,    98,    -1,
     100,    45,   102,    -1,   104,    49,    -1,   107,    52,    -1,
      54,   111,    -1,    -1,   114,     3,    -1,   117,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    17,
      18,    -1,    20,    -1,    78,    79,    -1,    25,    -1,    -1,
      -1,   141,    -1,    -1,    88,    33,    -1,    91,    -1,    -1,
      -1,    39,    -1,    97,    98,    -1,   100,    45,   102,    -1,
     104,    49,    -1,   107,    52,    -1,    54,   111,    -1,    -1,
     114,     3,   116,   117,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    17,    18,    -1,    20,    -1,
      78,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    97,
      98,    -1,   100,    45,   102,    -1,   104,    49,    -1,   107,
      52,    -1,    54,   111,   112,    -1,   114,     3,    -1,   117,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    17,    18,    -1,    20,    -1,    78,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    97,    98,    -1,   100,    45,
     102,    -1,   104,    49,    -1,   107,    52,    -1,    54,   111,
      -1,    -1,   114,     3,    -1,   117,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    17,    18,    -1,
      20,    -1,    78,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    97,    98,    -1,   100,    45,   102,    -1,   104,    49,
      -1,   107,    52,    -1,    54,   111,    -1,    -1,   114,    -1,
      -1,   117,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    -1,
     100,    -1,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,   117
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
     221,   171,   173,   301,    53,    74,    84,   310,    27,    76,
      79,    91,   110,   311,    27,    79,    91,   110,   309,   221,
     322,   323,     1,   328,   165,   166,   370,    12,    19,    31,
      90,    94,   125,   142,   143,   145,   146,   147,   149,   150,
     151,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   167,   168,   169,   178,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   170,   297,   173,   175,    90,    94,   370,   221,
      40,   175,   242,    56,   173,   187,   166,   207,   325,   166,
     148,   166,   222,   344,   345,   346,   372,   221,   221,   103,
     207,   103,   126,   207,   297,   204,   344,   166,   173,   173,
     207,   187,   112,   173,   221,   330,   331,     1,   147,   337,
      47,   148,   187,   226,   148,   226,    13,   173,   173,   226,
     344,   231,   231,    47,    91,   307,   308,   174,   148,   112,
     173,   221,   305,   147,   166,   370,   370,   124,   287,   166,
     171,   226,   173,   344,   166,   255,   255,   166,   172,   172,
     185,   148,   172,   370,   148,   148,   174,   174,     8,   175,
     242,   243,     3,   173,   198,     1,   172,   208,   215,   216,
     370,   210,   370,    66,    36,    73,   166,   266,   268,   110,
     237,   290,   370,   187,    79,   246,   370,   123,   176,   244,
     341,   370,   381,   244,   370,   221,   341,   171,     1,   194,
     221,   272,   275,   174,   302,   304,   306,   311,    91,     1,
     147,   335,   336,    91,     1,     3,    12,    17,    19,    20,
      25,    45,    52,    54,    55,    63,    71,    88,   100,   104,
     111,   117,   142,   143,   144,   145,   146,   147,   149,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   164,
     167,   168,   169,   170,   173,   221,   294,   295,   296,   297,
     347,   126,   324,   148,   166,   166,   166,   370,   370,   370,
     244,   370,   244,   370,   370,   370,   370,   370,   370,   370,
       3,    20,    33,    63,   104,   110,   222,   370,   370,   370,
     370,   370,   370,   370,   370,   370,   370,   370,   370,   370,
     370,   370,   370,   370,    69,   372,   372,   372,   372,   372,
     344,   344,   244,   370,   244,   370,   103,   173,   341,   370,
     221,    27,    47,    91,   116,   358,   361,   362,   370,   386,
     328,   370,   126,   174,   148,   126,   126,   187,    34,   187,
     370,    34,   370,    66,   174,   345,   221,   248,   249,   331,
     148,   174,   225,   370,   126,   332,   370,   329,   262,   221,
     325,   370,   370,   174,   305,   311,   283,   331,   147,   339,
     340,   338,   288,   206,     1,   260,   346,   174,    21,   256,
     172,   174,   174,   342,   372,   174,   342,   341,   370,   197,
     174,   199,   200,   346,   172,   172,   148,   166,    13,   150,
     148,   166,    13,    36,    73,   221,   142,   143,   144,   145,
     146,   160,   164,   169,   209,   296,   297,   298,   370,   209,
     211,   268,   171,   238,   325,    47,   356,   120,   221,   226,
     226,   173,   370,   121,   176,   359,    47,   148,   176,   359,
     124,   148,   176,   226,   359,    32,    47,   226,   359,   166,
     173,     1,   269,   172,   275,   126,   148,   172,   148,   174,
     337,     1,   225,   333,   334,   106,   312,   173,   300,   370,
     142,   150,   300,   300,   370,   323,   173,   175,   166,   166,
     166,   166,   166,   166,   174,   176,   345,    47,   176,    32,
      47,   173,    47,    91,   148,   174,    17,    20,    25,    45,
      52,    63,    71,    88,   100,   111,   117,   347,    90,    90,
     166,   372,   346,   370,   370,    34,   187,    34,    34,   187,
     103,   207,   221,   174,   148,   174,   174,   330,   337,    69,
     372,   221,   174,   174,   340,   147,   284,   174,   338,   152,
     299,   332,   370,   172,    75,   118,   172,   261,   174,   173,
     207,   221,   257,    47,   176,    47,   148,   174,   215,   222,
      17,    18,    20,    25,    45,    49,    52,    71,    78,    88,
      97,    98,   100,   102,   111,   114,   117,   171,   214,   298,
     370,   112,   370,   209,   211,   148,   166,    13,   166,   171,
     269,   322,   328,   244,   370,   226,   370,    47,   341,   173,
     187,   176,   244,   370,   176,   187,   370,   176,   370,   176,
     370,   226,    44,   370,   244,   370,   226,   345,   172,    83,
      86,   172,   186,   194,   228,   370,   273,   274,   304,   312,
      61,   119,   316,   302,   303,   174,   295,   297,   174,   176,
     174,   244,   370,    44,   244,   370,   345,   361,   337,   347,
     347,   187,   187,   370,   187,    34,   166,   166,   249,   207,
     332,   173,   173,   299,   332,   333,   312,   340,   370,   187,
     240,   341,   257,   147,   207,   244,   370,   244,   370,   200,
     209,    13,    36,    73,    36,    73,   166,   166,   298,   370,
     370,   269,   172,   166,   166,   356,   356,   166,   370,   174,
     360,   361,   187,   176,   359,   176,   359,   187,   124,   370,
      32,   226,   359,    32,   226,   359,   174,   194,   228,   228,
     275,   194,   316,     3,    54,    93,   104,   317,   318,   319,
     370,   291,   174,   300,   300,   176,   176,   370,    32,    32,
     174,   332,    34,   187,   344,   344,   299,   332,    32,   226,
     174,   370,   176,   176,   172,   112,   370,   209,   211,   209,
     211,    13,   172,   226,   226,   226,   148,   174,    44,   187,
     370,   176,    44,   187,   370,   176,   370,   103,    44,   370,
     226,    44,   370,   226,   166,   228,   275,   278,   319,   119,
     148,   126,   153,   155,   156,   158,   159,    61,    32,   166,
     206,   313,   314,    44,    44,   103,    44,    44,   187,   174,
     174,   187,   207,   166,   166,   166,   166,   370,   361,   370,
     187,   370,   187,   370,   370,   370,   314,   370,   318,   319,
     319,   319,   319,   319,   319,   317,   185,   370,   370,   370,
     370,   165,   165,   103,   103,   103,   103,   103,   103,   103,
     103,   370,   370,   370,   370
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
       0,     2,     0,     2,     3,     3,     4,     4,     4,     2,
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
#line 635 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6036 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 639 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6042 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 640 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6048 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 647 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6056 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 655 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6064 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 659 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6073 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 669 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6081 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 674 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6089 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 681 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6095 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 682 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6101 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 683 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6107 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 684 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6113 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 685 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6119 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6125 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 687 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6131 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 688 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6137 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 689 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6143 "bison-chpl-lib.cpp"
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
    BUILDER->noteBlockHeaderLocation(node.get(), LOC((yylsp[-1])));
    if (context->hasCurlyBracesLoc()) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC(context->curlyBracesLoc()));
      context->resetCurlyBracesLoc();
    }
    CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6165 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 708 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6171 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 709 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6177 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: TINIT TTHIS TSEMI  */
#line 711 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr));
    auto node = Init::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6189 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 719 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6202 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 728 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6215 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TLABEL ident_def stmt  */
#line 737 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6223 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 741 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6234 "bison-chpl-lib.cpp"
    break;

  case 29: /* stmt_base: error TSEMI  */
#line 748 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6243 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: assignment_stmt  */
#line 755 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6249 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: if_stmt  */
#line 756 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6255 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: loop_stmt  */
#line 757 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6261 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: select_stmt  */
#line 758 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6267 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: manage_stmt  */
#line 759 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6273 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 760 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6279 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: throw_stmt  */
#line 761 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6285 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 763 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6291 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 765 "chpl.ypp"
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
    BUILDER->noteBlockHeaderLocation(node.get(), LOC((yylsp[-2])));
    if (context->hasCurlyBracesLoc()) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC(context->curlyBracesLoc()));
      context->resetCurlyBracesLoc();
    }
    CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6315 "bison-chpl-lib.cpp"
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
#line 6327 "bison-chpl-lib.cpp"
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
    BUILDER->noteBlockHeaderLocation(node.get(), LOC2((yylsp[-2]), (yylsp[-1])));
    if (context->hasCurlyBracesLoc()) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC(context->curlyBracesLoc()));
      context->resetCurlyBracesLoc();
    }
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6348 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TLOCAL do_stmt  */
#line 810 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-1]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Local::build(BUILDER, LOC((yyloc)), blockStyle, std::move(stmts));
    BUILDER->noteBlockHeaderLocation(node.get(), LOC((yylsp[-1])));
    if (context->hasCurlyBracesLoc()) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC(context->curlyBracesLoc()));
      context->resetCurlyBracesLoc();
    }
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6368 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TON expr do_stmt  */
#line 826 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = On::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)), blockStyle,
                          std::move(stmts));
    BUILDER->noteBlockHeaderLocation(node.get(), LOC2((yylsp[-2]), (yylsp[-1])));
    if (context->hasCurlyBracesLoc()) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC(context->curlyBracesLoc()));
      context->resetCurlyBracesLoc();
    }
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6389 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 843 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Serial::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)), blockStyle,
                              std::move(stmts));
    BUILDER->noteBlockHeaderLocation(node.get(), LOC2((yylsp[-2]), (yylsp[-1])));
    if (context->hasCurlyBracesLoc()) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC(context->curlyBracesLoc()));
      context->resetCurlyBracesLoc();
    }
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6410 "bison-chpl-lib.cpp"
    break;

  case 44: /* tryable_stmt: TSERIAL do_stmt  */
#line 860 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-1]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Serial::build(BUILDER, LOC((yyloc)), blockStyle, std::move(stmts));
    BUILDER->noteBlockHeaderLocation(node.get(), LOC((yylsp[-1])));
    if (context->hasCurlyBracesLoc()) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC(context->curlyBracesLoc()));
      context->resetCurlyBracesLoc();
    }
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6430 "bison-chpl-lib.cpp"
    break;

  case 45: /* tryable_stmt: TSYNC stmt  */
#line 876 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-1]),
                               false, (yylsp[-1]), (yyvsp[0].commentsAndStmt));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Sync::build(BUILDER, LOC((yyloc)), blockStyle,
                            std::move(stmts));
    BUILDER->noteBlockHeaderLocation(node.get(), LOC((yylsp[-1])));
    if (context->hasCurlyBracesLoc()) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC(context->curlyBracesLoc()));
      context->resetCurlyBracesLoc();
    }
    CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6452 "bison-chpl-lib.cpp"
    break;

  case 48: /* collect_attributes: %empty  */
#line 901 "chpl.ypp"
  {
    auto attributeGroup = context->buildAttributeGroup((yyloc));
    if (attributeGroup != nullptr) {
      context->resetAttributeGroupPartsState();
    }
    context->loopAttributes.push_back(std::move(attributeGroup));
  }
#line 6464 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 926 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6472 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATMARK TIDENT  */
#line 933 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6481 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 940 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6489 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 944 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6500 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 954 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6506 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 956 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6514 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 960 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6522 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 967 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6531 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 972 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6541 "bison-chpl-lib.cpp"
    break;

  case 64: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 985 "chpl.ypp"
    {
      // take into account location of public/private, if any
      auto loc = context->declStartLoc((yylsp[-1]));

      ModuleParts parts = {
        .comments=context->gatherComments(loc),
        .attributeGroup=context->buildAttributeGroup((yyloc)).release(),
        .visibility=(yyvsp[-3].visibilityTag),
        .kind=(yyvsp[-2].moduleKind),
        .name=(yyvsp[0].uniqueStr),
        .locName=(yylsp[0])
      };

      (yyval.moduleParts) = parts;
      context->resetDeclState();
      context->clearComments();
    }
#line 6563 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 1006 "chpl.ypp"
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
      BUILDER->noteDeclNameLocation(mod.get(), LOC(parts.locName));
      BUILDER->noteDeclHeaderLocation(mod.get(), LOC((yylsp[-2])));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 6584 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 1023 "chpl.ypp"
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
      BUILDER->noteDeclNameLocation(mod.get(), LOC(parts.locName));
      BUILDER->noteDeclHeaderLocation(mod.get(), LOC((yylsp[-3])));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 6605 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1040 "chpl.ypp"
    {
      ModuleParts parts = (yyvsp[-3].moduleParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      ParserExprList* body = context->makeList(std::move(err));
      auto mod = Module::build(BUILDER, LOC((yyloc)), toOwned(parts.attributeGroup),
                               parts.visibility,
                               parts.name,
                               parts.kind,
                               context->consumeList(body));
      BUILDER->noteDeclNameLocation(mod.get(), LOC(parts.locName));
      BUILDER->noteDeclHeaderLocation(mod.get(), LOC((yylsp[-3])));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 6624 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1057 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6630 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1058 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6637 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1060 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6644 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1065 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6650 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1066 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6657 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1072 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6667 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1090 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-1]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6676 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1095 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6685 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR error TRCBR  */
#line 1100 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6698 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt: block_stmt_body  */
#line 1112 "chpl.ypp"
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
#line 6717 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1130 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6723 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1131 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6729 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1136 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6737 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1140 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6745 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1144 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6754 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1149 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6762 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1153 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6770 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1157 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6779 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1167 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6788 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1172 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6798 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1178 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6809 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1185 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6818 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1190 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6829 "bison-chpl-lib.cpp"
    break;

  case 91: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1197 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6841 "bison-chpl-lib.cpp"
    break;

  case 92: /* opt_only_ls: %empty  */
#line 1207 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6847 "bison-chpl-lib.cpp"
    break;

  case 93: /* opt_only_ls: renames_ls  */
#line 1208 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6853 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1213 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6862 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1218 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6874 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1226 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6887 "bison-chpl-lib.cpp"
    break;

  case 97: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1235 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6901 "bison-chpl-lib.cpp"
    break;

  case 98: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1245 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6913 "bison-chpl-lib.cpp"
    break;

  case 99: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1253 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6926 "bison-chpl-lib.cpp"
    break;

  case 100: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1262 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6940 "bison-chpl-lib.cpp"
    break;

  case 101: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1275 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6949 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_expr: expr  */
#line 1311 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6957 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_expr: expr TDOT all_op_name  */
#line 1315 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6966 "bison-chpl-lib.cpp"
    break;

  case 121: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1320 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6975 "bison-chpl-lib.cpp"
    break;

  case 122: /* import_expr: expr TAS ident_use  */
#line 1325 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6985 "bison-chpl-lib.cpp"
    break;

  case 123: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1331 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6995 "bison-chpl-lib.cpp"
    break;

  case 124: /* import_ls: import_expr  */
#line 1339 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7001 "bison-chpl-lib.cpp"
    break;

  case 125: /* import_ls: import_ls TCOMMA import_expr  */
#line 1340 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7007 "bison-chpl-lib.cpp"
    break;

  case 126: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1345 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7017 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1354 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7025 "bison-chpl-lib.cpp"
    break;

  case 128: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1358 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7033 "bison-chpl-lib.cpp"
    break;

  case 129: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1362 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7041 "bison-chpl-lib.cpp"
    break;

  case 130: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1366 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7049 "bison-chpl-lib.cpp"
    break;

  case 131: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1370 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7057 "bison-chpl-lib.cpp"
    break;

  case 132: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1374 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7065 "bison-chpl-lib.cpp"
    break;

  case 133: /* opt_label_ident: %empty  */
#line 1382 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7071 "bison-chpl-lib.cpp"
    break;

  case 134: /* opt_label_ident: TIDENT  */
#line 1383 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7077 "bison-chpl-lib.cpp"
    break;

  case 135: /* opt_label_ident: TINIT  */
#line 1384 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7083 "bison-chpl-lib.cpp"
    break;

  case 144: /* ident_use: TIDENT  */
#line 1410 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7089 "bison-chpl-lib.cpp"
    break;

  case 145: /* ident_use: TINIT  */
#line 1411 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7095 "bison-chpl-lib.cpp"
    break;

  case 146: /* ident_use: TTHIS  */
#line 1412 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7101 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TBOOL  */
#line 1439 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7107 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TENUM  */
#line 1440 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7113 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TINT  */
#line 1441 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7119 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TUINT  */
#line 1442 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7125 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TREAL  */
#line 1443 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7131 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TIMAG  */
#line 1444 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7137 "bison-chpl-lib.cpp"
    break;

  case 172: /* scalar_type: TCOMPLEX  */
#line 1445 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7143 "bison-chpl-lib.cpp"
    break;

  case 173: /* scalar_type: TBYTES  */
#line 1446 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7149 "bison-chpl-lib.cpp"
    break;

  case 174: /* scalar_type: TSTRING  */
#line 1447 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7155 "bison-chpl-lib.cpp"
    break;

  case 175: /* scalar_type: TLOCALE  */
#line 1448 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7161 "bison-chpl-lib.cpp"
    break;

  case 176: /* scalar_type: TNOTHING  */
#line 1449 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7167 "bison-chpl-lib.cpp"
    break;

  case 177: /* scalar_type: TVOID  */
#line 1450 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7173 "bison-chpl-lib.cpp"
    break;

  case 182: /* do_stmt: TDO stmt  */
#line 1464 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7179 "bison-chpl-lib.cpp"
    break;

  case 183: /* do_stmt: block_stmt  */
#line 1465 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7185 "bison-chpl-lib.cpp"
    break;

  case 184: /* return_stmt: TRETURN TSEMI  */
#line 1470 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7196 "bison-chpl-lib.cpp"
    break;

  case 185: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1477 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7207 "bison-chpl-lib.cpp"
    break;

  case 186: /* class_level_stmt: TSEMI  */
#line 1487 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7215 "bison-chpl-lib.cpp"
    break;

  case 187: /* class_level_stmt: inner_class_level_stmt  */
#line 1491 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7225 "bison-chpl-lib.cpp"
    break;

  case 188: /* $@1: %empty  */
#line 1496 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7232 "bison-chpl-lib.cpp"
    break;

  case 189: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1498 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7241 "bison-chpl-lib.cpp"
    break;

  case 190: /* $@2: %empty  */
#line 1502 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7248 "bison-chpl-lib.cpp"
    break;

  case 191: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1504 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7257 "bison-chpl-lib.cpp"
    break;

  case 199: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1522 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7266 "bison-chpl-lib.cpp"
    break;

  case 200: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1527 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7275 "bison-chpl-lib.cpp"
    break;

  case 201: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1532 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7284 "bison-chpl-lib.cpp"
    break;

  case 202: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1537 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7292 "bison-chpl-lib.cpp"
    break;

  case 203: /* forwarding_decl_start: TFORWARDING  */
#line 1544 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7301 "bison-chpl-lib.cpp"
    break;

  case 204: /* extern_or_export: TEXTERN  */
#line 1551 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7307 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_or_export: TEXPORT  */
#line 1552 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7313 "bison-chpl-lib.cpp"
    break;

  case 206: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1557 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7323 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1567 "chpl.ypp"
  {
    auto headerLoc = context->makeSpannedLocation((yylsp[-5]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7333 "bison-chpl-lib.cpp"
    break;

  case 208: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1574 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    auto headerLoc = context->makeSpannedLocation((yylsp[-6]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7345 "bison-chpl-lib.cpp"
    break;

  case 209: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1582 "chpl.ypp"
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
#line 7367 "bison-chpl-lib.cpp"
    break;

  case 210: /* $@3: %empty  */
#line 1600 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7375 "bison-chpl-lib.cpp"
    break;

  case 211: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1604 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7384 "bison-chpl-lib.cpp"
    break;

  case 212: /* $@4: %empty  */
#line 1609 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7393 "bison-chpl-lib.cpp"
    break;

  case 213: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1614 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7402 "bison-chpl-lib.cpp"
    break;

  case 214: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1622 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7410 "bison-chpl-lib.cpp"
    break;

  case 215: /* no_loop_attributes: %empty  */
#line 1627 "chpl.ypp"
                    { context->loopAttributes.push_back(nullptr); }
#line 7416 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1630 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7422 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1631 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7428 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1647 "chpl.ypp"
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
    if (context->hasCurlyBracesLoc()) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC(context->curlyBracesLoc()));
      context->resetCurlyBracesLoc();
    }
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7455 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1670 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto body = context->consumeToBlock((yylsp[0]), exprLst);
    auto node = While::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)),
                             blockStyle,
                             std::move(body),
                             context->popLoopAttributeGroup());
    BUILDER->noteLoopHeaderLocation(node.get(), LOC2((yylsp[-2]), (yylsp[-1])));
    if (context->hasCurlyBracesLoc()) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC(context->curlyBracesLoc()));
      context->resetCurlyBracesLoc();
    }
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7478 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1689 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto body = context->consumeToBlock((yylsp[0]), exprLst);
    auto node = While::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)),
                             blockStyle,
                             std::move(body),
                             context->popLoopAttributeGroup());
    BUILDER->noteLoopHeaderLocation(node.get(), LOC2((yylsp[-2]), (yylsp[-1])));
    if (context->hasCurlyBracesLoc()) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC(context->curlyBracesLoc()));
      context->resetCurlyBracesLoc();
    }
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7501 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1708 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7510 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1713 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7519 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1718 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7528 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1723 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-5]), (yylsp[0]), (yyvsp[0].blockOrDo));
    AstNode* ident = context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr));
    auto index = context->buildLoopIndexDecl((yylsp[-3]), toOwned(ident));
    auto body = context->consumeToBlock((yylsp[0]), exprLst);
    auto node = For::build(BUILDER, LOC((yyloc)), std::move(index),
                           toOwned((yyvsp[-1].expr)),
                           blockStyle,
                           std::move(body),
                           /*isExpressionLevel*/ false,
                           /*isParam*/ true,
                           context->popLoopAttributeGroup());
    BUILDER->noteLoopHeaderLocation(node.get(), LOC2((yylsp[-5]), (yylsp[-1])));
    if (context->hasCurlyBracesLoc()) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC(context->curlyBracesLoc()));
      context->resetCurlyBracesLoc();
    }
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    context->resetAttributeGroupPartsState();
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7557 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1748 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7566 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: for_loop_kw expr TIN expr forall_intent_clause do_stmt  */
#line 1753 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7575 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1758 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7584 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1763 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7593 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1768 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7602 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: for_loop_kw expr forall_intent_clause do_stmt  */
#line 1773 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7611 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1778 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7620 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: for_loop_kw zippered_iterator forall_intent_clause do_stmt  */
#line 1783 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7629 "bison-chpl-lib.cpp"
    break;

  case 236: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1788 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7638 "bison-chpl-lib.cpp"
    break;

  case 237: /* loop_stmt_base: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1793 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7647 "bison-chpl-lib.cpp"
    break;

  case 238: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1798 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7656 "bison-chpl-lib.cpp"
    break;

  case 239: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1803 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7665 "bison-chpl-lib.cpp"
    break;

  case 240: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1808 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7674 "bison-chpl-lib.cpp"
    break;

  case 241: /* loop_stmt_base: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1813 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7683 "bison-chpl-lib.cpp"
    break;

  case 242: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1818 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7693 "bison-chpl-lib.cpp"
    break;

  case 243: /* loop_stmt_base: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1824 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7703 "bison-chpl-lib.cpp"
    break;

  case 244: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1833 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7713 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF expr TTHEN stmt  */
#line 1842 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7721 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF expr block_stmt  */
#line 1846 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7729 "bison-chpl-lib.cpp"
    break;

  case 247: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1850 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7737 "bison-chpl-lib.cpp"
    break;

  case 248: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1854 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7745 "bison-chpl-lib.cpp"
    break;

  case 249: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1858 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7753 "bison-chpl-lib.cpp"
    break;

  case 250: /* if_stmt: TIF ifvar block_stmt  */
#line 1862 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7761 "bison-chpl-lib.cpp"
    break;

  case 251: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1866 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7769 "bison-chpl-lib.cpp"
    break;

  case 252: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1870 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7777 "bison-chpl-lib.cpp"
    break;

  case 253: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1874 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7786 "bison-chpl-lib.cpp"
    break;

  case 254: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1879 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7795 "bison-chpl-lib.cpp"
    break;

  case 255: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1884 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7804 "bison-chpl-lib.cpp"
    break;

  case 256: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1889 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7813 "bison-chpl-lib.cpp"
    break;

  case 257: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1897 "chpl.ypp"
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
#line 7832 "bison-chpl-lib.cpp"
    break;

  case 258: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1912 "chpl.ypp"
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
#line 7851 "bison-chpl-lib.cpp"
    break;

  case 259: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1930 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-4]), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7859 "bison-chpl-lib.cpp"
    break;

  case 260: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1934 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7867 "bison-chpl-lib.cpp"
    break;

  case 261: /* ifc_formal_ls: ifc_formal  */
#line 1940 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7873 "bison-chpl-lib.cpp"
    break;

  case 262: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1941 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7879 "bison-chpl-lib.cpp"
    break;

  case 263: /* ifc_formal: ident_def  */
#line 1946 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7885 "bison-chpl-lib.cpp"
    break;

  case 283: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1990 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7893 "bison-chpl-lib.cpp"
    break;

  case 284: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1994 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7901 "bison-chpl-lib.cpp"
    break;

  case 285: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1998 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7909 "bison-chpl-lib.cpp"
    break;

  case 286: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2005 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7917 "bison-chpl-lib.cpp"
    break;

  case 287: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 2009 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7925 "bison-chpl-lib.cpp"
    break;

  case 288: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2013 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7933 "bison-chpl-lib.cpp"
    break;

  case 289: /* try_stmt: TTRY tryable_stmt  */
#line 2020 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7941 "bison-chpl-lib.cpp"
    break;

  case 290: /* try_stmt: TTRYBANG tryable_stmt  */
#line 2024 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7949 "bison-chpl-lib.cpp"
    break;

  case 291: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 2028 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7957 "bison-chpl-lib.cpp"
    break;

  case 292: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 2032 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7965 "bison-chpl-lib.cpp"
    break;

  case 293: /* catch_expr_ls: %empty  */
#line 2038 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7971 "bison-chpl-lib.cpp"
    break;

  case 294: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2039 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7977 "bison-chpl-lib.cpp"
    break;

  case 295: /* catch_expr: TCATCH block_stmt  */
#line 2044 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 7985 "bison-chpl-lib.cpp"
    break;

  case 296: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2048 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 7993 "bison-chpl-lib.cpp"
    break;

  case 297: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2052 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8001 "bison-chpl-lib.cpp"
    break;

  case 298: /* catch_expr_inner: ident_def  */
#line 2059 "chpl.ypp"
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
#line 8018 "bison-chpl-lib.cpp"
    break;

  case 299: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2072 "chpl.ypp"
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
#line 8035 "bison-chpl-lib.cpp"
    break;

  case 300: /* throw_stmt: TTHROW expr TSEMI  */
#line 2088 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8045 "bison-chpl-lib.cpp"
    break;

  case 301: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2096 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), context->makeSpannedLocation((yylsp[-4]), (yylsp[-3])), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 8054 "bison-chpl-lib.cpp"
    break;

  case 302: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2101 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 8065 "bison-chpl-lib.cpp"
    break;

  case 303: /* when_stmt_ls: %empty  */
#line 2110 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8071 "bison-chpl-lib.cpp"
    break;

  case 304: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2111 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8077 "bison-chpl-lib.cpp"
    break;

  case 305: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2116 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), context->makeSpannedLocation((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8085 "bison-chpl-lib.cpp"
    break;

  case 306: /* when_stmt: TOTHERWISE stmt  */
#line 2120 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-1]), nullptr, blockOrDo);
  }
#line 8094 "bison-chpl-lib.cpp"
    break;

  case 307: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2125 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-3]), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 8104 "bison-chpl-lib.cpp"
    break;

  case 308: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2134 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8113 "bison-chpl-lib.cpp"
    break;

  case 309: /* manager_expr: expr TAS ident_def  */
#line 2139 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8121 "bison-chpl-lib.cpp"
    break;

  case 310: /* manager_expr: expr  */
#line 2143 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8129 "bison-chpl-lib.cpp"
    break;

  case 311: /* manager_expr_ls: manager_expr  */
#line 2149 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8135 "bison-chpl-lib.cpp"
    break;

  case 312: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2150 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8141 "bison-chpl-lib.cpp"
    break;

  case 313: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2155 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), locHeader, (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8150 "bison-chpl-lib.cpp"
    break;

  case 314: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2165 "chpl.ypp"
    {
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8160 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2171 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8172 "bison-chpl-lib.cpp"
    break;

  case 316: /* class_start: class_tag ident_def  */
#line 2183 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8180 "bison-chpl-lib.cpp"
    break;

  case 317: /* class_tag: TCLASS  */
#line 2189 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8186 "bison-chpl-lib.cpp"
    break;

  case 318: /* class_tag: TRECORD  */
#line 2190 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8192 "bison-chpl-lib.cpp"
    break;

  case 319: /* class_tag: TUNION  */
#line 2191 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8198 "bison-chpl-lib.cpp"
    break;

  case 320: /* opt_inherit: %empty  */
#line 2195 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8204 "bison-chpl-lib.cpp"
    break;

  case 321: /* opt_inherit: TCOLON expr_ls  */
#line 2196 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8210 "bison-chpl-lib.cpp"
    break;

  case 322: /* class_level_stmt_ls: %empty  */
#line 2200 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8219 "bison-chpl-lib.cpp"
    break;

  case 323: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2205 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8227 "bison-chpl-lib.cpp"
    break;

  case 324: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2209 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8235 "bison-chpl-lib.cpp"
    break;

  case 325: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2213 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8243 "bison-chpl-lib.cpp"
    break;

  case 326: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2217 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8251 "bison-chpl-lib.cpp"
    break;

  case 327: /* enum_decl_stmt: enum_header TLCBR enum_ls TRCBR  */
#line 2225 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-3].typeDeclParts);
      ParserExprList* list = (yyvsp[-1].exprList);
      // get any comments after the last element but before the closing brace
      context->appendList(list, context->gatherComments((yylsp[0])));

      auto decl = Enum::build(BUILDER, LOC((yyloc)), toOwned(parts.attributeGroup),
                              parts.visibility,
                              parts.name,
                              context->consumeList(list));
      BUILDER->noteDeclNameLocation(decl.get(), LOC(parts.locName));
      BUILDER->noteDeclHeaderLocation(decl.get(), LOC((yylsp[-3])));
      CommentsAndStmt cs = {parts.comments, decl.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8274 "bison-chpl-lib.cpp"
    break;

  case 328: /* enum_decl_stmt: enum_header TLCBR error TRCBR  */
#line 2244 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-3].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8288 "bison-chpl-lib.cpp"
    break;

  case 329: /* enum_header: TENUM ident_def  */
#line 2257 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Enum);
  }
#line 8296 "bison-chpl-lib.cpp"
    break;

  case 330: /* enum_ls: enum_item  */
#line 2264 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8305 "bison-chpl-lib.cpp"
    break;

  case 331: /* enum_ls: enum_ls TCOMMA  */
#line 2269 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8315 "bison-chpl-lib.cpp"
    break;

  case 332: /* $@5: %empty  */
#line 2275 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8324 "bison-chpl-lib.cpp"
    break;

  case 333: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2280 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8333 "bison-chpl-lib.cpp"
    break;

  case 334: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2285 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8342 "bison-chpl-lib.cpp"
    break;

  case 335: /* $@6: %empty  */
#line 2290 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8351 "bison-chpl-lib.cpp"
    break;

  case 336: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2295 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8360 "bison-chpl-lib.cpp"
    break;

  case 337: /* enum_item: ident_def  */
#line 2305 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8371 "bison-chpl-lib.cpp"
    break;

  case 338: /* enum_item: ident_def TASSIGN expr  */
#line 2312 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8384 "bison-chpl-lib.cpp"
    break;

  case 339: /* lambda_decl_start: TLAMBDA  */
#line 2324 "chpl.ypp"
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
#line 8400 "bison-chpl-lib.cpp"
    break;

  case 340: /* $@7: %empty  */
#line 2341 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8410 "bison-chpl-lib.cpp"
    break;

  case 341: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2347 "chpl.ypp"
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
#line 8432 "bison-chpl-lib.cpp"
    break;

  case 343: /* linkage_spec: linkage_spec_empty  */
#line 2370 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8438 "bison-chpl-lib.cpp"
    break;

  case 344: /* linkage_spec: TINLINE  */
#line 2371 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8445 "bison-chpl-lib.cpp"
    break;

  case 345: /* linkage_spec: TOVERRIDE  */
#line 2373 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8452 "bison-chpl-lib.cpp"
    break;

  case 346: /* opt_fn_type_formal_ls: %empty  */
#line 2378 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8458 "bison-chpl-lib.cpp"
    break;

  case 347: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2379 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8464 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_type_formal_ls: fn_type_formal  */
#line 2383 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8470 "bison-chpl-lib.cpp"
    break;

  case 349: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2384 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8476 "bison-chpl-lib.cpp"
    break;

  case 350: /* fn_type_formal: named_formal  */
#line 2389 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8482 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2392 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8488 "bison-chpl-lib.cpp"
    break;

  case 352: /* fn_type_formal: formal_type  */
#line 2394 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8494 "bison-chpl-lib.cpp"
    break;

  case 353: /* opt_fn_type_ret_type: %empty  */
#line 2398 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8500 "bison-chpl-lib.cpp"
    break;

  case 354: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2399 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8506 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2412 "chpl.ypp"
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
#line 8525 "bison-chpl-lib.cpp"
    break;

  case 356: /* $@8: %empty  */
#line 2430 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8535 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2436 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8548 "bison-chpl-lib.cpp"
    break;

  case 358: /* $@9: %empty  */
#line 2445 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8558 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2451 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8571 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2463 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8579 "bison-chpl-lib.cpp"
    break;

  case 361: /* $@10: %empty  */
#line 2472 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8587 "bison-chpl-lib.cpp"
    break;

  case 362: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2476 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-6].functionParts);
    fp.returnIntent = (yyvsp[-5].returnTag);
    fp.returnType = (yyvsp[-4].expr);
    fp.throws = ((yyvsp[-3].throwsTag) == ThrowsTag_THROWS);
    WhereAndLifetime wl = (yyvsp[-2].lifetimeAndWhere);
    fp.body = (yyvsp[0].exprList);
    fp.lifetime = wl.lifetime;
    fp.where = wl.where;
    // conditionals are needed since bison has weird behavior when the rule is empty
    fp.headerLoc = context->makeSpannedLocation((yylsp[-6]),
      (yyvsp[-3].throwsTag) != ThrowsTag_DEFAULT ? (yylsp[-3]) :
        ((yyvsp[-4].expr) != nullptr ? (yylsp[-4]) :
          ((yyvsp[-5].returnTag) != Function::DEFAULT_RETURN_INTENT ? (yylsp[-5]) : (yylsp[-6]))));
    context->exitScopeForFunctionDecl(fp);
    (yyval.functionParts) = fp;
  }
#line 8609 "bison-chpl-lib.cpp"
    break;

  case 363: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2497 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8621 "bison-chpl-lib.cpp"
    break;

  case 364: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2505 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8633 "bison-chpl-lib.cpp"
    break;

  case 365: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2513 "chpl.ypp"
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
#line 8648 "bison-chpl-lib.cpp"
    break;

  case 366: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2524 "chpl.ypp"
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
#line 8663 "bison-chpl-lib.cpp"
    break;

  case 367: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2535 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8673 "bison-chpl-lib.cpp"
    break;

  case 368: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2544 "chpl.ypp"
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
#line 8689 "bison-chpl-lib.cpp"
    break;

  case 370: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2559 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8695 "bison-chpl-lib.cpp"
    break;

  case 373: /* fn_ident: ident_def TBANG  */
#line 2566 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8705 "bison-chpl-lib.cpp"
    break;

  case 413: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2623 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8711 "bison-chpl-lib.cpp"
    break;

  case 414: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2624 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8717 "bison-chpl-lib.cpp"
    break;

  case 415: /* opt_formal_ls: %empty  */
#line 2628 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8723 "bison-chpl-lib.cpp"
    break;

  case 416: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2629 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8729 "bison-chpl-lib.cpp"
    break;

  case 417: /* req_formal_ls: TLP TRP  */
#line 2633 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8735 "bison-chpl-lib.cpp"
    break;

  case 418: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2634 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8741 "bison-chpl-lib.cpp"
    break;

  case 419: /* formal_ls_inner: formal  */
#line 2638 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8747 "bison-chpl-lib.cpp"
    break;

  case 420: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2639 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8753 "bison-chpl-lib.cpp"
    break;

  case 421: /* formal_ls: %empty  */
#line 2643 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8759 "bison-chpl-lib.cpp"
    break;

  case 422: /* formal_ls: formal_ls_inner  */
#line 2644 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8765 "bison-chpl-lib.cpp"
    break;

  case 426: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2658 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8773 "bison-chpl-lib.cpp"
    break;

  case 427: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2663 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8781 "bison-chpl-lib.cpp"
    break;

  case 428: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2668 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8789 "bison-chpl-lib.cpp"
    break;

  case 429: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2673 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8797 "bison-chpl-lib.cpp"
    break;

  case 430: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2678 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8805 "bison-chpl-lib.cpp"
    break;

  case 431: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2683 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8813 "bison-chpl-lib.cpp"
    break;

  case 432: /* opt_formal_intent_tag: %empty  */
#line 2689 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 8822 "bison-chpl-lib.cpp"
    break;

  case 433: /* opt_formal_intent_tag: required_intent_tag  */
#line 2694 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8831 "bison-chpl-lib.cpp"
    break;

  case 434: /* required_intent_tag: TIN  */
#line 2701 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 8837 "bison-chpl-lib.cpp"
    break;

  case 435: /* required_intent_tag: TINOUT  */
#line 2702 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 8843 "bison-chpl-lib.cpp"
    break;

  case 436: /* required_intent_tag: TOUT  */
#line 2703 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 8849 "bison-chpl-lib.cpp"
    break;

  case 437: /* required_intent_tag: TCONST TIN  */
#line 2704 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 8855 "bison-chpl-lib.cpp"
    break;

  case 438: /* required_intent_tag: TCONST TREF  */
#line 2705 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 8861 "bison-chpl-lib.cpp"
    break;

  case 439: /* required_intent_tag: TCONST  */
#line 2706 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 8867 "bison-chpl-lib.cpp"
    break;

  case 440: /* required_intent_tag: TPARAM  */
#line 2707 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 8873 "bison-chpl-lib.cpp"
    break;

  case 441: /* required_intent_tag: TREF  */
#line 2708 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 8879 "bison-chpl-lib.cpp"
    break;

  case 442: /* required_intent_tag: TTYPE  */
#line 2709 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 8885 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_this_intent_tag: %empty  */
#line 2713 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 8891 "bison-chpl-lib.cpp"
    break;

  case 444: /* opt_this_intent_tag: TPARAM  */
#line 2714 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 8897 "bison-chpl-lib.cpp"
    break;

  case 445: /* opt_this_intent_tag: TREF  */
#line 2715 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 8903 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_this_intent_tag: TCONST TREF  */
#line 2716 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 8909 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_this_intent_tag: TCONST  */
#line 2717 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 8915 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_this_intent_tag: TTYPE  */
#line 2718 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 8921 "bison-chpl-lib.cpp"
    break;

  case 449: /* proc_iter_or_op: TPROC  */
#line 2722 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8927 "bison-chpl-lib.cpp"
    break;

  case 450: /* proc_iter_or_op: TITER  */
#line 2723 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8933 "bison-chpl-lib.cpp"
    break;

  case 451: /* proc_iter_or_op: TOPERATOR  */
#line 2724 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8939 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: %empty  */
#line 2728 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 8945 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_ret_tag: TOUT  */
#line 2729 "chpl.ypp"
                    { (yyval.returnTag) = Function::OUT; }
#line 8951 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_ret_tag: TCONST  */
#line 2730 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 8957 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_ret_tag: TCONST TREF  */
#line 2731 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 8963 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_ret_tag: TREF  */
#line 2732 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 8969 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_ret_tag: TPARAM  */
#line 2733 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 8975 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_ret_tag: TTYPE  */
#line 2734 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 8981 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_throws_error: %empty  */
#line 2738 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8987 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_throws_error: TTHROWS  */
#line 2739 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8993 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_function_body_stmt: TSEMI  */
#line 2742 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8999 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_function_body_stmt: function_body_stmt  */
#line 2743 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9005 "bison-chpl-lib.cpp"
    break;

  case 463: /* function_body_stmt: block_stmt_body  */
#line 2747 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9011 "bison-chpl-lib.cpp"
    break;

  case 464: /* function_body_stmt: TDO toplevel_stmt  */
#line 2748 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 9017 "bison-chpl-lib.cpp"
    break;

  case 465: /* query_expr: TQUERIEDIDENT  */
#line 2752 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9023 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_lifetime_where: %empty  */
#line 2757 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9029 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_lifetime_where: TWHERE expr  */
#line 2759 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9035 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2761 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9041 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2763 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9047 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2765 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9053 "bison-chpl-lib.cpp"
    break;

  case 471: /* lifetime_components_expr: lifetime_expr  */
#line 2770 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9059 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2772 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9065 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2777 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9071 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2779 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9077 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2781 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9083 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2783 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9089 "bison-chpl-lib.cpp"
    break;

  case 477: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2785 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9095 "bison-chpl-lib.cpp"
    break;

  case 478: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2787 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9101 "bison-chpl-lib.cpp"
    break;

  case 479: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2789 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9107 "bison-chpl-lib.cpp"
    break;

  case 480: /* lifetime_ident: TIDENT  */
#line 2793 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9113 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_ident: TINIT  */
#line 2794 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9119 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_ident: TTHIS  */
#line 2795 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9125 "bison-chpl-lib.cpp"
    break;

  case 483: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2800 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9134 "bison-chpl-lib.cpp"
    break;

  case 484: /* type_alias_decl_stmt_start: TTYPE  */
#line 2809 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9142 "bison-chpl-lib.cpp"
    break;

  case 485: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2813 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9151 "bison-chpl-lib.cpp"
    break;

  case 486: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2821 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9159 "bison-chpl-lib.cpp"
    break;

  case 487: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2825 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9167 "bison-chpl-lib.cpp"
    break;

  case 488: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2832 "chpl.ypp"
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
#line 9189 "bison-chpl-lib.cpp"
    break;

  case 489: /* opt_init_type: %empty  */
#line 2852 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9195 "bison-chpl-lib.cpp"
    break;

  case 490: /* opt_init_type: TASSIGN expr  */
#line 2854 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9201 "bison-chpl-lib.cpp"
    break;

  case 491: /* var_decl_type: TPARAM  */
#line 2858 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9207 "bison-chpl-lib.cpp"
    break;

  case 492: /* var_decl_type: TCONST TREF  */
#line 2859 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9213 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_type: TREF  */
#line 2860 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9219 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_type: TCONST  */
#line 2861 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9225 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_type: TVAR  */
#line 2862 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9231 "bison-chpl-lib.cpp"
    break;

  case 496: /* $@11: %empty  */
#line 2867 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9240 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2871 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9249 "bison-chpl-lib.cpp"
    break;

  case 498: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2876 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9258 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_stmt: var_decl_type error TSEMI  */
#line 2881 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = { .comments=nullptr, .stmt=ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release() };
    context->resetDeclStateOnError();
  }
#line 9267 "bison-chpl-lib.cpp"
    break;

  case 500: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2889 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9275 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2893 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9283 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2900 "chpl.ypp"
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
#line 9306 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2919 "chpl.ypp"
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
#line 9326 "bison-chpl-lib.cpp"
    break;

  case 504: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2938 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9334 "bison-chpl-lib.cpp"
    break;

  case 505: /* tuple_var_decl_component: ident_def  */
#line 2942 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9342 "bison-chpl-lib.cpp"
    break;

  case 506: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2946 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9350 "bison-chpl-lib.cpp"
    break;

  case 507: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2953 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9356 "bison-chpl-lib.cpp"
    break;

  case 508: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2955 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9362 "bison-chpl-lib.cpp"
    break;

  case 509: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2957 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9368 "bison-chpl-lib.cpp"
    break;

  case 510: /* opt_init_expr: %empty  */
#line 2963 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9374 "bison-chpl-lib.cpp"
    break;

  case 511: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2964 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9380 "bison-chpl-lib.cpp"
    break;

  case 512: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2965 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9386 "bison-chpl-lib.cpp"
    break;

  case 513: /* formal_or_ret_type_expr: expr  */
#line 2969 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9392 "bison-chpl-lib.cpp"
    break;

  case 514: /* ret_type: formal_or_ret_type_expr  */
#line 2973 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9398 "bison-chpl-lib.cpp"
    break;

  case 515: /* ret_type: reserved_type_ident_use  */
#line 2974 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9404 "bison-chpl-lib.cpp"
    break;

  case 516: /* ret_type: error  */
#line 2975 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9410 "bison-chpl-lib.cpp"
    break;

  case 517: /* colon_ret_type: TCOLON ret_type  */
#line 2979 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9416 "bison-chpl-lib.cpp"
    break;

  case 518: /* colon_ret_type: error  */
#line 2980 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9422 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_ret_type: %empty  */
#line 2984 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9428 "bison-chpl-lib.cpp"
    break;

  case 521: /* opt_type: %empty  */
#line 2989 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9434 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_type: TCOLON expr  */
#line 2990 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9440 "bison-chpl-lib.cpp"
    break;

  case 523: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2991 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9446 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_type: error  */
#line 2992 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9452 "bison-chpl-lib.cpp"
    break;

  case 525: /* formal_type: formal_or_ret_type_expr  */
#line 2996 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9458 "bison-chpl-lib.cpp"
    break;

  case 526: /* formal_type: reserved_type_ident_use  */
#line 2997 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9464 "bison-chpl-lib.cpp"
    break;

  case 527: /* colon_formal_type: TCOLON formal_type  */
#line 3001 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9470 "bison-chpl-lib.cpp"
    break;

  case 528: /* opt_colon_formal_type: %empty  */
#line 3005 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9476 "bison-chpl-lib.cpp"
    break;

  case 529: /* opt_colon_formal_type: colon_formal_type  */
#line 3006 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9482 "bison-chpl-lib.cpp"
    break;

  case 530: /* expr_ls: expr  */
#line 3012 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9488 "bison-chpl-lib.cpp"
    break;

  case 531: /* expr_ls: expr_ls TCOMMA expr  */
#line 3013 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9494 "bison-chpl-lib.cpp"
    break;

  case 532: /* tuple_component: TUNDERSCORE  */
#line 3017 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9500 "bison-chpl-lib.cpp"
    break;

  case 533: /* tuple_component: opt_try_expr  */
#line 3018 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9506 "bison-chpl-lib.cpp"
    break;

  case 534: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 3023 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9514 "bison-chpl-lib.cpp"
    break;

  case 535: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 3027 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9522 "bison-chpl-lib.cpp"
    break;

  case 536: /* opt_actual_ls: %empty  */
#line 3033 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9528 "bison-chpl-lib.cpp"
    break;

  case 537: /* opt_actual_ls: actual_ls  */
#line 3034 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9534 "bison-chpl-lib.cpp"
    break;

  case 538: /* actual_ls: actual_expr  */
#line 3039 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9543 "bison-chpl-lib.cpp"
    break;

  case 539: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3044 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9553 "bison-chpl-lib.cpp"
    break;

  case 540: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3052 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9559 "bison-chpl-lib.cpp"
    break;

  case 541: /* actual_expr: opt_try_expr  */
#line 3053 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9565 "bison-chpl-lib.cpp"
    break;

  case 542: /* ident_expr: ident_use  */
#line 3057 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9571 "bison-chpl-lib.cpp"
    break;

  case 543: /* ident_expr: scalar_type  */
#line 3058 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9577 "bison-chpl-lib.cpp"
    break;

  case 549: /* sub_type_level_expr: TSINGLE expr  */
#line 3071 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9583 "bison-chpl-lib.cpp"
    break;

  case 550: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3073 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9589 "bison-chpl-lib.cpp"
    break;

  case 551: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3075 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9595 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3077 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9601 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 3079 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9611 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TATOMIC expr  */
#line 3085 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9617 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TSYNC expr  */
#line 3087 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9623 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TOWNED  */
#line 3090 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9629 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TOWNED expr  */
#line 3092 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9635 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TUNMANAGED  */
#line 3094 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9641 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3096 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9647 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TSHARED  */
#line 3098 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9653 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TSHARED expr  */
#line 3100 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9659 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TBORROWED  */
#line 3102 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9665 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TBORROWED expr  */
#line 3104 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9671 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TCLASS  */
#line 3106 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9677 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TRECORD  */
#line 3108 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9683 "bison-chpl-lib.cpp"
    break;

  case 566: /* for_expr: no_loop_attributes for_expr_base  */
#line 3112 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9689 "bison-chpl-lib.cpp"
    break;

  case 567: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3117 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9697 "bison-chpl-lib.cpp"
    break;

  case 568: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3121 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9705 "bison-chpl-lib.cpp"
    break;

  case 569: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3125 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-2]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9713 "bison-chpl-lib.cpp"
    break;

  case 570: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3129 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9726 "bison-chpl-lib.cpp"
    break;

  case 571: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3138 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9739 "bison-chpl-lib.cpp"
    break;

  case 572: /* for_expr_base: for_loop_kw expr TDO TIF expr TTHEN expr  */
#line 3147 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-5]), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9752 "bison-chpl-lib.cpp"
    break;

  case 573: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3158 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9758 "bison-chpl-lib.cpp"
    break;

  case 574: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3163 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9766 "bison-chpl-lib.cpp"
    break;

  case 575: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3167 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9774 "bison-chpl-lib.cpp"
    break;

  case 576: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3171 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9782 "bison-chpl-lib.cpp"
    break;

  case 577: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3175 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9790 "bison-chpl-lib.cpp"
    break;

  case 578: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3179 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9798 "bison-chpl-lib.cpp"
    break;

  case 579: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3183 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9806 "bison-chpl-lib.cpp"
    break;

  case 580: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3187 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9814 "bison-chpl-lib.cpp"
    break;

  case 581: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3194 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9828 "bison-chpl-lib.cpp"
    break;

  case 582: /* nil_expr: TNIL  */
#line 3211 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9834 "bison-chpl-lib.cpp"
    break;

  case 590: /* opt_task_intent_ls: %empty  */
#line 3229 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9840 "bison-chpl-lib.cpp"
    break;

  case 591: /* opt_task_intent_ls: task_intent_clause  */
#line 3230 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9846 "bison-chpl-lib.cpp"
    break;

  case 592: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3235 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9854 "bison-chpl-lib.cpp"
    break;

  case 593: /* task_intent_ls: intent_expr  */
#line 3241 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9860 "bison-chpl-lib.cpp"
    break;

  case 594: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3242 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9866 "bison-chpl-lib.cpp"
    break;

  case 595: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3247 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 9876 "bison-chpl-lib.cpp"
    break;

  case 596: /* forall_intent_ls: intent_expr  */
#line 3255 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9882 "bison-chpl-lib.cpp"
    break;

  case 597: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3256 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9888 "bison-chpl-lib.cpp"
    break;

  case 598: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3261 "chpl.ypp"
  {
    if (auto ident = (yyvsp[-2].expr)->toIdentifier()) {
      auto name = ident->name();
      auto node = TaskVar::build(BUILDER, LOC((yyloc)), /*attributeGroup*/ nullptr,
                                 name,
                                 /*intent*/ (yyvsp[-3].taskIntent),
                                 toOwned((yyvsp[-1].expr)),
                                 toOwned((yyvsp[0].expr)));
      BUILDER->noteDeclNameLocation(node.get(), LOC((yylsp[-2])));
      (yyval.expr) = node.release();
    } else {
      (yyval.expr) = context->syntax((yyloc), "expected identifier for task variable name.");
    }
  }
#line 9907 "bison-chpl-lib.cpp"
    break;

  case 599: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3276 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9915 "bison-chpl-lib.cpp"
    break;

  case 600: /* intent_expr: expr TREDUCE ident_expr  */
#line 3280 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9923 "bison-chpl-lib.cpp"
    break;

  case 601: /* task_var_prefix: TCONST  */
#line 3286 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 9929 "bison-chpl-lib.cpp"
    break;

  case 602: /* task_var_prefix: TIN  */
#line 3287 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 9935 "bison-chpl-lib.cpp"
    break;

  case 603: /* task_var_prefix: TCONST TIN  */
#line 3288 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 9941 "bison-chpl-lib.cpp"
    break;

  case 604: /* task_var_prefix: TREF  */
#line 3289 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 9947 "bison-chpl-lib.cpp"
    break;

  case 605: /* task_var_prefix: TCONST TREF  */
#line 3290 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 9953 "bison-chpl-lib.cpp"
    break;

  case 606: /* task_var_prefix: TVAR  */
#line 3291 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 9959 "bison-chpl-lib.cpp"
    break;

  case 607: /* new_maybe_decorated: TNEW  */
#line 3296 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 9965 "bison-chpl-lib.cpp"
    break;

  case 608: /* new_maybe_decorated: TNEW TOWNED  */
#line 3298 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 9971 "bison-chpl-lib.cpp"
    break;

  case 609: /* new_maybe_decorated: TNEW TSHARED  */
#line 3300 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 9977 "bison-chpl-lib.cpp"
    break;

  case 610: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3302 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 9983 "bison-chpl-lib.cpp"
    break;

  case 611: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3304 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 9989 "bison-chpl-lib.cpp"
    break;

  case 612: /* new_expr: new_maybe_decorated expr  */
#line 3310 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 9997 "bison-chpl-lib.cpp"
    break;

  case 613: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3317 "chpl.ypp"
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
#line 10013 "bison-chpl-lib.cpp"
    break;

  case 614: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3329 "chpl.ypp"
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
#line 10029 "bison-chpl-lib.cpp"
    break;

  case 615: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3341 "chpl.ypp"
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
#line 10046 "bison-chpl-lib.cpp"
    break;

  case 616: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3354 "chpl.ypp"
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
#line 10064 "bison-chpl-lib.cpp"
    break;

  case 617: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3371 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10072 "bison-chpl-lib.cpp"
    break;

  case 618: /* range_literal_expr: expr TDOTDOT expr  */
#line 3378 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10081 "bison-chpl-lib.cpp"
    break;

  case 619: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3383 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10090 "bison-chpl-lib.cpp"
    break;

  case 620: /* range_literal_expr: expr TDOTDOT  */
#line 3388 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10099 "bison-chpl-lib.cpp"
    break;

  case 621: /* range_literal_expr: TDOTDOT expr  */
#line 3393 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10109 "bison-chpl-lib.cpp"
    break;

  case 622: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3399 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10119 "bison-chpl-lib.cpp"
    break;

  case 623: /* range_literal_expr: TDOTDOT  */
#line 3405 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10129 "bison-chpl-lib.cpp"
    break;

  case 624: /* cast_expr: expr TCOLON expr  */
#line 3435 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10137 "bison-chpl-lib.cpp"
    break;

  case 625: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3442 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10145 "bison-chpl-lib.cpp"
    break;

  case 626: /* super_expr: fn_expr  */
#line 3448 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10151 "bison-chpl-lib.cpp"
    break;

  case 630: /* expr: sub_type_level_expr TQUESTION  */
#line 3457 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10157 "bison-chpl-lib.cpp"
    break;

  case 631: /* expr: TQUESTION  */
#line 3459 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10163 "bison-chpl-lib.cpp"
    break;

  case 635: /* expr: fn_type  */
#line 3464 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10169 "bison-chpl-lib.cpp"
    break;

  case 645: /* opt_expr: %empty  */
#line 3478 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10175 "bison-chpl-lib.cpp"
    break;

  case 646: /* opt_expr: expr  */
#line 3479 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10181 "bison-chpl-lib.cpp"
    break;

  case 647: /* opt_try_expr: TTRY expr  */
#line 3483 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10187 "bison-chpl-lib.cpp"
    break;

  case 648: /* opt_try_expr: TTRYBANG expr  */
#line 3484 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10193 "bison-chpl-lib.cpp"
    break;

  case 649: /* opt_try_expr: super_expr  */
#line 3485 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10199 "bison-chpl-lib.cpp"
    break;

  case 655: /* call_base_expr: expr TBANG  */
#line 3502 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10207 "bison-chpl-lib.cpp"
    break;

  case 656: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3505 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10213 "bison-chpl-lib.cpp"
    break;

  case 659: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3512 "chpl.ypp"
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
#line 10229 "bison-chpl-lib.cpp"
    break;

  case 660: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3524 "chpl.ypp"
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
#line 10245 "bison-chpl-lib.cpp"
    break;

  case 661: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3536 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10253 "bison-chpl-lib.cpp"
    break;

  case 662: /* dot_expr: expr TDOT ident_use  */
#line 3543 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10259 "bison-chpl-lib.cpp"
    break;

  case 663: /* dot_expr: expr TDOT TTYPE  */
#line 3545 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10265 "bison-chpl-lib.cpp"
    break;

  case 664: /* dot_expr: expr TDOT TDOMAIN  */
#line 3547 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10271 "bison-chpl-lib.cpp"
    break;

  case 665: /* dot_expr: expr TDOT TLOCALE  */
#line 3549 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10277 "bison-chpl-lib.cpp"
    break;

  case 666: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3551 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10283 "bison-chpl-lib.cpp"
    break;

  case 667: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3553 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10289 "bison-chpl-lib.cpp"
    break;

  case 668: /* parenthesized_expr: TLP opt_try_expr TRP  */
#line 3562 "chpl.ypp"
  {
    // Use 'tryNote' here in case an expression like ((x)) comes along.
    BUILDER->tryNoteExprParenthLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 10299 "bison-chpl-lib.cpp"
    break;

  case 669: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3568 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10307 "bison-chpl-lib.cpp"
    break;

  case 670: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3572 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10315 "bison-chpl-lib.cpp"
    break;

  case 671: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3576 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10323 "bison-chpl-lib.cpp"
    break;

  case 672: /* bool_literal: TFALSE  */
#line 3582 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10329 "bison-chpl-lib.cpp"
    break;

  case 673: /* bool_literal: TTRUE  */
#line 3583 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10335 "bison-chpl-lib.cpp"
    break;

  case 674: /* str_bytes_literal: STRINGLITERAL  */
#line 3587 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10341 "bison-chpl-lib.cpp"
    break;

  case 675: /* str_bytes_literal: BYTESLITERAL  */
#line 3588 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10347 "bison-chpl-lib.cpp"
    break;

  case 678: /* literal_expr: INTLITERAL  */
#line 3594 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10353 "bison-chpl-lib.cpp"
    break;

  case 679: /* literal_expr: REALLITERAL  */
#line 3595 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10359 "bison-chpl-lib.cpp"
    break;

  case 680: /* literal_expr: IMAGLITERAL  */
#line 3596 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10365 "bison-chpl-lib.cpp"
    break;

  case 681: /* literal_expr: CSTRINGLITERAL  */
#line 3597 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10371 "bison-chpl-lib.cpp"
    break;

  case 682: /* literal_expr: TNONE  */
#line 3598 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10377 "bison-chpl-lib.cpp"
    break;

  case 683: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3600 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10386 "bison-chpl-lib.cpp"
    break;

  case 684: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3605 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10395 "bison-chpl-lib.cpp"
    break;

  case 685: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3610 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10404 "bison-chpl-lib.cpp"
    break;

  case 686: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3615 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10413 "bison-chpl-lib.cpp"
    break;

  case 687: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3620 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10422 "bison-chpl-lib.cpp"
    break;

  case 688: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3625 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10431 "bison-chpl-lib.cpp"
    break;

  case 689: /* assoc_expr_ls: expr TALIAS expr  */
#line 3634 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10440 "bison-chpl-lib.cpp"
    break;

  case 690: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3639 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10450 "bison-chpl-lib.cpp"
    break;

  case 691: /* binary_op_expr: expr TPLUS expr  */
#line 3647 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10456 "bison-chpl-lib.cpp"
    break;

  case 692: /* binary_op_expr: expr TMINUS expr  */
#line 3648 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10462 "bison-chpl-lib.cpp"
    break;

  case 693: /* binary_op_expr: expr TSTAR expr  */
#line 3649 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10468 "bison-chpl-lib.cpp"
    break;

  case 694: /* binary_op_expr: expr TDIVIDE expr  */
#line 3650 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10474 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3651 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10480 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3652 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10486 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TMOD expr  */
#line 3653 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10492 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TEQUAL expr  */
#line 3654 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10498 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3655 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10504 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3656 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10510 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3657 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10516 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TLESS expr  */
#line 3658 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10522 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TGREATER expr  */
#line 3659 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10528 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TBAND expr  */
#line 3660 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10534 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TBOR expr  */
#line 3661 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10540 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TBXOR expr  */
#line 3662 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10546 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TAND expr  */
#line 3663 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10552 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TOR expr  */
#line 3664 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10558 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TEXP expr  */
#line 3665 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10564 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TBY expr  */
#line 3666 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10570 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TALIGN expr  */
#line 3667 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10576 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr THASH expr  */
#line 3668 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10582 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TDMAPPED expr  */
#line 3669 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10588 "bison-chpl-lib.cpp"
    break;

  case 714: /* unary_op_expr: TPLUS expr  */
#line 3673 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10594 "bison-chpl-lib.cpp"
    break;

  case 715: /* unary_op_expr: TMINUS expr  */
#line 3674 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10600 "bison-chpl-lib.cpp"
    break;

  case 716: /* unary_op_expr: TMINUSMINUS expr  */
#line 3675 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10606 "bison-chpl-lib.cpp"
    break;

  case 717: /* unary_op_expr: TPLUSPLUS expr  */
#line 3676 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10612 "bison-chpl-lib.cpp"
    break;

  case 718: /* unary_op_expr: TBANG expr  */
#line 3677 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10618 "bison-chpl-lib.cpp"
    break;

  case 719: /* unary_op_expr: expr TBANG  */
#line 3678 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10626 "bison-chpl-lib.cpp"
    break;

  case 720: /* unary_op_expr: TBNOT expr  */
#line 3681 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10632 "bison-chpl-lib.cpp"
    break;

  case 721: /* reduce_expr: expr TREDUCE expr  */
#line 3686 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10640 "bison-chpl-lib.cpp"
    break;

  case 722: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3690 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10648 "bison-chpl-lib.cpp"
    break;

  case 723: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3694 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10656 "bison-chpl-lib.cpp"
    break;

  case 724: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3698 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10664 "bison-chpl-lib.cpp"
    break;

  case 725: /* scan_expr: expr TSCAN expr  */
#line 3705 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10672 "bison-chpl-lib.cpp"
    break;

  case 726: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3709 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10680 "bison-chpl-lib.cpp"
    break;

  case 727: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3713 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10688 "bison-chpl-lib.cpp"
    break;

  case 728: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3717 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10696 "bison-chpl-lib.cpp"
    break;


#line 10700 "bison-chpl-lib.cpp"

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
