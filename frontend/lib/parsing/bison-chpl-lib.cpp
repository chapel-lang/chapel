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
#define YYLAST   18220

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
       0,   635,   635,   639,   640,   645,   646,   654,   658,   668,
     673,   680,   681,   682,   683,   684,   685,   686,   687,   688,
     689,   690,   704,   705,   706,   714,   723,   732,   736,   743,
     751,   752,   753,   754,   755,   756,   757,   758,   760,   776,
     784,   797,   809,   822,   835,   847,   864,   865,   869,   879,
     880,   884,   888,   889,   893,   900,   908,   911,   921,   923,
     928,   934,   939,   948,   952,   973,   990,  1007,  1025,  1026,
    1028,  1033,  1034,  1039,  1057,  1061,  1065,  1076,  1095,  1096,
    1100,  1104,  1108,  1113,  1117,  1121,  1131,  1136,  1142,  1149,
    1154,  1161,  1172,  1173,  1177,  1182,  1190,  1199,  1209,  1217,
    1226,  1239,  1247,  1248,  1249,  1250,  1251,  1252,  1253,  1254,
    1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,  1275,
    1279,  1284,  1289,  1295,  1304,  1305,  1309,  1318,  1322,  1326,
    1330,  1334,  1338,  1347,  1348,  1349,  1353,  1354,  1355,  1356,
    1357,  1361,  1362,  1363,  1375,  1376,  1377,  1382,  1383,  1384,
    1385,  1386,  1387,  1388,  1389,  1390,  1391,  1392,  1393,  1394,
    1395,  1396,  1397,  1398,  1399,  1400,  1404,  1405,  1406,  1407,
    1408,  1409,  1410,  1411,  1412,  1413,  1414,  1415,  1422,  1423,
    1424,  1425,  1429,  1430,  1434,  1441,  1451,  1455,  1461,  1461,
    1467,  1467,  1476,  1477,  1478,  1479,  1480,  1481,  1482,  1486,
    1491,  1496,  1501,  1508,  1516,  1517,  1521,  1530,  1537,  1546,
    1565,  1564,  1574,  1573,  1586,  1592,  1595,  1596,  1605,  1606,
    1607,  1611,  1630,  1645,  1660,  1665,  1670,  1675,  1696,  1701,
    1706,  1711,  1716,  1721,  1726,  1731,  1736,  1741,  1746,  1751,
    1756,  1761,  1766,  1772,  1781,  1790,  1794,  1798,  1802,  1806,
    1810,  1814,  1818,  1822,  1827,  1832,  1837,  1845,  1860,  1878,
    1882,  1889,  1890,  1895,  1900,  1901,  1902,  1903,  1904,  1905,
    1906,  1907,  1908,  1909,  1910,  1911,  1916,  1921,  1922,  1923,
    1924,  1933,  1934,  1938,  1942,  1946,  1953,  1957,  1961,  1968,
    1972,  1976,  1980,  1987,  1988,  1992,  1996,  2000,  2007,  2020,
    2036,  2044,  2048,  2057,  2058,  2062,  2066,  2071,  2080,  2085,
    2089,  2096,  2097,  2101,  2111,  2117,  2129,  2136,  2137,  2138,
    2142,  2143,  2147,  2151,  2155,  2159,  2163,  2171,  2190,  2203,
    2210,  2215,  2222,  2221,  2231,  2237,  2236,  2251,  2258,  2270,
    2288,  2285,  2313,  2317,  2318,  2320,  2325,  2326,  2330,  2331,
    2335,  2338,  2340,  2345,  2346,  2357,  2377,  2376,  2392,  2391,
    2409,  2419,  2416,  2443,  2451,  2459,  2470,  2481,  2490,  2505,
    2506,  2510,  2511,  2512,  2521,  2522,  2523,  2524,  2525,  2526,
    2527,  2528,  2529,  2530,  2531,  2532,  2533,  2534,  2535,  2536,
    2537,  2538,  2539,  2540,  2541,  2542,  2543,  2544,  2545,  2549,
    2550,  2551,  2552,  2553,  2554,  2555,  2556,  2557,  2558,  2559,
    2560,  2565,  2566,  2570,  2571,  2575,  2576,  2580,  2581,  2585,
    2586,  2590,  2591,  2595,  2599,  2600,  2604,  2608,  2613,  2618,
    2623,  2628,  2636,  2640,  2648,  2649,  2650,  2651,  2652,  2653,
    2654,  2655,  2656,  2660,  2661,  2662,  2663,  2664,  2665,  2669,
    2670,  2671,  2675,  2676,  2677,  2678,  2679,  2680,  2681,  2685,
    2686,  2689,  2690,  2694,  2695,  2699,  2704,  2705,  2707,  2709,
    2711,  2716,  2718,  2723,  2725,  2727,  2729,  2731,  2733,  2735,
    2740,  2741,  2742,  2746,  2755,  2759,  2767,  2771,  2778,  2799,
    2800,  2805,  2806,  2807,  2808,  2809,  2814,  2813,  2822,  2827,
    2835,  2839,  2846,  2865,  2884,  2888,  2892,  2899,  2901,  2903,
    2910,  2911,  2912,  2916,  2920,  2921,  2922,  2926,  2927,  2931,
    2932,  2936,  2937,  2938,  2939,  2943,  2944,  2948,  2952,  2953,
    2959,  2960,  2964,  2965,  2969,  2973,  2980,  2981,  2985,  2990,
    2999,  3000,  3004,  3005,  3012,  3013,  3014,  3015,  3016,  3017,
    3019,  3021,  3023,  3025,  3031,  3033,  3036,  3038,  3040,  3042,
    3044,  3046,  3048,  3050,  3052,  3054,  3059,  3063,  3067,  3071,
    3075,  3084,  3093,  3105,  3109,  3113,  3117,  3121,  3125,  3129,
    3133,  3140,  3158,  3166,  3167,  3168,  3169,  3170,  3171,  3172,
    3176,  3177,  3181,  3188,  3189,  3193,  3202,  3203,  3207,  3222,
    3226,  3233,  3234,  3235,  3236,  3237,  3238,  3242,  3244,  3246,
    3248,  3250,  3256,  3263,  3275,  3287,  3300,  3317,  3324,  3329,
    3334,  3339,  3345,  3351,  3381,  3388,  3395,  3396,  3400,  3402,
    3403,  3405,  3407,  3408,  3409,  3410,  3413,  3414,  3415,  3416,
    3417,  3418,  3419,  3420,  3421,  3425,  3426,  3430,  3431,  3432,
    3436,  3437,  3438,  3439,  3448,  3449,  3452,  3453,  3454,  3458,
    3470,  3482,  3489,  3491,  3493,  3495,  3497,  3499,  3508,  3514,
    3518,  3522,  3529,  3530,  3534,  3535,  3539,  3540,  3541,  3542,
    3543,  3544,  3545,  3546,  3551,  3556,  3561,  3566,  3571,  3580,
    3585,  3594,  3595,  3596,  3597,  3598,  3599,  3600,  3601,  3602,
    3603,  3604,  3605,  3606,  3607,  3608,  3609,  3610,  3611,  3612,
    3613,  3614,  3615,  3616,  3620,  3621,  3622,  3623,  3624,  3625,
    3628,  3632,  3636,  3640,  3644,  3651,  3655,  3659,  3663,  3671,
    3672,  3673,  3674,  3675,  3676,  3677
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

#define YYPACT_NINF (-892)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-736)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -892,    64,  3799,  -892,     3,   166,  -892,  -892,  -892,  -892,
    -892,  -892,  -892, 12071,   106,    84,   198,  3044,   211, 17991,
     106,   214,   177,   201,    84,  5207, 12071,   151, 18047,  -892,
     318,   281,  -892,  9783,   301,    72,   197,  -892,   339,   304,
   18047, 18047, 18047,  -892, 14037, 10663,   393, 12071, 12071,    37,
    -892,   436,   437, 12071,  -892,  3044,  -892, 12071,   429,   365,
     267,   360, 13275,   506, 18103,  -892, 12071,  8375, 12071, 10663,
    3044, 12071,   430,   525,   416,  5207,   539, 12071,   542,  6791,
    6791,  -892,   556,  -892,  3044,  -892,   563,  9959,  -892,   636,
   12071,  -892, 12071,  -892,  -892, 13758, 12071,  -892, 10135,  -892,
    -892,  -892,  4151,  7495, 12071,  -892,  4855,  -892,  -892,  -892,
    -892, 17469,   501,  -892,   499,   487,  -892,   132,  -892,  -892,
    -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,
    -892,  6967,  -892,  7143,  -892,  -892,    99,  -892,  -892,  -892,
     603,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,   513,
   18047,  -892,   492,   494,    89,  -892,   246,  -892,  -892,  -892,
     264,   319,  -892,  -892, 18047,  1686,  -892,   502,   500,  -892,
    -892,  -892,   503,   504, 12071,   505,   508,  -892,  -892,  -892,
   17280,   654,   219,   509,   511,  -892,  -892,   368,  -892,  -892,
    -892,  -892,  -892,     5,  -892,  -892,  -892, 12071,  -892, 18047,
    -892, 12071, 12071,    51,   622,   413,  -892,  -892,  -892,  -892,
   17280,   445,  -892,  -892,   507,  5207,  -892,  -892,  -892,   515,
     -32,   512,  -892,   257,  -892,   516,  -892,     8, 17280,  8551,
    -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,
    -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,
    -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892, 18047,
   18047,   -34,  3612,  -892,  -892,   594,  8551,   518,   514,   440,
    5207,  1936,   110,     6,  -892,  5207,  -892,  -892, 15284,  -892,
      24, 15724,   551,  -892,   517,   519,  -892, 15284,   -32,   551,
    -892,  8551,  1631,  1631,    33,    14,  -892,    38,  -892,  -892,
    -892,  -892,  7671,  7671,  -892,   395,  -892,   522,   538,  -892,
    -892,  2309,   541,  -892,  -892, 17280,   239, 12071, 12071,  -892,
     -27,  -892,  -892, 17280,   523, 15845,  -892, 15284,   -32, 17280,
     521,  8551,  -892, 17280, 15885,  -892,  -892,  -892,  -892,  -892,
     -32,   533,  -892,   200,   200,  1313,   551,   551,   227,  -892,
    -892,  4327,   179,  -892, 12071,    92,   115,  -892,  1313,   683,
    -892,  -892, 18047,  -892,  -892,  4151,  -892,   139,  -892,   594,
    -892,  -892,   700,   531,  4503, 12071,  -892, 12071,   642,  -892,
    -892, 14955,   226,   513, 17280,   370, 12071,  5207,   630,  -892,
    -892,  9783, 10311, 10839,  -892,  -892,  -892, 18047, 12071,   540,
    -892, 17767,   202,   264,  -892,  -892,  -892,  -892,   619,  -892,
    -892,  -892,  -892,    85,   623,  -892,  -892,  -892, 13587,   589,
     282,  -892,   550,   347,   462,  -892,   567, 12071, 12071, 12071,
   10839, 10839, 12071,   475, 12071, 12071, 12071, 12071, 12071,   536,
   13758, 12071, 12071, 12071, 12071, 12071, 12071, 12071, 12071, 12071,
   12071, 12071, 12071, 12071, 12071, 12071, 12071,   649,  -892,  -892,
    -892,  -892,  -892,  9959,  9959,  -892,  -892,  -892,  -892,  9959,
    -892,  -892,  9959,  9959,  8551,  8727, 10839, 10839, 15930,   548,
    -892, 11015, 12071, 18047,  7847,  -892,  -892,  -892, 14037,  -892,
   12071,  -892,   597,   553,   576,  -892,  -892,   599,   606,  5207,
     695,  5207,  -892,   699, 12071,   668,   564,  -892,  9959, 18047,
    -892,  -892,  -892,  1936,  -892,  -892,   129,  -892, 12247,   615,
   12071, 14037,  -892,  -892, 12071,  -892, 17879, 12071, 12071,  -892,
     569,  -892,  -892,  -892,  -892, 17935,  -892,   264, 13450,  -892,
    1936,  -892,   598, 12247,  -892, 17280, 17280,  -892,   512,  -892,
      44,  -892,  9959,   570,  -892,   726,   726,  -892,  -892,  -892,
    -892, 11191,  -892, 16006,  8023,  -892,  8199,  -892,  -892, 10839,
   10839,  -892,   499,  8903,  -892,   316,  -892,  4679,  -892,   364,
   16047,   369, 15047, 18047,  6615,  6439,  -892,   513,   577,  -892,
     257,  -892, 15369,   632, 18047,    29, 15284,   581, 13931,   -54,
     112, 16093,   -73,    22, 14787,   266,   602,  1839,   583, 17823,
     634,   190,  -892,  -892,   147,  -892,  -892,    47,  -892,  -892,
    6263,  -892,   650,  -892,   592,   612,  -892,   616,  -892,   617,
     620,   621,   625,   626,  -892,   628,   629,   645,   646,   647,
     648,   652,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,
    -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,
    -892,  -892,  -892,  -892, 12071,   627,   653,   592,  -892,   592,
    -892, 12071,  -892, 18047,  -892,  -892,  -892, 17446, 17446,   474,
    -892,   474,  -892,   474, 17526,  1150,  2098,   732,   -32,   200,
    -892,   482,  -892,  -892,  -892,  -892,  -892,  1313, 15002,   474,
    1422,  1422, 17446,  1422,  1422,  2066,   200, 15002, 17496,  2066,
     551,   551,   200,  1313,   633,   638,   639,   641,   643,   655,
     640,   596,  -892,   474,  -892,   474, 12071,  9959,    25, 15445,
     635,   143,  -892,  -892,  -892,   161,  -892,  2434, 17325,   391,
     376, 17280,  9959,  -892,  9959, 12071, 12071,   778,  5207,   779,
   16200,  5207, 15486, 18047,  -892,   184,  -892,   186,  -892,   233,
    1936,   110,  -892, 17280,  9079,  -892, 17280,  -892,  -892,  -892,
   18047, 16246, 16286,  -892,   598,   669,  -892,   269, 12247,  -892,
     289,  -892, 12071,  -892,   656,    34,   644,  -892,  2544,  -892,
    -892,  -892,  -892,  -892,  -892,  -892,   183, 14863,  -892,  -892,
     288,  -892,  -892,  -892,  -892, 12071,  -892,    60, 14140, 12071,
    -892, 11367,  6615,  6439,  -892,  9255,   461,  9431,   463,   520,
    9607,  7319,   537,   378,  -892,  -892,  -892, 16439,   671,   657,
     659,  -892, 18047, 14037, 10839,    29, 12071,   770,  -892,  -892,
   12071, 17280,   660,  5207,   658, 10839, 11543,  3975,   661, 12071,
   11719,  -892,  -892,    29,  5383, 10839,  -892,    29,  -892,  9959,
     663,  2053,  -892,  -892, 12071,    80,  -892,   575,  -892,   650,
    -892,  -892,  -892,  -892,  -892,   -12,   374,  -892, 16479,  -892,
   14300,  -892,  -892, 17280,  -892,   662,   664,  -892,  -892,  -892,
    -892,  -892,  -892,  -892,  -892,   292, 10839, 13931, 12423, 10839,
    9959,  -892,  -892,  7847,  -892,  -892,  -892,  -892,  -892,  -892,
    -892,  -892,  -892,  -892,  -892,  -892,    35, 10839, 10839,  -892,
    -892,  -892, 17280, 17280,  5207,  -892,  5207, 12071,  -892,  5207,
     786,   666,   672, 18047,   512,  -892,  -892,   615,  -892,  -892,
    -892,   670,   673,   289, 12071,   650,   598,  -892, 10487,  -892,
    -892, 17280,  -892,  5207, 12071,  -892,  -892,  -892, 18047,  -892,
     678,   512, 10839,  5207, 10839,  9959,  -892,  -892,  -892,  -892,
    -892,   482,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,
    -892,  -892,  -892,  -892,  -892,  -892,  6615,  -892,  -892, 16519,
      40, 15639,   406,   676,  6615,  -892, 12071,  -892,  -892,  2899,
     410,   417,   106, 16600,  -892, 16640, 12071,   330,  7847,  -892,
    5207,   -40, 15531,  -892,  -892, 17280,  5207, 17280,  -892, 16686,
    -892,  9783, 17280,    27, 15123,  -892,   341,  -892,  -892,  -892,
    -892, 17670, 17688,  -892, 17280, 18047,   501,  -892,   -12,   142,
   12071,  -892,   691,   674,  -892,   592,   592,  -892,  -892,  -892,
     675, 16793, 12071,   812, 16839,   346,  -892,   615,   348,   358,
    -892,  -892, 17280,   813,  5207,  -892,  -892,  -892,  -892,  -892,
    8551,  8551,  -892,  -892,  -892,  -892,   289, 17280,  -892,   276,
      28,   681, 12071,  -892,    -1, 15684,    22, 15208,  -892,   208,
   11895,  6615,  6439,  6615,  6439,  -892,  -892,  -892, 16880, 17280,
    3266,  -892,  -892,  -892,    29,    29,  -892, 15284,  -892,   362,
    -892,  -892,  5559,   680,  5735,   682,  -892, 12071, 14382,  5911,
    -892,    29,  6087,  -892,    29,   684, 17688,  -892,  -892,  -892,
   17823,  -892,  -892,  -892,    93,  -892,   131,  -892,   438, 16960,
      18,  -892,  -892,  -892, 12599, 12775, 17001, 12951, 13127,  -892,
    -892,  5207,  -892,   685,   686,  -892,  -892,  5207,  -892,   512,
   17280,  5207,  5207,  -892,  -892, 17280,   418,   696,   419,   701,
   12071,  -892,  -892,  -892,  -892,  7847,  -892,  9783,  -892, 17280,
    5207,  9783,  -892, 17280,  5207, 17280,  5207,  9783, 17280,  -892,
    9783, 17280,  -892,  -892,  -892,  -892,    36,  -892, 12071,   142,
      93,    93,    93,    93,    93,    93,   142,  5031,  -892,  -892,
    -892,  -892, 12071, 12071, 12071, 12071, 12071,  -892,   703,   705,
     632,  -892,  -892,  -892,  -892,  -892, 17280,  -892, 14462,  -892,
   14542,  -892, 16200, 14622, 14702,  -892, 17280,  -892,  -892,  -892,
    -892,  -892,  -892,  -892,   713,  -892, 17077, 17118, 17163, 17239,
    -892,  -892,  5207,  5207,  5207,  5207, 12071, 12071, 12071, 12071,
   16200, 16200, 16200, 16200
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
      79,   215,   683,     0,   215,   668,   215,   670,     8,   215,
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
     684,   625,   669,   534,   671,   535,     0,     0,    57,    58,
       0,    61,    63,    67,    66,   215,   101,     0,     0,   215,
      94,   215,   215,   215,    64,   215,   374,   215,   375,   376,
     215,   215,   386,     0,   411,   412,    81,    80,    93,     0,
       0,   322,     0,     0,   215,     0,   215,     0,   223,   222,
     215,   575,     0,     0,     0,   215,   215,     0,     0,   215,
     215,   687,   234,     0,     0,   215,   232,     0,   284,   215,
       0,   342,   328,   334,   215,   332,   327,   432,   418,   459,
     516,   515,   514,   517,   460,   466,   432,   367,     0,   373,
       0,   363,   364,   490,   487,     0,     0,   132,   130,   131,
     129,   128,   127,   659,   660,     0,   215,   685,   215,   215,
     215,   603,   605,   215,   592,   166,   173,   172,   171,   168,
     175,   176,   170,   174,   169,   177,     0,   215,   215,   497,
     540,   539,   258,   257,     0,   252,     0,   215,   248,     0,
     254,     0,   286,     0,     0,   506,   509,   510,   511,   512,
     308,     0,     0,   510,   215,   459,   528,   527,   215,   428,
     426,   359,   302,     0,   215,   301,   304,   553,     0,   295,
     298,     0,   215,     0,   215,   215,    59,   125,   122,   102,
     114,   108,   107,   106,   116,   103,   117,   112,   105,   113,
     111,   109,   110,   104,   115,   118,   215,   121,   120,    89,
      88,    87,     0,     0,   215,   200,   215,   201,   322,   342,
       0,     0,   590,   590,   226,     0,   215,     0,   215,   242,
       0,     0,     0,   686,   240,   576,     0,   689,   688,     0,
     235,   215,   569,     0,     0,   233,     0,   315,   190,   188,
     314,   342,   342,   323,   338,     0,     0,   420,   466,     0,
     215,   361,   422,     0,   370,   415,   415,   666,   667,   286,
       0,     0,   215,     0,     0,     0,   594,   510,   650,   650,
     251,   247,   581,   253,     0,    73,   283,   262,   259,   503,
     215,   215,   429,   427,   354,   355,   510,   414,   306,     0,
       0,     0,   215,   296,     0,     0,     0,     0,    62,     0,
     215,   215,   215,   215,   215,    95,    98,    84,    83,    82,
     342,   207,   213,   211,     0,     0,   221,     0,   244,     0,
     596,   243,     0,     0,     0,     0,   241,   215,     0,     0,
     230,     0,     0,   228,     0,   288,   342,   325,   324,   333,
       0,   340,   480,   481,     0,   482,   468,   471,     0,   467,
       0,   416,   365,   366,   215,   215,     0,   215,   215,   288,
     598,     0,   256,     0,     0,   431,   430,     0,   305,     0,
     299,     0,     0,   123,    91,    90,     0,     0,     0,     0,
     215,   208,   225,   224,   227,   215,   595,   215,   238,   578,
       0,   215,   236,   577,     0,   690,     0,   215,   568,   231,
     215,   567,   229,   285,   326,   336,     0,   479,   215,     0,
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
    -892,  -892,  -892,     2,   -61,  2543,  -892,   381,   754,  -892,
    -892,   422,  -398,  -102,  -892,   299,  -892,  -892,   -88,  -892,
    -892,    31,   618,  -892,  -535,  2350,   510,  -575,  -892,  -787,
    -892,  -892,  -892,    82,  -892,  -892,  -892,   864,  -892,  1655,
    -164,  -892,  -892,  -471,   345,  -892,  -829,  -892,  -892,   337,
    -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,    10,  -892,
    -159,   524,  -347,  -892,   526,  -892,  -892,   -44,  1521,  -892,
    -892,  -892,  -892,   555,  -892,   -68,  -892,  -892,  -892,  -892,
     372,  -892,  -892,  -892,  -100,  -892,  -342,  -793,  -892,  -892,
    -892,  -892,  -892,  -601,  -892,    26,  -892,  -892,  -892,  -892,
    -892,   354,  -892,    -5,  -892,  -892,  -892,  -892,   527,  -892,
    -892,  -892,  -892,    17,  -407,  -174,  -774,  -891,  -642,  -892,
      32,  -892,    42,   363,   -48,   600,   -47,  -892,  -892,  -360,
    -795,  -892,  -294,  -892,  -135,  -302,  -289,  -836,  -892,  -892,
      81,   245,  -892,  -204,   788,  -892,  -160,   399,   165,  -380,
    -751,  -599,  -892,  -892,  -892,  -593,  -506,  -892,  -734,    -9,
      70,  -892,  -260,  -488,  -534,    -2,  -892,  -892,  -892,  -892,
    -892,  -892,   163,  -892,   -18,  -892,  -892,  -478,  -892,  -873,
    -892,  -892,   377,   498,  -892,  -892,  -892,  -892,  2423,  -892,
     -45,   651,  -892,  1021,  1139,  -892,  -892,  -892,  -892,  -892,
    -892,  -892,  -892,  -892,  -468
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   350,   106,   107,   108,   109,   110,   367,
     368,   111,   112,   113,   114,   373,   574,   800,   801,   115,
     116,   117,   378,   118,   119,   120,   351,   828,   581,   829,
     121,   122,   987,   578,   579,   123,   124,   219,   255,   272,
     125,   257,   126,   306,   856,   127,   128,   293,   292,   129,
     130,   131,   132,   133,   134,   590,   832,   135,   203,   137,
     393,   394,   599,   138,   261,   139,   757,   758,   204,   141,
     142,   143,   144,   555,   789,   961,   145,   146,   785,   956,
     279,   280,   147,   148,   149,   150,   399,   861,   151,   152,
     611,  1035,  1036,   612,   153,   205,  1196,   155,   156,   307,
     308,   309,   945,   157,   321,   548,   782,   158,   159,  1140,
     160,   161,   666,   667,   824,   825,   826,   949,   877,   403,
     614,  1043,   615,   542,   616,   311,   536,   418,   407,   413,
     875,  1210,  1211,   162,  1041,  1136,  1137,  1138,   163,   164,
     420,   421,   672,   165,   166,   223,   273,   274,   515,   516,
     765,   313,   873,   621,   622,   519,   314,   779,   780,   352,
     355,   356,   493,   494,   495,   206,   168,   169,   395,   170,
     396,   171,   207,   173,   215,   216,   735,   848,  1109,   736,
     737,   174,   208,   209,   177,   178,   179,   322,   180,   385,
     496,   211,   182,   212,   213,   185,   186,   187,   188,   602,
     189,   190,   191,   192,   193
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   305,   221,   609,   105,   423,   506,   473,   863,   823,
     371,   668,   136,   783,   310,   312,   739,   227,   786,   488,
     755,   872,   324,   167,   869,   881,   993,   882,   154,   950,
    1056,   530,  1033,   383,   988,   136,   517,   781,   999,   802,
     943,   588,   341,   617,   482,   784,   603,   762,   517,  1039,
    1207,   154,  1072,   520,   275,   283,   275,   316,   357,  1119,
     275,   275,   320,   690,     3,   492,   265,   842,  1207,   499,
    -281,   553,   896,   167,  1038,   850,  1091,   167,   167,  -521,
     533,   842,   320,   680,   682,   136,   619,   217,   504,   136,
     136,   480,   389,   390,  -282,   476,  1132,   547,   320,   477,
     167,   154,   492,   851,   167,   154,   154,  1040,  -521,   953,
     433,   517,   136,  1092,   198,   284,   136,  -519,   439,  -303,
     842,   844,   843,   386,   534,   853,   857,   492,   154,   722,
     724,   387,   154,   759,   285,  1110,  1112,   365,   218,   388,
     389,   390,   369,   842,  -356,  1132,  -519,  1133,   842,   871,
    1075,   286,   954,  -521,   521,   263,   490,  -521,   264,   845,
     777,  -521,  -303,   386,   694,   172,  -521,   492,   937,   194,
    1033,   387,   524,   846,   491,  1161,   490,   775,   375,   388,
     389,   390,   518,  -521,  1208,  1155,  1069,   229,   172,   365,
     901,  -519,  1073,   365,   518,   365,  1133,  1135,   365,   365,
     365,   897,  1127,  1128,  -519,  1100,   955,   365,   570,  -521,
     921,   266,  1076,   167,   720,   721,  -303,   376,  -521,   391,
    1097,  -335,  -264,   603,   320,   136,   481,   214,  -331,   294,
     962,   429,   620,   842,   902,  1134,  -521,   992,   172,   895,
     564,   154,   172,   172,   669,   830,  1135,   377,   361,   296,
    1198,  -519,  -331,   298,  -265,  -587,  -519,   518,  -521,   391,
     846,   320,  -657,   566,  -657,   172,   565,  -271,   167,   172,
    -270,  1033,   947,   167,   392,   696,  -521,   760,   299,  1199,
     136,   300,   587,    58,    23,   136,   320,   222,   847,   567,
     430,   408,   224,   301,   431,   867,   154,  1194,  1197,   404,
     386,   154,  1227,   761,   842,  1167,  1150,  1169,  1157,   903,
     492,   492,   304,   -70,   569,   364,   388,   389,   390,   857,
     405,   868,   770,  1057,   229,  1156,   320,   561,   740,   258,
     406,   490,   744,   -70,   933,   904,    56,  -279,   865,    83,
     409,   305,   433,   410,   492,  1074,   414,   437,    65,   167,
     439,   562,   -70,   560,   442,   411,   994,  -269,   932,   963,
     934,   136,   866,   167,  1238,  1239,  1240,  1241,  1242,  1243,
     266,  1026,   167,    85,   412,   136,   613,   154,   172,   175,
    1163,   760,   -70,   600,   136,   167,   833,   490,   492,   606,
     277,   154,   474,   194,   475,  -266,   391,   136,   415,   558,
     154,   294,   175,  1142,  1143,   544,   -69,   935,   267,   492,
     416,  1089,  1055,   154,   326,   764,   670,   760,   715,   716,
      37,   296,   294,  -342,   717,   298,   -69,   718,   719,   417,
     673,   802,   858,   172,  1129,   739,   965,   290,   172,   859,
     744,   948,   296,   946,  -342,   -69,   298,    54,   674,  -273,
     299,   392,   175,   300,  -342,    58,   175,   175,   320,   320,
     335,   338,   966,  1032,   320,   301,  1049,   320,   320,   320,
     320,   299,   728,   668,   300,   -69,   359,   305,   490,   175,
     589,   918,   194,   175,   304,   477,   301,  1002,   803,   744,
     310,   312,  -277,  -274,   744,   521,  -600,   167,  1011,   167,
     176,   336,   336,   320,  1108,   304,  -599,   371,  1023,   136,
    1175,   136,   805,   676,   172,  1125,  1166,   809,  1168,   357,
    1149,   357,  -600,   176,   521,   154,   994,   154,   172,  1195,
     806,   330,  -599,  1113,  1115,   810,  1176,   172,   291,   690,
     739,  -658,   919,  -658,   995,  1121,  1124,   320,  -421,  1050,
     172,  -733,  1053,  -734,   994,  -733,   691,  -734,   673,   320,
     796,   320,  -268,   492,  1200,   521,   994,   994,   320,   692,
     680,   722,  1095,   176,   835,   167,  1102,   176,   176,   560,
     492,  -272,   429,  1103,  1222,  1224,  -657,   136,  -657,   331,
     198,  1201,   175,  1202,  1203,  -278,  1204,  1205,  -280,   693,
     176,  1032,   294,   154,   176,  1209,  1113,  1115,  1121,  1124,
    -735,   365,  -267,   509,  -735,  1084,   433,  1086,  -654,  -275,
    -654,   437,   296,   523,   439,   525,   298,  -730,   442,   531,
     532,  -730,   529,  1126,   793,  -656,   795,  -656,  1130,   342,
     694,   430,    89,   968,   696,   431,   695,   175,  -655,   372,
    -655,   299,   175,   181,   300,   885,    58,   886,   374,   397,
     398,  1209,   172,   401,   172,   424,   301,   402,  -584,  -583,
     425,  -588,   551,  1001,  -589,  -586,   181,  -585,   483,   376,
     484,   486,   489,   365,   507,   304,   538,   508,   543,   549,
     527,   568,   528,   433,   552,   492,   537,   920,   437,   557,
     438,   439,  1032,   572,   573,   442,  1046,   739,   583,   594,
     618,   607,   449,   176,   623,   671,   675,   439,   714,   939,
     455,   727,   320,   742,   744,   745,   181,   743,   175,   748,
     181,   181,   746,   751,   753,   916,   492,   320,   754,   320,
     172,   764,   175,   773,   787,   778,   167,   788,   831,   167,
     490,   175,   836,   181,   840,   862,   874,   181,   136,   320,
     864,   136,  -144,   429,   175,   876,  -166,  -173,   176,   879,
    -172,  -171,   894,   176,   154,  -168,  -145,   154,  -175,  -176,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,  -170,  -174,  -146,  -169,   887,
    1031,   492,  -177,   880,   888,   889,   305,   890,   900,   891,
    1153,  1154,   924,   926,   893,   305,   944,  1006,   957,   994,
    1064,   892,   430,   997,   472,  1082,   431,  -654,   952,  -654,
     998,  1007,  1065,  1008,  1010,  1027,  1047,  1016,  1066,   867,
    1048,   167,  1096,  1070,  1147,   167,  1071,  1151,  1141,   176,
    1193,  1144,   167,   136,   320,  1159,  1180,   136,  1184,  1218,
    1219,  1199,  1223,   176,   136,   366,   181,  1225,  1250,   154,
    1251,   798,   176,   154,   433,   434,   175,  1088,   175,   437,
     154,   438,   439,   505,   577,   176,   442,   967,   225,  1067,
    1081,   571,   776,   449,   556,   320,   768,  1045,   774,   453,
     454,   455,  1235,  1131,  1244,   535,   492,   492,  1042,  1037,
    1237,   172,   591,  1000,   172,  1058,  1059,   595,   884,   380,
     767,   181,   167,     0,   167,   936,   181,   167,     0,     0,
     371,     0,     0,     0,   136,     0,   136,     0,  1031,   136,
     838,   839,     0,     0,   504,  1080,     0,     0,   852,     0,
     154,   167,   154,     0,   175,   154,     0,     0,     0,     0,
     320,   167,     0,  1079,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   136,     0,     0,     0,     0,     0,   154,
       0,     0,     0,     0,  1104,  1105,     0,     0,     0,   154,
       0,     0,     0,     0,     0,     0,     0,   176,     0,   176,
       0,     0,   181,     0,     0,     0,   172,     0,   167,     0,
     172,     0,     0,     0,   167,     0,   181,   172,     0,     0,
     136,     0,     0,   183,   371,   181,   136,     0,   371,     0,
       0,     0,     0,     0,     0,     0,   154,     0,   181,  1031,
       0,     0,   154,     0,     0,     0,   183,     0,     0,     0,
       0,     0,     0,     0,   504,     0,   504,     0,     0,   504,
     504,     0,   167,     0,     0,   320,   320,     0,     0,     0,
       0,     0,     0,     0,   136,   176,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   172,     0,   172,
     154,     0,   172,     0,     0,     0,   183,     0,     0,     0,
     183,   183,     0,     0,     0,     0,     0,     0,     0,     0,
     167,     0,   167,     0,     0,     0,   172,   167,     0,     0,
     167,     0,   136,   183,   136,   175,   172,   183,   175,   136,
       0,     0,   136,     0,     0,     0,     0,     0,   154,     0,
     154,   184,     0,     0,     0,   154,     0,     0,   154,   167,
     181,     0,   181,     0,     0,   167,     0,     0,     0,   167,
     167,   136,     0,     0,   184,     0,     0,   136,     0,     0,
       0,   136,   136,   172,     0,     0,     0,   154,   167,   172,
    1004,   429,   167,   154,   167,     0,     0,   154,   154,     0,
     136,     0,     0,     0,   136,     0,   136,     0,  1020,     0,
       0,     0,  1025,     0,     0,   167,   154,     0,     0,  1245,
     154,     0,   154,     0,   184,     0,     0,   136,   184,   184,
     175,     0,     0,     0,   175,     0,     0,   172,   181,     0,
       0,   175,     0,   154,     0,     0,   183,     0,     0,     0,
     430,   184,     0,     0,   431,   184,   176,     0,     0,   176,
     167,   167,   167,   167,     0,     0,     0,     0,     0,     0,
       0,     0,   136,   136,   136,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   172,     0,   172,   154,   154,
     154,   154,   172,     0,     0,   172,     0,     0,     0,     0,
       0,   183,   433,     0,     0,     0,   183,   437,     0,   438,
     439,   175,     0,   175,   442,     0,   175,     0,     0,     0,
       0,   449,     0,     0,   172,     0,     0,   453,   454,   455,
     172,     0,     0,     0,   172,   172,     0,     0,     0,     0,
     175,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     175,   176,     0,   172,   429,   176,     0,   172,     0,   172,
       0,     0,   176,     0,   184,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1120,  1123,
     172,     0,   183,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   183,   175,     0,     0,
       0,     0,     0,   175,     0,   183,     0,     0,     0,   181,
       0,     0,   181,   430,     0,     0,     0,   431,   183,   184,
       0,     0,     0,     0,   184,   172,   172,   172,   172,     0,
       0,     0,   176,     0,   176,  1158,     0,   176,     0,     0,
       0,  1120,  1123,     0,     0,     0,     0,     0,     0,     0,
       0,   175,     0,     0,     0,     0,     0,     0,     0,  1172,
    1173,   176,  1174,   429,     0,   433,   434,     0,   435,   436,
     437,   176,   438,   439,     0,     0,  1189,   442,     0,  1192,
       0,     0,     0,   448,   449,     0,     0,   452,     0,     0,
     453,   454,   455,     0,     0,     0,     0,     0,     0,   175,
     184,   175,     0,     0,   181,     0,   175,     0,   181,   175,
       0,     0,     0,     0,   184,   181,     0,     0,   176,     0,
       0,     0,   430,   184,   176,     0,   431,     0,     0,     0,
     183,     0,   183,   140,     0,     0,   184,     0,   175,     0,
       0,     0,     0,     0,   175,     0,     0,     0,   175,   175,
       0,     0,     0,     0,     0,     0,   140,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   175,     0,     0,
       0,   175,   176,   175,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   181,   442,   181,     0,     0,
     181,     0,   448,   449,   175,     0,   452,     0,     0,   453,
     454,   455,     0,     0,     0,     0,   140,     0,   183,     0,
     456,     0,     0,     0,   181,     0,     0,     0,     0,     0,
     176,     0,   176,     0,   181,     0,     0,   176,     0,     0,
     176,     0,     0,   140,     0,     0,     0,   140,     0,   175,
     175,   175,   175,     0,     0,     0,     0,     0,   184,     0,
     184,     0,     0,     0,     0,     0,     0,     0,     0,   176,
       0,     0,     0,   361,     0,   176,     0,    22,    23,   176,
     176,   181,     0,     0,     0,     0,   362,   181,    29,   363,
       0,     0,     0,     0,    32,     0,     0,     0,   176,     0,
       0,    37,   176,   256,   176,     0,     0,   422,     0,   230,
       0,     0,     0,     0,     0,   268,   269,   270,     0,     0,
       0,     0,     0,   231,   232,   176,   233,     0,    54,     0,
      56,   234,     0,     0,     0,   181,   184,     0,     0,   235,
     364,     0,    65,     0,     0,   236,     0,     0,     0,     0,
       0,   237,     0,     0,     0,   238,   140,     0,   239,     0,
     240,    81,     0,     0,    83,     0,     0,    85,     0,   241,
     176,   176,   176,   176,     0,     0,   242,   243,     0,     0,
       0,     0,     0,   181,   244,   181,     0,     0,     0,   183,
     181,     0,   183,   181,   245,     0,     0,     0,     0,     0,
       0,     0,     0,   246,   247,     0,   248,     0,   249,     0,
     250,   140,     0,   251,     0,     0,   140,   252,     0,     0,
     253,     0,   181,   254,     0,   400,     0,     0,   181,     0,
       0,     0,   181,   181,     0,     0,     0,     0,     0,   419,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   181,     0,     0,     0,   181,     0,   181,     0,     0,
     860,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   479,     0,     0,     0,   181,   271,
       0,  -322,     0,     0,   183,  -322,  -322,     0,   183,     0,
       0,     0,   140,     0,  -322,   183,  -322,  -322,     0,     0,
       0,     0,  -322,     0,     0,     0,   140,   184,     0,  -322,
     184,     0,  -322,     0,     0,   140,     0,     0,     0,     0,
       0,     0,     0,   181,   181,   181,   181,     0,   140,     0,
       0,     0,     0,  -322,   497,   498,  -322,     0,  -322,     0,
    -322,     0,  -322,  -322,     0,  -322,   514,     0,  -322,     0,
    -322,     0,     0,     0,     0,     0,     0,     0,     0,   230,
       0,     0,     0,     0,     0,   183,     0,   183,     0,  -322,
     183,     0,  -322,   231,   232,  -322,   233,     0,     0,     0,
       0,   234,     0,     0,     0,     0,   541,     0,     0,   235,
       0,     0,     0,     0,   183,   236,     0,     0,     0,     0,
    -322,   237,   184,     0,   183,   238,   184,     0,   239,     0,
     240,     0,     0,   184,     0,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,  -322,   242,   243,     0,     0,
       0,  -322,     0,     0,   244,     0,     0,   256,     0,     0,
     140,     0,   140,     0,   245,     0,     0,     0,     0,     0,
       0,   183,     0,   246,   247,     0,   248,   183,   249,     0,
     250,     0,     0,   251,     0,     0,     0,   252,   512,     0,
     253,     0,   605,   254,     0,     0,   610,     0,     0,     0,
       0,     0,     0,   184,     0,   184,     0,     0,   184,     0,
       0,     0,     0,   665,     0,   361,     0,     0,     0,    22,
      23,     0,     0,     0,     0,   183,     0,     0,   362,     0,
      29,   363,   184,     0,     0,     0,    32,   429,   140,     0,
       0,     0,   184,    37,     0,     0,     0,     0,     0,   513,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
      54,     0,    56,   183,    58,   183,  1028,     0,   730,  1029,
     183,     0,   364,   183,    65,     0,     0,     0,     0,   184,
       0,     0,     0,     0,     0,   184,   430,     0,     0,     0,
     431,     0,     0,    81,   756,     0,    83,     0,   514,    85,
       0,     0,   183,     0,     0,     0,     0,     0,   183,     0,
       0,   769,   183,   183,     0,     0,     0,     0,   430,     0,
     541,     0,   431,     0,    89,   514,     0,     0,     0,     0,
       0,   183,     0,   184,     0,   183,     0,   183,   433,   434,
       0,   435,   436,   437,     0,   438,   439,     0,     0,   100,
     442,     0,     0,     0,     0,  1030,     0,   449,   183,     0,
       0,     0,     0,   453,   454,   455,     0,     0,   814,     0,
     433,   434,     0,     0,   436,   437,     0,   438,   439,   837,
       0,   184,   442,   184,     0,     0,     0,     0,   184,   449,
       0,   184,     0,     0,   610,   453,   454,   455,     0,   140,
       0,     0,   140,   183,   183,   183,   183,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     184,     0,     0,     0,     0,     0,   184,     0,     0,     0,
     184,   184,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   230,     0,     0,     0,     0,     0,     0,   184,
       0,     0,     0,   184,     0,   184,   231,   232,   419,   233,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   235,     0,     0,     0,   184,     0,   236,     0,
       0,     0,     0,     0,   237,     0,     0,     0,   238,     0,
       0,   239,     0,   240,   140,     0,     0,     0,   140,     0,
       0,     0,   241,     0,     0,   140,     0,     0,     0,   242,
     243,     0,     0,     0,     0,     0,     0,   244,     0,     0,
       0,   184,   184,   184,   184,   276,     0,   245,     0,     0,
       0,     0,     0,     0,     0,     0,   246,   247,   931,   248,
       0,   249,     0,   250,     0,   514,   251,     0,     0,   276,
     252,   539,     0,   253,     0,   940,   254,     0,     0,   337,
     339,     0,     0,     0,     0,     0,   210,   690,     0,     0,
     220,     0,     0,   960,     0,   140,     0,   140,     0,   228,
     140,   905,     0,     0,   906,     0,   262,     0,     0,   907,
       0,   370,     0,     0,     0,     0,     0,     0,   278,   196,
     281,   282,     0,     0,   140,     0,   287,     0,   288,   908,
     289,     0,   540,     0,   140,   315,   909,   419,   198,   228,
     323,   325,   327,   328,   329,     0,     0,   910,   333,     0,
     334,     0,     0,     0,     0,   911,     0,   340,     0,     0,
     323,     0,     0,   343,     0,   344,     0,     0,   345,   346,
       0,   347,   912,     0,     0,   228,   323,   358,     0,     0,
       0,   140,     0,     0,   913,   665,     0,   140,   694,     0,
       0,     0,     0,     0,     0,   914,     0,   230,     0,     0,
       0,   915,     0,     0,   381,     0,   384,     0,     0,     0,
       0,   231,   232,     0,   233,     0,     0,     0,   226,   234,
       0,   487,     0,     0,     0,     0,     0,   235,     0,     0,
       0,     0,     0,   236,     0,   140,     0,     0,   756,   237,
       0,     0,     0,   238,     0,     0,   239,   426,   240,     0,
       0,     0,     0,     0,     0,     0,     0,   241,     0,     0,
       0,   500,   503,   960,   242,   243,     0,     0,   332,   510,
     478,     0,   244,     0,   333,   228,     0,     0,   276,     0,
     276,     0,   245,   140,     0,   140,     0,   276,     0,     0,
     140,   246,   247,   140,   248,     0,   249,     0,   250,   360,
       0,   251,   323,     0,     0,   252,     0,     0,   253,     0,
       0,   254,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   140,     0,     0,     0,     0,   276,   140,     0,
       0,     0,   140,   140,     0,     0,     0,     0,     0,   323,
     610,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   140,     0,     0,     0,   140,     0,   140,     0,     0,
       0,     0,     0,     0,   323,   365,     0,   958,     0,     0,
       0,     0,     0,     0,     0,   329,   333,     0,   140,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     545,   546,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   323,     0,     0,     0,   485,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   140,   140,   140,   140,   563,     0,     0,
       0,     0,     0,     0,     0,   610,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   580,     0,
     582,     0,     0,     0,     0,     0,     0,     0,     0,   592,
       0,     0,     0,   511,   596,   601,   604,     0,   522,     0,
       0,   228,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     677,   678,   679,   681,   683,   684,     0,   685,   686,   687,
     688,   689,     0,   697,   698,   699,   700,   701,   702,   703,
     704,   705,   706,   707,   708,   709,   710,   711,   712,   713,
       0,     0,     0,     0,     0,     0,   323,   323,     0,     0,
       0,     0,   323,     0,     0,   323,   323,   323,   323,   723,
     725,     0,     0,     0,   601,   729,     0,   738,     0,     0,
       0,     0,     0,   741,     0,     0,     0,     0,     0,     0,
       0,   361,     0,     0,   750,    22,    23,   752,     0,     0,
     593,   323,     0,     0,   362,     0,    29,   363,     0,     0,
       0,   763,    32,   766,     0,   276,   276,   281,     0,    37,
     771,   772,     0,   276,   276,     0,     0,     0,     0,     0,
       0,   315,     0,     0,     0,     0,   315,     0,     0,     0,
       0,     0,     0,     0,     0,   323,    54,     0,    56,     0,
      58,     0,  1028,     0,   741,  1029,     0,   323,   364,   323,
      65,     0,   228,   797,     0,     0,   323,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   827,   827,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,   841,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,     0,   747,   315,   749,     0,     0,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,   100,   195,     0,     0,    21,
       0,  1101,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,  -215,  -215,  -215,   878,   197,    34,
       0,     0,     0,    36,   883,     0,    38,     0,   198,     0,
     199,     0,   930,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,     0,     0,     0,   959,     0,
       0,    70,    71,    72,    73,    74,   201,   276,    76,   750,
     323,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,   323,     0,   323,   922,   923,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   276,    90,   323,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   315,     0,   276,     0,   951,     0,   276,     0,    99,
       0,     0,     0,     0,     0,   202,     0,   103,     0,  -215,
       0,     0,     0,     0,     0,     0,     0,     0,   580,     0,
       0,     0,   989,     0,   991,   827,   827,     0,   343,     0,
     344,     0,     0,   346,   347,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1003,     0,  1005,
       0,     0,     0,   228,     0,     0,     0,     0,  1012,   741,
    1015,     0,  1017,  1019,     0,     0,     0,  1022,  1024,     0,
       0,     0,   323,     0,  1068,     0,     0,  1034,   361,     0,
       0,   925,    22,    23,   928,     0,     0,     0,     0,     0,
       0,   362,     0,    29,   363,     0,     0,     0,     0,    32,
       0,  1083,     0,     0,     0,     0,    37,     0,     0,  1051,
    1015,  1022,  1054,   323,     0,     0,   738,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     681,   723,     0,    54,     0,    56,     0,    58,     0,  1028,
    1062,     0,  1029,     0,     0,   364,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   315,     0,     0,
       0,  1077,     0,   276,   276,     0,    81,   228,     0,    83,
       0,     0,    85,     0,     0,  1085,  1009,  1087,   323,     0,
    1014,     0,     0,     0,     0,     0,     0,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,     0,   827,
       0,     0,     0,     0,     0,     0,     0,  1098,     0,  1099,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1107,
     276,   738,   100,     0,     0,     0,   276,   276,  1171,     0,
       0,     0,     0,     0,  1118,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   276,   276,     0,   276,     0,     0,
       0,     0,     0,  1139,     0,     0,     0,  1060,   503,  1061,
       0,   276,  1063,     0,   276,  1146,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   323,   323,     0,  1078,     0,     0,     0,
       0,     0,     0,     0,     0,  1160,  1014,     0,     0,  1221,
       0,     0,     0,  1165,   827,   827,   827,   827,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1179,     0,  1183,     0,     0,
    1185,     0,  1188,     0,     0,  1191,     0,     0,     0,     0,
       0,     0,     0,  1111,     0,     0,     0,     0,     0,  1116,
       0,     0,     0,     0,     0,     0,     0,  1179,  1183,     0,
    1188,  1191,     0,     0,     0,     0,     0,     0,   503,     0,
     503,     0,     0,   503,   503,     0,     0,     0,     0,     0,
       0,     0,     0,  1226,     0,     0,     0,     0,   738,     0,
    1228,     0,     0,     0,  1230,     0,     0,  1152,     0,  1232,
    1233,     0,     0,  1234,     0,     0,     0,     0,     0,     0,
       0,  1236,     0,     0,   427,     0,     0,     0,     0,     0,
       0,   428,     0,     0,     0,  1246,  1247,  1232,  1248,  1249,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1178,     0,  1182,     0,     0,
       0,     0,   522,     0,     0,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1260,  1261,  1262,  1263,  1260,
    1261,  1262,  1263,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1217,     0,     0,     0,     0,     0,
    1220,     0,   430,     0,  1178,  1182,   431,     0,     0,     0,
       0,     0,     0,     0,     0,   501,     0,     0,     0,     0,
       0,     0,     0,  1229,     0,     0,     0,  1231,     0,   749,
       0,     0,     0,     0,     0,     0,     0,   432,   502,   458,
     459,   460,   461,   462,     0,     0,   465,   466,   467,   468,
       0,   470,   471,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,     0,   365,     0,     0,     0,     0,     0,     0,
     456,     0,     0,     0,     0,   749,   749,   749,   749,    -2,
       4,     0,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -215,    21,    22,    23,    24,    25,    26,
       0,  -215,    27,     0,    28,     0,    29,    30,    31,  -215,
    -215,  -215,    32,    33,    34,   -68,     0,    35,    36,    37,
       0,    38,  -342,    39,     0,    40,    41,    42,    43,    44,
       0,    45,    46,    47,    48,   -68,    49,    50,     0,    51,
      52,    53,     0,  -342,     0,     0,    54,    55,    56,    57,
      58,    59,    60,  -342,   -68,    61,    62,    63,    64,     0,
      65,    66,    67,     0,    68,    69,    70,    71,    72,    73,
      74,    75,     0,    76,    77,     0,    78,    79,    80,    81,
      82,     0,    83,    84,   -68,    85,    86,     0,     0,  -215,
       0,    87,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,   100,     0,     0,   101,     0,
     102,     0,   103,     0,  -215,     0,     4,   104,     5,     6,
       7,     8,     9,    10,    11,    12,     0,  -685,     0,    13,
      14,    15,    16,    17,  -685,    18,     0,    19,    20,  -215,
      21,    22,    23,    24,    25,    26,  -685,  -215,    27,  -685,
      28,     0,    29,    30,    31,  -215,  -215,  -215,    32,    33,
      34,   -68,     0,    35,    36,    37,     0,    38,  -342,    39,
       0,    40,    41,    42,    43,    44,     0,    45,    46,    47,
      48,   -68,    49,    50,     0,    51,    52,    53,     0,  -342,
       0,     0,    54,    55,    56,    57,     0,    59,    60,  -342,
     -68,    61,    62,    63,    64,  -685,    65,    66,    67,  -685,
      68,    69,    70,    71,    72,    73,    74,    75,     0,    76,
      77,     0,    78,    79,    80,    81,    82,     0,    83,    84,
     -68,    85,    86,     0,     0,  -215,     0,    87,     0,     0,
    -685,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,  -685,    92,
    -685,  -685,  -685,  -685,  -685,  -685,  -685,     0,  -685,  -685,
    -685,  -685,  -685,  -685,  -685,  -685,  -685,  -685,  -685,  -685,
      99,   100,  -685,  -685,  -685,     0,   102,  -685,   103,     0,
    -215,     0,   348,  -685,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -215,    21,    22,    23,    24,
      25,    26,     0,  -215,    27,     0,    28,     0,    29,    30,
      31,  -215,  -215,  -215,    32,    33,    34,   -68,     0,    35,
      36,    37,     0,    38,  -342,    39,     0,    40,    41,    42,
      43,    44,     0,    45,    46,    47,    48,   -68,    49,    50,
       0,    51,    52,    53,     0,  -342,     0,     0,    54,    55,
      56,    57,    58,    59,    60,  -342,   -68,    61,    62,    63,
      64,     0,    65,    66,    67,     0,    68,    69,    70,    71,
      72,    73,    74,    75,     0,    76,    77,     0,    78,    79,
      80,    81,    82,     0,    83,    84,   -68,    85,    86,     0,
       0,  -215,     0,    87,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,   100,     0,     0,
     101,     0,   102,   349,   103,     0,  -215,     0,     4,   104,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,  -215,    21,    22,    23,    24,    25,    26,     0,  -215,
      27,     0,    28,     0,    29,    30,    31,  -215,  -215,  -215,
      32,    33,    34,   -68,     0,    35,    36,    37,     0,    38,
    -342,    39,     0,    40,    41,    42,    43,    44,     0,    45,
      46,    47,    48,   -68,    49,    50,     0,    51,    52,    53,
       0,  -342,     0,     0,    54,    55,    56,    57,    58,    59,
      60,  -342,   -68,    61,    62,    63,    64,     0,    65,    66,
      67,     0,    68,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,     0,    78,    79,    80,    81,    82,     0,
      83,    84,   -68,    85,    86,     0,     0,  -215,     0,    87,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,   100,     0,     0,   101,     0,   102,   559,
     103,     0,  -215,     0,   575,   104,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,  -215,    21,    22,
      23,    24,    25,    26,     0,  -215,    27,     0,    28,     0,
      29,    30,    31,  -215,  -215,  -215,    32,    33,    34,   -68,
       0,    35,    36,    37,     0,    38,  -342,    39,     0,    40,
      41,    42,    43,    44,     0,    45,    46,    47,    48,   -68,
      49,    50,     0,    51,    52,    53,     0,  -342,     0,     0,
      54,    55,    56,    57,    58,    59,    60,  -342,   -68,    61,
      62,    63,    64,     0,    65,    66,    67,     0,    68,    69,
      70,    71,    72,    73,    74,    75,     0,    76,    77,     0,
      78,    79,    80,    81,    82,     0,    83,    84,   -68,    85,
      86,     0,     0,  -215,     0,    87,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,   100,
       0,     0,   101,     0,   102,   576,   103,     0,  -215,     0,
       4,   104,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -215,    21,    22,    23,    24,    25,    26,
       0,  -215,    27,     0,    28,     0,    29,    30,    31,  -215,
    -215,  -215,    32,    33,    34,   -68,     0,    35,    36,    37,
       0,    38,  -342,    39,     0,    40,    41,    42,    43,    44,
       0,    45,    46,    47,    48,   -68,    49,    50,     0,    51,
      52,    53,     0,  -342,     0,     0,    54,    55,    56,    57,
      58,    59,    60,  -342,   -68,    61,    62,    63,    64,     0,
      65,    66,    67,     0,    68,    69,    70,    71,    72,    73,
      74,    75,     0,    76,    77,     0,    78,    79,    80,    81,
      82,     0,    83,    84,   -68,    85,    86,     0,     0,  -215,
       0,    87,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,   100,     0,     0,   101,     0,
     102,   804,   103,     0,  -215,     0,     4,   104,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -215,
      21,    22,    23,    24,    25,    26,     0,  -215,    27,     0,
      28,     0,    29,    30,    31,  -215,  -215,  -215,    32,    33,
      34,   -68,     0,    35,    36,    37,     0,    38,  -342,    39,
       0,    40,    41,    42,    43,    44,     0,    45,    46,    47,
      48,   -68,    49,    50,     0,    51,    52,    53,     0,  -342,
       0,     0,    54,    55,    56,    57,   359,    59,    60,  -342,
     -68,    61,    62,    63,    64,     0,    65,    66,    67,     0,
      68,    69,    70,    71,    72,    73,    74,    75,     0,    76,
      77,     0,    78,    79,    80,    81,    82,     0,    83,    84,
     -68,    85,    86,     0,     0,  -215,     0,    87,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,   100,     0,     0,   101,     0,   102,     0,   103,     0,
    -215,     0,     4,   104,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -215,    21,    22,    23,    24,
      25,    26,     0,  -215,    27,     0,    28,     0,    29,    30,
      31,  -215,  -215,  -215,    32,    33,    34,   -68,     0,    35,
      36,    37,     0,    38,  -342,    39,     0,    40,    41,    42,
      43,    44,     0,    45,    46,    47,    48,   -68,    49,    50,
       0,    51,    52,    53,     0,  -342,     0,     0,    54,    55,
      56,    57,    58,    59,    60,  -342,   -68,    61,    62,    63,
      64,     0,    65,    66,    67,     0,    68,    69,    70,    71,
      72,    73,    74,    75,     0,    76,    77,     0,    78,    79,
      80,    81,    82,     0,    83,    84,   -68,    85,    86,     0,
       0,  -215,     0,    87,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,   100,     0,     0,
     101,     0,   102,     0,   103,     0,  -215,     0,     4,   104,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,  -215,    21,    22,    23,    24,    25,    26,     0,  -215,
      27,     0,    28,     0,    29,    30,    31,  -215,  -215,  -215,
      32,    33,    34,   -68,     0,    35,    36,    37,     0,    38,
    -342,    39,     0,    40,    41,    42,    43,    44,     0,    45,
      46,    47,    48,   -68,    49,    50,     0,    51,    52,    53,
       0,  -342,     0,     0,    54,    55,    56,    57,     0,    59,
      60,  -342,   -68,    61,    62,    63,    64,     0,    65,    66,
      67,     0,    68,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,     0,    78,    79,    80,    81,    82,     0,
      83,    84,   -68,    85,    86,     0,     0,  -215,     0,    87,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,   100,     0,     0,   101,     0,   102,     0,
     103,     0,  -215,     0,     4,   104,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,  -215,    21,    22,
      23,    24,    25,    26,     0,  -215,    27,     0,    28,     0,
      29,    30,    31,  -215,  -215,  -215,    32,  1021,    34,   -68,
       0,    35,    36,    37,     0,    38,  -342,    39,     0,    40,
      41,    42,    43,    44,     0,    45,    46,    47,    48,   -68,
      49,    50,     0,    51,    52,    53,     0,  -342,     0,     0,
      54,    55,    56,    57,     0,    59,    60,  -342,   -68,    61,
      62,    63,    64,     0,    65,    66,    67,     0,    68,    69,
      70,    71,    72,    73,    74,    75,     0,    76,    77,     0,
      78,    79,    80,    81,    82,     0,    83,    84,   -68,    85,
      86,     0,     0,  -215,     0,    87,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,   100,
       0,     0,   101,     0,   102,     0,   103,     0,  -215,     0,
       4,   104,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -215,    21,    22,    23,    24,    25,    26,
       0,  -215,    27,     0,    28,     0,    29,    30,    31,  -215,
    -215,  -215,    32,  1177,    34,   -68,     0,    35,    36,    37,
       0,    38,  -342,    39,     0,    40,    41,    42,    43,    44,
       0,    45,    46,    47,    48,   -68,    49,    50,     0,    51,
      52,    53,     0,  -342,     0,     0,    54,    55,    56,    57,
       0,    59,    60,  -342,   -68,    61,    62,    63,    64,     0,
      65,    66,    67,     0,    68,    69,    70,    71,    72,    73,
      74,    75,     0,    76,    77,     0,    78,    79,    80,    81,
      82,     0,    83,    84,   -68,    85,    86,     0,     0,  -215,
       0,    87,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,   100,     0,     0,   101,     0,
     102,     0,   103,     0,  -215,     0,     4,   104,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -215,
      21,    22,    23,    24,    25,    26,     0,  -215,    27,     0,
      28,     0,    29,    30,    31,  -215,  -215,  -215,    32,  1181,
      34,   -68,     0,    35,    36,    37,     0,    38,  -342,    39,
       0,    40,    41,    42,    43,    44,     0,    45,    46,    47,
      48,   -68,    49,    50,     0,    51,    52,    53,     0,  -342,
       0,     0,    54,    55,    56,    57,     0,    59,    60,  -342,
     -68,    61,    62,    63,    64,     0,    65,    66,    67,     0,
      68,    69,    70,    71,    72,    73,    74,    75,     0,    76,
      77,     0,    78,    79,    80,    81,    82,     0,    83,    84,
     -68,    85,    86,     0,     0,  -215,     0,    87,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,   100,     0,     0,   101,     0,   102,     0,   103,     0,
    -215,     0,     4,   104,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -215,    21,    22,    23,    24,
      25,    26,     0,  -215,    27,     0,    28,     0,    29,    30,
      31,  -215,  -215,  -215,    32,  1187,    34,   -68,     0,    35,
      36,    37,     0,    38,  -342,    39,     0,    40,    41,    42,
      43,    44,     0,    45,    46,    47,    48,   -68,    49,    50,
       0,    51,    52,    53,     0,  -342,     0,     0,    54,    55,
      56,    57,     0,    59,    60,  -342,   -68,    61,    62,    63,
      64,     0,    65,    66,    67,     0,    68,    69,    70,    71,
      72,    73,    74,    75,     0,    76,    77,     0,    78,    79,
      80,    81,    82,     0,    83,    84,   -68,    85,    86,     0,
       0,  -215,     0,    87,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,   100,     0,     0,
     101,     0,   102,     0,   103,     0,  -215,     0,     4,   104,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,  -215,    21,    22,    23,    24,    25,    26,     0,  -215,
      27,     0,    28,     0,    29,    30,    31,  -215,  -215,  -215,
      32,  1190,    34,   -68,     0,    35,    36,    37,     0,    38,
    -342,    39,     0,    40,    41,    42,    43,    44,     0,    45,
      46,    47,    48,   -68,    49,    50,     0,    51,    52,    53,
       0,  -342,     0,     0,    54,    55,    56,    57,     0,    59,
      60,  -342,   -68,    61,    62,    63,    64,     0,    65,    66,
      67,     0,    68,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,     0,    78,    79,    80,    81,    82,     0,
      83,    84,   -68,    85,    86,     0,     0,  -215,     0,    87,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,   100,     0,     0,   101,     0,   102,     0,
     103,     0,  -215,     0,   870,   104,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   195,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,   295,     0,   196,     0,
       0,     0,    31,  -215,  -215,  -215,     0,   197,    34,     0,
       0,     0,   297,     0,     0,    38,     0,   198,     0,   199,
       0,     0,    43,    44,     0,     0,    46,     0,    48,     0,
      49,    50,     0,    51,    52,     0,     0,     0,     0,     0,
       0,    55,     0,    57,     0,    59,     0,     0,     0,     0,
      62,    63,   200,     0,     0,     0,     0,     0,     0,     0,
      70,   302,    72,    73,    74,   303,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   202,     0,   103,     0,  -215,     0,
       0,   104,     5,     6,     7,     8,     9,    10,    11,    12,
       0,   626,     0,    13,     0,     0,    16,    17,   628,    18,
       0,   195,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,     0,
       0,     0,     0,   197,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   198,   634,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,     0,    51,
      52,     0,     0,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,     0,     0,     0,    62,    63,   200,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   201,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    88,   502,   458,   459,   460,   461,
     462,     0,     0,   465,   466,   467,   468,     0,   470,   471,
       0,   815,   816,   817,   818,   819,   647,     0,   648,     0,
      95,     0,   649,   650,   651,   652,   653,   654,   655,   820,
     657,   658,    97,   821,    99,   -92,   660,   661,   822,   663,
     202,     0,   103,     0,     0,     0,     0,   104,     5,     6,
       7,     8,     9,    10,    11,    12,     0,   626,     0,    13,
       0,     0,    16,    17,   628,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   198,
     634,   199,     0,     0,    43,    44,     0,     0,    46,     0,
      48,     0,    49,    50,     0,    51,    52,     0,     0,     0,
       0,     0,     0,    55,     0,    57,     0,    59,     0,     0,
       0,     0,    62,    63,   200,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   201,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      88,   502,   458,   459,   460,   461,   462,     0,     0,   465,
     466,   467,   468,     0,   470,   471,     0,   815,   816,   817,
     818,   819,   647,     0,   648,     0,    95,     0,   649,   650,
     651,   652,   653,   654,   655,   820,   657,   658,    97,   821,
      99,     0,   660,   661,   822,   663,   202,     0,   103,     0,
       0,     0,     0,   104,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,     0,    16,    17,
       0,    18,     0,   195,    20,     0,    21,     0,     0,     0,
       0,    26,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,    33,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,    45,    46,    47,    48,     0,    49,    50,
       0,    51,    52,    53,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,    73,    74,    75,     0,    76,    77,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   102,     0,   103,     0,     0,     0,     0,   104,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   195,
       0,     0,    21,   379,    23,     0,     0,     0,     0,     0,
      27,     0,   196,     0,     0,     0,    31,     0,     0,     0,
       0,   197,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   198,     0,   199,     0,     0,    43,    44,     0,     0,
      46,     0,    48,     0,    49,    50,     0,    51,    52,     0,
       0,     0,     0,     0,     0,    55,    56,    57,     0,    59,
       0,     0,     0,     0,    62,    63,   200,     0,    65,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   201,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,    85,    86,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   202,     0,
     103,     0,     0,     0,     0,   104,     5,     6,     7,     8,
       9,   382,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,    19,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   196,     0,
       0,     0,    31,  -215,  -215,  -215,     0,   197,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   198,     0,   199,
       0,     0,    43,    44,     0,     0,    46,     0,    48,     0,
      49,    50,     0,    51,    52,     0,     0,     0,     0,     0,
       0,    55,     0,    57,     0,    59,     0,     0,     0,     0,
      62,    63,    64,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   201,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,    83,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   202,     0,   103,     0,  -215,     0,
       0,   104,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   195,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,     0,
       0,     0,     0,   197,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   198,     0,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,     0,    51,
      52,     0,     0,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,     0,     0,     0,    62,    63,   200,  -729,
       0,     0,     0,  -729,     0,     0,    70,    71,    72,    73,
      74,   201,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,  -384,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,  -384,     0,     0,   101,     0,
     202,  -384,   103,     0,     0,     0,     0,   104,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   198,
       0,   199,     0,     0,    43,    44,     0,     0,    46,     0,
      48,     0,    49,    50,     0,    51,    52,     0,     0,     0,
       0,     0,     0,    55,     0,    57,     0,    59,     0,     0,
       0,     0,    62,    63,   200,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   201,     0,    76,
       0,     0,    78,   317,   318,     0,    82,   353,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,   354,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,     0,
       0,     0,     0,   104,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,  -215,  -215,  -215,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,     0,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   201,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   202,     0,   103,     0,  -215,     0,     0,   104,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   195,
       0,     0,    21,     0,   731,     0,     0,     0,     0,     0,
      27,     0,   196,     0,     0,     0,    31,     0,     0,     0,
       0,   197,    34,     0,   732,     0,    36,     0,     0,    38,
       0,   198,     0,   199,     0,     0,    43,    44,     0,     0,
      46,     0,    48,     0,    49,    50,     0,    51,    52,     0,
       0,     0,     0,     0,     0,    55,     0,    57,     0,    59,
       0,     0,     0,     0,    62,    63,   200,     0,   733,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   201,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,   734,    86,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   202,     0,
     103,     0,     0,     0,     0,   104,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   195,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   196,     0,
       0,     0,    31,     0,     0,     0,     0,   197,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   198,     0,   199,
       0,     0,    43,    44,     0,     0,    46,     0,    48,     0,
      49,    50,     0,    51,    52,     0,     0,     0,     0,     0,
       0,    55,     0,    57,     0,    59,     0,     0,     0,     0,
      62,    63,   200,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   201,     0,    76,     0,     0,
      78,   317,   318,     0,    82,   353,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   202,     0,   103,   792,     0,     0,
       0,   104,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   195,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,     0,
       0,     0,     0,   197,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   198,     0,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,     0,    51,
      52,     0,     0,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,     0,     0,     0,    62,    63,   200,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   201,     0,    76,     0,     0,    78,   317,   318,     0,
      82,   353,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     202,     0,   103,   794,     0,     0,     0,   104,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   198,
       0,   199,     0,     0,    43,    44,     0,     0,    46,     0,
      48,     0,    49,    50,     0,    51,    52,     0,     0,     0,
       0,     0,     0,    55,     0,    57,     0,    59,     0,     0,
       0,     0,    62,    63,   200,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   201,     0,    76,
       0,     0,    78,   317,   318,     0,    82,     0,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,   319,     0,     0,   101,     0,   202,     0,   103,     0,
       0,     0,     0,   104,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,     0,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   201,     0,    76,     0,     0,    78,   317,
     318,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   202,     0,   103,  -536,     0,     0,     0,   104,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   195,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      27,     0,   196,     0,     0,     0,    31,     0,     0,     0,
       0,   197,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   198,     0,   199,     0,     0,    43,    44,     0,     0,
      46,     0,    48,     0,    49,    50,     0,    51,    52,     0,
       0,     0,     0,     0,     0,    55,     0,    57,     0,    59,
       0,     0,     0,     0,    62,    63,   200,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   201,
       0,    76,     0,     0,    78,   317,   318,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   202,     0,
     103,     0,     0,  -536,     0,   104,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   195,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   196,     0,
       0,     0,    31,     0,     0,     0,     0,   197,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   198,     0,   199,
       0,     0,    43,    44,     0,     0,    46,     0,    48,     0,
      49,    50,     0,    51,    52,     0,     0,     0,     0,     0,
       0,    55,     0,    57,     0,    59,     0,     0,     0,     0,
      62,    63,   200,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   201,     0,    76,     0,     0,
      78,   317,   318,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   202,     0,   103,   799,     0,     0,
       0,   104,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   195,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,     0,
       0,     0,     0,   197,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   198,     0,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,   938,    51,
      52,     0,     0,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,     0,     0,     0,    62,    63,   200,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   201,     0,    76,     0,     0,    78,   317,   318,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     202,     0,   103,     0,     0,     0,     0,   104,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   198,
       0,   199,     0,     0,    43,    44,     0,     0,    46,     0,
      48,     0,    49,    50,     0,    51,    52,     0,     0,     0,
       0,     0,     0,    55,     0,    57,     0,    59,     0,     0,
       0,     0,    62,    63,   200,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   201,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,  -392,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,  -392,     0,     0,   101,     0,   202,  -392,   103,     0,
       0,     0,     0,   104,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,     0,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   201,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,  -377,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,  -377,     0,     0,
     101,     0,   202,  -377,   103,     0,     0,     0,     0,   104,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   195,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      27,     0,   196,     0,     0,     0,    31,     0,     0,     0,
       0,   197,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   198,     0,   199,     0,     0,    43,    44,     0,     0,
      46,     0,    48,     0,    49,    50,     0,    51,    52,     0,
       0,     0,     0,     0,     0,    55,     0,    57,     0,    59,
       0,     0,     0,     0,    62,    63,   200,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   201,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,  -385,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,  -385,     0,     0,   101,     0,   202,  -385,
     103,     0,     0,     0,     0,   104,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   195,     0,     0,    21,     0,
     259,     0,     0,     0,     0,     0,    27,     0,   196,     0,
       0,     0,    31,     0,     0,     0,     0,   197,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   198,     0,   199,
       0,     0,    43,    44,     0,     0,    46,     0,    48,     0,
      49,    50,     0,    51,    52,     0,     0,     0,     0,     0,
       0,    55,     0,    57,     0,    59,     0,     0,     0,     0,
      62,    63,   200,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   201,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,   260,
      86,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   202,     0,   103,     0,     0,     0,
       0,   104,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   195,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,     0,
       0,     0,     0,   197,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   198,     0,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,     0,    51,
      52,     0,     0,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,     0,     0,     0,    62,    63,   200,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   201,     0,    76,     0,     0,    78,   317,   318,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     202,     0,   103,     0,     0,     0,     0,   104,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   198,
       0,   199,     0,     0,    43,    44,     0,     0,    46,     0,
      48,     0,    49,    50,     0,    51,    52,     0,     0,     0,
       0,     0,     0,    55,     0,    57,     0,    59,     0,     0,
       0,     0,    62,    63,   200,  -729,     0,     0,     0,  -729,
       0,     0,    70,    71,    72,    73,    74,   201,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,     0,
       0,     0,     0,   104,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,     0,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   201,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,   597,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   202,     0,   103,     0,     0,   598,     0,   104,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   195,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      27,     0,   196,     0,     0,     0,    31,     0,     0,     0,
       0,   197,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   198,     0,   199,     0,     0,    43,    44,     0,     0,
      46,     0,    48,     0,    49,    50,     0,    51,    52,     0,
       0,     0,     0,     0,     0,    55,     0,    57,     0,    59,
       0,     0,     0,     0,    62,    63,   200,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   201,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,  -413,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   202,     0,
     103,  -413,     0,     0,     0,   104,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   195,     0,     0,    21,     0,
       0,     0,     0,     0,     0,   275,    27,     0,   196,     0,
       0,     0,    31,     0,     0,     0,     0,   197,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   198,     0,   199,
       0,     0,    43,    44,     0,     0,    46,     0,    48,     0,
      49,    50,     0,    51,    52,     0,     0,     0,     0,     0,
       0,    55,     0,    57,     0,    59,     0,     0,     0,     0,
      62,    63,   200,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   201,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   102,     0,   103,     0,     0,     0,
       0,   104,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   195,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,     0,
       0,     0,     0,   197,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   198,     0,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,     0,    51,
      52,     0,     0,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,     0,     0,     0,    62,    63,   200,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   201,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,   597,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     202,     0,   103,     0,     0,     0,     0,   104,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   198,
       0,   199,     0,     0,    43,    44,     0,     0,    46,     0,
      48,     0,    49,    50,     0,    51,    52,     0,     0,     0,
       0,     0,     0,    55,     0,    57,     0,    59,     0,     0,
       0,     0,    62,    63,   200,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   201,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,     0,
       0,   598,     0,   104,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,     0,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   201,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   202,   790,   103,     0,     0,     0,     0,   104,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   195,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      27,     0,   196,     0,     0,     0,    31,     0,     0,     0,
       0,   197,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   198,     0,   199,     0,     0,    43,    44,     0,     0,
      46,     0,    48,     0,    49,    50,     0,    51,    52,     0,
       0,     0,     0,     0,     0,    55,     0,    57,     0,    59,
       0,     0,     0,     0,    62,    63,   200,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   201,
       0,    76,     0,     0,    78,     0,     0,     0,    82,   990,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   202,     0,
     103,     0,     0,     0,     0,   104,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   195,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   196,     0,
       0,     0,    31,     0,     0,     0,     0,   197,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   198,     0,   199,
       0,     0,    43,    44,     0,     0,    46,     0,    48,     0,
      49,    50,     0,    51,    52,     0,     0,     0,     0,     0,
       0,    55,     0,    57,     0,    59,     0,     0,     0,     0,
      62,    63,   200,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   201,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   202,     0,   103,     0,     0,  1013,
       0,   104,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   195,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,     0,
       0,     0,     0,   197,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   198,     0,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,     0,    51,
      52,     0,     0,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,     0,     0,     0,    62,    63,   200,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   201,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     202,     0,   103,     0,     0,  1018,     0,   104,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   198,
       0,   199,     0,     0,    43,    44,     0,     0,    46,     0,
      48,     0,    49,    50,     0,    51,    52,     0,     0,     0,
       0,     0,     0,    55,     0,    57,     0,    59,     0,     0,
       0,     0,    62,    63,   200,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   201,     0,    76,
       0,     0,    78,     0,     0,     0,    82,  1164,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,     0,
       0,     0,     0,   104,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,     0,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   201,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   202,     0,   103,     0,     0,     0,     0,   104,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   195,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
     295,     0,   196,     0,     0,     0,    31,     0,     0,     0,
       0,   197,    34,     0,     0,     0,   297,     0,     0,    38,
       0,   198,     0,   199,     0,     0,    43,    44,     0,     0,
      46,     0,    48,     0,    49,    50,     0,    51,    52,     0,
       0,     0,     0,     0,     0,    55,     0,    57,     0,    59,
       0,     0,     0,     0,    62,    63,   200,     0,     0,     0,
       0,     0,     0,     0,    70,   302,    72,    73,    74,   303,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   202,     0,
     103,     0,     0,     0,     0,   104,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   195,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   196,     0,
       0,     0,    31,     0,     0,     0,     0,  1052,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   198,     0,   199,
       0,     0,    43,    44,     0,     0,    46,     0,    48,     0,
      49,    50,     0,    51,    52,     0,     0,     0,     0,     0,
       0,    55,     0,    57,     0,    59,     0,     0,     0,     0,
      62,    63,   200,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   201,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   202,     0,   103,     0,     0,     0,
       0,   104,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   195,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,     0,
       0,     0,     0,  1212,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   198,     0,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,     0,    51,
      52,     0,     0,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,     0,     0,     0,    62,    63,   200,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   201,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     202,     0,   103,     0,     0,     0,     0,   104,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,  1213,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   198,
       0,   199,     0,     0,    43,    44,     0,     0,    46,     0,
      48,     0,    49,    50,     0,    51,    52,     0,     0,     0,
       0,     0,     0,    55,     0,    57,     0,    59,     0,     0,
       0,     0,    62,    63,   200,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   201,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,     0,
       0,     0,     0,   104,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,  1215,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,     0,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   201,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   202,     0,   103,     0,     0,     0,     0,   104,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   195,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      27,     0,   196,     0,     0,     0,    31,     0,     0,     0,
       0,  1216,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   198,     0,   199,     0,     0,    43,    44,     0,     0,
      46,     0,    48,     0,    49,    50,     0,    51,    52,     0,
       0,     0,     0,     0,     0,    55,     0,    57,     0,    59,
       0,     0,     0,     0,    62,    63,   200,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   201,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     6,
       7,     8,     9,    10,    11,    12,     0,    96,     0,    13,
      97,    98,    99,    17,     0,     0,   101,   195,   202,     0,
     103,     0,   294,     0,     0,   104,     0,     0,     0,     0,
     196,     0,     0,     0,     0,  -215,  -215,  -215,     0,   197,
       0,     0,   296,     0,     0,     0,   298,     0,     0,     0,
       0,   199,     0,     0,    43,    44,     0,     0,     0,     0,
      48,     0,    49,    50,     0,     0,     0,     0,     0,     0,
       0,   299,     0,    55,   300,    57,    58,    59,     0,     0,
       0,     0,    62,     0,   200,     0,   301,     0,     0,     0,
       0,     0,    70,     0,    72,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,   304,     0,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,  -346,
    -215,     0,     0,   104,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,     0,    17,     0,
       0,     0,   195,     0,     0,     0,     0,   294,     0,     0,
       0,     0,     0,     0,     0,   196,     0,     0,     0,     0,
    -215,  -215,  -215,     0,   197,     0,     0,   296,     0,     0,
       0,   298,     0,     0,     0,     0,   199,     0,     0,    43,
      44,     0,     0,     0,     0,    48,     0,    49,    50,     0,
       0,     0,     0,     0,     0,     0,   299,     0,    55,   300,
      57,    58,    59,     0,     0,     0,     0,    62,     0,   200,
       0,   301,     0,     0,     0,     0,     0,    70,     0,    72,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
     304,     0,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   624,     0,
     625,     0,    90,    91,    92,    93,    94,     0,     0,   626,
       0,    95,     0,     0,   627,   232,   628,   629,     0,     0,
      96,     0,   630,    97,    98,    99,     0,     0,     0,   101,
     235,   202,   196,   103,     0,  -215,   236,     0,   104,     0,
       0,     0,   631,     0,     0,     0,   238,     0,     0,   632,
       0,   633,   634,     0,     0,     0,     0,     0,     0,     0,
     635,     0,     0,     0,     0,     0,     0,   242,   636,     0,
       0,     0,     0,     0,     0,   244,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   637,     0,     0,     0,     0,
       0,     0,     0,     0,   246,   247,     0,   638,     0,   249,
       0,   639,     0,     0,   251,     0,     0,     0,   640,     0,
       0,   253,     0,     0,   641,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   458,   459,   460,   461,   462,     0,
       0,   465,   466,   467,   468,     0,   470,   471,     0,   642,
     643,   644,   645,   646,   647,     0,   648,     0,     0,     0,
     649,   650,   651,   652,   653,   654,   655,   656,   657,   658,
       0,   659,     0,     0,   660,   661,   662,   663,     0,     0,
     664,     5,     6,     7,     8,     9,    10,    11,    12,     0,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,     0,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,     0,  -215,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,  -215,  -215,  -215,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,     0,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   201,     0,    76,     0,     0,    78,     0,
     230,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,   231,   232,     0,   233,     0,     0,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
     235,     0,     0,    90,     0,    92,   236,     0,     0,     0,
       0,     0,   237,     0,     0,     0,   238,     0,     0,   239,
       0,   240,     0,     0,     0,     0,    99,     0,     0,     0,
     241,     0,     0,     0,   103,     0,  -215,   242,   243,     0,
       0,     0,     0,     0,     0,   244,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   245,     0,     0,     0,     0,
       0,     0,     0,     0,   246,   247,     0,   248,     0,   249,
       0,   250,     0,   690,   251,     0,     0,     0,   252,     0,
       0,   253,   626,     0,   254,     0,     0,   969,   970,   628,
     971,     0,     0,     0,     0,   972,     0,     0,     0,     0,
       0,     0,     0,   692,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   973,     0,     0,     0,   974,
       0,     0,   975,     0,   198,   634,     0,     0,     0,     0,
       0,     0,     0,   693,     0,     0,     0,     0,     0,     0,
     271,   976,     0,     0,     0,     0,     0,     0,   977,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   978,     0,
       0,     0,     0,     0,     0,     0,     0,   979,   980,     0,
     981,     0,   982,     0,   694,     0,     0,     0,     0,     0,
     695,   983,     0,     0,   984,     0,     0,   985,     0,     0,
       0,     0,     0,     0,     0,     0,   502,   458,   459,   460,
     461,   462,     0,     0,   465,   466,   467,   468,     0,   470,
     471,     0,   642,   643,   644,   645,   646,   647,     0,   648,
       0,     0,     0,   649,   650,   651,   652,   653,   654,   655,
     656,   657,   658,   230,   659,     0,     0,   660,   661,   662,
     663,   986,   626,     0,     0,     0,     0,   231,   232,   628,
     233,     0,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,   235,     0,     0,     0,     0,     0,   236,
       0,     0,     0,     0,     0,   237,     0,     0,     0,   238,
       0,     0,   239,     0,   240,   634,     0,     0,     0,     0,
       0,     0,     0,   241,     0,     0,     0,     0,     0,     0,
     242,   243,     0,     0,     0,     0,     0,     0,   244,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   245,     0,
       0,     0,     0,     0,   427,     0,     0,   246,   247,     0,
     248,   428,   249,     0,   250,     0,     0,   251,     0,     0,
       0,   252,     0,   429,   253,     0,     0,   254,     0,     0,
       0,     0,     0,     0,     0,     0,   502,   458,   459,   460,
     461,   462,     0,     0,   465,   466,   467,   468,     0,   470,
     471,     0,   642,   643,   644,   645,   646,   647,     0,   648,
       0,     0,     0,   649,   650,   651,   652,   653,   654,   655,
     656,   657,   658,     0,   659,     0,     0,   660,   661,   662,
     663,     0,   430,     0,   427,     0,   431,     0,     0,     0,
       0,   428,     0,     0,     0,  1186,     0,     0,     0,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   502,   458,
     459,   460,   461,   462,     0,     0,   465,   466,   467,   468,
       0,   470,   471,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,   430,   365,   427,     0,   431,     0,     0,     0,
     456,   428,     0,     0,     0,  1252,     0,     0,     0,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   502,   458,
     459,   460,   461,   462,     0,     0,   465,   466,   467,   468,
       0,   470,   471,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,   430,   365,   427,     0,   431,     0,     0,     0,
     456,   428,     0,     0,     0,  1253,     0,     0,     0,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   502,   458,
     459,   460,   461,   462,     0,     0,   465,   466,   467,   468,
       0,   470,   471,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,   430,   365,   427,     0,   431,     0,     0,     0,
     456,   428,     0,     0,     0,  1254,     0,     0,     0,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   502,   458,
     459,   460,   461,   462,     0,     0,   465,   466,   467,   468,
       0,   470,   471,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,   430,   365,     0,     0,   431,     0,     0,   427,
     456,     0,     0,     0,     0,  1255,   428,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   429,   854,
       0,     0,     0,     0,     0,     0,     0,   432,   502,   458,
     459,   460,   461,   462,   855,     0,   465,   466,   467,   468,
       0,   470,   471,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,     0,   365,     0,   427,     0,   430,     0,     0,
     456,   431,   428,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   429,   275,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   842,     0,
     964,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,   430,   453,   454,   455,   431,   365,     0,
       0,     0,     0,     0,     0,   456,     0,   427,     0,     0,
       0,     0,     0,     0,   428,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   842,     0,   429,     0,   432,     0,
       0,   584,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   585,     0,
     453,   454,   455,   429,   365,     0,     0,     0,     0,     0,
       0,   456,     0,     0,     0,   430,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   427,
     811,     0,     0,     0,     0,     0,   428,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   429,     0,
     432,     0,     0,   812,     0,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,   431,   433,   434,     0,
     435,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     813,   586,   453,   454,   455,     0,     0,     0,     0,     0,
       0,     0,     0,   456,     0,   427,     0,   430,     0,     0,
       0,   431,   428,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,   429,  1122,   442,   443,   444,     0,
     446,   447,   448,   449,     0,     0,   452,     0,     0,   453,
     454,   455,   432,     0,     0,     0,     0,     0,     0,     0,
     456,     0,     0,     0,     0,     0,     0,     0,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,   430,   453,   454,   455,   431,     0,     0,
     427,     0,     0,     0,     0,   456,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
     275,     0,     0,     0,   842,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,     0,     0,
     453,   454,   455,     0,   365,     0,   427,     0,   430,     0,
       0,   456,   431,   428,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   429,   275,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   842,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,   430,   453,   454,   455,   431,   365,
       0,   427,     0,     0,     0,     0,   456,     0,   428,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,   834,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
       0,   453,   454,   455,     0,   365,     0,   427,     0,   430,
       0,     0,   456,   431,   428,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   429,   898,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     214,     0,   899,     0,   432,     0,     0,     0,   427,     0,
       0,     0,     0,     0,     0,   428,     0,     0,     0,     0,
       0,   433,   434,     0,   435,   436,   437,   429,   438,   439,
     440,     0,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,     0,   430,   453,   454,   455,   431,
       0,     0,     0,   427,     0,     0,     0,   456,     0,     0,
     428,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,     0,
     432,     0,     0,     0,     0,     0,   430,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,   433,   434,   929,
     435,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
       0,   432,   453,   454,   455,     0,     0,     0,     0,     0,
       0,   430,     0,   456,     0,   431,     0,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   427,   842,   453,   454,   455,   432,   365,   428,     0,
       0,     0,     0,     0,   456,     0,     0,     0,     0,     0,
     429,     0,     0,   433,   434,  1093,   435,   436,   437,     0,
     438,   439,   440,     0,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   427,     0,   453,   454,
     455,     0,     0,   428,     0,     0,     0,  1114,     0,   456,
       0,     0,  1094,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,   431,     0,     0,   427,   526,     0,     0,
       0,     0,     0,   428,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,   431,     0,
       0,   433,   434,     0,   435,   436,   437,     0,   438,   439,
     440,     0,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,     0,   842,   453,   454,   455,   432,
       0,     0,     0,     0,   430,     0,     0,   456,   431,     0,
       0,     0,     0,     0,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   432,
       0,   453,   454,   455,     0,     0,     0,   427,     0,     0,
    1162,     0,   456,     0,   428,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,   429,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
       0,   453,   454,   455,     0,     0,     0,   427,     0,     0,
       0,     0,   456,     0,   428,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   430,     0,     0,     0,   431,
       0,     0,   427,     0,     0,     0,     0,     0,     0,   428,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,     0,
     432,     0,     0,     0,     0,   430,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,     0,   433,   434,     0,
     435,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     432,     0,   453,   454,   455,     0,   550,     0,   427,     0,
     430,     0,     0,   456,   431,   428,     0,   433,   434,     0,
     435,   436,   437,   726,   438,   439,   440,   429,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
       0,   554,   453,   454,   455,   432,     0,     0,     0,   427,
     807,     0,     0,   456,     0,     0,   428,     0,     0,     0,
       0,     0,   433,   434,     0,   435,   436,   437,   429,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,     0,   430,   453,   454,   455,
     431,     0,     0,     0,     0,   427,     0,     0,   456,     0,
       0,     0,   428,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,   432,     0,     0,     0,     0,     0,   430,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,     0,   432,   453,   454,   455,     0,     0,     0,     0,
     791,     0,     0,   430,   456,     0,     0,   431,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   808,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   427,     0,   453,   454,   455,   849,   432,   428,
       0,     0,     0,     0,     0,   456,     0,     0,     0,     0,
       0,   429,     0,     0,   927,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   427,     0,
     453,   454,   455,     0,     0,   428,     0,     0,     0,     0,
       0,   456,     0,     0,     0,     0,     0,   429,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,     0,     0,     0,   431,     0,     0,     0,   427,     0,
       0,     0,     0,     0,     0,   428,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
       0,     0,     0,     0,     0,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   430,     0,     0,     0,
     431,     0,   433,   434,     0,   435,   436,   437,     0,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,     0,     0,   453,   454,   455,
       0,   432,     0,     0,     0,     0,   430,     0,   456,     0,
     431,     0,     0,     0,     0,     0,     0,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   432,     0,   453,   454,   455,     0,     0,     0,     0,
     941,     0,     0,     0,   456,     0,     0,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   427,   996,   453,   454,   455,     0,     0,   428,     0,
     942,     0,     0,     0,   456,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   427,     0,     0,     0,     0,     0,     0,   428,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,   427,  1090,   431,     0,     0,     0,     0,   428,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   432,     0,     0,     0,     0,   430,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
       0,   433,   434,     0,   435,   436,   437,     0,   438,   439,
     440,     0,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   432,     0,   453,   454,   455,   430,
       0,     0,   427,   431,     0,     0,     0,   456,     0,   428,
       0,   433,   434,     0,   435,   436,   437,     0,   438,   439,
     440,   429,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   432,     0,   453,   454,   455,     0,
       0,     0,   427,  1044,     0,     0,     0,   456,     0,   428,
       0,   433,   434,     0,   435,   436,   437,     0,   438,   439,
     440,   429,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,     0,     0,   453,   454,   455,     0,
     430,     0,     0,     0,   431,     0,     0,   456,   427,     0,
       0,     0,     0,     0,     0,   428,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,     0,     0,
       0,   214,     0,     0,     0,   432,     0,     0,     0,     0,
     430,     0,     0,     0,   431,     0,     0,     0,     0,     0,
       0,     0,   433,   434,     0,   435,   436,   437,     0,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   432,     0,   453,   454,   455,
       0,     0,     0,     0,     0,     0,   430,     0,   456,     0,
     431,     0,   433,   434,     0,   435,   436,   437,     0,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   427,  1106,   453,   454,   455,
    1117,   432,   428,     0,     0,     0,     0,     0,   456,     0,
       0,     0,     0,     0,   429,     0,     0,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   427,     0,   453,   454,   455,     0,     0,   428,     0,
       0,     0,     0,     0,   456,     0,     0,     0,     0,     0,
     429,  1148,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,   431,     0,     0,
       0,     0,   427,  1170,     0,     0,     0,     0,     0,   428,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,   431,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,     0,     0,
     453,   454,   455,     0,   432,     0,     0,     0,     0,  1145,
     430,   456,   427,     0,   431,     0,     0,     0,     0,   428,
       0,   433,   434,     0,   435,   436,   437,     0,   438,   439,
     440,   429,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,     0,   432,   453,   454,   455,     0,
       0,     0,     0,   427,     0,     0,     0,   456,     0,     0,
     428,  1206,   433,   434,     0,   435,   436,   437,     0,   438,
     439,   440,   429,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,     0,     0,   453,   454,   455,
     430,     0,     0,     0,   431,     0,     0,     0,   456,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   432,     0,     0,     0,   427,
       0,   430,     0,     0,     0,   431,   428,     0,     0,     0,
       0,     0,   433,   434,  1214,   435,   436,   437,   429,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,     0,   432,   453,   454,   455,
     427,     0,     0,     0,     0,     0,     0,   428,   456,     0,
       0,     0,     0,   433,   434,     0,   435,   436,   437,   429,
     438,   439,   440,     0,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,     0,   430,   453,   454,
     455,   431,     0,     0,     0,   427,     0,     0,     0,   456,
    1256,     0,   428,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,   432,     0,     0,     0,     0,     0,   430,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,   433,
     434,  1257,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,   432,   453,   454,   455,     0,     0,     0,
       0,   427,     0,   430,     0,   456,     0,   431,   428,     0,
     433,   434,     0,   435,   436,   437,  1258,   438,   439,   440,
     429,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,     0,   453,   454,   455,   432,     0,
       0,     0,   427,     0,     0,     0,   456,     0,     0,   428,
       0,     0,     0,     0,     0,   433,   434,     0,   435,   436,
     437,   429,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,     0,   430,
     453,   454,   455,   431,     0,     0,     0,   427,     0,     0,
       0,   456,  1259,     0,   428,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,   432,     0,     0,     0,     0,     0,
     430,     0,     0,     0,   431,     0,     0,     0,     0,     0,
       0,   433,   434,     0,   435,   436,   437,     0,   438,   439,
     440,     0,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,     0,   432,   453,   454,   455,     0,
       0,     0,     0,     0,     0,   917,     0,   456,     0,   431,
       0,     0,   433,   434,     0,   435,   436,   437,     0,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,     0,     0,   453,   454,   455,
     432,     0,     0,     0,     0,     0,     0,     0,   456,     0,
       0,     0,     0,     0,     0,     0,     0,   433,   434,     0,
     435,   436,   437,     0,   438,   439,   440,   429,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
       0,   361,   453,   454,   455,    22,    23,     0,     0,     0,
       0,     0,     0,   456,   362,     0,    29,   363,     0,     0,
       0,     0,    32,     0,     0,     0,     0,     0,     0,    37,
       0,     0,  -342,     0,     0,     0,     0,   429,     0,     0,
       0,     0,     0,     0,     0,   -68,   430,     0,     0,     0,
     431,     0,     0,  -342,     0,     0,    54,     0,    56,     0,
       0,     0,    60,  -342,   -68,    61,     0,   429,   364,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,   430,     0,   433,   434,
     431,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,     0,   446,   447,   448,   449,   450,   451,
     452,     0,     0,   453,   454,   455,   430,     0,     0,     0,
     431,   432,     0,     0,   456,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   100,     0,     0,   433,   434,
     365,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,     0,   446,   447,   448,   449,   450,     0,
     452,     0,     0,   453,   454,   455,     0,     0,   433,   434,
       0,   435,   436,   437,   456,   438,   439,   440,     0,   441,
     442,   443,   444,     0,   446,   447,   448,   449,   450,     0,
     452,     0,   361,   453,   454,   455,    22,    23,     0,     0,
       0,     0,     0,     0,   456,   362,     0,    29,   363,     0,
     361,     0,     0,    32,    22,    23,     0,     0,     0,     0,
      37,     0,     0,   362,     0,    29,   363,     0,     0,     0,
       0,    32,     0,     0,     0,     0,     0,     0,    37,     0,
       0,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,   359,     0,  1028,     0,     0,  1029,     0,     0,   364,
       0,    65,     0,     0,     0,    54,     0,    56,   608,     0,
     230,  1028,     0,     0,  1029,     0,     0,   364,     0,    65,
      81,     0,     0,    83,   231,   232,    85,   233,     0,     0,
       0,     0,   234,     0,     0,     0,     0,     0,    81,     0,
     235,    83,     0,     0,    85,     0,   236,     0,     0,     0,
       0,    89,   237,     0,     0,     0,   238,     0,     0,   239,
       0,   240,     0,     0,     0,     0,   230,     0,     0,    89,
     241,     0,     0,     0,     0,     0,   100,   242,   243,     0,
     231,   232,     0,   233,     0,   244,     0,     0,   234,     0,
       0,     0,     0,     0,   100,   245,   235,     0,     0,     0,
       0,     0,   236,     0,   246,   247,     0,   248,   237,   249,
       0,   250,   238,     0,   251,   239,     0,   240,   252,     0,
       0,   253,   230,     0,   254,     0,   241,     0,     0,     0,
       0,     0,     0,   242,   243,     0,   231,   232,     0,   233,
       0,   244,     0,     0,   234,     0,    23,     0,    89,     0,
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
    -317,     0,   251,  -317,     0,  -317,   252,   539,     0,   253,
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
       2,    62,    20,   401,     2,   165,   266,   181,   609,   584,
     112,   418,     2,   548,    62,    62,   484,    26,   552,   223,
     508,   620,    67,    25,   617,   667,   813,   669,     2,   780,
     903,   291,   861,   133,   808,    25,     1,   543,   831,   573,
     774,   383,    87,   403,   203,     1,   393,   518,     1,    61,
      32,    25,   943,    47,    32,    18,    32,    66,   103,    32,
      32,    32,    67,     3,     0,   229,    35,   121,    32,   103,
      56,   331,    47,    75,   869,   148,    36,    79,    80,    32,
      47,   121,    87,   430,   431,    75,     1,     3,   262,    79,
      80,    40,    41,    42,    56,    90,     3,   124,   103,    94,
     102,    75,   266,   176,   106,    79,    80,   119,    61,    75,
     142,     1,   102,    73,    54,    78,   106,    32,   150,    75,
     121,   599,   176,    24,    91,   603,   604,   291,   102,   476,
     477,    32,   106,   513,    97,  1008,   176,   171,    54,    40,
      41,    42,   111,   121,   171,     3,    61,    54,   121,   620,
     945,   114,   118,   106,   148,    83,   148,    47,    86,    47,
     540,   126,   118,    24,   104,     2,   119,   331,   761,   166,
     999,    32,   148,   148,   166,   176,   148,   537,    46,    40,
      41,    42,   147,   148,   166,  1076,   937,   173,    25,   171,
      47,   106,   943,   171,   147,   171,    54,   104,   171,   171,
     171,   176,  1031,  1032,   119,   998,   172,   171,   367,   174,
     744,   173,   946,   215,   474,   475,   172,    85,   171,   120,
     994,   141,    56,   570,   229,   215,   175,   121,   148,    27,
      47,    31,   147,   121,    91,    93,   126,   812,    75,   727,
     148,   215,    79,    80,   418,   587,   104,   115,    22,    47,
     119,   166,   172,    51,    56,   166,   171,   147,   148,   120,
     148,   266,   173,   148,   175,   102,   174,    56,   270,   106,
      56,  1100,   778,   275,   175,   439,   166,   148,    76,   148,
     270,    79,   382,    81,    27,   275,   291,   110,   176,   174,
      90,    27,    91,    91,    94,   148,   270,  1126,  1134,    53,
      24,   275,  1175,   174,   121,  1092,  1057,  1094,    32,   148,
     474,   475,   110,    46,   175,    89,    40,    41,    42,   797,
      74,   174,   526,   916,   173,  1076,   331,   148,   488,    11,
      84,   148,   148,    66,   148,   174,    79,    56,   148,   113,
      76,   402,   142,    79,   508,   944,    27,   147,    91,   351,
     150,   172,    85,   351,   154,    91,   148,    56,   174,   176,
     174,   351,   172,   365,  1200,  1201,  1202,  1203,  1204,  1205,
     173,   859,   374,   116,   110,   365,   174,   351,   215,     2,
     172,   148,   115,   392,   374,   387,   590,   148,   552,   398,
      45,   365,   173,   166,   175,    56,   120,   387,    79,   172,
     374,    27,    25,  1045,  1046,   166,    46,   174,   104,   573,
      91,   986,   900,   387,    69,   126,   418,   148,   463,   464,
      50,    47,    27,    53,   469,    51,    66,   472,   473,   110,
     148,   965,   166,   270,  1035,   903,   148,     8,   275,   173,
     148,   152,    47,   174,    74,    85,    51,    77,   166,    56,
      76,   175,    75,    79,    84,    81,    79,    80,   463,   464,
      79,    80,   174,   861,   469,    91,   174,   472,   473,   474,
     475,    76,   481,   880,    79,   115,    81,   538,   148,   102,
     110,    90,   166,   106,   110,    94,    91,   834,   172,   148,
     538,   538,    56,    56,   148,   148,   148,   499,   845,   501,
       2,    79,    80,   508,   174,   110,   148,   609,   855,   499,
     148,   501,   148,   166,   351,   174,  1091,   148,  1093,   564,
     174,   566,   174,    25,   148,   499,   148,   501,   365,  1130,
     166,   101,   174,  1011,  1012,   166,   174,   374,   173,     3,
    1008,   173,   166,   175,   166,  1023,  1024,   552,   174,   896,
     387,    90,   899,    90,   148,    94,    20,    94,   148,   564,
     569,   566,    56,   727,   126,   148,   148,   148,   573,    33,
     917,   918,   166,    75,   592,   577,   166,    79,    80,   577,
     744,    56,    31,   166,   166,   166,   173,   577,   175,   173,
      54,   153,   215,   155,   156,    56,   158,   159,    56,    63,
     102,   999,    27,   577,   106,  1140,  1084,  1085,  1086,  1087,
      90,   171,    56,   173,    94,   962,   142,   964,   173,    56,
     175,   147,    47,   278,   150,   280,    51,    90,   154,   292,
     293,    94,   287,  1031,   564,   173,   566,   175,  1036,     3,
     104,    90,   141,   807,   808,    94,   110,   270,   173,   150,
     175,    76,   275,     2,    79,   173,    81,   175,   171,    56,
     147,  1196,   499,   171,   501,   165,    91,   173,   166,   166,
     166,   166,   327,   833,   166,   166,    25,   166,    56,    85,
     173,   166,   166,   171,   166,   110,   148,   173,   147,   166,
     173,     8,   173,   142,   173,   859,   174,   742,   147,   166,
     149,   150,  1100,     3,   173,   154,   880,  1175,    66,    79,
      91,   171,   161,   215,    91,   126,   166,   150,    69,   764,
     169,   173,   727,   126,   148,   126,    75,   174,   351,    34,
      79,    80,   126,    34,    66,   737,   900,   742,   174,   744,
     577,   126,   365,   174,   174,   147,   748,    21,   171,   751,
     148,   374,   120,   102,   173,   172,   106,   106,   748,   764,
     126,   751,   150,    31,   387,   173,   150,   150,   270,   142,
     150,   150,   176,   275,   748,   150,   150,   751,   150,   150,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   150,   150,   150,   150,   166,
     861,   965,   150,   150,   166,   166,   867,   166,   173,   166,
    1070,  1071,    34,    34,   174,   876,   147,    47,   174,   148,
      34,   166,    90,   166,   170,   147,    94,   173,   172,   175,
     171,   840,   166,   173,   176,   172,   174,   176,   166,   148,
     176,   843,   166,   173,    32,   847,   173,    34,   174,   351,
     166,   176,   854,   843,   859,   174,   176,   847,   176,   174,
     174,   148,   166,   365,   854,   111,   215,   166,   165,   843,
     165,   572,   374,   847,   142,   143,   499,   965,   501,   147,
     854,   149,   150,   265,   374,   387,   154,   805,    24,   933,
     958,   367,   538,   161,   339,   900,   524,   880,   535,   167,
     168,   169,  1196,  1038,  1206,   305,  1070,  1071,   876,   867,
    1199,   748,   385,   832,   751,   917,   918,   391,   673,   131,
     521,   270,   924,    -1,   926,   760,   275,   929,    -1,    -1,
    1032,    -1,    -1,    -1,   924,    -1,   926,    -1,   999,   929,
     595,   596,    -1,    -1,  1118,   954,    -1,    -1,   603,    -1,
     924,   953,   926,    -1,   577,   929,    -1,    -1,    -1,    -1,
     965,   963,    -1,   953,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   963,    -1,    -1,    -1,    -1,    -1,   953,
      -1,    -1,    -1,    -1,  1002,  1003,    -1,    -1,    -1,   963,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   499,    -1,   501,
      -1,    -1,   351,    -1,    -1,    -1,   843,    -1,  1010,    -1,
     847,    -1,    -1,    -1,  1016,    -1,   365,   854,    -1,    -1,
    1010,    -1,    -1,     2,  1126,   374,  1016,    -1,  1130,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1010,    -1,   387,  1100,
      -1,    -1,  1016,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1228,    -1,  1230,    -1,    -1,  1233,
    1234,    -1,  1064,    -1,    -1,  1070,  1071,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1064,   577,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   924,    -1,   926,
    1064,    -1,   929,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1112,    -1,  1114,    -1,    -1,    -1,   953,  1119,    -1,    -1,
    1122,    -1,  1112,   102,  1114,   748,   963,   106,   751,  1119,
      -1,    -1,  1122,    -1,    -1,    -1,    -1,    -1,  1112,    -1,
    1114,     2,    -1,    -1,    -1,  1119,    -1,    -1,  1122,  1151,
     499,    -1,   501,    -1,    -1,  1157,    -1,    -1,    -1,  1161,
    1162,  1151,    -1,    -1,    25,    -1,    -1,  1157,    -1,    -1,
      -1,  1161,  1162,  1010,    -1,    -1,    -1,  1151,  1180,  1016,
     835,    31,  1184,  1157,  1186,    -1,    -1,  1161,  1162,    -1,
    1180,    -1,    -1,    -1,  1184,    -1,  1186,    -1,   853,    -1,
      -1,    -1,   857,    -1,    -1,  1207,  1180,    -1,    -1,  1207,
    1184,    -1,  1186,    -1,    75,    -1,    -1,  1207,    79,    80,
     843,    -1,    -1,    -1,   847,    -1,    -1,  1064,   577,    -1,
      -1,   854,    -1,  1207,    -1,    -1,   215,    -1,    -1,    -1,
      90,   102,    -1,    -1,    94,   106,   748,    -1,    -1,   751,
    1252,  1253,  1254,  1255,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1252,  1253,  1254,  1255,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1112,    -1,  1114,  1252,  1253,
    1254,  1255,  1119,    -1,    -1,  1122,    -1,    -1,    -1,    -1,
      -1,   270,   142,    -1,    -1,    -1,   275,   147,    -1,   149,
     150,   924,    -1,   926,   154,    -1,   929,    -1,    -1,    -1,
      -1,   161,    -1,    -1,  1151,    -1,    -1,   167,   168,   169,
    1157,    -1,    -1,    -1,  1161,  1162,    -1,    -1,    -1,    -1,
     953,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     963,   843,    -1,  1180,    31,   847,    -1,  1184,    -1,  1186,
      -1,    -1,   854,    -1,   215,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1023,  1024,
    1207,    -1,   351,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   365,  1010,    -1,    -1,
      -1,    -1,    -1,  1016,    -1,   374,    -1,    -1,    -1,   748,
      -1,    -1,   751,    90,    -1,    -1,    -1,    94,   387,   270,
      -1,    -1,    -1,    -1,   275,  1252,  1253,  1254,  1255,    -1,
      -1,    -1,   924,    -1,   926,  1080,    -1,   929,    -1,    -1,
      -1,  1086,  1087,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1064,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1104,
    1105,   953,  1107,    31,    -1,   142,   143,    -1,   145,   146,
     147,   963,   149,   150,    -1,    -1,  1121,   154,    -1,  1124,
      -1,    -1,    -1,   160,   161,    -1,    -1,   164,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,  1112,
     351,  1114,    -1,    -1,   843,    -1,  1119,    -1,   847,  1122,
      -1,    -1,    -1,    -1,   365,   854,    -1,    -1,  1010,    -1,
      -1,    -1,    90,   374,  1016,    -1,    94,    -1,    -1,    -1,
     499,    -1,   501,     2,    -1,    -1,   387,    -1,  1151,    -1,
      -1,    -1,    -1,    -1,  1157,    -1,    -1,    -1,  1161,  1162,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1180,    -1,    -1,
      -1,  1184,  1064,  1186,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   924,   154,   926,    -1,    -1,
     929,    -1,   160,   161,  1207,    -1,   164,    -1,    -1,   167,
     168,   169,    -1,    -1,    -1,    -1,    75,    -1,   577,    -1,
     178,    -1,    -1,    -1,   953,    -1,    -1,    -1,    -1,    -1,
    1112,    -1,  1114,    -1,   963,    -1,    -1,  1119,    -1,    -1,
    1122,    -1,    -1,   102,    -1,    -1,    -1,   106,    -1,  1252,
    1253,  1254,  1255,    -1,    -1,    -1,    -1,    -1,   499,    -1,
     501,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1151,
      -1,    -1,    -1,    22,    -1,  1157,    -1,    26,    27,  1161,
    1162,  1010,    -1,    -1,    -1,    -1,    35,  1016,    37,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,  1180,    -1,
      -1,    50,  1184,    28,  1186,    -1,    -1,     1,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      -1,    -1,    -1,    17,    18,  1207,    20,    -1,    77,    -1,
      79,    25,    -1,    -1,    -1,  1064,   577,    -1,    -1,    33,
      89,    -1,    91,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    49,   215,    -1,    52,    -1,
      54,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    63,
    1252,  1253,  1254,  1255,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,  1112,    78,  1114,    -1,    -1,    -1,   748,
    1119,    -1,   751,  1122,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    -1,   100,    -1,   102,    -1,
     104,   270,    -1,   107,    -1,    -1,   275,   111,    -1,    -1,
     114,    -1,  1151,   117,    -1,   150,    -1,    -1,  1157,    -1,
      -1,    -1,  1161,  1162,    -1,    -1,    -1,    -1,    -1,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1180,    -1,    -1,    -1,  1184,    -1,  1186,    -1,    -1,
       1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   199,    -1,    -1,    -1,  1207,   173,
      -1,    22,    -1,    -1,   843,    26,    27,    -1,   847,    -1,
      -1,    -1,   351,    -1,    35,   854,    37,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    -1,   365,   748,    -1,    50,
     751,    -1,    53,    -1,    -1,   374,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1252,  1253,  1254,  1255,    -1,   387,    -1,
      -1,    -1,    -1,    74,   259,   260,    77,    -1,    79,    -1,
      81,    -1,    83,    84,    -1,    86,   271,    -1,    89,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    -1,    -1,    -1,    -1,   924,    -1,   926,    -1,   110,
     929,    -1,   113,    17,    18,   116,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,   311,    -1,    -1,    33,
      -1,    -1,    -1,    -1,   953,    39,    -1,    -1,    -1,    -1,
     141,    45,   843,    -1,   963,    49,   847,    -1,    52,    -1,
      54,    -1,    -1,   854,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,   166,    70,    71,    -1,    -1,
      -1,   172,    -1,    -1,    78,    -1,    -1,   362,    -1,    -1,
     499,    -1,   501,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,  1010,    -1,    97,    98,    -1,   100,  1016,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,   112,    -1,
     114,    -1,   397,   117,    -1,    -1,   401,    -1,    -1,    -1,
      -1,    -1,    -1,   924,    -1,   926,    -1,    -1,   929,    -1,
      -1,    -1,    -1,   418,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    -1,  1064,    -1,    -1,    35,    -1,
      37,    38,   953,    -1,    -1,    -1,    43,    31,   577,    -1,
      -1,    -1,   963,    50,    -1,    -1,    -1,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      77,    -1,    79,  1112,    81,  1114,    83,    -1,   483,    86,
    1119,    -1,    89,  1122,    91,    -1,    -1,    -1,    -1,  1010,
      -1,    -1,    -1,    -1,    -1,  1016,    90,    -1,    -1,    -1,
      94,    -1,    -1,   110,   509,    -1,   113,    -1,   513,   116,
      -1,    -1,  1151,    -1,    -1,    -1,    -1,    -1,  1157,    -1,
      -1,   526,  1161,  1162,    -1,    -1,    -1,    -1,    90,    -1,
     535,    -1,    94,    -1,   141,   540,    -1,    -1,    -1,    -1,
      -1,  1180,    -1,  1064,    -1,  1184,    -1,  1186,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,    -1,    -1,   166,
     154,    -1,    -1,    -1,    -1,   172,    -1,   161,  1207,    -1,
      -1,    -1,    -1,   167,   168,   169,    -1,    -1,   583,    -1,
     142,   143,    -1,    -1,   146,   147,    -1,   149,   150,   594,
      -1,  1112,   154,  1114,    -1,    -1,    -1,    -1,  1119,   161,
      -1,  1122,    -1,    -1,   609,   167,   168,   169,    -1,   748,
      -1,    -1,   751,  1252,  1253,  1254,  1255,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1151,    -1,    -1,    -1,    -1,    -1,  1157,    -1,    -1,    -1,
    1161,  1162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,  1180,
      -1,    -1,    -1,  1184,    -1,  1186,    17,    18,   673,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,  1207,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,   843,    -1,    -1,    -1,   847,    -1,
      -1,    -1,    63,    -1,    -1,   854,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,  1252,  1253,  1254,  1255,    45,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,   753,   100,
      -1,   102,    -1,   104,    -1,   760,   107,    -1,    -1,    69,
     111,   112,    -1,   114,    -1,   770,   117,    -1,    -1,    79,
      80,    -1,    -1,    -1,    -1,    -1,    13,     3,    -1,    -1,
      17,    -1,    -1,   788,    -1,   924,    -1,   926,    -1,    26,
     929,    17,    -1,    -1,    20,    -1,    33,    -1,    -1,    25,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    45,    35,
      47,    48,    -1,    -1,   953,    -1,    53,    -1,    55,    45,
      57,    -1,   173,    -1,   963,    62,    52,   832,    54,    66,
      67,    68,    69,    70,    71,    -1,    -1,    63,    75,    -1,
      77,    -1,    -1,    -1,    -1,    71,    -1,    84,    -1,    -1,
      87,    -1,    -1,    90,    -1,    92,    -1,    -1,    95,    96,
      -1,    98,    88,    -1,    -1,   102,   103,   104,    -1,    -1,
      -1,  1010,    -1,    -1,   100,   880,    -1,  1016,   104,    -1,
      -1,    -1,    -1,    -1,    -1,   111,    -1,     3,    -1,    -1,
      -1,   117,    -1,    -1,   131,    -1,   133,    -1,    -1,    -1,
      -1,    17,    18,    -1,    20,    -1,    -1,    -1,    25,    25,
      -1,   221,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,  1064,    -1,    -1,   933,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,   174,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,   261,   262,   958,    70,    71,    -1,    -1,    75,   269,
     197,    -1,    78,    -1,   201,   202,    -1,    -1,   278,    -1,
     280,    -1,    88,  1112,    -1,  1114,    -1,   287,    -1,    -1,
    1119,    97,    98,  1122,   100,    -1,   102,    -1,   104,   106,
      -1,   107,   229,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1151,    -1,    -1,    -1,    -1,   327,  1157,    -1,
      -1,    -1,  1161,  1162,    -1,    -1,    -1,    -1,    -1,   266,
    1035,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1180,    -1,    -1,    -1,  1184,    -1,  1186,    -1,    -1,
      -1,    -1,    -1,    -1,   291,   171,    -1,   173,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   302,   303,    -1,  1207,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     317,   318,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   331,    -1,    -1,    -1,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1252,  1253,  1254,  1255,   354,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1130,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   375,    -1,
     377,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   386,
      -1,    -1,    -1,   270,   391,   392,   393,    -1,   275,    -1,
      -1,   398,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     427,   428,   429,   430,   431,   432,    -1,   434,   435,   436,
     437,   438,    -1,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
      -1,    -1,    -1,    -1,    -1,    -1,   463,   464,    -1,    -1,
      -1,    -1,   469,    -1,    -1,   472,   473,   474,   475,   476,
     477,    -1,    -1,    -1,   481,   482,    -1,   484,    -1,    -1,
      -1,    -1,    -1,   490,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,   501,    26,    27,   504,    -1,    -1,
     387,   508,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,
      -1,   518,    43,   520,    -1,   595,   596,   524,    -1,    50,
     527,   528,    -1,   603,   604,    -1,    -1,    -1,    -1,    -1,
      -1,   538,    -1,    -1,    -1,    -1,   543,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   552,    77,    -1,    79,    -1,
      81,    -1,    83,    -1,   561,    86,    -1,   564,    89,   566,
      91,    -1,   569,   570,    -1,    -1,   573,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   584,   585,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,   598,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,    -1,   499,   620,   501,    -1,    -1,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,   166,    22,    -1,    -1,    25,
      -1,   172,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    42,   664,    44,    45,
      -1,    -1,    -1,    49,   671,    -1,    52,    -1,    54,    -1,
      56,    -1,   752,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    -1,   788,    -1,
      -1,    97,    98,    99,   100,   101,   102,   797,   104,   726,
     727,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,   742,    -1,   744,   745,   746,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   835,   142,   764,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   778,    -1,   853,    -1,   782,    -1,   857,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   171,    -1,   173,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   805,    -1,
      -1,    -1,   809,    -1,   811,   812,   813,    -1,   815,    -1,
     817,    -1,    -1,   820,   821,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   834,    -1,   836,
      -1,    -1,    -1,   840,    -1,    -1,    -1,    -1,   845,   846,
     847,    -1,   849,   850,    -1,    -1,    -1,   854,   855,    -1,
      -1,    -1,   859,    -1,   934,    -1,    -1,   864,    22,    -1,
      -1,   748,    26,    27,   751,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    37,    38,    -1,    -1,    -1,    -1,    43,
      -1,   961,    -1,    -1,    -1,    -1,    50,    -1,    -1,   896,
     897,   898,   899,   900,    -1,    -1,   903,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     917,   918,    -1,    77,    -1,    79,    -1,    81,    -1,    83,
     927,    -1,    86,    -1,    -1,    89,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   944,    -1,    -1,
      -1,   948,    -1,  1023,  1024,    -1,   110,   954,    -1,   113,
      -1,    -1,   116,    -1,    -1,   962,   843,   964,   965,    -1,
     847,    -1,    -1,    -1,    -1,    -1,    -1,   854,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,   986,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   994,    -1,   996,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1006,
    1080,  1008,   166,    -1,    -1,    -1,  1086,  1087,   172,    -1,
      -1,    -1,    -1,    -1,  1021,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1104,  1105,    -1,  1107,    -1,    -1,
      -1,    -1,    -1,  1040,    -1,    -1,    -1,   924,  1118,   926,
      -1,  1121,   929,    -1,  1124,  1052,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1070,  1071,    -1,   953,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1082,   963,    -1,    -1,  1159,
      -1,    -1,    -1,  1090,  1091,  1092,  1093,  1094,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1112,    -1,  1114,    -1,    -1,
    1117,    -1,  1119,    -1,    -1,  1122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1010,    -1,    -1,    -1,    -1,    -1,  1016,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1144,  1145,    -1,
    1147,  1148,    -1,    -1,    -1,    -1,    -1,    -1,  1228,    -1,
    1230,    -1,    -1,  1233,  1234,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1170,    -1,    -1,    -1,    -1,  1175,    -1,
    1177,    -1,    -1,    -1,  1181,    -1,    -1,  1064,    -1,  1186,
    1187,    -1,    -1,  1190,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1198,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,  1212,  1213,  1214,  1215,  1216,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1112,    -1,  1114,    -1,    -1,
      -1,    -1,  1119,    -1,    -1,  1122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1252,  1253,  1254,  1255,  1256,
    1257,  1258,  1259,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1151,    -1,    -1,    -1,    -1,    -1,
    1157,    -1,    90,    -1,  1161,  1162,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1180,    -1,    -1,    -1,  1184,    -1,  1186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,    -1,    -1,   134,   135,   136,   137,
      -1,   139,   140,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,  1252,  1253,  1254,  1255,     0,
       1,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,   104,   105,    -1,   107,   108,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,    -1,    -1,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,    -1,     1,   178,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    12,    -1,    14,
      15,    16,    17,    18,    19,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    -1,    70,    71,    72,    -1,    74,
      -1,    -1,    77,    78,    79,    80,    -1,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,    -1,   104,
     105,    -1,   107,   108,   109,   110,   111,    -1,   113,   114,
     115,   116,   117,    -1,    -1,   120,    -1,   122,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,    -1,   171,   172,   173,    -1,
     175,    -1,     1,   178,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    92,    93,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,   105,    -1,   107,   108,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,    -1,
      -1,   120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,
     169,    -1,   171,   172,   173,    -1,   175,    -1,     1,   178,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
      -1,   104,   105,    -1,   107,   108,   109,   110,   111,    -1,
     113,   114,   115,   116,   117,    -1,    -1,   120,    -1,   122,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,   166,    -1,    -1,   169,    -1,   171,   172,
     173,    -1,   175,    -1,     1,   178,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,    -1,   104,   105,    -1,
     107,   108,   109,   110,   111,    -1,   113,   114,   115,   116,
     117,    -1,    -1,   120,    -1,   122,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,
      -1,    -1,   169,    -1,   171,   172,   173,    -1,   175,    -1,
       1,   178,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,   104,   105,    -1,   107,   108,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,    -1,    -1,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,    -1,
     171,   172,   173,    -1,   175,    -1,     1,   178,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    -1,    70,    71,    72,    -1,    74,
      -1,    -1,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,    -1,   104,
     105,    -1,   107,   108,   109,   110,   111,    -1,   113,   114,
     115,   116,   117,    -1,    -1,   120,    -1,   122,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
     175,    -1,     1,   178,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    92,    93,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,   105,    -1,   107,   108,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,    -1,
      -1,   120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,    -1,     1,   178,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
      -1,   104,   105,    -1,   107,   108,   109,   110,   111,    -1,
     113,   114,   115,   116,   117,    -1,    -1,   120,    -1,   122,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,   166,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,   175,    -1,     1,   178,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,    -1,   104,   105,    -1,
     107,   108,   109,   110,   111,    -1,   113,   114,   115,   116,
     117,    -1,    -1,   120,    -1,   122,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,
       1,   178,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      -1,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,   104,   105,    -1,   107,   108,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,    -1,    -1,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,    -1,     1,   178,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    -1,    70,    71,    72,    -1,    74,
      -1,    -1,    77,    78,    79,    80,    -1,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,    -1,   104,
     105,    -1,   107,   108,   109,   110,   111,    -1,   113,   114,
     115,   116,   117,    -1,    -1,   120,    -1,   122,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
     175,    -1,     1,   178,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    -1,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    92,    93,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,   105,    -1,   107,   108,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,    -1,
      -1,   120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,    -1,     1,   178,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
      -1,   104,   105,    -1,   107,   108,   109,   110,   111,    -1,
     113,   114,   115,   116,   117,    -1,    -1,   120,    -1,   122,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,   166,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,   175,    -1,     1,   178,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    42,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,
      -1,   178,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    12,    -1,    14,    -1,    -1,    17,    18,    19,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    55,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,    -1,    -1,   134,   135,   136,   137,    -1,   139,   140,
      -1,   142,   143,   144,   145,   146,   147,    -1,   149,    -1,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,    -1,   173,    -1,    -1,    -1,    -1,   178,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    12,    -1,    14,
      -1,    -1,    17,    18,    19,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      55,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,   129,   130,   131,    -1,    -1,   134,
     135,   136,   137,    -1,   139,   140,    -1,   142,   143,   144,
     145,   146,   147,    -1,   149,    -1,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,   167,   168,   169,   170,   171,    -1,   173,    -1,
      -1,    -1,    -1,   178,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    -1,    17,    18,
      -1,    20,    -1,    22,    23,    -1,    25,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    60,    -1,    62,    63,    64,    65,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,   105,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,    -1,    -1,    -1,   178,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,    -1,    -1,   178,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    42,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,   113,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,
      -1,   178,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    -1,    94,    -1,    -1,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,   148,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,    -1,
     171,   172,   173,    -1,    -1,    -1,    -1,   178,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,   108,   109,    -1,   111,   112,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,   152,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
      -1,    -1,    -1,   178,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    42,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,    -1,    -1,   178,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,    -1,    -1,   178,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,   108,   109,    -1,   111,   112,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,   174,    -1,    -1,
      -1,   178,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,   108,   109,    -1,
     111,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,   174,    -1,    -1,    -1,   178,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,   108,   109,    -1,   111,    -1,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
      -1,    -1,    -1,   178,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,   108,
     109,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,   174,    -1,    -1,    -1,   178,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,   108,   109,    -1,   111,    -1,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,   176,    -1,   178,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,   108,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,   174,    -1,    -1,
      -1,   178,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    69,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,   108,   109,    -1,
     111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,    -1,    -1,    -1,   178,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,   148,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,    -1,   171,   172,   173,    -1,
      -1,    -1,    -1,   178,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,   148,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,
     169,    -1,   171,   172,   173,    -1,    -1,    -1,    -1,   178,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,   148,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,   166,    -1,    -1,   169,    -1,   171,   172,
     173,    -1,    -1,    -1,    -1,   178,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,    -1,
      -1,   178,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,   108,   109,    -1,
     111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,    -1,    -1,    -1,   178,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    -1,    94,
      -1,    -1,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
      -1,    -1,    -1,   178,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,   123,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,    -1,   176,    -1,   178,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,   148,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,   174,    -1,    -1,    -1,   178,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,    -1,
      -1,   178,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,   123,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,    -1,    -1,    -1,   178,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
      -1,   176,    -1,   178,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,   172,   173,    -1,    -1,    -1,    -1,   178,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,   112,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,    -1,    -1,   178,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,   176,
      -1,   178,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,    -1,   176,    -1,   178,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,   112,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
      -1,    -1,    -1,   178,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,    -1,    -1,    -1,   178,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,    -1,    -1,   178,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,    -1,
      -1,   178,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,    -1,    -1,    -1,   178,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
      -1,    -1,    -1,   178,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,    -1,    -1,    -1,   178,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,     4,
       5,     6,     7,     8,     9,    10,    -1,   160,    -1,    14,
     163,   164,   165,    18,    -1,    -1,   169,    22,   171,    -1,
     173,    -1,    27,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    42,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    79,    80,    81,    82,    -1,    -1,
      -1,    -1,    87,    -1,    89,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    97,    -1,    99,    -1,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,   174,
     175,    -1,    -1,   178,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    18,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    -1,    -1,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    79,
      80,    81,    82,    -1,    -1,    -1,    -1,    87,    -1,    89,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    97,    -1,    99,
      -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,    -1,   142,   143,   144,   145,   146,    -1,    -1,    12,
      -1,   151,    -1,    -1,    17,    18,    19,    20,    -1,    -1,
     160,    -1,    25,   163,   164,   165,    -1,    -1,    -1,   169,
      33,   171,    35,   173,    -1,   175,    39,    -1,   178,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    -1,   100,    -1,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,   129,   130,   131,    -1,
      -1,   134,   135,   136,   137,    -1,   139,   140,    -1,   142,
     143,   144,   145,   146,   147,    -1,   149,    -1,    -1,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
      -1,   164,    -1,    -1,   167,   168,   169,   170,    -1,    -1,
     173,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,    -1,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,   175,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    42,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
       3,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,   142,    -1,   144,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
      63,    -1,    -1,    -1,   173,    -1,   175,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    -1,   100,    -1,   102,
      -1,   104,    -1,     3,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    12,    -1,   117,    -1,    -1,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    -1,
     100,    -1,   102,    -1,   104,    -1,    -1,    -1,    -1,    -1,
     110,   111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
     130,   131,    -1,    -1,   134,   135,   136,   137,    -1,   139,
     140,    -1,   142,   143,   144,   145,   146,   147,    -1,   149,
      -1,    -1,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,     3,   164,    -1,    -1,   167,   168,   169,
     170,   171,    12,    -1,    -1,    -1,    -1,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    97,    98,    -1,
     100,    19,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,    31,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
     130,   131,    -1,    -1,   134,   135,   136,   137,    -1,   139,
     140,    -1,   142,   143,   144,   145,   146,   147,    -1,   149,
      -1,    -1,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    -1,   164,    -1,    -1,   167,   168,   169,
     170,    -1,    90,    -1,    12,    -1,    94,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,    -1,    -1,   134,   135,   136,   137,
      -1,   139,   140,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    90,   171,    12,    -1,    94,    -1,    -1,    -1,
     178,    19,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,    -1,    -1,   134,   135,   136,   137,
      -1,   139,   140,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    90,   171,    12,    -1,    94,    -1,    -1,    -1,
     178,    19,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,    -1,    -1,   134,   135,   136,   137,
      -1,   139,   140,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    90,   171,    12,    -1,    94,    -1,    -1,    -1,
     178,    19,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,    -1,    -1,   134,   135,   136,   137,
      -1,   139,   140,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    90,   171,    -1,    -1,    94,    -1,    -1,    12,
     178,    -1,    -1,    -1,    -1,   103,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,    47,    -1,   134,   135,   136,   137,
      -1,   139,   140,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,   171,    -1,    12,    -1,    90,    -1,    -1,
     178,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      47,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    90,   167,   168,   169,    94,   171,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    31,    -1,   125,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    73,    -1,
     167,   168,   169,    31,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
     125,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    94,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      73,   166,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    12,    -1,    90,    -1,    -1,
      -1,    94,    19,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    31,    32,   154,   155,   156,    -1,
     158,   159,   160,   161,    -1,    -1,   164,    -1,    -1,   167,
     168,   169,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    90,   167,   168,   169,    94,    -1,    -1,
      12,    -1,    -1,    -1,    -1,   178,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,   121,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,   171,    -1,    12,    -1,    90,    -1,
      -1,   178,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    90,   167,   168,   169,    94,   171,
      -1,    12,    -1,    -1,    -1,    -1,   178,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,   171,    -1,    12,    -1,    90,
      -1,    -1,   178,    94,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,    47,    -1,   125,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    31,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    90,   167,   168,   169,    94,
      -1,    -1,    -1,    12,    -1,    -1,    -1,   178,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   103,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,   125,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,   178,    -1,    94,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    12,   121,   167,   168,   169,   125,   171,    19,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,   142,   143,    36,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    12,    -1,   167,   168,
     169,    -1,    -1,    19,    -1,    -1,    -1,   176,    -1,   178,
      -1,    -1,    73,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,    12,    13,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,   121,   167,   168,   169,   125,
      -1,    -1,    -1,    -1,    90,    -1,    -1,   178,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   125,
      -1,   167,   168,   169,    -1,    -1,    -1,    12,    -1,    -1,
     176,    -1,   178,    -1,    19,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    31,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,   178,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     125,    -1,   167,   168,   169,    -1,   171,    -1,    12,    -1,
      90,    -1,    -1,   178,    94,    19,    -1,   142,   143,    -1,
     145,   146,   147,   103,   149,   150,   151,    31,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,   166,   167,   168,   169,   125,    -1,    -1,    -1,    12,
      13,    -1,    -1,   178,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    31,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    90,   167,   168,   169,
      94,    -1,    -1,    -1,    -1,    12,    -1,    -1,   178,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,   125,   167,   168,   169,    -1,    -1,    -1,    -1,
     174,    -1,    -1,    90,   178,    -1,    -1,    94,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    12,    -1,   167,   168,   169,   124,   125,    19,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    34,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    12,    -1,
     167,   168,   169,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      94,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,   125,    -1,    -1,    -1,    -1,    90,    -1,   178,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   125,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,
     174,    -1,    -1,    -1,   178,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    12,    13,   167,   168,   169,    -1,    -1,    19,    -1,
     174,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    12,    13,    94,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   125,    -1,   167,   168,   169,    90,
      -1,    -1,    12,    94,    -1,    -1,    -1,   178,    -1,    19,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    31,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   125,    -1,   167,   168,   169,    -1,
      -1,    -1,    12,   174,    -1,    -1,    -1,   178,    -1,    19,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    31,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,   178,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   125,    -1,   167,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,   178,    -1,
      94,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    12,   166,   167,   168,   169,
     124,   125,    19,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    12,    -1,   167,   168,   169,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,   125,    -1,    -1,    -1,    -1,   176,
      90,   178,    12,    -1,    94,    -1,    -1,    -1,    -1,    19,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    31,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,   125,   167,   168,   169,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,   178,    -1,    -1,
      19,    61,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    31,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      90,    -1,    -1,    -1,    94,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    12,
      -1,    90,    -1,    -1,    -1,    94,    19,    -1,    -1,    -1,
      -1,    -1,   142,   143,   103,   145,   146,   147,    31,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,   125,   167,   168,   169,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,   178,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    31,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    90,   167,   168,
     169,    94,    -1,    -1,    -1,    12,    -1,    -1,    -1,   178,
     103,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   103,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,   125,   167,   168,   169,    -1,    -1,    -1,
      -1,    12,    -1,    90,    -1,   178,    -1,    94,    19,    -1,
     142,   143,    -1,   145,   146,   147,   103,   149,   150,   151,
      31,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,   125,    -1,
      -1,    -1,    12,    -1,    -1,    -1,   178,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    31,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    90,
     167,   168,   169,    94,    -1,    -1,    -1,    12,    -1,    -1,
      -1,   178,   103,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,   125,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,   178,    -1,    94,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    31,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    22,   167,   168,   169,    26,    27,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    35,    -1,    37,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    90,    -1,    -1,    -1,
      94,    -1,    -1,    74,    -1,    -1,    77,    -1,    79,    -1,
      -1,    -1,    83,    84,    85,    86,    -1,    31,    89,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    90,    -1,   142,   143,
      94,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,    -1,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    90,    -1,    -1,    -1,
      94,   125,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,   142,   143,
     171,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,    -1,   158,   159,   160,   161,   162,    -1,
     164,    -1,    -1,   167,   168,   169,    -1,    -1,   142,   143,
      -1,   145,   146,   147,   178,   149,   150,   151,    -1,   153,
     154,   155,   156,    -1,   158,   159,   160,   161,   162,    -1,
     164,    -1,    22,   167,   168,   169,    26,    27,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    35,    -1,    37,    38,    -1,
      22,    -1,    -1,    43,    26,    27,    -1,    -1,    -1,    -1,
      50,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    83,    -1,    -1,    86,    -1,    -1,    89,
      -1,    91,    -1,    -1,    -1,    77,    -1,    79,     1,    -1,
       3,    83,    -1,    -1,    86,    -1,    -1,    89,    -1,    91,
     110,    -1,    -1,   113,    17,    18,   116,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      33,   113,    -1,    -1,   116,    -1,    39,    -1,    -1,    -1,
      -1,   141,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    -1,    -1,    -1,     3,    -1,    -1,   141,
      63,    -1,    -1,    -1,    -1,    -1,   166,    70,    71,    -1,
      17,    18,    -1,    20,    -1,    78,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,   166,    88,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    97,    98,    -1,   100,    45,   102,
      -1,   104,    49,    -1,   107,    52,    -1,    54,   111,    -1,
      -1,   114,     3,    -1,   117,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    17,    18,    -1,    20,
      -1,    78,    -1,    -1,    25,    -1,    27,    -1,   141,    -1,
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
     185,   148,   172,   370,   148,   174,   148,   174,     8,   175,
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
     172,   174,   174,   342,   174,   342,   341,   370,   197,   174,
     199,   200,   346,   172,   172,   148,   166,    13,   150,   148,
     166,    13,    36,    73,   221,   142,   143,   144,   145,   146,
     160,   164,   169,   209,   296,   297,   298,   370,   209,   211,
     268,   171,   238,   325,    47,   356,   120,   221,   226,   226,
     173,   370,   121,   176,   359,    47,   148,   176,   359,   124,
     148,   176,   226,   359,    32,    47,   226,   359,   166,   173,
       1,   269,   172,   275,   126,   148,   172,   148,   174,   337,
       1,   225,   333,   334,   106,   312,   173,   300,   370,   142,
     150,   300,   300,   370,   323,   173,   175,   166,   166,   166,
     166,   166,   166,   174,   176,   345,    47,   176,    32,    47,
     173,    47,    91,   148,   174,    17,    20,    25,    45,    52,
      63,    71,    88,   100,   111,   117,   347,    90,    90,   166,
     372,   346,   370,   370,    34,   187,    34,    34,   187,   103,
     207,   221,   174,   148,   174,   174,   330,   337,    69,   372,
     221,   174,   174,   340,   147,   284,   174,   338,   152,   299,
     332,   370,   172,    75,   118,   172,   261,   174,   173,   207,
     221,   257,    47,   176,    47,   148,   174,   215,   222,    17,
      18,    20,    25,    45,    49,    52,    71,    78,    88,    97,
      98,   100,   102,   111,   114,   117,   171,   214,   298,   370,
     112,   370,   209,   211,   148,   166,    13,   166,   171,   269,
     322,   328,   244,   370,   226,   370,    47,   341,   173,   187,
     176,   244,   370,   176,   187,   370,   176,   370,   176,   370,
     226,    44,   370,   244,   370,   226,   345,   172,    83,    86,
     172,   186,   194,   228,   370,   273,   274,   304,   312,    61,
     119,   316,   302,   303,   174,   295,   297,   174,   176,   174,
     244,   370,    44,   244,   370,   345,   361,   337,   347,   347,
     187,   187,   370,   187,    34,   166,   166,   249,   207,   332,
     173,   173,   299,   332,   333,   312,   340,   370,   187,   240,
     341,   257,   147,   207,   244,   370,   244,   370,   200,   209,
      13,    36,    73,    36,    73,   166,   166,   298,   370,   370,
     269,   172,   166,   166,   356,   356,   166,   370,   174,   360,
     361,   187,   176,   359,   176,   359,   187,   124,   370,    32,
     226,   359,    32,   226,   359,   174,   194,   228,   228,   275,
     194,   316,     3,    54,    93,   104,   317,   318,   319,   370,
     291,   174,   300,   300,   176,   176,   370,    32,    32,   174,
     332,    34,   187,   344,   344,   299,   332,    32,   226,   174,
     370,   176,   176,   172,   112,   370,   209,   211,   209,   211,
      13,   172,   226,   226,   226,   148,   174,    44,   187,   370,
     176,    44,   187,   370,   176,   370,   103,    44,   370,   226,
      44,   370,   226,   166,   228,   275,   278,   319,   119,   148,
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
#line 6032 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 639 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6038 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 640 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6044 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 647 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6052 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 655 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6060 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 659 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6069 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 669 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6077 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 674 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6085 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 681 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6091 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 682 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6097 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 683 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6103 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 684 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6109 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 685 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6115 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6121 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 687 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6127 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 688 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6133 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 689 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6139 "bison-chpl-lib.cpp"
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
    CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6157 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 704 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6163 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 705 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6169 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: TINIT TTHIS TSEMI  */
#line 707 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr));
    auto node = Init::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6181 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 715 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6194 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 724 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6207 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TLABEL ident_def stmt  */
#line 733 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6215 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 737 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6226 "bison-chpl-lib.cpp"
    break;

  case 29: /* stmt_base: error TSEMI  */
#line 744 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6235 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: assignment_stmt  */
#line 751 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6241 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: if_stmt  */
#line 752 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6247 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: loop_stmt  */
#line 753 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6253 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: select_stmt  */
#line 754 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6259 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: manage_stmt  */
#line 755 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6265 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 756 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6271 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: throw_stmt  */
#line 757 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6277 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 759 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6283 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 761 "chpl.ypp"
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
    CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6303 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 777 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6315 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 785 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Local::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)), blockStyle,
                             std::move(stmts));
    BUILDER->noteBlockHeaderLocation(node.get(), LOC2((yylsp[-2]), (yylsp[-1])));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6332 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TLOCAL do_stmt  */
#line 798 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-1]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Local::build(BUILDER, LOC((yyloc)), blockStyle, std::move(stmts));
    BUILDER->noteBlockHeaderLocation(node.get(), LOC((yylsp[-1])));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6348 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TON expr do_stmt  */
#line 810 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = On::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)), blockStyle,
                          std::move(stmts));
    BUILDER->noteBlockHeaderLocation(node.get(), LOC2((yylsp[-2]), (yylsp[-1])));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6365 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 823 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Serial::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)), blockStyle,
                              std::move(stmts));
    BUILDER->noteBlockHeaderLocation(node.get(), LOC2((yylsp[-2]), (yylsp[-1])));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6382 "bison-chpl-lib.cpp"
    break;

  case 44: /* tryable_stmt: TSERIAL do_stmt  */
#line 836 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-1]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Serial::build(BUILDER, LOC((yyloc)), blockStyle, std::move(stmts));
    BUILDER->noteBlockHeaderLocation(node.get(), LOC((yylsp[-1])));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6398 "bison-chpl-lib.cpp"
    break;

  case 45: /* tryable_stmt: TSYNC stmt  */
#line 848 "chpl.ypp"
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
    CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6416 "bison-chpl-lib.cpp"
    break;

  case 48: /* collect_attributes: %empty  */
#line 869 "chpl.ypp"
  {
    auto attributeGroup = context->buildAttributeGroup((yyloc));
    if (attributeGroup != nullptr) {
      context->resetAttributeGroupPartsState();
    }
    context->loopAttributes.push_back(std::move(attributeGroup));
  }
#line 6428 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 894 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6436 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATMARK TIDENT  */
#line 901 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6445 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 908 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6453 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 912 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6464 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 922 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6470 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 924 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6478 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 928 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6486 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 935 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6495 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 940 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6505 "bison-chpl-lib.cpp"
    break;

  case 64: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 953 "chpl.ypp"
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
#line 6527 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 974 "chpl.ypp"
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
#line 6548 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 991 "chpl.ypp"
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
#line 6569 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1008 "chpl.ypp"
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
#line 6588 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1025 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6594 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1026 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6601 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1028 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6608 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1033 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6614 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1034 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6621 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1040 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6631 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1058 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6639 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1062 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6647 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR error TRCBR  */
#line 1066 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6659 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt: block_stmt_body  */
#line 1077 "chpl.ypp"
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
#line 6678 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1095 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6684 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1096 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6690 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1101 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6698 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1105 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6706 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1109 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6715 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1114 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6723 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1118 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6731 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1122 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6740 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1132 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6749 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1137 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6759 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1143 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6770 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1150 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6779 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1155 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6790 "bison-chpl-lib.cpp"
    break;

  case 91: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1162 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6802 "bison-chpl-lib.cpp"
    break;

  case 92: /* opt_only_ls: %empty  */
#line 1172 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6808 "bison-chpl-lib.cpp"
    break;

  case 93: /* opt_only_ls: renames_ls  */
#line 1173 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6814 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1178 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6823 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1183 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6835 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1191 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6848 "bison-chpl-lib.cpp"
    break;

  case 97: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1200 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6862 "bison-chpl-lib.cpp"
    break;

  case 98: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1210 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6874 "bison-chpl-lib.cpp"
    break;

  case 99: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1218 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6887 "bison-chpl-lib.cpp"
    break;

  case 100: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1227 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6901 "bison-chpl-lib.cpp"
    break;

  case 101: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1240 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6910 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_expr: expr  */
#line 1276 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6918 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_expr: expr TDOT all_op_name  */
#line 1280 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6927 "bison-chpl-lib.cpp"
    break;

  case 121: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1285 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6936 "bison-chpl-lib.cpp"
    break;

  case 122: /* import_expr: expr TAS ident_use  */
#line 1290 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6946 "bison-chpl-lib.cpp"
    break;

  case 123: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1296 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6956 "bison-chpl-lib.cpp"
    break;

  case 124: /* import_ls: import_expr  */
#line 1304 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6962 "bison-chpl-lib.cpp"
    break;

  case 125: /* import_ls: import_ls TCOMMA import_expr  */
#line 1305 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6968 "bison-chpl-lib.cpp"
    break;

  case 126: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1310 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6978 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1319 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6986 "bison-chpl-lib.cpp"
    break;

  case 128: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1323 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6994 "bison-chpl-lib.cpp"
    break;

  case 129: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1327 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7002 "bison-chpl-lib.cpp"
    break;

  case 130: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1331 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7010 "bison-chpl-lib.cpp"
    break;

  case 131: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1335 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7018 "bison-chpl-lib.cpp"
    break;

  case 132: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1339 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7026 "bison-chpl-lib.cpp"
    break;

  case 133: /* opt_label_ident: %empty  */
#line 1347 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7032 "bison-chpl-lib.cpp"
    break;

  case 134: /* opt_label_ident: TIDENT  */
#line 1348 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7038 "bison-chpl-lib.cpp"
    break;

  case 135: /* opt_label_ident: TINIT  */
#line 1349 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7044 "bison-chpl-lib.cpp"
    break;

  case 144: /* ident_use: TIDENT  */
#line 1375 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7050 "bison-chpl-lib.cpp"
    break;

  case 145: /* ident_use: TINIT  */
#line 1376 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7056 "bison-chpl-lib.cpp"
    break;

  case 146: /* ident_use: TTHIS  */
#line 1377 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7062 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TBOOL  */
#line 1404 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7068 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TENUM  */
#line 1405 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7074 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TINT  */
#line 1406 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7080 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TUINT  */
#line 1407 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7086 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TREAL  */
#line 1408 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7092 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TIMAG  */
#line 1409 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7098 "bison-chpl-lib.cpp"
    break;

  case 172: /* scalar_type: TCOMPLEX  */
#line 1410 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7104 "bison-chpl-lib.cpp"
    break;

  case 173: /* scalar_type: TBYTES  */
#line 1411 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7110 "bison-chpl-lib.cpp"
    break;

  case 174: /* scalar_type: TSTRING  */
#line 1412 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7116 "bison-chpl-lib.cpp"
    break;

  case 175: /* scalar_type: TLOCALE  */
#line 1413 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7122 "bison-chpl-lib.cpp"
    break;

  case 176: /* scalar_type: TNOTHING  */
#line 1414 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7128 "bison-chpl-lib.cpp"
    break;

  case 177: /* scalar_type: TVOID  */
#line 1415 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7134 "bison-chpl-lib.cpp"
    break;

  case 182: /* do_stmt: TDO stmt  */
#line 1429 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7140 "bison-chpl-lib.cpp"
    break;

  case 183: /* do_stmt: block_stmt  */
#line 1430 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7146 "bison-chpl-lib.cpp"
    break;

  case 184: /* return_stmt: TRETURN TSEMI  */
#line 1435 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7157 "bison-chpl-lib.cpp"
    break;

  case 185: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1442 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7168 "bison-chpl-lib.cpp"
    break;

  case 186: /* class_level_stmt: TSEMI  */
#line 1452 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7176 "bison-chpl-lib.cpp"
    break;

  case 187: /* class_level_stmt: inner_class_level_stmt  */
#line 1456 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7186 "bison-chpl-lib.cpp"
    break;

  case 188: /* $@1: %empty  */
#line 1461 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7193 "bison-chpl-lib.cpp"
    break;

  case 189: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1463 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7202 "bison-chpl-lib.cpp"
    break;

  case 190: /* $@2: %empty  */
#line 1467 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7209 "bison-chpl-lib.cpp"
    break;

  case 191: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1469 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7218 "bison-chpl-lib.cpp"
    break;

  case 199: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1487 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7227 "bison-chpl-lib.cpp"
    break;

  case 200: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1492 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7236 "bison-chpl-lib.cpp"
    break;

  case 201: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1497 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7245 "bison-chpl-lib.cpp"
    break;

  case 202: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1502 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7253 "bison-chpl-lib.cpp"
    break;

  case 203: /* forwarding_decl_start: TFORWARDING  */
#line 1509 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7262 "bison-chpl-lib.cpp"
    break;

  case 204: /* extern_or_export: TEXTERN  */
#line 1516 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7268 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_or_export: TEXPORT  */
#line 1517 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7274 "bison-chpl-lib.cpp"
    break;

  case 206: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1522 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7284 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1532 "chpl.ypp"
  {
    auto headerLoc = context->makeSpannedLocation((yylsp[-5]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7294 "bison-chpl-lib.cpp"
    break;

  case 208: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1539 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    auto headerLoc = context->makeSpannedLocation((yylsp[-6]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7306 "bison-chpl-lib.cpp"
    break;

  case 209: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1547 "chpl.ypp"
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
#line 7328 "bison-chpl-lib.cpp"
    break;

  case 210: /* $@3: %empty  */
#line 1565 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7336 "bison-chpl-lib.cpp"
    break;

  case 211: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1569 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7345 "bison-chpl-lib.cpp"
    break;

  case 212: /* $@4: %empty  */
#line 1574 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7354 "bison-chpl-lib.cpp"
    break;

  case 213: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1579 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7363 "bison-chpl-lib.cpp"
    break;

  case 214: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1587 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7371 "bison-chpl-lib.cpp"
    break;

  case 215: /* no_loop_attributes: %empty  */
#line 1592 "chpl.ypp"
                    { context->loopAttributes.push_back(nullptr); }
#line 7377 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1595 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7383 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1596 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7389 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1612 "chpl.ypp"
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
#line 7412 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1631 "chpl.ypp"
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
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7431 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1646 "chpl.ypp"
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
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7450 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1661 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7459 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1666 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7468 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1671 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7477 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1676 "chpl.ypp"
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
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    context->resetAttributeGroupPartsState();
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7502 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1697 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7511 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: for_loop_kw expr TIN expr forall_intent_clause do_stmt  */
#line 1702 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7520 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1707 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7529 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1712 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7538 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1717 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7547 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: for_loop_kw expr forall_intent_clause do_stmt  */
#line 1722 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7556 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1727 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7565 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: for_loop_kw zippered_iterator forall_intent_clause do_stmt  */
#line 1732 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7574 "bison-chpl-lib.cpp"
    break;

  case 236: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1737 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7583 "bison-chpl-lib.cpp"
    break;

  case 237: /* loop_stmt_base: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1742 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7592 "bison-chpl-lib.cpp"
    break;

  case 238: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1747 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7601 "bison-chpl-lib.cpp"
    break;

  case 239: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1752 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7610 "bison-chpl-lib.cpp"
    break;

  case 240: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1757 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7619 "bison-chpl-lib.cpp"
    break;

  case 241: /* loop_stmt_base: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1762 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7628 "bison-chpl-lib.cpp"
    break;

  case 242: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1767 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7638 "bison-chpl-lib.cpp"
    break;

  case 243: /* loop_stmt_base: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1773 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7648 "bison-chpl-lib.cpp"
    break;

  case 244: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1782 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7658 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF expr TTHEN stmt  */
#line 1791 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7666 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF expr block_stmt  */
#line 1795 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7674 "bison-chpl-lib.cpp"
    break;

  case 247: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1799 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7682 "bison-chpl-lib.cpp"
    break;

  case 248: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1803 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7690 "bison-chpl-lib.cpp"
    break;

  case 249: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1807 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7698 "bison-chpl-lib.cpp"
    break;

  case 250: /* if_stmt: TIF ifvar block_stmt  */
#line 1811 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7706 "bison-chpl-lib.cpp"
    break;

  case 251: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1815 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7714 "bison-chpl-lib.cpp"
    break;

  case 252: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1819 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7722 "bison-chpl-lib.cpp"
    break;

  case 253: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1823 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7731 "bison-chpl-lib.cpp"
    break;

  case 254: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1828 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7740 "bison-chpl-lib.cpp"
    break;

  case 255: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1833 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7749 "bison-chpl-lib.cpp"
    break;

  case 256: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1838 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7758 "bison-chpl-lib.cpp"
    break;

  case 257: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1846 "chpl.ypp"
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
#line 7777 "bison-chpl-lib.cpp"
    break;

  case 258: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1861 "chpl.ypp"
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
#line 7796 "bison-chpl-lib.cpp"
    break;

  case 259: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1879 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-4]), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7804 "bison-chpl-lib.cpp"
    break;

  case 260: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1883 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7812 "bison-chpl-lib.cpp"
    break;

  case 261: /* ifc_formal_ls: ifc_formal  */
#line 1889 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7818 "bison-chpl-lib.cpp"
    break;

  case 262: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1890 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7824 "bison-chpl-lib.cpp"
    break;

  case 263: /* ifc_formal: ident_def  */
#line 1895 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7830 "bison-chpl-lib.cpp"
    break;

  case 283: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1939 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7838 "bison-chpl-lib.cpp"
    break;

  case 284: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1943 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7846 "bison-chpl-lib.cpp"
    break;

  case 285: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1947 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7854 "bison-chpl-lib.cpp"
    break;

  case 286: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1954 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7862 "bison-chpl-lib.cpp"
    break;

  case 287: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1958 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7870 "bison-chpl-lib.cpp"
    break;

  case 288: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1962 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7878 "bison-chpl-lib.cpp"
    break;

  case 289: /* try_stmt: TTRY tryable_stmt  */
#line 1969 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7886 "bison-chpl-lib.cpp"
    break;

  case 290: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1973 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7894 "bison-chpl-lib.cpp"
    break;

  case 291: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1977 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7902 "bison-chpl-lib.cpp"
    break;

  case 292: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1981 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7910 "bison-chpl-lib.cpp"
    break;

  case 293: /* catch_expr_ls: %empty  */
#line 1987 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7916 "bison-chpl-lib.cpp"
    break;

  case 294: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1988 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7922 "bison-chpl-lib.cpp"
    break;

  case 295: /* catch_expr: TCATCH block_stmt  */
#line 1993 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 7930 "bison-chpl-lib.cpp"
    break;

  case 296: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1997 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 7938 "bison-chpl-lib.cpp"
    break;

  case 297: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2001 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 7946 "bison-chpl-lib.cpp"
    break;

  case 298: /* catch_expr_inner: ident_def  */
#line 2008 "chpl.ypp"
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
#line 7963 "bison-chpl-lib.cpp"
    break;

  case 299: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2021 "chpl.ypp"
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
#line 7980 "bison-chpl-lib.cpp"
    break;

  case 300: /* throw_stmt: TTHROW expr TSEMI  */
#line 2037 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7990 "bison-chpl-lib.cpp"
    break;

  case 301: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2045 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 7998 "bison-chpl-lib.cpp"
    break;

  case 302: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2049 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8008 "bison-chpl-lib.cpp"
    break;

  case 303: /* when_stmt_ls: %empty  */
#line 2057 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8014 "bison-chpl-lib.cpp"
    break;

  case 304: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2058 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8020 "bison-chpl-lib.cpp"
    break;

  case 305: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2063 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8028 "bison-chpl-lib.cpp"
    break;

  case 306: /* when_stmt: TOTHERWISE stmt  */
#line 2067 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8037 "bison-chpl-lib.cpp"
    break;

  case 307: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2072 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 8047 "bison-chpl-lib.cpp"
    break;

  case 308: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2081 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8056 "bison-chpl-lib.cpp"
    break;

  case 309: /* manager_expr: expr TAS ident_def  */
#line 2086 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8064 "bison-chpl-lib.cpp"
    break;

  case 310: /* manager_expr: expr  */
#line 2090 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8072 "bison-chpl-lib.cpp"
    break;

  case 311: /* manager_expr_ls: manager_expr  */
#line 2096 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8078 "bison-chpl-lib.cpp"
    break;

  case 312: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2097 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8084 "bison-chpl-lib.cpp"
    break;

  case 313: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2102 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), locHeader, (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8093 "bison-chpl-lib.cpp"
    break;

  case 314: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2112 "chpl.ypp"
    {
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8103 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2118 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8115 "bison-chpl-lib.cpp"
    break;

  case 316: /* class_start: class_tag ident_def  */
#line 2130 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8123 "bison-chpl-lib.cpp"
    break;

  case 317: /* class_tag: TCLASS  */
#line 2136 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8129 "bison-chpl-lib.cpp"
    break;

  case 318: /* class_tag: TRECORD  */
#line 2137 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8135 "bison-chpl-lib.cpp"
    break;

  case 319: /* class_tag: TUNION  */
#line 2138 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8141 "bison-chpl-lib.cpp"
    break;

  case 320: /* opt_inherit: %empty  */
#line 2142 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8147 "bison-chpl-lib.cpp"
    break;

  case 321: /* opt_inherit: TCOLON expr_ls  */
#line 2143 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8153 "bison-chpl-lib.cpp"
    break;

  case 322: /* class_level_stmt_ls: %empty  */
#line 2147 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8162 "bison-chpl-lib.cpp"
    break;

  case 323: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2152 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8170 "bison-chpl-lib.cpp"
    break;

  case 324: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2156 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8178 "bison-chpl-lib.cpp"
    break;

  case 325: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2160 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8186 "bison-chpl-lib.cpp"
    break;

  case 326: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2164 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8194 "bison-chpl-lib.cpp"
    break;

  case 327: /* enum_decl_stmt: enum_header TLCBR enum_ls TRCBR  */
#line 2172 "chpl.ypp"
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
#line 8217 "bison-chpl-lib.cpp"
    break;

  case 328: /* enum_decl_stmt: enum_header TLCBR error TRCBR  */
#line 2191 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-3].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8231 "bison-chpl-lib.cpp"
    break;

  case 329: /* enum_header: TENUM ident_def  */
#line 2204 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Enum);
  }
#line 8239 "bison-chpl-lib.cpp"
    break;

  case 330: /* enum_ls: enum_item  */
#line 2211 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8248 "bison-chpl-lib.cpp"
    break;

  case 331: /* enum_ls: enum_ls TCOMMA  */
#line 2216 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8258 "bison-chpl-lib.cpp"
    break;

  case 332: /* $@5: %empty  */
#line 2222 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8267 "bison-chpl-lib.cpp"
    break;

  case 333: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2227 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8276 "bison-chpl-lib.cpp"
    break;

  case 334: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2232 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8285 "bison-chpl-lib.cpp"
    break;

  case 335: /* $@6: %empty  */
#line 2237 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8294 "bison-chpl-lib.cpp"
    break;

  case 336: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2242 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8303 "bison-chpl-lib.cpp"
    break;

  case 337: /* enum_item: ident_def  */
#line 2252 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8314 "bison-chpl-lib.cpp"
    break;

  case 338: /* enum_item: ident_def TASSIGN expr  */
#line 2259 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8327 "bison-chpl-lib.cpp"
    break;

  case 339: /* lambda_decl_start: TLAMBDA  */
#line 2271 "chpl.ypp"
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
#line 8343 "bison-chpl-lib.cpp"
    break;

  case 340: /* $@7: %empty  */
#line 2288 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8353 "bison-chpl-lib.cpp"
    break;

  case 341: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2294 "chpl.ypp"
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
#line 8375 "bison-chpl-lib.cpp"
    break;

  case 343: /* linkage_spec: linkage_spec_empty  */
#line 2317 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8381 "bison-chpl-lib.cpp"
    break;

  case 344: /* linkage_spec: TINLINE  */
#line 2318 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8388 "bison-chpl-lib.cpp"
    break;

  case 345: /* linkage_spec: TOVERRIDE  */
#line 2320 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8395 "bison-chpl-lib.cpp"
    break;

  case 346: /* opt_fn_type_formal_ls: %empty  */
#line 2325 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8401 "bison-chpl-lib.cpp"
    break;

  case 347: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2326 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8407 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_type_formal_ls: fn_type_formal  */
#line 2330 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8413 "bison-chpl-lib.cpp"
    break;

  case 349: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2331 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8419 "bison-chpl-lib.cpp"
    break;

  case 350: /* fn_type_formal: named_formal  */
#line 2336 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8425 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2339 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8431 "bison-chpl-lib.cpp"
    break;

  case 352: /* fn_type_formal: formal_type  */
#line 2341 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8437 "bison-chpl-lib.cpp"
    break;

  case 353: /* opt_fn_type_ret_type: %empty  */
#line 2345 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8443 "bison-chpl-lib.cpp"
    break;

  case 354: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2346 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8449 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2359 "chpl.ypp"
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
#line 8468 "bison-chpl-lib.cpp"
    break;

  case 356: /* $@8: %empty  */
#line 2377 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8478 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2383 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8491 "bison-chpl-lib.cpp"
    break;

  case 358: /* $@9: %empty  */
#line 2392 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8501 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2398 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8514 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2410 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8522 "bison-chpl-lib.cpp"
    break;

  case 361: /* $@10: %empty  */
#line 2419 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8530 "bison-chpl-lib.cpp"
    break;

  case 362: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2423 "chpl.ypp"
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
#line 8552 "bison-chpl-lib.cpp"
    break;

  case 363: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2444 "chpl.ypp"
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
#line 2452 "chpl.ypp"
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
#line 2460 "chpl.ypp"
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
#line 2471 "chpl.ypp"
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
#line 2482 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8616 "bison-chpl-lib.cpp"
    break;

  case 368: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2491 "chpl.ypp"
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
#line 2506 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8638 "bison-chpl-lib.cpp"
    break;

  case 373: /* fn_ident: ident_def TBANG  */
#line 2513 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8648 "bison-chpl-lib.cpp"
    break;

  case 413: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2570 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8654 "bison-chpl-lib.cpp"
    break;

  case 414: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2571 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8660 "bison-chpl-lib.cpp"
    break;

  case 415: /* opt_formal_ls: %empty  */
#line 2575 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8666 "bison-chpl-lib.cpp"
    break;

  case 416: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2576 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8672 "bison-chpl-lib.cpp"
    break;

  case 417: /* req_formal_ls: TLP TRP  */
#line 2580 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8678 "bison-chpl-lib.cpp"
    break;

  case 418: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2581 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8684 "bison-chpl-lib.cpp"
    break;

  case 419: /* formal_ls_inner: formal  */
#line 2585 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8690 "bison-chpl-lib.cpp"
    break;

  case 420: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2586 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8696 "bison-chpl-lib.cpp"
    break;

  case 421: /* formal_ls: %empty  */
#line 2590 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8702 "bison-chpl-lib.cpp"
    break;

  case 422: /* formal_ls: formal_ls_inner  */
#line 2591 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8708 "bison-chpl-lib.cpp"
    break;

  case 426: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2605 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8716 "bison-chpl-lib.cpp"
    break;

  case 427: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2610 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8724 "bison-chpl-lib.cpp"
    break;

  case 428: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2615 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8732 "bison-chpl-lib.cpp"
    break;

  case 429: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2620 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8740 "bison-chpl-lib.cpp"
    break;

  case 430: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2625 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8748 "bison-chpl-lib.cpp"
    break;

  case 431: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2630 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8756 "bison-chpl-lib.cpp"
    break;

  case 432: /* opt_formal_intent_tag: %empty  */
#line 2636 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 8765 "bison-chpl-lib.cpp"
    break;

  case 433: /* opt_formal_intent_tag: required_intent_tag  */
#line 2641 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8774 "bison-chpl-lib.cpp"
    break;

  case 434: /* required_intent_tag: TIN  */
#line 2648 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 8780 "bison-chpl-lib.cpp"
    break;

  case 435: /* required_intent_tag: TINOUT  */
#line 2649 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 8786 "bison-chpl-lib.cpp"
    break;

  case 436: /* required_intent_tag: TOUT  */
#line 2650 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 8792 "bison-chpl-lib.cpp"
    break;

  case 437: /* required_intent_tag: TCONST TIN  */
#line 2651 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 8798 "bison-chpl-lib.cpp"
    break;

  case 438: /* required_intent_tag: TCONST TREF  */
#line 2652 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 8804 "bison-chpl-lib.cpp"
    break;

  case 439: /* required_intent_tag: TCONST  */
#line 2653 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 8810 "bison-chpl-lib.cpp"
    break;

  case 440: /* required_intent_tag: TPARAM  */
#line 2654 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 8816 "bison-chpl-lib.cpp"
    break;

  case 441: /* required_intent_tag: TREF  */
#line 2655 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 8822 "bison-chpl-lib.cpp"
    break;

  case 442: /* required_intent_tag: TTYPE  */
#line 2656 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 8828 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_this_intent_tag: %empty  */
#line 2660 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 8834 "bison-chpl-lib.cpp"
    break;

  case 444: /* opt_this_intent_tag: TPARAM  */
#line 2661 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 8840 "bison-chpl-lib.cpp"
    break;

  case 445: /* opt_this_intent_tag: TREF  */
#line 2662 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 8846 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_this_intent_tag: TCONST TREF  */
#line 2663 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 8852 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_this_intent_tag: TCONST  */
#line 2664 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 8858 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_this_intent_tag: TTYPE  */
#line 2665 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 8864 "bison-chpl-lib.cpp"
    break;

  case 449: /* proc_iter_or_op: TPROC  */
#line 2669 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8870 "bison-chpl-lib.cpp"
    break;

  case 450: /* proc_iter_or_op: TITER  */
#line 2670 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8876 "bison-chpl-lib.cpp"
    break;

  case 451: /* proc_iter_or_op: TOPERATOR  */
#line 2671 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8882 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: %empty  */
#line 2675 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 8888 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_ret_tag: TOUT  */
#line 2676 "chpl.ypp"
                    { (yyval.returnTag) = Function::OUT; }
#line 8894 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_ret_tag: TCONST  */
#line 2677 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 8900 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_ret_tag: TCONST TREF  */
#line 2678 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 8906 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_ret_tag: TREF  */
#line 2679 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 8912 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_ret_tag: TPARAM  */
#line 2680 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 8918 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_ret_tag: TTYPE  */
#line 2681 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 8924 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_throws_error: %empty  */
#line 2685 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8930 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_throws_error: TTHROWS  */
#line 2686 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8936 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_function_body_stmt: TSEMI  */
#line 2689 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8942 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_function_body_stmt: function_body_stmt  */
#line 2690 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8948 "bison-chpl-lib.cpp"
    break;

  case 463: /* function_body_stmt: block_stmt_body  */
#line 2694 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8954 "bison-chpl-lib.cpp"
    break;

  case 464: /* function_body_stmt: TDO toplevel_stmt  */
#line 2695 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 8960 "bison-chpl-lib.cpp"
    break;

  case 465: /* query_expr: TQUERIEDIDENT  */
#line 2699 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 8966 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_lifetime_where: %empty  */
#line 2704 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 8972 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_lifetime_where: TWHERE expr  */
#line 2706 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 8978 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2708 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 8984 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2710 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 8990 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2712 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 8996 "bison-chpl-lib.cpp"
    break;

  case 471: /* lifetime_components_expr: lifetime_expr  */
#line 2717 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9002 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2719 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9008 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2724 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9014 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2726 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9020 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2728 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9026 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2730 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9032 "bison-chpl-lib.cpp"
    break;

  case 477: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2732 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9038 "bison-chpl-lib.cpp"
    break;

  case 478: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2734 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9044 "bison-chpl-lib.cpp"
    break;

  case 479: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2736 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9050 "bison-chpl-lib.cpp"
    break;

  case 480: /* lifetime_ident: TIDENT  */
#line 2740 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9056 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_ident: TINIT  */
#line 2741 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9062 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_ident: TTHIS  */
#line 2742 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9068 "bison-chpl-lib.cpp"
    break;

  case 483: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2747 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9077 "bison-chpl-lib.cpp"
    break;

  case 484: /* type_alias_decl_stmt_start: TTYPE  */
#line 2756 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9085 "bison-chpl-lib.cpp"
    break;

  case 485: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2760 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9094 "bison-chpl-lib.cpp"
    break;

  case 486: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2768 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9102 "bison-chpl-lib.cpp"
    break;

  case 487: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2772 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9110 "bison-chpl-lib.cpp"
    break;

  case 488: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2779 "chpl.ypp"
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
#line 2799 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9138 "bison-chpl-lib.cpp"
    break;

  case 490: /* opt_init_type: TASSIGN expr  */
#line 2801 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9144 "bison-chpl-lib.cpp"
    break;

  case 491: /* var_decl_type: TPARAM  */
#line 2805 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9150 "bison-chpl-lib.cpp"
    break;

  case 492: /* var_decl_type: TCONST TREF  */
#line 2806 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9156 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_type: TREF  */
#line 2807 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9162 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_type: TCONST  */
#line 2808 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9168 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_type: TVAR  */
#line 2809 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9174 "bison-chpl-lib.cpp"
    break;

  case 496: /* $@11: %empty  */
#line 2814 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9183 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2818 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9192 "bison-chpl-lib.cpp"
    break;

  case 498: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2823 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9201 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_stmt: var_decl_type error TSEMI  */
#line 2828 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = { .comments=nullptr, .stmt=ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release() };
    context->resetDeclStateOnError();
  }
#line 9210 "bison-chpl-lib.cpp"
    break;

  case 500: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2836 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9218 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2840 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9226 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
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
      BUILDER->noteDeclNameLocation(varDecl.get(), LOC((yylsp[-2])));
      // gather the comments like a statement does
      (yyval.commentsAndStmt) = STMT((yylsp[-2]), varDecl.release());
      context->clearComments();
    }
#line 9249 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2866 "chpl.ypp"
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
#line 2885 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9277 "bison-chpl-lib.cpp"
    break;

  case 505: /* tuple_var_decl_component: ident_def  */
#line 2889 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9285 "bison-chpl-lib.cpp"
    break;

  case 506: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2893 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9293 "bison-chpl-lib.cpp"
    break;

  case 507: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2900 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9299 "bison-chpl-lib.cpp"
    break;

  case 508: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2902 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9305 "bison-chpl-lib.cpp"
    break;

  case 509: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2904 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9311 "bison-chpl-lib.cpp"
    break;

  case 510: /* opt_init_expr: %empty  */
#line 2910 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9317 "bison-chpl-lib.cpp"
    break;

  case 511: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2911 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9323 "bison-chpl-lib.cpp"
    break;

  case 512: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2912 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9329 "bison-chpl-lib.cpp"
    break;

  case 513: /* formal_or_ret_type_expr: expr  */
#line 2916 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9335 "bison-chpl-lib.cpp"
    break;

  case 514: /* ret_type: formal_or_ret_type_expr  */
#line 2920 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9341 "bison-chpl-lib.cpp"
    break;

  case 515: /* ret_type: reserved_type_ident_use  */
#line 2921 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9347 "bison-chpl-lib.cpp"
    break;

  case 516: /* ret_type: error  */
#line 2922 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9353 "bison-chpl-lib.cpp"
    break;

  case 517: /* colon_ret_type: TCOLON ret_type  */
#line 2926 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9359 "bison-chpl-lib.cpp"
    break;

  case 518: /* colon_ret_type: error  */
#line 2927 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9365 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_ret_type: %empty  */
#line 2931 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9371 "bison-chpl-lib.cpp"
    break;

  case 521: /* opt_type: %empty  */
#line 2936 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9377 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_type: TCOLON expr  */
#line 2937 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9383 "bison-chpl-lib.cpp"
    break;

  case 523: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2938 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9389 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_type: error  */
#line 2939 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9395 "bison-chpl-lib.cpp"
    break;

  case 525: /* formal_type: formal_or_ret_type_expr  */
#line 2943 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9401 "bison-chpl-lib.cpp"
    break;

  case 526: /* formal_type: reserved_type_ident_use  */
#line 2944 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9407 "bison-chpl-lib.cpp"
    break;

  case 527: /* colon_formal_type: TCOLON formal_type  */
#line 2948 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9413 "bison-chpl-lib.cpp"
    break;

  case 528: /* opt_colon_formal_type: %empty  */
#line 2952 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9419 "bison-chpl-lib.cpp"
    break;

  case 529: /* opt_colon_formal_type: colon_formal_type  */
#line 2953 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9425 "bison-chpl-lib.cpp"
    break;

  case 530: /* expr_ls: expr  */
#line 2959 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9431 "bison-chpl-lib.cpp"
    break;

  case 531: /* expr_ls: expr_ls TCOMMA expr  */
#line 2960 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9437 "bison-chpl-lib.cpp"
    break;

  case 532: /* tuple_component: TUNDERSCORE  */
#line 2964 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9443 "bison-chpl-lib.cpp"
    break;

  case 533: /* tuple_component: opt_try_expr  */
#line 2965 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9449 "bison-chpl-lib.cpp"
    break;

  case 534: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2970 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9457 "bison-chpl-lib.cpp"
    break;

  case 535: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2974 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9465 "bison-chpl-lib.cpp"
    break;

  case 536: /* opt_actual_ls: %empty  */
#line 2980 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9471 "bison-chpl-lib.cpp"
    break;

  case 537: /* opt_actual_ls: actual_ls  */
#line 2981 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9477 "bison-chpl-lib.cpp"
    break;

  case 538: /* actual_ls: actual_expr  */
#line 2986 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9486 "bison-chpl-lib.cpp"
    break;

  case 539: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2991 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9496 "bison-chpl-lib.cpp"
    break;

  case 540: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2999 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9502 "bison-chpl-lib.cpp"
    break;

  case 541: /* actual_expr: opt_try_expr  */
#line 3000 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9508 "bison-chpl-lib.cpp"
    break;

  case 542: /* ident_expr: ident_use  */
#line 3004 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9514 "bison-chpl-lib.cpp"
    break;

  case 543: /* ident_expr: scalar_type  */
#line 3005 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9520 "bison-chpl-lib.cpp"
    break;

  case 549: /* sub_type_level_expr: TSINGLE expr  */
#line 3018 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9526 "bison-chpl-lib.cpp"
    break;

  case 550: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3020 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9532 "bison-chpl-lib.cpp"
    break;

  case 551: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3022 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9538 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3024 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9544 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 3026 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9554 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TATOMIC expr  */
#line 3032 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9560 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TSYNC expr  */
#line 3034 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9566 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TOWNED  */
#line 3037 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9572 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TOWNED expr  */
#line 3039 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9578 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TUNMANAGED  */
#line 3041 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9584 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3043 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9590 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TSHARED  */
#line 3045 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9596 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TSHARED expr  */
#line 3047 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9602 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TBORROWED  */
#line 3049 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9608 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TBORROWED expr  */
#line 3051 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9614 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TCLASS  */
#line 3053 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9620 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TRECORD  */
#line 3055 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9626 "bison-chpl-lib.cpp"
    break;

  case 566: /* for_expr: no_loop_attributes for_expr_base  */
#line 3059 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9632 "bison-chpl-lib.cpp"
    break;

  case 567: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3064 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9640 "bison-chpl-lib.cpp"
    break;

  case 568: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3068 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9648 "bison-chpl-lib.cpp"
    break;

  case 569: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3072 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-2]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9656 "bison-chpl-lib.cpp"
    break;

  case 570: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3076 "chpl.ypp"
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
#line 3085 "chpl.ypp"
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
#line 3094 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-5]), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9695 "bison-chpl-lib.cpp"
    break;

  case 573: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3105 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9701 "bison-chpl-lib.cpp"
    break;

  case 574: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3110 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9709 "bison-chpl-lib.cpp"
    break;

  case 575: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3114 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9717 "bison-chpl-lib.cpp"
    break;

  case 576: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3118 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9725 "bison-chpl-lib.cpp"
    break;

  case 577: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3122 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9733 "bison-chpl-lib.cpp"
    break;

  case 578: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3126 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9741 "bison-chpl-lib.cpp"
    break;

  case 579: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3130 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9749 "bison-chpl-lib.cpp"
    break;

  case 580: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3134 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9757 "bison-chpl-lib.cpp"
    break;

  case 581: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3141 "chpl.ypp"
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
#line 3158 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9777 "bison-chpl-lib.cpp"
    break;

  case 590: /* opt_task_intent_ls: %empty  */
#line 3176 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9783 "bison-chpl-lib.cpp"
    break;

  case 591: /* opt_task_intent_ls: task_intent_clause  */
#line 3177 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9789 "bison-chpl-lib.cpp"
    break;

  case 592: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3182 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9797 "bison-chpl-lib.cpp"
    break;

  case 593: /* task_intent_ls: intent_expr  */
#line 3188 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9803 "bison-chpl-lib.cpp"
    break;

  case 594: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3189 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9809 "bison-chpl-lib.cpp"
    break;

  case 595: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3194 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 9819 "bison-chpl-lib.cpp"
    break;

  case 596: /* forall_intent_ls: intent_expr  */
#line 3202 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9825 "bison-chpl-lib.cpp"
    break;

  case 597: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3203 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9831 "bison-chpl-lib.cpp"
    break;

  case 598: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3208 "chpl.ypp"
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
#line 9850 "bison-chpl-lib.cpp"
    break;

  case 599: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3223 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9858 "bison-chpl-lib.cpp"
    break;

  case 600: /* intent_expr: expr TREDUCE ident_expr  */
#line 3227 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9866 "bison-chpl-lib.cpp"
    break;

  case 601: /* task_var_prefix: TCONST  */
#line 3233 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 9872 "bison-chpl-lib.cpp"
    break;

  case 602: /* task_var_prefix: TIN  */
#line 3234 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 9878 "bison-chpl-lib.cpp"
    break;

  case 603: /* task_var_prefix: TCONST TIN  */
#line 3235 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 9884 "bison-chpl-lib.cpp"
    break;

  case 604: /* task_var_prefix: TREF  */
#line 3236 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 9890 "bison-chpl-lib.cpp"
    break;

  case 605: /* task_var_prefix: TCONST TREF  */
#line 3237 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 9896 "bison-chpl-lib.cpp"
    break;

  case 606: /* task_var_prefix: TVAR  */
#line 3238 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 9902 "bison-chpl-lib.cpp"
    break;

  case 607: /* new_maybe_decorated: TNEW  */
#line 3243 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 9908 "bison-chpl-lib.cpp"
    break;

  case 608: /* new_maybe_decorated: TNEW TOWNED  */
#line 3245 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 9914 "bison-chpl-lib.cpp"
    break;

  case 609: /* new_maybe_decorated: TNEW TSHARED  */
#line 3247 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 9920 "bison-chpl-lib.cpp"
    break;

  case 610: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3249 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 9926 "bison-chpl-lib.cpp"
    break;

  case 611: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3251 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 9932 "bison-chpl-lib.cpp"
    break;

  case 612: /* new_expr: new_maybe_decorated expr  */
#line 3257 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 9940 "bison-chpl-lib.cpp"
    break;

  case 613: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3264 "chpl.ypp"
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
#line 9956 "bison-chpl-lib.cpp"
    break;

  case 614: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3276 "chpl.ypp"
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
#line 9972 "bison-chpl-lib.cpp"
    break;

  case 615: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3288 "chpl.ypp"
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
#line 9989 "bison-chpl-lib.cpp"
    break;

  case 616: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3301 "chpl.ypp"
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
#line 10007 "bison-chpl-lib.cpp"
    break;

  case 617: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3318 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10015 "bison-chpl-lib.cpp"
    break;

  case 618: /* range_literal_expr: expr TDOTDOT expr  */
#line 3325 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10024 "bison-chpl-lib.cpp"
    break;

  case 619: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3330 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10033 "bison-chpl-lib.cpp"
    break;

  case 620: /* range_literal_expr: expr TDOTDOT  */
#line 3335 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10042 "bison-chpl-lib.cpp"
    break;

  case 621: /* range_literal_expr: TDOTDOT expr  */
#line 3340 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10052 "bison-chpl-lib.cpp"
    break;

  case 622: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3346 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10062 "bison-chpl-lib.cpp"
    break;

  case 623: /* range_literal_expr: TDOTDOT  */
#line 3352 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10072 "bison-chpl-lib.cpp"
    break;

  case 624: /* cast_expr: expr TCOLON expr  */
#line 3382 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10080 "bison-chpl-lib.cpp"
    break;

  case 625: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3389 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10088 "bison-chpl-lib.cpp"
    break;

  case 626: /* super_expr: fn_expr  */
#line 3395 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10094 "bison-chpl-lib.cpp"
    break;

  case 630: /* expr: sub_type_level_expr TQUESTION  */
#line 3404 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10100 "bison-chpl-lib.cpp"
    break;

  case 631: /* expr: TQUESTION  */
#line 3406 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10106 "bison-chpl-lib.cpp"
    break;

  case 635: /* expr: fn_type  */
#line 3411 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10112 "bison-chpl-lib.cpp"
    break;

  case 645: /* opt_expr: %empty  */
#line 3425 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10118 "bison-chpl-lib.cpp"
    break;

  case 646: /* opt_expr: expr  */
#line 3426 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10124 "bison-chpl-lib.cpp"
    break;

  case 647: /* opt_try_expr: TTRY expr  */
#line 3430 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10130 "bison-chpl-lib.cpp"
    break;

  case 648: /* opt_try_expr: TTRYBANG expr  */
#line 3431 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10136 "bison-chpl-lib.cpp"
    break;

  case 649: /* opt_try_expr: super_expr  */
#line 3432 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10142 "bison-chpl-lib.cpp"
    break;

  case 655: /* call_base_expr: expr TBANG  */
#line 3449 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10150 "bison-chpl-lib.cpp"
    break;

  case 656: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3452 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10156 "bison-chpl-lib.cpp"
    break;

  case 659: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3459 "chpl.ypp"
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
#line 10172 "bison-chpl-lib.cpp"
    break;

  case 660: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3471 "chpl.ypp"
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
#line 10188 "bison-chpl-lib.cpp"
    break;

  case 661: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3483 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10196 "bison-chpl-lib.cpp"
    break;

  case 662: /* dot_expr: expr TDOT ident_use  */
#line 3490 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10202 "bison-chpl-lib.cpp"
    break;

  case 663: /* dot_expr: expr TDOT TTYPE  */
#line 3492 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10208 "bison-chpl-lib.cpp"
    break;

  case 664: /* dot_expr: expr TDOT TDOMAIN  */
#line 3494 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10214 "bison-chpl-lib.cpp"
    break;

  case 665: /* dot_expr: expr TDOT TLOCALE  */
#line 3496 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10220 "bison-chpl-lib.cpp"
    break;

  case 666: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3498 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10226 "bison-chpl-lib.cpp"
    break;

  case 667: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3500 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10232 "bison-chpl-lib.cpp"
    break;

  case 668: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3509 "chpl.ypp"
  {
    // Use 'tryNote' here in case an expression like ((x)) comes along.
    BUILDER->tryNoteExprParenthLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 10242 "bison-chpl-lib.cpp"
    break;

  case 669: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3515 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10250 "bison-chpl-lib.cpp"
    break;

  case 670: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3519 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10258 "bison-chpl-lib.cpp"
    break;

  case 671: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3523 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10266 "bison-chpl-lib.cpp"
    break;

  case 672: /* bool_literal: TFALSE  */
#line 3529 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10272 "bison-chpl-lib.cpp"
    break;

  case 673: /* bool_literal: TTRUE  */
#line 3530 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10278 "bison-chpl-lib.cpp"
    break;

  case 674: /* str_bytes_literal: STRINGLITERAL  */
#line 3534 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10284 "bison-chpl-lib.cpp"
    break;

  case 675: /* str_bytes_literal: BYTESLITERAL  */
#line 3535 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10290 "bison-chpl-lib.cpp"
    break;

  case 678: /* literal_expr: INTLITERAL  */
#line 3541 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10296 "bison-chpl-lib.cpp"
    break;

  case 679: /* literal_expr: REALLITERAL  */
#line 3542 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10302 "bison-chpl-lib.cpp"
    break;

  case 680: /* literal_expr: IMAGLITERAL  */
#line 3543 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10308 "bison-chpl-lib.cpp"
    break;

  case 681: /* literal_expr: CSTRINGLITERAL  */
#line 3544 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10314 "bison-chpl-lib.cpp"
    break;

  case 682: /* literal_expr: TNONE  */
#line 3545 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10320 "bison-chpl-lib.cpp"
    break;

  case 683: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3547 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10329 "bison-chpl-lib.cpp"
    break;

  case 684: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3552 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10338 "bison-chpl-lib.cpp"
    break;

  case 685: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3557 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10347 "bison-chpl-lib.cpp"
    break;

  case 686: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3562 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10356 "bison-chpl-lib.cpp"
    break;

  case 687: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3567 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10365 "bison-chpl-lib.cpp"
    break;

  case 688: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3572 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10374 "bison-chpl-lib.cpp"
    break;

  case 689: /* assoc_expr_ls: expr TALIAS expr  */
#line 3581 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10383 "bison-chpl-lib.cpp"
    break;

  case 690: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3586 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10393 "bison-chpl-lib.cpp"
    break;

  case 691: /* binary_op_expr: expr TPLUS expr  */
#line 3594 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10399 "bison-chpl-lib.cpp"
    break;

  case 692: /* binary_op_expr: expr TMINUS expr  */
#line 3595 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10405 "bison-chpl-lib.cpp"
    break;

  case 693: /* binary_op_expr: expr TSTAR expr  */
#line 3596 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10411 "bison-chpl-lib.cpp"
    break;

  case 694: /* binary_op_expr: expr TDIVIDE expr  */
#line 3597 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10417 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3598 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10423 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3599 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10429 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TMOD expr  */
#line 3600 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10435 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TEQUAL expr  */
#line 3601 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10441 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3602 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10447 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3603 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10453 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3604 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10459 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TLESS expr  */
#line 3605 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10465 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TGREATER expr  */
#line 3606 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10471 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TBAND expr  */
#line 3607 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10477 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TBOR expr  */
#line 3608 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10483 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TBXOR expr  */
#line 3609 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10489 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TAND expr  */
#line 3610 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10495 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TOR expr  */
#line 3611 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10501 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TEXP expr  */
#line 3612 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10507 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TBY expr  */
#line 3613 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10513 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TALIGN expr  */
#line 3614 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10519 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr THASH expr  */
#line 3615 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10525 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TDMAPPED expr  */
#line 3616 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10531 "bison-chpl-lib.cpp"
    break;

  case 714: /* unary_op_expr: TPLUS expr  */
#line 3620 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10537 "bison-chpl-lib.cpp"
    break;

  case 715: /* unary_op_expr: TMINUS expr  */
#line 3621 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10543 "bison-chpl-lib.cpp"
    break;

  case 716: /* unary_op_expr: TMINUSMINUS expr  */
#line 3622 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10549 "bison-chpl-lib.cpp"
    break;

  case 717: /* unary_op_expr: TPLUSPLUS expr  */
#line 3623 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10555 "bison-chpl-lib.cpp"
    break;

  case 718: /* unary_op_expr: TBANG expr  */
#line 3624 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10561 "bison-chpl-lib.cpp"
    break;

  case 719: /* unary_op_expr: expr TBANG  */
#line 3625 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10569 "bison-chpl-lib.cpp"
    break;

  case 720: /* unary_op_expr: TBNOT expr  */
#line 3628 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10575 "bison-chpl-lib.cpp"
    break;

  case 721: /* reduce_expr: expr TREDUCE expr  */
#line 3633 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10583 "bison-chpl-lib.cpp"
    break;

  case 722: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3637 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10591 "bison-chpl-lib.cpp"
    break;

  case 723: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3641 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10599 "bison-chpl-lib.cpp"
    break;

  case 724: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3645 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10607 "bison-chpl-lib.cpp"
    break;

  case 725: /* scan_expr: expr TSCAN expr  */
#line 3652 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10615 "bison-chpl-lib.cpp"
    break;

  case 726: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3656 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10623 "bison-chpl-lib.cpp"
    break;

  case 727: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3660 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10631 "bison-chpl-lib.cpp"
    break;

  case 728: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3664 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10639 "bison-chpl-lib.cpp"
    break;


#line 10643 "bison-chpl-lib.cpp"

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
