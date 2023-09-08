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
  YYSYMBOL_loop_stmt_base = 242,           /* loop_stmt_base  */
  YYSYMBOL_zippered_iterator = 243,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 244,                  /* if_stmt  */
  YYSYMBOL_ifvar = 245,                    /* ifvar  */
  YYSYMBOL_interface_stmt = 246,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 247,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 248,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 249,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 250, /* implements_type_error_ident  */
  YYSYMBOL_implements_stmt = 251,          /* implements_stmt  */
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
#define YYLAST   20458

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  182
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  204
/* YYNRULES -- Number of rules.  */
#define YYNRULES  750
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1325

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
    1331,  1335,  1339,  1348,  1349,  1350,  1354,  1355,  1356,  1357,
    1358,  1362,  1363,  1364,  1376,  1377,  1378,  1383,  1384,  1385,
    1386,  1387,  1388,  1389,  1390,  1391,  1392,  1393,  1394,  1395,
    1396,  1397,  1398,  1399,  1400,  1401,  1405,  1406,  1407,  1408,
    1409,  1410,  1411,  1412,  1413,  1414,  1415,  1416,  1423,  1424,
    1425,  1426,  1430,  1431,  1435,  1442,  1452,  1456,  1462,  1462,
    1468,  1468,  1477,  1478,  1479,  1480,  1481,  1482,  1483,  1487,
    1492,  1497,  1502,  1509,  1517,  1518,  1522,  1531,  1537,  1545,
    1564,  1563,  1573,  1572,  1585,  1591,  1594,  1595,  1599,  1618,
    1632,  1646,  1650,  1654,  1658,  1662,  1666,  1670,  1674,  1694,
    1698,  1702,  1706,  1710,  1714,  1718,  1722,  1726,  1730,  1734,
    1738,  1742,  1746,  1750,  1754,  1758,  1762,  1766,  1770,  1774,
    1778,  1782,  1787,  1795,  1804,  1808,  1812,  1816,  1820,  1824,
    1828,  1832,  1836,  1841,  1846,  1851,  1859,  1874,  1892,  1896,
    1903,  1904,  1909,  1914,  1915,  1916,  1917,  1918,  1919,  1920,
    1921,  1922,  1923,  1924,  1925,  1926,  1935,  1936,  1937,  1938,
    1947,  1948,  1952,  1956,  1960,  1967,  1971,  1975,  1982,  1986,
    1990,  1994,  2001,  2002,  2006,  2010,  2014,  2021,  2034,  2050,
    2058,  2062,  2071,  2072,  2076,  2080,  2085,  2094,  2099,  2103,
    2110,  2111,  2115,  2124,  2129,  2140,  2147,  2148,  2149,  2153,
    2154,  2158,  2162,  2166,  2170,  2174,  2181,  2198,  2211,  2218,
    2223,  2230,  2229,  2239,  2245,  2244,  2259,  2266,  2278,  2296,
    2293,  2321,  2325,  2326,  2328,  2333,  2334,  2338,  2339,  2343,
    2346,  2348,  2353,  2354,  2365,  2385,  2384,  2400,  2399,  2417,
    2427,  2424,  2458,  2466,  2474,  2485,  2496,  2505,  2520,  2521,
    2525,  2526,  2527,  2536,  2537,  2538,  2539,  2540,  2541,  2542,
    2543,  2544,  2545,  2546,  2547,  2548,  2549,  2550,  2551,  2552,
    2553,  2554,  2555,  2556,  2557,  2558,  2559,  2560,  2564,  2565,
    2566,  2567,  2568,  2569,  2570,  2571,  2572,  2573,  2574,  2575,
    2580,  2581,  2585,  2586,  2590,  2591,  2595,  2596,  2600,  2601,
    2605,  2606,  2610,  2614,  2615,  2619,  2623,  2628,  2633,  2638,
    2643,  2651,  2655,  2663,  2664,  2665,  2666,  2667,  2668,  2669,
    2670,  2671,  2675,  2676,  2677,  2678,  2679,  2680,  2684,  2685,
    2686,  2690,  2691,  2692,  2693,  2694,  2695,  2696,  2700,  2701,
    2704,  2705,  2709,  2710,  2711,  2715,  2720,  2721,  2723,  2725,
    2727,  2732,  2734,  2739,  2741,  2743,  2745,  2747,  2749,  2751,
    2756,  2757,  2758,  2762,  2771,  2775,  2783,  2787,  2794,  2815,
    2816,  2821,  2822,  2823,  2824,  2825,  2830,  2829,  2838,  2846,
    2850,  2857,  2875,  2894,  2898,  2902,  2909,  2911,  2913,  2920,
    2921,  2922,  2926,  2930,  2931,  2932,  2936,  2937,  2941,  2942,
    2946,  2947,  2948,  2949,  2953,  2954,  2958,  2962,  2963,  2969,
    2970,  2974,  2975,  2979,  2983,  2990,  2991,  2995,  3000,  3009,
    3010,  3014,  3015,  3022,  3023,  3024,  3025,  3026,  3027,  3029,
    3031,  3033,  3035,  3041,  3043,  3046,  3048,  3050,  3052,  3054,
    3056,  3058,  3060,  3062,  3064,  3069,  3073,  3083,  3093,  3102,
    3117,  3132,  3147,  3157,  3167,  3176,  3191,  3206,  3224,  3228,
    3232,  3236,  3240,  3244,  3248,  3252,  3259,  3277,  3285,  3286,
    3287,  3288,  3289,  3290,  3291,  3295,  3296,  3300,  3309,  3310,
    3314,  3323,  3324,  3328,  3342,  3346,  3353,  3354,  3355,  3356,
    3357,  3358,  3362,  3364,  3366,  3368,  3370,  3376,  3383,  3395,
    3407,  3420,  3437,  3444,  3449,  3454,  3459,  3465,  3471,  3501,
    3508,  3515,  3516,  3520,  3522,  3523,  3525,  3527,  3528,  3529,
    3530,  3533,  3534,  3535,  3536,  3537,  3538,  3539,  3540,  3541,
    3545,  3546,  3550,  3551,  3552,  3556,  3557,  3558,  3559,  3568,
    3569,  3572,  3573,  3574,  3578,  3590,  3602,  3609,  3611,  3613,
    3615,  3617,  3623,  3636,  3637,  3641,  3645,  3652,  3653,  3657,
    3658,  3662,  3663,  3664,  3665,  3666,  3667,  3668,  3669,  3674,
    3679,  3684,  3689,  3694,  3703,  3708,  3717,  3718,  3719,  3720,
    3721,  3722,  3723,  3724,  3725,  3726,  3727,  3728,  3729,  3730,
    3731,  3732,  3733,  3734,  3735,  3736,  3737,  3738,  3739,  3743,
    3744,  3745,  3746,  3747,  3748,  3751,  3755,  3759,  3763,  3767,
    3774,  3778,  3782,  3786,  3794,  3795,  3796,  3797,  3798,  3799,
    3800
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

#define YYPACT_NINF (-1086)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-751)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1086,    69,  4534, -1086,   -61,   100, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, 13510,    -6,    68,   153, 15898,   157, 20229,
      -6,   203,   187,    27,    68,  5942, 13510,   143, 20285, -1086,
     328,   323, -1086, 11046,   333,   431,   179, -1086,   344, -1086,
   20285, 20285, 20285, -1086,   795, 12102,   394, 13510, 13510,   164,
   -1086,   404,   487, 13510, -1086, 15898, -1086, 13510,   419,   267,
     391,   456, 15242,   501, 20341, -1086, 13510,  9462, 13510, 12102,
   15898, 13510,   508,   527,   422,  5942,   560, 13510,   585,  8054,
    8054, -1086,   591, -1086, 15898, -1086,   595, 11222, -1086,   659,
   13510, -1086, 13510, -1086, -1086, 15725, 13510, -1086, 11398, -1086,
   -1086, -1086,  4886,  8582, 13510, -1086,  5590, -1086, -1086, -1086,
   -1086,  1623,   531, -1086,    42,   505, -1086,    43, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086,  8230, -1086,  8758, -1086, -1086,    66, -1086, -1086, -1086,
     624, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,   537,
   20285, -1086, 20061,   517,   -63, -1086,   234, -1086, -1086, -1086,
     375,   272, -1086, -1086, 20285,   795, -1086,   526,   528, -1086,
   -1086, -1086,   530,   532, 13510,   533,   534, -1086, -1086, -1086,
   19851,   988,   185,   535,   536, -1086, -1086,   353, -1086, -1086,
   -1086, -1086, -1086,   316, -1086, -1086, -1086, 13510, 20285, -1086,
   13510, 13510,    76,   638,   363, -1086, -1086, -1086, -1086, 19851,
     462, -1086, -1086,   541,  5942, -1086, -1086, -1086,   539,   170,
     540, -1086,   287, -1086,   543, -1086,   156, 19851,  9638, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086,   551, -1086, -1086, 20285, 20285,
     -35, 16361, -1086, -1086,   612,  9638,   550,    81,  5942,   542,
      82,    32, -1086,  5942, -1086, -1086, 17576, -1086,    19, 18092,
     265, -1086,   552,   553, -1086, 17576,   170,   265, -1086,  9638,
    2253,  2253,    46,     0, -1086,     3, -1086, -1086, -1086, -1086,
    9814,  9814, -1086,   453, -1086,   556,   559, -1086, -1086,  2332,
     563, -1086, -1086, 19851,   242, 13510, 13510, -1086,   115, -1086,
   -1086, 19851,   561, 18138, -1086, 17576,   170, 19851,   562,  9638,
   -1086, 19851, 18245, -1086, -1086, -1086, -1086, -1086,   170,   568,
   -1086,   311,   311,  1611,   265,   265,    91, -1086, -1086,  5062,
     121, -1086, 13510,   -13,   124, -1086,  1611,   728, -1086, -1086,
   20285, -1086, -1086,  4886, -1086,   102, -1086,   612, -1086, -1086,
   -1086,   737,   571,  5238, 13510, -1086, 13510,   677, -1086, -1086,
    4232,   216,   537, 19851,   581, 13510,  5942, 11574, 12278, 12278,
   11046, 11750, -1086, -1086, -1086, 20285, 13510,   574, -1086,   576,
   20117,   620,   225, -1086,   209,   375, -1086, -1086, -1086, -1086,
     658, -1086, -1086, -1086, -1086,    40,   660, -1086, -1086, -1086,
   15554,   627,   355, -1086,   364,   502, -1086,   600, 13510, 13510,
   13510, 12278, 12278, 13510,   506, 13510, 13510, 13510, 13510, 13510,
     464, 15725, 13510, 13510, 13510, 13510, 13510, 13510, 13510, 13510,
   13510, 13510, 13510, 13510, 13510, 13510, 13510, 13510,   685, -1086,
   -1086, -1086, -1086, -1086, 11222, 11222, -1086, -1086, -1086, -1086,
   11222, -1086, -1086, 11222, 11222,  9638,  9990, 12278, 12278, 18325,
     583, 13510, 13510, 12454, 20285,  8934, -1086, -1086, -1086,   795,
   -1086, 13510, -1086,   632,   586,   611, -1086, -1086, -1086,   636,
     639,  5942,   732,  5942, -1086,   733, 13510,   703,   597, 11222,
   20285, -1086, -1086, -1086,   542, -1086, -1086,   133, -1086, 13686,
     649, 13510,   795, -1086, -1086, 13510, -1086, 20173, 13510, 13510,
   -1086,   602, -1086, -1086, -1086, -1086, 17905, -1086,   375, 15417,
   -1086,   542, -1086,   631, 13686, -1086, 19851, 19851, -1086,   540,
   -1086,    48, -1086, 11222,   605, -1086,   759,   759, -1086, -1086,
   -1086, -1086, 12630, -1086, 18366,  9110, -1086,  9286, -1086, -1086,
   11574, 12278, 12278, -1086,   633, 10166, -1086,   165, -1086,  5414,
   -1086,   371, 18412,   392, 17254, 20285,  7878,  7702, -1086,   537,
     610, -1086,   287, -1086,  4319,   662, 20285,   613,    28,  2814,
      30,  3362,    30, 17086,    28, 17576, 16016,    86,    17, 18519,
     -46,   202,   637,  1943, -1086, -1086, 13510,   192, -1086, -1086,
     158, -1086, -1086,    79, -1086, -1086,  7526, -1086,   682, -1086,
     616,   640, -1086,   642, -1086,   643,   644,   646,   647,   652,
   -1086,   653,   654,   655,   656,   657,   661,   664, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   13510,   666,   668,   616, -1086,   616, -1086, 13510, -1086, 20285,
   -1086, -1086,  2333,  2333,   524, -1086,   524, -1086,   524,  3457,
     227,   570,   883,   170,   311, -1086,   510, -1086, -1086, -1086,
   -1086, -1086,  1611,  2725,   524,  3179,  3179,  2333,  3179,  3179,
     840,   311,  2725,  2103,   840,   265,   265,   311,  1611,   650,
     663,   665,   667,   669,   670,   635,   634, -1086,   524, -1086,
     524, 13510, 11222, 17652, 17737,    77,   651,    50, -1086, -1086,
   -1086,   162, -1086,  1391, 19892,   421,   416, 19851, 11222, -1086,
   11222, 13510, 13510,   788,  5942,   789, 18565,  5942, 17697, 20285,
   -1086,   176, -1086,   239, -1086,   250,   542,    82, -1086, 19851,
   10342, -1086, 19851, -1086, -1086, -1086, 20285, 18605, 18686, -1086,
     631,   678, -1086,   269, 13686, -1086,   295, -1086, 13510, -1086,
     676,     9,   681, -1086,  4062, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, 17330, 17162,    98, -1086, -1086,   282, -1086, -1086,
   -1086, -1086, 13510, -1086,    96, 16119, 13510, -1086, 12806,  7878,
    7702, -1086, 10518,   479, 10694,   482,   494, 10870,  8406,   514,
     445, -1086, -1086, -1086, 18726,   679,   671,   686, -1086, 20285,
     795, 12278,    28, 13510,   779, 13510, -1086,  6118, 12278, -1086,
     683, -1086,    28,  6294, 12278, -1086,    28, -1086,    28, 12278,
   -1086,    28, -1086, -1086, 19851,  5942,   674, 12278, 12982,  4710,
     694, 13510, 13158, -1086, -1086, 11222,   680,  2040, 19851, 20285,
     531,   538, -1086,   682, -1086, -1086, -1086, -1086, -1086,    59,
     262, -1086, 18772, -1086, 16279, -1086, -1086, 19851, -1086,   687,
     698, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,   294,
   13862, 12278, 14038, 12278, 12278, 16016, 11222, -1086, -1086,  8934,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086,    36, 12278, 12278, -1086, -1086, -1086, 19851, 19851,
    5942, -1086,  5942, 13510, -1086,  5942,   798,   675,   696, 20285,
     540, -1086, -1086,   649, -1086, -1086, -1086,   702,   704,   295,
   13510,   682,   631, -1086, 11926, -1086, -1086, 19851, -1086,  5942,
   13510, -1086, -1086, -1086, 20285, -1086,   695,   540, 12278, 12278,
   12278,  5942, 11222, -1086, -1086, -1086, -1086, -1086,   510, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086,  7878, -1086, -1086, 18879,   229, 17890,   467,
     712,  7878, -1086, 13510, -1086, -1086,  2619,   470,   484,    -6,
   18933, -1086, 18974, 13510,   296, 11046, 19851,    63, 17930,  8934,
   -1086, 11046, 19851,    53, 17415, -1086, -1086,    30, 17491, -1086,
   -1086,  5942,   116, 17975, -1086, -1086, 19851,  5942, 19851, -1086,
   19032,   307, -1086, -1086, -1086, -1086,  3675,  2075, -1086, -1086,
   20117, -1086,    59,   261, 13510, -1086,   711,   705, -1086,   616,
     616, -1086, -1086, -1086, 13510,   837, 19127, 13510,   848, 19203,
     706, 19244,   324, -1086,   649,   339,   357, -1086, -1086, 19851,
     847,  5942, -1086, -1086, -1086, -1086, -1086,  9638,  9638, -1086,
   -1086, -1086, -1086,   295, 19851, -1086,   107,    23,   710, 13510,
   -1086,    28, 17576,    30, 17491,   119, 18051, -1086,   263, 13334,
    7878,  7702,  7878,  7702, -1086, -1086, -1086, 19289, 19851,  2979,
   -1086, -1086, -1086,    28,    28, -1086, 17576, -1086, 16441,  6470,
   -1086,  6646, -1086,   359, -1086, 16521,  6822, -1086,    28,  6998,
   -1086,    28, -1086,    28, -1086,    28, -1086,  7174,   715,  7350,
     718, -1086, 13510,   720,  2075, -1086, -1086, -1086, -1086, -1086,
   -1086,   151, -1086,   -23, -1086,   444, 19365,    62, -1086, -1086,
   -1086, 19406, 14214, 14390, 19451, 14566, 14742, 14918, 15094, -1086,
   -1086,  5942, -1086,   714,   722, -1086, -1086,  5942, -1086,   540,
   19851,  5942,  5942, -1086, -1086, 19851,   495,   734,   497,   735,
   13510, -1086, -1086, -1086, -1086,  5942, 11046, 19851, 11046, 19851,
    8934, -1086,  5942, 11046, 19851, -1086, 11046, 19851, -1086, -1086,
   -1086, 11046, -1086, 19851,  5942, 11046, -1086, 19851,  5942, 19851,
   -1086, -1086,    26, -1086, 13510,   261,   151,   151,   151,   151,
     151,   151,   261,  5766, -1086, -1086, -1086, -1086, -1086, 13510,
   13510, 13510, 13510, 13510, 13510, 13510, 13510, -1086,   738,   740,
     662, -1086, -1086, -1086, -1086, -1086, 19851, 18565, 16601, 16681,
   -1086, 18565, 16761, 16841, 16921, -1086, 17001, -1086, -1086, 19851,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086,   750, -1086, 19527,
   19568, 19613, 19689, 19730, 19775, -1086, -1086,  5942,  5942,  5942,
    5942,  5942,  5942, 13510, 13510, 13510, 13510, 13510, 13510, 18565,
   18565, 18565, 18565, 18565, 18565
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   144,   475,   693,   694,   695,
     689,   690,   696,   215,   605,   133,   166,   571,   173,   573,
     605,   172,   506,   504,   133,     0,   215,   290,   167,   205,
     204,   687,   203,   215,   171,    68,   291,   353,   168,   145,
       0,     0,     0,   348,     0,   215,   175,   215,   215,   622,
     597,   697,   176,   215,   354,   565,   501,   215,     0,     0,
     190,   188,   441,   170,   574,   503,   215,   215,   215,   215,
     569,   215,     0,   174,     0,     0,   146,   215,   688,   215,
     215,   494,   169,   328,   567,   505,   177,   215,   746,     0,
     215,   748,   215,   749,   750,   638,   215,   747,   215,   646,
     186,   745,     0,   215,   215,     4,     0,     5,    10,    11,
      12,    47,    50,    51,    57,     0,    45,    71,    13,    77,
      14,    15,    16,    17,    29,   551,   552,    23,    46,   187,
     197,   215,   206,   660,   198,    18,     0,    31,    30,    20,
       0,   285,    19,   656,    22,    35,    32,    33,   196,   329,
       0,   194,     0,     0,   653,   352,     0,   650,   192,   369,
     461,   452,   648,   195,     0,     0,   193,   665,   644,   643,
     647,   555,   553,     0,   215,   654,   655,   659,   658,   657,
       0,   554,     0,   666,   667,   668,   691,   692,   649,   557,
     556,   651,   652,     0,    28,   573,   167,   215,     0,   574,
     215,   215,     0,     0,   653,   665,   553,   654,   655,   563,
     554,   666,   667,     0,     0,   606,   134,   135,     0,   572,
       0,   495,     0,   502,     0,    21,     0,   539,   215,   141,
     147,   159,   153,   152,   161,   138,   151,   162,   148,   142,
     163,   136,   164,   157,   150,   158,   156,   154,   155,   137,
     139,   149,   160,   165,   143,     0,   140,   214,     0,     0,
       0,     0,    70,    69,    71,   215,     0,     0,     0,     0,
       0,     0,   509,     0,   183,    40,     0,   320,     0,   319,
     731,   626,   623,   624,   625,     0,   566,   732,     7,   215,
     351,   351,   448,   180,   443,   181,   444,   445,   449,   450,
     179,   178,   451,   441,   535,     0,   356,   357,   359,     0,
     442,   534,   361,   522,     0,   215,   215,   184,   650,   641,
     664,   642,     0,     0,    43,     0,   570,   558,     0,   215,
      44,   564,     0,   298,    47,   302,   299,   302,   568,     0,
      56,   733,   735,   636,   730,   729,     0,    74,    78,     0,
       0,   541,   215,     0,     0,   542,   637,     0,     6,   326,
       0,   204,   327,     0,    48,     0,     9,    71,    49,    52,
      55,     0,    54,     0,   215,    72,   215,     0,   506,   202,
       0,   689,   329,   661,   210,   215,     0,   215,   215,   215,
     215,   215,   217,   575,   588,     0,   215,     0,   325,     0,
       0,   346,     0,   339,   441,   461,   459,   460,   458,   377,
     463,   462,   466,   465,   467,     0,   456,   453,   454,   457,
       0,   499,     0,   496,     0,   645,    34,   627,   215,   215,
     215,   215,   215,   215,   734,   215,   215,   215,   215,   215,
       0,   635,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   408,   415,
     416,   417,   412,   414,   215,   215,   410,   413,   411,   409,
     215,   419,   418,   215,   215,   215,   215,   215,   215,     0,
       0,   215,   215,   215,     0,   215,    36,    24,    37,     0,
      25,   215,    38,   551,     0,   546,   547,   550,   338,     0,
       0,     0,   259,     0,   408,   255,   215,     0,     0,   215,
       0,   269,    26,   513,     0,   514,   516,     0,   533,   215,
     519,   215,     0,   182,    39,   215,   322,     0,   215,   215,
      41,     0,   191,   189,   446,   447,     0,   442,   461,   441,
     434,     0,   433,   537,   215,   126,   662,   663,   367,     0,
     185,     0,    42,   215,     0,   309,   300,   301,    27,    76,
      75,    79,   215,   698,     0,   215,   683,   215,   685,     8,
     215,   215,   215,   216,    57,   215,    53,     0,    65,     0,
     124,     0,   119,     0,    86,     0,   215,   215,   199,   329,
       0,   212,     0,   209,   605,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   589,     0,     0,   539,
       0,   296,   330,     0,   337,   343,   215,   341,   336,   426,
       0,   428,   432,     0,   464,   527,     0,   529,   468,   455,
     424,   141,   404,   147,   402,   153,   152,   151,   148,   142,
     406,   163,   164,   150,   154,   137,   149,   165,   401,   383,
     386,   384,   385,   407,   396,   387,   400,   392,   390,   403,
     391,   389,   394,   399,   388,   393,   397,   398,   395,   405,
     215,   380,     0,   424,   381,   424,   378,   215,   498,     0,
     493,   508,   726,   725,   728,   737,   736,   741,   740,   722,
     719,   720,   721,   639,   709,   144,     0,   679,   680,   146,
     678,   677,   633,   713,   724,   718,   716,   727,   717,   715,
     707,   712,   714,   723,   706,   710,   711,   708,   634,     0,
       0,     0,     0,     0,     0,     0,     0,   739,   738,   743,
     742,   215,   215,     0,     0,     0,   296,   616,   617,   619,
     621,     0,   608,     0,     0,     0,     0,   540,   215,   560,
     215,   215,   215,   258,     0,   254,     0,     0,     0,     0,
     559,     0,   272,     0,   270,     0,   517,     0,   532,   531,
     215,   511,   632,   510,   321,   318,     0,     0,     0,   676,
     537,   362,   358,     0,   215,   538,   519,   360,   215,   366,
       0,     0,     0,   561,     0,   303,   699,   640,   684,   543,
     686,   544,     0,     0,     0,    58,    59,     0,    61,    63,
      67,    66,   215,   101,     0,     0,   215,    94,   215,   215,
     215,    64,   215,   383,   215,   384,   385,   215,   215,   395,
       0,   420,   421,    81,    80,    93,     0,     0,   331,     0,
       0,   215,     0,   215,     0,   215,   227,     0,   215,   226,
       0,   235,     0,     0,   215,   233,     0,   243,     0,   215,
     241,     0,   220,   219,   590,     0,     0,   215,   215,     0,
       0,   215,   215,   702,   293,   215,     0,   351,   347,     0,
       0,   441,   427,   468,   525,   524,   523,   526,   469,   476,
     441,   376,     0,   382,     0,   372,   373,   500,   497,     0,
       0,   132,   130,   131,   129,   128,   127,   674,   675,     0,
     215,   215,   215,   215,   215,   700,   215,   618,   620,   215,
     607,   166,   173,   172,   171,   168,   175,   176,   170,   174,
     169,   177,     0,   215,   215,   507,   549,   548,   267,   266,
       0,   261,     0,   215,   257,     0,   263,     0,   295,     0,
       0,   515,   518,   519,   520,   521,   317,     0,     0,   519,
     215,   468,   537,   536,   215,   437,   435,   368,   311,     0,
     215,   310,   313,   562,     0,   304,   307,     0,   215,   215,
     215,     0,   215,    60,   125,   122,   102,   114,   108,   107,
     106,   116,   103,   117,   112,   105,   113,   111,   109,   110,
     104,   115,   118,   215,   121,   120,    89,    88,    87,     0,
       0,   215,   200,   215,   201,   331,   351,     0,     0,   605,
     605,   223,     0,   215,     0,   215,   578,     0,     0,   215,
     236,   215,   584,     0,     0,   234,   244,     0,     0,   242,
     251,     0,     0,     0,   701,   249,   591,     0,   704,   703,
       0,     0,   324,   190,   188,   323,   351,   351,   332,   342,
       0,   429,   476,     0,   215,   370,   431,     0,   379,   424,
     424,   681,   682,   295,   215,     0,     0,   215,     0,     0,
       0,     0,     0,   609,   519,   665,   665,   260,   256,   596,
     262,     0,    73,   292,   271,   268,   512,   215,   215,   438,
     436,   363,   364,   519,   423,   315,     0,     0,     0,   215,
     305,     0,     0,     0,     0,     0,     0,    62,     0,   215,
     215,   215,   215,   215,    95,    98,    84,    83,    82,   351,
     207,   213,   211,     0,     0,   218,     0,   253,     0,     0,
     225,     0,   224,     0,   611,     0,     0,   231,     0,     0,
     229,     0,   239,     0,   237,     0,   252,     0,     0,     0,
       0,   250,   215,   297,   351,   334,   333,   345,   349,   490,
     491,     0,   492,   478,   481,     0,   477,     0,   425,   374,
     375,     0,   215,   215,     0,   215,   215,   215,   215,   297,
     613,     0,   265,     0,     0,   440,   439,     0,   314,     0,
     308,     0,     0,   123,    91,    90,     0,     0,     0,     0,
     215,   208,   222,   221,   228,     0,   215,   577,   215,   576,
     215,   610,     0,   215,   583,   232,   215,   582,   230,   240,
     238,   215,   247,   593,     0,   215,   245,   592,     0,   705,
     294,   335,     0,   489,   215,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   470,   472,   474,   371,   471,   215,
     215,   215,   215,   215,   215,   215,   215,   264,   628,   629,
     316,   306,    97,   100,    96,    99,    85,   581,     0,     0,
     612,   587,     0,     0,     0,   248,     0,   246,   350,   480,
     482,   483,   486,   487,   488,   484,   485,   479,   473,     0,
       0,     0,     0,     0,     0,   630,   631,     0,     0,     0,
       0,     0,     0,   215,   215,   215,   215,   215,   215,   580,
     579,   586,   585,   595,   594
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1086, -1086, -1086,    -1,   -60,  3026, -1086,   590,   799, -1086,
   -1086,   609,  -148,  -109, -1086,   337, -1086, -1086,   -69, -1086,
   -1086,    41,   673, -1086,  -539,  3170,   545,  -574, -1086,  -793,
   -1086, -1086, -1086,   104, -1086, -1086, -1086,   896, -1086,    25,
    -208, -1086, -1086,  -456,  3378, -1085,  -853, -1086, -1086,   259,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,     7, -1086,
     557,   942, -1086,   549, -1086, -1086,   -28,  1796, -1086, -1086,
   -1086, -1086,   587, -1086,   -51, -1086, -1086, -1086, -1086,   400,
   -1086, -1086, -1086,  -102, -1086,  -354,  -800, -1086, -1086, -1086,
   -1086, -1086,  -387, -1086,    89, -1086, -1086, -1086, -1086, -1086,
     387, -1086,  1485, -1086, -1086, -1086, -1086,   544, -1086, -1086,
   -1086, -1086,    33,  -406,  -173,  -780,  -915,  -621, -1086,    39,
   -1086,    52,   399,   -44,   641,   -43, -1086, -1086,  -359,  -808,
   -1086,  -301, -1086,  -117,  -306,  -295,  -624, -1086, -1086,   114,
     276, -1086,  -201,   826, -1086,  -154,   437,   194,  -271,  -757,
    -611, -1086, -1086, -1086,  -607,  -511, -1086,  -744,     8,   -96,
   -1086,  -259,  -483,  -536,    -2, -1086, -1086, -1086, -1086, -1086,
   -1086,   384, -1086,   -15, -1086, -1086,  -560, -1086,  -897, -1086,
   -1086,   414,   693, -1086, -1086, -1086, -1086,  2726, -1086,   -42,
     998, -1086,  1378,  1413, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086,  -478
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   348,   106,   755,   108,   109,   110,   365,
     366,   111,   112,   113,   114,   372,   576,   807,   808,   115,
     116,   117,   377,   118,   119,   120,   349,   835,   583,   836,
     121,   122,  1004,   580,   581,   123,   124,   218,   254,   270,
     125,   256,   126,   304,   849,   127,   128,   291,   290,   129,
     130,   131,   132,   133,   134,   592,   839,   135,   202,   137,
     392,   598,   138,   260,   139,   763,   764,   203,   141,   142,
     143,   144,   556,   795,   977,   145,   146,   791,   972,   277,
     278,   147,   148,   149,   150,   397,   877,   151,   152,   402,
     879,   880,   403,   153,   204,  1242,   155,   156,   305,   306,
     307,   961,   157,   319,   549,   788,   158,   159,  1177,   160,
     161,   672,   673,   831,   832,   833,   965,   891,   405,   620,
    1067,   621,   543,   622,   309,   537,   420,   409,   415,   889,
    1257,  1258,   162,  1065,  1173,  1174,  1175,   163,   164,   422,
     423,   678,   165,   166,   222,   271,   272,   516,   517,   771,
     311,   887,   627,   628,   520,   312,   785,   786,   350,   353,
     354,   494,   495,   496,   205,   168,   169,   393,   170,   394,
     171,   206,   173,   214,   215,   741,   856,  1143,   742,   743,
     174,   207,   208,   177,   178,   179,   320,   180,   384,   497,
     210,   182,   211,   212,   185,   186,   187,   188,   610,   189,
     190,   191,   192,   193
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   105,   303,   369,   400,   220,   508,   745,   474,   136,
     789,   424,   830,   615,   674,   886,   883,   792,   308,   310,
     493,   489,  1083,   167,  1058,   322,   761,  1010,   590,   966,
     531,   382,   136,   787,   226,  1005,   959,   518,  1016,   809,
     852,   625,   858,   861,  1099,   339,   623,   866,   870,   790,
     370,   273,   895,   255,   896,   273,  -290,   493,  1253,  -291,
     273,   355,   273,   768,   867,   266,   267,   268,   501,     3,
     554,   216,  -528,   167,   314,  1062,   264,   167,   167,   521,
     518,   493,   136,   518,   969,  1146,   136,   136,   506,   374,
     385,   154,  1256,   534,  1253,  1139,  1244,   917,   386,   695,
     167,  -528,   872,  -602,   167,   194,   387,   388,   389,   136,
    -672,  -530,  -672,   136,   154,   213,   481,   482,   223,    67,
    1063,   493,   217,  -312,   914,  1245,   385,   970,   375,  -530,
     873,   385,  1144,  -528,   386,   565,   363,   535,   850,  1197,
    -530,   918,   570,   571,   389,   980,  -528,   387,   388,   389,
      39,   850,   367,  1102,  1169,    67,  -273,  1256,   376,  -528,
     953,   566,  -530,  1058,   154,   868,  -312,   525,   154,   154,
     885,   491,  -530,   228,   850,   398,   265,   401,  1064,   781,
     522,   971,   281,   519,  -530,  -530,   390,   626,  1195,   421,
     363,   154,   371,   869,   363,   154,  1096,   363,  -530,   363,
     699,   363,  1100,  1165,  1166,  1170,  -528,   850,  -530,  -274,
    -530,  -528,   167,  -280,   937,  1129,   725,   726,  1103,   850,
    -312,   136,   390,   480,   363,   868,   519,   390,  1254,   519,
    -530,  1126,   701,   363,   363,   837,   292,   850,   359,   548,
     850,   391,   282,   765,   870,  1009,   491,   675,  -530,   909,
    -530,   483,   363,   915,   510,  1172,   294,   194,   430,  -279,
     296,   283,   865,   559,  1169,  1120,   167,   493,   493,   562,
     783,   167,   567,   963,   981,   136,  1058,   572,   284,   589,
     136,   766,   391,   499,   500,   297,  -365,   406,   298,   292,
      58,   369,  1157,   563,   515,  1201,   430,   221,   568,   416,
     299,   493,  1121,   154,   491,   362,   881,   767,   407,   294,
     919,  1241,   434,   296,    23,  1170,   228,   431,   408,   302,
     440,   432,   492,  1280,   750,  1084,   776,  1190,  1207,    83,
    1209,   194,   882,  -344,   542,   746,   920,   810,   297,   257,
    -340,   298,   430,    58,   303,   493,  1196,   167,   561,  1101,
     948,   417,   265,   299,  1171,   431,   136,   154,   475,   432,
     476,   167,   154,   418,  -340,  1172,    56,   493,   874,   434,
     136,   167,   302,   617,   438,   875,   439,   440,    65,  -288,
     136,   443,   419,   619,   167,   255,   172,   949,   450,  -278,
     491,   840,  1051,   136,   454,   455,   456,   618,   766,   608,
    -275,   431,   410,    85,   612,   432,   477,   434,   545,   172,
     478,  1011,   438,   950,   439,   440,   175,   766,   676,   443,
     611,   770,   720,   721,   951,   401,   450,   288,   722,  1118,
     982,   723,   724,  1082,   456,  1203,  -430,   -70,   154,   175,
     289,   745,   750,   962,   491,   671,   809,   964,  1179,  1180,
    -282,   411,   154,   434,   412,   750,   983,   -70,   438,   172,
    -286,   440,   154,   172,   172,   443,   413,   695,  1073,   799,
    1137,   801,   750,  1148,  1151,   154,   -70,  1153,  1155,   303,
     292,  1163,  1158,  1160,   696,   414,   172,  -615,   674,   175,
     172,   735,  1059,   175,   175,   308,   310,   697,  1189,   167,
     294,   167,   -69,   679,   296,  -614,   -70,  1220,   136,   736,
     136,   934,   522,  -615,   262,   478,   175,   263,    39,   812,
     175,   680,   -69,   355,   493,   355,  -673,   698,  -673,   297,
     681,  -614,   298,  1221,   357,   762,  -672,   813,  -672,   515,
     816,   -69,   493,  -283,   299,   229,  1206,  1243,  1208,   532,
     533,   745,   775,  1148,  1151,  1158,  1160,  -277,   817,   230,
     231,   542,   232,   302,   522,   292,   515,   233,   699,  -748,
    1246,   -69,  -749,  -748,   700,   234,  -749,   167,   561,   842,
     804,   235,   935,  -281,  -750,   294,   136,   236,  -750,   296,
     154,   237,   154,  1011,   238,   329,   239,  1247,   172,  1248,
    1249,   430,  1250,  1251,  -745,   240,   985,   701,  -745,   328,
     821,  1012,   241,   242,   297,  1011,  -287,   298,   679,    58,
     243,   844,  1291,  1292,  1293,  1294,  1295,  1296,   175,   299,
     244,    37,   522,  1124,  -351,  -669,  1131,  -669,  1255,   245,
     246,  -289,   247,  1011,   248,  1011,   249,  -276,   302,   250,
    1132,  -284,   172,   251,   513,  -351,   252,   172,    54,   253,
     431,  1272,   340,  1274,   432,  -351,   434,   493,   154,   333,
     336,   438,    89,  1167,   440,  -671,   373,  -671,   443,  -670,
     395,  -670,   175,   899,   396,   900,  1018,   175,   334,   334,
     404,   591,  -599,   425,   484,   176,  -598,   375,   426,  -603,
    -604,  -601,  -600,  1255,   421,   487,   936,   539,   493,   490,
     544,   363,   434,   435,   485,   514,   437,   438,   176,   439,
     440,  1070,   498,   509,   443,   528,   529,   550,   955,  1057,
     538,   450,  1060,   172,   558,   553,   569,   454,   455,   456,
     574,   932,   745,   585,   575,   613,   616,   172,   614,   624,
     440,   629,   167,   677,   719,   167,   732,   172,   748,   750,
     749,   136,   751,   175,   136,   752,   754,   757,   176,   759,
     172,   760,   176,   176,   493,   770,   779,   175,   784,   793,
     794,   838,   843,   371,   947,   491,   845,   175,   888,   890,
    -144,   515,  -166,  -173,  -172,   176,  -171,  -168,   229,   176,
     175,   956,  -145,  -175,  -176,  -170,  -174,  -146,   893,   907,
     908,  -169,   230,   231,  -177,   232,   901,  1056,   894,   976,
     233,   303,   940,   942,   916,   960,  1023,  1011,   234,   902,
     303,   903,  1091,   904,   235,   905,   906,  1014,  1193,  1194,
     236,  1092,  1109,   154,   237,   167,   154,   238,   968,   239,
    1041,   167,  1052,  1024,   136,   973,  1029,  1015,   240,   881,
     136,  1071,  1093,   167,   421,   241,   242,   167,  1057,  1182,
    1047,   430,   136,   243,  1072,  1097,   136,  1098,  1125,  1178,
    1185,  1191,  1187,   244,  1199,   172,  1240,   172,  1268,   493,
     493,  1234,   245,   246,  1238,   247,  1269,   248,  1245,   249,
    1273,  1275,   250,  1305,   401,  1306,   251,   176,  1164,   252,
     364,   805,   253,  1117,   430,   175,   984,   175,   579,   671,
     224,  1094,   573,  1108,   557,   774,   782,  1069,   593,  1066,
     431,  1085,  1086,  1061,   432,   780,   154,   507,   167,   604,
     167,  1288,   154,   167,   536,  1168,  1297,   136,   369,   136,
    1290,   369,   136,  1017,   154,   898,  1056,   379,   154,   773,
     952,   176,     0,   172,     0,   506,   176,   167,   269,     0,
       0,     0,   506,   431,   762,     0,  1106,   432,  1107,   167,
       0,  1057,   434,   435,     0,   436,   437,   438,   136,   439,
     440,     0,     0,   175,   443,     0,     0,     0,     0,   976,
     181,   450,     0,     0,  1133,  1134,     0,   454,   455,   456,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   181,     0,   434,   435,     0,     0,   154,
     438,   154,   439,   440,   154,     0,     0,   443,     0,   167,
       0,     0,   176,     0,   450,   167,     0,     0,   136,     0,
     454,   455,   456,     0,   136,   369,   176,     0,   154,     0,
       0,     0,     0,     0,     0,     0,   176,     0,     0,  1056,
     154,     0,     0,   181,     0,     0,     0,   181,   181,   176,
       0,     0,     0,     0,     0,   401,     0,     0,     0,   167,
       0,     0,     0,     0,     0,     0,     0,     0,   136,     0,
     181,     0,     0,     0,   181,   506,   506,     0,     0,   506,
     506,   506,     0,   506,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,   468,   469,   470,   471,   472,     0,
     154,     0,     0,     0,     0,     0,   154,   167,   172,   167,
       0,   172,     0,     0,   167,     0,   136,   167,   136,     0,
       0,     0,     0,   136,     0,   167,   136,   167,   473,     0,
       0,  -669,     0,  -669,   136,     0,   136,     0,   175,     0,
       0,   175,     0,     0,     0,     0,     0,     0,     0,     0,
     154,     0,     0,     0,     0,     0,     0,     0,     0,   167,
       0,     0,     0,     0,   176,   167,   176,     0,   136,   167,
     167,     0,     0,     0,   136,     0,     0,     0,   136,   136,
       0,     0,   181,   167,     0,     0,     0,     0,     0,     0,
     167,     0,   136,     0,     0,     0,     0,     0,   154,   136,
     154,   172,   167,     0,     0,   154,   167,   172,   154,     0,
       0,   136,     0,     0,     0,   136,   154,     0,   154,   172,
       0,   167,  1298,   172,     0,     0,     0,     0,     0,     0,
     136,   175,     0,     0,     0,     0,   181,   175,     0,     0,
       0,   181,   176,     0,     0,     0,     0,     0,     0,   175,
     154,     0,     0,   175,     0,     0,   154,     0,     0,     0,
     154,   154,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   154,   167,   167,   167,   167,   167,
     167,   154,     0,     0,   136,   136,   136,   136,   136,   136,
       0,     0,     0,   154,   172,     0,   172,   154,     0,   172,
     600,   602,     0,   607,     0,     0,     0,     0,     0,     0,
       0,     0,   154,     0,     0,     0,     0,   181,     0,     0,
       0,     0,     0,   172,   175,     0,   175,     0,     0,   175,
       0,   181,     0,     0,     0,   172,     0,     0,     0,     0,
       0,   181,     0,   685,   687,     0,     0,     0,     0,     0,
     183,     0,     0,   175,   181,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   695,   175,   154,   154,   154,   154,
     154,   154,     0,   183,     0,     0,     0,     0,   921,     0,
       0,   922,     0,     0,     0,   184,   923,     0,     0,   727,
     729,     0,     0,     0,     0,   172,   196,     0,     0,     0,
       0,   172,     0,     0,     0,     0,   924,     0,   184,     0,
       0,     0,     0,   925,     0,    39,     0,   176,     0,     0,
     176,     0,     0,   183,   926,   175,     0,   183,   183,     0,
       0,   175,   927,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   172,     0,     0,     0,   928,
     183,     0,     0,     0,   183,     0,     0,     0,   184,     0,
       0,   929,   184,   184,     0,   699,     0,     0,     0,   181,
       0,   181,   930,     0,     0,   175,     0,     0,   931,     0,
       0,     0,     0,   600,   607,   184,     0,     0,     0,   184,
       0,     0,     0,   172,     0,   172,     0,     0,     0,     0,
     172,     0,     0,   172,     0,     0,     0,     0,     0,     0,
     176,   172,     0,   172,     0,     0,   176,     0,     0,     0,
       0,     0,   318,   175,     0,   175,     0,     0,   176,     0,
     175,     0,   176,   175,     0,     0,     0,     0,     0,     0,
       0,   175,   318,   175,     0,   172,     0,   181,     0,     0,
       0,   172,     0,     0,     0,   172,   172,     0,   318,     0,
       0,     0,   183,     0,     0,     0,     0,     0,     0,   172,
       0,     0,     0,     0,     0,   175,   172,     0,     0,     0,
       0,   175,     0,     0,     0,   175,   175,     0,   172,     0,
       0,     0,   172,     0,     0,     0,     0,   184,     0,   175,
       0,     0,     0,   176,     0,   176,   175,   172,   176,     0,
       0,     0,   430,     0,     0,   359,   183,     0,   175,    22,
      23,   183,   175,     0,     0,     0,     0,     0,   360,     0,
      29,   361,   176,     0,     0,     0,    32,   175,     0,     0,
       0,     0,     0,    37,   176,     0,  -351,     0,     0,     0,
       0,   184,     0,     0,     0,     0,   184,     0,     0,   -68,
       0,   172,   172,   172,   172,   172,   172,  -351,     0,     0,
      54,   431,    56,     0,     0,   432,    60,  -351,   -68,    61,
       0,     0,   362,   318,    65,     0,     0,     0,     0,     0,
       0,   175,   175,   175,   175,   175,   175,   183,     0,     0,
       0,     0,     0,    81,   176,     0,    83,     0,     0,    85,
     176,   183,     0,     0,     0,     0,     0,     0,     0,     0,
     318,   183,   181,   434,   435,   181,   436,   437,   438,     0,
     439,   440,   184,     0,   183,   443,     0,     0,     0,     0,
       0,   449,   450,     0,   318,   453,   184,     0,   454,   455,
     456,     0,     0,  1019,   176,     0,   184,     0,     0,   100,
    1027,     0,     0,     0,   363,     0,  1033,     0,   140,   184,
       0,  1037,     0,     0,     0,     0,     0,     0,     0,  1042,
       0,     0,     0,     0,   318,     0,     0,     0,     0,     0,
       0,   140,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   176,     0,   176,     0,     0,     0,     0,   176,
       0,     0,   176,     0,     0,   181,     0,     0,     0,     0,
     176,   181,   176,  1075,     0,  1078,  1080,     0,     0,     0,
       0,     0,     0,   181,     0,     0,     0,   181,     0,     0,
       0,   140,     0,     0,     0,   685,   727,     0,     0,   183,
       0,   183,     0,     0,   176,     0,     0,     0,     0,     0,
     176,     0,     0,     0,   176,   176,     0,     0,   140,     0,
       0,     0,   140,     0,     0,     0,     0,     0,   176,     0,
       0,     0,     0,     0,   184,   176,   184,     0,     0,     0,
    1111,  1113,  1115,     0,     0,     0,     0,   176,     0,     0,
       0,   176,     0,     0,     0,     0,     0,     0,   181,     0,
     181,     0,     0,   181,   876,     0,   176,     0,     0,   318,
     318,     0,     0,     0,     0,   318,     0,   183,   318,   318,
     318,   318,     0,     0,     0,  -331,     0,   181,     0,  -331,
    -331,     0,     0,     0,     0,     0,     0,     0,  -331,   181,
    -331,  -331,     0,     0,     0,     0,  -331,     0,     0,     0,
       0,     0,   184,  -331,   318,     0,  -331,     0,     0,     0,
     176,   176,   176,   176,   176,   176,     0,     0,     0,     0,
     140,     0,     0,     0,     0,     0,     0,  -331,     0,     0,
    -331,     0,  -331,     0,  -331,     0,  -331,  -331,     0,  -331,
       0,     0,  -331,     0,  -331,     0,     0,     0,   318,   181,
       0,     0,     0,     0,     0,   181,     0,     0,     0,     0,
     318,     0,   318,  -331,     0,     0,  -331,     0,     0,  -331,
     318,     0,   359,     0,   140,     0,    22,    23,     0,   140,
       0,     0,     0,     0,     0,   360,     0,    29,   361,     0,
       0,     0,     0,    32,  -331,     0,     0,     0,     0,   181,
      37,     0,     0,     0,     0,     0,     0,   359,     0,     0,
       0,    22,    23,     0,     0,     0,     0,     0,     0,  -331,
     360,     0,    29,   361,     0,  -331,     0,    54,    32,    56,
       0,    58,     0,  1053,     0,    37,  1054,     0,     0,   362,
       0,    65,   183,     0,   430,   183,     0,   181,     0,   181,
       0,     0,     0,     0,   181,   140,     0,   181,     0,     0,
      81,     0,    54,    83,    56,   181,    85,   181,  1053,   140,
       0,  1054,     0,     0,   362,     0,    65,   184,     0,   140,
     184,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,   140,     0,     0,    81,     0,     0,    83,   181,
       0,    85,     0,   431,     0,   181,     0,   432,     0,   181,
     181,     0,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,  1055,   181,     0,     0,    89,   318,     0,     0,
     181,     0,     0,     0,     0,   183,     0,     0,   433,     0,
       0,   183,   181,   318,     0,   318,   181,     0,     0,     0,
       0,   100,     0,   183,     0,   434,   435,   183,   436,   437,
     438,   181,   439,   440,   441,   318,   442,   443,   444,   445,
     184,   447,   448,   449,   450,   451,   184,   453,     0,     0,
     454,   455,   456,     0,     0,   359,     0,     0,   184,    22,
      23,   457,   184,     0,     0,     0,     0,     0,   360,     0,
      29,   361,     0,     0,     0,     0,    32,   140,     0,   140,
       0,     0,     0,    37,     0,   181,   181,   181,   181,   181,
     181,     0,     0,     0,     0,     0,     0,     0,   183,     0,
     183,     0,     0,   183,     0,     0,     0,     0,     0,     0,
      54,     0,    56,     0,     0,   229,     0,     0,     0,     0,
       0,     0,   362,     0,    65,     0,     0,   183,     0,   230,
     231,     0,   232,   184,     0,   184,     0,   233,   184,   183,
     318,     0,     0,    81,   430,   234,    83,     0,     0,    85,
       0,   235,     0,     0,     0,   140,     0,   236,     0,     0,
       0,   237,   184,     0,   238,     0,   239,     0,     0,     0,
       0,     0,     0,     0,   184,   240,     0,     0,     0,     0,
       0,   318,   241,   242,     0,     0,     0,     0,     0,     0,
     243,     0,     0,     0,     0,     0,     0,     0,     0,   183,
     244,     0,     0,   431,     0,   183,     0,   432,     0,   245,
     246,     0,   247,     0,   248,     0,   249,     0,     0,   250,
       0,     0,     0,   251,   540,     0,   252,     0,     0,   253,
       0,     0,     0,     0,   184,     0,     0,     0,   433,     0,
     184,     0,     0,     0,     0,     0,     0,   318,     0,   183,
       0,     0,     0,     0,     0,   434,   435,     0,   436,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,   445,
       0,   447,   448,   449,   450,   451,   452,   453,     0,     0,
     454,   455,   456,     0,   184,   541,     0,     0,     0,     0,
       0,   457,     0,     0,     0,     0,     0,   183,     0,   183,
       0,     0,     0,     0,   183,     0,     0,   183,     0,     0,
       0,     0,     0,     0,     0,   183,     0,   183,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     140,     0,   184,   140,   184,     0,     0,     0,     0,   184,
       0,     0,   184,     0,     0,     0,     0,     0,     0,   183,
     184,     0,   184,     0,     0,   183,     0,     0,     0,   183,
     183,     0,   318,   318,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   183,     0,     0,     0,     0,     0,     0,
     183,     0,     0,     0,   184,     0,     0,     0,     0,     0,
     184,     0,   183,     0,   184,   184,   183,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   184,     0,
       0,   183,     0,     0,     0,   184,     0,     0,     0,     0,
       0,   359,     0,   140,     0,    22,    23,   184,     0,   140,
       0,   184,     0,     0,   360,     0,    29,   361,     0,     0,
       0,   140,    32,     0,     0,   140,   184,     0,     0,    37,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   183,   183,   183,   183,   183,
     183,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,  1053,     0,     0,  1054,     0,     0,   362,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     184,   184,   184,   184,   184,   184,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,   140,     0,   140,   209,
       0,   140,     0,   219,     0,     0,     0,     0,     0,     0,
       0,     0,   227,     0,     0,     0,   430,     0,     0,   261,
      89,     0,     0,     0,     0,   140,     0,     0,     0,     0,
       0,   276,     0,   279,   280,     0,     0,   140,     0,   285,
       0,   286,     0,   287,     0,   100,     0,     0,   313,     0,
       0,  1130,   227,   321,   323,   325,   326,   327,     0,     0,
       0,   331,     0,   332,     0,     0,     0,     0,     0,     0,
     338,     0,     0,   321,     0,   431,   341,     0,   342,   432,
       0,   343,   344,     0,   345,     0,   428,     0,   227,   321,
     356,     0,     0,   429,     0,     0,     0,   140,     0,     0,
       0,     0,     0,   140,     0,   430,   847,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   380,     0,   383,
       0,   848,     0,     0,     0,     0,     0,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,     0,     0,   443,
     444,   445,     0,   447,   448,   449,   450,   140,     0,   453,
       0,     0,   454,   455,   456,     0,     0,     0,     0,     0,
     427,     0,     0,   457,   431,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   479,     0,     0,   331,   227,     0,     0,
       0,     0,     0,     0,     0,   140,     0,   140,     0,   433,
       0,     0,   140,     0,     0,   140,     0,     0,     0,     0,
       0,     0,     0,   140,   321,   140,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,     0,
       0,   454,   455,   456,     0,   363,     0,   140,     0,     0,
       0,   321,   457,   140,     0,     0,     0,   140,   140,     0,
       0,   359,     0,     0,     0,    22,    23,     0,     0,     0,
       0,   140,     0,     0,   360,   321,    29,   361,   140,     0,
       0,     0,    32,     0,     0,     0,   327,   331,   107,    37,
     140,     0,     0,     0,   140,     0,     0,     0,     0,     0,
       0,   546,   547,     0,     0,     0,     0,     0,     0,   140,
       0,   225,     0,     0,     0,   321,    54,     0,    56,     0,
      58,     0,  1053,     0,     0,  1054,     0,     0,   362,     0,
      65,     0,     0,     0,     0,     0,     0,     0,   564,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
     582,   330,   584,   140,   140,   140,   140,   140,   140,     0,
       0,   594,     0,   599,   601,   603,   605,   609,     0,     0,
      89,     0,   227,     0,     0,     0,     0,     0,   107,     0,
       0,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   100,     0,     0,     0,     0,
       0,  1211,     0,     0,   682,   683,   684,   686,   688,   689,
       0,   690,   691,   692,   693,   694,     0,   702,   703,   704,
     705,   706,   707,   708,   709,   710,   711,   712,   713,   714,
     715,   716,   717,   718,     0,     0,     0,     0,     0,     0,
     321,   321,     0,     0,     0,     0,   321,     0,     0,   321,
     321,   321,   321,   728,   730,     0,     0,   733,   734,   609,
     430,   744,     0,     0,     0,   274,     0,   747,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   756,
       0,     0,   758,     0,     0,   321,     0,     0,     0,   274,
     486,     0,     0,     0,     0,   769,     0,   772,     0,   335,
     337,   279,     0,     0,   777,   778,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   313,     0,     0,     0,   431,
     313,     0,     0,   432,     0,     0,     0,     0,     0,   321,
       0,   368,     0,     0,     0,     0,     0,     0,   747,     0,
       0,   321,     0,   321,   512,     0,   802,   803,   227,   523,
       0,   321,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   834,   834,     0,     0,     0,     0,     0,     0,
       0,   434,   435,     0,   436,   437,   438,     0,   439,   440,
     441,     0,   864,   443,     0,     0,     0,     0,     0,   449,
     450,     0,   878,   453,     0,     0,   454,   455,   456,     0,
       0,     0,   313,     0,     0,     0,     0,   457,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   428,   107,     0,     0,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,   107,
     488,     0,     0,   430,   853,     0,   892,     0,     0,   107,
       0,     0,     0,   897,     0,     0,     0,     0,     0,   854,
       0,     0,   595,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   275,     0,     0,     0,     0,     0,     0,
     502,   505,     0,     0,     0,     0,     0,   511,     0,     0,
       0,     0,     0,     0,     0,     0,   274,   324,   274,     0,
       0,     0,   431,     0,     0,   274,   432,   756,   321,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   321,     0,   321,   938,   939,     0,
       0,     0,     0,   850,     0,     0,     0,   433,   430,     0,
       0,     0,     0,     0,     0,   274,   321,     0,     0,     0,
       0,     0,     0,     0,   434,   435,     0,   436,   437,   438,
     313,   439,   440,   441,   967,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   753,     0,   454,
     455,   456,     0,   363,     0,     0,     0,     0,   582,     0,
     457,     0,  1006,     0,  1008,   834,   834,   431,   341,     0,
     342,   432,     0,   344,   345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1020,     0,  1022,
       0,   227,     0,  1026,  1028,     0,     0,     0,     0,  1032,
    1034,     0,     0,     0,     0,  1038,     0,     0,     0,     0,
       0,     0,     0,  1043,   747,  1046,     0,  1048,  1050,   434,
     435,   321,   436,   437,   438,   107,   439,   440,   441,     0,
     442,   443,   444,   445,     0,   447,   448,   449,   450,   451,
       0,   453,     0,     0,   454,   455,   456,     0,     0,     0,
       0,     0,     0,     0,     0,   457,  1026,  1076,  1032,  1079,
    1081,  1046,   321,     0,     0,   744,     0,     0,     0,     0,
       0,     0,     0,     0,   524,     0,   526,     0,     0,   686,
     728,     0,     0,   530,     0,     0,     0,     0,     0,  1089,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,     0,     0,
    1104,     0,     0,     0,     0,     0,   227,   359,     0,     0,
       0,    22,    23,   552,  1112,  1114,  1116,     0,   321,     0,
     360,     0,    29,   361,     0,     0,     0,     0,    32,     0,
       0,     0,     0,     0,     0,    37,     0,     0,     0,   834,
       0,     0,     0,     0,     0,     0,     0,  1127,     0,  1128,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1136,
       0,  1138,    54,     0,    56,   744,   357,  1145,  1053,     0,
       0,  1054,     0,     0,   362,     0,    65,     0,   274,   274,
     274,   274,   274,   274,   274,   274,     0,     0,     0,     0,
     941,     0,     0,   944,     0,    81,     0,     0,    83,     0,
    1176,    85,     0,     0,     0,     0,     0,     0,     0,     0,
    1181,     0,     0,  1184,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,   321,   321,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1200,     0,     0,     0,     0,
       0,   100,     0,     0,     0,  1205,   834,   834,   834,   834,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1217,     0,  1219,     0,     0,
       0,     0,  1224,   523,     0,  1227,     0,     0,     0,   523,
       0,     0,     0,  1233,     0,  1237,     0,     0,  1239,     0,
       0,  1040,     0,     0,     0,  1045,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1217,  1219,
       0,  1224,  1227,  1233,  1237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   946,     0,
       0,     0,     0,     0,     0,     0,  1276,     0,     0,     0,
       0,  1277,  1278,     0,  1279,     0,   744,     0,  1281,  1282,
       0,     0,  1283,     0,     0,     0,     0,  1284,     0,     0,
       0,  1286,     0,     0,   975,     0,  1087,     0,  1088,     0,
    1289,  1090,   274,   274,     0,     0,   846,     0,   851,   855,
     857,   860,   862,   863,     0,  1277,  1299,  1300,  1281,  1301,
    1302,  1303,  1304,     0,     0,  1105,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1045,     0,     0,
       0,     0,   274,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   274,     0,     0,     0,   274,     0,   274,     0,
       0,   274,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1319,
    1320,  1321,  1322,  1323,  1324,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   229,     0,  1156,     0,     0,
       0,     0,     0,  1161,     0,     0,     0,     0,     0,   230,
     231,     0,   232,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,     0,     0,     0,     0,
       0,   235,     0,     0,     0,     0,     0,   236,     0,     0,
       0,   237,     0,     0,   238,     0,   239,  1192,     0,     0,
    1095,     0,     0,     0,     0,   240,     0,     0,     0,     0,
       0,     0,   241,   242,     0,     0,     0,     0,     0,     0,
     243,     0,     0,     0,     0,     0,     0,  1110,     0,     0,
     244,     0,     0,     0,     0,     0,     0,     0,     0,   245,
     246,     0,   247,     0,   248,   523,   249,   523,     0,   250,
       0,     0,   523,   251,     0,   523,   252,     0,     0,   253,
       0,   855,     0,  1232,     0,  1236,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   274,   274,     0,
       0,     0,     0,   274,   274,     0,     0,   274,   274,     0,
       0,     0,     0,     0,     0,     0,     0,  1267,     0,     0,
    1021,     0,     0,  1270,     0,     0,     0,  1232,  1236,     0,
    1030,     0,     0,   363,  1035,   974,  1036,     0,     0,  1039,
       0,     0,     0,     0,   428,     0,     0,     0,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,     0,
    1285,     0,     0,   430,  1287,     0,     0,     0,   586,     0,
       0,     0,     0,     0,     0,     0,     0,   274,     0,   107,
       0,   274,   274,   274,   274,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   274,   274,   587,   274,     0,   505,     0,
       0,     0,     0,     0,     0,   505,     0,     0,   274,     0,
       0,   274,   431,   274,     0,   274,   432,     0,     0,     0,
       0,   428,     0,     0,     0,     0,     0,     0,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,     0,     0,     0,     0,     0,     0,   433,     0,     0,
       0,     0,     0,     0,     0,     0,   841,     0,     0,  1271,
       0,     0,     0,     0,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,     0,   588,   454,
     455,   456,     0,     0,     0,  1140,  1142,     0,     0,   431,
     457,  1147,  1150,   432,     0,  1152,  1154,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     213,     0,     0,     0,   433,     0,     0,     0,   505,   505,
       0,     0,   505,   505,   505,     0,   505,     0,     0,     0,
       0,   434,   435,     0,   436,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,     0,  1198,   454,   455,   456,  1140,
    1142,  1147,  1150,     0,     0,     0,     0,   457,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1212,  1213,     0,  1214,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1225,     0,     0,  1228,
       0,  1229,     0,  1230,    -2,     4,     0,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -215,    21,
      22,    23,    24,    25,    26,     0,  -215,    27,     0,    28,
       0,    29,    30,    31,  -215,  -215,  -215,    32,    33,    34,
     -68,     0,    35,    36,    37,     0,    38,  -351,    39,     0,
      40,    41,    42,    43,    44,     0,    45,    46,    47,    48,
     -68,    49,    50,     0,    51,    52,    53,     0,  -351,     0,
       0,    54,    55,    56,    57,    58,    59,    60,  -351,   -68,
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
      12,     0,  -700,     0,    13,    14,    15,    16,    17,  -700,
      18,     0,    19,    20,  -215,    21,    22,    23,    24,    25,
      26,  -700,  -215,    27,  -700,    28,     0,    29,    30,    31,
    -215,  -215,  -215,    32,    33,    34,   -68,     0,    35,    36,
      37,     0,    38,  -351,    39,     0,    40,    41,    42,    43,
      44,     0,    45,    46,    47,    48,   -68,    49,    50,     0,
      51,    52,    53,     0,  -351,     0,     0,    54,    55,    56,
      57,     0,    59,    60,  -351,   -68,    61,    62,    63,    64,
    -700,    65,    66,    67,  -700,    68,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,     0,    83,    84,   -68,    85,    86,     0,     0,
    -215,     0,    87,     0,     0,  -700,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,  -700,    92,  -700,  -700,  -700,  -700,  -700,
    -700,  -700,     0,  -700,  -700,  -700,  -700,  -700,  -700,  -700,
    -700,  -700,  -700,  -700,  -700,    99,   100,  -700,  -700,  -700,
       0,   102,  -700,   103,     0,  -215,     0,   346,  -700,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -215,    21,    22,    23,    24,    25,    26,     0,  -215,    27,
       0,    28,     0,    29,    30,    31,  -215,  -215,  -215,    32,
      33,    34,   -68,     0,    35,    36,    37,     0,    38,  -351,
      39,     0,    40,    41,    42,    43,    44,     0,    45,    46,
      47,    48,   -68,    49,    50,     0,    51,    52,    53,     0,
    -351,     0,     0,    54,    55,    56,    57,    58,    59,    60,
    -351,   -68,    61,    62,    63,    64,     0,    65,    66,    67,
       0,    68,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,     0,    83,
      84,   -68,    85,    86,     0,     0,  -215,     0,    87,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,   347,   103,
       0,  -215,     0,     4,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,  -215,    21,    22,    23,
      24,    25,    26,     0,  -215,    27,     0,    28,     0,    29,
      30,    31,  -215,  -215,  -215,    32,    33,    34,   -68,     0,
      35,    36,    37,     0,    38,  -351,    39,     0,    40,    41,
      42,    43,    44,     0,    45,    46,    47,    48,   -68,    49,
      50,     0,    51,    52,    53,     0,  -351,     0,     0,    54,
      55,    56,    57,    58,    59,    60,  -351,   -68,    61,    62,
      63,    64,     0,    65,    66,    67,     0,    68,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,     0,    83,    84,   -68,    85,    86,
       0,     0,  -215,     0,    87,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,   560,   103,     0,  -215,     0,   577,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,  -215,    21,    22,    23,    24,    25,    26,     0,
    -215,    27,     0,    28,     0,    29,    30,    31,  -215,  -215,
    -215,    32,    33,    34,   -68,     0,    35,    36,    37,     0,
      38,  -351,    39,     0,    40,    41,    42,    43,    44,     0,
      45,    46,    47,    48,   -68,    49,    50,     0,    51,    52,
      53,     0,  -351,     0,     0,    54,    55,    56,    57,    58,
      59,    60,  -351,   -68,    61,    62,    63,    64,     0,    65,
      66,    67,     0,    68,    69,    70,    71,    72,    73,    74,
      75,     0,    76,    77,     0,    78,    79,    80,    81,    82,
       0,    83,    84,   -68,    85,    86,     0,     0,  -215,     0,
      87,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
     578,   103,     0,  -215,     0,     4,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -215,    21,
      22,    23,    24,    25,    26,     0,  -215,    27,     0,    28,
       0,    29,    30,    31,  -215,  -215,  -215,    32,    33,    34,
     -68,     0,    35,    36,    37,     0,    38,  -351,    39,     0,
      40,    41,    42,    43,    44,     0,    45,    46,    47,    48,
     -68,    49,    50,     0,    51,    52,    53,     0,  -351,     0,
       0,    54,    55,    56,    57,    58,    59,    60,  -351,   -68,
      61,    62,    63,    64,     0,    65,    66,    67,     0,    68,
      69,    70,    71,    72,    73,    74,    75,     0,    76,    77,
       0,    78,    79,    80,    81,    82,     0,    83,    84,   -68,
      85,    86,     0,     0,  -215,     0,    87,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     100,     0,     0,   101,     0,   102,   811,   103,     0,  -215,
       0,     4,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -215,    21,    22,    23,    24,    25,
      26,     0,  -215,    27,     0,    28,     0,    29,    30,    31,
    -215,  -215,  -215,    32,    33,    34,   -68,     0,    35,    36,
      37,     0,    38,  -351,    39,     0,    40,    41,    42,    43,
      44,     0,    45,    46,    47,    48,   -68,    49,    50,     0,
      51,    52,    53,     0,  -351,     0,     0,    54,    55,    56,
      57,   357,    59,    60,  -351,   -68,    61,    62,    63,    64,
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
      33,    34,   -68,     0,    35,    36,    37,     0,    38,  -351,
      39,     0,    40,    41,    42,    43,    44,     0,    45,    46,
      47,    48,   -68,    49,    50,     0,    51,    52,    53,     0,
    -351,     0,     0,    54,    55,    56,    57,    58,    59,    60,
    -351,   -68,    61,    62,    63,    64,     0,    65,    66,    67,
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
      30,    31,  -215,  -215,  -215,    32,    33,    34,   -68,     0,
      35,    36,    37,     0,    38,  -351,    39,     0,    40,    41,
      42,    43,    44,     0,    45,    46,    47,    48,   -68,    49,
      50,     0,    51,    52,    53,     0,  -351,     0,     0,    54,
      55,    56,    57,     0,    59,    60,  -351,   -68,    61,    62,
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
    -215,    32,  1025,    34,   -68,     0,    35,    36,    37,     0,
      38,  -351,    39,     0,    40,    41,    42,    43,    44,     0,
      45,    46,    47,    48,   -68,    49,    50,     0,    51,    52,
      53,     0,  -351,     0,     0,    54,    55,    56,    57,     0,
      59,    60,  -351,   -68,    61,    62,    63,    64,     0,    65,
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
       0,    29,    30,    31,  -215,  -215,  -215,    32,  1031,    34,
     -68,     0,    35,    36,    37,     0,    38,  -351,    39,     0,
      40,    41,    42,    43,    44,     0,    45,    46,    47,    48,
     -68,    49,    50,     0,    51,    52,    53,     0,  -351,     0,
       0,    54,    55,    56,    57,     0,    59,    60,  -351,   -68,
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
    -215,  -215,  -215,    32,  1216,    34,   -68,     0,    35,    36,
      37,     0,    38,  -351,    39,     0,    40,    41,    42,    43,
      44,     0,    45,    46,    47,    48,   -68,    49,    50,     0,
      51,    52,    53,     0,  -351,     0,     0,    54,    55,    56,
      57,     0,    59,    60,  -351,   -68,    61,    62,    63,    64,
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
    1218,    34,   -68,     0,    35,    36,    37,     0,    38,  -351,
      39,     0,    40,    41,    42,    43,    44,     0,    45,    46,
      47,    48,   -68,    49,    50,     0,    51,    52,    53,     0,
    -351,     0,     0,    54,    55,    56,    57,     0,    59,    60,
    -351,   -68,    61,    62,    63,    64,     0,    65,    66,    67,
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
      30,    31,  -215,  -215,  -215,    32,  1223,    34,   -68,     0,
      35,    36,    37,     0,    38,  -351,    39,     0,    40,    41,
      42,    43,    44,     0,    45,    46,    47,    48,   -68,    49,
      50,     0,    51,    52,    53,     0,  -351,     0,     0,    54,
      55,    56,    57,     0,    59,    60,  -351,   -68,    61,    62,
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
    -215,    32,  1226,    34,   -68,     0,    35,    36,    37,     0,
      38,  -351,    39,     0,    40,    41,    42,    43,    44,     0,
      45,    46,    47,    48,   -68,    49,    50,     0,    51,    52,
      53,     0,  -351,     0,     0,    54,    55,    56,    57,     0,
      59,    60,  -351,   -68,    61,    62,    63,    64,     0,    65,
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
       0,    29,    30,    31,  -215,  -215,  -215,    32,  1231,    34,
     -68,     0,    35,    36,    37,     0,    38,  -351,    39,     0,
      40,    41,    42,    43,    44,     0,    45,    46,    47,    48,
     -68,    49,    50,     0,    51,    52,    53,     0,  -351,     0,
       0,    54,    55,    56,    57,     0,    59,    60,  -351,   -68,
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
    -215,  -215,  -215,    32,  1235,    34,   -68,     0,    35,    36,
      37,     0,    38,  -351,    39,     0,    40,    41,    42,    43,
      44,     0,    45,    46,    47,    48,   -68,    49,    50,     0,
      51,    52,    53,     0,  -351,     0,     0,    54,    55,    56,
      57,     0,    59,    60,  -351,   -68,    61,    62,    63,    64,
       0,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,     0,    83,    84,   -68,    85,    86,     0,     0,
    -215,     0,    87,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   100,     0,     0,   101,
       0,   102,     0,   103,     0,  -215,     0,   884,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,   293,
       0,   196,     0,     0,     0,    31,  -215,  -215,     0,     0,
     197,    34,     0,     0,     0,   295,     0,     0,    38,     0,
      39,     0,   198,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   199,     0,     0,     0,     0,
       0,     0,     0,    70,   300,    72,    73,    74,   301,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   201,     0,   103,
       0,  -215,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,   632,     0,    13,     0,     0,    16,
      17,   634,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,    39,   640,   198,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   199,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   200,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,   504,   459,
     460,   461,   462,   463,     0,     0,   466,   467,   468,   469,
       0,   471,   472,     0,   822,   823,   824,   825,   826,   653,
       0,   654,     0,    95,     0,   655,   656,   657,   658,   659,
     660,   661,   827,   663,   664,    97,   828,    99,   -92,   666,
     667,   829,   669,   201,     0,   103,     0,     0,     0,     0,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
     632,     0,    13,     0,     0,    16,    17,   634,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,    39,   640,   198,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   199,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     200,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,   504,   459,   460,   461,   462,   463,
       0,     0,   466,   467,   468,   469,     0,   471,   472,     0,
     822,   823,   824,   825,   826,   653,     0,   654,     0,    95,
       0,   655,   656,   657,   658,   659,   660,   661,   827,   663,
     664,    97,   828,    99,     0,   666,   667,   829,   669,   201,
       0,   103,     0,     0,     0,     0,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
       0,    16,    17,     0,    18,     0,   195,    20,     0,    21,
       0,     0,     0,     0,    26,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,    33,    34,
       0,     0,     0,    36,     0,     0,    38,     0,    39,     0,
     198,     0,     0,    43,    44,     0,    45,    46,    47,    48,
       0,    49,    50,     0,    51,    52,    53,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   199,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,    73,    74,    75,     0,    76,    77,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   102,     0,   103,     0,     0,
       0,     0,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,   378,    23,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,    39,     0,   198,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,    56,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   199,
       0,    65,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   200,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,    85,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   201,     0,   103,     0,     0,     0,     0,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
      39,     0,   198,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   199,  -744,     0,     0,     0,
    -744,     0,     0,    70,    71,    72,    73,    74,   200,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,  -393,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,  -393,     0,     0,   101,     0,   201,  -393,   103,
       0,     0,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,    39,     0,   198,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   199,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   200,     0,    76,     0,     0,    78,
     315,   316,     0,    82,   351,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,   352,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   201,     0,   103,     0,     0,     0,     0,
     104,     5,     6,     7,     8,     9,   381,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
      19,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,  -215,  -215,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,    39,     0,   198,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,    64,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     200,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,    83,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   201,
       0,   103,     0,  -215,     0,     0,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,   737,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,   738,     0,    36,     0,     0,    38,     0,    39,     0,
     198,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   199,     0,   739,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   200,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
     740,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   201,     0,   103,     0,     0,
       0,     0,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,    39,     0,   198,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   199,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   200,     0,    76,     0,     0,    78,   315,   316,
       0,    82,   351,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   201,     0,   103,   798,     0,     0,     0,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
      39,     0,   198,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   199,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   200,     0,
      76,     0,     0,    78,   315,   316,     0,    82,   351,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   201,     0,   103,
     800,     0,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,    39,     0,   198,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   199,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   200,     0,    76,     0,     0,    78,
     315,   316,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   317,     0,
       0,   101,     0,   201,     0,   103,     0,     0,     0,     0,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,    39,     0,   198,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   199,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     200,     0,    76,     0,     0,    78,   315,   316,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   201,
       0,   103,  -545,     0,     0,     0,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,  -215,  -215,     0,     0,   197,    34,
       0,     0,     0,    36,     0,     0,    38,     0,    39,     0,
     198,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   199,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   200,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   201,     0,   103,     0,  -215,
       0,     0,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,    39,     0,   198,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   199,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   200,     0,    76,     0,     0,    78,   315,   316,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   201,     0,   103,     0,     0,  -545,     0,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
      39,     0,   198,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   199,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   200,     0,
      76,     0,     0,    78,   315,   316,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   201,     0,   103,
     806,     0,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,    39,     0,   198,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,   954,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   199,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   200,     0,    76,     0,     0,    78,
     315,   316,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   201,     0,   103,     0,     0,     0,     0,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,    39,     0,   198,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   199,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     200,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,  -401,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,  -401,     0,     0,   101,     0,   201,
    -401,   103,     0,     0,     0,     0,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,     0,     0,    36,     0,     0,    38,     0,    39,     0,
     198,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   199,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   200,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,  -386,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
    -386,     0,     0,   101,     0,   201,  -386,   103,     0,     0,
       0,     0,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,    39,     0,   198,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   199,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   200,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,  -394,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,  -394,     0,     0,   101,
       0,   201,  -394,   103,     0,     0,     0,     0,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,   258,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
      39,     0,   198,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   199,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   200,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,   259,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   201,     0,   103,
       0,     0,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,    39,     0,   198,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   199,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   200,     0,    76,     0,     0,    78,
     315,   316,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   201,     0,   103,     0,     0,     0,     0,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,    39,     0,   198,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   199,  -744,     0,
       0,     0,  -744,     0,     0,    70,    71,    72,    73,    74,
     200,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   201,
       0,   103,     0,     0,     0,     0,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,     0,     0,    36,     0,     0,    38,     0,    39,     0,
     198,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,   596,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   199,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   200,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,   597,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   201,     0,   103,     0,     0,
       0,     0,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,    39,     0,   198,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   199,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   200,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,   597,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   201,     0,   103,     0,     0,   606,     0,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
      39,     0,   198,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   199,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   200,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,  -422,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   201,     0,   103,
    -422,     0,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,   273,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,    39,     0,   198,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   199,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   200,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   102,     0,   103,     0,     0,     0,     0,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,    39,     0,   198,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   199,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     200,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,   597,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   201,
       0,   103,     0,     0,     0,     0,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,     0,     0,    36,     0,     0,    38,     0,    39,     0,
     198,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   199,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   200,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   201,     0,   103,     0,     0,
     606,     0,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,    39,     0,   198,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   199,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   200,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   201,   796,   103,     0,     0,     0,     0,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
      39,     0,   198,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   199,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   200,     0,
      76,     0,     0,    78,     0,     0,     0,    82,  1007,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   201,     0,   103,
       0,     0,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,    39,     0,   198,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   199,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   200,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   201,     0,   103,     0,     0,  1044,     0,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,    39,     0,   198,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   199,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     200,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   201,
       0,   103,     0,     0,  1049,     0,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,     0,     0,    36,     0,     0,    38,     0,    39,     0,
     198,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   199,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   200,     0,    76,     0,
       0,    78,     0,     0,     0,    82,  1204,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   201,     0,   103,     0,     0,
       0,     0,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,    39,     0,   198,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   199,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   200,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   201,     0,   103,     0,     0,     0,     0,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,   293,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,   295,     0,     0,    38,     0,
      39,     0,   198,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   199,     0,     0,     0,     0,
       0,     0,     0,    70,   300,    72,    73,    74,   301,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   201,     0,   103,
       0,     0,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,  1074,    34,     0,     0,
       0,    36,     0,     0,    38,     0,    39,     0,   198,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   199,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   200,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   201,     0,   103,     0,     0,     0,     0,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,  1077,    34,     0,     0,     0,    36,     0,     0,
      38,     0,    39,     0,   198,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   199,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     200,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   201,
       0,   103,     0,     0,     0,     0,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,  1260,    34,
       0,     0,     0,    36,     0,     0,    38,     0,    39,     0,
     198,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   199,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   200,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   201,     0,   103,     0,     0,
       0,     0,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,  1261,    34,     0,     0,     0,    36,
       0,     0,    38,     0,    39,     0,   198,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   199,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   200,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   201,     0,   103,     0,     0,     0,     0,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
    1263,    34,     0,     0,     0,    36,     0,     0,    38,     0,
      39,     0,   198,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   199,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   200,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   201,     0,   103,
       0,     0,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,  1264,    34,     0,     0,
       0,    36,     0,     0,    38,     0,    39,     0,   198,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   199,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   200,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   201,     0,   103,     0,     0,     0,     0,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,  1265,    34,     0,     0,     0,    36,     0,     0,
      38,     0,    39,     0,   198,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   199,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     200,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   201,
       0,   103,     0,     0,     0,     0,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,  1266,    34,
       0,     0,     0,    36,     0,     0,    38,     0,    39,     0,
     198,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   199,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   200,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     6,     7,     8,     9,
      10,    11,    12,     0,    96,     0,    13,    97,    98,    99,
      17,     0,     0,   101,   195,   201,     0,   103,     0,   292,
       0,     0,   104,     0,     0,     0,     0,   196,     0,     0,
       0,     0,  -215,  -215,     0,     0,   197,     0,     0,   294,
       0,     0,     0,   296,     0,     0,     0,     0,   198,     0,
       0,    43,    44,     0,     0,     0,     0,    48,     0,    49,
      50,     0,     0,     0,     0,     0,     0,     0,   297,     0,
      55,   298,    57,    58,    59,     0,     0,     0,     0,    62,
       0,   199,     0,   299,     0,     0,     0,     0,     0,    70,
       0,    72,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,   302,     0,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   201,     0,   103,  -355,  -215,     0,     0,
     104,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,     0,    17,     0,     0,     0,   195,
       0,     0,     0,     0,   292,     0,     0,     0,     0,     0,
       0,     0,   196,     0,     0,     0,     0,  -215,  -215,     0,
       0,   197,     0,     0,   294,     0,     0,     0,   296,     0,
       0,     0,     0,   198,     0,     0,    43,    44,     0,     0,
       0,     0,    48,     0,    49,    50,     0,     0,     0,     0,
       0,     0,     0,   297,     0,    55,   298,    57,    58,    59,
       0,     0,     0,     0,    62,     0,   199,     0,   299,     0,
       0,     0,     0,     0,    70,     0,    72,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,   302,     0,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   630,     0,   631,     0,    90,
      91,    92,    93,    94,     0,     0,   632,     0,    95,     0,
       0,   633,   231,   634,   635,     0,     0,    96,     0,   636,
      97,    98,    99,     0,     0,     0,   101,   234,   201,   196,
     103,     0,  -215,   235,     0,   104,     0,     0,     0,   637,
       0,     0,     0,   237,     0,     0,   638,     0,   639,   640,
       0,     0,     0,     0,     0,     0,     0,   641,     0,     0,
       0,     0,     0,     0,   241,   642,     0,     0,     0,     0,
       0,     0,   243,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   643,     0,     0,     0,     0,     0,     0,     0,
       0,   245,   246,     0,   644,     0,   248,     0,   645,     0,
       0,   250,     0,     0,     0,   646,     0,     0,   252,     0,
       0,   647,     0,     0,     0,     0,     0,     0,     0,     0,
     504,   459,   460,   461,   462,   463,     0,     0,   466,   467,
     468,   469,     0,   471,   472,     0,   648,   649,   650,   651,
     652,   653,     0,   654,     0,     0,     0,   655,   656,   657,
     658,   659,   660,   661,   662,   663,   664,     0,   665,     0,
       0,   666,   667,   668,   669,     0,     0,   670,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,  -215,  -215,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,    39,
       0,   198,     0,     0,    43,    44,     0,     0,    46,     0,
      48,     0,    49,    50,     0,    51,    52,     0,     0,     0,
       0,     0,     0,    55,     0,    57,     0,    59,     0,     0,
       0,     0,    62,    63,   199,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   200,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,     0,    98,
      99,     0,     0,     0,   101,     0,   201,     0,   103,     0,
    -215,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,  -215,  -215,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,    39,     0,   198,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   199,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     200,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
      90,    21,    92,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,  -215,  -215,     0,     0,
     197,    34,     0,    99,     0,    36,     0,     0,    38,   201,
      39,   103,   198,  -215,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   199,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   200,     0,
      76,     0,   695,    78,     0,     0,     0,    82,     0,     0,
      84,   632,     0,    86,     0,     0,   986,   987,   634,   988,
       0,     0,     0,     0,   989,     0,     0,     0,     0,     0,
       0,     0,   697,     0,     0,     0,     0,     0,    90,     0,
      92,     0,     0,     0,   990,     0,     0,     0,   991,     0,
       0,   992,     0,    39,   640,     0,     0,     0,     0,     0,
       0,    99,   698,     0,     0,     0,     0,     0,     0,   103,
     993,  -215,     0,     0,     0,     0,     0,   994,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   995,     0,     0,
       0,     0,     0,     0,     0,     0,   996,   997,     0,   998,
       0,   999,     0,   699,     0,     0,     0,     0,     0,   700,
    1000,     0,     0,  1001,     0,     0,  1002,     0,     0,     0,
       0,     0,     0,     0,     0,   504,   459,   460,   461,   462,
     463,     0,     0,   466,   467,   468,   469,     0,   471,   472,
       0,   648,   649,   650,   651,   652,   653,     0,   654,     0,
       0,     0,   655,   656,   657,   658,   659,   660,   661,   662,
     663,   664,   229,   665,     0,     0,   666,   667,   668,   669,
    1003,   632,     0,     0,     0,     0,   230,   231,   634,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,     0,     0,     0,   235,     0,
       0,     0,     0,     0,   236,     0,     0,     0,   237,     0,
       0,   238,     0,   239,   640,     0,     0,     0,     0,     0,
       0,     0,   240,     0,     0,     0,     0,     0,     0,   241,
     242,     0,     0,     0,     0,     0,     0,   243,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   244,     0,     0,
       0,     0,     0,   428,     0,     0,   245,   246,     0,   247,
     429,   248,     0,   249,     0,     0,   250,     0,     0,     0,
     251,     0,   430,   252,     0,     0,   253,     0,     0,     0,
       0,     0,     0,     0,     0,   504,   459,   460,   461,   462,
     463,     0,     0,   466,   467,   468,   469,     0,   471,   472,
       0,   648,   649,   650,   651,   652,   653,     0,   654,     0,
       0,     0,   655,   656,   657,   658,   659,   660,   661,   662,
     663,   664,     0,   665,     0,     0,   666,   667,   668,   669,
       0,   431,     0,   428,     0,   432,     0,     0,     0,     0,
     429,     0,     0,     0,   503,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   504,   459,   460,
     461,   462,   463,     0,     0,   466,   467,   468,   469,     0,
     471,   472,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,     0,   454,   455,
     456,   431,   363,   428,     0,   432,     0,     0,     0,   457,
     429,     0,     0,     0,  1215,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   504,   459,   460,
     461,   462,   463,     0,     0,   466,   467,   468,   469,     0,
     471,   472,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,     0,   454,   455,
     456,   431,   363,   428,     0,   432,     0,     0,     0,   457,
     429,     0,     0,     0,  1222,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   504,   459,   460,
     461,   462,   463,     0,     0,   466,   467,   468,   469,     0,
     471,   472,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,     0,   454,   455,
     456,   431,   363,   428,     0,   432,     0,     0,     0,   457,
     429,     0,     0,     0,  1307,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   504,   459,   460,
     461,   462,   463,     0,     0,   466,   467,   468,   469,     0,
     471,   472,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,     0,   454,   455,
     456,   431,   363,   428,     0,   432,     0,     0,     0,   457,
     429,     0,     0,     0,  1308,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   504,   459,   460,
     461,   462,   463,     0,     0,   466,   467,   468,   469,     0,
     471,   472,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,     0,   454,   455,
     456,   431,   363,   428,     0,   432,     0,     0,     0,   457,
     429,     0,     0,     0,  1309,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   504,   459,   460,
     461,   462,   463,     0,     0,   466,   467,   468,   469,     0,
     471,   472,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,     0,   454,   455,
     456,   431,   363,   428,     0,   432,     0,     0,     0,   457,
     429,     0,     0,     0,  1310,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   504,   459,   460,
     461,   462,   463,     0,     0,   466,   467,   468,   469,     0,
     471,   472,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,     0,   454,   455,
     456,   431,   363,   428,     0,   432,     0,     0,     0,   457,
     429,     0,     0,     0,  1311,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   504,   459,   460,
     461,   462,   463,     0,     0,   466,   467,   468,   469,     0,
     471,   472,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,     0,   454,   455,
     456,   431,   363,     0,     0,   432,     0,     0,   428,   457,
       0,     0,     0,     0,  1312,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,   273,     0,
       0,     0,     0,     0,     0,     0,   433,   504,   459,   460,
     461,   462,   463,   859,     0,   466,   467,   468,   469,     0,
     471,   472,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,     0,   454,   455,
     456,     0,   363,     0,   428,     0,   431,     0,     0,   457,
     432,   429,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   430,   273,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   850,     0,   979,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,     0,   431,   454,   455,   456,   432,   363,     0,     0,
       0,     0,     0,     0,   457,     0,   428,   818,     0,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   850,     0,   430,     0,   433,     0,     0,
     819,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   820,     0,   454,
     455,   456,     0,   363,     0,     0,     0,     0,     0,     0,
     457,     0,   428,     0,   431,     0,     0,     0,   432,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   430,   273,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   978,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,     0,
     431,   454,   455,   456,   432,     0,     0,   428,     0,     0,
       0,     0,   457,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   430,  1149,     0,     0,
       0,     0,     0,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,     0,     0,   454,   455,   456,
       0,   363,     0,   428,     0,   431,     0,     0,   457,   432,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,   273,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   850,     0,     0,     0,
     433,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
       0,   431,   454,   455,   456,   432,   363,     0,   428,     0,
       0,     0,     0,   457,     0,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,   273,     0,
       0,     0,   850,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,     0,   454,   455,
     456,     0,   363,     0,   428,     0,   431,     0,     0,   457,
     432,   429,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   430,   910,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   911,
       0,   433,     0,     0,     0,     0,     0,     0,     0,   428,
       0,     0,     0,     0,     0,     0,   429,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,   430,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,     0,   431,   454,   455,   456,   432,   363,     0,   428,
       0,     0,     0,     0,   457,     0,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,   912,
       0,     0,     0,     0,     0,     0,     0,   433,     0,     0,
       0,     0,     0,     0,   913,     0,     0,   431,     0,     0,
       0,   432,     0,     0,   434,   435,     0,   436,   437,   438,
     945,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,     0,     0,   454,
     455,   456,   433,     0,     0,     0,     0,   431,     0,     0,
     457,   432,     0,     0,     0,     0,     0,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   433,     0,   454,   455,   456,     0,   363,     0,
       0,     0,     0,     0,     0,   457,     0,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   428,     0,   454,   455,   456,     0,   229,   429,
       0,     0,     0,     0,     0,   457,     0,     0,     0,     0,
       0,   430,   230,   231,     0,   232,  1122,     0,     0,     0,
     233,     0,     0,     0,     0,     0,     0,     0,   234,     0,
       0,     0,   428,     0,   235,     0,     0,     0,     0,   429,
     236,     0,     0,     0,   237,     0,     0,   238,     0,   239,
       0,   430,  1141,  1123,     0,     0,     0,     0,   240,     0,
       0,     0,     0,     0,     0,   241,   242,     0,     0,     0,
     431,     0,     0,   243,   432,     0,     0,   428,     0,     0,
       0,     0,     0,   244,   429,     0,     0,     0,     0,     0,
       0,     0,   245,   246,     0,   247,   430,   248,     0,   249,
       0,     0,   250,     0,     0,   433,   251,   540,     0,   252,
     431,     0,   253,     0,   432,     0,     0,     0,     0,     0,
       0,     0,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   433,     0,   454,   455,   456,
       0,     0,     0,   428,     0,   431,     0,     0,   457,   432,
     429,     0,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,   430,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,     0,   850,   454,   455,   456,
     433,   363,     0,     0,   428,   527,     0,     0,   457,     0,
       0,   429,     0,     0,     0,     0,     0,   434,   435,     0,
     436,   437,   438,   430,   439,   440,   441,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
       0,   431,   454,   455,   456,   432,     0,     0,     0,     0,
     428,  1159,     0,   457,     0,     0,     0,   429,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,   850,     0,     0,     0,   433,     0,     0,     0,
       0,     0,   431,     0,     0,     0,   432,     0,     0,     0,
       0,     0,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,   433,   454,   455,
     456,     0,     0,     0,     0,     0,     0,  1202,   431,   457,
       0,     0,   432,     0,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   428,     0,   454,
     455,   456,     0,   433,   429,     0,     0,     0,     0,     0,
     457,     0,     0,     0,     0,     0,   430,     0,     0,     0,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,     0,     0,   454,   455,   456,     0,   551,
       0,     0,     0,     0,     0,     0,   457,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   431,     0,   428,     0,   432,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   430,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,     0,     0,     0,     0,   428,     0,
       0,     0,     0,     0,     0,   429,     0,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,   430,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
       0,   555,   454,   455,   456,   431,     0,     0,     0,   432,
       0,     0,     0,   457,   428,   814,     0,     0,   731,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,     0,     0,   431,     0,     0,     0,
     432,     0,     0,     0,     0,     0,     0,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
       0,   433,   454,   455,   456,     0,     0,     0,     0,     0,
       0,     0,   431,   457,     0,     0,   432,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   428,     0,   454,   455,   456,     0,   433,   429,     0,
     797,     0,     0,     0,   457,     0,     0,     0,     0,     0,
     430,     0,     0,     0,   434,   435,     0,   436,   437,   438,
       0,   439,   815,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   428,     0,   454,
     455,   456,     0,     0,   429,     0,     0,     0,     0,     0,
     457,     0,     0,     0,     0,     0,   430,     0,     0,   943,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,     0,   432,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   430,     0,     0,     0,
       0,     0,     0,   871,   433,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   431,     0,     0,     0,   432,
       0,   434,   435,     0,   436,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,     0,     0,   454,   455,   456,     0,
     433,     0,     0,     0,     0,   431,     0,   457,   428,   432,
       0,     0,     0,     0,     0,   429,     0,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,   430,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     433,     0,   454,   455,   456,     0,     0,     0,   428,  1013,
       0,     0,     0,   457,     0,   429,     0,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,   430,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
       0,     0,   454,   455,   456,     0,   431,     0,     0,   957,
     432,     0,     0,   457,   428,     0,     0,     0,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,   431,     0,     0,     0,
     432,     0,     0,     0,     0,     0,     0,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   433,     0,   454,   455,   456,     0,     0,     0,     0,
     958,     0,   431,     0,   457,     0,   432,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   428,  1119,   454,   455,   456,     0,   433,   429,     0,
       0,     0,     0,     0,   457,     0,     0,     0,     0,     0,
     430,     0,     0,     0,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,     0,     0,   454,
     455,   456,     0,     0,     0,   428,  1068,     0,     0,     0,
     457,     0,   429,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,     0,   431,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   428,     0,     0,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   434,   435,   431,   436,   437,   438,   432,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   428,     0,   454,   455,   456,     0,
       0,   429,     0,     0,   213,     0,     0,   457,   433,     0,
       0,     0,     0,   430,   431,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,   434,   435,     0,   436,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,     0,   433,
     454,   455,   456,     0,     0,     0,     0,     0,     0,     0,
       0,   457,     0,     0,     0,     0,   434,   435,     0,   436,
     437,   438,   431,   439,   440,   441,   432,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   428,
    1135,   454,   455,   456,     0,     0,   429,     0,     0,     0,
       0,     0,   457,     0,     0,     0,  1162,   433,   430,  1183,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,     0,     0,   454,
     455,   456,     0,     0,     0,     0,     0,     0,     0,     0,
     457,     0,     0,     0,     0,   428,     0,   431,     0,     0,
       0,   432,   429,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,  1186,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,     0,     0,     0,   428,     0,     0,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,   434,
     435,     0,   436,   437,   438,   430,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,   431,   454,   455,   456,   432,     0,     0,
       0,   428,  1210,     0,     0,   457,     0,     0,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,   431,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,   434,   435,     0,   436,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,     0,   433,
     454,   455,   456,     0,     0,     0,     0,   428,     0,   431,
       0,   457,     0,   432,   429,     0,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,   430,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,     0,
       0,   454,   455,   456,   433,     0,     0,     0,   428,     0,
    1188,     0,   457,     0,     0,   429,  1252,     0,     0,     0,
       0,   434,   435,     0,   436,   437,   438,   430,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,     0,   431,   454,   455,   456,   432,
       0,     0,     0,   428,     0,     0,     0,   457,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,     0,     0,   431,     0,     0,     0,
     432,     0,     0,     0,     0,     0,     0,   434,   435,  1259,
     436,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
       0,   433,   454,   455,   456,     0,     0,     0,     0,   428,
       0,   431,     0,   457,     0,   432,   429,     0,   434,   435,
       0,   436,   437,   438,  1262,   439,   440,   441,   430,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,     0,     0,   454,   455,   456,   433,     0,     0,     0,
     428,     0,     0,     0,   457,     0,     0,   429,     0,     0,
       0,     0,     0,   434,   435,     0,   436,   437,   438,   430,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,   431,   454,   455,
     456,   432,     0,     0,     0,   428,     0,     0,     0,   457,
    1313,     0,   429,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,     0,     0,
       0,     0,   433,     0,     0,     0,     0,     0,   431,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,   434,
     435,  1314,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,   433,   454,   455,   456,     0,     0,     0,
       0,   428,     0,   431,     0,   457,     0,   432,   429,     0,
     434,   435,     0,   436,   437,   438,  1315,   439,   440,   441,
     430,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,     0,     0,   454,   455,   456,   433,     0,
       0,     0,   428,     0,     0,     0,   457,     0,     0,   429,
       0,     0,     0,     0,     0,   434,   435,     0,   436,   437,
     438,   430,   439,   440,   441,     0,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,     0,   431,
     454,   455,   456,   432,     0,     0,     0,   428,     0,     0,
       0,   457,  1316,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   430,     0,     0,     0,
       0,     0,     0,     0,   433,     0,     0,     0,     0,     0,
     431,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,   434,   435,  1317,   436,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,     0,   433,   454,   455,   456,     0,
       0,     0,     0,   428,     0,   431,     0,   457,     0,   432,
     429,     0,   434,   435,     0,   436,   437,   438,  1318,   439,
     440,   441,   430,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,     0,     0,   454,   455,   456,
     433,     0,     0,     0,   428,     0,     0,     0,   457,     0,
       0,   429,     0,     0,     0,     0,     0,   434,   435,     0,
     436,   437,   438,   430,   439,   440,   441,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
       0,   431,   454,   455,   456,   432,     0,     0,     0,     0,
       0,     0,     0,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,     0,   933,     0,     0,     0,   432,     0,     0,     0,
       0,     0,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,   433,   454,   455,
     456,     0,     0,     0,     0,     0,     0,     0,     0,   457,
       0,     0,     0,     0,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,     0,     0,   454,
     455,   456,   399,     0,   229,     0,     0,     0,     0,     0,
     457,     0,     0,     0,     0,     0,     0,     0,   230,   231,
       0,   232,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
     235,     0,     0,     0,     0,     0,   236,     0,     0,     0,
     237,     0,     0,   238,     0,   239,     0,     0,     0,     0,
     229,     0,     0,     0,   240,     0,     0,     0,     0,     0,
       0,   241,   242,     0,   230,   231,     0,   232,     0,   243,
       0,     0,   233,     0,     0,     0,     0,     0,     0,   244,
     234,     0,     0,     0,     0,     0,   235,     0,   245,   246,
       0,   247,   236,   248,     0,   249,   237,     0,   250,   238,
       0,   239,   251,     0,     0,   252,   229,     0,   253,     0,
     240,     0,     0,     0,     0,     0,     0,   241,   242,     0,
     230,   231,     0,   232,     0,   243,     0,     0,   233,     0,
      23,     0,    89,     0,     0,   244,   234,     0,     0,     0,
       0,     0,   235,     0,   245,   246,     0,   247,   236,   248,
       0,   249,   237,     0,   250,   238,     0,   239,   251,     0,
       0,   252,  -326,     0,   253,     0,   240,     0,     0,     0,
       0,     0,     0,   241,   242,     0,  -326,  -326,     0,  -326,
       0,   243,    56,     0,  -326,     0,     0,     0,    89,     0,
       0,   244,  -326,     0,    65,     0,     0,     0,  -326,     0,
     245,   246,     0,   247,  -326,   248,     0,   249,  -326,     0,
     250,  -326,     0,  -326,   251,     0,     0,   252,   229,    85,
     253,     0,  -326,     0,     0,     0,     0,     0,     0,  -326,
    -326,     0,   230,   231,     0,   232,     0,  -326,     0,     0,
     233,     0,     0,     0,     0,     0,     0,  -326,   234,     0,
       0,     0,     0,     0,   235,     0,  -326,  -326,     0,  -326,
     236,  -326,     0,  -326,   237,     0,  -326,   238,     0,   239,
    -326,     0,     0,  -326,  -327,     0,  -326,     0,   240,     0,
       0,     0,     0,     0,     0,   241,   242,     0,  -327,  -327,
       0,  -327,     0,   243,     0,     0,  -327,     0,     0,     0,
       0,     0,     0,   244,  -327,     0,     0,     0,     0,     0,
    -327,     0,   245,   246,     0,   247,  -327,   248,     0,   249,
    -327,     0,   250,  -327,     0,  -327,   251,     0,     0,   252,
       0,     0,   253,     0,  -327,     0,     0,     0,     0,     0,
       0,  -327,  -327,     0,     0,     0,     0,     0,     0,  -327,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -327,
       0,     0,     0,     0,     0,     0,     0,     0,  -327,  -327,
       0,  -327,     0,  -327,     0,  -327,     0,     0,  -327,     0,
       0,     0,  -327,     0,     0,  -327,     0,     0,  -327
};

static const yytype_int16 yycheck[] =
{
       2,     2,    62,   112,   152,    20,   265,   485,   181,     2,
     549,   165,   586,   400,   420,   626,   623,   553,    62,    62,
     228,   222,   919,    25,   877,    67,   509,   820,   382,   786,
     289,   133,    25,   544,    26,   815,   780,     1,   838,   575,
     600,     1,   602,   603,   959,    87,   405,   607,   608,     1,
       8,    32,   673,    28,   675,    32,    56,   265,    32,    56,
      32,   103,    32,   519,    47,    40,    41,    42,   103,     0,
     329,     3,    32,    75,    66,   883,    35,    79,    80,    47,
       1,   289,    75,     1,    75,    32,    79,    80,   261,    46,
      24,     2,  1177,    47,    32,    32,   119,    47,    32,     3,
     102,    61,   148,   166,   106,   166,    40,    41,    42,   102,
     173,    32,   175,   106,    25,   121,    40,    41,    91,    93,
      61,   329,    54,    75,    47,   148,    24,   118,    85,    47,
     176,    24,  1029,    93,    32,   148,   171,    91,   121,    32,
      61,    91,    40,    41,    42,    47,   106,    40,    41,    42,
      54,   121,   111,   961,     3,    93,    56,  1242,   115,   119,
     767,   174,   126,  1016,    75,   148,   118,   148,    79,    80,
     626,   148,    93,   173,   121,   150,   173,   152,   119,   538,
     148,   172,    18,   147,   148,   106,   120,   147,  1103,   164,
     171,   102,   150,   176,   171,   106,   953,   171,   119,   171,
     104,   171,   959,  1056,  1057,    54,   166,   121,   126,    56,
     174,   171,   214,    56,   750,  1015,   475,   476,   962,   121,
     172,   214,   120,   198,   171,   148,   147,   120,   166,   147,
     148,  1011,   440,   171,   171,   589,    27,   121,    22,   124,
     121,   175,    78,   514,   804,   819,   148,   420,   166,   732,
     171,   175,   171,   176,   173,   104,    47,   166,    31,    56,
      51,    97,   176,   172,     3,    36,   268,   475,   476,   148,
     541,   273,   148,   784,   176,   268,  1129,   175,   114,   381,
     273,   148,   175,   258,   259,    76,   171,    53,    79,    27,
      81,   400,   176,   172,   269,   176,    31,   110,   174,    27,
      91,   509,    73,   214,   148,    89,   148,   174,    74,    47,
     148,  1164,   142,    51,    27,    54,   173,    90,    84,   110,
     150,    94,   166,  1220,   148,   932,   527,  1084,  1121,   113,
    1123,   166,   174,   141,   309,   489,   174,   172,    76,    11,
     148,    79,    31,    81,   404,   553,  1103,   349,   349,   960,
     174,    79,   173,    91,    93,    90,   349,   268,   173,    94,
     175,   363,   273,    91,   172,   104,    79,   575,   166,   142,
     363,   373,   110,   148,   147,   173,   149,   150,    91,    56,
     373,   154,   110,   174,   386,   360,     2,   148,   161,    56,
     148,   592,   875,   386,   167,   168,   169,   172,   148,   391,
      56,    90,    27,   116,   396,    94,    90,   142,   166,    25,
      94,   148,   147,   174,   149,   150,     2,   148,   420,   154,
     395,   126,   464,   465,   174,   400,   161,     8,   470,  1003,
     148,   473,   474,   916,   169,   172,   174,    46,   349,    25,
     173,   919,   148,   174,   148,   420,   982,   152,  1069,  1070,
      56,    76,   363,   142,    79,   148,   174,    66,   147,    75,
      56,   150,   373,    79,    80,   154,    91,     3,   174,   565,
     174,   567,   148,  1033,  1034,   386,    85,  1037,  1038,   539,
      27,   174,  1042,  1043,    20,   110,   102,   148,   894,    75,
     106,   483,   879,    79,    80,   539,   539,    33,   174,   501,
      47,   503,    46,   148,    51,   148,   115,   148,   501,   484,
     503,    90,   148,   174,    83,    94,   102,    86,    54,   148,
     106,   166,    66,   565,   732,   567,   173,    63,   175,    76,
     166,   174,    79,   174,    81,   510,   173,   166,   175,   514,
     148,    85,   750,    56,    91,     3,  1120,  1171,  1122,   290,
     291,  1029,   527,  1113,  1114,  1115,  1116,    56,   166,    17,
      18,   536,    20,   110,   148,    27,   541,    25,   104,    90,
     126,   115,    90,    94,   110,    33,    94,   579,   579,   594,
     572,    39,   166,    56,    90,    47,   579,    45,    94,    51,
     501,    49,   503,   148,    52,   173,    54,   153,   214,   155,
     156,    31,   158,   159,    90,    63,   814,   815,    94,   101,
     585,   166,    70,    71,    76,   148,    56,    79,   148,    81,
      78,   596,  1246,  1247,  1248,  1249,  1250,  1251,   214,    91,
      88,    50,   148,   166,    53,   173,   166,   175,  1177,    97,
      98,    56,   100,   148,   102,   148,   104,    56,   110,   107,
     166,    56,   268,   111,   112,    74,   114,   273,    77,   117,
      90,   166,     3,   166,    94,    84,   142,   875,   579,    79,
      80,   147,   141,  1060,   150,   173,   171,   175,   154,   173,
      56,   175,   268,   173,   147,   175,   840,   273,    79,    80,
     173,   110,   166,   165,    56,     2,   166,    85,   166,   166,
     166,   166,   166,  1242,   679,   166,   748,   148,   916,   166,
     147,   171,   142,   143,   173,   173,   146,   147,    25,   149,
     150,   894,   171,   173,   154,   173,   173,   166,   770,   877,
     174,   161,   880,   349,   166,   173,     8,   167,   168,   169,
       3,   743,  1220,    66,   173,   171,   126,   363,   172,    91,
     150,    91,   754,   126,    69,   757,   173,   373,   126,   148,
     174,   754,   126,   349,   757,   126,    34,    34,    75,    66,
     386,   174,    79,    80,   982,   126,   174,   363,   147,   174,
      21,   171,   120,   150,   759,   148,   173,   373,   106,   173,
     150,   766,   150,   150,   150,   102,   150,   150,     3,   106,
     386,   776,   150,   150,   150,   150,   150,   150,   142,   174,
     176,   150,    17,    18,   150,    20,   166,   877,   150,   794,
      25,   881,    34,    34,   173,   147,    47,   148,    33,   166,
     890,   166,    34,   166,    39,   166,   166,   166,  1097,  1098,
      45,   166,   147,   754,    49,   847,   757,    52,   172,    54,
     176,   853,   172,   845,   847,   174,   173,   171,    63,   148,
     853,   174,   166,   865,   839,    70,    71,   869,  1016,    32,
     176,    31,   865,    78,   176,   173,   869,   173,   166,   174,
      32,    34,   176,    88,   174,   501,   166,   503,   174,  1097,
    1098,   176,    97,    98,   176,   100,   174,   102,   148,   104,
     166,   166,   107,   165,   879,   165,   111,   214,  1056,   114,
     111,   574,   117,   982,    31,   501,   812,   503,   373,   894,
      24,   949,   365,   974,   337,   525,   539,   894,   384,   890,
      90,   933,   934,   881,    94,   536,   847,   264,   940,   390,
     942,  1242,   853,   945,   303,  1062,  1252,   940,  1057,   942,
    1245,  1060,   945,   839,   865,   679,  1016,   131,   869,   522,
     766,   268,    -1,   579,    -1,  1138,   273,   969,   173,    -1,
      -1,    -1,  1145,    90,   949,    -1,   969,    94,   970,   981,
      -1,  1129,   142,   143,    -1,   145,   146,   147,   981,   149,
     150,    -1,    -1,   579,   154,    -1,    -1,    -1,    -1,   974,
       2,   161,    -1,    -1,  1019,  1020,    -1,   167,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,   142,   143,    -1,    -1,   940,
     147,   942,   149,   150,   945,    -1,    -1,   154,    -1,  1041,
      -1,    -1,   349,    -1,   161,  1047,    -1,    -1,  1041,    -1,
     167,   168,   169,    -1,  1047,  1164,   363,    -1,   969,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   373,    -1,    -1,  1129,
     981,    -1,    -1,    75,    -1,    -1,    -1,    79,    80,   386,
      -1,    -1,    -1,    -1,    -1,  1060,    -1,    -1,    -1,  1091,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1091,    -1,
     102,    -1,    -1,    -1,   106,  1278,  1279,    -1,    -1,  1282,
    1283,  1284,    -1,  1286,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,    -1,
    1041,    -1,    -1,    -1,    -1,    -1,  1047,  1139,   754,  1141,
      -1,   757,    -1,    -1,  1146,    -1,  1139,  1149,  1141,    -1,
      -1,    -1,    -1,  1146,    -1,  1157,  1149,  1159,   170,    -1,
      -1,   173,    -1,   175,  1157,    -1,  1159,    -1,   754,    -1,
      -1,   757,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1091,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1191,
      -1,    -1,    -1,    -1,   501,  1197,   503,    -1,  1191,  1201,
    1202,    -1,    -1,    -1,  1197,    -1,    -1,    -1,  1201,  1202,
      -1,    -1,   214,  1215,    -1,    -1,    -1,    -1,    -1,    -1,
    1222,    -1,  1215,    -1,    -1,    -1,    -1,    -1,  1139,  1222,
    1141,   847,  1234,    -1,    -1,  1146,  1238,   853,  1149,    -1,
      -1,  1234,    -1,    -1,    -1,  1238,  1157,    -1,  1159,   865,
      -1,  1253,  1253,   869,    -1,    -1,    -1,    -1,    -1,    -1,
    1253,   847,    -1,    -1,    -1,    -1,   268,   853,    -1,    -1,
      -1,   273,   579,    -1,    -1,    -1,    -1,    -1,    -1,   865,
    1191,    -1,    -1,   869,    -1,    -1,  1197,    -1,    -1,    -1,
    1201,  1202,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1215,  1307,  1308,  1309,  1310,  1311,
    1312,  1222,    -1,    -1,  1307,  1308,  1309,  1310,  1311,  1312,
      -1,    -1,    -1,  1234,   940,    -1,   942,  1238,    -1,   945,
     388,   389,    -1,   391,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1253,    -1,    -1,    -1,    -1,   349,    -1,    -1,
      -1,    -1,    -1,   969,   940,    -1,   942,    -1,    -1,   945,
      -1,   363,    -1,    -1,    -1,   981,    -1,    -1,    -1,    -1,
      -1,   373,    -1,   431,   432,    -1,    -1,    -1,    -1,    -1,
       2,    -1,    -1,   969,   386,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,   981,  1307,  1308,  1309,  1310,
    1311,  1312,    -1,    25,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    20,    -1,    -1,    -1,     2,    25,    -1,    -1,   477,
     478,    -1,    -1,    -1,    -1,  1041,    35,    -1,    -1,    -1,
      -1,  1047,    -1,    -1,    -1,    -1,    45,    -1,    25,    -1,
      -1,    -1,    -1,    52,    -1,    54,    -1,   754,    -1,    -1,
     757,    -1,    -1,    75,    63,  1041,    -1,    79,    80,    -1,
      -1,  1047,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1091,    -1,    -1,    -1,    88,
     102,    -1,    -1,    -1,   106,    -1,    -1,    -1,    75,    -1,
      -1,   100,    79,    80,    -1,   104,    -1,    -1,    -1,   501,
      -1,   503,   111,    -1,    -1,  1091,    -1,    -1,   117,    -1,
      -1,    -1,    -1,   571,   572,   102,    -1,    -1,    -1,   106,
      -1,    -1,    -1,  1139,    -1,  1141,    -1,    -1,    -1,    -1,
    1146,    -1,    -1,  1149,    -1,    -1,    -1,    -1,    -1,    -1,
     847,  1157,    -1,  1159,    -1,    -1,   853,    -1,    -1,    -1,
      -1,    -1,    67,  1139,    -1,  1141,    -1,    -1,   865,    -1,
    1146,    -1,   869,  1149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1157,    87,  1159,    -1,  1191,    -1,   579,    -1,    -1,
      -1,  1197,    -1,    -1,    -1,  1201,  1202,    -1,   103,    -1,
      -1,    -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,  1215,
      -1,    -1,    -1,    -1,    -1,  1191,  1222,    -1,    -1,    -1,
      -1,  1197,    -1,    -1,    -1,  1201,  1202,    -1,  1234,    -1,
      -1,    -1,  1238,    -1,    -1,    -1,    -1,   214,    -1,  1215,
      -1,    -1,    -1,   940,    -1,   942,  1222,  1253,   945,    -1,
      -1,    -1,    31,    -1,    -1,    22,   268,    -1,  1234,    26,
      27,   273,  1238,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      37,    38,   969,    -1,    -1,    -1,    43,  1253,    -1,    -1,
      -1,    -1,    -1,    50,   981,    -1,    53,    -1,    -1,    -1,
      -1,   268,    -1,    -1,    -1,    -1,   273,    -1,    -1,    66,
      -1,  1307,  1308,  1309,  1310,  1311,  1312,    74,    -1,    -1,
      77,    90,    79,    -1,    -1,    94,    83,    84,    85,    86,
      -1,    -1,    89,   228,    91,    -1,    -1,    -1,    -1,    -1,
      -1,  1307,  1308,  1309,  1310,  1311,  1312,   349,    -1,    -1,
      -1,    -1,    -1,   110,  1041,    -1,   113,    -1,    -1,   116,
    1047,   363,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     265,   373,   754,   142,   143,   757,   145,   146,   147,    -1,
     149,   150,   349,    -1,   386,   154,    -1,    -1,    -1,    -1,
      -1,   160,   161,    -1,   289,   164,   363,    -1,   167,   168,
     169,    -1,    -1,   841,  1091,    -1,   373,    -1,    -1,   166,
     848,    -1,    -1,    -1,   171,    -1,   854,    -1,     2,   386,
      -1,   859,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   867,
      -1,    -1,    -1,    -1,   329,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1139,    -1,  1141,    -1,    -1,    -1,    -1,  1146,
      -1,    -1,  1149,    -1,    -1,   847,    -1,    -1,    -1,    -1,
    1157,   853,  1159,   911,    -1,   913,   914,    -1,    -1,    -1,
      -1,    -1,    -1,   865,    -1,    -1,    -1,   869,    -1,    -1,
      -1,    75,    -1,    -1,    -1,   933,   934,    -1,    -1,   501,
      -1,   503,    -1,    -1,  1191,    -1,    -1,    -1,    -1,    -1,
    1197,    -1,    -1,    -1,  1201,  1202,    -1,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,  1215,    -1,
      -1,    -1,    -1,    -1,   501,  1222,   503,    -1,    -1,    -1,
     978,   979,   980,    -1,    -1,    -1,    -1,  1234,    -1,    -1,
      -1,  1238,    -1,    -1,    -1,    -1,    -1,    -1,   940,    -1,
     942,    -1,    -1,   945,     1,    -1,  1253,    -1,    -1,   464,
     465,    -1,    -1,    -1,    -1,   470,    -1,   579,   473,   474,
     475,   476,    -1,    -1,    -1,    22,    -1,   969,    -1,    26,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,   981,
      37,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,
      -1,    -1,   579,    50,   509,    -1,    53,    -1,    -1,    -1,
    1307,  1308,  1309,  1310,  1311,  1312,    -1,    -1,    -1,    -1,
     214,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    83,    84,    -1,    86,
      -1,    -1,    89,    -1,    91,    -1,    -1,    -1,   553,  1041,
      -1,    -1,    -1,    -1,    -1,  1047,    -1,    -1,    -1,    -1,
     565,    -1,   567,   110,    -1,    -1,   113,    -1,    -1,   116,
     575,    -1,    22,    -1,   268,    -1,    26,    27,    -1,   273,
      -1,    -1,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,
      -1,    -1,    -1,    43,   141,    -1,    -1,    -1,    -1,  1091,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      35,    -1,    37,    38,    -1,   172,    -1,    77,    43,    79,
      -1,    81,    -1,    83,    -1,    50,    86,    -1,    -1,    89,
      -1,    91,   754,    -1,    31,   757,    -1,  1139,    -1,  1141,
      -1,    -1,    -1,    -1,  1146,   349,    -1,  1149,    -1,    -1,
     110,    -1,    77,   113,    79,  1157,   116,  1159,    83,   363,
      -1,    86,    -1,    -1,    89,    -1,    91,   754,    -1,   373,
     757,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   386,    -1,    -1,   110,    -1,    -1,   113,  1191,
      -1,   116,    -1,    90,    -1,  1197,    -1,    94,    -1,  1201,
    1202,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,
      -1,    -1,   172,  1215,    -1,    -1,   141,   732,    -1,    -1,
    1222,    -1,    -1,    -1,    -1,   847,    -1,    -1,   125,    -1,
      -1,   853,  1234,   748,    -1,   750,  1238,    -1,    -1,    -1,
      -1,   166,    -1,   865,    -1,   142,   143,   869,   145,   146,
     147,  1253,   149,   150,   151,   770,   153,   154,   155,   156,
     847,   158,   159,   160,   161,   162,   853,   164,    -1,    -1,
     167,   168,   169,    -1,    -1,    22,    -1,    -1,   865,    26,
      27,   178,   869,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      37,    38,    -1,    -1,    -1,    -1,    43,   501,    -1,   503,
      -1,    -1,    -1,    50,    -1,  1307,  1308,  1309,  1310,  1311,
    1312,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   940,    -1,
     942,    -1,    -1,   945,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    91,    -1,    -1,   969,    -1,    17,
      18,    -1,    20,   940,    -1,   942,    -1,    25,   945,   981,
     875,    -1,    -1,   110,    31,    33,   113,    -1,    -1,   116,
      -1,    39,    -1,    -1,    -1,   579,    -1,    45,    -1,    -1,
      -1,    49,   969,    -1,    52,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   981,    63,    -1,    -1,    -1,    -1,
      -1,   916,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1041,
      88,    -1,    -1,    90,    -1,  1047,    -1,    94,    -1,    97,
      98,    -1,   100,    -1,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,  1041,    -1,    -1,    -1,   125,    -1,
    1047,    -1,    -1,    -1,    -1,    -1,    -1,   982,    -1,  1091,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
      -1,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,  1091,   173,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    -1,  1139,    -1,  1141,
      -1,    -1,    -1,    -1,  1146,    -1,    -1,  1149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1157,    -1,  1159,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     754,    -1,  1139,   757,  1141,    -1,    -1,    -1,    -1,  1146,
      -1,    -1,  1149,    -1,    -1,    -1,    -1,    -1,    -1,  1191,
    1157,    -1,  1159,    -1,    -1,  1197,    -1,    -1,    -1,  1201,
    1202,    -1,  1097,  1098,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1215,    -1,    -1,    -1,    -1,    -1,    -1,
    1222,    -1,    -1,    -1,  1191,    -1,    -1,    -1,    -1,    -1,
    1197,    -1,  1234,    -1,  1201,  1202,  1238,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1215,    -1,
      -1,  1253,    -1,    -1,    -1,  1222,    -1,    -1,    -1,    -1,
      -1,    22,    -1,   847,    -1,    26,    27,  1234,    -1,   853,
      -1,  1238,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,
      -1,   865,    43,    -1,    -1,   869,  1253,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1307,  1308,  1309,  1310,  1311,
    1312,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    86,    -1,    -1,    89,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1307,  1308,  1309,  1310,  1311,  1312,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,   940,    -1,   942,    13,
      -1,   945,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,    -1,    -1,    31,    -1,    -1,    33,
     141,    -1,    -1,    -1,    -1,   969,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    47,    48,    -1,    -1,   981,    -1,    53,
      -1,    55,    -1,    57,    -1,   166,    -1,    -1,    62,    -1,
      -1,   172,    66,    67,    68,    69,    70,    71,    -1,    -1,
      -1,    75,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    87,    -1,    90,    90,    -1,    92,    94,
      -1,    95,    96,    -1,    98,    -1,    12,    -1,   102,   103,
     104,    -1,    -1,    19,    -1,    -1,    -1,  1041,    -1,    -1,
      -1,    -1,    -1,  1047,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,   133,
      -1,    47,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,    -1,   154,
     155,   156,    -1,   158,   159,   160,   161,  1091,    -1,   164,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
     174,    -1,    -1,   178,    90,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,    -1,    -1,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1139,    -1,  1141,    -1,   125,
      -1,    -1,  1146,    -1,    -1,  1149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1157,   228,  1159,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,   171,    -1,  1191,    -1,    -1,
      -1,   265,   178,  1197,    -1,    -1,    -1,  1201,  1202,    -1,
      -1,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,
      -1,  1215,    -1,    -1,    35,   289,    37,    38,  1222,    -1,
      -1,    -1,    43,    -1,    -1,    -1,   300,   301,     2,    50,
    1234,    -1,    -1,    -1,  1238,    -1,    -1,    -1,    -1,    -1,
      -1,   315,   316,    -1,    -1,    -1,    -1,    -1,    -1,  1253,
      -1,    25,    -1,    -1,    -1,   329,    77,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    86,    -1,    -1,    89,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   352,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
     374,    75,   376,  1307,  1308,  1309,  1310,  1311,  1312,    -1,
      -1,   385,    -1,   387,   388,   389,   390,   391,    -1,    -1,
     141,    -1,   396,    -1,    -1,    -1,    -1,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,   172,    -1,    -1,   428,   429,   430,   431,   432,   433,
      -1,   435,   436,   437,   438,   439,    -1,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,    -1,    -1,    -1,    -1,    -1,    -1,
     464,   465,    -1,    -1,    -1,    -1,   470,    -1,    -1,   473,
     474,   475,   476,   477,   478,    -1,    -1,   481,   482,   483,
      31,   485,    -1,    -1,    -1,    45,    -1,   491,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   503,
      -1,    -1,   506,    -1,    -1,   509,    -1,    -1,    -1,    69,
     214,    -1,    -1,    -1,    -1,   519,    -1,   521,    -1,    79,
      80,   525,    -1,    -1,   528,   529,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   539,    -1,    -1,    -1,    90,
     544,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,   553,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,   562,    -1,
      -1,   565,    -1,   567,   268,    -1,   570,   571,   572,   273,
      -1,   575,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   586,   587,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   606,   154,    -1,    -1,    -1,    -1,    -1,   160,
     161,    -1,   616,   164,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,   626,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,   349,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   363,
     220,    -1,    -1,    31,    32,    -1,   670,    -1,    -1,   373,
      -1,    -1,    -1,   677,    -1,    -1,    -1,    -1,    -1,    47,
      -1,    -1,   386,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,
     260,   261,    -1,    -1,    -1,    -1,    -1,   267,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   276,    69,   278,    -1,
      -1,    -1,    90,    -1,    -1,   285,    94,   731,   732,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   748,    -1,   750,   751,   752,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    -1,   125,    31,    -1,
      -1,    -1,    -1,    -1,    -1,   325,   770,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
     784,   149,   150,   151,   788,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   501,    -1,   167,
     168,   169,    -1,   171,    -1,    -1,    -1,    -1,   812,    -1,
     178,    -1,   816,    -1,   818,   819,   820,    90,   822,    -1,
     824,    94,    -1,   827,   828,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   841,    -1,   843,
      -1,   845,    -1,   847,   848,    -1,    -1,    -1,    -1,   853,
     854,    -1,    -1,    -1,    -1,   859,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   867,   868,   869,    -1,   871,   872,   142,
     143,   875,   145,   146,   147,   579,   149,   150,   151,    -1,
     153,   154,   155,   156,    -1,   158,   159,   160,   161,   162,
      -1,   164,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,   910,   911,   912,   913,
     914,   915,   916,    -1,    -1,   919,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   276,    -1,   278,    -1,    -1,   933,
     934,    -1,    -1,   285,    -1,    -1,    -1,    -1,    -1,   943,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   960,    -1,    -1,    -1,
     964,    -1,    -1,    -1,    -1,    -1,   970,    22,    -1,    -1,
      -1,    26,    27,   325,   978,   979,   980,    -1,   982,    -1,
      35,    -1,    37,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,  1003,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1011,    -1,  1013,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1023,
      -1,  1025,    77,    -1,    79,  1029,    81,  1031,    83,    -1,
      -1,    86,    -1,    -1,    89,    -1,    91,    -1,   598,   599,
     600,   601,   602,   603,   604,   605,    -1,    -1,    -1,    -1,
     754,    -1,    -1,   757,    -1,   110,    -1,    -1,   113,    -1,
    1064,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1074,    -1,    -1,  1077,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,  1097,  1098,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1109,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    -1,    -1,  1119,  1120,  1121,  1122,  1123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1139,    -1,  1141,    -1,    -1,
      -1,    -1,  1146,   847,    -1,  1149,    -1,    -1,    -1,   853,
      -1,    -1,    -1,  1157,    -1,  1159,    -1,    -1,  1162,    -1,
      -1,   865,    -1,    -1,    -1,   869,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1182,  1183,
      -1,  1185,  1186,  1187,  1188,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   758,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1210,    -1,    -1,    -1,
      -1,  1215,  1216,    -1,  1218,    -1,  1220,    -1,  1222,  1223,
      -1,    -1,  1226,    -1,    -1,    -1,    -1,  1231,    -1,    -1,
      -1,  1235,    -1,    -1,   794,    -1,   940,    -1,   942,    -1,
    1244,   945,   802,   803,    -1,    -1,   598,    -1,   600,   601,
     602,   603,   604,   605,    -1,  1259,  1260,  1261,  1262,  1263,
    1264,  1265,  1266,    -1,    -1,   969,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   981,    -1,    -1,
      -1,    -1,   842,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   852,    -1,    -1,    -1,   856,    -1,   858,    -1,
      -1,   861,    -1,  1307,  1308,  1309,  1310,  1311,  1312,  1313,
    1314,  1315,  1316,  1317,  1318,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,  1041,    -1,    -1,
      -1,    -1,    -1,  1047,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,  1091,    -1,    -1,
     950,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,   977,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    -1,   100,    -1,   102,  1139,   104,  1141,    -1,   107,
      -1,    -1,  1146,   111,    -1,  1149,   114,    -1,    -1,   117,
      -1,   803,    -1,  1157,    -1,  1159,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1027,  1028,    -1,
      -1,    -1,    -1,  1033,  1034,    -1,    -1,  1037,  1038,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1191,    -1,    -1,
     842,    -1,    -1,  1197,    -1,    -1,    -1,  1201,  1202,    -1,
     852,    -1,    -1,   171,   856,   173,   858,    -1,    -1,   861,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1234,    -1,    -1,    31,  1238,    -1,    -1,    -1,    36,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1107,    -1,  1253,
      -1,  1111,  1112,  1113,  1114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1133,  1134,    73,  1136,    -1,  1138,    -1,
      -1,    -1,    -1,    -1,    -1,  1145,    -1,    -1,  1148,    -1,
      -1,  1151,    90,  1153,    -1,  1155,    94,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,  1199,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,   166,   167,
     168,   169,    -1,    -1,    -1,  1027,  1028,    -1,    -1,    90,
     178,  1033,  1034,    94,    -1,  1037,  1038,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    -1,   125,    -1,    -1,    -1,  1278,  1279,
      -1,    -1,  1282,  1283,  1284,    -1,  1286,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,  1107,   167,   168,   169,  1111,
    1112,  1113,  1114,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1133,  1134,    -1,  1136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1148,    -1,    -1,  1151,
      -1,  1153,    -1,  1155,     0,     1,    -1,     3,     4,     5,
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
     166,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,
      -1,     1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    12,    -1,    14,    15,    16,    17,    18,    19,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    71,    72,    -1,    74,    -1,    -1,    77,    78,    79,
      80,    -1,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,   105,    -1,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
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
      -1,   169,    -1,   171,   172,   173,    -1,   175,    -1,     1,
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
      -1,   171,    -1,   173,    -1,   175,    -1,     1,   178,     3,
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
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
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
      -1,   175,    -1,    -1,   178,     3,     4,     5,     6,     7,
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
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,    -1,   173,    -1,    -1,    -1,    -1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      12,    -1,    14,    -1,    -1,    17,    18,    19,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    55,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,   131,
      -1,    -1,   134,   135,   136,   137,    -1,   139,   140,    -1,
     142,   143,   144,   145,   146,   147,    -1,   149,    -1,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,   167,   168,   169,   170,   171,
      -1,   173,    -1,    -1,    -1,    -1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      -1,    17,    18,    -1,    20,    -1,    22,    23,    -1,    25,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      56,    -1,    -1,    59,    60,    -1,    62,    63,    64,    65,
      -1,    67,    68,    -1,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,    -1,   104,   105,
      -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,
      -1,    -1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    26,    27,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    79,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
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
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,   113,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
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
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,
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
     104,    -1,    -1,   107,   108,   109,    -1,   111,    -1,    -1,
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
      68,    69,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
     108,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,
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
      -1,    -1,   142,   143,   144,   145,   146,    -1,   148,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,
      -1,   171,   172,   173,    -1,    -1,    -1,    -1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,
     114,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
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
     108,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,
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
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    -1,    94,    -1,    -1,    97,    98,    99,   100,   101,
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
      -1,    -1,    78,    79,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,
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
      -1,    -1,    -1,   123,    -1,   125,    -1,    -1,    -1,    -1,
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
     144,   145,   146,    -1,   148,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
     174,    -1,    -1,    -1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    35,    -1,    -1,
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
      -1,   123,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
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
     100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,   172,   173,    -1,    -1,    -1,    -1,   178,     3,
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
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,   112,    -1,
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
      -1,    -1,    40,    41,    -1,    -1,    44,    -1,    -1,    47,
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
      -1,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
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
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,
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
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
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
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
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
      -1,    -1,    -1,    -1,   178,    -1,    12,    13,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    31,    -1,   125,    -1,    -1,
      36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    73,    -1,   167,
     168,   169,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    12,    -1,    90,    -1,    -1,    -1,    94,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      90,   167,   168,   169,    94,    -1,    -1,    12,    -1,    -1,
      -1,    -1,   178,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,   171,    -1,    12,    -1,    90,    -1,    -1,   178,    94,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    90,   167,   168,   169,    94,   171,    -1,    12,    -1,
      -1,    -1,    -1,   178,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    -1,   171,    -1,    12,    -1,    90,    -1,    -1,   178,
      94,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    31,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    90,   167,   168,   169,    94,   171,    -1,    12,
      -1,    -1,    -1,    -1,   178,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,   142,   143,    -1,   145,   146,   147,
     103,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,   125,    -1,    -1,    -1,    -1,    90,    -1,    -1,
     178,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   125,    -1,   167,   168,   169,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    12,    -1,   167,   168,   169,    -1,     3,    19,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    31,    17,    18,    -1,    20,    36,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    12,    -1,    39,    -1,    -1,    -1,    -1,    19,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    31,    32,    73,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      90,    -1,    -1,    78,    94,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    88,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    -1,   100,    31,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,   125,   111,   112,    -1,   114,
      90,    -1,   117,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   125,    -1,   167,   168,   169,
      -1,    -1,    -1,    12,    -1,    90,    -1,    -1,   178,    94,
      19,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    31,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,   121,   167,   168,   169,
     125,   171,    -1,    -1,    12,    13,    -1,    -1,   178,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    31,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    90,   167,   168,   169,    94,    -1,    -1,    -1,    -1,
      12,   176,    -1,   178,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,   125,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,   176,    90,   178,
      -1,    -1,    94,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    12,    -1,   167,
     168,   169,    -1,   125,    19,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    12,    -1,    94,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    31,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,   166,   167,   168,   169,    90,    -1,    -1,    -1,    94,
      -1,    -1,    -1,   178,    12,    13,    -1,    -1,   103,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,   125,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,   178,    -1,    -1,    94,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    12,    -1,   167,   168,   169,    -1,   125,    19,    -1,
     174,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    12,    -1,   167,
     168,   169,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    -1,
     125,    -1,    -1,    -1,    -1,    90,    -1,   178,    12,    94,
      -1,    -1,    -1,    -1,    -1,    19,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    31,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     125,    -1,   167,   168,   169,    -1,    -1,    -1,    12,    13,
      -1,    -1,    -1,   178,    -1,    19,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    31,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,    -1,    90,    -1,    -1,   174,
      94,    -1,    -1,   178,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   125,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,
     174,    -1,    90,    -1,   178,    -1,    94,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    12,    13,   167,   168,   169,    -1,   125,    19,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,    -1,    -1,    12,   174,    -1,    -1,    -1,
     178,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    90,   145,   146,   147,    94,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    12,    -1,   167,   168,   169,    -1,
      -1,    19,    -1,    -1,   121,    -1,    -1,   178,   125,    -1,
      -1,    -1,    -1,    31,    90,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,   125,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    90,   149,   150,   151,    94,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    12,
     166,   167,   168,   169,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,   124,   125,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    12,    -1,    90,    -1,    -1,
      -1,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    31,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    90,   167,   168,   169,    94,    -1,    -1,
      -1,    12,    13,    -1,    -1,   178,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,   125,
     167,   168,   169,    -1,    -1,    -1,    -1,    12,    -1,    90,
      -1,   178,    -1,    94,    19,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    31,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,   125,    -1,    -1,    -1,    12,    -1,
     176,    -1,   178,    -1,    -1,    19,    61,    -1,    -1,    -1,
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
      -1,   125,   167,   168,   169,    -1,    -1,    -1,    -1,    12,
      -1,    90,    -1,   178,    -1,    94,    19,    -1,   142,   143,
      -1,   145,   146,   147,   103,   149,   150,   151,    31,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,   125,    -1,    -1,    -1,
      12,    -1,    -1,    -1,   178,    -1,    -1,    19,    -1,    -1,
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
      -1,   142,   143,   103,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,   125,   167,   168,   169,    -1,
      -1,    -1,    -1,    12,    -1,    90,    -1,   178,    -1,    94,
      19,    -1,   142,   143,    -1,   145,   146,   147,   103,   149,
     150,   151,    31,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
     125,    -1,    -1,    -1,    12,    -1,    -1,    -1,   178,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    31,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    90,   167,   168,   169,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,   125,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,     1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,    -1,
       3,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    17,    18,    -1,    20,    -1,    78,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    97,    98,
      -1,   100,    45,   102,    -1,   104,    49,    -1,   107,    52,
      -1,    54,   111,    -1,    -1,   114,     3,    -1,   117,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      17,    18,    -1,    20,    -1,    78,    -1,    -1,    25,    -1,
      27,    -1,   141,    -1,    -1,    88,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    97,    98,    -1,   100,    45,   102,
      -1,   104,    49,    -1,   107,    52,    -1,    54,   111,    -1,
      -1,   114,     3,    -1,   117,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    17,    18,    -1,    20,
      -1,    78,    79,    -1,    25,    -1,    -1,    -1,   141,    -1,
      -1,    88,    33,    -1,    91,    -1,    -1,    -1,    39,    -1,
      97,    98,    -1,   100,    45,   102,    -1,   104,    49,    -1,
     107,    52,    -1,    54,   111,    -1,    -1,   114,     3,   116,
     117,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    17,    18,    -1,    20,    -1,    78,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    88,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    97,    98,    -1,   100,
      45,   102,    -1,   104,    49,    -1,   107,    52,    -1,    54,
     111,    -1,    -1,   114,     3,    -1,   117,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    17,    18,
      -1,    20,    -1,    78,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    97,    98,    -1,   100,    45,   102,    -1,   104,
      49,    -1,   107,    52,    -1,    54,   111,    -1,    -1,   114,
      -1,    -1,   117,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      -1,   100,    -1,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117
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
     232,   233,   234,   235,   236,   239,   240,   241,   244,   246,
     249,   250,   251,   252,   253,   257,   258,   263,   264,   265,
     266,   269,   270,   275,   276,   278,   279,   284,   288,   289,
     291,   292,   314,   319,   320,   324,   325,   346,   347,   348,
     350,   352,   353,   354,   362,   363,   364,   365,   366,   367,
     369,   372,   373,   374,   375,   376,   377,   378,   379,   381,
     382,   383,   384,   385,   166,    22,    35,    44,    56,    89,
     102,   171,   240,   249,   276,   346,   353,   363,   364,   369,
     372,   374,   375,   121,   355,   356,     3,    54,   219,   369,
     355,   110,   326,    91,   219,   187,   340,   369,   173,     3,
      17,    18,    20,    25,    33,    39,    45,    49,    52,    54,
      63,    70,    71,    78,    88,    97,    98,   100,   102,   104,
     107,   111,   114,   117,   220,   221,   223,    11,    27,   116,
     245,   369,    83,    86,   203,   173,   221,   221,   221,   173,
     221,   327,   328,    32,   207,   226,   369,   261,   262,   369,
     369,    18,    78,    97,   114,   369,   369,   369,     8,   173,
     230,   229,    27,    33,    47,    49,    51,    76,    79,    91,
      98,   102,   110,   186,   225,   280,   281,   282,   305,   306,
     307,   332,   337,   369,   340,   108,   109,   166,   284,   285,
     368,   369,   371,   369,   226,   369,   369,   369,   101,   173,
     187,   369,   369,   189,   193,   207,   189,   207,   369,   371,
       3,   369,   369,   369,   369,   369,     1,   172,   185,   208,
     340,   112,   152,   341,   342,   371,   369,    81,   187,    22,
      35,    38,    89,   171,   190,   191,   192,   203,   207,   195,
       8,   150,   197,   171,    46,    85,   115,   204,    26,   325,
     369,     8,   265,   369,   370,    24,    32,    40,    41,    42,
     120,   175,   242,   349,   351,    56,   147,   267,   221,     1,
     194,   221,   271,   274,   173,   300,    53,    74,    84,   309,
      27,    76,    79,    91,   110,   310,    27,    79,    91,   110,
     308,   221,   321,   322,   327,   165,   166,   369,    12,    19,
      31,    90,    94,   125,   142,   143,   145,   146,   147,   149,
     150,   151,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   167,   168,   169,   178,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   170,   296,   173,   175,    90,    94,   369,
     221,    40,    41,   175,    56,   173,   187,   166,   207,   324,
     166,   148,   166,   222,   343,   344,   345,   371,   171,   221,
     221,   103,   207,   103,   126,   207,   296,   204,   343,   173,
     173,   207,   187,   112,   173,   221,   329,   330,     1,   147,
     336,    47,   148,   187,   226,   148,   226,    13,   173,   173,
     226,   343,   231,   231,    47,    91,   306,   307,   174,   148,
     112,   173,   221,   304,   147,   166,   369,   369,   124,   286,
     166,   171,   226,   173,   343,   166,   254,   254,   166,   172,
     172,   185,   148,   172,   369,   148,   174,   148,   174,     8,
      40,    41,   175,   242,     3,   173,   198,     1,   172,   208,
     215,   216,   369,   210,   369,    66,    36,    73,   166,   265,
     267,   110,   237,   289,   369,   187,    79,   123,   243,   369,
     243,   369,   243,   369,   245,   369,   176,   243,   340,   369,
     380,   221,   340,   171,   172,   274,   126,   148,   172,   174,
     301,   303,   305,   310,    91,     1,   147,   334,   335,    91,
       1,     3,    12,    17,    19,    20,    25,    45,    52,    54,
      55,    63,    71,    88,   100,   104,   111,   117,   142,   143,
     144,   145,   146,   147,   149,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   164,   167,   168,   169,   170,
     173,   221,   293,   294,   295,   296,   346,   126,   323,   148,
     166,   166,   369,   369,   369,   243,   369,   243,   369,   369,
     369,   369,   369,   369,   369,     3,    20,    33,    63,   104,
     110,   222,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   369,    69,
     371,   371,   371,   371,   371,   343,   343,   243,   369,   243,
     369,   103,   173,   369,   369,   340,   221,    27,    47,    91,
     116,   357,   360,   361,   369,   385,   327,   369,   126,   174,
     148,   126,   126,   187,    34,   187,   369,    34,   369,    66,
     174,   344,   221,   247,   248,   330,   148,   174,   225,   369,
     126,   331,   369,   328,   261,   221,   324,   369,   369,   174,
     304,   310,   282,   330,   147,   338,   339,   337,   287,   206,
       1,   259,   345,   174,    21,   255,   172,   174,   174,   341,
     174,   341,   369,   369,   340,   197,   174,   199,   200,   345,
     172,   172,   148,   166,    13,   150,   148,   166,    13,    36,
      73,   221,   142,   143,   144,   145,   146,   160,   164,   169,
     209,   295,   296,   297,   369,   209,   211,   267,   171,   238,
     324,    47,   355,   120,   221,   173,   226,    32,    47,   226,
     121,   226,   358,    32,    47,   226,   358,   226,   358,    47,
     226,   358,   226,   226,   369,   176,   358,    47,   148,   176,
     358,   124,   148,   176,   166,   173,     1,   268,   369,   272,
     273,   148,   174,   336,     1,   225,   332,   333,   106,   311,
     173,   299,   369,   142,   150,   299,   299,   369,   322,   173,
     175,   166,   166,   166,   166,   166,   166,   174,   176,   344,
      32,    47,    32,    47,    47,   176,   173,    47,    91,   148,
     174,    17,    20,    25,    45,    52,    63,    71,    88,   100,
     111,   117,   346,    90,    90,   166,   371,   345,   369,   369,
      34,   187,    34,    34,   187,   103,   207,   221,   174,   148,
     174,   174,   329,   336,    69,   371,   221,   174,   174,   339,
     147,   283,   174,   337,   152,   298,   331,   369,   172,    75,
     118,   172,   260,   174,   173,   207,   221,   256,    47,    47,
      47,   176,   148,   174,   215,   222,    17,    18,    20,    25,
      45,    49,    52,    71,    78,    88,    97,    98,   100,   102,
     111,   114,   117,   171,   214,   297,   369,   112,   369,   209,
     211,   148,   166,    13,   166,   171,   268,   321,   327,   243,
     369,   226,   369,    47,   340,    44,   369,   243,   369,   173,
     226,    44,   369,   243,   369,   226,   226,   243,   369,   226,
     187,   176,   243,   369,   176,   187,   369,   176,   369,   176,
     369,   344,   172,    83,    86,   172,   186,   194,   228,   274,
     194,   303,   311,    61,   119,   315,   301,   302,   174,   294,
     296,   174,   176,   174,    44,   243,   369,    44,   243,   369,
     243,   369,   344,   360,   336,   346,   346,   187,   187,   369,
     187,    34,   166,   166,   248,   207,   331,   173,   173,   298,
     331,   332,   311,   339,   369,   187,   240,   340,   256,   147,
     207,   243,   369,   243,   369,   243,   369,   200,   209,    13,
      36,    73,    36,    73,   166,   166,   297,   369,   369,   268,
     172,   166,   166,   355,   355,   166,   369,   174,   369,    32,
     226,    32,   226,   359,   360,   369,    32,   226,   358,    32,
     226,   358,   226,   358,   226,   358,   187,   176,   358,   176,
     358,   187,   124,   174,   194,   228,   228,   274,   315,     3,
      54,    93,   104,   316,   317,   318,   369,   290,   174,   299,
     299,   369,    32,    32,   369,    32,    32,   176,   176,   174,
     331,    34,   187,   343,   343,   298,   331,    32,   226,   174,
     369,   176,   176,   172,   112,   369,   209,   211,   209,   211,
      13,   172,   226,   226,   226,   103,    44,   369,    44,   369,
     148,   174,   103,    44,   369,   226,    44,   369,   226,   226,
     226,    44,   187,   369,   176,    44,   187,   369,   176,   369,
     166,   228,   277,   318,   119,   148,   126,   153,   155,   156,
     158,   159,    61,    32,   166,   206,   227,   312,   313,   103,
      44,    44,   103,    44,    44,    44,    44,   187,   174,   174,
     187,   207,   166,   166,   166,   166,   369,   369,   369,   369,
     360,   369,   369,   369,   369,   187,   369,   187,   313,   369,
     317,   318,   318,   318,   318,   318,   318,   316,   185,   369,
     369,   369,   369,   369,   369,   165,   165,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   369,
     369,   369,   369,   369,   369
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   182,   183,   184,   184,   185,   185,   186,   186,   187,
     187,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   190,   190,   191,   192,   192,
     193,   194,   194,   195,   195,   195,   196,   197,   197,   198,
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
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   243,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   245,   245,   246,   246,
     247,   247,   248,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   250,   250,   250,   250,
     250,   250,   251,   251,   251,   252,   252,   252,   253,   253,
     253,   253,   254,   254,   255,   255,   255,   256,   256,   257,
     258,   258,   259,   259,   260,   260,   260,   261,   261,   261,
     262,   262,   263,   264,   264,   265,   266,   266,   266,   267,
     267,   268,   268,   268,   268,   268,   269,   269,   270,   271,
     271,   272,   271,   271,   273,   271,   274,   274,   275,   277,
     276,   278,   279,   279,   279,   280,   280,   281,   281,   282,
     282,   282,   283,   283,   284,   286,   285,   287,   285,   288,
     290,   289,   291,   291,   291,   291,   291,   292,   293,   293,
     294,   294,   294,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     297,   297,   298,   298,   299,   299,   300,   300,   301,   301,
     302,   302,   303,   304,   304,   305,   305,   305,   305,   305,
     305,   306,   306,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   308,   308,   308,   308,   308,   308,   309,   309,
     309,   310,   310,   310,   310,   310,   310,   310,   311,   311,
     312,   312,   313,   313,   313,   314,   315,   315,   315,   315,
     315,   316,   316,   317,   317,   317,   317,   317,   317,   317,
     318,   318,   318,   319,   320,   320,   321,   321,   322,   323,
     323,   324,   324,   324,   324,   324,   326,   325,   325,   327,
     327,   328,   328,   329,   329,   329,   330,   330,   330,   331,
     331,   331,   332,   333,   333,   333,   334,   334,   335,   335,
     336,   336,   336,   336,   337,   337,   338,   339,   339,   340,
     340,   341,   341,   342,   342,   343,   343,   344,   344,   345,
     345,   346,   346,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   348,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   350,   351,
     351,   351,   351,   351,   351,   351,   352,   353,   354,   354,
     354,   354,   354,   354,   354,   355,   355,   356,   357,   357,
     358,   359,   359,   360,   360,   360,   361,   361,   361,   361,
     361,   361,   362,   362,   362,   362,   362,   363,   363,   363,
     363,   363,   364,   365,   365,   365,   365,   365,   365,   366,
     367,   368,   368,   369,   369,   369,   369,   369,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   369,   369,
     370,   370,   371,   371,   371,   372,   372,   372,   372,   373,
     373,   373,   373,   373,   374,   374,   374,   375,   375,   375,
     375,   375,   375,   376,   376,   376,   376,   377,   377,   378,
     378,   379,   379,   379,   379,   379,   379,   379,   379,   379,
     379,   379,   379,   379,   380,   380,   381,   381,   381,   381,
     381,   381,   381,   381,   381,   381,   381,   381,   381,   381,
     381,   381,   381,   381,   381,   381,   381,   381,   381,   382,
     382,   382,   382,   382,   382,   382,   383,   383,   383,   383,
     384,   384,   384,   384,   385,   385,   385,   385,   385,   385,
     385
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     2,     3,     1,     1,     0,     3,
       0,     3,     1,     1,     1,     1,     1,     1,     1,     3,
       5,     5,     2,     1,     1,     1,     1,     6,     7,     3,
       0,     6,     0,     6,     2,     0,     3,     2,     5,     3,
       3,     6,     6,     4,     5,     5,     3,     3,     6,     5,
       6,     5,     6,     3,     4,     3,     4,     5,     6,     5,
       6,     3,     4,     3,     4,     6,     7,     6,     7,     4,
       5,     4,     5,     4,     4,     3,     6,     5,     4,     3,
       6,     5,     6,     5,     8,     7,     4,     4,     6,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     6,     4,     7,     5,     3,     6,     2,     2,
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
       1,     1,     1,     3,     1,     2,     1,     3,     2,     0,
       2,     1,     2,     1,     1,     1,     0,     5,     3,     1,
       3,     3,     5,     1,     1,     3,     1,     2,     3,     0,
       2,     2,     1,     1,     1,     1,     2,     1,     0,     1,
       0,     2,     2,     1,     1,     1,     2,     0,     1,     1,
       3,     1,     1,     3,     3,     0,     1,     1,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     4,
       4,     4,     5,     2,     2,     1,     2,     1,     2,     1,
       2,     1,     2,     1,     1,     2,     6,     6,     4,     9,
       9,     7,     6,     6,     4,     9,     9,     7,     2,     2,
       3,     4,     6,     6,     9,     9,     6,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     4,     1,     3,
       4,     1,     3,     4,     3,     3,     1,     1,     2,     1,
       2,     1,     1,     2,     2,     2,     2,     2,     8,     8,
       9,     9,     4,     3,     3,     2,     2,     2,     1,     3,
       4,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       2,     2,     1,     1,     4,     4,     4,     3,     3,     3,
       3,     5,     5,     3,     4,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     4,
       4,     5,     4,     5,     3,     5,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     1,
       1
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
#line 6501 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 639 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6507 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 640 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6513 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 647 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6521 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 655 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6529 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 659 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6538 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 669 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6546 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 674 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6554 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 681 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6560 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 682 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6566 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 683 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6572 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 684 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6578 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 685 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6584 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6590 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 687 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6596 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 688 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6602 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 689 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6608 "bison-chpl-lib.cpp"
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
#line 6625 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 703 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6631 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 704 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6637 "bison-chpl-lib.cpp"
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
#line 6650 "bison-chpl-lib.cpp"
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
#line 6663 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TLABEL ident_def stmt  */
#line 724 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6671 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 728 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6682 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: error TSEMI  */
#line 735 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6691 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: assignment_stmt  */
#line 742 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6697 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: if_stmt  */
#line 743 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6703 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: loop_stmt  */
#line 744 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6709 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: select_stmt  */
#line 745 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6715 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: manage_stmt  */
#line 746 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6721 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 747 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6727 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: throw_stmt  */
#line 748 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6733 "bison-chpl-lib.cpp"
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
#line 6751 "bison-chpl-lib.cpp"
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
#line 6770 "bison-chpl-lib.cpp"
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
#line 6782 "bison-chpl-lib.cpp"
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
#line 6798 "bison-chpl-lib.cpp"
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
#line 6813 "bison-chpl-lib.cpp"
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
#line 6829 "bison-chpl-lib.cpp"
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
#line 6845 "bison-chpl-lib.cpp"
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
#line 6860 "bison-chpl-lib.cpp"
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
#line 6877 "bison-chpl-lib.cpp"
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
#line 6889 "bison-chpl-lib.cpp"
    break;

  case 53: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 890 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), true, (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6897 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace  */
#line 894 "chpl.ypp"
 {
  // this allows us to distinguish between @attribute() and @attribute
  // but should not be needed once `@unstable "msg"` is deprecated and gone
  auto lst = new MaybeNamedActualList();
  context->noteAttribute((yylsp[-1]), (yyvsp[-1].expr), false, (yyvsp[0].exprList), lst);
 }
#line 6908 "bison-chpl-lib.cpp"
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
#line 6921 "bison-chpl-lib.cpp"
    break;

  case 56: /* attribute_decl_begin: TATMARK TIDENT  */
#line 913 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6930 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: %empty  */
#line 920 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6938 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 924 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6949 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP TRP  */
#line 934 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6955 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 936 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6963 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 943 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6972 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 948 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6982 "bison-chpl-lib.cpp"
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
#line 7003 "bison-chpl-lib.cpp"
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
#line 7022 "bison-chpl-lib.cpp"
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
#line 7041 "bison-chpl-lib.cpp"
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
#line 7058 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1026 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 7064 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1027 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 7071 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1029 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 7078 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1034 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 7084 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1035 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 7091 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1041 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 7101 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1059 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 7109 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1063 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 7117 "bison-chpl-lib.cpp"
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
#line 7129 "bison-chpl-lib.cpp"
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
#line 7148 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1096 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 7154 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1097 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7160 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1102 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 7168 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1106 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7176 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1110 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 7185 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1115 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 7193 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1119 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7201 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1123 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 7210 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1133 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7219 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1138 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7229 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1144 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7240 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1151 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 7249 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1156 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7260 "bison-chpl-lib.cpp"
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
#line 7272 "bison-chpl-lib.cpp"
    break;

  case 92: /* opt_only_ls: %empty  */
#line 1173 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7278 "bison-chpl-lib.cpp"
    break;

  case 93: /* opt_only_ls: renames_ls  */
#line 1174 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7284 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1179 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7293 "bison-chpl-lib.cpp"
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
#line 7305 "bison-chpl-lib.cpp"
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
#line 7318 "bison-chpl-lib.cpp"
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
#line 7332 "bison-chpl-lib.cpp"
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
#line 7344 "bison-chpl-lib.cpp"
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
#line 7357 "bison-chpl-lib.cpp"
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
#line 7371 "bison-chpl-lib.cpp"
    break;

  case 101: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1241 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7380 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_expr: expr  */
#line 1277 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 7388 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_expr: expr TDOT all_op_name  */
#line 1281 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7397 "bison-chpl-lib.cpp"
    break;

  case 121: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1286 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7406 "bison-chpl-lib.cpp"
    break;

  case 122: /* import_expr: expr TAS ident_use  */
#line 1291 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 7416 "bison-chpl-lib.cpp"
    break;

  case 123: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1297 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 7426 "bison-chpl-lib.cpp"
    break;

  case 124: /* import_ls: import_expr  */
#line 1305 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7432 "bison-chpl-lib.cpp"
    break;

  case 125: /* import_ls: import_ls TCOMMA import_expr  */
#line 1306 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7438 "bison-chpl-lib.cpp"
    break;

  case 126: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1311 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7448 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1320 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7456 "bison-chpl-lib.cpp"
    break;

  case 128: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1324 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7464 "bison-chpl-lib.cpp"
    break;

  case 129: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1328 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7472 "bison-chpl-lib.cpp"
    break;

  case 130: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1332 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7480 "bison-chpl-lib.cpp"
    break;

  case 131: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1336 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7488 "bison-chpl-lib.cpp"
    break;

  case 132: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1340 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7496 "bison-chpl-lib.cpp"
    break;

  case 133: /* opt_label_ident: %empty  */
#line 1348 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7502 "bison-chpl-lib.cpp"
    break;

  case 134: /* opt_label_ident: TIDENT  */
#line 1349 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7508 "bison-chpl-lib.cpp"
    break;

  case 135: /* opt_label_ident: TINIT  */
#line 1350 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7514 "bison-chpl-lib.cpp"
    break;

  case 144: /* ident_use: TIDENT  */
#line 1376 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7520 "bison-chpl-lib.cpp"
    break;

  case 145: /* ident_use: TINIT  */
#line 1377 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7526 "bison-chpl-lib.cpp"
    break;

  case 146: /* ident_use: TTHIS  */
#line 1378 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7532 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TBOOL  */
#line 1405 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7538 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TENUM  */
#line 1406 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7544 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TINT  */
#line 1407 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7550 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TUINT  */
#line 1408 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7556 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TREAL  */
#line 1409 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7562 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TIMAG  */
#line 1410 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7568 "bison-chpl-lib.cpp"
    break;

  case 172: /* scalar_type: TCOMPLEX  */
#line 1411 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7574 "bison-chpl-lib.cpp"
    break;

  case 173: /* scalar_type: TBYTES  */
#line 1412 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7580 "bison-chpl-lib.cpp"
    break;

  case 174: /* scalar_type: TSTRING  */
#line 1413 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7586 "bison-chpl-lib.cpp"
    break;

  case 175: /* scalar_type: TLOCALE  */
#line 1414 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7592 "bison-chpl-lib.cpp"
    break;

  case 176: /* scalar_type: TNOTHING  */
#line 1415 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7598 "bison-chpl-lib.cpp"
    break;

  case 177: /* scalar_type: TVOID  */
#line 1416 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7604 "bison-chpl-lib.cpp"
    break;

  case 182: /* do_stmt: TDO stmt  */
#line 1430 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7610 "bison-chpl-lib.cpp"
    break;

  case 183: /* do_stmt: block_stmt  */
#line 1431 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7616 "bison-chpl-lib.cpp"
    break;

  case 184: /* return_stmt: TRETURN TSEMI  */
#line 1436 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7627 "bison-chpl-lib.cpp"
    break;

  case 185: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1443 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7638 "bison-chpl-lib.cpp"
    break;

  case 186: /* class_level_stmt: TSEMI  */
#line 1453 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7646 "bison-chpl-lib.cpp"
    break;

  case 187: /* class_level_stmt: inner_class_level_stmt  */
#line 1457 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7656 "bison-chpl-lib.cpp"
    break;

  case 188: /* $@1: %empty  */
#line 1462 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7663 "bison-chpl-lib.cpp"
    break;

  case 189: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1464 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7672 "bison-chpl-lib.cpp"
    break;

  case 190: /* $@2: %empty  */
#line 1468 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7679 "bison-chpl-lib.cpp"
    break;

  case 191: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1470 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7688 "bison-chpl-lib.cpp"
    break;

  case 199: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1488 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7697 "bison-chpl-lib.cpp"
    break;

  case 200: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1493 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7706 "bison-chpl-lib.cpp"
    break;

  case 201: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1498 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7715 "bison-chpl-lib.cpp"
    break;

  case 202: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1503 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7723 "bison-chpl-lib.cpp"
    break;

  case 203: /* forwarding_decl_start: TFORWARDING  */
#line 1510 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7732 "bison-chpl-lib.cpp"
    break;

  case 204: /* extern_or_export: TEXTERN  */
#line 1517 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7738 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_or_export: TEXPORT  */
#line 1518 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7744 "bison-chpl-lib.cpp"
    break;

  case 206: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1523 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7754 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1533 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7763 "bison-chpl-lib.cpp"
    break;

  case 208: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1539 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7774 "bison-chpl-lib.cpp"
    break;

  case 209: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
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
#line 7796 "bison-chpl-lib.cpp"
    break;

  case 210: /* $@3: %empty  */
#line 1564 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7804 "bison-chpl-lib.cpp"
    break;

  case 211: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1568 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7813 "bison-chpl-lib.cpp"
    break;

  case 212: /* $@4: %empty  */
#line 1573 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7822 "bison-chpl-lib.cpp"
    break;

  case 213: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1578 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7831 "bison-chpl-lib.cpp"
    break;

  case 214: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1586 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7839 "bison-chpl-lib.cpp"
    break;

  case 215: /* no_loop_attributes: %empty  */
#line 1591 "chpl.ypp"
                    { context->loopAttributes.push_back(nullptr); }
#line 7845 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1594 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7851 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1595 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7857 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
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
#line 7880 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: TWHILE expr do_stmt  */
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
#line 7898 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: TWHILE ifvar do_stmt  */
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
#line 7916 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1647 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7924 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1651 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7932 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1655 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7940 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: TFOR expr TIN expr do_stmt  */
#line 1659 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7948 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: TFOR expr TIN zippered_iterator do_stmt  */
#line 1663 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7956 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: TFOR expr do_stmt  */
#line 1667 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7964 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: TFOR zippered_iterator do_stmt  */
#line 1671 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7972 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
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
#line 7996 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: TFORALL expr TIN expr do_stmt  */
#line 1695 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8004 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1699 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8012 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1703 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8020 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1707 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8028 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TFORALL expr do_stmt  */
#line 1711 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8036 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TFORALL expr forall_intent_clause do_stmt  */
#line 1715 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8044 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: TFORALL zippered_iterator do_stmt  */
#line 1719 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8052 "bison-chpl-lib.cpp"
    break;

  case 236: /* loop_stmt_base: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1723 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8060 "bison-chpl-lib.cpp"
    break;

  case 237: /* loop_stmt_base: TFOREACH expr TIN expr do_stmt  */
#line 1727 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8068 "bison-chpl-lib.cpp"
    break;

  case 238: /* loop_stmt_base: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1731 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8076 "bison-chpl-lib.cpp"
    break;

  case 239: /* loop_stmt_base: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1735 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8084 "bison-chpl-lib.cpp"
    break;

  case 240: /* loop_stmt_base: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1739 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8092 "bison-chpl-lib.cpp"
    break;

  case 241: /* loop_stmt_base: TFOREACH expr do_stmt  */
#line 1743 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8100 "bison-chpl-lib.cpp"
    break;

  case 242: /* loop_stmt_base: TFOREACH expr forall_intent_clause do_stmt  */
#line 1747 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8108 "bison-chpl-lib.cpp"
    break;

  case 243: /* loop_stmt_base: TFOREACH zippered_iterator do_stmt  */
#line 1751 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8116 "bison-chpl-lib.cpp"
    break;

  case 244: /* loop_stmt_base: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1755 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8124 "bison-chpl-lib.cpp"
    break;

  case 245: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1759 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8132 "bison-chpl-lib.cpp"
    break;

  case 246: /* loop_stmt_base: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1763 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8140 "bison-chpl-lib.cpp"
    break;

  case 247: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1767 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8148 "bison-chpl-lib.cpp"
    break;

  case 248: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1771 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8156 "bison-chpl-lib.cpp"
    break;

  case 249: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1775 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8164 "bison-chpl-lib.cpp"
    break;

  case 250: /* loop_stmt_base: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1779 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8172 "bison-chpl-lib.cpp"
    break;

  case 251: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1783 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8181 "bison-chpl-lib.cpp"
    break;

  case 252: /* loop_stmt_base: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1788 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8190 "bison-chpl-lib.cpp"
    break;

  case 253: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1796 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 8200 "bison-chpl-lib.cpp"
    break;

  case 254: /* if_stmt: TIF expr TTHEN stmt  */
#line 1805 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8208 "bison-chpl-lib.cpp"
    break;

  case 255: /* if_stmt: TIF expr block_stmt  */
#line 1809 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8216 "bison-chpl-lib.cpp"
    break;

  case 256: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1813 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8224 "bison-chpl-lib.cpp"
    break;

  case 257: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1817 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8232 "bison-chpl-lib.cpp"
    break;

  case 258: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1821 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8240 "bison-chpl-lib.cpp"
    break;

  case 259: /* if_stmt: TIF ifvar block_stmt  */
#line 1825 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8248 "bison-chpl-lib.cpp"
    break;

  case 260: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1829 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8256 "bison-chpl-lib.cpp"
    break;

  case 261: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1833 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8264 "bison-chpl-lib.cpp"
    break;

  case 262: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1837 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8273 "bison-chpl-lib.cpp"
    break;

  case 263: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1842 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8282 "bison-chpl-lib.cpp"
    break;

  case 264: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1847 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8291 "bison-chpl-lib.cpp"
    break;

  case 265: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1852 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8300 "bison-chpl-lib.cpp"
    break;

  case 266: /* ifvar: TVAR ident_def TASSIGN expr  */
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
#line 8319 "bison-chpl-lib.cpp"
    break;

  case 267: /* ifvar: TCONST ident_def TASSIGN expr  */
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
#line 8338 "bison-chpl-lib.cpp"
    break;

  case 268: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1893 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8346 "bison-chpl-lib.cpp"
    break;

  case 269: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1897 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8354 "bison-chpl-lib.cpp"
    break;

  case 270: /* ifc_formal_ls: ifc_formal  */
#line 1903 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8360 "bison-chpl-lib.cpp"
    break;

  case 271: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1904 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8366 "bison-chpl-lib.cpp"
    break;

  case 272: /* ifc_formal: ident_def  */
#line 1909 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8372 "bison-chpl-lib.cpp"
    break;

  case 285: /* implements_type_ident: implements_type_error_ident  */
#line 1927 "chpl.ypp"
  {
    context->syntax((yyloc), "type '%s' cannot implement an interface.", (yyvsp[0].uniqueStr).c_str());
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8381 "bison-chpl-lib.cpp"
    break;

  case 292: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1953 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8389 "bison-chpl-lib.cpp"
    break;

  case 293: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1957 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 8397 "bison-chpl-lib.cpp"
    break;

  case 294: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1961 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8405 "bison-chpl-lib.cpp"
    break;

  case 295: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1968 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8413 "bison-chpl-lib.cpp"
    break;

  case 296: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1972 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 8421 "bison-chpl-lib.cpp"
    break;

  case 297: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1976 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8429 "bison-chpl-lib.cpp"
    break;

  case 298: /* try_stmt: TTRY tryable_stmt  */
#line 1983 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8437 "bison-chpl-lib.cpp"
    break;

  case 299: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1987 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8445 "bison-chpl-lib.cpp"
    break;

  case 300: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1991 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8453 "bison-chpl-lib.cpp"
    break;

  case 301: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1995 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8461 "bison-chpl-lib.cpp"
    break;

  case 302: /* catch_expr_ls: %empty  */
#line 2001 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8467 "bison-chpl-lib.cpp"
    break;

  case 303: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2002 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8473 "bison-chpl-lib.cpp"
    break;

  case 304: /* catch_expr: TCATCH block_stmt  */
#line 2007 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8481 "bison-chpl-lib.cpp"
    break;

  case 305: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2011 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8489 "bison-chpl-lib.cpp"
    break;

  case 306: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2015 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8497 "bison-chpl-lib.cpp"
    break;

  case 307: /* catch_expr_inner: ident_def  */
#line 2022 "chpl.ypp"
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
#line 8514 "bison-chpl-lib.cpp"
    break;

  case 308: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2035 "chpl.ypp"
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
#line 8531 "bison-chpl-lib.cpp"
    break;

  case 309: /* throw_stmt: TTHROW expr TSEMI  */
#line 2051 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8541 "bison-chpl-lib.cpp"
    break;

  case 310: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2059 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8549 "bison-chpl-lib.cpp"
    break;

  case 311: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2063 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8559 "bison-chpl-lib.cpp"
    break;

  case 312: /* when_stmt_ls: %empty  */
#line 2071 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8565 "bison-chpl-lib.cpp"
    break;

  case 313: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2072 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8571 "bison-chpl-lib.cpp"
    break;

  case 314: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2077 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8579 "bison-chpl-lib.cpp"
    break;

  case 315: /* when_stmt: TOTHERWISE stmt  */
#line 2081 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8588 "bison-chpl-lib.cpp"
    break;

  case 316: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2086 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 8598 "bison-chpl-lib.cpp"
    break;

  case 317: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2095 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8607 "bison-chpl-lib.cpp"
    break;

  case 318: /* manager_expr: expr TAS ident_def  */
#line 2100 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8615 "bison-chpl-lib.cpp"
    break;

  case 319: /* manager_expr: expr  */
#line 2104 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8623 "bison-chpl-lib.cpp"
    break;

  case 320: /* manager_expr_ls: manager_expr  */
#line 2110 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8629 "bison-chpl-lib.cpp"
    break;

  case 321: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2111 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8635 "bison-chpl-lib.cpp"
    break;

  case 322: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2116 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8643 "bison-chpl-lib.cpp"
    break;

  case 323: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2125 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8652 "bison-chpl-lib.cpp"
    break;

  case 324: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2130 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8663 "bison-chpl-lib.cpp"
    break;

  case 325: /* class_start: class_tag ident_def  */
#line 2141 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8671 "bison-chpl-lib.cpp"
    break;

  case 326: /* class_tag: TCLASS  */
#line 2147 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8677 "bison-chpl-lib.cpp"
    break;

  case 327: /* class_tag: TRECORD  */
#line 2148 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8683 "bison-chpl-lib.cpp"
    break;

  case 328: /* class_tag: TUNION  */
#line 2149 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8689 "bison-chpl-lib.cpp"
    break;

  case 329: /* opt_inherit: %empty  */
#line 2153 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8695 "bison-chpl-lib.cpp"
    break;

  case 330: /* opt_inherit: TCOLON expr_ls  */
#line 2154 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8701 "bison-chpl-lib.cpp"
    break;

  case 331: /* class_level_stmt_ls: %empty  */
#line 2158 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8710 "bison-chpl-lib.cpp"
    break;

  case 332: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2163 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8718 "bison-chpl-lib.cpp"
    break;

  case 333: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2167 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8726 "bison-chpl-lib.cpp"
    break;

  case 334: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2171 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8734 "bison-chpl-lib.cpp"
    break;

  case 335: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2175 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8742 "bison-chpl-lib.cpp"
    break;

  case 336: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2182 "chpl.ypp"
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
#line 8763 "bison-chpl-lib.cpp"
    break;

  case 337: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2199 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8777 "bison-chpl-lib.cpp"
    break;

  case 338: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2212 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8785 "bison-chpl-lib.cpp"
    break;

  case 339: /* enum_ls: enum_item  */
#line 2219 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8794 "bison-chpl-lib.cpp"
    break;

  case 340: /* enum_ls: enum_ls TCOMMA  */
#line 2224 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8804 "bison-chpl-lib.cpp"
    break;

  case 341: /* $@5: %empty  */
#line 2230 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8813 "bison-chpl-lib.cpp"
    break;

  case 342: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2235 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8822 "bison-chpl-lib.cpp"
    break;

  case 343: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2240 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8831 "bison-chpl-lib.cpp"
    break;

  case 344: /* $@6: %empty  */
#line 2245 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8840 "bison-chpl-lib.cpp"
    break;

  case 345: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2250 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8849 "bison-chpl-lib.cpp"
    break;

  case 346: /* enum_item: ident_def  */
#line 2260 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8860 "bison-chpl-lib.cpp"
    break;

  case 347: /* enum_item: ident_def TASSIGN expr  */
#line 2267 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8873 "bison-chpl-lib.cpp"
    break;

  case 348: /* lambda_decl_start: TLAMBDA  */
#line 2279 "chpl.ypp"
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
#line 8889 "bison-chpl-lib.cpp"
    break;

  case 349: /* $@7: %empty  */
#line 2296 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8899 "bison-chpl-lib.cpp"
    break;

  case 350: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2302 "chpl.ypp"
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
#line 8921 "bison-chpl-lib.cpp"
    break;

  case 352: /* linkage_spec: linkage_spec_empty  */
#line 2325 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8927 "bison-chpl-lib.cpp"
    break;

  case 353: /* linkage_spec: TINLINE  */
#line 2326 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8934 "bison-chpl-lib.cpp"
    break;

  case 354: /* linkage_spec: TOVERRIDE  */
#line 2328 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8941 "bison-chpl-lib.cpp"
    break;

  case 355: /* opt_fn_type_formal_ls: %empty  */
#line 2333 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8947 "bison-chpl-lib.cpp"
    break;

  case 356: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2334 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8953 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_type_formal_ls: fn_type_formal  */
#line 2338 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8959 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2339 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8965 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_type_formal: named_formal  */
#line 2344 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8971 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2347 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8977 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_type_formal: formal_type  */
#line 2349 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8983 "bison-chpl-lib.cpp"
    break;

  case 362: /* opt_fn_type_ret_type: %empty  */
#line 2353 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8989 "bison-chpl-lib.cpp"
    break;

  case 363: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2354 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8995 "bison-chpl-lib.cpp"
    break;

  case 364: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2367 "chpl.ypp"
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
#line 9014 "bison-chpl-lib.cpp"
    break;

  case 365: /* $@8: %empty  */
#line 2385 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 9024 "bison-chpl-lib.cpp"
    break;

  case 366: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2391 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 9037 "bison-chpl-lib.cpp"
    break;

  case 367: /* $@9: %empty  */
#line 2400 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 9047 "bison-chpl-lib.cpp"
    break;

  case 368: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2406 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 9060 "bison-chpl-lib.cpp"
    break;

  case 369: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2418 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 9068 "bison-chpl-lib.cpp"
    break;

  case 370: /* $@10: %empty  */
#line 2427 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();

    // May never have been built if there was a syntax error.
    if ((yyvsp[-4].functionParts).errorExpr == nullptr) {
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name->name());
    }
  }
#line 9082 "bison-chpl-lib.cpp"
    break;

  case 371: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2437 "chpl.ypp"
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
#line 9105 "bison-chpl-lib.cpp"
    break;

  case 372: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2459 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9117 "bison-chpl-lib.cpp"
    break;

  case 373: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2467 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9129 "bison-chpl-lib.cpp"
    break;

  case 374: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2475 "chpl.ypp"
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
#line 9144 "bison-chpl-lib.cpp"
    break;

  case 375: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2486 "chpl.ypp"
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
#line 9159 "bison-chpl-lib.cpp"
    break;

  case 376: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2497 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 9169 "bison-chpl-lib.cpp"
    break;

  case 377: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2506 "chpl.ypp"
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
#line 9185 "bison-chpl-lib.cpp"
    break;

  case 379: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2521 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 9191 "bison-chpl-lib.cpp"
    break;

  case 382: /* fn_ident: ident_def TBANG  */
#line 2528 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 9201 "bison-chpl-lib.cpp"
    break;

  case 422: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2585 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 9207 "bison-chpl-lib.cpp"
    break;

  case 423: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2586 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9213 "bison-chpl-lib.cpp"
    break;

  case 424: /* opt_formal_ls: %empty  */
#line 2590 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 9219 "bison-chpl-lib.cpp"
    break;

  case 425: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2591 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9225 "bison-chpl-lib.cpp"
    break;

  case 426: /* req_formal_ls: TLP TRP  */
#line 2595 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 9231 "bison-chpl-lib.cpp"
    break;

  case 427: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2596 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9237 "bison-chpl-lib.cpp"
    break;

  case 428: /* formal_ls_inner: formal  */
#line 2600 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9243 "bison-chpl-lib.cpp"
    break;

  case 429: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2601 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9249 "bison-chpl-lib.cpp"
    break;

  case 430: /* formal_ls: %empty  */
#line 2605 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 9255 "bison-chpl-lib.cpp"
    break;

  case 431: /* formal_ls: formal_ls_inner  */
#line 2606 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9261 "bison-chpl-lib.cpp"
    break;

  case 435: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2620 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9269 "bison-chpl-lib.cpp"
    break;

  case 436: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2625 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 9277 "bison-chpl-lib.cpp"
    break;

  case 437: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2630 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9285 "bison-chpl-lib.cpp"
    break;

  case 438: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2635 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 9293 "bison-chpl-lib.cpp"
    break;

  case 439: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2640 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9301 "bison-chpl-lib.cpp"
    break;

  case 440: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2645 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 9309 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_formal_intent_tag: %empty  */
#line 2651 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 9318 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_formal_intent_tag: required_intent_tag  */
#line 2656 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 9327 "bison-chpl-lib.cpp"
    break;

  case 443: /* required_intent_tag: TIN  */
#line 2663 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 9333 "bison-chpl-lib.cpp"
    break;

  case 444: /* required_intent_tag: TINOUT  */
#line 2664 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 9339 "bison-chpl-lib.cpp"
    break;

  case 445: /* required_intent_tag: TOUT  */
#line 2665 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 9345 "bison-chpl-lib.cpp"
    break;

  case 446: /* required_intent_tag: TCONST TIN  */
#line 2666 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 9351 "bison-chpl-lib.cpp"
    break;

  case 447: /* required_intent_tag: TCONST TREF  */
#line 2667 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9357 "bison-chpl-lib.cpp"
    break;

  case 448: /* required_intent_tag: TCONST  */
#line 2668 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9363 "bison-chpl-lib.cpp"
    break;

  case 449: /* required_intent_tag: TPARAM  */
#line 2669 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9369 "bison-chpl-lib.cpp"
    break;

  case 450: /* required_intent_tag: TREF  */
#line 2670 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9375 "bison-chpl-lib.cpp"
    break;

  case 451: /* required_intent_tag: TTYPE  */
#line 2671 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9381 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_this_intent_tag: %empty  */
#line 2675 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9387 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_this_intent_tag: TPARAM  */
#line 2676 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9393 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_this_intent_tag: TREF  */
#line 2677 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9399 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_this_intent_tag: TCONST TREF  */
#line 2678 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9405 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_this_intent_tag: TCONST  */
#line 2679 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9411 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_this_intent_tag: TTYPE  */
#line 2680 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9417 "bison-chpl-lib.cpp"
    break;

  case 458: /* proc_iter_or_op: TPROC  */
#line 2684 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9423 "bison-chpl-lib.cpp"
    break;

  case 459: /* proc_iter_or_op: TITER  */
#line 2685 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9429 "bison-chpl-lib.cpp"
    break;

  case 460: /* proc_iter_or_op: TOPERATOR  */
#line 2686 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9435 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_ret_tag: %empty  */
#line 2690 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9441 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_ret_tag: TOUT  */
#line 2691 "chpl.ypp"
                    { (yyval.returnTag) = Function::OUT; }
#line 9447 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_ret_tag: TCONST  */
#line 2692 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 9453 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_ret_tag: TCONST TREF  */
#line 2693 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 9459 "bison-chpl-lib.cpp"
    break;

  case 465: /* opt_ret_tag: TREF  */
#line 2694 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 9465 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_ret_tag: TPARAM  */
#line 2695 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 9471 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_ret_tag: TTYPE  */
#line 2696 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 9477 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_throws_error: %empty  */
#line 2700 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9483 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_throws_error: TTHROWS  */
#line 2701 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9489 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_function_body_stmt: TSEMI  */
#line 2704 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9495 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_function_body_stmt: function_body_stmt  */
#line 2705 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9501 "bison-chpl-lib.cpp"
    break;

  case 472: /* function_body_stmt: block_stmt_body  */
#line 2709 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9507 "bison-chpl-lib.cpp"
    break;

  case 473: /* function_body_stmt: TDO toplevel_stmt  */
#line 2710 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 9513 "bison-chpl-lib.cpp"
    break;

  case 474: /* function_body_stmt: return_stmt  */
#line 2711 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt), &((yylsp[0]))); }
#line 9519 "bison-chpl-lib.cpp"
    break;

  case 475: /* query_expr: TQUERIEDIDENT  */
#line 2715 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9525 "bison-chpl-lib.cpp"
    break;

  case 476: /* opt_lifetime_where: %empty  */
#line 2720 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9531 "bison-chpl-lib.cpp"
    break;

  case 477: /* opt_lifetime_where: TWHERE expr  */
#line 2722 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9537 "bison-chpl-lib.cpp"
    break;

  case 478: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2724 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9543 "bison-chpl-lib.cpp"
    break;

  case 479: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2726 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9549 "bison-chpl-lib.cpp"
    break;

  case 480: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2728 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9555 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_components_expr: lifetime_expr  */
#line 2733 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9561 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2735 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9567 "bison-chpl-lib.cpp"
    break;

  case 483: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2740 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9573 "bison-chpl-lib.cpp"
    break;

  case 484: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2742 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9579 "bison-chpl-lib.cpp"
    break;

  case 485: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2744 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9585 "bison-chpl-lib.cpp"
    break;

  case 486: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2746 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9591 "bison-chpl-lib.cpp"
    break;

  case 487: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2748 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9597 "bison-chpl-lib.cpp"
    break;

  case 488: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2750 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9603 "bison-chpl-lib.cpp"
    break;

  case 489: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2752 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9609 "bison-chpl-lib.cpp"
    break;

  case 490: /* lifetime_ident: TIDENT  */
#line 2756 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9615 "bison-chpl-lib.cpp"
    break;

  case 491: /* lifetime_ident: TINIT  */
#line 2757 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9621 "bison-chpl-lib.cpp"
    break;

  case 492: /* lifetime_ident: TTHIS  */
#line 2758 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9627 "bison-chpl-lib.cpp"
    break;

  case 493: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2763 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9636 "bison-chpl-lib.cpp"
    break;

  case 494: /* type_alias_decl_stmt_start: TTYPE  */
#line 2772 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9644 "bison-chpl-lib.cpp"
    break;

  case 495: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2776 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9653 "bison-chpl-lib.cpp"
    break;

  case 496: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2784 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9661 "bison-chpl-lib.cpp"
    break;

  case 497: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2788 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9669 "bison-chpl-lib.cpp"
    break;

  case 498: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2795 "chpl.ypp"
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
#line 9691 "bison-chpl-lib.cpp"
    break;

  case 499: /* opt_init_type: %empty  */
#line 2815 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9697 "bison-chpl-lib.cpp"
    break;

  case 500: /* opt_init_type: TASSIGN expr  */
#line 2817 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9703 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_type: TPARAM  */
#line 2821 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9709 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_type: TCONST TREF  */
#line 2822 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9715 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_type: TREF  */
#line 2823 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9721 "bison-chpl-lib.cpp"
    break;

  case 504: /* var_decl_type: TCONST  */
#line 2824 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9727 "bison-chpl-lib.cpp"
    break;

  case 505: /* var_decl_type: TVAR  */
#line 2825 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9733 "bison-chpl-lib.cpp"
    break;

  case 506: /* $@11: %empty  */
#line 2830 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9742 "bison-chpl-lib.cpp"
    break;

  case 507: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2834 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9751 "bison-chpl-lib.cpp"
    break;

  case 508: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2839 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9760 "bison-chpl-lib.cpp"
    break;

  case 509: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2847 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9768 "bison-chpl-lib.cpp"
    break;

  case 510: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2851 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9776 "bison-chpl-lib.cpp"
    break;

  case 511: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2858 "chpl.ypp"
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
#line 9798 "bison-chpl-lib.cpp"
    break;

  case 512: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2876 "chpl.ypp"
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
#line 9818 "bison-chpl-lib.cpp"
    break;

  case 513: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2895 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9826 "bison-chpl-lib.cpp"
    break;

  case 514: /* tuple_var_decl_component: ident_def  */
#line 2899 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9834 "bison-chpl-lib.cpp"
    break;

  case 515: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2903 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9842 "bison-chpl-lib.cpp"
    break;

  case 516: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2910 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9848 "bison-chpl-lib.cpp"
    break;

  case 517: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2912 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9854 "bison-chpl-lib.cpp"
    break;

  case 518: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2914 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9860 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_init_expr: %empty  */
#line 2920 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9866 "bison-chpl-lib.cpp"
    break;

  case 520: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2921 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9872 "bison-chpl-lib.cpp"
    break;

  case 521: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2922 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9878 "bison-chpl-lib.cpp"
    break;

  case 522: /* formal_or_ret_type_expr: expr  */
#line 2926 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9884 "bison-chpl-lib.cpp"
    break;

  case 523: /* ret_type: formal_or_ret_type_expr  */
#line 2930 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9890 "bison-chpl-lib.cpp"
    break;

  case 524: /* ret_type: reserved_type_ident_use  */
#line 2931 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9896 "bison-chpl-lib.cpp"
    break;

  case 525: /* ret_type: error  */
#line 2932 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9902 "bison-chpl-lib.cpp"
    break;

  case 526: /* colon_ret_type: TCOLON ret_type  */
#line 2936 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9908 "bison-chpl-lib.cpp"
    break;

  case 527: /* colon_ret_type: error  */
#line 2937 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9914 "bison-chpl-lib.cpp"
    break;

  case 528: /* opt_ret_type: %empty  */
#line 2941 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9920 "bison-chpl-lib.cpp"
    break;

  case 530: /* opt_type: %empty  */
#line 2946 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9926 "bison-chpl-lib.cpp"
    break;

  case 531: /* opt_type: TCOLON expr  */
#line 2947 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9932 "bison-chpl-lib.cpp"
    break;

  case 532: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2948 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9938 "bison-chpl-lib.cpp"
    break;

  case 533: /* opt_type: error  */
#line 2949 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9944 "bison-chpl-lib.cpp"
    break;

  case 534: /* formal_type: formal_or_ret_type_expr  */
#line 2953 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9950 "bison-chpl-lib.cpp"
    break;

  case 535: /* formal_type: reserved_type_ident_use  */
#line 2954 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9956 "bison-chpl-lib.cpp"
    break;

  case 536: /* colon_formal_type: TCOLON formal_type  */
#line 2958 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9962 "bison-chpl-lib.cpp"
    break;

  case 537: /* opt_colon_formal_type: %empty  */
#line 2962 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9968 "bison-chpl-lib.cpp"
    break;

  case 538: /* opt_colon_formal_type: colon_formal_type  */
#line 2963 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9974 "bison-chpl-lib.cpp"
    break;

  case 539: /* expr_ls: expr  */
#line 2969 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9980 "bison-chpl-lib.cpp"
    break;

  case 540: /* expr_ls: expr_ls TCOMMA expr  */
#line 2970 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9986 "bison-chpl-lib.cpp"
    break;

  case 541: /* tuple_component: TUNDERSCORE  */
#line 2974 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9992 "bison-chpl-lib.cpp"
    break;

  case 542: /* tuple_component: opt_try_expr  */
#line 2975 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9998 "bison-chpl-lib.cpp"
    break;

  case 543: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2980 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 10006 "bison-chpl-lib.cpp"
    break;

  case 544: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2984 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10014 "bison-chpl-lib.cpp"
    break;

  case 545: /* opt_actual_ls: %empty  */
#line 2990 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 10020 "bison-chpl-lib.cpp"
    break;

  case 546: /* opt_actual_ls: actual_ls  */
#line 2991 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 10026 "bison-chpl-lib.cpp"
    break;

  case 547: /* actual_ls: actual_expr  */
#line 2996 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10035 "bison-chpl-lib.cpp"
    break;

  case 548: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3001 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10045 "bison-chpl-lib.cpp"
    break;

  case 549: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3009 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10051 "bison-chpl-lib.cpp"
    break;

  case 550: /* actual_expr: opt_try_expr  */
#line 3010 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10057 "bison-chpl-lib.cpp"
    break;

  case 551: /* ident_expr: ident_use  */
#line 3014 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10063 "bison-chpl-lib.cpp"
    break;

  case 552: /* ident_expr: scalar_type  */
#line 3015 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10069 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TSINGLE expr  */
#line 3028 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10075 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3030 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10081 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3032 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10087 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3034 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10093 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 3036 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 10103 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TATOMIC expr  */
#line 3042 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10109 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TSYNC expr  */
#line 3044 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10115 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TOWNED  */
#line 3047 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10121 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TOWNED expr  */
#line 3049 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10127 "bison-chpl-lib.cpp"
    break;

  case 567: /* sub_type_level_expr: TUNMANAGED  */
#line 3051 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10133 "bison-chpl-lib.cpp"
    break;

  case 568: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3053 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10139 "bison-chpl-lib.cpp"
    break;

  case 569: /* sub_type_level_expr: TSHARED  */
#line 3055 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10145 "bison-chpl-lib.cpp"
    break;

  case 570: /* sub_type_level_expr: TSHARED expr  */
#line 3057 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10151 "bison-chpl-lib.cpp"
    break;

  case 571: /* sub_type_level_expr: TBORROWED  */
#line 3059 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10157 "bison-chpl-lib.cpp"
    break;

  case 572: /* sub_type_level_expr: TBORROWED expr  */
#line 3061 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10163 "bison-chpl-lib.cpp"
    break;

  case 573: /* sub_type_level_expr: TCLASS  */
#line 3063 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10169 "bison-chpl-lib.cpp"
    break;

  case 574: /* sub_type_level_expr: TRECORD  */
#line 3065 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10175 "bison-chpl-lib.cpp"
    break;

  case 575: /* for_expr: no_loop_attributes for_expr_base  */
#line 3069 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 10181 "bison-chpl-lib.cpp"
    break;

  case 576: /* for_expr_base: TFOR expr TIN expr TDO expr  */
#line 3074 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10195 "bison-chpl-lib.cpp"
    break;

  case 577: /* for_expr_base: TFOR expr TIN zippered_iterator TDO expr  */
#line 3084 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10209 "bison-chpl-lib.cpp"
    break;

  case 578: /* for_expr_base: TFOR expr TDO expr  */
#line 3094 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10222 "bison-chpl-lib.cpp"
    break;

  case 579: /* for_expr_base: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 3103 "chpl.ypp"
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
#line 10241 "bison-chpl-lib.cpp"
    break;

  case 580: /* for_expr_base: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3118 "chpl.ypp"
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
#line 10260 "bison-chpl-lib.cpp"
    break;

  case 581: /* for_expr_base: TFOR expr TDO TIF expr TTHEN expr  */
#line 3133 "chpl.ypp"
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
#line 10279 "bison-chpl-lib.cpp"
    break;

  case 582: /* for_expr_base: TFORALL expr TIN expr TDO expr  */
#line 3148 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10293 "bison-chpl-lib.cpp"
    break;

  case 583: /* for_expr_base: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3158 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10307 "bison-chpl-lib.cpp"
    break;

  case 584: /* for_expr_base: TFORALL expr TDO expr  */
#line 3168 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10320 "bison-chpl-lib.cpp"
    break;

  case 585: /* for_expr_base: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3177 "chpl.ypp"
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
#line 10339 "bison-chpl-lib.cpp"
    break;

  case 586: /* for_expr_base: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3192 "chpl.ypp"
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
#line 10358 "bison-chpl-lib.cpp"
    break;

  case 587: /* for_expr_base: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3207 "chpl.ypp"
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
#line 10377 "bison-chpl-lib.cpp"
    break;

  case 588: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3224 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 10383 "bison-chpl-lib.cpp"
    break;

  case 589: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3229 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 10391 "bison-chpl-lib.cpp"
    break;

  case 590: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3233 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 10399 "bison-chpl-lib.cpp"
    break;

  case 591: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3237 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10407 "bison-chpl-lib.cpp"
    break;

  case 592: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3241 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10415 "bison-chpl-lib.cpp"
    break;

  case 593: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3245 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10423 "bison-chpl-lib.cpp"
    break;

  case 594: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3249 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10431 "bison-chpl-lib.cpp"
    break;

  case 595: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3253 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10439 "bison-chpl-lib.cpp"
    break;

  case 596: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3260 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10453 "bison-chpl-lib.cpp"
    break;

  case 597: /* nil_expr: TNIL  */
#line 3277 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10459 "bison-chpl-lib.cpp"
    break;

  case 605: /* opt_task_intent_ls: %empty  */
#line 3295 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10465 "bison-chpl-lib.cpp"
    break;

  case 606: /* opt_task_intent_ls: task_intent_clause  */
#line 3296 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10471 "bison-chpl-lib.cpp"
    break;

  case 607: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3301 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10481 "bison-chpl-lib.cpp"
    break;

  case 608: /* task_intent_ls: intent_expr  */
#line 3309 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10487 "bison-chpl-lib.cpp"
    break;

  case 609: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3310 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10493 "bison-chpl-lib.cpp"
    break;

  case 610: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3315 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10503 "bison-chpl-lib.cpp"
    break;

  case 611: /* forall_intent_ls: intent_expr  */
#line 3323 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10509 "bison-chpl-lib.cpp"
    break;

  case 612: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3324 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10515 "bison-chpl-lib.cpp"
    break;

  case 613: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3329 "chpl.ypp"
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
#line 10533 "bison-chpl-lib.cpp"
    break;

  case 614: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3343 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10541 "bison-chpl-lib.cpp"
    break;

  case 615: /* intent_expr: expr TREDUCE ident_expr  */
#line 3347 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10549 "bison-chpl-lib.cpp"
    break;

  case 616: /* task_var_prefix: TCONST  */
#line 3353 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10555 "bison-chpl-lib.cpp"
    break;

  case 617: /* task_var_prefix: TIN  */
#line 3354 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10561 "bison-chpl-lib.cpp"
    break;

  case 618: /* task_var_prefix: TCONST TIN  */
#line 3355 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10567 "bison-chpl-lib.cpp"
    break;

  case 619: /* task_var_prefix: TREF  */
#line 3356 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10573 "bison-chpl-lib.cpp"
    break;

  case 620: /* task_var_prefix: TCONST TREF  */
#line 3357 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10579 "bison-chpl-lib.cpp"
    break;

  case 621: /* task_var_prefix: TVAR  */
#line 3358 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10585 "bison-chpl-lib.cpp"
    break;

  case 622: /* new_maybe_decorated: TNEW  */
#line 3363 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10591 "bison-chpl-lib.cpp"
    break;

  case 623: /* new_maybe_decorated: TNEW TOWNED  */
#line 3365 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10597 "bison-chpl-lib.cpp"
    break;

  case 624: /* new_maybe_decorated: TNEW TSHARED  */
#line 3367 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10603 "bison-chpl-lib.cpp"
    break;

  case 625: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3369 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10609 "bison-chpl-lib.cpp"
    break;

  case 626: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3371 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10615 "bison-chpl-lib.cpp"
    break;

  case 627: /* new_expr: new_maybe_decorated expr  */
#line 3377 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10623 "bison-chpl-lib.cpp"
    break;

  case 628: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3384 "chpl.ypp"
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
#line 10639 "bison-chpl-lib.cpp"
    break;

  case 629: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3396 "chpl.ypp"
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
#line 10655 "bison-chpl-lib.cpp"
    break;

  case 630: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3408 "chpl.ypp"
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
#line 10672 "bison-chpl-lib.cpp"
    break;

  case 631: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3421 "chpl.ypp"
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
#line 10690 "bison-chpl-lib.cpp"
    break;

  case 632: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3438 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10698 "bison-chpl-lib.cpp"
    break;

  case 633: /* range_literal_expr: expr TDOTDOT expr  */
#line 3445 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10707 "bison-chpl-lib.cpp"
    break;

  case 634: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3450 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10716 "bison-chpl-lib.cpp"
    break;

  case 635: /* range_literal_expr: expr TDOTDOT  */
#line 3455 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10725 "bison-chpl-lib.cpp"
    break;

  case 636: /* range_literal_expr: TDOTDOT expr  */
#line 3460 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10735 "bison-chpl-lib.cpp"
    break;

  case 637: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3466 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10745 "bison-chpl-lib.cpp"
    break;

  case 638: /* range_literal_expr: TDOTDOT  */
#line 3472 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10755 "bison-chpl-lib.cpp"
    break;

  case 639: /* cast_expr: expr TCOLON expr  */
#line 3502 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10763 "bison-chpl-lib.cpp"
    break;

  case 640: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3509 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10771 "bison-chpl-lib.cpp"
    break;

  case 641: /* super_expr: fn_expr  */
#line 3515 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10777 "bison-chpl-lib.cpp"
    break;

  case 645: /* expr: sub_type_level_expr TQUESTION  */
#line 3524 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10783 "bison-chpl-lib.cpp"
    break;

  case 646: /* expr: TQUESTION  */
#line 3526 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10789 "bison-chpl-lib.cpp"
    break;

  case 650: /* expr: fn_type  */
#line 3531 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10795 "bison-chpl-lib.cpp"
    break;

  case 660: /* opt_expr: %empty  */
#line 3545 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10801 "bison-chpl-lib.cpp"
    break;

  case 661: /* opt_expr: expr  */
#line 3546 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10807 "bison-chpl-lib.cpp"
    break;

  case 662: /* opt_try_expr: TTRY expr  */
#line 3550 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10813 "bison-chpl-lib.cpp"
    break;

  case 663: /* opt_try_expr: TTRYBANG expr  */
#line 3551 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10819 "bison-chpl-lib.cpp"
    break;

  case 664: /* opt_try_expr: super_expr  */
#line 3552 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10825 "bison-chpl-lib.cpp"
    break;

  case 670: /* call_base_expr: expr TBANG  */
#line 3569 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10833 "bison-chpl-lib.cpp"
    break;

  case 671: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3572 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10839 "bison-chpl-lib.cpp"
    break;

  case 674: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3579 "chpl.ypp"
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
#line 10855 "bison-chpl-lib.cpp"
    break;

  case 675: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3591 "chpl.ypp"
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
#line 10871 "bison-chpl-lib.cpp"
    break;

  case 676: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3603 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10879 "bison-chpl-lib.cpp"
    break;

  case 677: /* dot_expr: expr TDOT ident_use  */
#line 3610 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10885 "bison-chpl-lib.cpp"
    break;

  case 678: /* dot_expr: expr TDOT TTYPE  */
#line 3612 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10891 "bison-chpl-lib.cpp"
    break;

  case 679: /* dot_expr: expr TDOT TDOMAIN  */
#line 3614 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10897 "bison-chpl-lib.cpp"
    break;

  case 680: /* dot_expr: expr TDOT TLOCALE  */
#line 3616 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10903 "bison-chpl-lib.cpp"
    break;

  case 681: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3618 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10913 "bison-chpl-lib.cpp"
    break;

  case 682: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3624 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10923 "bison-chpl-lib.cpp"
    break;

  case 683: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3636 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10929 "bison-chpl-lib.cpp"
    break;

  case 684: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3638 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10937 "bison-chpl-lib.cpp"
    break;

  case 685: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3642 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10945 "bison-chpl-lib.cpp"
    break;

  case 686: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3646 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10953 "bison-chpl-lib.cpp"
    break;

  case 687: /* bool_literal: TFALSE  */
#line 3652 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10959 "bison-chpl-lib.cpp"
    break;

  case 688: /* bool_literal: TTRUE  */
#line 3653 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10965 "bison-chpl-lib.cpp"
    break;

  case 689: /* str_bytes_literal: STRINGLITERAL  */
#line 3657 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10971 "bison-chpl-lib.cpp"
    break;

  case 690: /* str_bytes_literal: BYTESLITERAL  */
#line 3658 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10977 "bison-chpl-lib.cpp"
    break;

  case 693: /* literal_expr: INTLITERAL  */
#line 3664 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10983 "bison-chpl-lib.cpp"
    break;

  case 694: /* literal_expr: REALLITERAL  */
#line 3665 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10989 "bison-chpl-lib.cpp"
    break;

  case 695: /* literal_expr: IMAGLITERAL  */
#line 3666 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10995 "bison-chpl-lib.cpp"
    break;

  case 696: /* literal_expr: CSTRINGLITERAL  */
#line 3667 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 11001 "bison-chpl-lib.cpp"
    break;

  case 697: /* literal_expr: TNONE  */
#line 3668 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 11007 "bison-chpl-lib.cpp"
    break;

  case 698: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3670 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11016 "bison-chpl-lib.cpp"
    break;

  case 699: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3675 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11025 "bison-chpl-lib.cpp"
    break;

  case 700: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3680 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 11034 "bison-chpl-lib.cpp"
    break;

  case 701: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3685 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 11043 "bison-chpl-lib.cpp"
    break;

  case 702: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3690 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 11052 "bison-chpl-lib.cpp"
    break;

  case 703: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3695 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 11061 "bison-chpl-lib.cpp"
    break;

  case 704: /* assoc_expr_ls: expr TALIAS expr  */
#line 3704 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 11070 "bison-chpl-lib.cpp"
    break;

  case 705: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3709 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 11080 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TPLUS expr  */
#line 3717 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11086 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TMINUS expr  */
#line 3718 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11092 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TSTAR expr  */
#line 3719 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11098 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TDIVIDE expr  */
#line 3720 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11104 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3721 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11110 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3722 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11116 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TMOD expr  */
#line 3723 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11122 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TEQUAL expr  */
#line 3724 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11128 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3725 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11134 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3726 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11140 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3727 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11146 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TLESS expr  */
#line 3728 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11152 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr TGREATER expr  */
#line 3729 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11158 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr TBAND expr  */
#line 3730 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11164 "bison-chpl-lib.cpp"
    break;

  case 720: /* binary_op_expr: expr TBOR expr  */
#line 3731 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11170 "bison-chpl-lib.cpp"
    break;

  case 721: /* binary_op_expr: expr TBXOR expr  */
#line 3732 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11176 "bison-chpl-lib.cpp"
    break;

  case 722: /* binary_op_expr: expr TAND expr  */
#line 3733 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11182 "bison-chpl-lib.cpp"
    break;

  case 723: /* binary_op_expr: expr TOR expr  */
#line 3734 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11188 "bison-chpl-lib.cpp"
    break;

  case 724: /* binary_op_expr: expr TEXP expr  */
#line 3735 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11194 "bison-chpl-lib.cpp"
    break;

  case 725: /* binary_op_expr: expr TBY expr  */
#line 3736 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11200 "bison-chpl-lib.cpp"
    break;

  case 726: /* binary_op_expr: expr TALIGN expr  */
#line 3737 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11206 "bison-chpl-lib.cpp"
    break;

  case 727: /* binary_op_expr: expr THASH expr  */
#line 3738 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11212 "bison-chpl-lib.cpp"
    break;

  case 728: /* binary_op_expr: expr TDMAPPED expr  */
#line 3739 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11218 "bison-chpl-lib.cpp"
    break;

  case 729: /* unary_op_expr: TPLUS expr  */
#line 3743 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11224 "bison-chpl-lib.cpp"
    break;

  case 730: /* unary_op_expr: TMINUS expr  */
#line 3744 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11230 "bison-chpl-lib.cpp"
    break;

  case 731: /* unary_op_expr: TMINUSMINUS expr  */
#line 3745 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11236 "bison-chpl-lib.cpp"
    break;

  case 732: /* unary_op_expr: TPLUSPLUS expr  */
#line 3746 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11242 "bison-chpl-lib.cpp"
    break;

  case 733: /* unary_op_expr: TBANG expr  */
#line 3747 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11248 "bison-chpl-lib.cpp"
    break;

  case 734: /* unary_op_expr: expr TBANG  */
#line 3748 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 11256 "bison-chpl-lib.cpp"
    break;

  case 735: /* unary_op_expr: TBNOT expr  */
#line 3751 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11262 "bison-chpl-lib.cpp"
    break;

  case 736: /* reduce_expr: expr TREDUCE expr  */
#line 3756 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11270 "bison-chpl-lib.cpp"
    break;

  case 737: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3760 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11278 "bison-chpl-lib.cpp"
    break;

  case 738: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3764 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11286 "bison-chpl-lib.cpp"
    break;

  case 739: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3768 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11294 "bison-chpl-lib.cpp"
    break;

  case 740: /* scan_expr: expr TSCAN expr  */
#line 3775 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11302 "bison-chpl-lib.cpp"
    break;

  case 741: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3779 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11310 "bison-chpl-lib.cpp"
    break;

  case 742: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3783 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11318 "bison-chpl-lib.cpp"
    break;

  case 743: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3787 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11326 "bison-chpl-lib.cpp"
    break;


#line 11330 "bison-chpl-lib.cpp"

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
