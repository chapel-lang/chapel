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
  YYSYMBOL_implements_stmt = 250,          /* implements_stmt  */
  YYSYMBOL_ifc_constraint = 251,           /* ifc_constraint  */
  YYSYMBOL_try_stmt = 252,                 /* try_stmt  */
  YYSYMBOL_catch_expr_ls = 253,            /* catch_expr_ls  */
  YYSYMBOL_catch_expr = 254,               /* catch_expr  */
  YYSYMBOL_catch_expr_inner = 255,         /* catch_expr_inner  */
  YYSYMBOL_throw_stmt = 256,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 257,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 258,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 259,                /* when_stmt  */
  YYSYMBOL_manager_expr = 260,             /* manager_expr  */
  YYSYMBOL_manager_expr_ls = 261,          /* manager_expr_ls  */
  YYSYMBOL_manage_stmt = 262,              /* manage_stmt  */
  YYSYMBOL_class_decl_stmt = 263,          /* class_decl_stmt  */
  YYSYMBOL_class_start = 264,              /* class_start  */
  YYSYMBOL_class_tag = 265,                /* class_tag  */
  YYSYMBOL_opt_inherit = 266,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 267,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 268,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header_lcbr = 269,         /* enum_header_lcbr  */
  YYSYMBOL_enum_ls = 270,                  /* enum_ls  */
  YYSYMBOL_271_5 = 271,                    /* $@5  */
  YYSYMBOL_272_6 = 272,                    /* $@6  */
  YYSYMBOL_enum_item = 273,                /* enum_item  */
  YYSYMBOL_lambda_decl_start = 274,        /* lambda_decl_start  */
  YYSYMBOL_lambda_decl_expr = 275,         /* lambda_decl_expr  */
  YYSYMBOL_276_7 = 276,                    /* $@7  */
  YYSYMBOL_linkage_spec_empty = 277,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 278,             /* linkage_spec  */
  YYSYMBOL_opt_fn_type_formal_ls = 279,    /* opt_fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal_ls = 280,        /* fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal = 281,           /* fn_type_formal  */
  YYSYMBOL_opt_fn_type_ret_type = 282,     /* opt_fn_type_ret_type  */
  YYSYMBOL_fn_type = 283,                  /* fn_type  */
  YYSYMBOL_fn_expr = 284,                  /* fn_expr  */
  YYSYMBOL_285_8 = 285,                    /* $@8  */
  YYSYMBOL_286_9 = 286,                    /* $@9  */
  YYSYMBOL_fn_decl_stmt_complete = 287,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 288,             /* fn_decl_stmt  */
  YYSYMBOL_289_10 = 289,                   /* $@10  */
  YYSYMBOL_fn_decl_stmt_inner = 290,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 291,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 292,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 293,                 /* fn_ident  */
  YYSYMBOL_op_ident = 294,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 295,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 296,              /* all_op_name  */
  YYSYMBOL_formal_var_arg_expr = 297,      /* formal_var_arg_expr  */
  YYSYMBOL_opt_formal_ls = 298,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 299,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 300,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 301,                /* formal_ls  */
  YYSYMBOL_formal = 302,                   /* formal  */
  YYSYMBOL_formal_ident_def = 303,         /* formal_ident_def  */
  YYSYMBOL_named_formal = 304,             /* named_formal  */
  YYSYMBOL_opt_formal_intent_tag = 305,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 306,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 307,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 308,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 309,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 310,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 311,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 312,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 313,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 314,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 315, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 316,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 317,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 318,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 319, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 320, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 321, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 322,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 323,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 324,            /* var_decl_stmt  */
  YYSYMBOL_325_11 = 325,                   /* $@11  */
  YYSYMBOL_var_decl_stmt_inner_ls = 326,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 327,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 328, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 329, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 330,            /* opt_init_expr  */
  YYSYMBOL_formal_or_ret_type_expr = 331,  /* formal_or_ret_type_expr  */
  YYSYMBOL_ret_type = 332,                 /* ret_type  */
  YYSYMBOL_colon_ret_type = 333,           /* colon_ret_type  */
  YYSYMBOL_opt_ret_type = 334,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 335,                 /* opt_type  */
  YYSYMBOL_formal_type = 336,              /* formal_type  */
  YYSYMBOL_colon_formal_type = 337,        /* colon_formal_type  */
  YYSYMBOL_opt_colon_formal_type = 338,    /* opt_colon_formal_type  */
  YYSYMBOL_expr_ls = 339,                  /* expr_ls  */
  YYSYMBOL_tuple_component = 340,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 341,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 342,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 343,                /* actual_ls  */
  YYSYMBOL_actual_expr = 344,              /* actual_expr  */
  YYSYMBOL_ident_expr = 345,               /* ident_expr  */
  YYSYMBOL_sub_type_level_expr = 346,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 347,                 /* for_expr  */
  YYSYMBOL_for_expr_base = 348,            /* for_expr_base  */
  YYSYMBOL_bracket_loop_expr = 349,        /* bracket_loop_expr  */
  YYSYMBOL_bracket_loop_expr_base = 350,   /* bracket_loop_expr_base  */
  YYSYMBOL_cond_expr = 351,                /* cond_expr  */
  YYSYMBOL_nil_expr = 352,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 353,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 354,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 355,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 356,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 357,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 358,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 359,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 360,          /* task_var_prefix  */
  YYSYMBOL_new_maybe_decorated = 361,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 362,                 /* new_expr  */
  YYSYMBOL_let_expr = 363,                 /* let_expr  */
  YYSYMBOL_range_literal_expr = 364,       /* range_literal_expr  */
  YYSYMBOL_cast_expr = 365,                /* cast_expr  */
  YYSYMBOL_tuple_expand_expr = 366,        /* tuple_expand_expr  */
  YYSYMBOL_super_expr = 367,               /* super_expr  */
  YYSYMBOL_expr = 368,                     /* expr  */
  YYSYMBOL_opt_expr = 369,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 370,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 371,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 372,           /* call_base_expr  */
  YYSYMBOL_call_expr = 373,                /* call_expr  */
  YYSYMBOL_dot_expr = 374,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 375,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 376,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 377,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 378,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 379,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 380,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 381,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 382,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 383,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 384       /* reduce_scan_op_expr  */
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

#line 508 "bison-chpl-lib.cpp"

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
#define YYLAST   20647

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  204
/* YYNRULES -- Number of rules.  */
#define YYNRULES  746
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1320

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
     880,   884,   885,   889,   893,   900,   912,   920,   923,   933,
     935,   942,   947,   956,   960,   980,   995,  1010,  1026,  1027,
    1029,  1034,  1035,  1040,  1058,  1062,  1066,  1077,  1096,  1097,
    1101,  1105,  1109,  1114,  1118,  1122,  1132,  1137,  1143,  1150,
    1155,  1162,  1173,  1174,  1178,  1183,  1191,  1200,  1210,  1218,
    1227,  1240,  1248,  1249,  1250,  1251,  1252,  1253,  1254,  1255,
    1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,  1264,  1276,
    1280,  1285,  1290,  1296,  1305,  1306,  1310,  1319,  1323,  1327,
    1331,  1335,  1339,  1348,  1349,  1353,  1354,  1355,  1356,  1357,
    1361,  1362,  1374,  1375,  1380,  1381,  1382,  1383,  1384,  1385,
    1386,  1387,  1388,  1389,  1390,  1391,  1392,  1393,  1394,  1395,
    1396,  1397,  1398,  1402,  1403,  1404,  1405,  1406,  1407,  1408,
    1409,  1410,  1411,  1412,  1413,  1420,  1421,  1422,  1423,  1427,
    1428,  1432,  1439,  1449,  1453,  1459,  1459,  1465,  1465,  1474,
    1475,  1476,  1477,  1478,  1479,  1480,  1484,  1489,  1494,  1499,
    1506,  1514,  1515,  1519,  1528,  1534,  1542,  1561,  1560,  1570,
    1569,  1582,  1588,  1591,  1592,  1596,  1615,  1629,  1643,  1647,
    1651,  1655,  1659,  1663,  1667,  1671,  1691,  1695,  1699,  1703,
    1707,  1711,  1715,  1719,  1723,  1727,  1731,  1735,  1739,  1743,
    1747,  1751,  1755,  1759,  1763,  1767,  1771,  1775,  1779,  1784,
    1792,  1801,  1805,  1809,  1813,  1817,  1821,  1825,  1829,  1833,
    1838,  1843,  1848,  1856,  1871,  1889,  1893,  1900,  1901,  1906,
    1911,  1912,  1913,  1914,  1915,  1916,  1917,  1918,  1919,  1920,
    1921,  1922,  1923,  1932,  1933,  1934,  1935,  1944,  1945,  1949,
    1953,  1957,  1964,  1968,  1972,  1979,  1983,  1987,  1991,  1998,
    1999,  2003,  2007,  2011,  2018,  2031,  2047,  2055,  2059,  2068,
    2069,  2073,  2077,  2082,  2091,  2096,  2100,  2107,  2108,  2112,
    2121,  2126,  2137,  2144,  2145,  2146,  2150,  2151,  2155,  2159,
    2163,  2167,  2171,  2178,  2195,  2208,  2215,  2220,  2227,  2226,
    2236,  2242,  2241,  2256,  2263,  2275,  2293,  2290,  2318,  2322,
    2323,  2325,  2330,  2331,  2335,  2336,  2340,  2343,  2345,  2350,
    2351,  2362,  2382,  2381,  2397,  2396,  2414,  2424,  2421,  2455,
    2463,  2471,  2482,  2493,  2502,  2517,  2518,  2522,  2523,  2524,
    2533,  2534,  2535,  2536,  2537,  2538,  2539,  2540,  2541,  2542,
    2543,  2544,  2545,  2546,  2547,  2548,  2549,  2550,  2551,  2552,
    2553,  2554,  2555,  2556,  2557,  2561,  2562,  2563,  2564,  2565,
    2566,  2567,  2568,  2569,  2570,  2571,  2572,  2577,  2578,  2582,
    2583,  2587,  2588,  2592,  2593,  2597,  2598,  2602,  2603,  2607,
    2611,  2612,  2616,  2620,  2625,  2630,  2635,  2640,  2648,  2652,
    2660,  2661,  2662,  2663,  2664,  2665,  2666,  2667,  2668,  2672,
    2673,  2674,  2675,  2676,  2677,  2681,  2682,  2683,  2687,  2688,
    2689,  2690,  2691,  2692,  2693,  2697,  2698,  2701,  2702,  2706,
    2707,  2708,  2712,  2717,  2718,  2720,  2722,  2724,  2729,  2731,
    2736,  2738,  2740,  2742,  2744,  2746,  2748,  2753,  2754,  2758,
    2767,  2771,  2779,  2783,  2790,  2811,  2812,  2817,  2818,  2819,
    2820,  2821,  2826,  2825,  2834,  2842,  2846,  2853,  2871,  2890,
    2894,  2898,  2905,  2907,  2909,  2916,  2917,  2918,  2922,  2926,
    2927,  2928,  2932,  2933,  2937,  2938,  2942,  2943,  2944,  2945,
    2949,  2950,  2954,  2958,  2959,  2965,  2966,  2970,  2971,  2975,
    2979,  2986,  2987,  2991,  2996,  3005,  3006,  3010,  3011,  3018,
    3019,  3020,  3021,  3022,  3023,  3025,  3027,  3029,  3031,  3037,
    3039,  3042,  3044,  3046,  3048,  3050,  3052,  3054,  3056,  3058,
    3060,  3065,  3069,  3079,  3089,  3098,  3113,  3128,  3143,  3153,
    3163,  3172,  3187,  3202,  3220,  3224,  3228,  3232,  3236,  3240,
    3244,  3248,  3255,  3273,  3281,  3282,  3283,  3284,  3285,  3286,
    3287,  3291,  3292,  3296,  3305,  3306,  3310,  3319,  3320,  3324,
    3338,  3342,  3349,  3350,  3351,  3352,  3353,  3354,  3358,  3360,
    3362,  3364,  3366,  3372,  3379,  3391,  3403,  3416,  3433,  3440,
    3445,  3450,  3455,  3461,  3467,  3497,  3504,  3511,  3512,  3516,
    3518,  3519,  3521,  3523,  3524,  3525,  3526,  3529,  3530,  3531,
    3532,  3533,  3534,  3535,  3536,  3537,  3541,  3542,  3546,  3547,
    3548,  3552,  3553,  3554,  3555,  3564,  3565,  3568,  3569,  3570,
    3574,  3586,  3598,  3605,  3607,  3609,  3611,  3613,  3619,  3632,
    3633,  3637,  3641,  3648,  3649,  3653,  3654,  3658,  3659,  3660,
    3661,  3662,  3663,  3664,  3665,  3670,  3675,  3680,  3685,  3690,
    3699,  3704,  3713,  3714,  3715,  3716,  3717,  3718,  3719,  3720,
    3721,  3722,  3723,  3724,  3725,  3726,  3727,  3728,  3729,  3730,
    3731,  3732,  3733,  3734,  3735,  3739,  3740,  3741,  3742,  3743,
    3744,  3747,  3751,  3755,  3759,  3763,  3770,  3774,  3778,  3782,
    3790,  3791,  3792,  3793,  3794,  3795,  3796
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

#define YYPACT_NINF (-1081)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-747)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1081,    71,  4769, -1081,   -61,    92, -1081, -1081, -1081, -1081,
   -1081, -1081, -1081, 13694,   147,   172,   207,  4365,   227, 20401,
     147,   246,   208,   228,   172,  6169, 13694,   166, 20456, -1081,
     369,   330, -1081, 11244,   361,   121,   258, -1081,   397, 20456,
   20456, 20456, -1081,  2910, 12294,   402, 13694, 13694,   351, -1081,
     422,   438, 13694, -1081,  4365, -1081, 13694,   430,   293,   206,
     235, 15416,   463, 20531, -1081, 13694,  9669, 13694, 12294,  4365,
   13694,   432,   466,   363,  6169,   494, 13694,   504,  8269,  8269,
   -1081,   510, -1081,  4365, -1081,   515, 11419, -1081,   588, 13694,
   -1081, 13694, -1081, -1081, 15896, 13694, -1081, 11594, -1081, -1081,
   -1081,  5119,  8794, 13694, -1081,  5819, -1081, -1081, -1081, -1081,
    3613,   452, -1081,    31,   436, -1081,    55, -1081, -1081, -1081,
   -1081, -1081, -1081, -1081, -1081, -1081, -1081, -1081, -1081, -1081,
    8444, -1081,  8969, -1081, -1081,   109, -1081, -1081, -1081,   566,
   -1081, -1081, -1081, -1081, -1081, -1081, -1081, -1081,   480, 20456,
   -1081, 20141,   455,   336, -1081,   310, -1081, -1081, -1081,   364,
     346, -1081, -1081, 20456,  2910, -1081,   473,   471, -1081, -1081,
   -1081,   474,   477, 13694,   483,   485, -1081, -1081, -1081, 19928,
    2552,   183,   486,   492, -1081, -1081,   337, -1081, -1081, -1081,
   -1081, -1081,   164, -1081, -1081, -1081, 13694, 20456, -1081, 13694,
   13694,    53,   603,   399, -1081, -1081, -1081, -1081, 19928,   406,
   -1081, -1081,   488,  6169, -1081, -1081,   496,   -30,   495, -1081,
     272, -1081,   499, -1081,   108, 19928,  9844, -1081, -1081, -1081,
   -1081, -1081, -1081, -1081, -1081, -1081, -1081, -1081, -1081, -1081,
   -1081, -1081, -1081, -1081, -1081, -1081, -1081, -1081, -1081, -1081,
   -1081, -1081,   497, -1081, -1081, 20456, 20456,   -32, 16255, -1081,
   -1081,   582,  9844,   502,   384,  6169,  1648,    42,    -1, -1081,
    6169, -1081, -1081, 17451, -1081,    15, 18039,   469, -1081,   507,
     509, -1081, 17451,   -30,   469, -1081,  9844,   944,   944,    37,
      18, -1081,    25, -1081, -1081, -1081, -1081, 10019, 10019, -1081,
     550, -1081,   498,   522, -1081, -1081,  1986,   537, -1081, -1081,
   19928,   213, 13694, 13694, -1081,   -49, -1081, -1081, 19928,   521,
   18084, -1081, 17451,   -30, 19928,   517,  9844, -1081, 19928, 18191,
   -1081, -1081, -1081, -1081, -1081,   -30,   525, -1081,   378,   378,
    3046,   469,   469,   303, -1081, -1081,  5294,    98, -1081, 13694,
      93,   112, -1081,  3046,   683, -1081, -1081, 20456, -1081, -1081,
    5119, -1081,   252, -1081,   582, -1081, -1081, -1081,   689,   523,
    5469, 13694, -1081, 13694,   631, -1081, -1081,   637,   236,   480,
   19928,   322, 13694,  6169, 11769, 12469, 12469, 11244, 11944, -1081,
   -1081, -1081, 20456, 13694,   527, -1081,   528, 20196,   573,   142,
   -1081,   261,   364, -1081, -1081, -1081, -1081,   610, -1081, -1081,
   -1081, -1081,    68,   612, -1081, -1081, -1081, 15726,   578,   260,
   -1081,   298,   440, -1081,   556, 13694, 13694, 13694, 12469, 12469,
   13694,   459, 13694, 13694, 13694, 13694, 13694,   332, 15896, 13694,
   13694, 13694, 13694, 13694, 13694, 13694, 13694, 13694, 13694, 13694,
   13694, 13694, 13694, 13694, 13694,   638, -1081, -1081, -1081, -1081,
   -1081, 11419, 11419, -1081, -1081, -1081, -1081, 11419, -1081, -1081,
   11419, 11419,  9844, 10194, 12469, 12469, 18236,   535, 13694, 13694,
   12644, 20456,  9144, -1081, -1081, -1081,  2910, -1081, 13694, -1081,
     583,   538,   565, -1081, -1081, -1081,   590,   593,  6169,   679,
    6169, -1081,   686, 13694,   656,   549, 11419, 20456, -1081, -1081,
   -1081,  1648, -1081, -1081,   133, -1081, 13869,   598, 13694,  2910,
   -1081, -1081, 13694, -1081, 20271, 13694, 13694, -1081,   551, -1081,
   -1081, -1081, -1081, 20326, -1081,   364, 15590, -1081,  1648, -1081,
     579, 13869, -1081, 19928, 19928, -1081,   495, -1081,    39, -1081,
   11419,   554, -1081,   708,   708, -1081, -1081, -1081, -1081, 12819,
   -1081, 18276,  9319, -1081,  9494, -1081, -1081, 11769, 12469, 12469,
   -1081,   585, 10369, -1081,   372, -1081,  5644, -1081,   341, 18396,
     382,  3791, 20456,  8094,  7919, -1081,   480,   562, -1081,   272,
   -1081, 17527,   617, 20456,   567,    21, 17147,    17,  3687,    17,
   16995,    21, 17451, 16068,   -33,   214, 18442,   -45,   -48,   591,
    1442, -1081, -1081, 13694,   -42, -1081, -1081,   149, -1081, -1081,
      56, -1081, -1081,  7744, -1081,   636, -1081,   571,   595, -1081,
     596, -1081,   601,   604,   605,   607, -1081,   609,   613,   614,
     615,   616,   618,   619, -1081, -1081, -1081, -1081, -1081, -1081,
   -1081, -1081, -1081, -1081, -1081, -1081, -1081, -1081, -1081, -1081,
   -1081, -1081, -1081, -1081, -1081, -1081, 13694,   611,   622,   571,
   -1081,   571, -1081, 13694, -1081, 20456, -1081, -1081,  2644,  2644,
     470, -1081,   470, -1081,   470,  2465,   161,   749,  1633,   -30,
     378, -1081,   472, -1081, -1081, -1081, -1081, -1081,  3046,  1239,
     470,  3137,  3137,  2644,  3137,  3137,   920,   378,  1239, 15461,
     920,   469,   469,   378,  3046,   594,   623,   641,   642,   643,
     644,   602,   599, -1081,   470, -1081,   470, 13694, 11419, 17603,
   17679,    85,   629,    50, -1081, -1081, -1081,   229, -1081,  1821,
   19973,   185,   383, 19928, 11419, -1081, 11419, 13694, 13694,   732,
    6169,   742, 18482,  6169,  3308, 20456, -1081,   237, -1081,   249,
   -1081,   250,  1648,    42, -1081, 19928, 10544, -1081, 19928, -1081,
   -1081, -1081, 20456, 18558, 18634, -1081,   579,   664, -1081,   256,
   13869, -1081,   307, -1081, 13694, -1081,   606,    -7,   639, -1081,
    3222, -1081, -1081, -1081, -1081, -1081, -1081, -1081, 17223, 17071,
     239, -1081, -1081,   296, -1081, -1081, -1081, -1081, 13694, -1081,
      76,  4560, 13694, -1081, 12994,  8094,  7919, -1081, 10719,   500,
   10894,   501,   520, 11069,  8619,   531,   387, -1081, -1081, -1081,
   18674,   666,   646,   647, -1081, 20456,  2910, 12469,    21, 13694,
     737, 13694, -1081,  6344, 12469, -1081,   648, -1081,    21,  6519,
   12469, -1081,    21, -1081,    21, 12469, -1081,    21, -1081, -1081,
   19928,  6169,   649, 12469, 13169,  4944,   655, 13694, 13344, -1081,
   -1081, 11419,   645,  1515, 19928, 20456,   452,   802, -1081,   636,
   -1081, -1081, -1081, -1081, -1081,     8,   489, -1081, 18720, -1081,
   16174, -1081, -1081, 19928, -1081,   650,   657, -1081, -1081, -1081,
   -1081, -1081, -1081, -1081, -1081,   297, 14044, 12469, 14219, 12469,
   12469, 16068, 11419, -1081, -1081,  9144, -1081, -1081, -1081, -1081,
   -1081, -1081, -1081, -1081, -1081, -1081, -1081, -1081,    58, 12469,
   12469, -1081, -1081, -1081, 19928, 19928,  6169, -1081,  6169, 13694,
   -1081,  6169,   784,   669,   670, 20456,   495, -1081, -1081,   598,
   -1081, -1081, -1081,   659,   661,   307, 13694,   636,   579, -1081,
   12119, -1081, -1081, 19928, -1081,  6169, 13694, -1081, -1081, -1081,
   20456, -1081,   676,   495, 12469, 12469, 12469,  6169, 11419, -1081,
   -1081, -1081, -1081, -1081,   472, -1081, -1081, -1081, -1081, -1081,
   -1081, -1081, -1081, -1081, -1081, -1081, -1081, -1081, -1081,  8094,
   -1081, -1081, 18826,    59, 17771,   395,   671,  8094, -1081, 13694,
   -1081, -1081,  2083,   398,   410,   147, 18880, -1081, 18920, 13694,
     299, 11244, 19928,    26, 17847,  9144, -1081, 11244, 19928,    24,
   17299, -1081, -1081,    17, 17375, -1081, -1081,  6169,     3, 17923,
   -1081, -1081, 19928,  6169, 19928, -1081, 19000,   302, -1081, -1081,
   -1081, -1081,  2392,  2101, -1081, -1081, 20196, -1081,     8,   131,
   13694, -1081,   690,   667, -1081,   571,   571, -1081, -1081, -1081,
   13694,   807, 19080, 13694,   811, 19160,   680, 19120,   306, -1081,
     598,   329,   350, -1081, -1081, 19928,   810,  6169, -1081, -1081,
   -1081, -1081, -1081,  9844,  9844, -1081, -1081, -1081, -1081,   307,
   19928, -1081,   263,    29,   672, 13694, -1081,    21, 17451,    17,
   17375,   136, 17999, -1081,   169, 13519,  8094,  7919,  8094,  7919,
   -1081, -1081, -1081, 19280, 19928,  2208, -1081, -1081, -1081,    21,
      21, -1081, 17451, -1081, 16335,  6694, -1081,  6869, -1081,   352,
   -1081, 16415,  7044, -1081,    21,  7219, -1081,    21, -1081,    21,
   -1081,    21, -1081,  7394,   687,  7569,   691, -1081, 13694,   681,
    2101, -1081, -1081, -1081, -1081, -1081,   106, -1081,   -22, -1081,
     530, 19240,    28, -1081, -1081, -1081, 19356, 14394, 14569, 19432,
   14744, 14919, 15094, 15269, -1081, -1081,  6169, -1081,   678,   688,
   -1081, -1081,  6169, -1081,   495, 19928,  6169,  6169, -1081, -1081,
   19928,   419,   699,   421,   700, 13694, -1081, -1081, -1081, -1081,
    6169, 11244, 19928, 11244, 19928,  9144, -1081,  6169, 11244, 19928,
   -1081, 11244, 19928, -1081, -1081, -1081, 11244, -1081, 19928,  6169,
   11244, -1081, 19928,  6169, 19928, -1081, -1081,    30, -1081, 13694,
     131,   106,   106,   106,   106,   106,   106,   131,  5994, -1081,
   -1081, -1081, -1081, -1081, 13694, 13694, 13694, 13694, 13694, 13694,
   13694, 13694, -1081,   705,   706,   617, -1081, -1081, -1081, -1081,
   -1081, 19928, 18482, 16495, 16575, -1081, 18482, 16655, 16735, 16815,
   -1081, 16895, -1081, -1081, 19928, -1081, -1081, -1081, -1081, -1081,
   -1081, -1081,   707, -1081, 19508, 19584, 19660, 19736, 19812, 19888,
   -1081, -1081,  6169,  6169,  6169,  6169,  6169,  6169, 13694, 13694,
   13694, 13694, 13694, 13694, 18482, 18482, 18482, 18482, 18482, 18482
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   142,   472,   689,   690,   691,
     685,   686,   692,   212,   601,   133,   163,   567,   170,   569,
     601,   169,   502,   500,   133,     0,   212,   287,   164,   202,
     201,   683,   200,   212,   168,    68,   288,   350,   165,     0,
       0,     0,   345,     0,   212,   172,   212,   212,   618,   593,
     693,   173,   212,   351,   561,   497,   212,     0,     0,   187,
     185,   438,   167,   570,   499,   212,   212,   212,   212,   565,
     212,     0,   171,     0,     0,   143,   212,   684,   212,   212,
     490,   166,   325,   563,   501,   174,   212,   742,     0,   212,
     744,   212,   745,   746,   634,   212,   743,   212,   642,   183,
     741,     0,   212,   212,     4,     0,     5,    10,    11,    12,
      47,    50,    51,    57,     0,    45,    71,    13,    77,    14,
      15,    16,    17,    29,   547,   548,    23,    46,   184,   194,
     212,   203,   656,   195,    18,     0,    31,    30,    20,     0,
     282,    19,   652,    22,    35,    32,    33,   193,   326,     0,
     191,     0,     0,   649,   349,     0,   646,   189,   366,   458,
     449,   644,   192,     0,     0,   190,   661,   640,   639,   643,
     551,   549,     0,   212,   650,   651,   655,   654,   653,     0,
     550,     0,   662,   663,   664,   687,   688,   645,   553,   552,
     647,   648,     0,    28,   569,   164,   212,     0,   570,   212,
     212,     0,     0,   649,   661,   549,   650,   651,   559,   550,
     662,   663,     0,     0,   602,   134,     0,   568,     0,   491,
       0,   498,     0,    21,     0,   535,   212,   140,   144,   156,
     150,   149,   158,   137,   148,   159,   145,   160,   135,   161,
     154,   147,   155,   153,   151,   152,   136,   138,   146,   157,
     162,   141,     0,   139,   211,     0,     0,     0,     0,    70,
      69,    71,   212,     0,     0,     0,     0,     0,     0,   505,
       0,   180,    40,     0,   317,     0,   316,   727,   622,   619,
     620,   621,     0,   562,   728,     7,   212,   348,   348,   445,
     177,   440,   178,   441,   442,   446,   447,   176,   175,   448,
     438,   531,     0,   353,   354,   356,     0,   439,   530,   358,
     518,     0,   212,   212,   181,   646,   637,   660,   638,     0,
       0,    43,     0,   566,   554,     0,   212,    44,   560,     0,
     295,    47,   299,   296,   299,   564,     0,    56,   729,   731,
     632,   726,   725,     0,    74,    78,     0,     0,   537,   212,
       0,     0,   538,   633,     0,     6,   323,     0,   201,   324,
       0,    48,     0,     9,    71,    49,    52,    55,     0,    54,
       0,   212,    72,   212,     0,   502,   199,     0,   685,   326,
     657,   207,   212,     0,   212,   212,   212,   212,   212,   214,
     571,   584,     0,   212,     0,   322,     0,     0,   343,     0,
     336,   438,   458,   456,   457,   455,   374,   460,   459,   463,
     462,   464,     0,   453,   450,   451,   454,     0,   495,     0,
     492,     0,   641,    34,   623,   212,   212,   212,   212,   212,
     212,   730,   212,   212,   212,   212,   212,     0,   631,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   405,   412,   413,   414,   409,
     411,   212,   212,   407,   410,   408,   406,   212,   416,   415,
     212,   212,   212,   212,   212,   212,     0,     0,   212,   212,
     212,     0,   212,    36,    24,    37,     0,    25,   212,    38,
     547,     0,   542,   543,   546,   335,     0,     0,     0,   256,
       0,   405,   252,   212,     0,     0,   212,     0,   266,    26,
     509,     0,   510,   512,     0,   529,   212,   515,   212,     0,
     179,    39,   212,   319,     0,   212,   212,    41,     0,   188,
     186,   443,   444,     0,   439,   458,   438,   431,     0,   430,
     533,   212,   126,   658,   659,   364,     0,   182,     0,    42,
     212,     0,   306,   297,   298,    27,    76,    75,    79,   212,
     694,     0,   212,   679,   212,   681,     8,   212,   212,   212,
     213,    57,   212,    53,     0,    65,     0,   124,     0,   119,
       0,    86,     0,   212,   212,   196,   326,     0,   209,     0,
     206,   601,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   585,     0,     0,   535,     0,   293,   327,
       0,   334,   340,   212,   338,   333,   423,     0,   425,   429,
       0,   461,   523,     0,   525,   465,   452,   421,   140,   401,
     144,   399,   150,   149,   148,   145,   403,   160,   161,   147,
     151,   136,   146,   162,   398,   380,   383,   381,   382,   404,
     393,   384,   397,   389,   387,   400,   388,   386,   391,   396,
     385,   390,   394,   395,   392,   402,   212,   377,     0,   421,
     378,   421,   375,   212,   494,     0,   489,   504,   722,   721,
     724,   733,   732,   737,   736,   718,   715,   716,   717,   635,
     705,   142,     0,   675,   676,   143,   674,   673,   629,   709,
     720,   714,   712,   723,   713,   711,   703,   708,   710,   719,
     702,   706,   707,   704,   630,     0,     0,     0,     0,     0,
       0,     0,     0,   735,   734,   739,   738,   212,   212,     0,
       0,     0,   293,   612,   613,   615,   617,     0,   604,     0,
       0,     0,     0,   536,   212,   556,   212,   212,   212,   255,
       0,   251,     0,     0,     0,     0,   555,     0,   269,     0,
     267,     0,   513,     0,   528,   527,   212,   507,   628,   506,
     318,   315,     0,     0,     0,   672,   533,   359,   355,     0,
     212,   534,   515,   357,   212,   363,     0,     0,     0,   557,
       0,   300,   695,   636,   680,   539,   682,   540,     0,     0,
       0,    58,    59,     0,    61,    63,    67,    66,   212,   101,
       0,     0,   212,    94,   212,   212,   212,    64,   212,   380,
     212,   381,   382,   212,   212,   392,     0,   417,   418,    81,
      80,    93,     0,     0,   328,     0,     0,   212,     0,   212,
       0,   212,   224,     0,   212,   223,     0,   232,     0,     0,
     212,   230,     0,   240,     0,   212,   238,     0,   217,   216,
     586,     0,     0,   212,   212,     0,     0,   212,   212,   698,
     290,   212,     0,   348,   344,     0,     0,   438,   424,   465,
     521,   520,   519,   522,   466,   473,   438,   373,     0,   379,
       0,   369,   370,   496,   493,     0,     0,   132,   130,   131,
     129,   128,   127,   670,   671,     0,   212,   212,   212,   212,
     212,   696,   212,   614,   616,   212,   603,   163,   170,   169,
     168,   165,   172,   173,   167,   171,   166,   174,     0,   212,
     212,   503,   545,   544,   264,   263,     0,   258,     0,   212,
     254,     0,   260,     0,   292,     0,     0,   511,   514,   515,
     516,   517,   314,     0,     0,   515,   212,   465,   533,   532,
     212,   434,   432,   365,   308,     0,   212,   307,   310,   558,
       0,   301,   304,     0,   212,   212,   212,     0,   212,    60,
     125,   122,   102,   114,   108,   107,   106,   116,   103,   117,
     112,   105,   113,   111,   109,   110,   104,   115,   118,   212,
     121,   120,    89,    88,    87,     0,     0,   212,   197,   212,
     198,   328,   348,     0,     0,   601,   601,   220,     0,   212,
       0,   212,   574,     0,     0,   212,   233,   212,   580,     0,
       0,   231,   241,     0,     0,   239,   248,     0,     0,     0,
     697,   246,   587,     0,   700,   699,     0,     0,   321,   187,
     185,   320,   348,   348,   329,   339,     0,   426,   473,     0,
     212,   367,   428,     0,   376,   421,   421,   677,   678,   292,
     212,     0,     0,   212,     0,     0,     0,     0,     0,   605,
     515,   661,   661,   257,   253,   592,   259,     0,    73,   289,
     268,   265,   508,   212,   212,   435,   433,   360,   361,   515,
     420,   312,     0,     0,     0,   212,   302,     0,     0,     0,
       0,     0,     0,    62,     0,   212,   212,   212,   212,   212,
      95,    98,    84,    83,    82,   348,   204,   210,   208,     0,
       0,   215,     0,   250,     0,     0,   222,     0,   221,     0,
     607,     0,     0,   228,     0,     0,   226,     0,   236,     0,
     234,     0,   249,     0,     0,     0,     0,   247,   212,   294,
     348,   331,   330,   342,   346,   487,     0,   488,   475,   478,
       0,   474,     0,   422,   371,   372,     0,   212,   212,     0,
     212,   212,   212,   212,   294,   609,     0,   262,     0,     0,
     437,   436,     0,   311,     0,   305,     0,     0,   123,    91,
      90,     0,     0,     0,     0,   212,   205,   219,   218,   225,
       0,   212,   573,   212,   572,   212,   606,     0,   212,   579,
     229,   212,   578,   227,   237,   235,   212,   244,   589,     0,
     212,   242,   588,     0,   701,   291,   332,     0,   486,   212,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   467,
     469,   471,   368,   468,   212,   212,   212,   212,   212,   212,
     212,   212,   261,   624,   625,   313,   303,    97,   100,    96,
      99,    85,   577,     0,     0,   608,   583,     0,     0,     0,
     245,     0,   243,   347,   477,   479,   480,   483,   484,   485,
     481,   482,   476,   470,     0,     0,     0,     0,     0,     0,
     626,   627,     0,     0,     0,     0,     0,     0,   212,   212,
     212,   212,   212,   212,   576,   575,   582,   581,   591,   590
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1081, -1081, -1081,     0,   -58,  3354, -1081,    36,   761, -1081,
   -1081,   503,  -145,  -106, -1081,   304, -1081, -1081,  -105, -1081,
   -1081,    48,   627, -1081,  -536,  3487,   506,  -572, -1081,  -786,
   -1081, -1081, -1081,    66, -1081, -1081, -1081,   854, -1081,  2306,
    -208, -1081, -1081,  -468,  1606, -1080,  -828, -1081, -1081,   113,
   -1081, -1081, -1081, -1081, -1081, -1081, -1081, -1081,     7, -1081,
     541,  -322, -1081,   512, -1081, -1081,   -66,  1917, -1081, -1081,
   -1081, -1081,   555, -1081,   -87, -1081, -1081, -1081, -1081,   386,
   -1081, -1081, -1081,   -99, -1081,  -344,  -793, -1081, -1081, -1081,
   -1081, -1081,  -384, -1081,   134, -1081, -1081, -1081, -1081, -1081,
     368, -1081,   -11, -1081, -1081, -1081, -1081,   519, -1081, -1081,
   -1081, -1081,    16,  -403,  -176,  -777,  -917,  -647, -1081,    19,
   -1081,    33,   379,   -46,   620,   -44, -1081, -1081,  -358,  -814,
   -1081,  -324, -1081,  -144,  -329,  -321,  -760, -1081, -1081,    86,
     247, -1081,  -201,   793, -1081,  -156,   405,   163,  -357,  -754,
    -602, -1081, -1081, -1081,  -591,  -510, -1081,  -739,   -14,    83,
   -1081,  -236,  -479,  -530,    -2, -1081, -1081, -1081, -1081, -1081,
   -1081,   143, -1081,   -19, -1081, -1081,  -434, -1081,  -890, -1081,
   -1081,   668,   747, -1081, -1081, -1081, -1081,  3054, -1081,   -50,
    1216, -1081,  1353,  1728, -1081, -1081, -1081, -1081, -1081, -1081,
   -1081, -1081, -1081,  -475
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   345,   105,   751,   107,   108,   109,   362,
     363,   110,   111,   112,   113,   369,   573,   803,   804,   114,
     115,   116,   374,   117,   118,   119,   346,   831,   580,   832,
     120,   121,  1000,   577,   578,   122,   123,   216,   251,   267,
     124,   253,   125,   301,   845,   126,   127,   288,   287,   128,
     129,   130,   131,   132,   133,   589,   835,   134,   201,   136,
     389,   595,   137,   257,   138,   759,   760,   202,   140,   141,
     142,   143,   553,   791,   973,   144,   145,   787,   968,   274,
     275,   146,   147,   148,   149,   394,   873,   150,   151,   399,
     875,   876,   400,   152,   203,  1237,   154,   155,   302,   303,
     304,   957,   156,   316,   546,   784,   157,   158,  1172,   159,
     160,   668,   669,   827,   828,   829,   961,   887,   402,   617,
    1063,   618,   540,   619,   306,   534,   417,   406,   412,   885,
    1252,  1253,   161,  1061,  1168,  1169,  1170,   162,   163,   419,
     420,   674,   164,   165,   220,   268,   269,   513,   514,   767,
     308,   883,   624,   625,   517,   309,   781,   782,   347,   350,
     351,   491,   492,   493,   204,   167,   168,   390,   169,   391,
     170,   205,   172,   213,   214,   737,   852,  1139,   738,   739,
     173,   206,   207,   176,   177,   178,   317,   179,   381,   494,
     209,   181,   210,   211,   184,   185,   186,   187,   607,   188,
     189,   190,   191,   192
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     166,   218,   104,   300,   471,   366,   397,   741,   421,   135,
     785,   826,   224,   612,   670,   305,   319,   307,   490,   486,
     788,   882,   891,   166,   892,  1079,   505,   757,   962,   879,
    1006,   783,   135,   379,  1001,   587,   336,   955,  1095,   367,
     786,  1012,   805,   515,   620,  1054,   518,   270,   764,   270,
     528,   311,   352,   270,   490,   315,  1142,   515,  1135,   515,
    1248,   270,  1248,   597,   599,  1058,   604,   965,  1059,   622,
     498,     3,   166,  -287,   545,   315,   166,   166,   490,   691,
    -288,   135,   503,   261,   531,   135,   135,   846,  -526,  -526,
     551,   315,  1251,   478,   479,  1116,  1239,   913,  -341,   166,
    -524,   371,   868,   166,   193,  -337,   681,   683,   135,  1165,
     966,   431,   135,  -309,   330,   333,  -526,   870,   490,   437,
      66,  -362,    66,   846,   871,  1240,  1060,   532,  -524,  -337,
     869,  1117,   910,   382,  1165,  1140,   153,   846,   360,   372,
     914,   383,   861,  1098,   846,   171,   519,  -270,  -526,   384,
     385,   386,   723,   725,   761,   881,  -309,  1251,   364,   153,
    -524,  -526,   522,   848,   967,   854,   857,  -526,   171,   373,
     862,   866,   949,  -524,  -526,   215,   488,   777,  1153,   695,
     368,   779,  1190,  -526,  1054,   360,  -524,   360,   516,  -526,
     226,   360,   427,  1249,   360,  1092,   360,   262,   360,   360,
     360,  1096,   516,   259,   516,  -526,   260,  -526,   153,  1167,
    -309,   166,   153,   153,   623,   315,   933,   171,  1125,  1099,
     135,   171,   171,  1166,  1161,  1162,  -526,   480,   387,   697,
    1122,  -526,   864,  -524,  1167,   153,   721,   722,  -524,   153,
     562,   671,   833,  1005,   171,   559,   597,   604,   171,   905,
     428,   315,   -70,   474,   429,   488,   846,   475,   356,   564,
     911,   863,  -271,   166,   490,   490,   563,   212,   166,   560,
     959,   -70,   135,   489,   930,   315,   382,   135,   475,   586,
     762,   -69,  -277,   388,   383,   565,   976,   382,   289,   614,
     -70,   366,   567,   568,   386,  1192,   877,  1054,   490,    23,
     -69,  -276,   431,   384,   385,   386,   763,   435,   291,   436,
     437,  1196,   293,   615,   440,   315,  1007,   219,   221,   -69,
     -70,   447,   878,   772,   359,  1275,  1185,   451,   452,   453,
     742,  1202,  1236,  1204,   846,   691,   294,  1080,   226,   295,
    1198,    57,   490,   300,   166,  1191,   558,   153,    82,   -69,
      55,   296,   692,   135,  1097,   472,   171,   473,   166,   846,
     488,   864,    64,   403,   490,   693,   866,   135,   166,   278,
     299,   387,    37,   413,   605,  -348,   915,   135,   542,   609,
     254,   166,   387,   404,   746,  -285,   488,    84,   836,   865,
     135,   407,  1047,   405,   694,  -348,   945,   762,    53,   153,
     529,   530,   916,   762,   153,  -348,  1238,   675,   171,   427,
     944,   716,   717,   171,   977,   672,  -275,   718,  1174,  1175,
     719,   720,   946,   947,   414,   676,   569,  1114,   279,   958,
     262,   588,   766,  1078,   616,   695,   415,   388,   285,   408,
     741,   696,   409,   978,   746,   519,   488,   280,   805,   746,
     315,   315,  -272,   746,   410,   416,   315,  -279,   960,   315,
     315,   315,   315,   677,   281,   286,   731,   428,   193,   979,
    1069,   429,  1133,   411,   556,  1159,  -611,  -283,   300,  1184,
     153,  1286,  1287,  1288,  1289,  1290,  1291,   670,   808,   171,
     305,  1055,   307,  -280,   153,   315,   166,  -610,   166,  1215,
     427,  -598,  -611,   171,   153,   135,   809,   135,  -668,  -669,
    -668,  -669,   352,   171,   352,  1015,   289,   153,  -274,   431,
     490,  -278,  1023,  -610,   435,  1216,   171,   437,  1029,   812,
     519,   440,   325,  1033,  1007,   326,   291,   193,   490,   315,
     293,  1038,  1007,   806,  1201,   675,  1203,   813,   931,  -284,
     741,   315,  1008,   315,   360,   800,   507,   519,   428,  -286,
    1120,   315,   429,  1127,   294,  -273,  1007,   295,  1007,    57,
    -281,  -668,   838,  -668,   166,  1128,   558,   289,  -665,   296,
    -665,   331,   331,   135,  1267,  1071,  1269,  1074,  1076,  -744,
    -745,   337,    88,  -744,  -745,  1144,  1147,   291,   299,  1149,
    1151,   293,   981,   697,  1154,  1156,   370,   681,   723,  -746,
     431,   431,  -667,  -746,  -667,   435,   435,   436,   437,   437,
    -741,   392,   440,   440,  -741,   294,   393,   401,   295,   447,
     354,  -666,   153,  -666,   153,   422,  1250,   453,  -595,  -594,
     296,   171,   423,   171,   895,   795,   896,   797,  -599,   425,
    -600,  -597,  1107,  1109,  1111,  1241,   426,  -596,   481,   299,
     482,   484,  -427,   490,   487,   360,   372,   495,   427,   536,
     174,   535,  1163,   583,   506,  1144,  1147,  1154,  1156,   525,
    1014,   526,  1242,   541,  1243,  1244,   547,  1245,  1246,   550,
     555,   566,   571,   174,   932,   572,   582,   610,   613,   611,
     621,  1250,   626,   673,   490,   437,   715,   728,   744,   584,
     153,   745,   746,   750,  1066,   747,   951,   315,   748,   171,
     753,   755,   756,   766,   775,   780,   428,   789,  1053,   790,
     429,  1056,   834,   315,   368,   315,   839,   928,   488,   841,
     741,   884,   174,   886,  -142,  -163,   174,   174,   166,   175,
    -170,   166,   889,  -169,  -168,   315,  -165,   135,  -172,   897,
     135,   430,  -173,  -167,  -171,  -143,   936,  -166,  -174,   174,
     490,   890,   175,   174,   904,   903,   938,   964,   431,   432,
     427,   433,   434,   435,  1019,   436,   437,   438,   898,   439,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   912,   585,   451,   452,   453,   899,   900,   901,   902,
     956,  1010,   969,  1007,   454,  1052,  1048,  1011,  1087,   300,
    1025,   175,  1105,  1067,  1037,   175,   175,  1020,   300,   289,
    1043,  1093,  1068,  1094,  1088,  1089,  1121,   877,   428,  1177,
    1173,   166,   429,  1180,  1186,  1194,  1235,   166,   175,   291,
     135,  1263,   175,   293,  1240,  1182,   135,  1188,  1189,   166,
     315,  1264,  1229,   166,  1268,  1270,  1233,  1053,   135,  1300,
    1301,   361,   135,  1113,   980,   801,   576,   294,   222,  1090,
     295,   174,    57,  1104,   153,   490,   490,   153,   504,   554,
     431,   432,   296,   171,   434,   435,   171,   436,   437,   601,
     590,   315,   440,   570,   778,  1062,  1065,  1160,   770,   447,
    1057,   299,   776,  1283,  1164,   451,   452,   453,  1292,  1285,
     533,  1013,   894,   376,   769,   948,     0,  1081,  1082,     0,
       0,     0,     0,   174,   166,     0,   166,     0,   174,   166,
       0,     0,     0,   135,     0,   135,     0,   366,   135,     0,
     366,   427,  1103,     0,  1052,     0,     0,     0,   503,     0,
     175,     0,     0,   166,     0,   503,   356,   315,     0,     0,
      22,    23,  1102,     0,     0,   166,     0,   153,     0,   357,
    1053,    29,   358,   153,   135,     0,   171,    32,     0,     0,
       0,     0,   171,     0,    37,   153,  1129,  1130,     0,   153,
       0,     0,     0,     0,   171,     0,     0,     0,   171,   428,
       0,     0,   175,   429,   174,     0,     0,   175,     0,     0,
      53,     0,    55,     0,     0,     0,     0,     0,   174,     0,
       0,     0,   359,     0,    64,   166,     0,     0,   174,     0,
       0,   166,     0,     0,   135,     0,     0,     0,     0,     0,
     135,   174,     0,    80,   366,     0,    82,     0,     0,    84,
       0,   431,   432,     0,   433,   434,   435,  1052,   436,   437,
     153,     0,   153,   440,     0,   153,     0,     0,     0,   171,
     447,   171,   315,   315,   171,   166,   451,   452,   453,     0,
       0,     0,     0,   175,   135,     0,     0,   503,   503,   153,
       0,   503,   503,   503,     0,   503,     0,   175,   171,     0,
       0,   153,     0,     0,     0,     0,     0,   175,     0,     0,
     171,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     175,     0,     0,   166,     0,   166,     0,     0,     0,     0,
     166,     0,   135,   166,   135,     0,     0,     0,     0,   135,
       0,   166,   135,   166,     0,     0,     0,     0,     0,     0,
     135,     0,   135,     0,     0,     0,   174,     0,   174,     0,
       0,   153,     0,     0,     0,     0,     0,   153,     0,     0,
     171,     0,     0,     0,   166,     0,   171,     0,     0,     0,
     166,     0,     0,   135,   166,   166,     0,     0,     0,   135,
       0,     0,     0,   135,   135,     0,     0,     0,   166,     0,
       0,     0,     0,     0,     0,   166,     0,   135,   180,     0,
       0,   153,     0,     0,   135,     0,     0,   166,     0,     0,
     171,   166,     0,     0,     0,     0,   135,     0,     0,     0,
     135,   180,     0,     0,   174,   175,   166,   175,  1293,     0,
       0,     0,     0,     0,     0,   135,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   153,
     427,   153,     0,     0,     0,     0,   153,     0,   171,   153,
     171,     0,     0,     0,     0,   171,     0,   153,   171,   153,
     180,     0,     0,     0,   180,   180,   171,     0,   171,     0,
     166,   166,   166,   166,   166,   166,     0,     0,     0,   135,
     135,   135,   135,   135,   135,     0,     0,   180,     0,     0,
     153,   180,     0,   175,     0,     0,   153,     0,   428,   171,
     153,   153,   429,     0,     0,   171,     0,     0,     0,   171,
     171,     0,     0,     0,   153,     0,     0,     0,     0,     0,
       0,   153,     0,   171,     0,   182,     0,     0,     0,     0,
     171,     0,     0,   153,     0,     0,     0,   153,     0,     0,
       0,     0,   171,     0,     0,     0,   171,     0,   182,     0,
     431,   432,   153,   433,   434,   435,     0,   436,   437,   438,
       0,   171,   440,   441,   442,     0,   444,   445,   446,   447,
       0,     0,   450,     0,     0,   451,   452,   453,     0,     0,
       0,     0,     0,     0,     0,     0,   454,     0,   174,     0,
       0,   174,     0,     0,     0,     0,     0,   182,     0,   180,
       0,   182,   182,     0,     0,     0,   153,   153,   153,   153,
     153,   153,     0,   872,     0,   171,   171,   171,   171,   171,
     171,     0,     0,     0,   182,     0,     0,     0,   182,     0,
       0,     0,     0,     0,  -328,     0,     0,     0,  -328,  -328,
       0,     0,     0,     0,     0,     0,     0,  -328,     0,  -328,
    -328,   180,     0,     0,     0,  -328,   180,     0,     0,     0,
       0,     0,  -328,     0,     0,  -328,     0,   175,     0,     0,
     175,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   174,     0,     0,     0,  -328,     0,   174,  -328,     0,
    -328,     0,  -328,     0,  -328,  -328,     0,  -328,     0,   174,
    -328,     0,  -328,   174,     0,     0,     0,   356,     0,     0,
       0,    22,    23,     0,     0,     0,     0,     0,     0,     0,
     357,  -328,    29,   358,  -328,     0,     0,  -328,    32,     0,
       0,     0,   180,     0,     0,    37,   182,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   180,     0,     0,     0,
       0,     0,  -328,     0,     0,     0,   180,     0,     0,     0,
     175,    53,     0,    55,     0,    57,   175,  1049,     0,   180,
    1050,     0,     0,   359,   174,    64,   174,  -328,   175,   174,
       0,     0,   175,  -328,     0,     0,     0,     0,   182,     0,
       0,     0,     0,   182,    80,     0,     0,    82,     0,     0,
      84,     0,     0,   174,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   174,     0,     0,     0,     0,
     272,   227,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,   427,   228,   229,     0,   230,     0,
       0,     0,     0,   231,   321,     0,     0,     0,     0,     0,
      99,   232,     0,   175,     0,   175,  1051,   233,   175,     0,
       0,     0,     0,   234,     0,     0,     0,   235,     0,   182,
     236,     0,     0,     0,     0,   174,     0,     0,     0,     0,
     237,   174,   175,   182,   180,     0,   180,   238,   239,     0,
       0,     0,   428,   182,   175,   240,   429,     0,     0,     0,
     183,     0,     0,     0,     0,   241,   182,     0,     0,     0,
       0,     0,     0,     0,   242,   243,     0,   244,     0,   245,
       0,   246,     0,   183,   247,   174,     0,     0,   248,   510,
       0,   249,     0,     0,   250,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   431,   432,     0,     0,     0,   435,
       0,   436,   437,     0,   175,     0,   440,     0,     0,     0,
     175,     0,   180,   447,     0,     0,     0,     0,     0,   451,
     452,   453,   183,   174,     0,   174,   183,   183,     0,     0,
     174,     0,     0,   174,     0,     0,     0,     0,     0,     0,
     511,   174,     0,   174,   691,     0,     0,     0,     0,   183,
       0,     0,     0,   183,   175,     0,     0,     0,   917,     0,
       0,   918,     0,     0,     0,     0,   919,     0,     0,     0,
       0,   182,     0,   182,   174,     0,   195,     0,     0,     0,
     174,     0,     0,     0,   174,   174,   920,     0,     0,     0,
       0,     0,     0,   921,     0,     0,     0,     0,   174,   521,
       0,   523,   175,   922,   175,   174,     0,     0,   527,   175,
       0,   923,   175,     0,     0,     0,     0,   174,     0,     0,
     175,   174,   175,     0,     0,     0,     0,     0,   924,     0,
       0,     0,     0,     0,     0,     0,   174,     0,     0,   139,
     925,     0,     0,     0,   695,     0,     0,     0,   549,   182,
       0,   926,     0,   175,     0,     0,     0,   927,     0,   175,
       0,   183,   139,   175,   175,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   175,     0,     0,
       0,     0,     0,     0,   175,     0,   180,     0,     0,   180,
     174,   174,   174,   174,   174,   174,   175,     0,     0,     0,
     175,     0,     0,     0,     0,     0,     0,     0,     0,   227,
       0,   139,     0,   183,     0,   175,     0,     0,   183,     0,
       0,     0,     0,   228,   229,     0,   230,     0,     0,     0,
       0,   231,     0,     0,     0,     0,     0,     0,   139,   232,
       0,     0,   139,     0,     0,   233,     0,     0,     0,     0,
       0,   234,     0,     0,     0,   235,     0,     0,   236,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   237,   175,
     175,   175,   175,   175,   175,   238,   239,     0,     0,   180,
       0,     0,     0,   240,     0,   180,     0,     0,     0,     0,
       0,     0,     0,   241,   183,     0,     0,   180,     0,     0,
       0,   180,   242,   243,     0,   244,     0,   245,   183,   246,
       0,     0,   247,     0,     0,     0,   248,   537,   183,   249,
       0,     0,   250,   182,     0,   356,   182,     0,     0,    22,
      23,   183,     0,     0,     0,     0,     0,     0,   357,     0,
      29,   358,     0,   356,     0,     0,    32,    22,    23,     0,
     139,     0,     0,    37,     0,     0,   357,     0,    29,   358,
       0,     0,     0,     0,    32,     0,     0,     0,     0,     0,
       0,    37,   180,     0,   180,     0,     0,   180,   538,    53,
       0,    55,     0,    57,     0,  1049,     0,     0,  1050,     0,
       0,   359,     0,    64,     0,     0,     0,    53,     0,    55,
       0,   180,   139,  1049,     0,     0,  1050,   139,     0,   359,
       0,    64,    80,   180,     0,    82,   182,     0,    84,     0,
       0,   842,   182,   847,   851,   853,   856,   858,   859,     0,
      80,     0,     0,    82,   182,     0,    84,     0,   182,     0,
       0,     0,     0,    88,     0,     0,   183,     0,   183,     0,
     356,     0,     0,     0,    22,    23,     0,     0,     0,     0,
       0,    88,     0,   357,     0,    29,   358,     0,    99,     0,
       0,    32,     0,   180,  1126,     0,     0,     0,    37,   180,
       0,     0,     0,   139,     0,     0,    99,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   139,     0,     0,
       0,     0,     0,     0,    53,     0,    55,   139,    57,   182,
    1049,   182,     0,  1050,   182,     0,   359,     0,    64,     0,
     139,     0,     0,   180,   183,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    80,   182,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
     182,     0,     0,     0,   252,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   263,   264,   265,    88,     0,
       0,   180,     0,   180,     0,     0,     0,     0,   180,     0,
       0,   180,     0,     0,     0,     0,     0,     0,     0,   180,
       0,   180,     0,    99,     0,     0,     0,     0,     0,  1206,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     182,     0,     0,     0,     0,     0,   182,     0,     0,     0,
       0,     0,   180,     0,     0,   851,     0,     0,   180,     0,
       0,     0,   180,   180,   356,   139,     0,   139,    22,    23,
       0,     0,     0,     0,     0,     0,   180,   357,     0,    29,
     358,     0,     0,   180,     0,    32,     0,     0,     0,     0,
     182,     0,    37,     0,  1017,   180,     0,     0,     0,   180,
       0,     0,     0,     0,  1026,   395,     0,   398,  1031,     0,
    1032,     0,     0,  1035,   180,     0,     0,     0,    53,   418,
      55,     0,   354,     0,  1049,     0,     0,  1050,   183,     0,
     359,   183,    64,     0,     0,     0,     0,     0,   182,     0,
     182,     0,     0,   139,     0,   182,   427,     0,   182,     0,
       0,    80,     0,   477,    82,     0,   182,    84,   182,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   180,   180,
     180,   180,   180,   180,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,   182,
       0,     0,     0,     0,     0,   182,     0,     0,     0,   182,
     182,     0,     0,     0,   428,     0,     0,    99,   429,     0,
       0,   496,   497,   182,     0,     0,     0,     0,     0,     0,
     182,   183,   512,     0,     0,     0,     0,   183,     0,     0,
       0,     0,   182,     0,     0,     0,   182,     0,     0,   183,
       0,     0,     0,   183,     0,     0,     0,     0,     0,     0,
       0,   182,     0,     0,     0,     0,   431,   432,     0,   433,
     434,   435,   539,   436,   437,   438,     0,   439,   440,   441,
     442,     0,   444,   445,   446,   447,   448,     0,   450,  1136,
    1138,   451,   452,   453,     0,  1143,  1146,     0,     0,  1148,
    1150,     0,   454,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   182,   182,   182,   182,   182,
     182,     0,     0,   252,   183,     0,   183,   139,     0,   183,
     139,     0,     0,     0,     0,   427,     0,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,     0,   183,     0,     0,     0,     0,   608,     0,
       0,     0,     0,   398,     0,   183,     0,     0,     0,  1193,
       0,     0,     0,  1136,  1138,  1143,  1146,     0,     0,     0,
       0,   470,     0,   667,  -665,     0,  -665,     0,     0,     0,
       0,     0,     0,   428,     0,  1207,  1208,   429,  1209,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1220,     0,     0,  1223,     0,  1224,     0,  1225,     0,     0,
     139,     0,     0,     0,     0,   183,   139,     0,   430,     0,
       0,   183,     0,     0,     0,     0,     0,     0,   139,     0,
       0,     0,   139,     0,     0,   431,   432,   732,   433,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,   442,
       0,   444,   445,   446,   447,   448,   449,   450,     0,     0,
     451,   452,   453,   758,     0,   183,     0,   512,     0,     0,
       0,   454,     0,     0,     0,     0,     0,     0,     0,     0,
     771,     0,     0,     0,     0,     0,     0,     0,     0,   539,
       0,     0,     0,     0,   512,     0,     0,     0,     0,     0,
       0,     0,     0,   139,     0,   139,     0,     0,   139,     0,
       0,     0,     0,   183,     0,   183,     0,     0,     0,     0,
     183,     0,     0,   183,     0,     0,     0,     0,     0,     0,
       0,   183,   139,   183,     0,     0,     0,     0,   817,     0,
       0,     0,     0,     0,   139,     0,     0,     0,     0,   840,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   227,   183,     0,     0,     0,     0,     0,
     183,     0,     0,     0,   183,   183,     0,   228,   229,     0,
     230,     0,     0,     0,     0,   231,     0,     0,   183,     0,
       0,     0,     0,   232,     0,   183,     0,     0,     0,   233,
       0,     0,     0,     0,   139,   234,     0,   183,     0,   235,
     139,   183,   236,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   237,     0,     0,     0,   183,     0,     0,   238,
     239,   418,     0,     0,     0,     0,     0,   240,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,     0,   139,     0,   242,   243,     0,   244,
       0,   245,     0,   246,     0,     0,   247,     0,     0,     0,
     248,     0,     0,   249,     0,     0,   250,     0,     0,     0,
     183,   183,   183,   183,   183,   183,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   139,     0,   139,     0,     0,     0,     0,   139,
       0,   943,   139,     0,     0,     0,     0,   208,   512,     0,
     139,   217,   139,     0,     0,     0,     0,   427,   952,     0,
     225,     0,   266,     0,     0,     0,     0,   258,     0,     0,
       0,     0,     0,     0,     0,     0,   972,     0,   273,     0,
     276,   277,     0,   139,     0,     0,   282,     0,   283,   139,
     284,     0,     0,   139,   139,   310,     0,     0,     0,   225,
     318,   320,   322,   323,   324,     0,     0,   139,   328,     0,
     329,     0,     0,     0,   139,   428,     0,   335,     0,   429,
     318,   418,     0,   338,     0,   339,   139,     0,   340,   341,
     139,   342,     0,     0,     0,   225,   318,   353,     0,     0,
       0,     0,     0,     0,     0,   139,     0,     0,   427,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   398,     0,     0,   377,     0,   380,   431,   432,     0,
     433,   434,   435,     0,   436,   437,   667,     0,     0,   440,
       0,     0,     0,     0,     0,   446,   447,     0,     0,   450,
       0,     0,   451,   452,   453,     0,     0,     0,     0,   139,
     139,   139,   139,   139,   139,   227,   428,   424,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,   228,
     229,     0,   230,     0,     0,     0,     0,   231,     0,     0,
     476,   758,     0,   328,   225,   232,     0,     0,     0,     0,
       0,   233,     0,     0,     0,     0,     0,   234,     0,     0,
       0,   235,     0,     0,   236,     0,   972,     0,   431,   432,
     318,   433,   434,   435,   237,   436,   437,   438,     0,     0,
     440,   238,   239,     0,     0,     0,   446,   447,     0,   240,
     450,     0,     0,   451,   452,   453,     0,     0,     0,   241,
       0,     0,     0,     0,   454,     0,   318,     0,   242,   243,
     425,   244,     0,   245,     0,   246,     0,   426,   247,     0,
       0,     0,   248,     0,     0,   249,     0,     0,   250,   427,
     318,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   324,   328,     0,     0,     0,   106,     0,     0,     0,
       0,     0,   398,     0,     0,     0,   543,   544,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   223,
     318,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   360,     0,   970,     0,     0,   428,     0,     0,
       0,   429,     0,   561,     0,     0,     0,     0,     0,     0,
     941,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   579,     0,   581,   327,     0,
       0,     0,   430,     0,     0,     0,   591,     0,   596,   598,
     600,   602,   606,     0,     0,     0,     0,   225,     0,   431,
     432,     0,   433,   434,   435,   106,   436,   437,   438,   355,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,     0,     0,   451,   452,   453,     0,   360,   678,
     679,   680,   682,   684,   685,   454,   686,   687,   688,   689,
     690,     0,   698,   699,   700,   701,   702,   703,   704,   705,
     706,   707,   708,   709,   710,   711,   712,   713,   714,     0,
       0,     0,     0,     0,     0,   318,   318,     0,     0,     0,
       0,   318,     0,     0,   318,   318,   318,   318,   724,   726,
       0,   271,   729,   730,   606,     0,   740,     0,     0,     0,
       0,     0,   743,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   752,   271,     0,   754,     0,     0,
     318,     0,     0,     0,     0,   332,   334,   483,     0,     0,
     765,     0,   768,     0,     0,     0,   276,     0,     0,   773,
     774,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     310,     0,     0,     0,     0,   310,     0,   365,     0,     0,
       0,     0,     0,     0,   318,     0,     0,     0,     0,     0,
       0,     0,     0,   743,     0,     0,   318,     0,   318,   509,
       0,   798,   799,   225,   520,     0,   318,     0,     0,     0,
       0,     0,     0,     0,     0,   356,     0,   830,   830,    22,
      23,     0,     0,     0,     0,     0,     0,     0,   357,     0,
      29,   358,     0,     0,     0,     0,    32,   860,     0,     0,
       0,     0,     0,    37,     0,     0,  -348,   874,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   310,   -68,     0,
       0,     0,     0,     0,     0,     0,  -348,     0,     0,    53,
       0,    55,     0,     0,     0,    59,  -348,   -68,    60,   425,
     106,   359,     0,    64,     0,   485,   426,     0,     0,     0,
       0,     0,     0,     0,   106,     0,     0,     0,   427,   849,
     888,     0,    80,     0,   106,    82,     0,   893,    84,     0,
       0,     0,     0,     0,   850,     0,     0,   592,     0,     0,
       0,     0,     0,     0,   499,   502,     0,     0,     0,     0,
       0,   508,     0,     0,     0,     0,     0,     0,     0,     0,
     271,     0,   271,     0,     0,     0,     0,     0,     0,   271,
       0,     0,     0,     0,     0,     0,   428,     0,    99,     0,
     429,   752,   318,   360,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   318,     0,
     318,   934,   935,   425,   814,     0,     0,   846,     0,   271,
     426,   430,     0,     0,     0,     0,     0,     0,     0,     0,
     318,     0,   427,     0,     0,     0,     0,   815,   431,   432,
       0,   433,   434,   435,   310,   436,   437,   438,   963,   439,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,     0,   749,   451,   452,   453,     0,   360,     0,     0,
       0,     0,   579,   816,   454,     0,  1002,     0,  1004,   830,
     830,     0,   338,     0,   339,     0,     0,   341,   342,     0,
     428,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,  1016,     0,  1018,     0,   225,     0,  1022,  1024,     0,
       0,     0,     0,  1028,  1030,     0,     0,     0,     0,  1034,
       0,     0,     0,     0,     0,   430,     0,  1039,   743,  1042,
       0,  1044,  1046,     0,     0,   318,     0,     0,     0,     0,
     106,     0,   431,   432,     0,   433,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,     0,     0,   451,   452,   453,
    1022,  1072,  1028,  1075,  1077,  1042,   318,     0,   454,   740,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   682,   724,     0,     0,     0,     0,     0,
       0,     0,     0,  1085,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     310,     0,     0,     0,  1100,     0,     0,     0,     0,     0,
     225,     0,     0,     0,     0,     0,     0,     0,  1108,  1110,
    1112,     0,   318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   830,     0,     0,     0,     0,     0,     0,
       0,  1123,     0,  1124,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1132,     0,  1134,     0,     0,     0,   740,
       0,  1141,   271,   271,   271,   271,   271,   271,   271,   271,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   937,     0,     0,   940,     0,     0,
       0,     0,     0,     0,  1171,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1176,     0,     0,  1179,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   318,   318,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1195,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1200,
     830,   830,   830,   830,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1212,
       0,  1214,     0,     0,     0,     0,  1219,   520,     0,  1222,
       0,     0,     0,   520,     0,     0,     0,  1228,     0,  1232,
       0,     0,  1234,     0,     0,  1036,     0,     0,     0,  1041,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1212,  1214,     0,  1219,  1222,  1228,  1232,     0,     0,
       0,   942,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1271,
       0,     0,     0,     0,  1272,  1273,     0,  1274,     0,   740,
       0,  1276,  1277,     0,     0,  1278,     0,   971,     0,     0,
    1279,     0,     0,     0,  1281,   271,   271,     0,     0,     0,
    1083,     0,  1084,  1284,     0,  1086,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1272,  1294,
    1295,  1276,  1296,  1297,  1298,  1299,     0,     0,     0,  1101,
       0,     0,     0,     0,     0,   271,     0,     0,     0,     0,
       0,  1041,     0,     0,     0,   271,     0,     0,     0,   271,
       0,   271,     0,     0,   271,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1314,  1315,  1316,  1317,
    1318,  1319,  1314,  1315,  1316,  1317,  1318,  1319,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   194,     0,     0,
      21,  1152,     0,     0,     0,     0,     0,  1157,    27,     0,
     195,     0,     0,     0,    31,  -212,  -212,     0,     0,   196,
      34,     0,     0,     0,    36,     0,     0,    38,     0,     0,
     197,     0,     0,    42,    43,     0,     0,    45,     0,    47,
       0,    48,    49,  1091,    50,    51,     0,     0,     0,     0,
       0,  1187,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   198,     0,     0,     0,     0,     0,     0,
    1106,    69,    70,    71,    72,    73,   199,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,   520,
       0,   520,     0,     0,     0,     0,   520,     0,     0,   520,
       0,     0,     0,     0,     0,     0,    89,  1227,    91,  1231,
     271,   271,     0,     0,     0,     0,   271,   271,     0,     0,
     271,   271,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,   200,     0,   102,     0,  -212,
    1262,     0,     0,     0,     0,     0,  1265,     0,     0,     0,
    1227,  1231,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   691,     0,     0,     0,     0,     0,     0,
       0,     0,   629,     0,     0,     0,     0,   982,   983,   631,
     984,     0,     0,  1280,     0,   985,     0,  1282,     0,     0,
     271,     0,     0,   693,   271,   271,   271,   271,     0,     0,
       0,     0,   106,     0,     0,   986,     0,     0,     0,   987,
       0,     0,   988,     0,   636,     0,   271,   271,     0,   271,
       0,   502,   694,     0,     0,     0,     0,     0,   502,     0,
     989,   271,     0,     0,   271,     0,   271,   990,   271,     0,
       0,     0,     0,     0,     0,     0,     0,   991,     0,     0,
       0,     0,     0,     0,     0,     0,   992,   993,     0,   994,
       0,   995,     0,   695,     0,     0,     0,     0,     0,   696,
     996,     0,     0,   997,     0,     0,   998,     0,     0,     0,
       0,  1266,     0,     0,     0,   501,   456,   457,   458,   459,
     460,     0,     0,   463,   464,   465,   466,     0,   468,   469,
       0,   644,   645,   646,   647,   648,   649,     0,   650,     0,
       0,     0,   651,   652,   653,   654,   655,   656,   657,   658,
     659,   660,     0,   661,     0,     0,   662,   663,   664,   665,
     999,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     502,   502,     0,     0,   502,   502,   502,     0,   502,    -2,
       4,     0,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -212,    21,    22,    23,    24,    25,    26,
       0,  -212,    27,     0,    28,     0,    29,    30,    31,  -212,
    -212,  -212,    32,    33,    34,   -68,     0,    35,    36,    37,
       0,    38,  -348,     0,    39,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -68,    48,    49,     0,    50,    51,
      52,     0,  -348,     0,     0,    53,    54,    55,    56,    57,
      58,    59,  -348,   -68,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
       0,    82,    83,   -68,    84,    85,     0,     0,  -212,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,    99,     0,     0,   100,     0,   101,
       0,   102,     0,  -212,     0,     4,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,  -696,     0,    13,    14,
      15,    16,    17,  -696,    18,     0,    19,    20,  -212,    21,
      22,    23,    24,    25,    26,  -696,  -212,    27,  -696,    28,
       0,    29,    30,    31,  -212,  -212,  -212,    32,    33,    34,
     -68,     0,    35,    36,    37,     0,    38,  -348,     0,    39,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -68,
      48,    49,     0,    50,    51,    52,     0,  -348,     0,     0,
      53,    54,    55,    56,     0,    58,    59,  -348,   -68,    60,
      61,    62,    63,  -696,    64,    65,    66,  -696,    67,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,    78,    79,    80,    81,     0,    82,    83,   -68,    84,
      85,     0,     0,  -212,     0,    86,     0,     0,  -696,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,  -696,    91,  -696,  -696,
    -696,  -696,  -696,  -696,  -696,     0,  -696,  -696,  -696,  -696,
    -696,  -696,  -696,  -696,  -696,  -696,  -696,  -696,    98,    99,
    -696,  -696,  -696,     0,   101,  -696,   102,     0,  -212,     0,
     343,  -696,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -212,    21,    22,    23,    24,    25,    26,
       0,  -212,    27,     0,    28,     0,    29,    30,    31,  -212,
    -212,  -212,    32,    33,    34,   -68,     0,    35,    36,    37,
       0,    38,  -348,     0,    39,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -68,    48,    49,     0,    50,    51,
      52,     0,  -348,     0,     0,    53,    54,    55,    56,    57,
      58,    59,  -348,   -68,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
       0,    82,    83,   -68,    84,    85,     0,     0,  -212,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,    99,     0,     0,   100,     0,   101,
     344,   102,     0,  -212,     0,     4,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -212,    21,
      22,    23,    24,    25,    26,     0,  -212,    27,     0,    28,
       0,    29,    30,    31,  -212,  -212,  -212,    32,    33,    34,
     -68,     0,    35,    36,    37,     0,    38,  -348,     0,    39,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -68,
      48,    49,     0,    50,    51,    52,     0,  -348,     0,     0,
      53,    54,    55,    56,    57,    58,    59,  -348,   -68,    60,
      61,    62,    63,     0,    64,    65,    66,     0,    67,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,    78,    79,    80,    81,     0,    82,    83,   -68,    84,
      85,     0,     0,  -212,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,    99,
       0,     0,   100,     0,   101,   557,   102,     0,  -212,     0,
     574,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -212,    21,    22,    23,    24,    25,    26,
       0,  -212,    27,     0,    28,     0,    29,    30,    31,  -212,
    -212,  -212,    32,    33,    34,   -68,     0,    35,    36,    37,
       0,    38,  -348,     0,    39,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -68,    48,    49,     0,    50,    51,
      52,     0,  -348,     0,     0,    53,    54,    55,    56,    57,
      58,    59,  -348,   -68,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
       0,    82,    83,   -68,    84,    85,     0,     0,  -212,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,    99,     0,     0,   100,     0,   101,
     575,   102,     0,  -212,     0,     4,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -212,    21,
      22,    23,    24,    25,    26,     0,  -212,    27,     0,    28,
       0,    29,    30,    31,  -212,  -212,  -212,    32,    33,    34,
     -68,     0,    35,    36,    37,     0,    38,  -348,     0,    39,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -68,
      48,    49,     0,    50,    51,    52,     0,  -348,     0,     0,
      53,    54,    55,    56,    57,    58,    59,  -348,   -68,    60,
      61,    62,    63,     0,    64,    65,    66,     0,    67,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,    78,    79,    80,    81,     0,    82,    83,   -68,    84,
      85,     0,     0,  -212,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,    99,
       0,     0,   100,     0,   101,   807,   102,     0,  -212,     0,
       4,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -212,    21,    22,    23,    24,    25,    26,
       0,  -212,    27,     0,    28,     0,    29,    30,    31,  -212,
    -212,  -212,    32,    33,    34,   -68,     0,    35,    36,    37,
       0,    38,  -348,     0,    39,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -68,    48,    49,     0,    50,    51,
      52,     0,  -348,     0,     0,    53,    54,    55,    56,   354,
      58,    59,  -348,   -68,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
       0,    82,    83,   -68,    84,    85,     0,     0,  -212,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,    99,     0,     0,   100,     0,   101,
       0,   102,     0,  -212,     0,     4,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -212,    21,
      22,    23,    24,    25,    26,     0,  -212,    27,     0,    28,
       0,    29,    30,    31,  -212,  -212,  -212,    32,    33,    34,
     -68,     0,    35,    36,    37,     0,    38,  -348,     0,    39,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -68,
      48,    49,     0,    50,    51,    52,     0,  -348,     0,     0,
      53,    54,    55,    56,    57,    58,    59,  -348,   -68,    60,
      61,    62,    63,     0,    64,    65,    66,     0,    67,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,    78,    79,    80,    81,     0,    82,    83,   -68,    84,
      85,     0,     0,  -212,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,    99,
       0,     0,   100,     0,   101,     0,   102,     0,  -212,     0,
       4,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -212,    21,    22,    23,    24,    25,    26,
       0,  -212,    27,     0,    28,     0,    29,    30,    31,  -212,
    -212,  -212,    32,    33,    34,   -68,     0,    35,    36,    37,
       0,    38,  -348,     0,    39,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -68,    48,    49,     0,    50,    51,
      52,     0,  -348,     0,     0,    53,    54,    55,    56,     0,
      58,    59,  -348,   -68,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
       0,    82,    83,   -68,    84,    85,     0,     0,  -212,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,    99,     0,     0,   100,     0,   101,
       0,   102,     0,  -212,     0,     4,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -212,    21,
      22,    23,    24,    25,    26,     0,  -212,    27,     0,    28,
       0,    29,    30,    31,  -212,  -212,  -212,    32,  1021,    34,
     -68,     0,    35,    36,    37,     0,    38,  -348,     0,    39,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -68,
      48,    49,     0,    50,    51,    52,     0,  -348,     0,     0,
      53,    54,    55,    56,     0,    58,    59,  -348,   -68,    60,
      61,    62,    63,     0,    64,    65,    66,     0,    67,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,    78,    79,    80,    81,     0,    82,    83,   -68,    84,
      85,     0,     0,  -212,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,    99,
       0,     0,   100,     0,   101,     0,   102,     0,  -212,     0,
       4,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -212,    21,    22,    23,    24,    25,    26,
       0,  -212,    27,     0,    28,     0,    29,    30,    31,  -212,
    -212,  -212,    32,  1027,    34,   -68,     0,    35,    36,    37,
       0,    38,  -348,     0,    39,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -68,    48,    49,     0,    50,    51,
      52,     0,  -348,     0,     0,    53,    54,    55,    56,     0,
      58,    59,  -348,   -68,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
       0,    82,    83,   -68,    84,    85,     0,     0,  -212,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,    99,     0,     0,   100,     0,   101,
       0,   102,     0,  -212,     0,     4,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -212,    21,
      22,    23,    24,    25,    26,     0,  -212,    27,     0,    28,
       0,    29,    30,    31,  -212,  -212,  -212,    32,  1211,    34,
     -68,     0,    35,    36,    37,     0,    38,  -348,     0,    39,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -68,
      48,    49,     0,    50,    51,    52,     0,  -348,     0,     0,
      53,    54,    55,    56,     0,    58,    59,  -348,   -68,    60,
      61,    62,    63,     0,    64,    65,    66,     0,    67,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,    78,    79,    80,    81,     0,    82,    83,   -68,    84,
      85,     0,     0,  -212,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,    99,
       0,     0,   100,     0,   101,     0,   102,     0,  -212,     0,
       4,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -212,    21,    22,    23,    24,    25,    26,
       0,  -212,    27,     0,    28,     0,    29,    30,    31,  -212,
    -212,  -212,    32,  1213,    34,   -68,     0,    35,    36,    37,
       0,    38,  -348,     0,    39,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -68,    48,    49,     0,    50,    51,
      52,     0,  -348,     0,     0,    53,    54,    55,    56,     0,
      58,    59,  -348,   -68,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
       0,    82,    83,   -68,    84,    85,     0,     0,  -212,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,    99,     0,     0,   100,     0,   101,
       0,   102,     0,  -212,     0,     4,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -212,    21,
      22,    23,    24,    25,    26,     0,  -212,    27,     0,    28,
       0,    29,    30,    31,  -212,  -212,  -212,    32,  1218,    34,
     -68,     0,    35,    36,    37,     0,    38,  -348,     0,    39,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -68,
      48,    49,     0,    50,    51,    52,     0,  -348,     0,     0,
      53,    54,    55,    56,     0,    58,    59,  -348,   -68,    60,
      61,    62,    63,     0,    64,    65,    66,     0,    67,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,    78,    79,    80,    81,     0,    82,    83,   -68,    84,
      85,     0,     0,  -212,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,    99,
       0,     0,   100,     0,   101,     0,   102,     0,  -212,     0,
       4,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -212,    21,    22,    23,    24,    25,    26,
       0,  -212,    27,     0,    28,     0,    29,    30,    31,  -212,
    -212,  -212,    32,  1221,    34,   -68,     0,    35,    36,    37,
       0,    38,  -348,     0,    39,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -68,    48,    49,     0,    50,    51,
      52,     0,  -348,     0,     0,    53,    54,    55,    56,     0,
      58,    59,  -348,   -68,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
       0,    82,    83,   -68,    84,    85,     0,     0,  -212,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,    99,     0,     0,   100,     0,   101,
       0,   102,     0,  -212,     0,     4,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -212,    21,
      22,    23,    24,    25,    26,     0,  -212,    27,     0,    28,
       0,    29,    30,    31,  -212,  -212,  -212,    32,  1226,    34,
     -68,     0,    35,    36,    37,     0,    38,  -348,     0,    39,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -68,
      48,    49,     0,    50,    51,    52,     0,  -348,     0,     0,
      53,    54,    55,    56,     0,    58,    59,  -348,   -68,    60,
      61,    62,    63,     0,    64,    65,    66,     0,    67,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,    78,    79,    80,    81,     0,    82,    83,   -68,    84,
      85,     0,     0,  -212,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,    99,
       0,     0,   100,     0,   101,     0,   102,     0,  -212,     0,
       4,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -212,    21,    22,    23,    24,    25,    26,
       0,  -212,    27,     0,    28,     0,    29,    30,    31,  -212,
    -212,  -212,    32,  1230,    34,   -68,     0,    35,    36,    37,
       0,    38,  -348,     0,    39,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -68,    48,    49,     0,    50,    51,
      52,     0,  -348,     0,     0,    53,    54,    55,    56,     0,
      58,    59,  -348,   -68,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
       0,    82,    83,   -68,    84,    85,     0,     0,  -212,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,    99,     0,     0,   100,     0,   101,
       0,   102,     0,  -212,     0,   880,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,   290,     0,   195,
       0,     0,     0,    31,  -212,  -212,     0,     0,   196,    34,
       0,     0,     0,   292,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,     0,     0,     0,     0,     0,     0,
      69,   297,    71,    72,    73,   298,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   200,     0,   102,     0,  -212,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,   629,     0,    13,     0,     0,    16,    17,   631,    18,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   195,     0,     0,     0,    31,     0,
       0,     0,     0,   196,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   636,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     199,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,   501,   456,   457,   458,   459,   460,
       0,     0,   463,   464,   465,   466,     0,   468,   469,     0,
     818,   819,   820,   821,   822,   649,     0,   650,     0,    94,
       0,   651,   652,   653,   654,   655,   656,   657,   823,   659,
     660,    96,   824,    98,   -92,   662,   663,   825,   665,   200,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,   629,     0,    13,     0,
       0,    16,    17,   631,    18,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,   196,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   636,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,   501,
     456,   457,   458,   459,   460,     0,     0,   463,   464,   465,
     466,     0,   468,   469,     0,   818,   819,   820,   821,   822,
     649,     0,   650,     0,    94,     0,   651,   652,   653,   654,
     655,   656,   657,   823,   659,   660,    96,   824,    98,     0,
     662,   663,   825,   665,   200,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,     0,    16,    17,     0,    18,
       0,   194,    20,     0,    21,     0,     0,     0,     0,    26,
       0,     0,    27,     0,   195,     0,     0,     0,    31,     0,
       0,     0,     0,    33,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
      44,    45,    46,    47,     0,    48,    49,     0,    50,    51,
      52,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,     0,     0,
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
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
     375,    23,     0,     0,     0,     0,     0,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,   196,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,    55,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,    64,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,    84,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   200,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   195,     0,     0,     0,    31,     0,
       0,     0,     0,   196,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,  -740,     0,
       0,     0,  -740,     0,     0,    69,    70,    71,    72,    73,
     199,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,  -390,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,  -390,     0,     0,   100,     0,   200,
    -390,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,   196,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,   312,   313,     0,    81,   348,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,   349,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   200,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,   378,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,    19,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   195,     0,     0,     0,    31,  -212,
    -212,     0,     0,   196,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,    63,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     199,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,    82,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   200,
       0,   102,     0,  -212,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
       0,   733,     0,     0,     0,     0,     0,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,   196,    34,
       0,   734,     0,    36,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,   735,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,   736,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   200,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   195,     0,     0,     0,    31,     0,
       0,     0,     0,   196,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     199,     0,    75,     0,     0,    77,   312,   313,     0,    81,
     348,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   200,
       0,   102,   794,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,   196,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,   312,   313,     0,    81,   348,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   200,     0,   102,   796,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   195,     0,     0,     0,    31,     0,
       0,     0,     0,   196,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     199,     0,    75,     0,     0,    77,   312,   313,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,   314,     0,     0,   100,     0,   200,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,   196,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,   312,   313,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   200,     0,   102,  -541,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   195,     0,     0,     0,    31,  -212,
    -212,     0,     0,   196,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     199,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   200,
       0,   102,     0,  -212,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,   196,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,   312,   313,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   200,     0,   102,     0,     0,  -541,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   195,     0,     0,     0,    31,     0,
       0,     0,     0,   196,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     199,     0,    75,     0,     0,    77,   312,   313,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   200,
       0,   102,   802,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,   196,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,   950,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,   312,   313,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   200,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   195,     0,     0,     0,    31,     0,
       0,     0,     0,   196,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     199,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,  -398,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,  -398,     0,     0,   100,     0,   200,
    -398,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,   196,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,  -383,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,  -383,
       0,     0,   100,     0,   200,  -383,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   195,     0,     0,     0,    31,     0,
       0,     0,     0,   196,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     199,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,  -391,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,  -391,     0,     0,   100,     0,   200,
    -391,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
       0,   255,     0,     0,     0,     0,     0,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,   196,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,   256,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   200,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   195,     0,     0,     0,    31,     0,
       0,     0,     0,   196,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     199,     0,    75,     0,     0,    77,   312,   313,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   200,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,   196,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,  -740,     0,     0,     0,  -740,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   200,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   195,     0,     0,     0,    31,     0,
       0,     0,     0,   196,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,   593,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     199,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,   594,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   200,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,   196,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,   594,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   200,     0,   102,     0,     0,   603,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   195,     0,     0,     0,    31,     0,
       0,     0,     0,   196,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     199,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,  -419,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   200,
       0,   102,  -419,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,   270,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,   196,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   101,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   195,     0,     0,     0,    31,     0,
       0,     0,     0,   196,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     199,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,   594,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   200,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,   196,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   200,     0,   102,     0,     0,   603,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   195,     0,     0,     0,    31,     0,
       0,     0,     0,   196,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     199,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   200,
     792,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,   196,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,     0,     0,     0,    81,  1003,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   200,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   195,     0,     0,     0,    31,     0,
       0,     0,     0,   196,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     199,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   200,
       0,   102,     0,     0,  1040,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,   196,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   200,     0,   102,     0,     0,  1045,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   195,     0,     0,     0,    31,     0,
       0,     0,     0,   196,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     199,     0,    75,     0,     0,    77,     0,     0,     0,    81,
    1199,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   200,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,   196,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   200,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,   290,     0,   195,     0,     0,     0,    31,     0,
       0,     0,     0,   196,    34,     0,     0,     0,   292,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,     0,     0,
       0,     0,     0,     0,     0,    69,   297,    71,    72,    73,
     298,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   200,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,  1070,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   200,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   195,     0,     0,     0,    31,     0,
       0,     0,     0,  1073,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     199,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   200,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,  1255,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   200,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   195,     0,     0,     0,    31,     0,
       0,     0,     0,  1256,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     199,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   200,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,  1258,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   200,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   195,     0,     0,     0,    31,     0,
       0,     0,     0,  1259,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     199,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   200,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   195,
       0,     0,     0,    31,     0,     0,     0,     0,  1260,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   197,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   199,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   200,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   195,     0,     0,     0,    31,     0,
       0,     0,     0,  1261,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   197,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   198,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     199,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       6,     7,     8,     9,    10,    11,    12,     0,    95,     0,
      13,    96,    97,    98,    17,     0,     0,   100,   194,   200,
       0,   102,     0,   289,     0,     0,   103,     0,     0,     0,
       0,   195,     0,     0,     0,     0,  -212,  -212,     0,     0,
     196,     0,     0,   291,     0,     0,     0,   293,     0,     0,
       0,   197,     0,     0,    42,    43,     0,     0,     0,     0,
      47,     0,    48,    49,     0,     0,     0,     0,     0,     0,
       0,   294,   427,    54,   295,    56,    57,    58,     0,     0,
       0,     0,    61,     0,   198,     0,   296,     0,     0,     0,
       0,     0,    69,     0,    71,     0,    73,     0,     0,     0,
       0,     0,     0,     0,     0,   299,     0,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     428,     0,     0,     0,   429,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,   430,   200,     0,   102,  -352,
    -212,     0,     0,   103,     6,     7,     8,     9,    10,    11,
      12,     0,   431,   432,    13,   433,   434,   435,    17,   436,
     437,   438,   194,   439,   440,   441,   442,   289,   444,   445,
     446,   447,   448,     0,   450,   195,     0,   451,   452,   453,
    -212,  -212,     0,     0,   196,     0,     0,   291,   454,     0,
       0,   293,     0,     0,     0,   197,     0,     0,    42,    43,
       0,     0,     0,     0,    47,     0,    48,    49,     0,     0,
       0,     0,     0,     0,     0,   294,     0,    54,   295,    56,
      57,    58,     0,     0,     0,     0,    61,     0,   198,     0,
     296,     0,     0,     0,     0,     0,    69,     0,    71,     0,
      73,     0,     0,     0,     0,     0,     0,     0,     0,   299,
       0,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   627,     0,   628,
       0,    89,    90,    91,    92,    93,     0,     0,   629,     0,
      94,     0,     0,   630,   229,   631,   632,     0,     0,    95,
       0,   633,    96,    97,    98,     0,     0,     0,   100,   232,
     200,   195,   102,     0,  -212,   233,     0,   103,     0,     0,
       0,   634,     0,     0,     0,   235,     0,     0,   635,     0,
     636,     0,     0,     0,     0,     0,     0,     0,   637,     0,
       0,     0,     0,     0,     0,   238,   638,     0,     0,     0,
       0,     0,     0,   240,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   639,     0,     0,     0,     0,     0,     0,
       0,     0,   242,   243,     0,   640,     0,   245,     0,   641,
       0,     0,   247,     0,     0,     0,   642,     0,     0,   249,
       0,     0,   643,     0,     0,     0,     0,     0,     0,     0,
       0,   501,   456,   457,   458,   459,   460,     0,     0,   463,
     464,   465,   466,     0,   468,   469,     0,   644,   645,   646,
     647,   648,   649,     0,   650,     0,     0,     0,   651,   652,
     653,   654,   655,   656,   657,   658,   659,   660,     0,   661,
       0,     0,   662,   663,   664,   665,     0,     0,   666,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   194,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   195,     0,     0,     0,    31,  -212,  -212,     0,     0,
     196,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   197,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   198,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   199,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,     0,    97,
      98,     0,     0,     0,   100,     0,   200,     0,   102,     0,
    -212,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   195,     0,     0,     0,    31,  -212,  -212,
       0,     0,   196,    34,     0,     0,     0,    36,     0,     0,
      38,     0,     0,   197,     0,     0,    42,    43,     0,     0,
      45,     0,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,    62,   198,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,    73,   199,
       0,    75,     0,     0,    77,     0,     0,   227,    81,     0,
       0,    83,     0,     0,    85,     0,   629,     0,     0,     0,
       0,   228,   229,   631,   230,     0,     0,     0,     0,   231,
       0,     0,     0,     0,     0,     0,     0,   232,     0,    89,
       0,    91,     0,   233,     0,     0,     0,     0,     0,   234,
       0,     0,     0,   235,     0,     0,   236,     0,   636,     0,
       0,     0,    98,     0,     0,     0,   237,     0,     0,     0,
     102,     0,  -212,   238,   239,     0,     0,     0,     0,     0,
       0,   240,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   241,     0,     0,     0,     0,     0,   425,     0,     0,
     242,   243,     0,   244,   426,   245,     0,   246,     0,     0,
     247,     0,     0,     0,   248,     0,   427,   249,     0,     0,
     250,     0,     0,     0,     0,     0,     0,     0,     0,   501,
     456,   457,   458,   459,   460,     0,     0,   463,   464,   465,
     466,     0,   468,   469,     0,   644,   645,   646,   647,   648,
     649,     0,   650,     0,     0,     0,   651,   652,   653,   654,
     655,   656,   657,   658,   659,   660,     0,   661,     0,     0,
     662,   663,   664,   665,   428,     0,     0,   425,   429,     0,
       0,     0,     0,     0,   426,     0,     0,   500,     0,     0,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
     501,   456,   457,   458,   459,   460,     0,     0,   463,   464,
     465,   466,     0,   468,   469,     0,   431,   432,     0,   433,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,     0,
       0,   451,   452,   453,   428,   360,     0,   425,   429,     0,
       0,     0,   454,     0,   426,     0,     0,  1210,     0,     0,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
     501,   456,   457,   458,   459,   460,     0,     0,   463,   464,
     465,   466,     0,   468,   469,     0,   431,   432,     0,   433,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,     0,
       0,   451,   452,   453,   428,   360,     0,   425,   429,     0,
       0,     0,   454,     0,   426,     0,     0,  1217,     0,     0,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
     501,   456,   457,   458,   459,   460,     0,     0,   463,   464,
     465,   466,     0,   468,   469,     0,   431,   432,     0,   433,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,     0,
       0,   451,   452,   453,   428,   360,     0,   425,   429,     0,
       0,     0,   454,     0,   426,     0,     0,  1302,     0,     0,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
     501,   456,   457,   458,   459,   460,     0,     0,   463,   464,
     465,   466,     0,   468,   469,     0,   431,   432,     0,   433,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,     0,
       0,   451,   452,   453,   428,   360,     0,   425,   429,     0,
       0,     0,   454,     0,   426,     0,     0,  1303,     0,     0,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
     501,   456,   457,   458,   459,   460,     0,     0,   463,   464,
     465,   466,     0,   468,   469,     0,   431,   432,     0,   433,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,     0,
       0,   451,   452,   453,   428,   360,     0,   425,   429,     0,
       0,     0,   454,     0,   426,     0,     0,  1304,     0,     0,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
     501,   456,   457,   458,   459,   460,     0,     0,   463,   464,
     465,   466,     0,   468,   469,     0,   431,   432,     0,   433,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,     0,
       0,   451,   452,   453,   428,   360,     0,   425,   429,     0,
       0,     0,   454,     0,   426,     0,     0,  1305,     0,     0,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
     501,   456,   457,   458,   459,   460,     0,     0,   463,   464,
     465,   466,     0,   468,   469,     0,   431,   432,     0,   433,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,     0,
       0,   451,   452,   453,   428,   360,     0,   425,   429,     0,
       0,     0,   454,     0,   426,     0,     0,  1306,     0,     0,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
     501,   456,   457,   458,   459,   460,     0,     0,   463,   464,
     465,   466,     0,   468,   469,     0,   431,   432,     0,   433,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,     0,
       0,   451,   452,   453,   428,   360,     0,     0,   429,     0,
       0,     0,   454,     0,     0,     0,     0,  1307,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   425,     0,     0,
       0,     0,     0,     0,   426,     0,     0,     0,     0,   430,
     501,   456,   457,   458,   459,   460,   427,   270,   463,   464,
     465,   466,     0,   468,   469,     0,   431,   432,     0,   433,
     434,   435,   855,   436,   437,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,     0,
       0,   451,   452,   453,     0,   360,     0,     0,     0,     0,
       0,     0,   454,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   425,   428,     0,     0,     0,   429,     0,
     426,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   427,   270,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   846,     0,     0,   975,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   431,   432,     0,   433,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   425,
     428,   451,   452,   453,   429,   360,   426,     0,     0,     0,
       0,     0,   454,     0,     0,     0,     0,     0,   427,   843,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   846,     0,     0,   844,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   431,   432,     0,   433,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   425,   428,   451,   452,   453,
     429,   360,   426,     0,     0,     0,     0,     0,   454,     0,
       0,     0,     0,     0,   427,   270,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     974,   430,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   431,   432,
       0,   433,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   425,   428,   451,   452,   453,   429,   360,   426,     0,
       0,     0,     0,     0,   454,     0,     0,     0,     0,     0,
     427,  1145,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   431,   432,     0,   433,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   425,   428,   451,
     452,   453,   429,   360,   426,     0,     0,     0,     0,     0,
     454,     0,     0,     0,     0,     0,   427,   270,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   846,
       0,     0,     0,   430,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,   433,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   425,   428,   451,   452,   453,   429,   360,
     426,     0,     0,     0,     0,     0,   454,     0,     0,     0,
       0,     0,   427,   270,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   846,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   431,   432,     0,   433,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   425,
     428,   451,   452,   453,   429,   360,   426,     0,     0,     0,
       0,     0,   454,     0,     0,     0,     0,     0,   427,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   837,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   431,   432,     0,   433,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   425,   428,   451,   452,   453,
     429,   360,   426,     0,     0,     0,     0,     0,   454,     0,
       0,     0,     0,     0,   427,   906,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   212,     0,     0,
     907,   430,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   431,   432,
       0,   433,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   425,   428,   451,   452,   453,   429,     0,   426,     0,
       0,     0,     0,     0,   454,     0,     0,     0,     0,     0,
     427,   908,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   909,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   431,   432,     0,   433,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,     0,   428,   451,
     452,   453,   429,     0,     0,     0,     0,     0,     0,     0,
     454,     0,     0,   425,     0,     0,     0,     0,     0,     0,
     426,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   427,   430,     0,     0,     0,  1118,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,   433,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,  1119,     0,   451,   452,   453,     0,     0,
       0,     0,     0,     0,     0,     0,   454,     0,     0,   425,
     428,     0,     0,     0,   429,     0,   426,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   427,  1137,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   431,   432,     0,   433,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   425,   428,   451,   452,   453,
     429,     0,   426,     0,     0,     0,     0,     0,   454,     0,
       0,     0,     0,     0,   427,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   430,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   431,   432,
       0,   433,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   425,   428,   451,   452,   453,   429,   360,   426,     0,
       0,     0,     0,     0,   454,     0,     0,     0,     0,     0,
     427,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   846,     0,     0,     0,   430,     0,     0,
       0,   425,   524,     0,     0,     0,     0,     0,   426,     0,
       0,     0,     0,     0,   431,   432,     0,   433,   434,   435,
     427,   436,   437,   438,     0,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,     0,   428,   451,
     452,   453,   429,     0,     0,     0,   425,     0,  1155,     0,
     454,     0,     0,   426,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   427,     0,     0,     0,   846,
       0,     0,     0,   430,     0,     0,     0,     0,   428,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,   433,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   430,     0,   451,   452,   453,     0,     0,
       0,     0,     0,   428,  1197,     0,   454,   429,     0,     0,
     431,   432,     0,   433,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   425,     0,   451,   452,   453,   430,     0,
     426,     0,     0,     0,     0,     0,   454,     0,     0,     0,
       0,     0,   427,     0,     0,   431,   432,     0,   433,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   425,     0,
     451,   452,   453,     0,   548,   426,     0,     0,     0,     0,
       0,   454,     0,     0,     0,     0,     0,   427,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     428,     0,     0,     0,   429,     0,     0,     0,   425,     0,
       0,     0,     0,     0,     0,   426,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   427,     0,     0,
       0,     0,     0,     0,     0,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   428,     0,     0,     0,   429,
       0,     0,   431,   432,     0,   433,   434,   435,   727,   436,
     437,   438,     0,   439,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,     0,   552,   451,   452,   453,
     430,     0,     0,     0,     0,   428,     0,     0,   454,   429,
       0,     0,     0,     0,     0,     0,     0,   431,   432,     0,
     433,   434,   435,     0,   436,   437,   438,     0,   439,   440,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     430,     0,   451,   452,   453,     0,     0,     0,   425,   810,
       0,     0,     0,   454,     0,   426,     0,   431,   432,     0,
     433,   434,   435,     0,   436,   437,   438,   427,   439,   440,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
       0,     0,   451,   452,   453,     0,     0,     0,     0,   793,
       0,     0,     0,   454,   425,     0,     0,     0,     0,     0,
       0,   426,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   427,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   428,     0,     0,     0,   429,
       0,     0,     0,     0,   425,     0,     0,     0,     0,     0,
       0,   426,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   427,     0,     0,   939,     0,     0,     0,
     430,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   428,     0,     0,     0,   429,     0,   431,   432,     0,
     433,   434,   435,     0,   436,   811,   438,     0,   439,   440,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
       0,     0,   451,   452,   453,   867,   430,     0,     0,     0,
     425,   428,     0,   454,     0,   429,     0,   426,     0,     0,
       0,     0,     0,   431,   432,     0,   433,   434,   435,   427,
     436,   437,   438,     0,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   430,     0,   451,   452,
     453,     0,     0,     0,     0,     0,     0,     0,     0,   454,
       0,     0,     0,   431,   432,     0,   433,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   425,   428,   451,   452,
     453,   429,     0,   426,     0,     0,     0,     0,     0,   454,
       0,     0,     0,     0,     0,   427,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,   425,  1009,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   431,
     432,     0,   433,   434,   435,   427,   436,   437,   438,     0,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,     0,   428,   451,   452,   453,   429,     0,     0,
       0,   953,   425,     0,     0,   454,     0,     0,     0,   426,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   427,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,     0,   428,     0,     0,     0,   429,     0,     0,
       0,     0,     0,     0,     0,   431,   432,     0,   433,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   430,     0,
     451,   452,   453,     0,     0,     0,     0,   954,     0,   428,
       0,   454,     0,   429,     0,   431,   432,     0,   433,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   425,  1115,
     451,   452,   453,     0,   430,   426,     0,     0,     0,     0,
       0,   454,     0,     0,     0,     0,     0,   427,     0,     0,
       0,   431,   432,     0,   433,   434,   435,     0,   436,   437,
     438,     0,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,     0,     0,   451,   452,   453,     0,
       0,     0,   425,  1064,     0,     0,     0,   454,     0,   426,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   427,     0,     0,     0,   428,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   425,     0,     0,     0,     0,     0,     0,   426,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,   427,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   431,   432,   428,
     433,   434,   435,   429,   436,   437,   438,     0,   439,   440,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
       0,     0,   451,   452,   453,     0,     0,     0,     0,     0,
     212,     0,     0,   454,   430,     0,     0,     0,     0,   428,
       0,     0,   425,   429,     0,     0,     0,     0,     0,   426,
       0,   431,   432,     0,   433,   434,   435,     0,   436,   437,
     438,   427,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   430,     0,   451,   452,   453,     0,
       0,     0,     0,     0,     0,     0,     0,   454,     0,     0,
       0,   431,   432,     0,   433,   434,   435,     0,   436,   437,
     438,     0,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,     0,  1131,   451,   452,   453,   428,
       0,     0,   425,   429,     0,     0,     0,   454,     0,   426,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   427,  1178,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1158,   430,     0,     0,     0,     0,     0,
       0,     0,   425,     0,     0,     0,     0,     0,     0,   426,
       0,   431,   432,     0,   433,   434,   435,     0,   436,   437,
     438,   427,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,     0,     0,   451,   452,   453,   428,
       0,     0,   425,   429,     0,     0,     0,   454,     0,   426,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   427,  1181,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,     0,   428,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
       0,   431,   432,     0,   433,   434,   435,     0,   436,   437,
     438,     0,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   430,     0,   451,   452,   453,   428,
       0,     0,   425,   429,     0,     0,     0,   454,     0,   426,
       0,   431,   432,     0,   433,   434,   435,     0,   436,   437,
     438,   427,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   430,     0,   451,   452,   453,     0,
       0,     0,   425,  1205,     0,  1183,     0,   454,     0,   426,
    1247,   431,   432,     0,   433,   434,   435,     0,   436,   437,
     438,   427,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,     0,     0,   451,   452,   453,   428,
       0,     0,     0,   429,     0,     0,     0,   454,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,   425,   428,
       0,     0,     0,   429,     0,   426,     0,     0,     0,     0,
       0,   431,   432,     0,   433,   434,   435,   427,   436,   437,
     438,     0,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   430,     0,   451,   452,   453,     0,
       0,     0,     0,     0,     0,     0,     0,   454,     0,     0,
       0,   431,   432,     0,   433,   434,   435,     0,   436,   437,
     438,     0,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   425,   428,   451,   452,   453,   429,
       0,   426,     0,     0,     0,     0,     0,   454,  1254,     0,
       0,     0,     0,   427,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   431,   432,     0,
     433,   434,   435,     0,   436,   437,   438,     0,   439,   440,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     425,   428,   451,   452,   453,   429,     0,   426,     0,     0,
       0,     0,     0,   454,  1257,     0,     0,     0,     0,   427,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   430,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   431,   432,     0,   433,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   425,   428,   451,   452,
     453,   429,     0,   426,     0,     0,     0,     0,     0,   454,
    1308,     0,     0,     0,     0,   427,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
     432,     0,   433,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   425,   428,   451,   452,   453,   429,     0,   426,
       0,     0,     0,     0,     0,   454,  1309,     0,     0,     0,
       0,   427,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   431,   432,     0,   433,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   425,   428,
     451,   452,   453,   429,     0,   426,     0,     0,     0,     0,
       0,   454,  1310,     0,     0,     0,     0,   427,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   431,   432,     0,   433,   434,   435,     0,   436,   437,
     438,     0,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   425,   428,   451,   452,   453,   429,
       0,   426,     0,     0,     0,     0,     0,   454,  1311,     0,
       0,     0,     0,   427,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   431,   432,     0,
     433,   434,   435,     0,   436,   437,   438,     0,   439,   440,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     425,   428,   451,   452,   453,   429,     0,   426,     0,     0,
       0,     0,     0,   454,  1312,     0,     0,     0,     0,   427,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   430,     0,     0,     0,
     425,     0,     0,     0,     0,     0,     0,   426,     0,     0,
       0,     0,     0,   431,   432,     0,   433,   434,   435,   427,
     436,   437,   438,     0,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,     0,   428,   451,   452,
     453,   429,     0,     0,     0,   425,     0,     0,     0,   454,
    1313,     0,   426,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   427,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,   428,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,   431,
     432,     0,   433,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   430,     0,   451,   452,   453,     0,     0,     0,
       0,     0,   929,     0,     0,   454,   429,     0,     0,   431,
     432,     0,   433,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,     0,     0,   451,   452,   453,   430,     0,     0,
       0,     0,     0,     0,     0,   454,     0,     0,     0,     0,
       0,     0,     0,     0,   431,   432,     0,   433,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,     0,     0,   451,
     452,   453,   396,     0,   227,     0,     0,     0,     0,     0,
     454,     0,     0,     0,     0,     0,     0,     0,   228,   229,
       0,   230,     0,     0,     0,     0,   231,     0,     0,     0,
       0,     0,     0,     0,   232,     0,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,   234,     0,     0,     0,
     235,     0,     0,   236,     0,     0,     0,     0,     0,   227,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
     238,   239,     0,   228,   229,     0,   230,     0,   240,     0,
       0,   231,     0,     0,     0,     0,     0,     0,   241,   232,
       0,     0,     0,     0,     0,   233,     0,   242,   243,     0,
     244,   234,   245,     0,   246,   235,     0,   247,   236,     0,
       0,   248,     0,     0,   249,     0,     0,   250,   237,     0,
       0,     0,     0,     0,     0,   238,   239,     0,     0,     0,
       0,     0,     0,   240,   227,     0,     0,     0,     0,     0,
       0,    88,     0,   241,     0,     0,     0,     0,   228,   229,
       0,   230,   242,   243,     0,   244,   231,   245,    23,   246,
       0,     0,   247,     0,   232,     0,   248,     0,     0,   249,
     233,     0,   250,     0,     0,     0,   234,     0,     0,     0,
     235,     0,     0,   236,     0,     0,     0,     0,     0,   227,
       0,     0,     0,   237,     0,     0,    88,     0,     0,     0,
     238,   239,     0,   228,   229,     0,   230,     0,   240,    55,
       0,   231,     0,     0,     0,     0,     0,     0,   241,   232,
       0,    64,     0,     0,     0,   233,     0,   242,   243,     0,
     244,   234,   245,     0,   246,   235,     0,   247,   236,     0,
       0,   248,     0,     0,   249,     0,    84,   250,   237,     0,
       0,     0,     0,     0,     0,   238,   239,     0,     0,     0,
       0,     0,     0,   240,  -323,     0,     0,     0,     0,     0,
       0,     0,     0,   241,     0,     0,     0,     0,  -323,  -323,
       0,  -323,   242,   243,     0,   244,  -323,   245,     0,   246,
       0,     0,   247,     0,  -323,     0,   248,   537,     0,   249,
    -323,     0,   250,     0,     0,     0,  -323,     0,     0,     0,
    -323,     0,     0,  -323,     0,     0,     0,     0,     0,   227,
       0,     0,     0,  -323,     0,     0,     0,     0,     0,     0,
    -323,  -323,     0,   228,   229,     0,   230,     0,  -323,     0,
       0,   231,     0,     0,     0,     0,     0,     0,  -323,   232,
       0,     0,     0,     0,     0,   233,     0,  -323,  -323,     0,
    -323,   234,  -323,     0,  -323,   235,     0,  -323,   236,     0,
       0,  -323,     0,     0,  -323,     0,     0,  -323,   237,     0,
       0,     0,     0,     0,     0,   238,   239,     0,     0,     0,
       0,     0,     0,   240,  -324,     0,     0,     0,     0,     0,
       0,     0,     0,   241,     0,     0,     0,     0,  -324,  -324,
       0,  -324,   242,   243,     0,   244,  -324,   245,     0,   246,
       0,     0,   247,     0,  -324,     0,   248,     0,     0,   249,
    -324,     0,   250,     0,     0,     0,  -324,     0,     0,     0,
    -324,     0,     0,  -324,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -324,     0,     0,     0,     0,     0,     0,
    -324,  -324,     0,     0,     0,     0,     0,     0,  -324,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -324,     0,
       0,     0,     0,     0,     0,     0,     0,  -324,  -324,     0,
    -324,     0,  -324,     0,  -324,     0,     0,  -324,     0,     0,
       0,  -324,     0,     0,  -324,     0,     0,  -324
};

static const yytype_int16 yycheck[] =
{
       2,    20,     2,    61,   180,   111,   151,   482,   164,     2,
     546,   583,    26,   397,   417,    61,    66,    61,   226,   220,
     550,   623,   669,    25,   671,   915,   262,   506,   782,   620,
     816,   541,    25,   132,   811,   379,    86,   776,   955,     8,
       1,   834,   572,     1,   402,   873,    47,    32,   516,    32,
     286,    65,   102,    32,   262,    66,    32,     1,    32,     1,
      32,    32,    32,   385,   386,   879,   388,    74,    60,     1,
     102,     0,    74,    55,   123,    86,    78,    79,   286,     3,
      55,    74,   258,    35,    47,    78,    79,   120,    32,    47,
     326,   102,  1172,    40,    41,    36,   118,    47,   140,   101,
      32,    46,   147,   105,   165,   147,   428,   429,   101,     3,
     117,   141,   105,    74,    78,    79,    60,   165,   326,   149,
      92,   170,    92,   120,   172,   147,   118,    90,    60,   171,
     175,    72,    47,    24,     3,  1025,     2,   120,   170,    84,
      90,    32,   175,   957,   120,     2,   147,    55,    92,    40,
      41,    42,   474,   475,   511,   623,   117,  1237,   110,    25,
      92,   105,   147,   597,   171,   599,   600,   125,    25,   114,
     604,   605,   763,   105,   118,     3,   147,   535,   175,   103,
     149,   538,  1099,   125,  1012,   170,   118,   170,   146,   147,
     172,   170,    31,   165,   170,   949,   170,   172,   170,   170,
     170,   955,   146,    82,   146,   147,    85,   165,    74,   103,
     171,   213,    78,    79,   146,   226,   746,    74,  1011,   958,
     213,    78,    79,    92,  1052,  1053,   170,   174,   119,   437,
    1007,   173,   147,   165,   103,   101,   472,   473,   170,   105,
     147,   417,   586,   815,   101,   147,   568,   569,   105,   728,
      89,   262,    46,    89,    93,   147,   120,    93,    22,   147,
     175,    47,    55,   265,   472,   473,   173,   120,   270,   171,
     780,    65,   265,   165,    89,   286,    24,   270,    93,   378,
     147,    46,    55,   174,    32,   173,    47,    24,    27,   147,
      84,   397,    40,    41,    42,    32,   147,  1125,   506,    27,
      65,    55,   141,    40,    41,    42,   173,   146,    47,   148,
     149,   175,    51,   171,   153,   326,   147,   109,    90,    84,
     114,   160,   173,   524,    88,  1215,  1080,   166,   167,   168,
     486,  1117,  1160,  1119,   120,     3,    75,   928,   172,    78,
     171,    80,   550,   401,   346,  1099,   346,   213,   112,   114,
      78,    90,    20,   346,   956,   172,   213,   174,   360,   120,
     147,   147,    90,    53,   572,    33,   800,   360,   370,    18,
     109,   119,    50,    27,   388,    53,   147,   370,   165,   393,
      11,   383,   119,    73,   147,    55,   147,   115,   589,   175,
     383,    27,   871,    83,    62,    73,   147,   147,    76,   265,
     287,   288,   173,   147,   270,    83,  1166,   147,   265,    31,
     173,   461,   462,   270,   175,   417,    55,   467,  1065,  1066,
     470,   471,   173,   173,    78,   165,   174,   999,    77,   173,
     172,   109,   125,   912,   173,   103,    90,   174,     8,    75,
     915,   109,    78,   147,   147,   147,   147,    96,   978,   147,
     461,   462,    55,   147,    90,   109,   467,    55,   151,   470,
     471,   472,   473,   165,   113,   172,   480,    89,   165,   173,
     173,    93,   173,   109,   171,   173,   147,    55,   536,   173,
     346,  1241,  1242,  1243,  1244,  1245,  1246,   890,   147,   346,
     536,   875,   536,    55,   360,   506,   498,   147,   500,   147,
      31,   165,   173,   360,   370,   498,   165,   500,   172,   172,
     174,   174,   562,   370,   564,   837,    27,   383,    55,   141,
     728,    55,   844,   173,   146,   173,   383,   149,   850,   147,
     147,   153,   100,   855,   147,   172,    47,   165,   746,   550,
      51,   863,   147,   171,  1116,   147,  1118,   165,   165,    55,
    1025,   562,   165,   564,   170,   569,   172,   147,    89,    55,
     165,   572,    93,   165,    75,    55,   147,    78,   147,    80,
      55,   172,   591,   174,   576,   165,   576,    27,   172,    90,
     174,    78,    79,   576,   165,   907,   165,   909,   910,    89,
      89,     3,   140,    93,    93,  1029,  1030,    47,   109,  1033,
    1034,    51,   810,   811,  1038,  1039,   170,   929,   930,    89,
     141,   141,   172,    93,   174,   146,   146,   148,   149,   149,
      89,    55,   153,   153,    93,    75,   146,   172,    78,   160,
      80,   172,   498,   174,   500,   164,  1172,   168,   165,   165,
      90,   498,   165,   500,   172,   562,   174,   564,   165,    12,
     165,   165,   974,   975,   976,   125,    19,   165,    55,   109,
     172,   165,   173,   871,   165,   170,    84,   170,    31,   147,
       2,   173,  1056,    36,   172,  1109,  1110,  1111,  1112,   172,
     836,   172,   152,   146,   154,   155,   165,   157,   158,   172,
     165,     8,     3,    25,   744,   172,    65,   170,   125,   171,
      90,  1237,    90,   125,   912,   149,    68,   172,   125,    72,
     576,   173,   147,    34,   890,   125,   766,   728,   125,   576,
      34,    65,   173,   125,   173,   146,    89,   173,   873,    21,
      93,   876,   170,   744,   149,   746,   119,   739,   147,   172,
    1215,   105,    74,   172,   149,   149,    78,    79,   750,     2,
     149,   753,   141,   149,   149,   766,   149,   750,   149,   165,
     753,   124,   149,   149,   149,   149,    34,   149,   149,   101,
     978,   149,    25,   105,   175,   173,    34,   171,   141,   142,
      31,   144,   145,   146,    47,   148,   149,   150,   165,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   172,   165,   166,   167,   168,   165,   165,   165,   165,
     146,   165,   173,   147,   177,   873,   171,   170,    34,   877,
     172,    74,   146,   173,   175,    78,    79,   841,   886,    27,
     175,   172,   175,   172,   165,   165,   165,   147,    89,    32,
     173,   843,    93,    32,    34,   173,   165,   849,   101,    47,
     843,   173,   105,    51,   147,   175,   849,  1093,  1094,   861,
     871,   173,   175,   865,   165,   165,   175,  1012,   861,   164,
     164,   110,   865,   978,   808,   571,   370,    75,    24,   945,
      78,   213,    80,   970,   750,  1093,  1094,   753,   261,   334,
     141,   142,    90,   750,   145,   146,   753,   148,   149,   387,
     381,   912,   153,   362,   536,   886,   890,  1052,   522,   160,
     877,   109,   533,  1237,  1058,   166,   167,   168,  1247,  1240,
     300,   835,   675,   130,   519,   762,    -1,   929,   930,    -1,
      -1,    -1,    -1,   265,   936,    -1,   938,    -1,   270,   941,
      -1,    -1,    -1,   936,    -1,   938,    -1,  1053,   941,    -1,
    1056,    31,   966,    -1,  1012,    -1,    -1,    -1,  1134,    -1,
     213,    -1,    -1,   965,    -1,  1141,    22,   978,    -1,    -1,
      26,    27,   965,    -1,    -1,   977,    -1,   843,    -1,    35,
    1125,    37,    38,   849,   977,    -1,   843,    43,    -1,    -1,
      -1,    -1,   849,    -1,    50,   861,  1015,  1016,    -1,   865,
      -1,    -1,    -1,    -1,   861,    -1,    -1,    -1,   865,    89,
      -1,    -1,   265,    93,   346,    -1,    -1,   270,    -1,    -1,
      76,    -1,    78,    -1,    -1,    -1,    -1,    -1,   360,    -1,
      -1,    -1,    88,    -1,    90,  1037,    -1,    -1,   370,    -1,
      -1,  1043,    -1,    -1,  1037,    -1,    -1,    -1,    -1,    -1,
    1043,   383,    -1,   109,  1160,    -1,   112,    -1,    -1,   115,
      -1,   141,   142,    -1,   144,   145,   146,  1125,   148,   149,
     936,    -1,   938,   153,    -1,   941,    -1,    -1,    -1,   936,
     160,   938,  1093,  1094,   941,  1087,   166,   167,   168,    -1,
      -1,    -1,    -1,   346,  1087,    -1,    -1,  1273,  1274,   965,
      -1,  1277,  1278,  1279,    -1,  1281,    -1,   360,   965,    -1,
      -1,   977,    -1,    -1,    -1,    -1,    -1,   370,    -1,    -1,
     977,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     383,    -1,    -1,  1135,    -1,  1137,    -1,    -1,    -1,    -1,
    1142,    -1,  1135,  1145,  1137,    -1,    -1,    -1,    -1,  1142,
      -1,  1153,  1145,  1155,    -1,    -1,    -1,    -1,    -1,    -1,
    1153,    -1,  1155,    -1,    -1,    -1,   498,    -1,   500,    -1,
      -1,  1037,    -1,    -1,    -1,    -1,    -1,  1043,    -1,    -1,
    1037,    -1,    -1,    -1,  1186,    -1,  1043,    -1,    -1,    -1,
    1192,    -1,    -1,  1186,  1196,  1197,    -1,    -1,    -1,  1192,
      -1,    -1,    -1,  1196,  1197,    -1,    -1,    -1,  1210,    -1,
      -1,    -1,    -1,    -1,    -1,  1217,    -1,  1210,     2,    -1,
      -1,  1087,    -1,    -1,  1217,    -1,    -1,  1229,    -1,    -1,
    1087,  1233,    -1,    -1,    -1,    -1,  1229,    -1,    -1,    -1,
    1233,    25,    -1,    -1,   576,   498,  1248,   500,  1248,    -1,
      -1,    -1,    -1,    -1,    -1,  1248,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1135,
      31,  1137,    -1,    -1,    -1,    -1,  1142,    -1,  1135,  1145,
    1137,    -1,    -1,    -1,    -1,  1142,    -1,  1153,  1145,  1155,
      74,    -1,    -1,    -1,    78,    79,  1153,    -1,  1155,    -1,
    1302,  1303,  1304,  1305,  1306,  1307,    -1,    -1,    -1,  1302,
    1303,  1304,  1305,  1306,  1307,    -1,    -1,   101,    -1,    -1,
    1186,   105,    -1,   576,    -1,    -1,  1192,    -1,    89,  1186,
    1196,  1197,    93,    -1,    -1,  1192,    -1,    -1,    -1,  1196,
    1197,    -1,    -1,    -1,  1210,    -1,    -1,    -1,    -1,    -1,
      -1,  1217,    -1,  1210,    -1,     2,    -1,    -1,    -1,    -1,
    1217,    -1,    -1,  1229,    -1,    -1,    -1,  1233,    -1,    -1,
      -1,    -1,  1229,    -1,    -1,    -1,  1233,    -1,    25,    -1,
     141,   142,  1248,   144,   145,   146,    -1,   148,   149,   150,
      -1,  1248,   153,   154,   155,    -1,   157,   158,   159,   160,
      -1,    -1,   163,    -1,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   750,    -1,
      -1,   753,    -1,    -1,    -1,    -1,    -1,    74,    -1,   213,
      -1,    78,    79,    -1,    -1,    -1,  1302,  1303,  1304,  1305,
    1306,  1307,    -1,     1,    -1,  1302,  1303,  1304,  1305,  1306,
    1307,    -1,    -1,    -1,   101,    -1,    -1,    -1,   105,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    37,
      38,   265,    -1,    -1,    -1,    43,   270,    -1,    -1,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,   750,    -1,    -1,
     753,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   843,    -1,    -1,    -1,    73,    -1,   849,    76,    -1,
      78,    -1,    80,    -1,    82,    83,    -1,    85,    -1,   861,
      88,    -1,    90,   865,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,   109,    37,    38,   112,    -1,    -1,   115,    43,    -1,
      -1,    -1,   346,    -1,    -1,    50,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   360,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,   370,    -1,    -1,    -1,
     843,    76,    -1,    78,    -1,    80,   849,    82,    -1,   383,
      85,    -1,    -1,    88,   936,    90,   938,   165,   861,   941,
      -1,    -1,   865,   171,    -1,    -1,    -1,    -1,   265,    -1,
      -1,    -1,    -1,   270,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,   965,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   977,    -1,    -1,    -1,    -1,
      44,     3,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    17,    18,    -1,    20,    -1,
      -1,    -1,    -1,    25,    68,    -1,    -1,    -1,    -1,    -1,
     165,    33,    -1,   936,    -1,   938,   171,    39,   941,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,   346,
      52,    -1,    -1,    -1,    -1,  1037,    -1,    -1,    -1,    -1,
      62,  1043,   965,   360,   498,    -1,   500,    69,    70,    -1,
      -1,    -1,    89,   370,   977,    77,    93,    -1,    -1,    -1,
       2,    -1,    -1,    -1,    -1,    87,   383,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      -1,   103,    -1,    25,   106,  1087,    -1,    -1,   110,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,    -1,    -1,   146,
      -1,   148,   149,    -1,  1037,    -1,   153,    -1,    -1,    -1,
    1043,    -1,   576,   160,    -1,    -1,    -1,    -1,    -1,   166,
     167,   168,    74,  1135,    -1,  1137,    78,    79,    -1,    -1,
    1142,    -1,    -1,  1145,    -1,    -1,    -1,    -1,    -1,    -1,
     172,  1153,    -1,  1155,     3,    -1,    -1,    -1,    -1,   101,
      -1,    -1,    -1,   105,  1087,    -1,    -1,    -1,    17,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,   498,    -1,   500,  1186,    -1,    35,    -1,    -1,    -1,
    1192,    -1,    -1,    -1,  1196,  1197,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,  1210,   273,
      -1,   275,  1135,    62,  1137,  1217,    -1,    -1,   282,  1142,
      -1,    70,  1145,    -1,    -1,    -1,    -1,  1229,    -1,    -1,
    1153,  1233,  1155,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1248,    -1,    -1,     2,
      99,    -1,    -1,    -1,   103,    -1,    -1,    -1,   322,   576,
      -1,   110,    -1,  1186,    -1,    -1,    -1,   116,    -1,  1192,
      -1,   213,    25,  1196,  1197,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1210,    -1,    -1,
      -1,    -1,    -1,    -1,  1217,    -1,   750,    -1,    -1,   753,
    1302,  1303,  1304,  1305,  1306,  1307,  1229,    -1,    -1,    -1,
    1233,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    74,    -1,   265,    -1,  1248,    -1,    -1,   270,    -1,
      -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,   101,    33,
      -1,    -1,   105,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,  1302,
    1303,  1304,  1305,  1306,  1307,    69,    70,    -1,    -1,   843,
      -1,    -1,    -1,    77,    -1,   849,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,   346,    -1,    -1,   861,    -1,    -1,
      -1,   865,    96,    97,    -1,    99,    -1,   101,   360,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,   111,   370,   113,
      -1,    -1,   116,   750,    -1,    22,   753,    -1,    -1,    26,
      27,   383,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      37,    38,    -1,    22,    -1,    -1,    43,    26,    27,    -1,
     213,    -1,    -1,    50,    -1,    -1,    35,    -1,    37,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,
      -1,    50,   936,    -1,   938,    -1,    -1,   941,   172,    76,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    85,    -1,
      -1,    88,    -1,    90,    -1,    -1,    -1,    76,    -1,    78,
      -1,   965,   265,    82,    -1,    -1,    85,   270,    -1,    88,
      -1,    90,   109,   977,    -1,   112,   843,    -1,   115,    -1,
      -1,   595,   849,   597,   598,   599,   600,   601,   602,    -1,
     109,    -1,    -1,   112,   861,    -1,   115,    -1,   865,    -1,
      -1,    -1,    -1,   140,    -1,    -1,   498,    -1,   500,    -1,
      22,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,
      -1,   140,    -1,    35,    -1,    37,    38,    -1,   165,    -1,
      -1,    43,    -1,  1037,   171,    -1,    -1,    -1,    50,  1043,
      -1,    -1,    -1,   346,    -1,    -1,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   360,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    78,   370,    80,   936,
      82,   938,    -1,    85,   941,    -1,    88,    -1,    90,    -1,
     383,    -1,    -1,  1087,   576,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   965,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
     977,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    40,    41,   140,    -1,
      -1,  1135,    -1,  1137,    -1,    -1,    -1,    -1,  1142,    -1,
      -1,  1145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1153,
      -1,  1155,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1037,    -1,    -1,    -1,    -1,    -1,  1043,    -1,    -1,    -1,
      -1,    -1,  1186,    -1,    -1,   799,    -1,    -1,  1192,    -1,
      -1,    -1,  1196,  1197,    22,   498,    -1,   500,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,  1210,    35,    -1,    37,
      38,    -1,    -1,  1217,    -1,    43,    -1,    -1,    -1,    -1,
    1087,    -1,    50,    -1,   838,  1229,    -1,    -1,    -1,  1233,
      -1,    -1,    -1,    -1,   848,   149,    -1,   151,   852,    -1,
     854,    -1,    -1,   857,  1248,    -1,    -1,    -1,    76,   163,
      78,    -1,    80,    -1,    82,    -1,    -1,    85,   750,    -1,
      88,   753,    90,    -1,    -1,    -1,    -1,    -1,  1135,    -1,
    1137,    -1,    -1,   576,    -1,  1142,    31,    -1,  1145,    -1,
      -1,   109,    -1,   197,   112,    -1,  1153,   115,  1155,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1302,  1303,
    1304,  1305,  1306,  1307,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,  1186,
      -1,    -1,    -1,    -1,    -1,  1192,    -1,    -1,    -1,  1196,
    1197,    -1,    -1,    -1,    89,    -1,    -1,   165,    93,    -1,
      -1,   255,   256,  1210,    -1,    -1,    -1,    -1,    -1,    -1,
    1217,   843,   266,    -1,    -1,    -1,    -1,   849,    -1,    -1,
      -1,    -1,  1229,    -1,    -1,    -1,  1233,    -1,    -1,   861,
      -1,    -1,    -1,   865,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1248,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,   306,   148,   149,   150,    -1,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,    -1,   163,  1023,
    1024,   166,   167,   168,    -1,  1029,  1030,    -1,    -1,  1033,
    1034,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1302,  1303,  1304,  1305,  1306,
    1307,    -1,    -1,   357,   936,    -1,   938,   750,    -1,   941,
     753,    -1,    -1,    -1,    -1,    31,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,    -1,   965,    -1,    -1,    -1,    -1,   392,    -1,
      -1,    -1,    -1,   397,    -1,   977,    -1,    -1,    -1,  1103,
      -1,    -1,    -1,  1107,  1108,  1109,  1110,    -1,    -1,    -1,
      -1,   169,    -1,   417,   172,    -1,   174,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,  1129,  1130,    93,  1132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1144,    -1,    -1,  1147,    -1,  1149,    -1,  1151,    -1,    -1,
     843,    -1,    -1,    -1,    -1,  1037,   849,    -1,   124,    -1,
      -1,  1043,    -1,    -1,    -1,    -1,    -1,    -1,   861,    -1,
      -1,    -1,   865,    -1,    -1,   141,   142,   481,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
     166,   167,   168,   507,    -1,  1087,    -1,   511,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     524,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   533,
      -1,    -1,    -1,    -1,   538,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   936,    -1,   938,    -1,    -1,   941,    -1,
      -1,    -1,    -1,  1135,    -1,  1137,    -1,    -1,    -1,    -1,
    1142,    -1,    -1,  1145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1153,   965,  1155,    -1,    -1,    -1,    -1,   582,    -1,
      -1,    -1,    -1,    -1,   977,    -1,    -1,    -1,    -1,   593,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,  1186,    -1,    -1,    -1,    -1,    -1,
    1192,    -1,    -1,    -1,  1196,  1197,    -1,    17,    18,    -1,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,  1210,    -1,
      -1,    -1,    -1,    33,    -1,  1217,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,  1037,    45,    -1,  1229,    -1,    49,
    1043,  1233,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    -1,    -1,  1248,    -1,    -1,    69,
      70,   675,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,  1087,    -1,    96,    97,    -1,    99,
      -1,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
    1302,  1303,  1304,  1305,  1306,  1307,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1135,    -1,  1137,    -1,    -1,    -1,    -1,  1142,
      -1,   755,  1145,    -1,    -1,    -1,    -1,    13,   762,    -1,
    1153,    17,  1155,    -1,    -1,    -1,    -1,    31,   772,    -1,
      26,    -1,   172,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   790,    -1,    44,    -1,
      46,    47,    -1,  1186,    -1,    -1,    52,    -1,    54,  1192,
      56,    -1,    -1,  1196,  1197,    61,    -1,    -1,    -1,    65,
      66,    67,    68,    69,    70,    -1,    -1,  1210,    74,    -1,
      76,    -1,    -1,    -1,  1217,    89,    -1,    83,    -1,    93,
      86,   835,    -1,    89,    -1,    91,  1229,    -1,    94,    95,
    1233,    97,    -1,    -1,    -1,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1248,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   875,    -1,    -1,   130,    -1,   132,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   890,    -1,    -1,   153,
      -1,    -1,    -1,    -1,    -1,   159,   160,    -1,    -1,   163,
      -1,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,  1302,
    1303,  1304,  1305,  1306,  1307,     3,    89,   173,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
     196,   945,    -1,   199,   200,    33,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,   970,    -1,   141,   142,
     226,   144,   145,   146,    62,   148,   149,   150,    -1,    -1,
     153,    69,    70,    -1,    -1,    -1,   159,   160,    -1,    77,
     163,    -1,    -1,   166,   167,   168,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,   177,    -1,   262,    -1,    96,    97,
      12,    99,    -1,   101,    -1,   103,    -1,    19,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    31,
     286,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   297,   298,    -1,    -1,    -1,     2,    -1,    -1,    -1,
      -1,    -1,  1056,    -1,    -1,    -1,   312,   313,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
     326,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   170,    -1,   172,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,   349,    -1,    -1,    -1,    -1,    -1,    -1,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   371,    -1,   373,    74,    -1,
      -1,    -1,   124,    -1,    -1,    -1,   382,    -1,   384,   385,
     386,   387,   388,    -1,    -1,    -1,    -1,   393,    -1,   141,
     142,    -1,   144,   145,   146,   101,   148,   149,   150,   105,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,   170,   425,
     426,   427,   428,   429,   430,   177,   432,   433,   434,   435,
     436,    -1,   438,   439,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,    -1,
      -1,    -1,    -1,    -1,    -1,   461,   462,    -1,    -1,    -1,
      -1,   467,    -1,    -1,   470,   471,   472,   473,   474,   475,
      -1,    44,   478,   479,   480,    -1,   482,    -1,    -1,    -1,
      -1,    -1,   488,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   500,    68,    -1,   503,    -1,    -1,
     506,    -1,    -1,    -1,    -1,    78,    79,   213,    -1,    -1,
     516,    -1,   518,    -1,    -1,    -1,   522,    -1,    -1,   525,
     526,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     536,    -1,    -1,    -1,    -1,   541,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,   550,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   559,    -1,    -1,   562,    -1,   564,   265,
      -1,   567,   568,   569,   270,    -1,   572,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,   583,   584,    26,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      37,    38,    -1,    -1,    -1,    -1,    43,   603,    -1,    -1,
      -1,    -1,    -1,    50,    -1,    -1,    53,   613,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   623,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,    76,
      -1,    78,    -1,    -1,    -1,    82,    83,    84,    85,    12,
     346,    88,    -1,    90,    -1,   218,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   360,    -1,    -1,    -1,    31,    32,
     666,    -1,   109,    -1,   370,   112,    -1,   673,   115,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,   383,    -1,    -1,
      -1,    -1,    -1,    -1,   257,   258,    -1,    -1,    -1,    -1,
      -1,   264,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     273,    -1,   275,    -1,    -1,    -1,    -1,    -1,    -1,   282,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,   165,    -1,
      93,   727,   728,   170,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   744,    -1,
     746,   747,   748,    12,    13,    -1,    -1,   120,    -1,   322,
      19,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     766,    -1,    31,    -1,    -1,    -1,    -1,    36,   141,   142,
      -1,   144,   145,   146,   780,   148,   149,   150,   784,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,   498,   166,   167,   168,    -1,   170,    -1,    -1,
      -1,    -1,   808,    72,   177,    -1,   812,    -1,   814,   815,
     816,    -1,   818,    -1,   820,    -1,    -1,   823,   824,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,   837,    -1,   839,    -1,   841,    -1,   843,   844,    -1,
      -1,    -1,    -1,   849,   850,    -1,    -1,    -1,    -1,   855,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   863,   864,   865,
      -1,   867,   868,    -1,    -1,   871,    -1,    -1,    -1,    -1,
     576,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
     906,   907,   908,   909,   910,   911,   912,    -1,   177,   915,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   929,   930,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   939,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     956,    -1,    -1,    -1,   960,    -1,    -1,    -1,    -1,    -1,
     966,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   974,   975,
     976,    -1,   978,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   999,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1007,    -1,  1009,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1019,    -1,  1021,    -1,    -1,    -1,  1025,
      -1,  1027,   595,   596,   597,   598,   599,   600,   601,   602,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   750,    -1,    -1,   753,    -1,    -1,
      -1,    -1,    -1,    -1,  1060,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1070,    -1,    -1,  1073,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1093,  1094,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1115,
    1116,  1117,  1118,  1119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1135,
      -1,  1137,    -1,    -1,    -1,    -1,  1142,   843,    -1,  1145,
      -1,    -1,    -1,   849,    -1,    -1,    -1,  1153,    -1,  1155,
      -1,    -1,  1158,    -1,    -1,   861,    -1,    -1,    -1,   865,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1177,  1178,    -1,  1180,  1181,  1182,  1183,    -1,    -1,
      -1,   754,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1205,
      -1,    -1,    -1,    -1,  1210,  1211,    -1,  1213,    -1,  1215,
      -1,  1217,  1218,    -1,    -1,  1221,    -1,   790,    -1,    -1,
    1226,    -1,    -1,    -1,  1230,   798,   799,    -1,    -1,    -1,
     936,    -1,   938,  1239,    -1,   941,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1254,  1255,
    1256,  1257,  1258,  1259,  1260,  1261,    -1,    -1,    -1,   965,
      -1,    -1,    -1,    -1,    -1,   838,    -1,    -1,    -1,    -1,
      -1,   977,    -1,    -1,    -1,   848,    -1,    -1,    -1,   852,
      -1,   854,    -1,    -1,   857,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1302,  1303,  1304,  1305,
    1306,  1307,  1308,  1309,  1310,  1311,  1312,  1313,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,  1037,    -1,    -1,    -1,    -1,    -1,  1043,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,   946,    69,    70,    -1,    -1,    -1,    -1,
      -1,  1087,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
     973,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1135,
      -1,  1137,    -1,    -1,    -1,    -1,  1142,    -1,    -1,  1145,
      -1,    -1,    -1,    -1,    -1,    -1,   141,  1153,   143,  1155,
    1023,  1024,    -1,    -1,    -1,    -1,  1029,  1030,    -1,    -1,
    1033,  1034,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,   174,
    1186,    -1,    -1,    -1,    -1,    -1,  1192,    -1,    -1,    -1,
    1196,  1197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    17,    18,    19,
      20,    -1,    -1,  1229,    -1,    25,    -1,  1233,    -1,    -1,
    1103,    -1,    -1,    33,  1107,  1108,  1109,  1110,    -1,    -1,
      -1,    -1,  1248,    -1,    -1,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,  1129,  1130,    -1,  1132,
      -1,  1134,    62,    -1,    -1,    -1,    -1,    -1,  1141,    -1,
      70,  1144,    -1,    -1,  1147,    -1,  1149,    77,  1151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,
      -1,   101,    -1,   103,    -1,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,  1194,    -1,    -1,    -1,   125,   126,   127,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,    -1,   138,   139,
      -1,   141,   142,   143,   144,   145,   146,    -1,   148,    -1,
      -1,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,   163,    -1,    -1,   166,   167,   168,   169,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1273,  1274,    -1,    -1,  1277,  1278,  1279,    -1,  1281,     0,
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
     164,    -1,    -1,    -1,   168,   124,   170,    -1,   172,   173,
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
      -1,   103,    -1,    -1,   106,    -1,    -1,     3,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    12,    -1,    -1,    -1,
      -1,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,   141,
      -1,   143,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      -1,    -1,   164,    -1,    -1,    -1,    62,    -1,    -1,    -1,
     172,    -1,   174,    69,    70,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    12,    89,    -1,    -1,    -1,    93,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    47,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    12,
      89,   166,   167,   168,    93,   170,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    47,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    12,    89,   166,   167,   168,
      93,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      47,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,   170,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,   170,    19,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
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
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    47,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    12,    89,   166,   167,   168,
      93,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
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
     157,   158,   159,   160,   161,   162,   163,    -1,    89,   166,
     167,   168,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,   124,    -1,    -1,    -1,    36,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    72,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    12,
      89,    -1,    -1,    -1,    93,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    12,    89,   166,   167,   168,
      93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,   170,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      31,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    89,   166,
     167,   168,    93,    -1,    -1,    -1,    12,    -1,   175,    -1,
     177,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,   120,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    89,   175,    -1,   177,    93,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    12,    -1,   166,   167,   168,   124,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    12,    -1,
     166,   167,   168,    -1,   170,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,   141,   142,    -1,   144,   145,   146,   102,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,   165,   166,   167,   168,
     124,    -1,    -1,    -1,    -1,    89,    -1,    -1,   177,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    -1,    -1,    -1,    12,    13,
      -1,    -1,    -1,   177,    -1,    19,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    31,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,   173,
      -1,    -1,    -1,   177,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    34,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,   123,   124,    -1,    -1,    -1,
      12,    89,    -1,   177,    -1,    93,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    31,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    12,    89,   166,   167,
     168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    12,    13,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    31,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    89,   166,   167,   168,    93,    -1,    -1,
      -1,   173,    12,    -1,    -1,   177,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   124,    -1,
     166,   167,   168,    -1,    -1,    -1,    -1,   173,    -1,    89,
      -1,   177,    -1,    93,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    12,    13,
     166,   167,   168,    -1,   124,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,    -1,
      -1,    -1,    12,   173,    -1,    -1,    -1,   177,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    89,
     144,   145,   146,    93,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,   177,   124,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    12,    93,    -1,    -1,    -1,    -1,    -1,    19,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    31,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,   165,   166,   167,   168,    89,
      -1,    -1,    12,    93,    -1,    -1,    -1,   177,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    31,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,    89,
      -1,    -1,    12,    93,    -1,    -1,    -1,   177,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    89,
      -1,    -1,    12,    93,    -1,    -1,    -1,   177,    -1,    19,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    31,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    -1,    12,    13,    -1,   175,    -1,   177,    -1,    19,
      60,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    31,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    12,    89,
      -1,    -1,    -1,    93,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    31,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
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
     167,   168,     1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    17,    18,    -1,    20,    -1,    77,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    87,    33,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    96,    97,    -1,
      99,    45,   101,    -1,   103,    49,    -1,   106,    52,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,     3,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,    87,    -1,    -1,    -1,    -1,    17,    18,
      -1,    20,    96,    97,    -1,    99,    25,   101,    27,   103,
      -1,    -1,   106,    -1,    33,    -1,   110,    -1,    -1,   113,
      39,    -1,   116,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    -1,    -1,    62,    -1,    -1,   140,    -1,    -1,    -1,
      69,    70,    -1,    17,    18,    -1,    20,    -1,    77,    78,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    87,    33,
      -1,    90,    -1,    -1,    -1,    39,    -1,    96,    97,    -1,
      99,    45,   101,    -1,   103,    49,    -1,   106,    52,    -1,
      -1,   110,    -1,    -1,   113,    -1,   115,   116,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    17,    18,
      -1,    20,    96,    97,    -1,    99,    25,   101,    -1,   103,
      -1,    -1,   106,    -1,    33,    -1,   110,   111,    -1,   113,
      39,    -1,   116,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    17,    18,    -1,    20,    -1,    77,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    87,    33,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    96,    97,    -1,
      99,    45,   101,    -1,   103,    49,    -1,   106,    52,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    17,    18,
      -1,    20,    96,    97,    -1,    99,    25,   101,    -1,   103,
      -1,    -1,   106,    -1,    33,    -1,   110,    -1,    -1,   113,
      39,    -1,   116,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116
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
     249,   250,   251,   252,   256,   257,   262,   263,   264,   265,
     268,   269,   274,   275,   277,   278,   283,   287,   288,   290,
     291,   313,   318,   319,   323,   324,   345,   346,   347,   349,
     351,   352,   353,   361,   362,   363,   364,   365,   366,   368,
     371,   372,   373,   374,   375,   376,   377,   378,   380,   381,
     382,   383,   384,   165,    22,    35,    44,    55,    88,   101,
     170,   239,   248,   275,   345,   352,   362,   363,   368,   371,
     373,   374,   120,   354,   355,     3,   218,   368,   354,   109,
     325,    90,   218,   186,   339,   368,   172,     3,    17,    18,
      20,    25,    33,    39,    45,    49,    52,    62,    69,    70,
      77,    87,    96,    97,    99,   101,   103,   106,   110,   113,
     116,   219,   220,   222,    11,    27,   115,   244,   368,    82,
      85,   202,   172,   220,   220,   220,   172,   220,   326,   327,
      32,   206,   225,   368,   260,   261,   368,   368,    18,    77,
      96,   113,   368,   368,   368,     8,   172,   229,   228,    27,
      33,    47,    49,    51,    75,    78,    90,    97,   101,   109,
     185,   224,   279,   280,   281,   304,   305,   306,   331,   336,
     368,   339,   107,   108,   165,   283,   284,   367,   368,   370,
     368,   225,   368,   368,   368,   100,   172,   186,   368,   368,
     188,   192,   206,   188,   206,   368,   370,     3,   368,   368,
     368,   368,   368,     1,   171,   184,   207,   339,   111,   151,
     340,   341,   370,   368,    80,   186,    22,    35,    38,    88,
     170,   189,   190,   191,   202,   206,   194,     8,   149,   196,
     170,    46,    84,   114,   203,    26,   324,   368,     8,   264,
     368,   369,    24,    32,    40,    41,    42,   119,   174,   241,
     348,   350,    55,   146,   266,   220,     1,   193,   220,   270,
     273,   172,   299,    53,    73,    83,   308,    27,    75,    78,
      90,   109,   309,    27,    78,    90,   109,   307,   220,   320,
     321,   326,   164,   165,   368,    12,    19,    31,    89,    93,
     124,   141,   142,   144,   145,   146,   148,   149,   150,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   166,   167,   168,   177,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     169,   295,   172,   174,    89,    93,   368,   220,    40,    41,
     174,    55,   172,   186,   165,   206,   323,   165,   147,   165,
     221,   342,   343,   344,   370,   170,   220,   220,   102,   206,
     102,   125,   206,   295,   203,   342,   172,   172,   206,   186,
     111,   172,   220,   328,   329,     1,   146,   335,    47,   147,
     186,   225,   147,   225,    13,   172,   172,   225,   342,   230,
     230,    47,    90,   305,   306,   173,   147,   111,   172,   220,
     303,   146,   165,   368,   368,   123,   285,   165,   170,   225,
     172,   342,   165,   253,   253,   165,   171,   171,   184,   147,
     171,   368,   147,   173,   147,   173,     8,    40,    41,   174,
     241,     3,   172,   197,     1,   171,   207,   214,   215,   368,
     209,   368,    65,    36,    72,   165,   264,   266,   109,   236,
     288,   368,   186,    78,   122,   242,   368,   242,   368,   242,
     368,   244,   368,   175,   242,   339,   368,   379,   220,   339,
     170,   171,   273,   125,   147,   171,   173,   300,   302,   304,
     309,    90,     1,   146,   333,   334,    90,     1,     3,    12,
      17,    19,    20,    25,    45,    52,    54,    62,    70,    87,
      99,   103,   110,   116,   141,   142,   143,   144,   145,   146,
     148,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   163,   166,   167,   168,   169,   172,   220,   292,   293,
     294,   295,   345,   125,   322,   147,   165,   165,   368,   368,
     368,   242,   368,   242,   368,   368,   368,   368,   368,   368,
     368,     3,    20,    33,    62,   103,   109,   221,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   368,   368,    68,   370,   370,   370,   370,
     370,   342,   342,   242,   368,   242,   368,   102,   172,   368,
     368,   339,   220,    27,    47,    90,   115,   356,   359,   360,
     368,   384,   326,   368,   125,   173,   147,   125,   125,   186,
      34,   186,   368,    34,   368,    65,   173,   343,   220,   246,
     247,   329,   147,   173,   224,   368,   125,   330,   368,   327,
     260,   220,   323,   368,   368,   173,   303,   309,   281,   329,
     146,   337,   338,   336,   286,   205,     1,   258,   344,   173,
      21,   254,   171,   173,   173,   340,   173,   340,   368,   368,
     339,   196,   173,   198,   199,   344,   171,   171,   147,   165,
      13,   149,   147,   165,    13,    36,    72,   220,   141,   142,
     143,   144,   145,   159,   163,   168,   208,   294,   295,   296,
     368,   208,   210,   266,   170,   237,   323,    47,   354,   119,
     220,   172,   225,    32,    47,   225,   120,   225,   357,    32,
      47,   225,   357,   225,   357,    47,   225,   357,   225,   225,
     368,   175,   357,    47,   147,   175,   357,   123,   147,   175,
     165,   172,     1,   267,   368,   271,   272,   147,   173,   335,
       1,   224,   331,   332,   105,   310,   172,   298,   368,   141,
     149,   298,   298,   368,   321,   172,   174,   165,   165,   165,
     165,   165,   165,   173,   175,   343,    32,    47,    32,    47,
      47,   175,   172,    47,    90,   147,   173,    17,    20,    25,
      45,    52,    62,    70,    87,    99,   110,   116,   345,    89,
      89,   165,   370,   344,   368,   368,    34,   186,    34,    34,
     186,   102,   206,   220,   173,   147,   173,   173,   328,   335,
      68,   370,   220,   173,   173,   338,   146,   282,   173,   336,
     151,   297,   330,   368,   171,    74,   117,   171,   259,   173,
     172,   206,   220,   255,    47,    47,    47,   175,   147,   173,
     214,   221,    17,    18,    20,    25,    45,    49,    52,    70,
      77,    87,    96,    97,    99,   101,   110,   113,   116,   170,
     213,   296,   368,   111,   368,   208,   210,   147,   165,    13,
     165,   170,   267,   320,   326,   242,   368,   225,   368,    47,
     339,    44,   368,   242,   368,   172,   225,    44,   368,   242,
     368,   225,   225,   242,   368,   225,   186,   175,   242,   368,
     175,   186,   368,   175,   368,   175,   368,   343,   171,    82,
      85,   171,   185,   193,   227,   273,   193,   302,   310,    60,
     118,   314,   300,   301,   173,   293,   295,   173,   175,   173,
      44,   242,   368,    44,   242,   368,   242,   368,   343,   359,
     335,   345,   345,   186,   186,   368,   186,    34,   165,   165,
     247,   206,   330,   172,   172,   297,   330,   331,   310,   338,
     368,   186,   239,   339,   255,   146,   206,   242,   368,   242,
     368,   242,   368,   199,   208,    13,    36,    72,    36,    72,
     165,   165,   296,   368,   368,   267,   171,   165,   165,   354,
     354,   165,   368,   173,   368,    32,   225,    32,   225,   358,
     359,   368,    32,   225,   357,    32,   225,   357,   225,   357,
     225,   357,   186,   175,   357,   175,   357,   186,   123,   173,
     193,   227,   227,   273,   314,     3,    92,   103,   315,   316,
     317,   368,   289,   173,   298,   298,   368,    32,    32,   368,
      32,    32,   175,   175,   173,   330,    34,   186,   342,   342,
     297,   330,    32,   225,   173,   368,   175,   175,   171,   111,
     368,   208,   210,   208,   210,    13,   171,   225,   225,   225,
     102,    44,   368,    44,   368,   147,   173,   102,    44,   368,
     225,    44,   368,   225,   225,   225,    44,   186,   368,   175,
      44,   186,   368,   175,   368,   165,   227,   276,   317,   118,
     147,   125,   152,   154,   155,   157,   158,    60,    32,   165,
     205,   226,   311,   312,   102,    44,    44,   102,    44,    44,
      44,    44,   186,   173,   173,   186,   206,   165,   165,   165,
     165,   368,   368,   368,   368,   359,   368,   368,   368,   368,
     186,   368,   186,   312,   368,   316,   317,   317,   317,   317,
     317,   317,   315,   184,   368,   368,   368,   368,   368,   368,
     164,   164,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   368,   368,   368,   368,   368,   368
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
     230,   230,   230,   230,   230,   230,   231,   231,   231,   231,
     232,   233,   233,   234,   235,   235,   235,   236,   235,   237,
     235,   238,   239,   240,   240,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     242,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   244,   244,   245,   245,   246,   246,   247,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   249,   249,   249,   249,   249,   249,   250,
     250,   250,   251,   251,   251,   252,   252,   252,   252,   253,
     253,   254,   254,   254,   255,   255,   256,   257,   257,   258,
     258,   259,   259,   259,   260,   260,   260,   261,   261,   262,
     263,   263,   264,   265,   265,   265,   266,   266,   267,   267,
     267,   267,   267,   268,   268,   269,   270,   270,   271,   270,
     270,   272,   270,   273,   273,   274,   276,   275,   277,   278,
     278,   278,   279,   279,   280,   280,   281,   281,   281,   282,
     282,   283,   285,   284,   286,   284,   287,   289,   288,   290,
     290,   290,   290,   290,   291,   292,   292,   293,   293,   293,
     294,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   294,   294,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   296,   296,   297,
     297,   298,   298,   299,   299,   300,   300,   301,   301,   302,
     303,   303,   304,   304,   304,   304,   304,   304,   305,   305,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   307,
     307,   307,   307,   307,   307,   308,   308,   308,   309,   309,
     309,   309,   309,   309,   309,   310,   310,   311,   311,   312,
     312,   312,   313,   314,   314,   314,   314,   314,   315,   315,
     316,   316,   316,   316,   316,   316,   316,   317,   317,   318,
     319,   319,   320,   320,   321,   322,   322,   323,   323,   323,
     323,   323,   325,   324,   324,   326,   326,   327,   327,   328,
     328,   328,   329,   329,   329,   330,   330,   330,   331,   332,
     332,   332,   333,   333,   334,   334,   335,   335,   335,   335,
     336,   336,   337,   338,   338,   339,   339,   340,   340,   341,
     341,   342,   342,   343,   343,   344,   344,   345,   345,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     346,   347,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   349,   350,   350,   350,   350,   350,
     350,   350,   351,   352,   353,   353,   353,   353,   353,   353,
     353,   354,   354,   355,   356,   356,   357,   358,   358,   359,
     359,   359,   360,   360,   360,   360,   360,   360,   361,   361,
     361,   361,   361,   362,   362,   362,   362,   362,   363,   364,
     364,   364,   364,   364,   364,   365,   366,   367,   367,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   368,   368,   368,   369,   369,   370,   370,
     370,   371,   371,   371,   371,   372,   372,   372,   372,   372,
     373,   373,   373,   374,   374,   374,   374,   374,   374,   375,
     375,   375,   375,   376,   376,   377,   377,   378,   378,   378,
     378,   378,   378,   378,   378,   378,   378,   378,   378,   378,
     379,   379,   380,   380,   380,   380,   380,   380,   380,   380,
     380,   380,   380,   380,   380,   380,   380,   380,   380,   380,
     380,   380,   380,   380,   380,   381,   381,   381,   381,   381,
     381,   381,   382,   382,   382,   382,   383,   383,   383,   383,
     384,   384,   384,   384,   384,   384,   384
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
       1,     1,     1,     1,     1,     1,     3,     5,     5,     2,
       1,     1,     1,     1,     6,     7,     3,     0,     6,     0,
       6,     2,     0,     3,     2,     5,     3,     3,     6,     6,
       4,     5,     5,     3,     3,     6,     5,     6,     5,     6,
       3,     4,     3,     4,     5,     6,     5,     6,     3,     4,
       3,     4,     6,     7,     6,     7,     4,     5,     4,     5,
       4,     4,     3,     6,     5,     4,     3,     6,     5,     6,
       5,     8,     7,     4,     4,     6,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     6,
       4,     7,     5,     3,     6,     2,     2,     3,     3,     0,
       2,     2,     3,     5,     1,     3,     3,     5,     5,     0,
       2,     3,     2,     4,     4,     3,     1,     1,     3,     3,
       5,     5,     2,     1,     1,     1,     0,     2,     0,     2,
       3,     3,     4,     3,     3,     3,     1,     2,     0,     4,
       2,     0,     5,     1,     3,     1,     0,     8,     0,     1,
       1,     1,     0,     1,     1,     3,     1,     3,     1,     0,
       2,     6,     0,     3,     0,     4,     1,     0,     7,     4,
       4,     6,     6,     4,     2,     1,     3,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     0,     3,     2,     3,     1,     3,     0,     1,     1,
       1,     1,     4,     5,     4,     5,     6,     6,     0,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     0,
       1,     1,     2,     1,     1,     1,     1,     1,     0,     1,
       1,     2,     1,     1,     1,     0,     1,     1,     1,     1,
       2,     1,     1,     0,     2,     2,     4,     4,     1,     3,
       3,     3,     3,     3,     3,     3,     2,     1,     1,     3,
       1,     2,     1,     3,     2,     0,     2,     1,     2,     1,
       1,     1,     0,     5,     3,     1,     3,     3,     5,     1,
       1,     3,     1,     2,     3,     0,     2,     2,     1,     1,
       1,     1,     2,     1,     0,     1,     0,     2,     2,     1,
       1,     1,     2,     0,     1,     1,     3,     1,     1,     3,
       3,     0,     1,     1,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     4,     4,     4,     5,     2,
       2,     1,     2,     1,     2,     1,     2,     1,     2,     1,
       1,     2,     6,     6,     4,     9,     9,     7,     6,     6,
       4,     9,     9,     7,     2,     2,     3,     4,     6,     6,
       9,     9,     6,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     1,     4,     1,     3,     4,     1,     3,     4,
       3,     3,     1,     1,     2,     1,     2,     1,     1,     2,
       2,     2,     2,     2,     8,     8,     9,     9,     4,     3,
       3,     2,     2,     2,     1,     3,     4,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       4,     4,     4,     3,     3,     3,     3,     5,     5,     3,
       4,     3,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     4,     4,     5,     4,     5,
       3,     5,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     1,     1
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
#line 6532 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 639 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6538 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 640 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6544 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 647 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6552 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 655 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6560 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 659 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6569 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 669 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6577 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 674 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6585 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 681 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6591 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 682 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6597 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 683 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6603 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 684 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6609 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 685 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6615 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6621 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 687 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6627 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 688 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6633 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 689 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6639 "bison-chpl-lib.cpp"
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
#line 6656 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 703 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6662 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 704 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6668 "bison-chpl-lib.cpp"
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
#line 6681 "bison-chpl-lib.cpp"
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
#line 6694 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TLABEL ident_def stmt  */
#line 724 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6702 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 728 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6713 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: error TSEMI  */
#line 735 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6722 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: assignment_stmt  */
#line 742 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6728 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: if_stmt  */
#line 743 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6734 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: loop_stmt  */
#line 744 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6740 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: select_stmt  */
#line 745 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6746 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: manage_stmt  */
#line 746 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6752 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 747 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6758 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: throw_stmt  */
#line 748 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6764 "bison-chpl-lib.cpp"
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
#line 6782 "bison-chpl-lib.cpp"
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
#line 6801 "bison-chpl-lib.cpp"
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
#line 6813 "bison-chpl-lib.cpp"
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
#line 6829 "bison-chpl-lib.cpp"
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
#line 6844 "bison-chpl-lib.cpp"
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
#line 6860 "bison-chpl-lib.cpp"
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
#line 6876 "bison-chpl-lib.cpp"
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
#line 6891 "bison-chpl-lib.cpp"
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
#line 6908 "bison-chpl-lib.cpp"
    break;

  case 47: /* collect_attributes: %empty  */
#line 865 "chpl.ypp"
  {
    auto attributeGroup = context->buildAttributeGroup((yyloc));
    if (attributeGroup != nullptr) {
      context->resetAttributeGroupPartsState();
    }
    context->loopAttributes.push_back(std::move(attributeGroup));
  }
#line 6920 "bison-chpl-lib.cpp"
    break;

  case 53: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 890 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), true, (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6928 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace  */
#line 894 "chpl.ypp"
 {
  // this allows us to distinguish between @attribute() and @attribute
  // but should not be needed once `@unstable "msg"` is deprecated and gone
  auto lst = new MaybeNamedActualList();
  context->noteAttribute((yylsp[-1]), (yyvsp[-1].expr), false, (yyvsp[0].exprList), lst);
 }
#line 6939 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_stmt: attribute_decl_begin STRINGLITERAL  */
#line 901 "chpl.ypp"
 {
  // this allows us to maintain @unstable "message" without requiring parens
  // but should not be needed once `@unstable "msg"` is deprecated and gone
  auto lst = new MaybeNamedActualList();
  auto actual = makeMaybeNamedActual((yyvsp[0].expr));
  lst->push_back(actual);
  context->noteAttribute((yylsp[-1]), (yyvsp[-1].expr), false, context->makeList(), lst);
 }
#line 6952 "bison-chpl-lib.cpp"
    break;

  case 56: /* attribute_decl_begin: TATMARK TIDENT  */
#line 913 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6961 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: %empty  */
#line 920 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6969 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 924 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6980 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP TRP  */
#line 934 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6986 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 936 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6994 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 943 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 7003 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 948 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 7013 "bison-chpl-lib.cpp"
    break;

  case 64: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 961 "chpl.ypp"
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
#line 7034 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 981 "chpl.ypp"
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
#line 7053 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 996 "chpl.ypp"
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
#line 7072 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1011 "chpl.ypp"
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
#line 7089 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1026 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 7095 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1027 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 7102 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1029 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 7109 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1034 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 7115 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1035 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 7122 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1041 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 7132 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1059 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 7140 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1063 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 7148 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR error TRCBR  */
#line 1067 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 7160 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt: block_stmt_body  */
#line 1078 "chpl.ypp"
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
#line 7179 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1096 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 7185 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1097 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7191 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1102 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 7199 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1106 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7207 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1110 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 7216 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1115 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 7224 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1119 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7232 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1123 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 7241 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1133 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7250 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1138 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7260 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1144 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7271 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1151 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 7280 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1156 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7291 "bison-chpl-lib.cpp"
    break;

  case 91: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1163 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7303 "bison-chpl-lib.cpp"
    break;

  case 92: /* opt_only_ls: %empty  */
#line 1173 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7309 "bison-chpl-lib.cpp"
    break;

  case 93: /* opt_only_ls: renames_ls  */
#line 1174 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7315 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1179 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7324 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1184 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7336 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1192 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7349 "bison-chpl-lib.cpp"
    break;

  case 97: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1201 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7363 "bison-chpl-lib.cpp"
    break;

  case 98: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1211 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7375 "bison-chpl-lib.cpp"
    break;

  case 99: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1219 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7388 "bison-chpl-lib.cpp"
    break;

  case 100: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1228 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7402 "bison-chpl-lib.cpp"
    break;

  case 101: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1241 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7411 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_expr: expr  */
#line 1277 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 7419 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_expr: expr TDOT all_op_name  */
#line 1281 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7428 "bison-chpl-lib.cpp"
    break;

  case 121: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1286 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7437 "bison-chpl-lib.cpp"
    break;

  case 122: /* import_expr: expr TAS ident_use  */
#line 1291 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 7447 "bison-chpl-lib.cpp"
    break;

  case 123: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1297 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 7457 "bison-chpl-lib.cpp"
    break;

  case 124: /* import_ls: import_expr  */
#line 1305 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7463 "bison-chpl-lib.cpp"
    break;

  case 125: /* import_ls: import_ls TCOMMA import_expr  */
#line 1306 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7469 "bison-chpl-lib.cpp"
    break;

  case 126: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1311 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7479 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1320 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7487 "bison-chpl-lib.cpp"
    break;

  case 128: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1324 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7495 "bison-chpl-lib.cpp"
    break;

  case 129: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1328 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7503 "bison-chpl-lib.cpp"
    break;

  case 130: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1332 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7511 "bison-chpl-lib.cpp"
    break;

  case 131: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1336 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7519 "bison-chpl-lib.cpp"
    break;

  case 132: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1340 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7527 "bison-chpl-lib.cpp"
    break;

  case 133: /* opt_label_ident: %empty  */
#line 1348 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7533 "bison-chpl-lib.cpp"
    break;

  case 134: /* opt_label_ident: TIDENT  */
#line 1349 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7539 "bison-chpl-lib.cpp"
    break;

  case 142: /* ident_use: TIDENT  */
#line 1374 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7545 "bison-chpl-lib.cpp"
    break;

  case 143: /* ident_use: TTHIS  */
#line 1375 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7551 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TBOOL  */
#line 1402 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7557 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TENUM  */
#line 1403 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7563 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TINT  */
#line 1404 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7569 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TUINT  */
#line 1405 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7575 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TREAL  */
#line 1406 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7581 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TIMAG  */
#line 1407 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7587 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TCOMPLEX  */
#line 1408 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7593 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TBYTES  */
#line 1409 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7599 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TSTRING  */
#line 1410 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7605 "bison-chpl-lib.cpp"
    break;

  case 172: /* scalar_type: TLOCALE  */
#line 1411 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7611 "bison-chpl-lib.cpp"
    break;

  case 173: /* scalar_type: TNOTHING  */
#line 1412 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7617 "bison-chpl-lib.cpp"
    break;

  case 174: /* scalar_type: TVOID  */
#line 1413 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7623 "bison-chpl-lib.cpp"
    break;

  case 179: /* do_stmt: TDO stmt  */
#line 1427 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7629 "bison-chpl-lib.cpp"
    break;

  case 180: /* do_stmt: block_stmt  */
#line 1428 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7635 "bison-chpl-lib.cpp"
    break;

  case 181: /* return_stmt: TRETURN TSEMI  */
#line 1433 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7646 "bison-chpl-lib.cpp"
    break;

  case 182: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1440 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7657 "bison-chpl-lib.cpp"
    break;

  case 183: /* class_level_stmt: TSEMI  */
#line 1450 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7665 "bison-chpl-lib.cpp"
    break;

  case 184: /* class_level_stmt: inner_class_level_stmt  */
#line 1454 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7675 "bison-chpl-lib.cpp"
    break;

  case 185: /* $@1: %empty  */
#line 1459 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7682 "bison-chpl-lib.cpp"
    break;

  case 186: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1461 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7691 "bison-chpl-lib.cpp"
    break;

  case 187: /* $@2: %empty  */
#line 1465 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7698 "bison-chpl-lib.cpp"
    break;

  case 188: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1467 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7707 "bison-chpl-lib.cpp"
    break;

  case 196: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1485 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7716 "bison-chpl-lib.cpp"
    break;

  case 197: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1490 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7725 "bison-chpl-lib.cpp"
    break;

  case 198: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1495 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7734 "bison-chpl-lib.cpp"
    break;

  case 199: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1500 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7742 "bison-chpl-lib.cpp"
    break;

  case 200: /* forwarding_decl_start: TFORWARDING  */
#line 1507 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7751 "bison-chpl-lib.cpp"
    break;

  case 201: /* extern_or_export: TEXTERN  */
#line 1514 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7757 "bison-chpl-lib.cpp"
    break;

  case 202: /* extern_or_export: TEXPORT  */
#line 1515 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7763 "bison-chpl-lib.cpp"
    break;

  case 203: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1520 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7773 "bison-chpl-lib.cpp"
    break;

  case 204: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1530 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7782 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1536 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7793 "bison-chpl-lib.cpp"
    break;

  case 206: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1543 "chpl.ypp"
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
#line 7815 "bison-chpl-lib.cpp"
    break;

  case 207: /* $@3: %empty  */
#line 1561 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7823 "bison-chpl-lib.cpp"
    break;

  case 208: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1565 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7832 "bison-chpl-lib.cpp"
    break;

  case 209: /* $@4: %empty  */
#line 1570 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7841 "bison-chpl-lib.cpp"
    break;

  case 210: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1575 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7850 "bison-chpl-lib.cpp"
    break;

  case 211: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1583 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7858 "bison-chpl-lib.cpp"
    break;

  case 212: /* no_loop_attributes: %empty  */
#line 1588 "chpl.ypp"
                    { context->loopAttributes.push_back(nullptr); }
#line 7864 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1591 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7870 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1592 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7876 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1597 "chpl.ypp"
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
#line 7899 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1616 "chpl.ypp"
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
#line 7917 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1630 "chpl.ypp"
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
#line 7935 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt_base: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1644 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7943 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1648 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7951 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1652 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7959 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: TFOR expr TIN expr do_stmt  */
#line 1656 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7967 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: TFOR expr TIN zippered_iterator do_stmt  */
#line 1660 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7975 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: TFOR expr do_stmt  */
#line 1664 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7983 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: TFOR zippered_iterator do_stmt  */
#line 1668 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7991 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1672 "chpl.ypp"
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
#line 8015 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: TFORALL expr TIN expr do_stmt  */
#line 1692 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8023 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1696 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8031 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1700 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8039 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1704 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8047 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: TFORALL expr do_stmt  */
#line 1708 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8055 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: TFORALL expr forall_intent_clause do_stmt  */
#line 1712 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8063 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TFORALL zippered_iterator do_stmt  */
#line 1716 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8071 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1720 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8079 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TFOREACH expr TIN expr do_stmt  */
#line 1724 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8087 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1728 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8095 "bison-chpl-lib.cpp"
    break;

  case 236: /* loop_stmt_base: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1732 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8103 "bison-chpl-lib.cpp"
    break;

  case 237: /* loop_stmt_base: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1736 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8111 "bison-chpl-lib.cpp"
    break;

  case 238: /* loop_stmt_base: TFOREACH expr do_stmt  */
#line 1740 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8119 "bison-chpl-lib.cpp"
    break;

  case 239: /* loop_stmt_base: TFOREACH expr forall_intent_clause do_stmt  */
#line 1744 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8127 "bison-chpl-lib.cpp"
    break;

  case 240: /* loop_stmt_base: TFOREACH zippered_iterator do_stmt  */
#line 1748 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8135 "bison-chpl-lib.cpp"
    break;

  case 241: /* loop_stmt_base: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1752 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8143 "bison-chpl-lib.cpp"
    break;

  case 242: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1756 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8151 "bison-chpl-lib.cpp"
    break;

  case 243: /* loop_stmt_base: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1760 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8159 "bison-chpl-lib.cpp"
    break;

  case 244: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1764 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8167 "bison-chpl-lib.cpp"
    break;

  case 245: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1768 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8175 "bison-chpl-lib.cpp"
    break;

  case 246: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1772 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8183 "bison-chpl-lib.cpp"
    break;

  case 247: /* loop_stmt_base: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1776 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8191 "bison-chpl-lib.cpp"
    break;

  case 248: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1780 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8200 "bison-chpl-lib.cpp"
    break;

  case 249: /* loop_stmt_base: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1785 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8209 "bison-chpl-lib.cpp"
    break;

  case 250: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1793 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 8219 "bison-chpl-lib.cpp"
    break;

  case 251: /* if_stmt: TIF expr TTHEN stmt  */
#line 1802 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8227 "bison-chpl-lib.cpp"
    break;

  case 252: /* if_stmt: TIF expr block_stmt  */
#line 1806 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8235 "bison-chpl-lib.cpp"
    break;

  case 253: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1810 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8243 "bison-chpl-lib.cpp"
    break;

  case 254: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1814 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8251 "bison-chpl-lib.cpp"
    break;

  case 255: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1818 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8259 "bison-chpl-lib.cpp"
    break;

  case 256: /* if_stmt: TIF ifvar block_stmt  */
#line 1822 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8267 "bison-chpl-lib.cpp"
    break;

  case 257: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1826 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8275 "bison-chpl-lib.cpp"
    break;

  case 258: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1830 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8283 "bison-chpl-lib.cpp"
    break;

  case 259: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1834 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8292 "bison-chpl-lib.cpp"
    break;

  case 260: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1839 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8301 "bison-chpl-lib.cpp"
    break;

  case 261: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1844 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8310 "bison-chpl-lib.cpp"
    break;

  case 262: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1849 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8319 "bison-chpl-lib.cpp"
    break;

  case 263: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1857 "chpl.ypp"
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
#line 8338 "bison-chpl-lib.cpp"
    break;

  case 264: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1872 "chpl.ypp"
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
#line 8357 "bison-chpl-lib.cpp"
    break;

  case 265: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1890 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8365 "bison-chpl-lib.cpp"
    break;

  case 266: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1894 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8373 "bison-chpl-lib.cpp"
    break;

  case 267: /* ifc_formal_ls: ifc_formal  */
#line 1900 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8379 "bison-chpl-lib.cpp"
    break;

  case 268: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1901 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8385 "bison-chpl-lib.cpp"
    break;

  case 269: /* ifc_formal: ident_def  */
#line 1906 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8391 "bison-chpl-lib.cpp"
    break;

  case 282: /* implements_type_ident: implements_type_error_ident  */
#line 1924 "chpl.ypp"
  {
    context->syntax((yyloc), "type '%s' cannot implement an interface.", (yyvsp[0].uniqueStr).c_str());
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8400 "bison-chpl-lib.cpp"
    break;

  case 289: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1950 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8408 "bison-chpl-lib.cpp"
    break;

  case 290: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1954 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 8416 "bison-chpl-lib.cpp"
    break;

  case 291: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1958 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8424 "bison-chpl-lib.cpp"
    break;

  case 292: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1965 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8432 "bison-chpl-lib.cpp"
    break;

  case 293: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1969 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 8440 "bison-chpl-lib.cpp"
    break;

  case 294: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1973 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8448 "bison-chpl-lib.cpp"
    break;

  case 295: /* try_stmt: TTRY tryable_stmt  */
#line 1980 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8456 "bison-chpl-lib.cpp"
    break;

  case 296: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1984 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8464 "bison-chpl-lib.cpp"
    break;

  case 297: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1988 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8472 "bison-chpl-lib.cpp"
    break;

  case 298: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1992 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8480 "bison-chpl-lib.cpp"
    break;

  case 299: /* catch_expr_ls: %empty  */
#line 1998 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8486 "bison-chpl-lib.cpp"
    break;

  case 300: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1999 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8492 "bison-chpl-lib.cpp"
    break;

  case 301: /* catch_expr: TCATCH block_stmt  */
#line 2004 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8500 "bison-chpl-lib.cpp"
    break;

  case 302: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2008 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8508 "bison-chpl-lib.cpp"
    break;

  case 303: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2012 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8516 "bison-chpl-lib.cpp"
    break;

  case 304: /* catch_expr_inner: ident_def  */
#line 2019 "chpl.ypp"
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
#line 8533 "bison-chpl-lib.cpp"
    break;

  case 305: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2032 "chpl.ypp"
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
#line 8550 "bison-chpl-lib.cpp"
    break;

  case 306: /* throw_stmt: TTHROW expr TSEMI  */
#line 2048 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8560 "bison-chpl-lib.cpp"
    break;

  case 307: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2056 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8568 "bison-chpl-lib.cpp"
    break;

  case 308: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2060 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8578 "bison-chpl-lib.cpp"
    break;

  case 309: /* when_stmt_ls: %empty  */
#line 2068 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8584 "bison-chpl-lib.cpp"
    break;

  case 310: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2069 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8590 "bison-chpl-lib.cpp"
    break;

  case 311: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2074 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8598 "bison-chpl-lib.cpp"
    break;

  case 312: /* when_stmt: TOTHERWISE stmt  */
#line 2078 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8607 "bison-chpl-lib.cpp"
    break;

  case 313: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2083 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 8617 "bison-chpl-lib.cpp"
    break;

  case 314: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2092 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8626 "bison-chpl-lib.cpp"
    break;

  case 315: /* manager_expr: expr TAS ident_def  */
#line 2097 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8634 "bison-chpl-lib.cpp"
    break;

  case 316: /* manager_expr: expr  */
#line 2101 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8642 "bison-chpl-lib.cpp"
    break;

  case 317: /* manager_expr_ls: manager_expr  */
#line 2107 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8648 "bison-chpl-lib.cpp"
    break;

  case 318: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2108 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8654 "bison-chpl-lib.cpp"
    break;

  case 319: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2113 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8662 "bison-chpl-lib.cpp"
    break;

  case 320: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2122 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8671 "bison-chpl-lib.cpp"
    break;

  case 321: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2127 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8682 "bison-chpl-lib.cpp"
    break;

  case 322: /* class_start: class_tag ident_def  */
#line 2138 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8690 "bison-chpl-lib.cpp"
    break;

  case 323: /* class_tag: TCLASS  */
#line 2144 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8696 "bison-chpl-lib.cpp"
    break;

  case 324: /* class_tag: TRECORD  */
#line 2145 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8702 "bison-chpl-lib.cpp"
    break;

  case 325: /* class_tag: TUNION  */
#line 2146 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8708 "bison-chpl-lib.cpp"
    break;

  case 326: /* opt_inherit: %empty  */
#line 2150 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8714 "bison-chpl-lib.cpp"
    break;

  case 327: /* opt_inherit: TCOLON expr_ls  */
#line 2151 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8720 "bison-chpl-lib.cpp"
    break;

  case 328: /* class_level_stmt_ls: %empty  */
#line 2155 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8729 "bison-chpl-lib.cpp"
    break;

  case 329: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2160 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8737 "bison-chpl-lib.cpp"
    break;

  case 330: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2164 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8745 "bison-chpl-lib.cpp"
    break;

  case 331: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2168 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8753 "bison-chpl-lib.cpp"
    break;

  case 332: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2172 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8761 "bison-chpl-lib.cpp"
    break;

  case 333: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2179 "chpl.ypp"
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
#line 8782 "bison-chpl-lib.cpp"
    break;

  case 334: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2196 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8796 "bison-chpl-lib.cpp"
    break;

  case 335: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2209 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8804 "bison-chpl-lib.cpp"
    break;

  case 336: /* enum_ls: enum_item  */
#line 2216 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8813 "bison-chpl-lib.cpp"
    break;

  case 337: /* enum_ls: enum_ls TCOMMA  */
#line 2221 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8823 "bison-chpl-lib.cpp"
    break;

  case 338: /* $@5: %empty  */
#line 2227 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8832 "bison-chpl-lib.cpp"
    break;

  case 339: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2232 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8841 "bison-chpl-lib.cpp"
    break;

  case 340: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2237 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8850 "bison-chpl-lib.cpp"
    break;

  case 341: /* $@6: %empty  */
#line 2242 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8859 "bison-chpl-lib.cpp"
    break;

  case 342: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2247 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8868 "bison-chpl-lib.cpp"
    break;

  case 343: /* enum_item: ident_def  */
#line 2257 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8879 "bison-chpl-lib.cpp"
    break;

  case 344: /* enum_item: ident_def TASSIGN expr  */
#line 2264 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8892 "bison-chpl-lib.cpp"
    break;

  case 345: /* lambda_decl_start: TLAMBDA  */
#line 2276 "chpl.ypp"
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
#line 8908 "bison-chpl-lib.cpp"
    break;

  case 346: /* $@7: %empty  */
#line 2293 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8918 "bison-chpl-lib.cpp"
    break;

  case 347: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2299 "chpl.ypp"
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
#line 8940 "bison-chpl-lib.cpp"
    break;

  case 349: /* linkage_spec: linkage_spec_empty  */
#line 2322 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8946 "bison-chpl-lib.cpp"
    break;

  case 350: /* linkage_spec: TINLINE  */
#line 2323 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8953 "bison-chpl-lib.cpp"
    break;

  case 351: /* linkage_spec: TOVERRIDE  */
#line 2325 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8960 "bison-chpl-lib.cpp"
    break;

  case 352: /* opt_fn_type_formal_ls: %empty  */
#line 2330 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8966 "bison-chpl-lib.cpp"
    break;

  case 353: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2331 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8972 "bison-chpl-lib.cpp"
    break;

  case 354: /* fn_type_formal_ls: fn_type_formal  */
#line 2335 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8978 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2336 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8984 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_type_formal: named_formal  */
#line 2341 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8990 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2344 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8996 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_type_formal: formal_type  */
#line 2346 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 9002 "bison-chpl-lib.cpp"
    break;

  case 359: /* opt_fn_type_ret_type: %empty  */
#line 2350 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 9008 "bison-chpl-lib.cpp"
    break;

  case 360: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2351 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9014 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2364 "chpl.ypp"
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
#line 9033 "bison-chpl-lib.cpp"
    break;

  case 362: /* $@8: %empty  */
#line 2382 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 9043 "bison-chpl-lib.cpp"
    break;

  case 363: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2388 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 9056 "bison-chpl-lib.cpp"
    break;

  case 364: /* $@9: %empty  */
#line 2397 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 9066 "bison-chpl-lib.cpp"
    break;

  case 365: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2403 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 9079 "bison-chpl-lib.cpp"
    break;

  case 366: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2415 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 9087 "bison-chpl-lib.cpp"
    break;

  case 367: /* $@10: %empty  */
#line 2424 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();

    // May never have been built if there was a syntax error.
    if ((yyvsp[-4].functionParts).errorExpr == nullptr) {
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name->name());
    }
  }
#line 9101 "bison-chpl-lib.cpp"
    break;

  case 368: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2434 "chpl.ypp"
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
#line 9124 "bison-chpl-lib.cpp"
    break;

  case 369: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2456 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9136 "bison-chpl-lib.cpp"
    break;

  case 370: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2464 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9148 "bison-chpl-lib.cpp"
    break;

  case 371: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2472 "chpl.ypp"
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
#line 9163 "bison-chpl-lib.cpp"
    break;

  case 372: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2483 "chpl.ypp"
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
#line 9178 "bison-chpl-lib.cpp"
    break;

  case 373: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2494 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 9188 "bison-chpl-lib.cpp"
    break;

  case 374: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2503 "chpl.ypp"
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
#line 9204 "bison-chpl-lib.cpp"
    break;

  case 376: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2518 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 9210 "bison-chpl-lib.cpp"
    break;

  case 379: /* fn_ident: ident_def TBANG  */
#line 2525 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 9220 "bison-chpl-lib.cpp"
    break;

  case 419: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2582 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 9226 "bison-chpl-lib.cpp"
    break;

  case 420: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2583 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9232 "bison-chpl-lib.cpp"
    break;

  case 421: /* opt_formal_ls: %empty  */
#line 2587 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 9238 "bison-chpl-lib.cpp"
    break;

  case 422: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2588 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9244 "bison-chpl-lib.cpp"
    break;

  case 423: /* req_formal_ls: TLP TRP  */
#line 2592 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 9250 "bison-chpl-lib.cpp"
    break;

  case 424: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2593 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9256 "bison-chpl-lib.cpp"
    break;

  case 425: /* formal_ls_inner: formal  */
#line 2597 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9262 "bison-chpl-lib.cpp"
    break;

  case 426: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2598 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9268 "bison-chpl-lib.cpp"
    break;

  case 427: /* formal_ls: %empty  */
#line 2602 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 9274 "bison-chpl-lib.cpp"
    break;

  case 428: /* formal_ls: formal_ls_inner  */
#line 2603 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9280 "bison-chpl-lib.cpp"
    break;

  case 432: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2617 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9288 "bison-chpl-lib.cpp"
    break;

  case 433: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2622 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 9296 "bison-chpl-lib.cpp"
    break;

  case 434: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2627 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9304 "bison-chpl-lib.cpp"
    break;

  case 435: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2632 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 9312 "bison-chpl-lib.cpp"
    break;

  case 436: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2637 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9320 "bison-chpl-lib.cpp"
    break;

  case 437: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2642 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 9328 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_formal_intent_tag: %empty  */
#line 2648 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 9337 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_formal_intent_tag: required_intent_tag  */
#line 2653 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 9346 "bison-chpl-lib.cpp"
    break;

  case 440: /* required_intent_tag: TIN  */
#line 2660 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 9352 "bison-chpl-lib.cpp"
    break;

  case 441: /* required_intent_tag: TINOUT  */
#line 2661 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 9358 "bison-chpl-lib.cpp"
    break;

  case 442: /* required_intent_tag: TOUT  */
#line 2662 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 9364 "bison-chpl-lib.cpp"
    break;

  case 443: /* required_intent_tag: TCONST TIN  */
#line 2663 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 9370 "bison-chpl-lib.cpp"
    break;

  case 444: /* required_intent_tag: TCONST TREF  */
#line 2664 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9376 "bison-chpl-lib.cpp"
    break;

  case 445: /* required_intent_tag: TCONST  */
#line 2665 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9382 "bison-chpl-lib.cpp"
    break;

  case 446: /* required_intent_tag: TPARAM  */
#line 2666 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9388 "bison-chpl-lib.cpp"
    break;

  case 447: /* required_intent_tag: TREF  */
#line 2667 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9394 "bison-chpl-lib.cpp"
    break;

  case 448: /* required_intent_tag: TTYPE  */
#line 2668 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9400 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_this_intent_tag: %empty  */
#line 2672 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9406 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_this_intent_tag: TPARAM  */
#line 2673 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9412 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_this_intent_tag: TREF  */
#line 2674 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9418 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_this_intent_tag: TCONST TREF  */
#line 2675 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9424 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_this_intent_tag: TCONST  */
#line 2676 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9430 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_this_intent_tag: TTYPE  */
#line 2677 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9436 "bison-chpl-lib.cpp"
    break;

  case 455: /* proc_iter_or_op: TPROC  */
#line 2681 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9442 "bison-chpl-lib.cpp"
    break;

  case 456: /* proc_iter_or_op: TITER  */
#line 2682 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9448 "bison-chpl-lib.cpp"
    break;

  case 457: /* proc_iter_or_op: TOPERATOR  */
#line 2683 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9454 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_ret_tag: %empty  */
#line 2687 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9460 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_ret_tag: TOUT  */
#line 2688 "chpl.ypp"
                    { (yyval.returnTag) = Function::OUT; }
#line 9466 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_ret_tag: TCONST  */
#line 2689 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 9472 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_ret_tag: TCONST TREF  */
#line 2690 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 9478 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_ret_tag: TREF  */
#line 2691 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 9484 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_ret_tag: TPARAM  */
#line 2692 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 9490 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_ret_tag: TTYPE  */
#line 2693 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 9496 "bison-chpl-lib.cpp"
    break;

  case 465: /* opt_throws_error: %empty  */
#line 2697 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9502 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_throws_error: TTHROWS  */
#line 2698 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9508 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_function_body_stmt: TSEMI  */
#line 2701 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9514 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_function_body_stmt: function_body_stmt  */
#line 2702 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9520 "bison-chpl-lib.cpp"
    break;

  case 469: /* function_body_stmt: block_stmt_body  */
#line 2706 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9526 "bison-chpl-lib.cpp"
    break;

  case 470: /* function_body_stmt: TDO toplevel_stmt  */
#line 2707 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 9532 "bison-chpl-lib.cpp"
    break;

  case 471: /* function_body_stmt: return_stmt  */
#line 2708 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt), &((yylsp[0]))); }
#line 9538 "bison-chpl-lib.cpp"
    break;

  case 472: /* query_expr: TQUERIEDIDENT  */
#line 2712 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9544 "bison-chpl-lib.cpp"
    break;

  case 473: /* opt_lifetime_where: %empty  */
#line 2717 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9550 "bison-chpl-lib.cpp"
    break;

  case 474: /* opt_lifetime_where: TWHERE expr  */
#line 2719 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9556 "bison-chpl-lib.cpp"
    break;

  case 475: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2721 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9562 "bison-chpl-lib.cpp"
    break;

  case 476: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2723 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9568 "bison-chpl-lib.cpp"
    break;

  case 477: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2725 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9574 "bison-chpl-lib.cpp"
    break;

  case 478: /* lifetime_components_expr: lifetime_expr  */
#line 2730 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9580 "bison-chpl-lib.cpp"
    break;

  case 479: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2732 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9586 "bison-chpl-lib.cpp"
    break;

  case 480: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2737 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9592 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2739 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9598 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2741 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9604 "bison-chpl-lib.cpp"
    break;

  case 483: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2743 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9610 "bison-chpl-lib.cpp"
    break;

  case 484: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2745 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9616 "bison-chpl-lib.cpp"
    break;

  case 485: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2747 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9622 "bison-chpl-lib.cpp"
    break;

  case 486: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2749 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9628 "bison-chpl-lib.cpp"
    break;

  case 487: /* lifetime_ident: TIDENT  */
#line 2753 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9634 "bison-chpl-lib.cpp"
    break;

  case 488: /* lifetime_ident: TTHIS  */
#line 2754 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9640 "bison-chpl-lib.cpp"
    break;

  case 489: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2759 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9649 "bison-chpl-lib.cpp"
    break;

  case 490: /* type_alias_decl_stmt_start: TTYPE  */
#line 2768 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9657 "bison-chpl-lib.cpp"
    break;

  case 491: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2772 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9666 "bison-chpl-lib.cpp"
    break;

  case 492: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2780 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9674 "bison-chpl-lib.cpp"
    break;

  case 493: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2784 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9682 "bison-chpl-lib.cpp"
    break;

  case 494: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2791 "chpl.ypp"
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
#line 9704 "bison-chpl-lib.cpp"
    break;

  case 495: /* opt_init_type: %empty  */
#line 2811 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9710 "bison-chpl-lib.cpp"
    break;

  case 496: /* opt_init_type: TASSIGN expr  */
#line 2813 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9716 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_type: TPARAM  */
#line 2817 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9722 "bison-chpl-lib.cpp"
    break;

  case 498: /* var_decl_type: TCONST TREF  */
#line 2818 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9728 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_type: TREF  */
#line 2819 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9734 "bison-chpl-lib.cpp"
    break;

  case 500: /* var_decl_type: TCONST  */
#line 2820 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9740 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_type: TVAR  */
#line 2821 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9746 "bison-chpl-lib.cpp"
    break;

  case 502: /* $@11: %empty  */
#line 2826 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9755 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2830 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9764 "bison-chpl-lib.cpp"
    break;

  case 504: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2835 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9773 "bison-chpl-lib.cpp"
    break;

  case 505: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2843 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9781 "bison-chpl-lib.cpp"
    break;

  case 506: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2847 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9789 "bison-chpl-lib.cpp"
    break;

  case 507: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2854 "chpl.ypp"
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
#line 9811 "bison-chpl-lib.cpp"
    break;

  case 508: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2872 "chpl.ypp"
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
#line 9831 "bison-chpl-lib.cpp"
    break;

  case 509: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2891 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9839 "bison-chpl-lib.cpp"
    break;

  case 510: /* tuple_var_decl_component: ident_def  */
#line 2895 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9847 "bison-chpl-lib.cpp"
    break;

  case 511: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2899 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9855 "bison-chpl-lib.cpp"
    break;

  case 512: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2906 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9861 "bison-chpl-lib.cpp"
    break;

  case 513: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2908 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9867 "bison-chpl-lib.cpp"
    break;

  case 514: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2910 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9873 "bison-chpl-lib.cpp"
    break;

  case 515: /* opt_init_expr: %empty  */
#line 2916 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9879 "bison-chpl-lib.cpp"
    break;

  case 516: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2917 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9885 "bison-chpl-lib.cpp"
    break;

  case 517: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2918 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9891 "bison-chpl-lib.cpp"
    break;

  case 518: /* formal_or_ret_type_expr: expr  */
#line 2922 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9897 "bison-chpl-lib.cpp"
    break;

  case 519: /* ret_type: formal_or_ret_type_expr  */
#line 2926 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9903 "bison-chpl-lib.cpp"
    break;

  case 520: /* ret_type: reserved_type_ident_use  */
#line 2927 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9909 "bison-chpl-lib.cpp"
    break;

  case 521: /* ret_type: error  */
#line 2928 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9915 "bison-chpl-lib.cpp"
    break;

  case 522: /* colon_ret_type: TCOLON ret_type  */
#line 2932 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9921 "bison-chpl-lib.cpp"
    break;

  case 523: /* colon_ret_type: error  */
#line 2933 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9927 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_ret_type: %empty  */
#line 2937 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9933 "bison-chpl-lib.cpp"
    break;

  case 526: /* opt_type: %empty  */
#line 2942 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9939 "bison-chpl-lib.cpp"
    break;

  case 527: /* opt_type: TCOLON expr  */
#line 2943 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9945 "bison-chpl-lib.cpp"
    break;

  case 528: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2944 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9951 "bison-chpl-lib.cpp"
    break;

  case 529: /* opt_type: error  */
#line 2945 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9957 "bison-chpl-lib.cpp"
    break;

  case 530: /* formal_type: formal_or_ret_type_expr  */
#line 2949 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9963 "bison-chpl-lib.cpp"
    break;

  case 531: /* formal_type: reserved_type_ident_use  */
#line 2950 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9969 "bison-chpl-lib.cpp"
    break;

  case 532: /* colon_formal_type: TCOLON formal_type  */
#line 2954 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9975 "bison-chpl-lib.cpp"
    break;

  case 533: /* opt_colon_formal_type: %empty  */
#line 2958 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9981 "bison-chpl-lib.cpp"
    break;

  case 534: /* opt_colon_formal_type: colon_formal_type  */
#line 2959 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9987 "bison-chpl-lib.cpp"
    break;

  case 535: /* expr_ls: expr  */
#line 2965 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9993 "bison-chpl-lib.cpp"
    break;

  case 536: /* expr_ls: expr_ls TCOMMA expr  */
#line 2966 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9999 "bison-chpl-lib.cpp"
    break;

  case 537: /* tuple_component: TUNDERSCORE  */
#line 2970 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10005 "bison-chpl-lib.cpp"
    break;

  case 538: /* tuple_component: opt_try_expr  */
#line 2971 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 10011 "bison-chpl-lib.cpp"
    break;

  case 539: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2976 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 10019 "bison-chpl-lib.cpp"
    break;

  case 540: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2980 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10027 "bison-chpl-lib.cpp"
    break;

  case 541: /* opt_actual_ls: %empty  */
#line 2986 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 10033 "bison-chpl-lib.cpp"
    break;

  case 542: /* opt_actual_ls: actual_ls  */
#line 2987 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 10039 "bison-chpl-lib.cpp"
    break;

  case 543: /* actual_ls: actual_expr  */
#line 2992 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10048 "bison-chpl-lib.cpp"
    break;

  case 544: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2997 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10058 "bison-chpl-lib.cpp"
    break;

  case 545: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3005 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10064 "bison-chpl-lib.cpp"
    break;

  case 546: /* actual_expr: opt_try_expr  */
#line 3006 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10070 "bison-chpl-lib.cpp"
    break;

  case 547: /* ident_expr: ident_use  */
#line 3010 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10076 "bison-chpl-lib.cpp"
    break;

  case 548: /* ident_expr: scalar_type  */
#line 3011 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10082 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TSINGLE expr  */
#line 3024 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10088 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3026 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10094 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3028 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10100 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3030 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10106 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 3032 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 10116 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TATOMIC expr  */
#line 3038 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10122 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TSYNC expr  */
#line 3040 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10128 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TOWNED  */
#line 3043 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10134 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TOWNED expr  */
#line 3045 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10140 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TUNMANAGED  */
#line 3047 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10146 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3049 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10152 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TSHARED  */
#line 3051 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10158 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TSHARED expr  */
#line 3053 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10164 "bison-chpl-lib.cpp"
    break;

  case 567: /* sub_type_level_expr: TBORROWED  */
#line 3055 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10170 "bison-chpl-lib.cpp"
    break;

  case 568: /* sub_type_level_expr: TBORROWED expr  */
#line 3057 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10176 "bison-chpl-lib.cpp"
    break;

  case 569: /* sub_type_level_expr: TCLASS  */
#line 3059 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10182 "bison-chpl-lib.cpp"
    break;

  case 570: /* sub_type_level_expr: TRECORD  */
#line 3061 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10188 "bison-chpl-lib.cpp"
    break;

  case 571: /* for_expr: no_loop_attributes for_expr_base  */
#line 3065 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 10194 "bison-chpl-lib.cpp"
    break;

  case 572: /* for_expr_base: TFOR expr TIN expr TDO expr  */
#line 3070 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10208 "bison-chpl-lib.cpp"
    break;

  case 573: /* for_expr_base: TFOR expr TIN zippered_iterator TDO expr  */
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
#line 10222 "bison-chpl-lib.cpp"
    break;

  case 574: /* for_expr_base: TFOR expr TDO expr  */
#line 3090 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10235 "bison-chpl-lib.cpp"
    break;

  case 575: /* for_expr_base: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 3099 "chpl.ypp"
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
#line 10254 "bison-chpl-lib.cpp"
    break;

  case 576: /* for_expr_base: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3114 "chpl.ypp"
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
#line 10273 "bison-chpl-lib.cpp"
    break;

  case 577: /* for_expr_base: TFOR expr TDO TIF expr TTHEN expr  */
#line 3129 "chpl.ypp"
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
#line 10292 "bison-chpl-lib.cpp"
    break;

  case 578: /* for_expr_base: TFORALL expr TIN expr TDO expr  */
#line 3144 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10306 "bison-chpl-lib.cpp"
    break;

  case 579: /* for_expr_base: TFORALL expr TIN zippered_iterator TDO expr  */
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
#line 10320 "bison-chpl-lib.cpp"
    break;

  case 580: /* for_expr_base: TFORALL expr TDO expr  */
#line 3164 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10333 "bison-chpl-lib.cpp"
    break;

  case 581: /* for_expr_base: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3173 "chpl.ypp"
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
#line 10352 "bison-chpl-lib.cpp"
    break;

  case 582: /* for_expr_base: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3188 "chpl.ypp"
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
#line 10371 "bison-chpl-lib.cpp"
    break;

  case 583: /* for_expr_base: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3203 "chpl.ypp"
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
#line 10390 "bison-chpl-lib.cpp"
    break;

  case 584: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3220 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 10396 "bison-chpl-lib.cpp"
    break;

  case 585: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3225 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 10404 "bison-chpl-lib.cpp"
    break;

  case 586: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3229 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 10412 "bison-chpl-lib.cpp"
    break;

  case 587: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3233 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10420 "bison-chpl-lib.cpp"
    break;

  case 588: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3237 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10428 "bison-chpl-lib.cpp"
    break;

  case 589: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3241 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10436 "bison-chpl-lib.cpp"
    break;

  case 590: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3245 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10444 "bison-chpl-lib.cpp"
    break;

  case 591: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3249 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10452 "bison-chpl-lib.cpp"
    break;

  case 592: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3256 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10466 "bison-chpl-lib.cpp"
    break;

  case 593: /* nil_expr: TNIL  */
#line 3273 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10472 "bison-chpl-lib.cpp"
    break;

  case 601: /* opt_task_intent_ls: %empty  */
#line 3291 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10478 "bison-chpl-lib.cpp"
    break;

  case 602: /* opt_task_intent_ls: task_intent_clause  */
#line 3292 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10484 "bison-chpl-lib.cpp"
    break;

  case 603: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3297 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10494 "bison-chpl-lib.cpp"
    break;

  case 604: /* task_intent_ls: intent_expr  */
#line 3305 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10500 "bison-chpl-lib.cpp"
    break;

  case 605: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3306 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10506 "bison-chpl-lib.cpp"
    break;

  case 606: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3311 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10516 "bison-chpl-lib.cpp"
    break;

  case 607: /* forall_intent_ls: intent_expr  */
#line 3319 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10522 "bison-chpl-lib.cpp"
    break;

  case 608: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3320 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10528 "bison-chpl-lib.cpp"
    break;

  case 609: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3325 "chpl.ypp"
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
#line 10546 "bison-chpl-lib.cpp"
    break;

  case 610: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3339 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10554 "bison-chpl-lib.cpp"
    break;

  case 611: /* intent_expr: expr TREDUCE ident_expr  */
#line 3343 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10562 "bison-chpl-lib.cpp"
    break;

  case 612: /* task_var_prefix: TCONST  */
#line 3349 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10568 "bison-chpl-lib.cpp"
    break;

  case 613: /* task_var_prefix: TIN  */
#line 3350 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10574 "bison-chpl-lib.cpp"
    break;

  case 614: /* task_var_prefix: TCONST TIN  */
#line 3351 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10580 "bison-chpl-lib.cpp"
    break;

  case 615: /* task_var_prefix: TREF  */
#line 3352 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10586 "bison-chpl-lib.cpp"
    break;

  case 616: /* task_var_prefix: TCONST TREF  */
#line 3353 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10592 "bison-chpl-lib.cpp"
    break;

  case 617: /* task_var_prefix: TVAR  */
#line 3354 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10598 "bison-chpl-lib.cpp"
    break;

  case 618: /* new_maybe_decorated: TNEW  */
#line 3359 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10604 "bison-chpl-lib.cpp"
    break;

  case 619: /* new_maybe_decorated: TNEW TOWNED  */
#line 3361 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10610 "bison-chpl-lib.cpp"
    break;

  case 620: /* new_maybe_decorated: TNEW TSHARED  */
#line 3363 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10616 "bison-chpl-lib.cpp"
    break;

  case 621: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3365 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10622 "bison-chpl-lib.cpp"
    break;

  case 622: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3367 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10628 "bison-chpl-lib.cpp"
    break;

  case 623: /* new_expr: new_maybe_decorated expr  */
#line 3373 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10636 "bison-chpl-lib.cpp"
    break;

  case 624: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3380 "chpl.ypp"
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
#line 10652 "bison-chpl-lib.cpp"
    break;

  case 625: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3392 "chpl.ypp"
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
#line 10668 "bison-chpl-lib.cpp"
    break;

  case 626: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3404 "chpl.ypp"
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
#line 10685 "bison-chpl-lib.cpp"
    break;

  case 627: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3417 "chpl.ypp"
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
#line 10703 "bison-chpl-lib.cpp"
    break;

  case 628: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3434 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10711 "bison-chpl-lib.cpp"
    break;

  case 629: /* range_literal_expr: expr TDOTDOT expr  */
#line 3441 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10720 "bison-chpl-lib.cpp"
    break;

  case 630: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3446 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10729 "bison-chpl-lib.cpp"
    break;

  case 631: /* range_literal_expr: expr TDOTDOT  */
#line 3451 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10738 "bison-chpl-lib.cpp"
    break;

  case 632: /* range_literal_expr: TDOTDOT expr  */
#line 3456 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10748 "bison-chpl-lib.cpp"
    break;

  case 633: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3462 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10758 "bison-chpl-lib.cpp"
    break;

  case 634: /* range_literal_expr: TDOTDOT  */
#line 3468 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10768 "bison-chpl-lib.cpp"
    break;

  case 635: /* cast_expr: expr TCOLON expr  */
#line 3498 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10776 "bison-chpl-lib.cpp"
    break;

  case 636: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3505 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10784 "bison-chpl-lib.cpp"
    break;

  case 637: /* super_expr: fn_expr  */
#line 3511 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10790 "bison-chpl-lib.cpp"
    break;

  case 641: /* expr: sub_type_level_expr TQUESTION  */
#line 3520 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10796 "bison-chpl-lib.cpp"
    break;

  case 642: /* expr: TQUESTION  */
#line 3522 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10802 "bison-chpl-lib.cpp"
    break;

  case 646: /* expr: fn_type  */
#line 3527 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10808 "bison-chpl-lib.cpp"
    break;

  case 656: /* opt_expr: %empty  */
#line 3541 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10814 "bison-chpl-lib.cpp"
    break;

  case 657: /* opt_expr: expr  */
#line 3542 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10820 "bison-chpl-lib.cpp"
    break;

  case 658: /* opt_try_expr: TTRY expr  */
#line 3546 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10826 "bison-chpl-lib.cpp"
    break;

  case 659: /* opt_try_expr: TTRYBANG expr  */
#line 3547 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10832 "bison-chpl-lib.cpp"
    break;

  case 660: /* opt_try_expr: super_expr  */
#line 3548 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10838 "bison-chpl-lib.cpp"
    break;

  case 666: /* call_base_expr: expr TBANG  */
#line 3565 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10846 "bison-chpl-lib.cpp"
    break;

  case 667: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3568 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10852 "bison-chpl-lib.cpp"
    break;

  case 670: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3575 "chpl.ypp"
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
#line 10868 "bison-chpl-lib.cpp"
    break;

  case 671: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3587 "chpl.ypp"
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
#line 10884 "bison-chpl-lib.cpp"
    break;

  case 672: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3599 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10892 "bison-chpl-lib.cpp"
    break;

  case 673: /* dot_expr: expr TDOT ident_use  */
#line 3606 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10898 "bison-chpl-lib.cpp"
    break;

  case 674: /* dot_expr: expr TDOT TTYPE  */
#line 3608 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10904 "bison-chpl-lib.cpp"
    break;

  case 675: /* dot_expr: expr TDOT TDOMAIN  */
#line 3610 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10910 "bison-chpl-lib.cpp"
    break;

  case 676: /* dot_expr: expr TDOT TLOCALE  */
#line 3612 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10916 "bison-chpl-lib.cpp"
    break;

  case 677: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3614 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10926 "bison-chpl-lib.cpp"
    break;

  case 678: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3620 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10936 "bison-chpl-lib.cpp"
    break;

  case 679: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3632 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10942 "bison-chpl-lib.cpp"
    break;

  case 680: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3634 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10950 "bison-chpl-lib.cpp"
    break;

  case 681: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3638 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10958 "bison-chpl-lib.cpp"
    break;

  case 682: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3642 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10966 "bison-chpl-lib.cpp"
    break;

  case 683: /* bool_literal: TFALSE  */
#line 3648 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10972 "bison-chpl-lib.cpp"
    break;

  case 684: /* bool_literal: TTRUE  */
#line 3649 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10978 "bison-chpl-lib.cpp"
    break;

  case 685: /* str_bytes_literal: STRINGLITERAL  */
#line 3653 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10984 "bison-chpl-lib.cpp"
    break;

  case 686: /* str_bytes_literal: BYTESLITERAL  */
#line 3654 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10990 "bison-chpl-lib.cpp"
    break;

  case 689: /* literal_expr: INTLITERAL  */
#line 3660 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10996 "bison-chpl-lib.cpp"
    break;

  case 690: /* literal_expr: REALLITERAL  */
#line 3661 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 11002 "bison-chpl-lib.cpp"
    break;

  case 691: /* literal_expr: IMAGLITERAL  */
#line 3662 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 11008 "bison-chpl-lib.cpp"
    break;

  case 692: /* literal_expr: CSTRINGLITERAL  */
#line 3663 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 11014 "bison-chpl-lib.cpp"
    break;

  case 693: /* literal_expr: TNONE  */
#line 3664 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 11020 "bison-chpl-lib.cpp"
    break;

  case 694: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3666 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11029 "bison-chpl-lib.cpp"
    break;

  case 695: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3671 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11038 "bison-chpl-lib.cpp"
    break;

  case 696: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3676 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 11047 "bison-chpl-lib.cpp"
    break;

  case 697: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3681 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 11056 "bison-chpl-lib.cpp"
    break;

  case 698: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3686 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 11065 "bison-chpl-lib.cpp"
    break;

  case 699: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3691 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 11074 "bison-chpl-lib.cpp"
    break;

  case 700: /* assoc_expr_ls: expr TALIAS expr  */
#line 3700 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 11083 "bison-chpl-lib.cpp"
    break;

  case 701: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3705 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 11093 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TPLUS expr  */
#line 3713 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11099 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TMINUS expr  */
#line 3714 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11105 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TSTAR expr  */
#line 3715 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11111 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TDIVIDE expr  */
#line 3716 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11117 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3717 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11123 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3718 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11129 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TMOD expr  */
#line 3719 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11135 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TEQUAL expr  */
#line 3720 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11141 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3721 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11147 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3722 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11153 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3723 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11159 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TLESS expr  */
#line 3724 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11165 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TGREATER expr  */
#line 3725 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11171 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TBAND expr  */
#line 3726 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11177 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TBOR expr  */
#line 3727 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11183 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TBXOR expr  */
#line 3728 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11189 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr TAND expr  */
#line 3729 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11195 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr TOR expr  */
#line 3730 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11201 "bison-chpl-lib.cpp"
    break;

  case 720: /* binary_op_expr: expr TEXP expr  */
#line 3731 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11207 "bison-chpl-lib.cpp"
    break;

  case 721: /* binary_op_expr: expr TBY expr  */
#line 3732 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11213 "bison-chpl-lib.cpp"
    break;

  case 722: /* binary_op_expr: expr TALIGN expr  */
#line 3733 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11219 "bison-chpl-lib.cpp"
    break;

  case 723: /* binary_op_expr: expr THASH expr  */
#line 3734 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11225 "bison-chpl-lib.cpp"
    break;

  case 724: /* binary_op_expr: expr TDMAPPED expr  */
#line 3735 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11231 "bison-chpl-lib.cpp"
    break;

  case 725: /* unary_op_expr: TPLUS expr  */
#line 3739 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11237 "bison-chpl-lib.cpp"
    break;

  case 726: /* unary_op_expr: TMINUS expr  */
#line 3740 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11243 "bison-chpl-lib.cpp"
    break;

  case 727: /* unary_op_expr: TMINUSMINUS expr  */
#line 3741 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11249 "bison-chpl-lib.cpp"
    break;

  case 728: /* unary_op_expr: TPLUSPLUS expr  */
#line 3742 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11255 "bison-chpl-lib.cpp"
    break;

  case 729: /* unary_op_expr: TBANG expr  */
#line 3743 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11261 "bison-chpl-lib.cpp"
    break;

  case 730: /* unary_op_expr: expr TBANG  */
#line 3744 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 11269 "bison-chpl-lib.cpp"
    break;

  case 731: /* unary_op_expr: TBNOT expr  */
#line 3747 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11275 "bison-chpl-lib.cpp"
    break;

  case 732: /* reduce_expr: expr TREDUCE expr  */
#line 3752 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11283 "bison-chpl-lib.cpp"
    break;

  case 733: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3756 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11291 "bison-chpl-lib.cpp"
    break;

  case 734: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3760 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11299 "bison-chpl-lib.cpp"
    break;

  case 735: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3764 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11307 "bison-chpl-lib.cpp"
    break;

  case 736: /* scan_expr: expr TSCAN expr  */
#line 3771 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11315 "bison-chpl-lib.cpp"
    break;

  case 737: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3775 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11323 "bison-chpl-lib.cpp"
    break;

  case 738: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3779 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11331 "bison-chpl-lib.cpp"
    break;

  case 739: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3783 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11339 "bison-chpl-lib.cpp"
    break;


#line 11343 "bison-chpl-lib.cpp"

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
