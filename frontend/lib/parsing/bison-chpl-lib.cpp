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
#define YYLAST   20706

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  182
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  204
/* YYNRULES -- Number of rules.  */
#define YYNRULES  751
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1328

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
     689,   690,   703,   704,   705,   713,   722,   731,   735,   742,
     750,   751,   752,   753,   754,   755,   756,   757,   771,   786,
     794,   806,   817,   829,   841,   852,   868,   869,   873,   883,
     884,   888,   892,   893,   897,   901,   908,   920,   928,   931,
     941,   943,   950,   955,   964,   968,   988,  1003,  1018,  1034,
    1035,  1037,  1042,  1043,  1048,  1066,  1070,  1074,  1085,  1104,
    1105,  1109,  1113,  1117,  1122,  1126,  1130,  1140,  1145,  1151,
    1158,  1163,  1170,  1181,  1182,  1186,  1191,  1199,  1208,  1218,
    1226,  1235,  1248,  1256,  1257,  1258,  1259,  1260,  1261,  1262,
    1263,  1264,  1265,  1266,  1267,  1268,  1269,  1270,  1271,  1272,
    1284,  1288,  1293,  1298,  1304,  1313,  1314,  1318,  1327,  1331,
    1335,  1339,  1343,  1347,  1356,  1357,  1358,  1362,  1363,  1364,
    1365,  1366,  1370,  1371,  1372,  1384,  1385,  1386,  1391,  1392,
    1393,  1394,  1395,  1396,  1397,  1398,  1399,  1400,  1401,  1402,
    1403,  1404,  1405,  1406,  1407,  1408,  1409,  1413,  1414,  1415,
    1416,  1417,  1418,  1419,  1420,  1421,  1422,  1423,  1424,  1431,
    1432,  1433,  1434,  1438,  1439,  1443,  1450,  1460,  1464,  1470,
    1470,  1476,  1476,  1485,  1486,  1487,  1488,  1489,  1490,  1491,
    1495,  1500,  1505,  1510,  1517,  1525,  1526,  1530,  1539,  1545,
    1553,  1572,  1571,  1581,  1580,  1593,  1599,  1602,  1603,  1607,
    1626,  1640,  1654,  1658,  1662,  1666,  1670,  1674,  1678,  1682,
    1702,  1706,  1710,  1714,  1718,  1722,  1726,  1730,  1734,  1738,
    1742,  1746,  1750,  1754,  1758,  1762,  1766,  1770,  1774,  1778,
    1782,  1786,  1790,  1795,  1803,  1812,  1816,  1820,  1824,  1828,
    1832,  1836,  1840,  1844,  1849,  1854,  1859,  1867,  1882,  1900,
    1904,  1911,  1912,  1917,  1922,  1923,  1924,  1925,  1926,  1927,
    1928,  1929,  1930,  1931,  1932,  1933,  1934,  1943,  1944,  1945,
    1946,  1955,  1956,  1960,  1964,  1968,  1975,  1979,  1983,  1990,
    1994,  1998,  2002,  2009,  2010,  2014,  2018,  2022,  2029,  2042,
    2058,  2066,  2070,  2079,  2080,  2084,  2088,  2093,  2102,  2107,
    2111,  2118,  2119,  2123,  2132,  2137,  2148,  2155,  2156,  2157,
    2161,  2162,  2166,  2170,  2174,  2178,  2182,  2189,  2206,  2219,
    2226,  2231,  2238,  2237,  2247,  2253,  2252,  2267,  2274,  2286,
    2304,  2301,  2329,  2333,  2334,  2336,  2341,  2342,  2346,  2347,
    2351,  2354,  2356,  2361,  2362,  2373,  2393,  2392,  2408,  2407,
    2425,  2435,  2432,  2466,  2474,  2482,  2493,  2504,  2513,  2528,
    2529,  2533,  2534,  2535,  2544,  2545,  2546,  2547,  2548,  2549,
    2550,  2551,  2552,  2553,  2554,  2555,  2556,  2557,  2558,  2559,
    2560,  2561,  2562,  2563,  2564,  2565,  2566,  2567,  2568,  2572,
    2573,  2574,  2575,  2576,  2577,  2578,  2579,  2580,  2581,  2582,
    2583,  2588,  2589,  2593,  2594,  2598,  2599,  2603,  2604,  2608,
    2609,  2613,  2614,  2618,  2622,  2623,  2627,  2631,  2636,  2641,
    2646,  2651,  2659,  2663,  2671,  2672,  2673,  2674,  2675,  2676,
    2677,  2678,  2679,  2683,  2684,  2685,  2686,  2687,  2688,  2692,
    2693,  2694,  2698,  2699,  2700,  2701,  2702,  2703,  2704,  2708,
    2709,  2712,  2713,  2717,  2718,  2719,  2723,  2728,  2729,  2731,
    2733,  2735,  2740,  2742,  2747,  2749,  2751,  2753,  2755,  2757,
    2759,  2764,  2765,  2766,  2770,  2779,  2783,  2791,  2795,  2802,
    2823,  2824,  2829,  2830,  2831,  2832,  2833,  2838,  2837,  2846,
    2854,  2858,  2865,  2883,  2902,  2906,  2910,  2917,  2919,  2921,
    2928,  2929,  2930,  2934,  2938,  2939,  2940,  2944,  2945,  2949,
    2950,  2954,  2955,  2956,  2957,  2961,  2962,  2966,  2970,  2971,
    2977,  2978,  2982,  2983,  2987,  2991,  2998,  2999,  3003,  3008,
    3017,  3018,  3022,  3023,  3030,  3031,  3032,  3033,  3034,  3035,
    3037,  3039,  3041,  3043,  3049,  3051,  3054,  3056,  3058,  3060,
    3062,  3064,  3066,  3068,  3070,  3072,  3077,  3081,  3091,  3101,
    3110,  3125,  3140,  3155,  3165,  3175,  3184,  3199,  3214,  3232,
    3236,  3240,  3244,  3248,  3252,  3256,  3260,  3267,  3285,  3293,
    3294,  3295,  3296,  3297,  3298,  3299,  3303,  3304,  3308,  3317,
    3318,  3322,  3331,  3332,  3336,  3350,  3354,  3361,  3362,  3363,
    3364,  3365,  3366,  3370,  3372,  3374,  3376,  3378,  3384,  3391,
    3403,  3415,  3428,  3445,  3452,  3457,  3462,  3467,  3473,  3479,
    3509,  3516,  3523,  3524,  3528,  3530,  3531,  3533,  3535,  3536,
    3537,  3538,  3541,  3542,  3543,  3544,  3545,  3546,  3547,  3548,
    3549,  3553,  3554,  3558,  3559,  3560,  3564,  3565,  3566,  3567,
    3576,  3577,  3580,  3581,  3582,  3586,  3598,  3610,  3617,  3619,
    3621,  3623,  3625,  3631,  3644,  3645,  3649,  3653,  3660,  3661,
    3665,  3666,  3670,  3671,  3672,  3673,  3674,  3675,  3676,  3677,
    3682,  3687,  3692,  3697,  3702,  3711,  3716,  3725,  3726,  3727,
    3728,  3729,  3730,  3731,  3732,  3733,  3734,  3735,  3736,  3737,
    3738,  3739,  3740,  3741,  3742,  3743,  3744,  3745,  3746,  3747,
    3751,  3752,  3753,  3754,  3755,  3756,  3759,  3763,  3767,  3771,
    3775,  3782,  3786,  3790,  3794,  3802,  3803,  3804,  3805,  3806,
    3807,  3808
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

#define YYPACT_NINF (-1085)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-752)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1085,    76,  4692, -1085,   -60,    95, -1085, -1085, -1085, -1085,
   -1085, -1085, -1085, 13668,    53,   269,   183, 16056,   193, 20477,
      53,   208,   144,   194,   269,  6100, 13668,   134, 20533, -1085,
     303,   285, -1085, 11204,   336,   374,   184, -1085,   345,   331,
   20533, 20533, 20533, -1085,  2970, 12260,   438, 13668, 13668,   198,
   -1085,   452,   490, 13668, -1085, 16056, -1085, 13668,   397,   406,
     204,   232, 15400,   534, 20589, -1085, 13668,  9620, 13668, 12260,
   16056, 13668,   492,   546,   434,  6100,   554, 13668,   572,  8212,
    8212, -1085,   586, -1085, 16056, -1085,   589, 11380, -1085,   645,
   13668, -1085, 13668, -1085, -1085, 15883, 13668, -1085, 11556, -1085,
   -1085, -1085,  5044,  8740, 13668, -1085,  5748, -1085, -1085, -1085,
   -1085,  4128,   512, -1085,    36,   486, -1085,    35, -1085, -1085,
   -1085, -1085, -1085, -1085, -1085, -1085, -1085, -1085, -1085, -1085,
   -1085,  8388, -1085,  8916, -1085, -1085,    17, -1085, -1085, -1085,
     605, -1085, -1085, -1085, -1085, -1085, -1085, -1085, -1085,   526,
   20533, -1085, 20309,   497,   425, -1085,   301, -1085, -1085, -1085,
     229,   288, -1085, -1085, 20533,  2970, -1085,   506,   511, -1085,
   -1085, -1085,   513,   515, 13668,   516,   517, -1085, -1085, -1085,
   20026,  1774,   360,   518,   519, -1085, -1085,   383, -1085, -1085,
   -1085, -1085, -1085,   266, -1085, -1085, -1085, 13668, -1085, 20533,
   -1085, 13668, 13668,    92,   637,   447, -1085, -1085, -1085, -1085,
   20026,   450, -1085, -1085,   527,  6100, -1085, -1085, -1085,   530,
     299,   533, -1085,   211, -1085,   539, -1085,   214, 20026,  9796,
   -1085, -1085, -1085, -1085, -1085, -1085, -1085, -1085, -1085, -1085,
   -1085, -1085, -1085, -1085, -1085, -1085, -1085, -1085, -1085, -1085,
   -1085, -1085, -1085, -1085, -1085, -1085,   540, -1085, -1085, 20533,
   20533,   -29,  4290, -1085, -1085,   625,  9796,   550,   544,   456,
    6100,  1921,    54,     6, -1085,  6100, -1085, -1085, 17666, -1085,
      10, 18267,   934, -1085,   552,   553, -1085, 17666,   299,   934,
   -1085,  9796,  1945,  1945,    55,   -11, -1085,     4, -1085, -1085,
   -1085, -1085,  9972,  9972, -1085,   599, -1085,   549,   571, -1085,
   -1085,  2116,   580, -1085, -1085, 20026,   233, 13668, 13668, -1085,
     123, -1085, -1085, 20026,   563, 18313, -1085, 17666,   299, 20026,
     559,  9796, -1085, 20026, 18420, -1085, -1085, -1085, -1085, -1085,
     299,   564, -1085,   372,   372,   417,   934,   934,   279, -1085,
   -1085,  5220,   -37, -1085, 13668,   -85,   -80, -1085,   417,   726,
   -1085, -1085, 20533, -1085, -1085,  5044, -1085,    83, -1085,   625,
   -1085, -1085, -1085,   734,   569,  5396, 13668, -1085, 13668,   679,
   -1085, -1085, 17176,   220,   526, 20026,   477, 13668,  6100, 11732,
   12436, 12436, 11204, 11908, -1085, -1085, -1085, 20533, 13668,   577,
   -1085,   582, 20365,   630,   112, -1085,   259,   229, -1085, -1085,
   -1085, -1085,   666, -1085, -1085, -1085, -1085,    38,   668, -1085,
   -1085, -1085, 15712,   634,   260, -1085,   273,   468, -1085,   615,
   13668, 13668, 13668, 12436, 12436, 13668,   476, 13668, 13668, 13668,
   13668, 13668,   611, 15883, 13668, 13668, 13668, 13668, 13668, 13668,
   13668, 13668, 13668, 13668, 13668, 13668, 13668, 13668, 13668, 13668,
     692, -1085, -1085, -1085, -1085, -1085, 11380, 11380, -1085, -1085,
   -1085, -1085, 11380, -1085, -1085, 11380, 11380,  9796, 10148, 12436,
   12436, 18500,   593, 13668, 13668, 12612, 20533,  9092, -1085, -1085,
   -1085,  2970, -1085, 13668, -1085,   643,   596,   623, -1085, -1085,
   -1085,   646,   648,  6100,   741,  6100, -1085,   742, 13668,   711,
     604, -1085, 11380, 20533, -1085, -1085, -1085,  1921, -1085, -1085,
     -76, -1085, 13844,   653, 13668,  2970, -1085, -1085, 13668, -1085,
   20421, 13668, 13668, -1085,   606, -1085, -1085, -1085, -1085, 18080,
   -1085,   229, 15575, -1085,  1921, -1085,   635, 13844, -1085, 20026,
   20026, -1085,   533, -1085,    42, -1085, 11380,   609, -1085,   763,
     763, -1085, -1085, -1085, -1085, 12788, -1085, 18541,  9268, -1085,
    9444, -1085, -1085, 11732, 12436, 12436, -1085,   638, 10324, -1085,
     437, -1085,  5572, -1085,   324, 18587,   330, 17268, 20533,  8036,
    7860, -1085,   526,   614, -1085,   211, -1085, 17751,   667, 20533,
     616,    52, 17344,    34,  2637,    34,  3363,    52, 17666, 16174,
     -33,    24, 18694,   -73,    96,   644,  1492, -1085, -1085, 13668,
     230, -1085, -1085,   139, -1085, -1085,    60, -1085, -1085,  7684,
   -1085,   687, -1085,   621,   650, -1085,   654, -1085,   655,   656,
     658,   659,   660, -1085,   661,   662,   670,   672,   674,   675,
     676, -1085, -1085, -1085, -1085, -1085, -1085, -1085, -1085, -1085,
   -1085, -1085, -1085, -1085, -1085, -1085, -1085, -1085, -1085, -1085,
   -1085, -1085, -1085, 13668,   685,   680,   621, -1085,   621, -1085,
   13668, -1085, 20533, -1085, -1085, 20107, 20107,   454, -1085,   454,
   -1085,   454, 20147,  1130,   872,   545,   299,   372, -1085,   479,
   -1085, -1085, -1085, -1085, -1085,   417,  2466,   454,  1706,  1706,
   20107,  1706,  1706,   337,   372,  2466,  3697,   337,   934,   934,
     372,   417,   629,   632,   651,   652,   665,   673,   642,   620,
   -1085,   454, -1085,   454, 13668, 11380, 17827, 17912,    87,   663,
     210, -1085, -1085, -1085,   148, -1085,  1322, 20067,   338,   349,
   20026, 11380, -1085, 11380, 13668, 13668,   769,  6100,   787, 18740,
    6100, 17872, 20533, -1085,   168, -1085,   213, -1085,   217,  1921,
      54, -1085, 20026, 10500, -1085, 20026, -1085, -1085, -1085, 20533,
   18780, 18861, -1085,   635,   686, -1085,   226, 13844, -1085,   263,
   -1085, 13668, -1085,   684,   103,   677, -1085,  2241, -1085, -1085,
   -1085, -1085, -1085, -1085, -1085, 17429, 17084,   105, -1085, -1085,
     249, -1085, -1085, -1085, -1085, 13668, -1085,   113, 16277, 13668,
   -1085, 12964,  8036,  7860, -1085, 10676,   442, 10852,   522,   523,
   11028,  8564,   525,   354, -1085, -1085, -1085, 18901,   694,   695,
     669, -1085, 20533,  2970, 12436,    52, 13668,   805, 13668, -1085,
    6276, 12436, -1085,   689, -1085,    52,  6452, 12436, -1085,    52,
   -1085,    52, 12436, -1085,    52, -1085, -1085, 20026,  6100,   688,
   12436, 13140,  4868,   691, 13668, 13316, -1085, -1085, 11380,   693,
     764, 20026, 20533,   512,   671, -1085,   687, -1085, -1085, -1085,
   -1085, -1085,     3,   343, -1085, 18947, -1085, 16437, -1085, -1085,
   20026, -1085,   697,   700, -1085, -1085, -1085, -1085, -1085, -1085,
   -1085, -1085,   256, 14020, 12436, 14196, 12436, 12436, 16174, 11380,
   -1085, -1085,  9092, -1085, -1085, -1085, -1085, -1085, -1085, -1085,
   -1085, -1085, -1085, -1085, -1085,    50, 12436, 12436, -1085, -1085,
   -1085, 20026, 20026,  6100, -1085,  6100, 13668, -1085,  6100,   800,
     703,   707, 20533,   533, -1085, -1085,   653, -1085, -1085, -1085,
     705,   709,   263, 13668,   687,   635, -1085, 12084, -1085, -1085,
   20026, -1085,  6100, 13668, -1085, -1085, -1085, 20533, -1085,   736,
     533, 12436, 12436, 12436,  6100, 11380, -1085, -1085, -1085, -1085,
   -1085,   479, -1085, -1085, -1085, -1085, -1085, -1085, -1085, -1085,
   -1085, -1085, -1085, -1085, -1085, -1085,  8036, -1085, -1085, 19054,
     219, 18065,   365,   718,  8036, -1085, 13668, -1085, -1085,  2019,
     376,   377,    53, 19108, -1085, 19149, 13668,   289, 11204, 20026,
      63, 18105,  9092, -1085, 11204, 20026,    73, 17505, -1085, -1085,
      34, 17590, -1085, -1085,  6100,   -28, 18150, -1085, -1085, 20026,
    6100, 20026, -1085, 19207,   295, -1085, -1085, -1085, -1085,  3320,
    3664, -1085, -1085, 20365, -1085,     3,    43, 13668, -1085,   737,
     712, -1085,   621,   621, -1085, -1085, -1085, 13668,   855, 19302,
   13668,   858, 19378,   715, 19419,   302, -1085,   653,   311,   325,
   -1085, -1085, 20026,   859,  6100, -1085, -1085, -1085, -1085, -1085,
    9796,  9796, -1085, -1085, -1085, -1085,   263, 20026, -1085,    86,
      48,   720, 13668, -1085,    52, 17666,    34, 17590,   -13, 18226,
   -1085,   131, 13492,  8036,  7860,  8036,  7860, -1085, -1085, -1085,
   19464, 20026,  2350, -1085, -1085, -1085,    52,    52, -1085, 17666,
   -1085,  4450,  6628, -1085,  6804, -1085,   326, -1085, 16519,  6980,
   -1085,    52,  7156, -1085,    52, -1085,    52, -1085,    52, -1085,
    7332,   719,  7508,   722, -1085, 13668,   730,  3664, -1085, -1085,
   -1085, -1085, -1085, -1085,   136, -1085,   140, -1085,   312, 19540,
      80, -1085, -1085, -1085, 19581, 14372, 14548, 19626, 14724, 14900,
   15076, 15252, -1085, -1085,  6100, -1085,   725,   727, -1085, -1085,
    6100, -1085,   533, 20026,  6100,  6100, -1085, -1085, 20026,   409,
     738,   422,   740, 13668, -1085, -1085, -1085, -1085,  6100, 11204,
   20026, 11204, 20026,  9092, -1085,  6100, 11204, 20026, -1085, 11204,
   20026, -1085, -1085, -1085, 11204, -1085, 20026,  6100, 11204, -1085,
   20026,  6100, 20026, -1085, -1085,    37, -1085, 13668,    43,   136,
     136,   136,   136,   136,   136,    43,  5924, -1085, -1085, -1085,
   -1085, -1085, 13668, 13668, 13668, 13668, 13668, 13668, 13668, 13668,
   -1085,   735,   743,   667, -1085, -1085, -1085, -1085, -1085, 20026,
   18740, 16599, 16679, -1085, 18740, 16759, 16839, 16919, -1085, 16999,
   -1085, -1085, 20026, -1085, -1085, -1085, -1085, -1085, -1085, -1085,
     754, -1085, 19702, 19743, 19788, 19864, 19905, 19950, -1085, -1085,
    6100,  6100,  6100,  6100,  6100,  6100, 13668, 13668, 13668, 13668,
   13668, 13668, 18740, 18740, 18740, 18740, 18740, 18740
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   145,   476,   694,   695,   696,
     690,   691,   697,   216,   606,   134,   167,   572,   174,   574,
     606,   173,   507,   505,   134,     0,   216,   291,   168,   206,
     205,   688,   204,   216,   172,    69,   292,   354,   169,   146,
       0,     0,     0,   349,     0,   216,   176,   216,   216,   623,
     598,   698,   177,   216,   355,   566,   502,   216,     0,     0,
     191,   189,   442,   171,   575,   504,   216,   216,   216,   216,
     570,   216,     0,   175,     0,     0,   147,   216,   689,   216,
     216,   495,   170,   329,   568,   506,   178,   216,   747,     0,
     216,   749,   216,   750,   751,   639,   216,   748,   216,   647,
     187,   746,     0,   216,   216,     4,     0,     5,    10,    11,
      12,    48,    51,    52,    58,     0,    46,    72,    13,    78,
      14,    15,    16,    17,    30,   552,   553,    23,    47,   188,
     198,   216,   207,   661,   199,    18,     0,    32,    31,    20,
       0,   286,    19,   657,    22,    36,    33,    34,   197,   330,
       0,   195,     0,     0,   654,   353,     0,   651,   193,   370,
     462,   453,   649,   196,     0,     0,   194,   666,   645,   644,
     648,   556,   554,     0,   216,   655,   656,   660,   659,   658,
       0,   555,     0,   667,   668,   669,   692,   693,   650,   558,
     557,   652,   653,     0,    29,   574,   168,   216,   146,     0,
     575,   216,   216,     0,     0,   654,   666,   554,   655,   656,
     564,   555,   667,   668,     0,     0,   607,   135,   136,     0,
     573,     0,   496,     0,   503,     0,    21,     0,   540,   216,
     142,   148,   160,   154,   153,   162,   139,   152,   163,   149,
     143,   164,   137,   165,   158,   151,   159,   157,   155,   156,
     138,   140,   150,   161,   166,   144,     0,   141,   215,     0,
       0,     0,     0,    71,    70,    72,   216,     0,     0,     0,
       0,     0,     0,     0,   510,     0,   184,    41,     0,   321,
       0,   320,   732,   627,   624,   625,   626,     0,   567,   733,
       7,   216,   352,   352,   449,   181,   444,   182,   445,   446,
     450,   451,   180,   179,   452,   442,   536,     0,   357,   358,
     360,     0,   443,   535,   362,   523,     0,   216,   216,   185,
     651,   642,   665,   643,     0,     0,    44,     0,   571,   559,
       0,   216,    45,   565,     0,   299,    48,   303,   300,   303,
     569,     0,    57,   734,   736,   637,   731,   730,     0,    75,
      79,     0,     0,   542,   216,     0,     0,   543,   638,     0,
       6,   327,     0,   205,   328,     0,    49,     0,     9,    72,
      50,    53,    56,     0,    55,     0,   216,    73,   216,     0,
     507,   203,     0,   690,   330,   662,   211,   216,     0,   216,
     216,   216,   216,   216,   218,   576,   589,     0,   216,     0,
     326,     0,     0,   347,     0,   340,   442,   462,   460,   461,
     459,   378,   464,   463,   467,   466,   468,     0,   457,   454,
     455,   458,     0,   500,     0,   497,     0,   646,    35,   628,
     216,   216,   216,   216,   216,   216,   735,   216,   216,   216,
     216,   216,     0,   636,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     409,   416,   417,   418,   413,   415,   216,   216,   411,   414,
     412,   410,   216,   420,   419,   216,   216,   216,   216,   216,
     216,     0,     0,   216,   216,   216,     0,   216,    37,    25,
      38,     0,    26,   216,    39,   552,     0,   547,   548,   551,
     339,     0,     0,     0,   260,     0,   409,   256,   216,     0,
       0,    24,   216,     0,   270,    27,   514,     0,   515,   517,
       0,   534,   216,   520,   216,     0,   183,    40,   216,   323,
       0,   216,   216,    42,     0,   192,   190,   447,   448,     0,
     443,   462,   442,   435,     0,   434,   538,   216,   127,   663,
     664,   368,     0,   186,     0,    43,   216,     0,   310,   301,
     302,    28,    77,    76,    80,   216,   699,     0,   216,   684,
     216,   686,     8,   216,   216,   216,   217,    58,   216,    54,
       0,    66,     0,   125,     0,   120,     0,    87,     0,   216,
     216,   200,   330,     0,   213,     0,   210,   606,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   590,
       0,     0,   540,     0,   297,   331,     0,   338,   344,   216,
     342,   337,   427,     0,   429,   433,     0,   465,   528,     0,
     530,   469,   456,   425,   142,   405,   148,   403,   154,   153,
     152,   149,   143,   407,   164,   165,   151,   155,   138,   150,
     166,   402,   384,   387,   385,   386,   408,   397,   388,   401,
     393,   391,   404,   392,   390,   395,   400,   389,   394,   398,
     399,   396,   406,   216,   381,     0,   425,   382,   425,   379,
     216,   499,     0,   494,   509,   727,   726,   729,   738,   737,
     742,   741,   723,   720,   721,   722,   640,   710,   145,     0,
     680,   681,   147,   679,   678,   634,   714,   725,   719,   717,
     728,   718,   716,   708,   713,   715,   724,   707,   711,   712,
     709,   635,     0,     0,     0,     0,     0,     0,     0,     0,
     740,   739,   744,   743,   216,   216,     0,     0,     0,   297,
     617,   618,   620,   622,     0,   609,     0,     0,     0,     0,
     541,   216,   561,   216,   216,   216,   259,     0,   255,     0,
       0,     0,     0,   560,     0,   273,     0,   271,     0,   518,
       0,   533,   532,   216,   512,   633,   511,   322,   319,     0,
       0,     0,   677,   538,   363,   359,     0,   216,   539,   520,
     361,   216,   367,     0,     0,     0,   562,     0,   304,   700,
     641,   685,   544,   687,   545,     0,     0,     0,    59,    60,
       0,    62,    64,    68,    67,   216,   102,     0,     0,   216,
      95,   216,   216,   216,    65,   216,   384,   216,   385,   386,
     216,   216,   396,     0,   421,   422,    82,    81,    94,     0,
       0,   332,     0,     0,   216,     0,   216,     0,   216,   228,
       0,   216,   227,     0,   236,     0,     0,   216,   234,     0,
     244,     0,   216,   242,     0,   221,   220,   591,     0,     0,
     216,   216,     0,     0,   216,   216,   703,   294,   216,     0,
     352,   348,     0,     0,   442,   428,   469,   526,   525,   524,
     527,   470,   477,   442,   377,     0,   383,     0,   373,   374,
     501,   498,     0,     0,   133,   131,   132,   130,   129,   128,
     675,   676,     0,   216,   216,   216,   216,   216,   701,   216,
     619,   621,   216,   608,   167,   174,   173,   172,   169,   176,
     177,   171,   175,   170,   178,     0,   216,   216,   508,   550,
     549,   268,   267,     0,   262,     0,   216,   258,     0,   264,
       0,   296,     0,     0,   516,   519,   520,   521,   522,   318,
       0,     0,   520,   216,   469,   538,   537,   216,   438,   436,
     369,   312,     0,   216,   311,   314,   563,     0,   305,   308,
       0,   216,   216,   216,     0,   216,    61,   126,   123,   103,
     115,   109,   108,   107,   117,   104,   118,   113,   106,   114,
     112,   110,   111,   105,   116,   119,   216,   122,   121,    90,
      89,    88,     0,     0,   216,   201,   216,   202,   332,   352,
       0,     0,   606,   606,   224,     0,   216,     0,   216,   579,
       0,     0,   216,   237,   216,   585,     0,     0,   235,   245,
       0,     0,   243,   252,     0,     0,     0,   702,   250,   592,
       0,   705,   704,     0,     0,   325,   191,   189,   324,   352,
     352,   333,   343,     0,   430,   477,     0,   216,   371,   432,
       0,   380,   425,   425,   682,   683,   296,   216,     0,     0,
     216,     0,     0,     0,     0,     0,   610,   520,   666,   666,
     261,   257,   597,   263,     0,    74,   293,   272,   269,   513,
     216,   216,   439,   437,   364,   365,   520,   424,   316,     0,
       0,     0,   216,   306,     0,     0,     0,     0,     0,     0,
      63,     0,   216,   216,   216,   216,   216,    96,    99,    85,
      84,    83,   352,   208,   214,   212,     0,     0,   219,     0,
     254,     0,     0,   226,     0,   225,     0,   612,     0,     0,
     232,     0,     0,   230,     0,   240,     0,   238,     0,   253,
       0,     0,     0,     0,   251,   216,   298,   352,   335,   334,
     346,   350,   491,   492,     0,   493,   479,   482,     0,   478,
       0,   426,   375,   376,     0,   216,   216,     0,   216,   216,
     216,   216,   298,   614,     0,   266,     0,     0,   441,   440,
       0,   315,     0,   309,     0,     0,   124,    92,    91,     0,
       0,     0,     0,   216,   209,   223,   222,   229,     0,   216,
     578,   216,   577,   216,   611,     0,   216,   584,   233,   216,
     583,   231,   241,   239,   216,   248,   594,     0,   216,   246,
     593,     0,   706,   295,   336,     0,   490,   216,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   471,   473,   475,
     372,   472,   216,   216,   216,   216,   216,   216,   216,   216,
     265,   629,   630,   317,   307,    98,   101,    97,   100,    86,
     582,     0,     0,   613,   588,     0,     0,     0,   249,     0,
     247,   351,   481,   483,   484,   487,   488,   489,   485,   486,
     480,   474,     0,     0,     0,     0,     0,     0,   631,   632,
       0,     0,     0,     0,     0,     0,   216,   216,   216,   216,
     216,   216,   581,   580,   587,   586,   596,   595
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1085, -1085, -1085,     0,   -61,  3114, -1085,   297,   796, -1085,
   -1085,   557,  -147,  -109, -1085,   333, -1085, -1085,   -74, -1085,
   -1085,    30,   649, -1085,  -542,  3370,   538,  -570, -1085,  -794,
   -1085, -1085, -1085,   100, -1085, -1085, -1085,   893, -1085,   147,
    -212, -1085, -1085,  -470,  3533, -1084,  -849, -1085, -1085,   363,
   -1085, -1085, -1085, -1085, -1085, -1085, -1085, -1085,     7, -1085,
     551,   980, -1085,   528, -1085, -1085,   -31,  1789, -1085, -1085,
   -1085, -1085,   583, -1085,   -58, -1085, -1085, -1085, -1085,   395,
   -1085, -1085, -1085,  -103, -1085,  -349,  -803, -1085, -1085, -1085,
   -1085, -1085,  -394, -1085,    89, -1085, -1085, -1085, -1085, -1085,
     382, -1085,    62, -1085, -1085, -1085, -1085,   541, -1085, -1085,
   -1085, -1085,    28,  -408,  -177,  -785,  -922,  -656, -1085,    39,
   -1085,    44,   390,   -50,   626,   -49, -1085, -1085,  -359,  -824,
   -1085,  -312, -1085,  -128,  -317,  -306,  -840, -1085, -1085,   106,
     265, -1085,  -199,   818, -1085,  -154,   428,   187,  -361,  -763,
    -608, -1085, -1085, -1085,  -599,  -513, -1085,  -747,   -10,   -23,
   -1085,  -241,  -494,  -541,    -2, -1085, -1085, -1085, -1085, -1085,
   -1085,   246, -1085,   -14, -1085, -1085,  -378, -1085,  -894, -1085,
   -1085,   493,   622, -1085, -1085, -1085, -1085,  2803, -1085,   -20,
    1253, -1085,  1382,  1511, -1085, -1085, -1085, -1085, -1085, -1085,
   -1085, -1085, -1085,  -480
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   350,   106,   758,   108,   109,   110,   367,
     368,   111,   112,   113,   114,   374,   579,   810,   811,   115,
     116,   117,   379,   118,   119,   120,   351,   838,   586,   839,
     121,   122,  1007,   583,   584,   123,   124,   219,   255,   272,
     125,   257,   126,   306,   852,   127,   128,   293,   292,   129,
     130,   131,   132,   133,   134,   595,   842,   135,   203,   137,
     394,   601,   138,   261,   139,   766,   767,   204,   141,   142,
     143,   144,   559,   798,   980,   145,   146,   794,   975,   279,
     280,   147,   148,   149,   150,   399,   880,   151,   152,   404,
     882,   883,   405,   153,   205,  1245,   155,   156,   307,   308,
     309,   964,   157,   321,   552,   791,   158,   159,  1180,   160,
     161,   675,   676,   834,   835,   836,   968,   894,   407,   623,
    1070,   624,   546,   625,   311,   540,   422,   411,   417,   892,
    1260,  1261,   162,  1068,  1176,  1177,  1178,   163,   164,   424,
     425,   681,   165,   166,   223,   273,   274,   519,   520,   774,
     313,   890,   630,   631,   523,   314,   788,   789,   352,   355,
     356,   496,   497,   498,   206,   168,   169,   395,   170,   396,
     171,   207,   173,   215,   216,   744,   859,  1146,   745,   746,
     174,   208,   209,   177,   178,   179,   322,   180,   386,   499,
     211,   182,   212,   213,   185,   186,   187,   188,   613,   189,
     190,   191,   192,   193
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   305,   105,   371,   476,   402,   221,   748,   618,   136,
     792,   426,   310,   312,   677,   795,   227,   495,   764,   833,
     898,   889,   899,   167,   491,   510,   969,   886,  1086,  1013,
     384,  1061,   136,  1008,   790,   593,   962,   812,  1019,   628,
    1102,   387,   275,   793,   372,  -291,  1172,   324,   626,   388,
     534,   521,   771,   524,   495,   521,   316,   389,   390,   391,
    -292,   521,  1065,   568,  1066,   265,   275,   341,   570,  1256,
    -529,   870,   769,   167,   503,   875,     3,   167,   167,   495,
     275,   376,   136,   357,   275,   508,   136,   136,   853,   569,
     557,   154,  -531,   853,   571,  1142,  1259,  1173,   770,  -529,
     167,  -531,   537,   876,   167,  1149,   194,   387,   853,   136,
     387,   565,  1256,   136,   154,   388,   698,  -313,  1200,   495,
     377,  -531,  1067,   573,   574,   391,   389,   390,   391,   320,
      67,  -529,   483,   484,   917,   566,  1174,   392,  1147,  1172,
    1105,   369,   365,   868,  -529,   853,   538,  1175,  1160,   320,
     378,  -274,   983,  -531,   525,   853,   768,  -529,   528,   888,
    -313,  1259,   229,  1204,   154,   320,  -531,   198,   154,   154,
    1061,   956,   871,    67,   214,   256,  -531,   266,   972,  -531,
    -531,   365,   784,   786,  1198,   629,   373,   268,   269,   270,
    1173,   154,   393,  1099,   853,   154,   493,   522,  -531,  1103,
     872,   522,  -531,   392,  -529,   365,   392,   522,   365,  -529,
    1168,  1169,   940,   167,  -313,  1132,   283,   702,  1106,   365,
    -531,   973,   136,   365,  -531,   855,   853,   861,   864,  1129,
     704,  -531,   869,   873,   365,   871,   728,   729,    23,  -275,
    1175,   912,   361,   840,   365,   678,  1257,   551,   172,  -281,
     -71,   365,  1012,   493,   222,  1123,   412,   920,   575,  1247,
     620,   393,   877,   918,  -280,   495,   495,   485,   167,   878,
     -71,   172,   217,   167,   966,   974,   284,   136,   -70,  1014,
     592,   984,   136,  1061,   621,   224,   294,   884,  1248,   -71,
      56,   320,  1124,   371,  -366,   285,   922,   400,   -70,   403,
     495,   921,    65,  1206,   154,   413,   296,   229,   414,   364,
     298,   423,   286,   885,   258,   418,   753,   -70,  1244,   -71,
     415,   172,   923,   218,  1193,   172,   172,    85,   320,  1283,
    1210,   779,  1212,    83,  1246,   299,  1087,   749,   300,   416,
      58,  -289,   951,  1199,   495,   305,   482,   -70,   172,   167,
     301,   564,   172,   320,   408,  1104,   479,   266,   136,   154,
     480,   952,   493,   167,   154,   769,   495,   419,   432,   304,
     294,  -345,   136,   167,   769,   409,   335,   338,  -341,   420,
     494,   493,   136,   611,  1054,   410,   167,   953,   615,   773,
     296,   954,  -279,   320,   298,   136,   843,   985,   421,   548,
     965,  -276,  -341,   432,   753,   290,   501,   502,   682,  1294,
    1295,  1296,  1297,  1298,  1299,   967,  1182,  1183,   518,   299,
     679,   525,   300,   986,    58,  1085,   683,   433,   937,   873,
    1076,   434,   480,   622,   301,   267,  1121,   493,  1249,   684,
     154,   436,   748,   753,   812,   194,   723,   724,   432,   442,
     753,   562,   725,   304,   154,   726,   727,   263,   545,  -616,
     264,   172,   433,  1140,   154,  1250,   434,  1251,  1252,  1166,
    1253,  1254,   815,  -615,  1223,   738,  1192,   154,   819,   436,
     437,   305,   438,   439,   440,  -616,   441,   442,  1062,   677,
     816,   445,   310,   312,  -283,   175,   820,   525,   452,  -615,
    1224,   167,  1014,   167,   456,   457,   458,   433,  -287,   256,
     136,   434,   136,  1014,   436,   938,   172,  -431,   175,   440,
    1015,   172,   442,   495,   682,   525,   445,    37,   320,   320,
    -352,  1127,  -749,   477,   320,   478,  -749,   320,   320,   320,
     320,   495,  1134,  1135,   614,   802,  -284,   804,   357,   403,
     357,  -352,   748,  1209,    54,  1211,  -674,  1014,  -674,   436,
     437,  -352,   438,   439,   440,   807,   441,   442,   175,   674,
    1014,   445,   175,   175,   320,  1275,   432,   451,   452,   291,
     167,   455,   564,   845,   456,   457,   458,   594,  1277,   136,
    -278,  -603,   154,   330,   154,   175,   436,   172,  -673,   175,
    -673,   440,  -282,   194,   442,   988,   704,   331,   445,   813,
    -288,   172,  -750,  -751,   698,  -746,  -750,  -751,   320,  -746,
    -673,   172,  -673,  -670,   176,  -670,   294,   365,  -290,   513,
     320,   699,   320,   739,   172,   433,   336,   336,  1258,   434,
     320,  -672,  -277,  -672,   700,  -285,   296,   176,   342,  -671,
     298,  -671,   902,    89,   903,   535,   536,   375,  1151,  1154,
     765,   397,  1156,  1158,   518,   198,   495,  1161,  1163,  1170,
     406,   154,  -600,   398,   701,   299,   427,   778,   300,  -599,
     359,   428,  -604,  -605,  -602,  -601,   545,   436,   437,  1021,
     301,   518,   440,   486,   441,   442,   489,   176,   294,   445,
     487,   176,   176,  1258,   365,   492,   452,   495,   175,   304,
     377,   500,   456,   457,   458,   702,   511,   512,   296,   542,
    1073,   703,   298,   541,   176,   531,   532,   547,   176,   553,
     561,   939,   556,  1060,   572,   824,  1063,   577,  1151,  1154,
    1161,  1163,   578,   748,   935,   588,   847,   299,   616,   172,
     300,   172,    58,   958,   617,   167,   619,   627,   167,   632,
     680,   722,   301,   175,   136,   442,   735,   136,   175,   751,
     752,   753,   754,   495,   755,   757,   760,   762,   763,   773,
     782,   304,   787,   796,   797,   841,   361,   846,   373,   848,
      22,    23,   493,   891,   893,   904,   911,   320,   905,   362,
    -145,    29,   363,   943,  -167,  -174,  -173,    32,  -172,  -169,
    -146,  -176,  -177,   320,    37,   320,   910,   906,   907,  1059,
    -171,   945,  -175,   305,  -147,  -170,  -178,   896,   172,   423,
     897,   908,   305,   963,  1094,   320,   919,   176,  1027,   909,
    1018,    54,  1014,    56,   175,    58,   154,  1056,   167,   154,
    1057,   976,  1026,   364,   167,    65,   971,   136,   175,  1196,
    1197,  1017,  1032,   136,  1044,  1055,   167,  1050,   175,  1095,
     167,  1074,  1060,  1096,    81,   136,  1075,    83,  1100,   136,
      85,   175,  1101,  1112,  1128,   884,  1181,  1185,   495,   495,
    1188,  1190,   176,  1194,  1202,  1237,  1243,   176,  1241,  1271,
    1308,  1272,  1248,   432,  1276,    89,  1278,   366,  1309,   950,
     808,  1120,  1167,   582,   509,   987,   518,   225,   576,  1111,
     607,  1097,   560,   777,   785,  1072,   959,   596,  1064,   783,
     100,   539,  1069,  1291,  1088,  1089,  1058,  1171,  1300,   154,
     320,   167,  1293,   167,   979,   154,   167,   901,  1020,   381,
     136,   371,   136,   776,   371,   136,   955,   154,  1059,     0,
       0,   154,   433,  1110,   508,   432,   434,     0,     0,     0,
     167,   508,     0,   176,     0,     0,     0,     0,     0,  1109,
       0,   320,   167,     0,     0,  1060,     0,   176,     0,   423,
       0,   136,     0,     0,     0,     0,   175,   176,   175,     0,
       0,     0,     0,   172,     0,     0,   172,     0,  1136,  1137,
     176,     0,     0,     0,   436,   437,     0,     0,   439,   440,
       0,   441,   442,     0,   433,     0,   445,     0,   434,   403,
       0,     0,   154,   452,   154,     0,     0,   154,     0,   456,
     457,   458,   167,     0,   674,     0,     0,   320,   167,     0,
       0,   136,     0,     0,     0,     0,     0,   136,   371,     0,
       0,   154,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1059,     0,   154,     0,   175,   436,     0,     0,     0,
       0,   440,     0,   441,   442,     0,     0,     0,   445,     0,
       0,     0,   167,     0,     0,   452,   172,     0,     0,   765,
       0,   136,   172,   458,   508,   508,     0,     0,   508,   508,
     508,     0,   508,     0,   172,     0,     0,     0,   172,     0,
       0,     0,     0,     0,   979,   176,     0,   176,     0,     0,
       0,     0,     0,   154,     0,     0,     0,     0,     0,   154,
     167,     0,   167,     0,     0,     0,     0,   167,     0,   136,
     167,   136,     0,     0,     0,     0,   136,     0,   167,   136,
     167,   432,   320,   320,     0,     0,     0,   136,     0,   136,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   154,     0,     0,     0,     0,     0,   172,
       0,   172,   167,     0,   172,     0,     0,     0,   167,     0,
       0,   136,   167,   167,   176,     0,     0,   136,     0,     0,
     403,   136,   136,     0,     0,     0,   167,     0,   172,     0,
     433,     0,     0,   167,   434,   136,     0,     0,     0,     0,
     172,   154,   136,   154,     0,   167,     0,     0,   154,   167,
       0,   154,     0,     0,   136,     0,     0,     0,   136,   154,
     175,   154,     0,   175,   167,   181,  1301,     0,     0,     0,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,   440,   181,   441,
     442,     0,     0,   154,   445,     0,     0,     0,     0,   154,
     172,   452,     0,   154,   154,     0,   172,   456,   457,   458,
       0,     0,     0,     0,     0,     0,     0,   154,   167,   167,
     167,   167,   167,   167,   154,     0,     0,   136,   136,   136,
     136,   136,   136,     0,     0,   698,   154,     0,   181,     0,
     154,     0,   181,   181,     0,     0,     0,     0,     0,   924,
     172,     0,   925,   175,     0,   154,     0,   926,     0,   175,
       0,     0,     0,     0,     0,   181,     0,   196,     0,   181,
       0,   175,     0,     0,     0,   175,     0,   927,     0,     0,
     603,   605,     0,   610,   928,     0,   198,     0,     0,   176,
       0,     0,   176,     0,   183,   929,     0,     0,   172,     0,
     172,     0,     0,   930,     0,   172,     0,     0,   172,   154,
     154,   154,   154,   154,   154,     0,   172,   183,   172,     0,
     931,     0,     0,   688,   690,     0,     0,     0,     0,     0,
       0,     0,   932,     0,     0,     0,   702,     0,     0,     0,
       0,     0,     0,   933,     0,     0,   175,     0,   175,   934,
     172,   175,     0,     0,     0,     0,   172,     0,     0,     0,
     172,   172,     0,     0,     0,     0,     0,   183,     0,   730,
     732,   183,   183,     0,   172,   175,     0,     0,   181,     0,
       0,   172,   176,     0,     0,     0,     0,   175,   176,     0,
       0,     0,     0,   172,   183,     0,     0,   172,   183,     0,
     176,     0,     0,   879,   176,     0,     0,     0,     0,     0,
       0,     0,   172,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   184,  -332,     0,     0,     0,  -332,  -332,
       0,     0,     0,   181,     0,     0,     0,  -332,   181,  -332,
    -332,     0,     0,     0,     0,  -332,   184,   175,     0,     0,
       0,     0,  -332,   175,     0,  -332,     0,     0,     0,     0,
       0,     0,     0,     0,   603,   610,   172,   172,   172,   172,
     172,   172,     0,     0,     0,   176,  -332,   176,     0,  -332,
     176,  -332,     0,  -332,     0,  -332,  -332,     0,  -332,     0,
       0,  -332,     0,  -332,     0,     0,   184,   175,     0,     0,
     184,   184,     0,     0,   176,     0,     0,   183,     0,     0,
       0,     0,  -332,     0,   181,  -332,   176,     0,  -332,     0,
       0,     0,     0,   184,     0,     0,     0,   184,   181,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   181,     0,
       0,     0,     0,  -332,     0,   175,     0,   175,     0,     0,
       0,   181,   175,     0,     0,   175,     0,     0,     0,     0,
       0,     0,   183,   175,     0,   175,     0,   183,  -332,     0,
       0,     0,     0,     0,  -332,     0,   176,     0,     0,     0,
       0,     0,   176,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   175,     0,     0,
       0,     0,     0,   175,     0,     0,     0,   175,   175,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   175,     0,     0,     0,     0,   176,     0,   175,     0,
       0,     0,     0,     0,     0,     0,   184,     0,     0,     0,
     175,     0,     0,   183,   175,     0,     0,   432,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   183,     0,   175,
       0,     0,     0,     0,     0,     0,   181,   183,   181,     0,
       0,     0,     0,     0,   176,     0,   176,     0,     0,     0,
     183,   176,     0,     0,   176,     0,     0,     0,     0,     0,
       0,   184,   176,     0,   176,     0,   184,     0,     0,     0,
       0,   140,     0,     0,     0,     0,   433,     0,     0,     0,
     434,     0,     0,   175,   175,   175,   175,   175,   175,     0,
       0,     0,     0,     0,   140,     0,   176,     0,     0,     0,
       0,     0,   176,     0,  1022,     0,   176,   176,     0,     0,
       0,  1030,     0,     0,     0,   181,     0,  1036,     0,     0,
     176,     0,  1040,     0,     0,     0,     0,   176,   436,   437,
    1045,   438,   439,   440,     0,   441,   442,   443,     0,   176,
     445,     0,   184,   176,   140,     0,   451,   452,     0,     0,
     455,     0,     0,   456,   457,   458,   184,     0,   176,     0,
       0,     0,     0,     0,   459,   183,   184,   183,     0,     0,
       0,   140,     0,     0,  1078,   140,  1081,  1083,     0,   184,
     460,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     470,   471,   472,   473,   474,     0,   688,   730,     0,     0,
       0,     0,     0,     0,   230,     0,     0,     0,     0,     0,
       0,     0,   176,   176,   176,   176,   176,   176,   231,   232,
       0,   233,     0,     0,   475,     0,   234,  -670,     0,  -670,
       0,     0,     0,     0,   235,     0,     0,     0,     0,     0,
     236,  1114,  1116,  1118,   183,     0,   237,   361,     0,     0,
     238,    22,    23,   239,     0,   240,     0,     0,     0,     0,
     362,     0,    29,   363,   241,     0,     0,     0,    32,     0,
       0,   242,   243,     0,     0,    37,     0,     0,     0,   244,
       0,     0,     0,     0,   140,     0,     0,     0,     0,   245,
     181,     0,     0,   181,   184,     0,   184,     0,   246,   247,
       0,   248,    54,   249,    56,   250,     0,     0,   251,     0,
       0,     0,   252,   516,   364,   253,    65,     0,   254,     0,
       0,   361,     0,     0,     0,    22,    23,     0,     0,     0,
       0,     0,     0,     0,   362,    81,    29,   363,    83,   140,
       0,    85,    32,     0,   140,     0,     0,     0,     0,    37,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   184,   517,     0,    54,     0,    56,     0,
      58,     0,  1056,   181,     0,  1057,     0,     0,   364,   181,
      65,     0,     0,     0,     0,     0,     0,     0,     0,   230,
       0,   181,     0,     0,     0,   181,     0,     0,     0,    81,
       0,     0,    83,   231,   232,    85,   233,     0,     0,   183,
     140,   234,   183,     0,     0,     0,     0,     0,     0,   235,
       0,     0,     0,     0,   140,   236,     0,     0,     0,     0,
      89,   237,     0,     0,   140,   238,     0,     0,   239,     0,
     240,     0,     0,     0,     0,     0,     0,   140,     0,   241,
       0,     0,     0,     0,     0,   100,   242,   243,     0,     0,
       0,  1133,     0,     0,   244,     0,   181,     0,   181,     0,
       0,   181,     0,     0,   245,     0,     0,     0,     0,     0,
       0,     0,     0,   246,   247,     0,   248,     0,   249,     0,
     250,     0,     0,   251,     0,   181,     0,   252,   543,     0,
     253,     0,   183,   254,     0,     0,     0,   181,   183,     0,
       0,     0,     0,     0,   230,     0,     0,     0,     0,     0,
     183,     0,     0,     0,   183,     0,     0,     0,   231,   232,
       0,   233,     0,     0,     0,     0,   234,     0,   184,     0,
       0,   184,     0,     0,   235,     0,     0,     0,     0,     0,
     236,     0,     0,     0,     0,     0,   237,     0,     0,   544,
     238,     0,   140,   239,   140,   240,     0,   181,     0,     0,
       0,     0,     0,   181,   241,     0,     0,     0,     0,     0,
       0,   242,   243,     0,     0,     0,     0,     0,     0,   244,
       0,     0,     0,     0,     0,   183,     0,   183,     0,   245,
     183,     0,     0,     0,     0,     0,     0,     0,   246,   247,
       0,   248,     0,   249,     0,   250,     0,   181,   251,     0,
       0,     0,   252,     0,   183,   253,     0,     0,   254,     0,
       0,   184,     0,     0,     0,     0,   183,   184,     0,     0,
       0,   140,   361,     0,     0,     0,    22,    23,     0,   184,
       0,     0,     0,   184,     0,   362,     0,    29,   363,     0,
       0,     0,     0,    32,     0,   181,     0,   181,     0,     0,
      37,     0,   181,     0,     0,   181,     0,     0,     0,     0,
       0,     0,   365,   181,   977,   181,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   183,    54,     0,    56,
       0,    58,   183,  1056,     0,     0,  1057,     0,     0,   364,
       0,    65,     0,     0,     0,     0,     0,   181,     0,     0,
       0,     0,     0,   181,   184,     0,   184,   181,   181,   184,
      81,     0,     0,    83,     0,     0,    85,     0,     0,     0,
       0,   181,     0,     0,     0,     0,   183,     0,   181,     0,
       0,     0,     0,   184,     0,     0,     0,     0,     0,     0,
     181,    89,     0,     0,   181,   184,     0,   432,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   181,
       0,     0,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,  1214,     0,   183,     0,   183,     0,     0,     0,
       0,   183,     0,     0,   183,     0,     0,     0,     0,     0,
       0,     0,   183,     0,   183,     0,   140,     0,     0,   140,
       0,     0,     0,     0,     0,   184,   433,     0,     0,     0,
     434,   184,     0,   181,   181,   181,   181,   181,   181,     0,
       0,     0,     0,     0,     0,     0,   183,     0,     0,     0,
       0,     0,   183,     0,     0,     0,   183,   183,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     183,     0,     0,     0,     0,   184,     0,   183,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,     0,   183,
     445,   446,   447,   183,   449,   450,   451,   452,     0,     0,
     455,     0,     0,   456,   457,   458,     0,     0,   183,   140,
       0,     0,     0,     0,   459,   140,     0,     0,     0,   430,
       0,     0,     0,   184,     0,   184,   431,   140,     0,     0,
     184,   140,     0,   184,     0,     0,     0,     0,   432,   856,
       0,   184,     0,   184,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   857,     0,     0,     0,     0,     0,
       0,     0,   183,   183,   183,   183,   183,   183,     0,     0,
       0,     0,     0,     0,     0,   184,     0,     0,     0,     0,
       0,   184,     0,     0,     0,   184,   184,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   433,     0,   184,
       0,   434,   140,     0,   140,     0,   184,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   184,     0,
       0,     0,   184,     0,     0,     0,     0,     0,   853,     0,
       0,   140,   435,     0,     0,     0,     0,   184,     0,     0,
       0,     0,     0,   140,     0,     0,     0,     0,     0,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,     0,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,     0,     0,   456,   457,   458,     0,   365,     0,
       0,     0,     0,     0,     0,   459,   210,     0,     0,     0,
     220,   184,   184,   184,   184,   184,   184,     0,     0,   228,
       0,     0,     0,   140,     0,     0,   262,     0,     0,   140,
       0,     0,     0,     0,     0,     0,     0,     0,   278,     0,
     281,   282,     0,     0,     0,     0,   287,     0,   288,     0,
     289,     0,     0,     0,     0,   315,     0,     0,     0,   228,
     323,   325,   327,   328,   329,     0,     0,     0,   333,     0,
     334,     0,     0,   140,     0,     0,     0,   340,     0,     0,
     323,     0,     0,   343,     0,   344,     0,     0,   345,   346,
       0,   347,     0,     0,     0,   228,   323,   358,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   140,     0,   140,   382,     0,   385,     0,   140,     0,
       0,   140,     0,     0,     0,     0,     0,     0,     0,   140,
       0,   140,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   230,     0,     0,     0,   429,     0,     0,
       0,     0,     0,   140,     0,     0,     0,   231,   232,   140,
     233,     0,     0,   140,   140,   234,     0,     0,     0,     0,
     481,     0,     0,   235,   333,   228,     0,   140,     0,   236,
       0,     0,     0,     0,   140,   237,     0,     0,     0,   238,
       0,     0,   239,     0,   240,     0,   140,     0,     0,     0,
     140,     0,   323,   241,     0,     0,     0,     0,     0,     0,
     242,   243,     0,     0,     0,   140,     0,     0,   244,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   245,     0,
       0,     0,     0,     0,     0,     0,     0,   246,   247,   323,
     248,     0,   249,     0,   250,     0,     0,   251,     0,     0,
       0,   252,     0,     0,   253,     0,     0,   254,     0,     0,
       0,     0,     0,     0,   323,     0,     0,     0,     0,   140,
     140,   140,   140,   140,   140,   329,   333,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   107,     0,     0,     0,
     549,   550,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   323,     0,     0,     0,     0,   226,
       0,     0,     0,   271,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   567,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   585,
       0,   587,     0,     0,     0,     0,     0,     0,     0,   332,
     597,     0,   602,   604,   606,   608,   612,     0,     0,     0,
       0,   228,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   107,     0,     0,     0,
     360,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   685,   686,   687,   689,   691,   692,     0,
     693,   694,   695,   696,   697,     0,   705,   706,   707,   708,
     709,   710,   711,   712,   713,   714,   715,   716,   717,   718,
     719,   720,   721,     0,     0,     0,     0,     0,     0,   323,
     323,     0,     0,     0,     0,   323,     0,     0,   323,   323,
     323,   323,   731,   733,     0,     0,   736,   737,   612,     0,
     747,     0,     0,     0,     0,     0,   750,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   759,     0,
       0,   761,     0,     0,     0,   323,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   772,     0,   775,     0,   488,
       0,   281,     0,     0,   780,   781,     0,     0,     0,     0,
       0,     0,   361,     0,     0,   315,    22,    23,     0,     0,
     315,     0,     0,     0,     0,   362,     0,    29,   363,   323,
       0,     0,     0,    32,     0,     0,     0,     0,   750,     0,
      37,   323,     0,   323,     0,   430,   805,   806,   228,     0,
       0,   323,   431,     0,   515,     0,     0,     0,     0,   526,
       0,     0,   837,   837,   432,   275,     0,    54,     0,    56,
       0,   359,     0,  1056,     0,     0,  1057,     0,     0,   364,
     862,    65,   867,     0,     0,   276,     0,     0,     0,     0,
       0,     0,   881,     0,     0,     0,     0,     0,     0,     0,
      81,     0,   315,    83,     0,     0,    85,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   337,
     339,     0,     0,   433,     0,     0,     0,   434,     0,     0,
       0,    89,     0,     0,     0,   107,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   895,     0,     0,   107,
       0,   370,     0,   900,   853,     0,   100,     0,   435,   107,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   598,     0,     0,   436,   437,     0,   438,   439,
     440,     0,   441,   442,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,     0,     0,
     456,   457,   458,     0,   365,     0,     0,   759,   323,     0,
       0,   459,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   323,     0,   323,   941,   942,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,     0,   277,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,   490,     0,     0,   970,     0,     0,     0,     0,     0,
       0,     0,   326,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   756,   585,     0,
       0,     0,  1009,     0,  1011,   837,   837,     0,   343,     0,
     344,   504,   507,   346,   347,     0,     0,     0,     0,   514,
       0,     0,     0,     0,     0,     0,     0,  1023,   276,  1025,
     276,   228,     0,  1029,  1031,     0,     0,   276,     0,  1035,
    1037,     0,     0,     0,     0,  1041,     0,     0,     0,     0,
       0,     0,     0,  1046,   750,  1049,     0,  1051,  1053,     0,
       0,   323,     0,     0,     0,     0,   361,     0,     0,     0,
      22,    23,     0,     0,     0,     0,   107,   276,     0,   362,
       0,    29,   363,     0,     0,     0,     0,    32,     0,     0,
       0,     0,     0,     0,    37,     0,  1029,  1079,  1035,  1082,
    1084,  1049,   323,     0,     0,   747,     0,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   689,
     731,    54,     0,    56,     0,     0,     0,  1056,     0,  1092,
    1057,     0,     0,   364,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   315,     0,     0,     0,
    1107,     0,     0,     0,    81,     0,   228,    83,     0,     0,
      85,     0,     0,     0,  1115,  1117,  1119,   433,   323,     0,
       0,   434,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,   837,
       0,   527,     0,   529,     0,     0,     0,  1130,     0,  1131,
     533,     0,   435,     0,     0,     0,     0,     0,     0,  1139,
     100,  1141,     0,     0,     0,   747,     0,  1148,     0,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,     0,
     444,   445,   446,   447,     0,   449,   450,   451,   452,   453,
     555,   455,     0,     0,   456,   457,   458,     0,     0,     0,
    1179,   944,     0,     0,   947,   459,     0,     0,     0,     0,
    1184,     0,     0,  1187,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   323,   323,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1203,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1208,   837,   837,   837,   837,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1220,     0,  1222,     0,     0,
       0,     0,  1227,     0,     0,  1230,     0,     0,     0,     0,
       0,     0,     0,  1236,   526,  1240,     0,     0,  1242,     0,
     526,   276,   276,   276,   276,   276,   276,   276,   276,     0,
       0,     0,  1043,     0,     0,     0,  1048,     0,  1220,  1222,
       0,  1227,  1230,  1236,  1240,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1279,     0,     0,     0,
       0,  1280,  1281,     0,  1282,     0,   747,     0,  1284,  1285,
       0,     0,  1286,     0,     0,     0,     0,  1287,     0,     0,
       0,  1289,     0,     0,     0,     0,     0,     0,     0,     0,
    1292,     0,     0,     0,     0,     0,     0,  1090,     0,  1091,
       0,     0,  1093,     0,     0,  1280,  1302,  1303,  1284,  1304,
    1305,  1306,  1307,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1108,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1048,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1322,  1323,  1324,  1325,  1326,  1327,  1322,
    1323,  1324,  1325,  1326,  1327,     0,     0,     0,     0,     0,
       0,   949,     0,     0,   849,     0,   854,   858,   860,   863,
     865,   866,     0,     0,     0,     0,     0,     0,     0,     0,
     361,     0,     0,     0,    22,    23,     0,     0,  1159,     0,
       0,     0,     0,   362,  1164,    29,   363,   978,     0,     0,
       0,    32,     0,     0,     0,   276,   276,     0,    37,     0,
       0,  -352,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   -69,     0,     0,     0,     0,     0,
       0,     0,  -352,     0,     0,    54,     0,    56,  1195,     0,
       0,    60,  -352,   -69,    61,   276,     0,   364,     0,    65,
       0,     0,     0,     0,     0,   276,     0,     0,     0,   276,
       0,   276,     0,     0,   276,     0,     0,     0,    81,     0,
       0,    83,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   526,     0,   526,     0,
       0,     0,     0,   526,     0,     0,   526,     0,     0,     0,
       0,     0,     0,     0,  1235,     0,  1239,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   100,     0,     0,     0,     0,   365,
       0,     0,   430,     0,     0,     0,     0,     0,  1270,   431,
       0,     0,     0,     0,  1273,     0,     0,     0,  1235,  1239,
       0,   432,     0,  1098,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   858,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1113,  1288,     0,     0,     0,  1290,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     107,     0,     0,     0,     0,     0,     0,     0,  1024,     0,
     433,     0,     0,     0,   434,     0,     0,     0,  1033,     0,
       0,     0,  1038,   505,  1039,     0,     0,  1042,     0,     0,
     276,   276,     0,     0,     0,     0,   276,   276,     0,     0,
     276,   276,     0,     0,     0,   435,   506,   461,   462,   463,
     464,   465,     0,     0,   468,   469,   470,   471,     0,   473,
     474,     0,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,     0,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,     0,     0,   456,   457,   458,
       0,   365,   430,     0,     0,     0,     0,     0,   459,   431,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     276,   432,     0,     0,   276,   276,   276,   276,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   276,   276,     0,   276,
       0,   507,     0,     0,     0,     0,     0,     0,   507,     0,
       0,   276,     0,     0,   276,     0,   276,     0,   276,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,   434,     0,     0,     0,     0,     0,
       0,     0,     0,  1218,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1143,  1145,     0,     0,     0,     0,  1150,
    1153,     0,  1274,  1155,  1157,   435,   506,   461,   462,   463,
     464,   465,     0,     0,   468,   469,   470,   471,     0,   473,
     474,     0,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,     0,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,     0,     0,   456,   457,   458,
       0,   365,     0,     0,     0,     0,     0,     0,   459,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1201,     0,     0,     0,  1143,  1145,  1150,
    1153,   507,   507,     0,     0,   507,   507,   507,     0,   507,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1215,
    1216,     0,  1217,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1228,     0,     0,  1231,     0,  1232,
       0,  1233,    -2,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,  -216,    21,    22,    23,
      24,    25,    26,     0,  -216,    27,     0,    28,     0,    29,
      30,    31,  -216,  -216,  -216,    32,    33,    34,   -69,     0,
      35,    36,    37,     0,    38,  -352,    39,     0,    40,    41,
      42,    43,    44,     0,    45,    46,    47,    48,   -69,    49,
      50,     0,    51,    52,    53,     0,  -352,     0,     0,    54,
      55,    56,    57,    58,    59,    60,  -352,   -69,    61,    62,
      63,    64,     0,    65,    66,    67,     0,    68,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,     0,    83,    84,   -69,    85,    86,
       0,     0,  -216,     0,    87,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,     0,   103,     0,  -216,     0,     4,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
    -701,     0,    13,    14,    15,    16,    17,  -701,    18,     0,
      19,    20,  -216,    21,    22,    23,    24,    25,    26,  -701,
    -216,    27,  -701,    28,     0,    29,    30,    31,  -216,  -216,
    -216,    32,    33,    34,   -69,     0,    35,    36,    37,     0,
      38,  -352,    39,     0,    40,    41,    42,    43,    44,     0,
      45,    46,    47,    48,   -69,    49,    50,     0,    51,    52,
      53,     0,  -352,     0,     0,    54,    55,    56,    57,     0,
      59,    60,  -352,   -69,    61,    62,    63,    64,  -701,    65,
      66,    67,  -701,    68,    69,    70,    71,    72,    73,    74,
      75,     0,    76,    77,     0,    78,    79,    80,    81,    82,
       0,    83,    84,   -69,    85,    86,     0,     0,  -216,     0,
      87,     0,     0,  -701,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,  -701,    92,  -701,  -701,  -701,  -701,  -701,  -701,  -701,
       0,  -701,  -701,  -701,  -701,  -701,  -701,  -701,  -701,  -701,
    -701,  -701,  -701,    99,   100,  -701,  -701,  -701,     0,   102,
    -701,   103,     0,  -216,     0,   348,  -701,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -216,    21,
      22,    23,    24,    25,    26,     0,  -216,    27,     0,    28,
       0,    29,    30,    31,  -216,  -216,  -216,    32,    33,    34,
     -69,     0,    35,    36,    37,     0,    38,  -352,    39,     0,
      40,    41,    42,    43,    44,     0,    45,    46,    47,    48,
     -69,    49,    50,     0,    51,    52,    53,     0,  -352,     0,
       0,    54,    55,    56,    57,    58,    59,    60,  -352,   -69,
      61,    62,    63,    64,     0,    65,    66,    67,     0,    68,
      69,    70,    71,    72,    73,    74,    75,     0,    76,    77,
       0,    78,    79,    80,    81,    82,     0,    83,    84,   -69,
      85,    86,     0,     0,  -216,     0,    87,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     100,     0,     0,   101,     0,   102,   349,   103,     0,  -216,
       0,     4,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -216,    21,    22,    23,    24,    25,
      26,     0,  -216,    27,     0,    28,     0,    29,    30,    31,
    -216,  -216,  -216,    32,    33,    34,   -69,     0,    35,    36,
      37,     0,    38,  -352,    39,     0,    40,    41,    42,    43,
      44,     0,    45,    46,    47,    48,   -69,    49,    50,     0,
      51,    52,    53,     0,  -352,     0,     0,    54,    55,    56,
      57,    58,    59,    60,  -352,   -69,    61,    62,    63,    64,
       0,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,     0,    83,    84,   -69,    85,    86,     0,     0,
    -216,     0,    87,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   100,     0,     0,   101,
       0,   102,   563,   103,     0,  -216,     0,   580,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -216,    21,    22,    23,    24,    25,    26,     0,  -216,    27,
       0,    28,     0,    29,    30,    31,  -216,  -216,  -216,    32,
      33,    34,   -69,     0,    35,    36,    37,     0,    38,  -352,
      39,     0,    40,    41,    42,    43,    44,     0,    45,    46,
      47,    48,   -69,    49,    50,     0,    51,    52,    53,     0,
    -352,     0,     0,    54,    55,    56,    57,    58,    59,    60,
    -352,   -69,    61,    62,    63,    64,     0,    65,    66,    67,
       0,    68,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,     0,    83,
      84,   -69,    85,    86,     0,     0,  -216,     0,    87,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,   581,   103,
       0,  -216,     0,     4,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,  -216,    21,    22,    23,
      24,    25,    26,     0,  -216,    27,     0,    28,     0,    29,
      30,    31,  -216,  -216,  -216,    32,    33,    34,   -69,     0,
      35,    36,    37,     0,    38,  -352,    39,     0,    40,    41,
      42,    43,    44,     0,    45,    46,    47,    48,   -69,    49,
      50,     0,    51,    52,    53,     0,  -352,     0,     0,    54,
      55,    56,    57,    58,    59,    60,  -352,   -69,    61,    62,
      63,    64,     0,    65,    66,    67,     0,    68,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,     0,    83,    84,   -69,    85,    86,
       0,     0,  -216,     0,    87,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,   814,   103,     0,  -216,     0,     4,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,  -216,    21,    22,    23,    24,    25,    26,     0,
    -216,    27,     0,    28,     0,    29,    30,    31,  -216,  -216,
    -216,    32,    33,    34,   -69,     0,    35,    36,    37,     0,
      38,  -352,    39,     0,    40,    41,    42,    43,    44,     0,
      45,    46,    47,    48,   -69,    49,    50,     0,    51,    52,
      53,     0,  -352,     0,     0,    54,    55,    56,    57,   359,
      59,    60,  -352,   -69,    61,    62,    63,    64,     0,    65,
      66,    67,     0,    68,    69,    70,    71,    72,    73,    74,
      75,     0,    76,    77,     0,    78,    79,    80,    81,    82,
       0,    83,    84,   -69,    85,    86,     0,     0,  -216,     0,
      87,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
       0,   103,     0,  -216,     0,     4,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -216,    21,
      22,    23,    24,    25,    26,     0,  -216,    27,     0,    28,
       0,    29,    30,    31,  -216,  -216,  -216,    32,    33,    34,
     -69,     0,    35,    36,    37,     0,    38,  -352,    39,     0,
      40,    41,    42,    43,    44,     0,    45,    46,    47,    48,
     -69,    49,    50,     0,    51,    52,    53,     0,  -352,     0,
       0,    54,    55,    56,    57,    58,    59,    60,  -352,   -69,
      61,    62,    63,    64,     0,    65,    66,    67,     0,    68,
      69,    70,    71,    72,    73,    74,    75,     0,    76,    77,
       0,    78,    79,    80,    81,    82,     0,    83,    84,   -69,
      85,    86,     0,     0,  -216,     0,    87,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     100,     0,     0,   101,     0,   102,     0,   103,     0,  -216,
       0,     4,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -216,    21,    22,    23,    24,    25,
      26,     0,  -216,    27,     0,    28,     0,    29,    30,    31,
    -216,  -216,  -216,    32,    33,    34,   -69,     0,    35,    36,
      37,     0,    38,  -352,    39,     0,    40,    41,    42,    43,
      44,     0,    45,    46,    47,    48,   -69,    49,    50,     0,
      51,    52,    53,     0,  -352,     0,     0,    54,    55,    56,
      57,     0,    59,    60,  -352,   -69,    61,    62,    63,    64,
       0,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,     0,    83,    84,   -69,    85,    86,     0,     0,
    -216,     0,    87,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   100,     0,     0,   101,
       0,   102,     0,   103,     0,  -216,     0,     4,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -216,    21,    22,    23,    24,    25,    26,     0,  -216,    27,
       0,    28,     0,    29,    30,    31,  -216,  -216,  -216,    32,
    1028,    34,   -69,     0,    35,    36,    37,     0,    38,  -352,
      39,     0,    40,    41,    42,    43,    44,     0,    45,    46,
      47,    48,   -69,    49,    50,     0,    51,    52,    53,     0,
    -352,     0,     0,    54,    55,    56,    57,     0,    59,    60,
    -352,   -69,    61,    62,    63,    64,     0,    65,    66,    67,
       0,    68,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,     0,    83,
      84,   -69,    85,    86,     0,     0,  -216,     0,    87,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,     0,   103,
       0,  -216,     0,     4,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,  -216,    21,    22,    23,
      24,    25,    26,     0,  -216,    27,     0,    28,     0,    29,
      30,    31,  -216,  -216,  -216,    32,  1034,    34,   -69,     0,
      35,    36,    37,     0,    38,  -352,    39,     0,    40,    41,
      42,    43,    44,     0,    45,    46,    47,    48,   -69,    49,
      50,     0,    51,    52,    53,     0,  -352,     0,     0,    54,
      55,    56,    57,     0,    59,    60,  -352,   -69,    61,    62,
      63,    64,     0,    65,    66,    67,     0,    68,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,     0,    83,    84,   -69,    85,    86,
       0,     0,  -216,     0,    87,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,     0,   103,     0,  -216,     0,     4,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,  -216,    21,    22,    23,    24,    25,    26,     0,
    -216,    27,     0,    28,     0,    29,    30,    31,  -216,  -216,
    -216,    32,  1219,    34,   -69,     0,    35,    36,    37,     0,
      38,  -352,    39,     0,    40,    41,    42,    43,    44,     0,
      45,    46,    47,    48,   -69,    49,    50,     0,    51,    52,
      53,     0,  -352,     0,     0,    54,    55,    56,    57,     0,
      59,    60,  -352,   -69,    61,    62,    63,    64,     0,    65,
      66,    67,     0,    68,    69,    70,    71,    72,    73,    74,
      75,     0,    76,    77,     0,    78,    79,    80,    81,    82,
       0,    83,    84,   -69,    85,    86,     0,     0,  -216,     0,
      87,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
       0,   103,     0,  -216,     0,     4,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -216,    21,
      22,    23,    24,    25,    26,     0,  -216,    27,     0,    28,
       0,    29,    30,    31,  -216,  -216,  -216,    32,  1221,    34,
     -69,     0,    35,    36,    37,     0,    38,  -352,    39,     0,
      40,    41,    42,    43,    44,     0,    45,    46,    47,    48,
     -69,    49,    50,     0,    51,    52,    53,     0,  -352,     0,
       0,    54,    55,    56,    57,     0,    59,    60,  -352,   -69,
      61,    62,    63,    64,     0,    65,    66,    67,     0,    68,
      69,    70,    71,    72,    73,    74,    75,     0,    76,    77,
       0,    78,    79,    80,    81,    82,     0,    83,    84,   -69,
      85,    86,     0,     0,  -216,     0,    87,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     100,     0,     0,   101,     0,   102,     0,   103,     0,  -216,
       0,     4,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -216,    21,    22,    23,    24,    25,
      26,     0,  -216,    27,     0,    28,     0,    29,    30,    31,
    -216,  -216,  -216,    32,  1226,    34,   -69,     0,    35,    36,
      37,     0,    38,  -352,    39,     0,    40,    41,    42,    43,
      44,     0,    45,    46,    47,    48,   -69,    49,    50,     0,
      51,    52,    53,     0,  -352,     0,     0,    54,    55,    56,
      57,     0,    59,    60,  -352,   -69,    61,    62,    63,    64,
       0,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,     0,    83,    84,   -69,    85,    86,     0,     0,
    -216,     0,    87,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   100,     0,     0,   101,
       0,   102,     0,   103,     0,  -216,     0,     4,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -216,    21,    22,    23,    24,    25,    26,     0,  -216,    27,
       0,    28,     0,    29,    30,    31,  -216,  -216,  -216,    32,
    1229,    34,   -69,     0,    35,    36,    37,     0,    38,  -352,
      39,     0,    40,    41,    42,    43,    44,     0,    45,    46,
      47,    48,   -69,    49,    50,     0,    51,    52,    53,     0,
    -352,     0,     0,    54,    55,    56,    57,     0,    59,    60,
    -352,   -69,    61,    62,    63,    64,     0,    65,    66,    67,
       0,    68,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,     0,    83,
      84,   -69,    85,    86,     0,     0,  -216,     0,    87,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,     0,   103,
       0,  -216,     0,     4,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,  -216,    21,    22,    23,
      24,    25,    26,     0,  -216,    27,     0,    28,     0,    29,
      30,    31,  -216,  -216,  -216,    32,  1234,    34,   -69,     0,
      35,    36,    37,     0,    38,  -352,    39,     0,    40,    41,
      42,    43,    44,     0,    45,    46,    47,    48,   -69,    49,
      50,     0,    51,    52,    53,     0,  -352,     0,     0,    54,
      55,    56,    57,     0,    59,    60,  -352,   -69,    61,    62,
      63,    64,     0,    65,    66,    67,     0,    68,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,     0,    83,    84,   -69,    85,    86,
       0,     0,  -216,     0,    87,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,     0,   103,     0,  -216,     0,     4,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,  -216,    21,    22,    23,    24,    25,    26,     0,
    -216,    27,     0,    28,     0,    29,    30,    31,  -216,  -216,
    -216,    32,  1238,    34,   -69,     0,    35,    36,    37,     0,
      38,  -352,    39,     0,    40,    41,    42,    43,    44,     0,
      45,    46,    47,    48,   -69,    49,    50,     0,    51,    52,
      53,     0,  -352,     0,     0,    54,    55,    56,    57,     0,
      59,    60,  -352,   -69,    61,    62,    63,    64,     0,    65,
      66,    67,     0,    68,    69,    70,    71,    72,    73,    74,
      75,     0,    76,    77,     0,    78,    79,    80,    81,    82,
       0,    83,    84,   -69,    85,    86,     0,     0,  -216,     0,
      87,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
       0,   103,     0,  -216,     0,   887,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,   295,     0,   196,
       0,     0,     0,    31,  -216,  -216,     0,     0,   197,    34,
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
       0,     0,     0,   101,     0,   202,     0,   103,     0,  -216,
       0,     0,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,   635,     0,    13,     0,     0,    16,    17,   637,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   198,   643,   199,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   200,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   201,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,   506,   461,   462,   463,
     464,   465,     0,     0,   468,   469,   470,   471,     0,   473,
     474,     0,   825,   826,   827,   828,   829,   656,     0,   657,
       0,    95,     0,   658,   659,   660,   661,   662,   663,   664,
     830,   666,   667,    97,   831,    99,   -93,   669,   670,   832,
     672,   202,     0,   103,     0,     0,     0,     0,   104,     5,
       6,     7,     8,     9,    10,    11,    12,     0,   635,     0,
      13,     0,     0,    16,    17,   637,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,   643,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   201,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,   506,   461,   462,   463,   464,   465,     0,     0,
     468,   469,   470,   471,     0,   473,   474,     0,   825,   826,
     827,   828,   829,   656,     0,   657,     0,    95,     0,   658,
     659,   660,   661,   662,   663,   664,   830,   666,   667,    97,
     831,    99,     0,   669,   670,   832,   672,   202,     0,   103,
       0,     0,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,     0,    16,
      17,     0,    18,     0,   195,    20,     0,    21,     0,     0,
       0,     0,    26,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,    33,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   198,     0,   199,     0,
       0,    43,    44,     0,    45,    46,    47,    48,     0,    49,
      50,     0,    51,    52,    53,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   200,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   102,     0,   103,     0,     0,     0,     0,
     104,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,   380,    23,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   198,     0,   199,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,    56,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   200,     0,    65,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     201,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,    85,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,     0,     0,     0,     0,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   198,     0,
     199,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,  -745,     0,     0,     0,  -745,     0,
       0,    70,    71,    72,    73,    74,   201,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,  -394,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
    -394,     0,     0,   101,     0,   202,  -394,   103,     0,     0,
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
       0,    95,   354,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,     0,     0,     0,     0,   104,     5,
       6,     7,     8,     9,   383,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,    19,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,  -216,  -216,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,    64,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   201,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,    83,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   202,     0,   103,
       0,  -216,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,   740,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,   741,
       0,    36,     0,     0,    38,     0,   198,     0,   199,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   200,     0,   742,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   201,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,   743,    86,
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
     201,     0,    76,     0,     0,    78,   317,   318,     0,    82,
     353,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,   801,     0,     0,     0,   104,     5,     6,     7,
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
       0,    78,   317,   318,     0,    82,   353,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   202,     0,   103,   803,     0,
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
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   319,     0,     0,   101,
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
    -546,     0,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,  -216,  -216,     0,     0,   197,    34,     0,     0,
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
       0,   101,     0,   202,     0,   103,     0,  -216,     0,     0,
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
       0,   103,     0,     0,  -546,     0,   104,     5,     6,     7,
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
       0,     0,     0,   101,     0,   202,     0,   103,   809,     0,
       0,     0,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   198,     0,   199,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,   957,
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
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,  -402,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,  -402,     0,     0,   101,     0,   202,  -402,   103,
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
    -387,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,  -387,     0,
       0,   101,     0,   202,  -387,   103,     0,     0,     0,     0,
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
      90,    91,    92,    93,    94,     0,  -395,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,  -395,     0,     0,   101,     0,   202,
    -395,   103,     0,     0,     0,     0,   104,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,   259,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   198,     0,
     199,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   201,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
     260,    86,     0,     0,     0,     0,     0,     0,     0,    88,
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
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,  -745,     0,     0,     0,
    -745,     0,     0,    70,    71,    72,    73,    74,   201,     0,
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
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   198,     0,   199,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,   599,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   200,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   201,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,   600,     0,    88,     0,     0,
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
       0,   600,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,     0,     0,   609,     0,   104,     5,     6,     7,
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
      94,     0,  -423,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   202,     0,   103,  -423,     0,
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
      84,     0,     0,    86,     0,     0,     0,     0,     0,   600,
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
       0,   101,     0,   202,     0,   103,     0,     0,   609,     0,
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
     799,   103,     0,     0,     0,     0,   104,     5,     6,     7,
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
       0,    78,     0,     0,     0,    82,  1010,     0,    84,     0,
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
       0,   202,     0,   103,     0,     0,  1047,     0,   104,     5,
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
       0,     0,  1052,     0,   104,     5,     6,     7,     8,     9,
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
       0,     0,     0,    82,  1207,     0,    84,     0,     0,    86,
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
       0,     0,     0,     0,  1077,    34,     0,     0,     0,    36,
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
    1080,    34,     0,     0,     0,    36,     0,     0,    38,     0,
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
       0,    31,     0,     0,     0,     0,  1263,    34,     0,     0,
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
       0,     0,  1264,    34,     0,     0,     0,    36,     0,     0,
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
       0,     0,     0,    31,     0,     0,     0,     0,  1266,    34,
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
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   202,     0,   103,     0,     0,
       0,     0,   104,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,  1267,    34,     0,     0,     0,    36,
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
    1268,    34,     0,     0,     0,    36,     0,     0,    38,     0,
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
       0,    31,     0,     0,     0,     0,  1269,    34,     0,     0,
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
       0,     0,     0,    95,     6,     7,     8,     9,    10,    11,
      12,     0,    96,     0,    13,    97,    98,    99,    17,     0,
       0,   101,   195,   202,     0,   103,     0,   294,     0,     0,
     104,     0,     0,     0,     0,   196,     0,     0,     0,     0,
    -216,  -216,     0,     0,   197,     0,     0,   296,     0,     0,
       0,   298,     0,     0,     0,     0,   199,     0,     0,    43,
      44,     0,     0,     0,     0,    48,     0,    49,    50,     0,
       0,     0,     0,     0,     0,     0,   299,     0,    55,   300,
      57,    58,    59,     0,     0,     0,     0,    62,     0,   200,
       0,   301,     0,     0,     0,     0,     0,    70,     0,    72,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
     304,     0,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,  -356,  -216,     0,     0,   104,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,     0,    17,     0,     0,     0,   195,     0,     0,
       0,     0,   294,     0,     0,     0,     0,     0,     0,     0,
     196,     0,     0,     0,     0,  -216,  -216,     0,     0,   197,
       0,     0,   296,     0,     0,     0,   298,     0,     0,     0,
       0,   199,     0,     0,    43,    44,     0,     0,     0,     0,
      48,     0,    49,    50,     0,     0,     0,     0,     0,     0,
       0,   299,     0,    55,   300,    57,    58,    59,     0,     0,
       0,     0,    62,     0,   200,     0,   301,     0,     0,     0,
       0,     0,    70,     0,    72,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,   304,     0,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   633,     0,   634,     0,    90,    91,    92,
      93,    94,     0,     0,   635,     0,    95,     0,     0,   636,
     232,   637,   638,     0,     0,    96,     0,   639,    97,    98,
      99,     0,     0,     0,   101,   235,   202,   196,   103,     0,
    -216,   236,     0,   104,     0,     0,     0,   640,     0,     0,
       0,   238,     0,     0,   641,     0,   642,   643,     0,     0,
       0,     0,     0,     0,     0,   644,     0,     0,     0,     0,
       0,     0,   242,   645,     0,     0,     0,     0,     0,     0,
     244,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     646,     0,     0,     0,     0,     0,     0,     0,     0,   246,
     247,     0,   647,     0,   249,     0,   648,     0,     0,   251,
       0,     0,     0,   649,     0,     0,   253,     0,     0,   650,
       0,     0,     0,     0,     0,     0,     0,     0,   506,   461,
     462,   463,   464,   465,     0,     0,   468,   469,   470,   471,
       0,   473,   474,     0,   651,   652,   653,   654,   655,   656,
       0,   657,     0,     0,     0,   658,   659,   660,   661,   662,
     663,   664,   665,   666,   667,     0,   668,     0,     0,   669,
     670,   671,   672,     0,     0,   673,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   195,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   196,     0,
       0,     0,    31,  -216,  -216,     0,     0,   197,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   198,     0,   199,
       0,     0,    43,    44,     0,     0,    46,     0,    48,     0,
      49,    50,     0,    51,    52,     0,     0,     0,     0,     0,
       0,    55,     0,    57,     0,    59,     0,     0,     0,     0,
      62,    63,   200,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   201,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,     0,    98,    99,     0,
       0,     0,   101,     0,   202,     0,   103,     0,  -216,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,  -216,  -216,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   201,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,    90,    21,
      92,     0,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,  -216,  -216,     0,     0,   197,    34,
       0,    99,     0,    36,     0,     0,    38,   202,   198,   103,
     199,  -216,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   201,     0,    76,     0,
     698,    78,     0,     0,     0,    82,     0,     0,    84,   635,
       0,    86,     0,     0,   989,   990,   637,   991,     0,     0,
       0,     0,   992,     0,     0,     0,     0,     0,     0,     0,
     700,     0,     0,     0,     0,     0,    90,     0,    92,     0,
       0,     0,   993,     0,     0,     0,   994,     0,     0,   995,
       0,   198,   643,     0,     0,     0,     0,     0,     0,    99,
     701,     0,     0,     0,     0,     0,     0,   103,   996,  -216,
       0,     0,     0,     0,     0,   997,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   998,     0,     0,     0,     0,
       0,     0,     0,     0,   999,  1000,     0,  1001,     0,  1002,
       0,   702,     0,     0,     0,     0,     0,   703,  1003,     0,
       0,  1004,     0,     0,  1005,     0,     0,     0,     0,     0,
       0,     0,     0,   506,   461,   462,   463,   464,   465,     0,
       0,   468,   469,   470,   471,     0,   473,   474,     0,   651,
     652,   653,   654,   655,   656,     0,   657,     0,     0,     0,
     658,   659,   660,   661,   662,   663,   664,   665,   666,   667,
     230,   668,     0,     0,   669,   670,   671,   672,  1006,   635,
       0,     0,     0,     0,   231,   232,   637,   233,     0,     0,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
     235,     0,     0,     0,     0,     0,   236,     0,     0,     0,
       0,     0,   237,     0,     0,     0,   238,     0,     0,   239,
       0,   240,   643,     0,     0,     0,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,   242,   243,     0,
       0,     0,     0,     0,     0,   244,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   245,     0,     0,     0,     0,
       0,   430,     0,     0,   246,   247,     0,   248,   431,   249,
       0,   250,     0,     0,   251,     0,     0,     0,   252,     0,
     432,   253,     0,     0,   254,     0,     0,     0,     0,     0,
       0,     0,     0,   506,   461,   462,   463,   464,   465,     0,
       0,   468,   469,   470,   471,     0,   473,   474,     0,   651,
     652,   653,   654,   655,   656,     0,   657,     0,     0,     0,
     658,   659,   660,   661,   662,   663,   664,   665,   666,   667,
       0,   668,     0,     0,   669,   670,   671,   672,     0,   433,
       0,   430,     0,   434,     0,     0,     0,     0,   431,     0,
       0,     0,  1225,     0,     0,     0,     0,     0,     0,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   435,   506,   461,   462,   463,   464,
     465,     0,     0,   468,   469,   470,   471,     0,   473,   474,
       0,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,     0,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,     0,     0,   456,   457,   458,   433,
     365,   430,     0,   434,     0,     0,     0,   459,   431,     0,
       0,     0,  1310,     0,     0,     0,     0,     0,     0,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   435,   506,   461,   462,   463,   464,
     465,     0,     0,   468,   469,   470,   471,     0,   473,   474,
       0,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,     0,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,     0,     0,   456,   457,   458,   433,
     365,   430,     0,   434,     0,     0,     0,   459,   431,     0,
       0,     0,  1311,     0,     0,     0,     0,     0,     0,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   435,   506,   461,   462,   463,   464,
     465,     0,     0,   468,   469,   470,   471,     0,   473,   474,
       0,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,     0,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,     0,     0,   456,   457,   458,   433,
     365,   430,     0,   434,     0,     0,     0,   459,   431,     0,
       0,     0,  1312,     0,     0,     0,     0,     0,     0,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   435,   506,   461,   462,   463,   464,
     465,     0,     0,   468,   469,   470,   471,     0,   473,   474,
       0,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,     0,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,     0,     0,   456,   457,   458,   433,
     365,   430,     0,   434,     0,     0,     0,   459,   431,     0,
       0,     0,  1313,     0,     0,     0,     0,     0,     0,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   435,   506,   461,   462,   463,   464,
     465,     0,     0,   468,   469,   470,   471,     0,   473,   474,
       0,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,     0,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,     0,     0,   456,   457,   458,   433,
     365,   430,     0,   434,     0,     0,     0,   459,   431,     0,
       0,     0,  1314,     0,     0,     0,     0,     0,     0,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   435,   506,   461,   462,   463,   464,
     465,     0,     0,   468,   469,   470,   471,     0,   473,   474,
       0,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,     0,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,     0,     0,   456,   457,   458,   433,
     365,     0,     0,   434,     0,     0,   430,   459,     0,     0,
       0,     0,  1315,   431,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   432,   275,     0,     0,     0,
       0,     0,     0,     0,   435,   506,   461,   462,   463,   464,
     465,   982,     0,   468,   469,   470,   471,     0,   473,   474,
       0,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,     0,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,     0,     0,   456,   457,   458,     0,
     365,     0,     0,     0,   433,     0,     0,   459,   434,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   853,     0,   432,     0,   435,
       0,     0,   589,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   436,   437,     0,   438,
     439,   440,     0,   441,   442,   443,     0,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   590,
       0,   456,   457,   458,     0,   365,     0,     0,     0,     0,
       0,     0,   459,     0,     0,     0,   433,     0,     0,     0,
     434,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,   821,     0,     0,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   432,
       0,   435,     0,     0,   822,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,     0,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   823,   591,   456,   457,   458,     0,     0,     0,     0,
       0,     0,     0,     0,   459,     0,   430,     0,   433,     0,
       0,     0,   434,   431,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   432,   850,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   851,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
       0,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,     0,   433,   456,   457,   458,   434,     0,
       0,   430,     0,     0,     0,     0,   459,     0,   431,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     432,   275,     0,     0,     0,     0,     0,     0,     0,   435,
       0,     0,     0,     0,     0,     0,   981,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   436,   437,     0,   438,
     439,   440,     0,   441,   442,   443,     0,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,     0,
       0,   456,   457,   458,     0,   365,     0,   430,     0,   433,
       0,     0,   459,   434,   431,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   432,  1152,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,     0,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,     0,   433,   456,   457,   458,   434,
     365,     0,   430,     0,     0,     0,     0,   459,     0,   431,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,   275,     0,     0,     0,   853,     0,     0,     0,
     435,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,   437,     0,
     438,   439,   440,     0,   441,   442,   443,     0,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
       0,     0,   456,   457,   458,     0,   365,     0,   430,     0,
     433,     0,     0,   459,   434,   431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   275,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   853,     0,     0,     0,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,     0,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,     0,   433,   456,   457,   458,
     434,   365,     0,   430,     0,     0,     0,     0,   459,     0,
     431,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,   435,     0,     0,     0,     0,     0,     0,   844,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,     0,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,     0,     0,   456,   457,   458,     0,   365,     0,   430,
       0,   433,     0,     0,   459,   434,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   432,   913,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   214,     0,   914,     0,   435,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,     0,     0,
       0,   431,     0,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   432,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,     0,   433,   456,   457,
     458,   434,     0,     0,   430,     0,     0,     0,     0,   459,
       0,   431,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   432,   915,     0,     0,     0,     0,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,   916,
       0,     0,   433,     0,     0,     0,   434,     0,     0,   436,
     437,     0,   438,   439,   440,   948,   441,   442,   443,     0,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,     0,     0,   456,   457,   458,   435,     0,     0,
       0,     0,   433,     0,     0,   459,   434,     0,     0,     0,
       0,     0,     0,     0,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,     0,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   435,     0,   456,
     457,   458,     0,   365,     0,     0,     0,     0,     0,     0,
     459,     0,     0,     0,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,     0,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   430,     0,   456,
     457,   458,     0,   230,   431,     0,     0,     0,     0,     0,
     459,     0,     0,     0,     0,     0,   432,   231,   232,     0,
     233,  1125,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,   235,     0,     0,     0,   430,     0,   236,
       0,     0,     0,     0,   431,   237,     0,     0,     0,   238,
       0,     0,   239,     0,   240,     0,   432,  1144,  1126,     0,
       0,     0,     0,   241,     0,     0,     0,     0,     0,     0,
     242,   243,     0,     0,     0,   433,     0,     0,   244,   434,
       0,     0,   430,     0,     0,     0,     0,     0,   245,   431,
       0,     0,     0,     0,     0,     0,     0,   246,   247,     0,
     248,   432,   249,     0,   250,     0,     0,   251,     0,     0,
     435,   252,   543,     0,   253,   433,     0,   254,     0,   434,
       0,     0,     0,     0,     0,     0,     0,   436,   437,     0,
     438,   439,   440,     0,   441,   442,   443,     0,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     435,     0,   456,   457,   458,     0,     0,     0,   430,     0,
     433,     0,     0,   459,   434,   431,     0,   436,   437,     0,
     438,   439,   440,     0,   441,   442,   443,   432,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
       0,   853,   456,   457,   458,   435,   365,     0,     0,   430,
     530,     0,     0,   459,     0,     0,   431,     0,     0,     0,
       0,     0,   436,   437,     0,   438,   439,   440,   432,   441,
     442,   443,     0,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,     0,   433,   456,   457,   458,
     434,     0,     0,     0,     0,   430,  1162,     0,   459,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   432,     0,     0,   853,     0,     0,
       0,   435,     0,     0,     0,     0,     0,   433,     0,     0,
       0,   434,     0,     0,     0,     0,     0,     0,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,     0,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,     0,   435,   456,   457,   458,     0,     0,     0,     0,
       0,     0,  1205,   433,   459,     0,     0,   434,     0,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,     0,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   430,     0,   456,   457,   458,     0,   435,   431,
       0,     0,     0,     0,     0,   459,     0,     0,     0,     0,
       0,   432,     0,     0,     0,   436,   437,     0,   438,   439,
     440,     0,   441,   442,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,     0,     0,
     456,   457,   458,     0,   554,     0,     0,     0,     0,     0,
       0,   459,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,     0,   430,     0,   434,     0,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   435,     0,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,     0,
     431,     0,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   432,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,     0,   558,   456,   457,   458,
     433,     0,     0,     0,   434,     0,     0,     0,   459,   430,
     817,     0,     0,   734,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,   435,     0,     0,     0,     0,
       0,   433,     0,     0,     0,   434,     0,     0,     0,     0,
       0,     0,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,     0,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,     0,   435,   456,   457,   458,
       0,     0,     0,     0,     0,     0,     0,   433,   459,     0,
       0,   434,     0,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,     0,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   430,     0,   456,   457,
     458,     0,   435,   431,     0,   800,     0,     0,     0,   459,
       0,     0,     0,     0,     0,   432,     0,     0,     0,   436,
     437,     0,   438,   439,   440,     0,   441,   818,   443,     0,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   430,     0,   456,   457,   458,     0,     0,   431,
       0,     0,     0,     0,     0,   459,     0,     0,     0,     0,
       0,   432,     0,     0,   946,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,     0,     0,     0,   434,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,   874,   435,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,   434,     0,   436,   437,     0,   438,
     439,   440,     0,   441,   442,   443,     0,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,     0,
       0,   456,   457,   458,     0,   435,     0,     0,     0,     0,
     433,     0,   459,   430,   434,     0,     0,     0,     0,     0,
     431,     0,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   432,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   435,     0,   456,   457,   458,
       0,     0,     0,   430,  1016,     0,     0,     0,   459,     0,
     431,     0,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   432,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,     0,     0,   456,   457,   458,
       0,   433,     0,     0,   960,   434,     0,     0,   459,   430,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,     0,   435,     0,     0,     0,
       0,   433,     0,     0,     0,   434,     0,     0,     0,     0,
       0,     0,     0,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,     0,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   435,     0,   456,   457,
     458,     0,     0,     0,     0,   961,     0,   433,     0,   459,
       0,   434,     0,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,     0,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   430,  1122,   456,   457,
     458,     0,   435,   431,     0,     0,     0,     0,     0,   459,
       0,     0,     0,     0,     0,   432,     0,     0,     0,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,     0,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,     0,     0,   456,   457,   458,     0,     0,     0,
     430,  1071,     0,     0,     0,   459,     0,   431,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   432,
       0,     0,     0,     0,   433,     0,     0,     0,   434,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   430,     0,     0,     0,     0,     0,     0,   431,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   435,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   436,   437,   433,   438,
     439,   440,   434,   441,   442,   443,     0,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   430,
       0,   456,   457,   458,     0,     0,   431,     0,     0,   214,
       0,     0,   459,   435,     0,     0,     0,     0,   432,   433,
       0,     0,     0,   434,     0,     0,     0,     0,     0,     0,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
       0,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,     0,   435,   456,   457,   458,     0,     0,
       0,     0,     0,     0,     0,     0,   459,     0,     0,     0,
       0,   436,   437,     0,   438,   439,   440,   433,   441,   442,
     443,   434,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   430,  1138,   456,   457,   458,     0,
       0,   431,     0,     0,     0,     0,     0,   459,     0,     0,
       0,  1165,   435,   432,  1186,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,     0,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,     0,     0,   456,   457,   458,     0,     0,     0,
       0,     0,     0,     0,     0,   459,     0,     0,     0,     0,
     430,     0,   433,     0,     0,     0,   434,   431,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   432,
    1189,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   435,     0,     0,
       0,   430,     0,     0,     0,     0,     0,     0,   431,     0,
       0,     0,     0,     0,   436,   437,     0,   438,   439,   440,
     432,   441,   442,   443,     0,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,     0,   433,   456,
     457,   458,   434,     0,     0,     0,   430,  1213,     0,     0,
     459,     0,     0,   431,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   432,     0,     0,     0,     0,
       0,     0,     0,   435,     0,     0,     0,     0,     0,   433,
       0,     0,     0,   434,     0,     0,     0,     0,     0,     0,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
       0,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,     0,   435,   456,   457,   458,     0,     0,
       0,     0,   430,     0,   433,     0,   459,     0,   434,   431,
       0,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,   432,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,     0,     0,   456,   457,   458,   435,
       0,     0,     0,   430,     0,  1191,     0,   459,     0,     0,
     431,  1255,     0,     0,     0,     0,   436,   437,     0,   438,
     439,   440,   432,   441,   442,   443,     0,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,     0,
     433,   456,   457,   458,   434,     0,     0,     0,   430,     0,
       0,     0,   459,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,     0,     0,
       0,     0,     0,     0,     0,   435,     0,     0,     0,     0,
       0,   433,     0,     0,     0,   434,     0,     0,     0,     0,
       0,     0,   436,   437,  1262,   438,   439,   440,     0,   441,
     442,   443,     0,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,     0,   435,   456,   457,   458,
       0,     0,     0,     0,   430,     0,   433,     0,   459,     0,
     434,   431,     0,   436,   437,     0,   438,   439,   440,  1265,
     441,   442,   443,   432,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,     0,     0,   456,   457,
     458,   435,     0,     0,     0,   430,     0,     0,     0,   459,
       0,     0,   431,     0,     0,     0,     0,     0,   436,   437,
       0,   438,   439,   440,   432,   441,   442,   443,     0,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,     0,   433,   456,   457,   458,   434,     0,     0,     0,
     430,     0,     0,     0,   459,  1316,     0,   431,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,   435,     0,     0,
       0,     0,     0,   433,     0,     0,     0,   434,     0,     0,
       0,     0,     0,     0,   436,   437,  1317,   438,   439,   440,
       0,   441,   442,   443,     0,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,     0,   435,   456,
     457,   458,     0,     0,     0,     0,   430,     0,   433,     0,
     459,     0,   434,   431,     0,   436,   437,     0,   438,   439,
     440,  1318,   441,   442,   443,   432,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,     0,     0,
     456,   457,   458,   435,     0,     0,     0,   430,     0,     0,
       0,   459,     0,     0,   431,     0,     0,     0,     0,     0,
     436,   437,     0,   438,   439,   440,   432,   441,   442,   443,
       0,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,     0,   433,   456,   457,   458,   434,     0,
       0,     0,   430,     0,     0,     0,   459,  1319,     0,   431,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,     0,   435,
       0,     0,     0,     0,     0,   433,     0,     0,     0,   434,
       0,     0,     0,     0,     0,     0,   436,   437,  1320,   438,
     439,   440,     0,   441,   442,   443,     0,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,     0,
     435,   456,   457,   458,     0,     0,     0,     0,   430,     0,
     433,     0,   459,     0,   434,   431,     0,   436,   437,     0,
     438,   439,   440,  1321,   441,   442,   443,   432,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
       0,     0,   456,   457,   458,   435,     0,     0,     0,   430,
       0,     0,     0,   459,     0,     0,   431,     0,     0,     0,
       0,     0,   436,   437,     0,   438,   439,   440,   432,   441,
     442,   443,     0,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,     0,   433,   456,   457,   458,
     434,     0,     0,     0,     0,     0,     0,     0,   459,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   435,     0,     0,     0,     0,     0,   936,     0,     0,
       0,   434,     0,     0,     0,     0,     0,     0,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   432,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,     0,   435,   456,   457,   458,     0,   433,     0,     0,
       0,   434,     0,     0,   459,     0,     0,     0,     0,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,     0,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   435,     0,   456,   457,   458,   433,     0,     0,
       0,   434,     0,     0,     0,   459,     0,     0,     0,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,     0,
     444,   445,   446,   447,     0,   449,   450,   451,   452,   453,
     454,   455,     0,     0,   456,   457,   458,     0,     0,     0,
       0,     0,     0,     0,     0,   459,     0,     0,     0,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,     0,
     444,   445,   446,   447,     0,   449,   450,   451,   452,   453,
     401,   455,   230,     0,   456,   457,   458,     0,     0,     0,
       0,     0,     0,     0,     0,   459,   231,   232,     0,   233,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   235,     0,     0,     0,     0,     0,   236,     0,
       0,     0,     0,     0,   237,     0,     0,     0,   238,     0,
       0,   239,     0,   240,     0,     0,     0,     0,   230,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,   242,
     243,     0,   231,   232,     0,   233,     0,   244,     0,     0,
     234,     0,     0,     0,     0,     0,     0,   245,   235,     0,
       0,     0,     0,     0,   236,     0,   246,   247,     0,   248,
     237,   249,     0,   250,   238,     0,   251,   239,     0,   240,
     252,     0,     0,   253,   230,     0,   254,     0,   241,     0,
       0,     0,     0,     0,     0,   242,   243,     0,   231,   232,
       0,   233,     0,   244,     0,     0,   234,     0,    23,     0,
      89,     0,     0,   245,   235,     0,     0,     0,     0,     0,
     236,     0,   246,   247,     0,   248,   237,   249,     0,   250,
     238,     0,   251,   239,     0,   240,   252,     0,     0,   253,
    -327,     0,   254,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,     0,  -327,  -327,     0,  -327,     0,   244,
      56,     0,  -327,     0,     0,     0,    89,     0,     0,   245,
    -327,     0,    65,     0,     0,     0,  -327,     0,   246,   247,
       0,   248,  -327,   249,     0,   250,  -327,     0,   251,  -327,
       0,  -327,   252,     0,     0,   253,   230,    85,   254,     0,
    -327,     0,     0,     0,     0,     0,     0,  -327,  -327,     0,
     231,   232,     0,   233,     0,  -327,     0,     0,   234,     0,
       0,     0,     0,     0,     0,  -327,   235,     0,     0,     0,
       0,     0,   236,     0,  -327,  -327,     0,  -327,   237,  -327,
       0,  -327,   238,     0,  -327,   239,     0,   240,  -327,     0,
       0,  -327,  -328,     0,  -327,     0,   241,     0,     0,     0,
       0,     0,     0,   242,   243,     0,  -328,  -328,     0,  -328,
       0,   244,     0,     0,  -328,     0,     0,     0,     0,     0,
       0,   245,  -328,     0,     0,     0,     0,     0,  -328,     0,
     246,   247,     0,   248,  -328,   249,     0,   250,  -328,     0,
     251,  -328,     0,  -328,   252,     0,     0,   253,     0,     0,
     254,     0,  -328,     0,     0,     0,     0,     0,     0,  -328,
    -328,     0,     0,     0,     0,     0,     0,  -328,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -328,     0,     0,
       0,     0,     0,     0,     0,     0,  -328,  -328,     0,  -328,
       0,  -328,     0,  -328,     0,     0,  -328,     0,     0,     0,
    -328,     0,     0,  -328,     0,     0,  -328
};

static const yytype_int16 yycheck[] =
{
       2,    62,     2,   112,   181,   152,    20,   487,   402,     2,
     552,   165,    62,    62,   422,   556,    26,   229,   512,   589,
     676,   629,   678,    25,   223,   266,   789,   626,   922,   823,
     133,   880,    25,   818,   547,   384,   783,   578,   841,     1,
     962,    24,    32,     1,     8,    56,     3,    67,   407,    32,
     291,     1,   522,    47,   266,     1,    66,    40,    41,    42,
      56,     1,   886,   148,    61,    35,    32,    87,   148,    32,
      32,    47,   148,    75,   103,   148,     0,    79,    80,   291,
      32,    46,    75,   103,    32,   262,    79,    80,   121,   174,
     331,     2,    32,   121,   174,    32,  1180,    54,   174,    61,
     102,    47,    47,   176,   106,    32,   166,    24,   121,   102,
      24,   148,    32,   106,    25,    32,     3,    75,    32,   331,
      85,    61,   119,    40,    41,    42,    40,    41,    42,    67,
      93,    93,    40,    41,    47,   172,    93,   120,  1032,     3,
     964,   111,   171,   176,   106,   121,    91,   104,   176,    87,
     115,    56,    47,    93,   148,   121,   517,   119,   148,   629,
     118,  1245,   173,   176,    75,   103,   106,    54,    79,    80,
    1019,   770,   148,    93,   121,    28,   126,   173,    75,   119,
     126,   171,   541,   544,  1106,   147,   150,    40,    41,    42,
      54,   102,   175,   956,   121,   106,   148,   147,   148,   962,
     176,   147,   148,   120,   166,   171,   120,   147,   171,   171,
    1059,  1060,   753,   215,   172,  1018,    18,   104,   965,   171,
     166,   118,   215,   171,   174,   603,   121,   605,   606,  1014,
     442,   171,   610,   611,   171,   148,   477,   478,    27,    56,
     104,   735,    22,   592,   171,   422,   166,   124,     2,    56,
      46,   171,   822,   148,   110,    36,    27,    47,   175,   119,
     148,   175,   166,   176,    56,   477,   478,   175,   270,   173,
      66,    25,     3,   275,   787,   172,    78,   270,    46,   148,
     383,   176,   275,  1132,   172,    91,    27,   148,   148,    85,
      79,   229,    73,   402,   171,    97,   148,   150,    66,   152,
     512,    91,    91,   172,   215,    76,    47,   173,    79,    89,
      51,   164,   114,   174,    11,    27,   148,    85,  1167,   115,
      91,    75,   174,    54,  1087,    79,    80,   116,   266,  1223,
    1124,   530,  1126,   113,  1174,    76,   935,   491,    79,   110,
      81,    56,   174,  1106,   556,   406,   199,   115,   102,   351,
      91,   351,   106,   291,    53,   963,    90,   173,   351,   270,
      94,   148,   148,   365,   275,   148,   578,    79,    31,   110,
      27,   141,   365,   375,   148,    74,    79,    80,   148,    91,
     166,   148,   375,   393,   878,    84,   388,   174,   398,   126,
      47,   174,    56,   331,    51,   388,   595,   148,   110,   166,
     174,    56,   172,    31,   148,     8,   259,   260,   148,  1249,
    1250,  1251,  1252,  1253,  1254,   152,  1072,  1073,   271,    76,
     422,   148,    79,   174,    81,   919,   166,    90,    90,   807,
     174,    94,    94,   174,    91,   104,  1006,   148,   126,   166,
     351,   142,   922,   148,   985,   166,   466,   467,    31,   150,
     148,   172,   472,   110,   365,   475,   476,    83,   311,   148,
      86,   215,    90,   174,   375,   153,    94,   155,   156,   174,
     158,   159,   148,   148,   148,   485,   174,   388,   148,   142,
     143,   542,   145,   146,   147,   174,   149,   150,   882,   897,
     166,   154,   542,   542,    56,     2,   166,   148,   161,   174,
     174,   503,   148,   505,   167,   168,   169,    90,    56,   362,
     503,    94,   505,   148,   142,   166,   270,   174,    25,   147,
     166,   275,   150,   735,   148,   148,   154,    50,   466,   467,
      53,   166,    90,   173,   472,   175,    94,   475,   476,   477,
     478,   753,   166,   166,   397,   568,    56,   570,   568,   402,
     570,    74,  1032,  1123,    77,  1125,   173,   148,   175,   142,
     143,    84,   145,   146,   147,   575,   149,   150,    75,   422,
     148,   154,    79,    80,   512,   166,    31,   160,   161,   173,
     582,   164,   582,   597,   167,   168,   169,   110,   166,   582,
      56,   166,   503,   101,   505,   102,   142,   351,   173,   106,
     175,   147,    56,   166,   150,   817,   818,   173,   154,   172,
      56,   365,    90,    90,     3,    90,    94,    94,   556,    94,
     173,   375,   175,   173,     2,   175,    27,   171,    56,   173,
     568,    20,   570,   486,   388,    90,    79,    80,  1180,    94,
     578,   173,    56,   175,    33,    56,    47,    25,     3,   173,
      51,   175,   173,   141,   175,   292,   293,   171,  1036,  1037,
     513,    56,  1040,  1041,   517,    54,   878,  1045,  1046,  1063,
     173,   582,   166,   147,    63,    76,   165,   530,    79,   166,
      81,   166,   166,   166,   166,   166,   539,   142,   143,   843,
      91,   544,   147,    56,   149,   150,   166,    75,    27,   154,
     173,    79,    80,  1245,   171,   166,   161,   919,   215,   110,
      85,   171,   167,   168,   169,   104,   166,   173,    47,   148,
     897,   110,    51,   174,   102,   173,   173,   147,   106,   166,
     166,   751,   173,   880,     8,   588,   883,     3,  1116,  1117,
    1118,  1119,   173,  1223,   746,    66,   599,    76,   171,   503,
      79,   505,    81,   773,   172,   757,   126,    91,   760,    91,
     126,    69,    91,   270,   757,   150,   173,   760,   275,   126,
     174,   148,   126,   985,   126,    34,    34,    66,   174,   126,
     174,   110,   147,   174,    21,   171,    22,   120,   150,   173,
      26,    27,   148,   106,   173,   166,   176,   735,   166,    35,
     150,    37,    38,    34,   150,   150,   150,    43,   150,   150,
     150,   150,   150,   751,    50,   753,   174,   166,   166,   880,
     150,    34,   150,   884,   150,   150,   150,   142,   582,   682,
     150,   166,   893,   147,    34,   773,   173,   215,   848,   166,
     171,    77,   148,    79,   351,    81,   757,    83,   850,   760,
      86,   174,    47,    89,   856,    91,   172,   850,   365,  1100,
    1101,   166,   173,   856,   176,   172,   868,   176,   375,   166,
     872,   174,  1019,   166,   110,   868,   176,   113,   173,   872,
     116,   388,   173,   147,   166,   148,   174,    32,  1100,  1101,
      32,   176,   270,    34,   174,   176,   166,   275,   176,   174,
     165,   174,   148,    31,   166,   141,   166,   111,   165,   762,
     577,   985,  1059,   375,   265,   815,   769,    24,   367,   977,
     392,   952,   339,   528,   542,   897,   779,   386,   884,   539,
     166,   305,   893,  1245,   936,   937,   172,  1065,  1255,   850,
     878,   943,  1248,   945,   797,   856,   948,   682,   842,   131,
     943,  1060,   945,   525,  1063,   948,   769,   868,  1019,    -1,
      -1,   872,    90,   973,  1141,    31,    94,    -1,    -1,    -1,
     972,  1148,    -1,   351,    -1,    -1,    -1,    -1,    -1,   972,
      -1,   919,   984,    -1,    -1,  1132,    -1,   365,    -1,   842,
      -1,   984,    -1,    -1,    -1,    -1,   503,   375,   505,    -1,
      -1,    -1,    -1,   757,    -1,    -1,   760,    -1,  1022,  1023,
     388,    -1,    -1,    -1,   142,   143,    -1,    -1,   146,   147,
      -1,   149,   150,    -1,    90,    -1,   154,    -1,    94,   882,
      -1,    -1,   943,   161,   945,    -1,    -1,   948,    -1,   167,
     168,   169,  1044,    -1,   897,    -1,    -1,   985,  1050,    -1,
      -1,  1044,    -1,    -1,    -1,    -1,    -1,  1050,  1167,    -1,
      -1,   972,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1132,    -1,   984,    -1,   582,   142,    -1,    -1,    -1,
      -1,   147,    -1,   149,   150,    -1,    -1,    -1,   154,    -1,
      -1,    -1,  1094,    -1,    -1,   161,   850,    -1,    -1,   952,
      -1,  1094,   856,   169,  1281,  1282,    -1,    -1,  1285,  1286,
    1287,    -1,  1289,    -1,   868,    -1,    -1,    -1,   872,    -1,
      -1,    -1,    -1,    -1,   977,   503,    -1,   505,    -1,    -1,
      -1,    -1,    -1,  1044,    -1,    -1,    -1,    -1,    -1,  1050,
    1142,    -1,  1144,    -1,    -1,    -1,    -1,  1149,    -1,  1142,
    1152,  1144,    -1,    -1,    -1,    -1,  1149,    -1,  1160,  1152,
    1162,    31,  1100,  1101,    -1,    -1,    -1,  1160,    -1,  1162,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1094,    -1,    -1,    -1,    -1,    -1,   943,
      -1,   945,  1194,    -1,   948,    -1,    -1,    -1,  1200,    -1,
      -1,  1194,  1204,  1205,   582,    -1,    -1,  1200,    -1,    -1,
    1063,  1204,  1205,    -1,    -1,    -1,  1218,    -1,   972,    -1,
      90,    -1,    -1,  1225,    94,  1218,    -1,    -1,    -1,    -1,
     984,  1142,  1225,  1144,    -1,  1237,    -1,    -1,  1149,  1241,
      -1,  1152,    -1,    -1,  1237,    -1,    -1,    -1,  1241,  1160,
     757,  1162,    -1,   760,  1256,     2,  1256,    -1,    -1,    -1,
      -1,    -1,    -1,  1256,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,   147,    25,   149,
     150,    -1,    -1,  1194,   154,    -1,    -1,    -1,    -1,  1200,
    1044,   161,    -1,  1204,  1205,    -1,  1050,   167,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1218,  1310,  1311,
    1312,  1313,  1314,  1315,  1225,    -1,    -1,  1310,  1311,  1312,
    1313,  1314,  1315,    -1,    -1,     3,  1237,    -1,    75,    -1,
    1241,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,    17,
    1094,    -1,    20,   850,    -1,  1256,    -1,    25,    -1,   856,
      -1,    -1,    -1,    -1,    -1,   102,    -1,    35,    -1,   106,
      -1,   868,    -1,    -1,    -1,   872,    -1,    45,    -1,    -1,
     390,   391,    -1,   393,    52,    -1,    54,    -1,    -1,   757,
      -1,    -1,   760,    -1,     2,    63,    -1,    -1,  1142,    -1,
    1144,    -1,    -1,    71,    -1,  1149,    -1,    -1,  1152,  1310,
    1311,  1312,  1313,  1314,  1315,    -1,  1160,    25,  1162,    -1,
      88,    -1,    -1,   433,   434,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,   111,    -1,    -1,   943,    -1,   945,   117,
    1194,   948,    -1,    -1,    -1,    -1,  1200,    -1,    -1,    -1,
    1204,  1205,    -1,    -1,    -1,    -1,    -1,    75,    -1,   479,
     480,    79,    80,    -1,  1218,   972,    -1,    -1,   215,    -1,
      -1,  1225,   850,    -1,    -1,    -1,    -1,   984,   856,    -1,
      -1,    -1,    -1,  1237,   102,    -1,    -1,  1241,   106,    -1,
     868,    -1,    -1,     1,   872,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1256,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     2,    22,    -1,    -1,    -1,    26,    27,
      -1,    -1,    -1,   270,    -1,    -1,    -1,    35,   275,    37,
      38,    -1,    -1,    -1,    -1,    43,    25,  1044,    -1,    -1,
      -1,    -1,    50,  1050,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   574,   575,  1310,  1311,  1312,  1313,
    1314,  1315,    -1,    -1,    -1,   943,    74,   945,    -1,    77,
     948,    79,    -1,    81,    -1,    83,    84,    -1,    86,    -1,
      -1,    89,    -1,    91,    -1,    -1,    75,  1094,    -1,    -1,
      79,    80,    -1,    -1,   972,    -1,    -1,   215,    -1,    -1,
      -1,    -1,   110,    -1,   351,   113,   984,    -1,   116,    -1,
      -1,    -1,    -1,   102,    -1,    -1,    -1,   106,   365,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   375,    -1,
      -1,    -1,    -1,   141,    -1,  1142,    -1,  1144,    -1,    -1,
      -1,   388,  1149,    -1,    -1,  1152,    -1,    -1,    -1,    -1,
      -1,    -1,   270,  1160,    -1,  1162,    -1,   275,   166,    -1,
      -1,    -1,    -1,    -1,   172,    -1,  1044,    -1,    -1,    -1,
      -1,    -1,  1050,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1194,    -1,    -1,
      -1,    -1,    -1,  1200,    -1,    -1,    -1,  1204,  1205,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1218,    -1,    -1,    -1,    -1,  1094,    -1,  1225,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   215,    -1,    -1,    -1,
    1237,    -1,    -1,   351,  1241,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   365,    -1,  1256,
      -1,    -1,    -1,    -1,    -1,    -1,   503,   375,   505,    -1,
      -1,    -1,    -1,    -1,  1142,    -1,  1144,    -1,    -1,    -1,
     388,  1149,    -1,    -1,  1152,    -1,    -1,    -1,    -1,    -1,
      -1,   270,  1160,    -1,  1162,    -1,   275,    -1,    -1,    -1,
      -1,     2,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      94,    -1,    -1,  1310,  1311,  1312,  1313,  1314,  1315,    -1,
      -1,    -1,    -1,    -1,    25,    -1,  1194,    -1,    -1,    -1,
      -1,    -1,  1200,    -1,   844,    -1,  1204,  1205,    -1,    -1,
      -1,   851,    -1,    -1,    -1,   582,    -1,   857,    -1,    -1,
    1218,    -1,   862,    -1,    -1,    -1,    -1,  1225,   142,   143,
     870,   145,   146,   147,    -1,   149,   150,   151,    -1,  1237,
     154,    -1,   351,  1241,    75,    -1,   160,   161,    -1,    -1,
     164,    -1,    -1,   167,   168,   169,   365,    -1,  1256,    -1,
      -1,    -1,    -1,    -1,   178,   503,   375,   505,    -1,    -1,
      -1,   102,    -1,    -1,   914,   106,   916,   917,    -1,   388,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,    -1,   936,   937,    -1,    -1,
      -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1310,  1311,  1312,  1313,  1314,  1315,    17,    18,
      -1,    20,    -1,    -1,   170,    -1,    25,   173,    -1,   175,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,   981,   982,   983,   582,    -1,    45,    22,    -1,    -1,
      49,    26,    27,    52,    -1,    54,    -1,    -1,    -1,    -1,
      35,    -1,    37,    38,    63,    -1,    -1,    -1,    43,    -1,
      -1,    70,    71,    -1,    -1,    50,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,    88,
     757,    -1,    -1,   760,   503,    -1,   505,    -1,    97,    98,
      -1,   100,    77,   102,    79,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,   112,    89,   114,    91,    -1,   117,    -1,
      -1,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,   110,    37,    38,   113,   270,
      -1,   116,    43,    -1,   275,    -1,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   582,   173,    -1,    77,    -1,    79,    -1,
      81,    -1,    83,   850,    -1,    86,    -1,    -1,    89,   856,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,   868,    -1,    -1,    -1,   872,    -1,    -1,    -1,   110,
      -1,    -1,   113,    17,    18,   116,    20,    -1,    -1,   757,
     351,    25,   760,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    -1,   365,    39,    -1,    -1,    -1,    -1,
     141,    45,    -1,    -1,   375,    49,    -1,    -1,    52,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,   388,    -1,    63,
      -1,    -1,    -1,    -1,    -1,   166,    70,    71,    -1,    -1,
      -1,   172,    -1,    -1,    78,    -1,   943,    -1,   945,    -1,
      -1,   948,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    -1,   100,    -1,   102,    -1,
     104,    -1,    -1,   107,    -1,   972,    -1,   111,   112,    -1,
     114,    -1,   850,   117,    -1,    -1,    -1,   984,   856,    -1,
      -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
     868,    -1,    -1,    -1,   872,    -1,    -1,    -1,    17,    18,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,   757,    -1,
      -1,   760,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,   173,
      49,    -1,   503,    52,   505,    54,    -1,  1044,    -1,    -1,
      -1,    -1,    -1,  1050,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,   943,    -1,   945,    -1,    88,
     948,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      -1,   100,    -1,   102,    -1,   104,    -1,  1094,   107,    -1,
      -1,    -1,   111,    -1,   972,   114,    -1,    -1,   117,    -1,
      -1,   850,    -1,    -1,    -1,    -1,   984,   856,    -1,    -1,
      -1,   582,    22,    -1,    -1,    -1,    26,    27,    -1,   868,
      -1,    -1,    -1,   872,    -1,    35,    -1,    37,    38,    -1,
      -1,    -1,    -1,    43,    -1,  1142,    -1,  1144,    -1,    -1,
      50,    -1,  1149,    -1,    -1,  1152,    -1,    -1,    -1,    -1,
      -1,    -1,   171,  1160,   173,  1162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1044,    77,    -1,    79,
      -1,    81,  1050,    83,    -1,    -1,    86,    -1,    -1,    89,
      -1,    91,    -1,    -1,    -1,    -1,    -1,  1194,    -1,    -1,
      -1,    -1,    -1,  1200,   943,    -1,   945,  1204,  1205,   948,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,  1218,    -1,    -1,    -1,    -1,  1094,    -1,  1225,    -1,
      -1,    -1,    -1,   972,    -1,    -1,    -1,    -1,    -1,    -1,
    1237,   141,    -1,    -1,  1241,   984,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1256,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,
      -1,    -1,   172,    -1,  1142,    -1,  1144,    -1,    -1,    -1,
      -1,  1149,    -1,    -1,  1152,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1160,    -1,  1162,    -1,   757,    -1,    -1,   760,
      -1,    -1,    -1,    -1,    -1,  1044,    90,    -1,    -1,    -1,
      94,  1050,    -1,  1310,  1311,  1312,  1313,  1314,  1315,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1194,    -1,    -1,    -1,
      -1,    -1,  1200,    -1,    -1,    -1,  1204,  1205,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1218,    -1,    -1,    -1,    -1,  1094,    -1,  1225,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,  1237,
     154,   155,   156,  1241,   158,   159,   160,   161,    -1,    -1,
     164,    -1,    -1,   167,   168,   169,    -1,    -1,  1256,   850,
      -1,    -1,    -1,    -1,   178,   856,    -1,    -1,    -1,    12,
      -1,    -1,    -1,  1142,    -1,  1144,    19,   868,    -1,    -1,
    1149,   872,    -1,  1152,    -1,    -1,    -1,    -1,    31,    32,
      -1,  1160,    -1,  1162,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1310,  1311,  1312,  1313,  1314,  1315,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1194,    -1,    -1,    -1,    -1,
      -1,  1200,    -1,    -1,    -1,  1204,  1205,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,  1218,
      -1,    94,   943,    -1,   945,    -1,  1225,   948,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1237,    -1,
      -1,    -1,  1241,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,   972,   125,    -1,    -1,    -1,    -1,  1256,    -1,    -1,
      -1,    -1,    -1,   984,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    13,    -1,    -1,    -1,
      17,  1310,  1311,  1312,  1313,  1314,  1315,    -1,    -1,    26,
      -1,    -1,    -1,  1044,    -1,    -1,    33,    -1,    -1,  1050,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      47,    48,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      57,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    66,
      67,    68,    69,    70,    71,    -1,    -1,    -1,    75,    -1,
      77,    -1,    -1,  1094,    -1,    -1,    -1,    84,    -1,    -1,
      87,    -1,    -1,    90,    -1,    92,    -1,    -1,    95,    96,
      -1,    98,    -1,    -1,    -1,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1142,    -1,  1144,   131,    -1,   133,    -1,  1149,    -1,
      -1,  1152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1160,
      -1,  1162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,    -1,    -1,    -1,   174,    -1,    -1,
      -1,    -1,    -1,  1194,    -1,    -1,    -1,    17,    18,  1200,
      20,    -1,    -1,  1204,  1205,    25,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    33,   201,   202,    -1,  1218,    -1,    39,
      -1,    -1,    -1,    -1,  1225,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,  1237,    -1,    -1,    -1,
    1241,    -1,   229,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,  1256,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,   266,
     100,    -1,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,   291,    -1,    -1,    -1,    -1,  1310,
    1311,  1312,  1313,  1314,  1315,   302,   303,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,    -1,
     317,   318,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   331,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   354,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   376,
      -1,   378,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
     387,    -1,   389,   390,   391,   392,   393,    -1,    -1,    -1,
      -1,   398,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   430,   431,   432,   433,   434,   435,    -1,
     437,   438,   439,   440,   441,    -1,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,    -1,    -1,    -1,    -1,    -1,    -1,   466,
     467,    -1,    -1,    -1,    -1,   472,    -1,    -1,   475,   476,
     477,   478,   479,   480,    -1,    -1,   483,   484,   485,    -1,
     487,    -1,    -1,    -1,    -1,    -1,   493,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   505,    -1,
      -1,   508,    -1,    -1,    -1,   512,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   522,    -1,   524,    -1,   215,
      -1,   528,    -1,    -1,   531,   532,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,   542,    26,    27,    -1,    -1,
     547,    -1,    -1,    -1,    -1,    35,    -1,    37,    38,   556,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,   565,    -1,
      50,   568,    -1,   570,    -1,    12,   573,   574,   575,    -1,
      -1,   578,    19,    -1,   270,    -1,    -1,    -1,    -1,   275,
      -1,    -1,   589,   590,    31,    32,    -1,    77,    -1,    79,
      -1,    81,    -1,    83,    -1,    -1,    86,    -1,    -1,    89,
      47,    91,   609,    -1,    -1,    45,    -1,    -1,    -1,    -1,
      -1,    -1,   619,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,   629,   113,    -1,    -1,   116,    -1,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,
      -1,   141,    -1,    -1,    -1,   351,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   673,    -1,    -1,   365,
      -1,   111,    -1,   680,   121,    -1,   166,    -1,   125,   375,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   388,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,   171,    -1,    -1,   734,   735,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   751,    -1,   753,   754,   755,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   773,    -1,    45,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     787,   221,    -1,    -1,   791,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   503,   815,    -1,
      -1,    -1,   819,    -1,   821,   822,   823,    -1,   825,    -1,
     827,   261,   262,   830,   831,    -1,    -1,    -1,    -1,   269,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   844,   278,   846,
     280,   848,    -1,   850,   851,    -1,    -1,   287,    -1,   856,
     857,    -1,    -1,    -1,    -1,   862,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   870,   871,   872,    -1,   874,   875,    -1,
      -1,   878,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,   582,   327,    -1,    35,
      -1,    37,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    -1,   913,   914,   915,   916,
     917,   918,   919,    -1,    -1,   922,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   936,
     937,    77,    -1,    79,    -1,    -1,    -1,    83,    -1,   946,
      86,    -1,    -1,    89,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   963,    -1,    -1,    -1,
     967,    -1,    -1,    -1,   110,    -1,   973,   113,    -1,    -1,
     116,    -1,    -1,    -1,   981,   982,   983,    90,   985,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,  1006,
      -1,   278,    -1,   280,    -1,    -1,    -1,  1014,    -1,  1016,
     287,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,  1026,
     166,  1028,    -1,    -1,    -1,  1032,    -1,  1034,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,    -1,   158,   159,   160,   161,   162,
     327,   164,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
    1067,   757,    -1,    -1,   760,   178,    -1,    -1,    -1,    -1,
    1077,    -1,    -1,  1080,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1100,  1101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1122,  1123,  1124,  1125,  1126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1142,    -1,  1144,    -1,    -1,
      -1,    -1,  1149,    -1,    -1,  1152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1160,   850,  1162,    -1,    -1,  1165,    -1,
     856,   601,   602,   603,   604,   605,   606,   607,   608,    -1,
      -1,    -1,   868,    -1,    -1,    -1,   872,    -1,  1185,  1186,
      -1,  1188,  1189,  1190,  1191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1213,    -1,    -1,    -1,
      -1,  1218,  1219,    -1,  1221,    -1,  1223,    -1,  1225,  1226,
      -1,    -1,  1229,    -1,    -1,    -1,    -1,  1234,    -1,    -1,
      -1,  1238,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1247,    -1,    -1,    -1,    -1,    -1,    -1,   943,    -1,   945,
      -1,    -1,   948,    -1,    -1,  1262,  1263,  1264,  1265,  1266,
    1267,  1268,  1269,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   972,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   984,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1310,  1311,  1312,  1313,  1314,  1315,  1316,
    1317,  1318,  1319,  1320,  1321,    -1,    -1,    -1,    -1,    -1,
      -1,   761,    -1,    -1,   601,    -1,   603,   604,   605,   606,
     607,   608,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    26,    27,    -1,    -1,  1044,    -1,
      -1,    -1,    -1,    35,  1050,    37,    38,   797,    -1,    -1,
      -1,    43,    -1,    -1,    -1,   805,   806,    -1,    50,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    77,    -1,    79,  1094,    -1,
      -1,    83,    84,    85,    86,   845,    -1,    89,    -1,    91,
      -1,    -1,    -1,    -1,    -1,   855,    -1,    -1,    -1,   859,
      -1,   861,    -1,    -1,   864,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1142,    -1,  1144,    -1,
      -1,    -1,    -1,  1149,    -1,    -1,  1152,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1160,    -1,  1162,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,  1194,    19,
      -1,    -1,    -1,    -1,  1200,    -1,    -1,    -1,  1204,  1205,
      -1,    31,    -1,   953,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   806,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     980,  1237,    -1,    -1,    -1,  1241,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1256,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   845,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,    -1,   855,    -1,
      -1,    -1,   859,   103,   861,    -1,    -1,   864,    -1,    -1,
    1030,  1031,    -1,    -1,    -1,    -1,  1036,  1037,    -1,    -1,
    1040,  1041,    -1,    -1,    -1,   125,   126,   127,   128,   129,
     130,   131,    -1,    -1,   134,   135,   136,   137,    -1,   139,
     140,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,   171,    12,    -1,    -1,    -1,    -1,    -1,   178,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1110,    31,    -1,    -1,  1114,  1115,  1116,  1117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1136,  1137,    -1,  1139,
      -1,  1141,    -1,    -1,    -1,    -1,    -1,    -1,  1148,    -1,
      -1,  1151,    -1,    -1,  1154,    -1,  1156,    -1,  1158,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1030,  1031,    -1,    -1,    -1,    -1,  1036,
    1037,    -1,  1202,  1040,  1041,   125,   126,   127,   128,   129,
     130,   131,    -1,    -1,   134,   135,   136,   137,    -1,   139,
     140,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1110,    -1,    -1,    -1,  1114,  1115,  1116,
    1117,  1281,  1282,    -1,    -1,  1285,  1286,  1287,    -1,  1289,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1136,
    1137,    -1,  1139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1151,    -1,    -1,  1154,    -1,  1156,
      -1,  1158,     0,     1,    -1,     3,     4,     5,     6,     7,
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
      12,    -1,    14,    15,    16,    17,    18,    19,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,    -1,   104,   105,    -1,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,    -1,    -1,   120,    -1,
     122,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,    -1,   171,
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
      -1,   173,    -1,   175,    -1,     1,   178,     3,     4,     5,
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
      10,    -1,    12,    -1,    14,    -1,    -1,    17,    18,    19,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    55,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,
     130,   131,    -1,    -1,   134,   135,   136,   137,    -1,   139,
     140,    -1,   142,   143,   144,   145,   146,   147,    -1,   149,
      -1,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,    -1,   173,    -1,    -1,    -1,    -1,   178,     3,
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
     164,   165,    -1,   167,   168,   169,   170,   171,    -1,   173,
      -1,    -1,    -1,    -1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    -1,    17,
      18,    -1,    20,    -1,    22,    23,    -1,    25,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    60,    -1,    62,    63,    64,    65,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,   105,    -1,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,    -1,    -1,    -1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,    -1,   114,    -1,   116,   117,    -1,    -1,    -1,    -1,
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
      -1,    87,    88,    89,    90,    -1,    -1,    -1,    94,    -1,
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
     100,   101,   102,    -1,   104,    -1,    -1,   107,   108,   109,
      -1,   111,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,    -1,    -1,    -1,   178,     3,
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
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,   175,    -1,    -1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    91,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,   116,   117,
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
     102,    -1,   104,    -1,    -1,   107,   108,   109,    -1,   111,
     112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
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
      -1,   107,   108,   109,    -1,   111,   112,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,   174,    -1,
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
     160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,
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
     174,    -1,    -1,    -1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
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
      -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,    -1,
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
      -1,   107,   108,   109,    -1,   111,    -1,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,   174,    -1,
      -1,    -1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    69,
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
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    -1,
      94,    -1,    -1,    97,    98,    99,   100,   101,   102,    -1,
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
      78,    79,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,    -1,
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
      -1,    -1,    -1,   151,     4,     5,     6,     7,     8,     9,
      10,    -1,   160,    -1,    14,   163,   164,   165,    18,    -1,
      -1,   169,    22,   171,    -1,   173,    -1,    27,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    -1,    -1,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    79,
      80,    81,    82,    -1,    -1,    -1,    -1,    87,    -1,    89,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    97,    -1,    99,
      -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,   174,   175,    -1,    -1,   178,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    79,    80,    81,    82,    -1,    -1,
      -1,    -1,    87,    -1,    89,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    97,    -1,    99,    -1,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    12,    -1,   151,    -1,    -1,    17,
      18,    19,    20,    -1,    -1,   160,    -1,    25,   163,   164,
     165,    -1,    -1,    -1,   169,    33,   171,    35,   173,    -1,
     175,    39,    -1,   178,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    -1,   100,    -1,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,   130,   131,    -1,    -1,   134,   135,   136,   137,
      -1,   139,   140,    -1,   142,   143,   144,   145,   146,   147,
      -1,   149,    -1,    -1,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,   164,    -1,    -1,   167,
     168,   169,   170,    -1,    -1,   173,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,    -1,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,     3,
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
     114,    -1,    -1,   117,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,   142,    25,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,   165,    -1,    49,    -1,    -1,    52,   171,    54,   173,
      56,   175,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   102,    -1,   104,    -1,
       3,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,    12,
      -1,   117,    -1,    -1,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      63,    -1,    -1,    -1,    -1,    -1,    -1,   173,    71,   175,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    -1,   100,    -1,   102,
      -1,   104,    -1,    -1,    -1,    -1,    -1,   110,   111,    -1,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,   129,   130,   131,    -1,
      -1,   134,   135,   136,   137,    -1,   139,   140,    -1,   142,
     143,   144,   145,   146,   147,    -1,   149,    -1,    -1,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
       3,   164,    -1,    -1,   167,   168,   169,   170,   171,    12,
      -1,    -1,    -1,    -1,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    97,    98,    -1,   100,    19,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
      31,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,   129,   130,   131,    -1,
      -1,   134,   135,   136,   137,    -1,   139,   140,    -1,   142,
     143,   144,   145,   146,   147,    -1,   149,    -1,    -1,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
      -1,   164,    -1,    -1,   167,   168,   169,   170,    -1,    90,
      -1,    12,    -1,    94,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,    -1,    -1,   134,   135,   136,   137,    -1,   139,   140,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    90,
     171,    12,    -1,    94,    -1,    -1,    -1,   178,    19,    -1,
      -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,    -1,    -1,   134,   135,   136,   137,    -1,   139,   140,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    90,
     171,    12,    -1,    94,    -1,    -1,    -1,   178,    19,    -1,
      -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,    -1,    -1,   134,   135,   136,   137,    -1,   139,   140,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    90,
     171,    12,    -1,    94,    -1,    -1,    -1,   178,    19,    -1,
      -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,    -1,    -1,   134,   135,   136,   137,    -1,   139,   140,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    90,
     171,    12,    -1,    94,    -1,    -1,    -1,   178,    19,    -1,
      -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,    -1,    -1,   134,   135,   136,   137,    -1,   139,   140,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    90,
     171,    12,    -1,    94,    -1,    -1,    -1,   178,    19,    -1,
      -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,    -1,    -1,   134,   135,   136,   137,    -1,   139,   140,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    90,
     171,    -1,    -1,    94,    -1,    -1,    12,   178,    -1,    -1,
      -1,    -1,   103,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,    47,    -1,   134,   135,   136,   137,    -1,   139,   140,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    -1,
     171,    -1,    -1,    -1,    90,    -1,    -1,   178,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    31,    -1,   125,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    73,
      -1,   167,   168,   169,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,   125,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    73,   166,   167,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    12,    -1,    90,    -1,
      -1,    -1,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    47,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    90,   167,   168,   169,    94,    -1,
      -1,    12,    -1,    -1,    -1,    -1,   178,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,   171,    -1,    12,    -1,    90,
      -1,    -1,   178,    94,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    90,   167,   168,   169,    94,
     171,    -1,    12,    -1,    -1,    -1,    -1,   178,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,   121,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,    -1,   171,    -1,    12,    -1,
      90,    -1,    -1,   178,    94,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    90,   167,   168,   169,
      94,   171,    -1,    12,    -1,    -1,    -1,    -1,   178,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    -1,   171,    -1,    12,
      -1,    90,    -1,    -1,   178,    94,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    47,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    31,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    90,   167,   168,
     169,    94,    -1,    -1,    12,    -1,    -1,    -1,    -1,   178,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,   142,
     143,    -1,   145,   146,   147,   103,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,   125,    -1,    -1,
      -1,    -1,    90,    -1,    -1,   178,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   125,    -1,   167,
     168,   169,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    12,    -1,   167,
     168,   169,    -1,     3,    19,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    31,    17,    18,    -1,
      20,    36,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    12,    -1,    39,
      -1,    -1,    -1,    -1,    19,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    31,    32,    73,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    90,    -1,    -1,    78,    94,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    88,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    -1,
     100,    31,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
     125,   111,   112,    -1,   114,    90,    -1,   117,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     125,    -1,   167,   168,   169,    -1,    -1,    -1,    12,    -1,
      90,    -1,    -1,   178,    94,    19,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    31,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,   121,   167,   168,   169,   125,   171,    -1,    -1,    12,
      13,    -1,    -1,   178,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    31,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    90,   167,   168,   169,
      94,    -1,    -1,    -1,    -1,    12,   176,    -1,   178,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,   121,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,   125,   167,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,   176,    90,   178,    -1,    -1,    94,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    12,    -1,   167,   168,   169,    -1,   125,    19,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    12,    -1,    94,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    31,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,   166,   167,   168,   169,
      90,    -1,    -1,    -1,    94,    -1,    -1,    -1,   178,    12,
      13,    -1,    -1,   103,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,   125,   167,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,   178,    -1,
      -1,    94,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    12,    -1,   167,   168,
     169,    -1,   125,    19,    -1,   174,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    12,    -1,   167,   168,   169,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    94,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,   125,    -1,    -1,    -1,    -1,
      90,    -1,   178,    12,    94,    -1,    -1,    -1,    -1,    -1,
      19,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    31,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   125,    -1,   167,   168,   169,
      -1,    -1,    -1,    12,    13,    -1,    -1,    -1,   178,    -1,
      19,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    31,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,    90,    -1,    -1,   174,    94,    -1,    -1,   178,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   125,    -1,   167,   168,
     169,    -1,    -1,    -1,    -1,   174,    -1,    90,    -1,   178,
      -1,    94,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    12,    13,   167,   168,
     169,    -1,   125,    19,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
      12,   174,    -1,    -1,    -1,   178,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    90,   145,
     146,   147,    94,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    12,
      -1,   167,   168,   169,    -1,    -1,    19,    -1,    -1,   121,
      -1,    -1,   178,   125,    -1,    -1,    -1,    -1,    31,    90,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,   125,   167,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    90,   149,   150,
     151,    94,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    12,   166,   167,   168,   169,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      -1,   124,   125,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      12,    -1,    90,    -1,    -1,    -1,    94,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      31,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    90,   167,
     168,   169,    94,    -1,    -1,    -1,    12,    13,    -1,    -1,
     178,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,   125,   167,   168,   169,    -1,    -1,
      -1,    -1,    12,    -1,    90,    -1,   178,    -1,    94,    19,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    31,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,   125,
      -1,    -1,    -1,    12,    -1,   176,    -1,   178,    -1,    -1,
      19,    61,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    31,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      90,   167,   168,   169,    94,    -1,    -1,    -1,    12,    -1,
      -1,    -1,   178,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   103,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,   125,   167,   168,   169,
      -1,    -1,    -1,    -1,    12,    -1,    90,    -1,   178,    -1,
      94,    19,    -1,   142,   143,    -1,   145,   146,   147,   103,
     149,   150,   151,    31,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,   125,    -1,    -1,    -1,    12,    -1,    -1,    -1,   178,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    31,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    90,   167,   168,   169,    94,    -1,    -1,    -1,
      12,    -1,    -1,    -1,   178,   103,    -1,    19,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    31,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,   125,   167,   168,   169,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,   178,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   125,    -1,   167,   168,   169,    90,    -1,    -1,
      -1,    94,    -1,    -1,    -1,   178,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,    -1,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,    -1,   158,   159,   160,   161,   162,
       1,   164,     3,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    17,    18,    -1,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    17,    18,    -1,    20,    -1,    78,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    88,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    97,    98,    -1,   100,
      45,   102,    -1,   104,    49,    -1,   107,    52,    -1,    54,
     111,    -1,    -1,   114,     3,    -1,   117,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    17,    18,
      -1,    20,    -1,    78,    -1,    -1,    25,    -1,    27,    -1,
     141,    -1,    -1,    88,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    97,    98,    -1,   100,    45,   102,    -1,   104,
      49,    -1,   107,    52,    -1,    54,   111,    -1,    -1,   114,
       3,    -1,   117,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    17,    18,    -1,    20,    -1,    78,
      79,    -1,    25,    -1,    -1,    -1,   141,    -1,    -1,    88,
      33,    -1,    91,    -1,    -1,    -1,    39,    -1,    97,    98,
      -1,   100,    45,   102,    -1,   104,    49,    -1,   107,    52,
      -1,    54,   111,    -1,    -1,   114,     3,   116,   117,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      17,    18,    -1,    20,    -1,    78,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    97,    98,    -1,   100,    45,   102,
      -1,   104,    49,    -1,   107,    52,    -1,    54,   111,    -1,
      -1,   114,     3,    -1,   117,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    17,    18,    -1,    20,
      -1,    78,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      97,    98,    -1,   100,    45,   102,    -1,   104,    49,    -1,
     107,    52,    -1,    54,   111,    -1,    -1,   114,    -1,    -1,
     117,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    -1,   100,
      -1,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,   117
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
     382,   383,   384,   385,   166,    22,    35,    44,    54,    56,
      89,   102,   171,   240,   249,   276,   346,   353,   363,   364,
     369,   372,   374,   375,   121,   355,   356,     3,    54,   219,
     369,   355,   110,   326,    91,   219,   187,   340,   369,   173,
       3,    17,    18,    20,    25,    33,    39,    45,    49,    52,
      54,    63,    70,    71,    78,    88,    97,    98,   100,   102,
     104,   107,   111,   114,   117,   220,   221,   223,    11,    27,
     116,   245,   369,    83,    86,   203,   173,   104,   221,   221,
     221,   173,   221,   327,   328,    32,   207,   226,   369,   261,
     262,   369,   369,    18,    78,    97,   114,   369,   369,   369,
       8,   173,   230,   229,    27,    33,    47,    49,    51,    76,
      79,    91,    98,   102,   110,   186,   225,   280,   281,   282,
     305,   306,   307,   332,   337,   369,   340,   108,   109,   166,
     284,   285,   368,   369,   371,   369,   226,   369,   369,   369,
     101,   173,   187,   369,   369,   189,   193,   207,   189,   207,
     369,   371,     3,   369,   369,   369,   369,   369,     1,   172,
     185,   208,   340,   112,   152,   341,   342,   371,   369,    81,
     187,    22,    35,    38,    89,   171,   190,   191,   192,   203,
     207,   195,     8,   150,   197,   171,    46,    85,   115,   204,
      26,   325,   369,     8,   265,   369,   370,    24,    32,    40,
      41,    42,   120,   175,   242,   349,   351,    56,   147,   267,
     221,     1,   194,   221,   271,   274,   173,   300,    53,    74,
      84,   309,    27,    76,    79,    91,   110,   310,    27,    79,
      91,   110,   308,   221,   321,   322,   327,   165,   166,   369,
      12,    19,    31,    90,    94,   125,   142,   143,   145,   146,
     147,   149,   150,   151,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   167,   168,   169,   178,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   170,   296,   173,   175,    90,
      94,   369,   221,    40,    41,   175,    56,   173,   187,   166,
     207,   324,   166,   148,   166,   222,   343,   344,   345,   371,
     171,   221,   221,   103,   207,   103,   126,   207,   296,   204,
     343,   166,   173,   173,   207,   187,   112,   173,   221,   329,
     330,     1,   147,   336,    47,   148,   187,   226,   148,   226,
      13,   173,   173,   226,   343,   231,   231,    47,    91,   306,
     307,   174,   148,   112,   173,   221,   304,   147,   166,   369,
     369,   124,   286,   166,   171,   226,   173,   343,   166,   254,
     254,   166,   172,   172,   185,   148,   172,   369,   148,   174,
     148,   174,     8,    40,    41,   175,   242,     3,   173,   198,
       1,   172,   208,   215,   216,   369,   210,   369,    66,    36,
      73,   166,   265,   267,   110,   237,   289,   369,   187,    79,
     123,   243,   369,   243,   369,   243,   369,   245,   369,   176,
     243,   340,   369,   380,   221,   340,   171,   172,   274,   126,
     148,   172,   174,   301,   303,   305,   310,    91,     1,   147,
     334,   335,    91,     1,     3,    12,    17,    19,    20,    25,
      45,    52,    54,    55,    63,    71,    88,   100,   104,   111,
     117,   142,   143,   144,   145,   146,   147,   149,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   164,   167,
     168,   169,   170,   173,   221,   293,   294,   295,   296,   346,
     126,   323,   148,   166,   166,   369,   369,   369,   243,   369,
     243,   369,   369,   369,   369,   369,   369,   369,     3,    20,
      33,    63,   104,   110,   222,   369,   369,   369,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   369,    69,   371,   371,   371,   371,   371,   343,   343,
     243,   369,   243,   369,   103,   173,   369,   369,   340,   221,
      27,    47,    91,   116,   357,   360,   361,   369,   385,   327,
     369,   126,   174,   148,   126,   126,   187,    34,   187,   369,
      34,   369,    66,   174,   344,   221,   247,   248,   330,   148,
     174,   225,   369,   126,   331,   369,   328,   261,   221,   324,
     369,   369,   174,   304,   310,   282,   330,   147,   338,   339,
     337,   287,   206,     1,   259,   345,   174,    21,   255,   172,
     174,   174,   341,   174,   341,   369,   369,   340,   197,   174,
     199,   200,   345,   172,   172,   148,   166,    13,   150,   148,
     166,    13,    36,    73,   221,   142,   143,   144,   145,   146,
     160,   164,   169,   209,   295,   296,   297,   369,   209,   211,
     267,   171,   238,   324,    47,   355,   120,   221,   173,   226,
      32,    47,   226,   121,   226,   358,    32,    47,   226,   358,
     226,   358,    47,   226,   358,   226,   226,   369,   176,   358,
      47,   148,   176,   358,   124,   148,   176,   166,   173,     1,
     268,   369,   272,   273,   148,   174,   336,     1,   225,   332,
     333,   106,   311,   173,   299,   369,   142,   150,   299,   299,
     369,   322,   173,   175,   166,   166,   166,   166,   166,   166,
     174,   176,   344,    32,    47,    32,    47,    47,   176,   173,
      47,    91,   148,   174,    17,    20,    25,    45,    52,    63,
      71,    88,   100,   111,   117,   346,    90,    90,   166,   371,
     345,   369,   369,    34,   187,    34,    34,   187,   103,   207,
     221,   174,   148,   174,   174,   329,   336,    69,   371,   221,
     174,   174,   339,   147,   283,   174,   337,   152,   298,   331,
     369,   172,    75,   118,   172,   260,   174,   173,   207,   221,
     256,    47,    47,    47,   176,   148,   174,   215,   222,    17,
      18,    20,    25,    45,    49,    52,    71,    78,    88,    97,
      98,   100,   102,   111,   114,   117,   171,   214,   297,   369,
     112,   369,   209,   211,   148,   166,    13,   166,   171,   268,
     321,   327,   243,   369,   226,   369,    47,   340,    44,   369,
     243,   369,   173,   226,    44,   369,   243,   369,   226,   226,
     243,   369,   226,   187,   176,   243,   369,   176,   187,   369,
     176,   369,   176,   369,   344,   172,    83,    86,   172,   186,
     194,   228,   274,   194,   303,   311,    61,   119,   315,   301,
     302,   174,   294,   296,   174,   176,   174,    44,   243,   369,
      44,   243,   369,   243,   369,   344,   360,   336,   346,   346,
     187,   187,   369,   187,    34,   166,   166,   248,   207,   331,
     173,   173,   298,   331,   332,   311,   339,   369,   187,   240,
     340,   256,   147,   207,   243,   369,   243,   369,   243,   369,
     200,   209,    13,    36,    73,    36,    73,   166,   166,   297,
     369,   369,   268,   172,   166,   166,   355,   355,   166,   369,
     174,   369,    32,   226,    32,   226,   359,   360,   369,    32,
     226,   358,    32,   226,   358,   226,   358,   226,   358,   187,
     176,   358,   176,   358,   187,   124,   174,   194,   228,   228,
     274,   315,     3,    54,    93,   104,   316,   317,   318,   369,
     290,   174,   299,   299,   369,    32,    32,   369,    32,    32,
     176,   176,   174,   331,    34,   187,   343,   343,   298,   331,
      32,   226,   174,   369,   176,   176,   172,   112,   369,   209,
     211,   209,   211,    13,   172,   226,   226,   226,   103,    44,
     369,    44,   369,   148,   174,   103,    44,   369,   226,    44,
     369,   226,   226,   226,    44,   187,   369,   176,    44,   187,
     369,   176,   369,   166,   228,   277,   318,   119,   148,   126,
     153,   155,   156,   158,   159,    61,    32,   166,   206,   227,
     312,   313,   103,    44,    44,   103,    44,    44,    44,    44,
     187,   174,   174,   187,   207,   166,   166,   166,   166,   369,
     369,   369,   369,   360,   369,   369,   369,   369,   187,   369,
     187,   313,   369,   317,   318,   318,   318,   318,   318,   318,
     316,   185,   369,   369,   369,   369,   369,   369,   165,   165,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   369,   369,   369,   369,   369,   369
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   182,   183,   184,   184,   185,   185,   186,   186,   187,
     187,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   190,   190,   191,   192,
     192,   193,   194,   194,   195,   195,   195,   196,   197,   197,
     198,   198,   199,   199,   200,   201,   202,   202,   202,   203,
     203,   203,   204,   204,   205,   206,   206,   206,   207,   208,
     208,   209,   209,   209,   209,   209,   209,   210,   210,   210,
     210,   210,   210,   211,   211,   212,   212,   212,   212,   212,
     212,   212,   213,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     215,   215,   215,   215,   215,   216,   216,   217,   218,   218,
     218,   218,   218,   218,   219,   219,   219,   220,   220,   220,
     220,   220,   221,   221,   221,   222,   222,   222,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   225,
     225,   225,   225,   226,   226,   227,   227,   228,   228,   229,
     228,   230,   228,   231,   231,   231,   231,   231,   231,   231,
     232,   232,   232,   232,   233,   234,   234,   235,   236,   236,
     236,   237,   236,   238,   236,   239,   240,   241,   241,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   243,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   245,   245,   246,
     246,   247,   247,   248,   249,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   250,   250,   250,
     250,   250,   250,   251,   251,   251,   252,   252,   252,   253,
     253,   253,   253,   254,   254,   255,   255,   255,   256,   256,
     257,   258,   258,   259,   259,   260,   260,   260,   261,   261,
     261,   262,   262,   263,   264,   264,   265,   266,   266,   266,
     267,   267,   268,   268,   268,   268,   268,   269,   269,   270,
     271,   271,   272,   271,   271,   273,   271,   274,   274,   275,
     277,   276,   278,   279,   279,   279,   280,   280,   281,   281,
     282,   282,   282,   283,   283,   284,   286,   285,   287,   285,
     288,   290,   289,   291,   291,   291,   291,   291,   292,   293,
     293,   294,   294,   294,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   297,   297,   298,   298,   299,   299,   300,   300,   301,
     301,   302,   302,   303,   304,   304,   305,   305,   305,   305,
     305,   305,   306,   306,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   308,   308,   308,   308,   308,   308,   309,
     309,   309,   310,   310,   310,   310,   310,   310,   310,   311,
     311,   312,   312,   313,   313,   313,   314,   315,   315,   315,
     315,   315,   316,   316,   317,   317,   317,   317,   317,   317,
     317,   318,   318,   318,   319,   320,   320,   321,   321,   322,
     323,   323,   324,   324,   324,   324,   324,   326,   325,   325,
     327,   327,   328,   328,   329,   329,   329,   330,   330,   330,
     331,   331,   331,   332,   333,   333,   333,   334,   334,   335,
     335,   336,   336,   336,   336,   337,   337,   338,   339,   339,
     340,   340,   341,   341,   342,   342,   343,   343,   344,   344,
     345,   345,   346,   346,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   348,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   349,   350,
     351,   351,   351,   351,   351,   351,   351,   352,   353,   354,
     354,   354,   354,   354,   354,   354,   355,   355,   356,   357,
     357,   358,   359,   359,   360,   360,   360,   361,   361,   361,
     361,   361,   361,   362,   362,   362,   362,   362,   363,   363,
     363,   363,   363,   364,   365,   365,   365,   365,   365,   365,
     366,   367,   368,   368,   369,   369,   369,   369,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   370,   370,   371,   371,   371,   372,   372,   372,   372,
     373,   373,   373,   373,   373,   374,   374,   374,   375,   375,
     375,   375,   375,   375,   376,   376,   376,   376,   377,   377,
     378,   378,   379,   379,   379,   379,   379,   379,   379,   379,
     379,   379,   379,   379,   379,   380,   380,   381,   381,   381,
     381,   381,   381,   381,   381,   381,   381,   381,   381,   381,
     381,   381,   381,   381,   381,   381,   381,   381,   381,   381,
     382,   382,   382,   382,   382,   382,   382,   383,   383,   383,
     383,   384,   384,   384,   384,   385,   385,   385,   385,   385,
     385,   385
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     1,     3,     3,     3,     3,     3,     2,
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
       1,     1,     1,     2,     1,     2,     3,     1,     1,     0,
       3,     0,     3,     1,     1,     1,     1,     1,     1,     1,
       3,     5,     5,     2,     1,     1,     1,     1,     6,     7,
       3,     0,     6,     0,     6,     2,     0,     3,     2,     5,
       3,     3,     6,     6,     4,     5,     5,     3,     3,     6,
       5,     6,     5,     6,     3,     4,     3,     4,     5,     6,
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
       1,     1,     1,     1,     2,     1,     1,     0,     2,     2,
       4,     4,     1,     3,     3,     3,     3,     3,     3,     3,
       2,     1,     1,     1,     3,     1,     2,     1,     3,     2,
       0,     2,     1,     2,     1,     1,     1,     0,     5,     3,
       1,     3,     3,     5,     1,     1,     3,     1,     2,     3,
       0,     2,     2,     1,     1,     1,     1,     2,     1,     0,
       1,     0,     2,     2,     1,     1,     1,     2,     0,     1,
       1,     3,     1,     1,     3,     3,     0,     1,     1,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       4,     4,     4,     5,     2,     2,     1,     2,     1,     2,
       1,     2,     1,     2,     1,     1,     2,     6,     6,     4,
       9,     9,     7,     6,     6,     4,     9,     9,     7,     2,
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
       4,     4,     5,     4,     5,     3,     5,     3,     3,     3,
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
#line 6551 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 639 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6557 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 640 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6563 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 647 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6571 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 655 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6579 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 659 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6588 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 669 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6596 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 674 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6604 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 681 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6610 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 682 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6616 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 683 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6622 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 684 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6628 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 685 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6634 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6640 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 687 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6646 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 688 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6652 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 689 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6658 "bison-chpl-lib.cpp"
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
#line 6675 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 703 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6681 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 704 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6687 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: TINIT TTHIS TSEMI  */
#line 706 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr));
    auto node = Init::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6699 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 714 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6712 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 723 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6725 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TLABEL ident_def stmt  */
#line 732 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6733 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 736 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6744 "bison-chpl-lib.cpp"
    break;

  case 29: /* stmt_base: error TSEMI  */
#line 743 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6753 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: assignment_stmt  */
#line 750 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6759 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: if_stmt  */
#line 751 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6765 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: loop_stmt  */
#line 752 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6771 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: select_stmt  */
#line 753 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6777 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: manage_stmt  */
#line 754 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6783 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 755 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6789 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: throw_stmt  */
#line 756 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6795 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 758 "chpl.ypp"
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
#line 6813 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 772 "chpl.ypp"
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
#line 6832 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 787 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6844 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 795 "chpl.ypp"
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
#line 6860 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TLOCAL do_stmt  */
#line 807 "chpl.ypp"
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
#line 6875 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TON expr do_stmt  */
#line 818 "chpl.ypp"
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
#line 6891 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 830 "chpl.ypp"
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
#line 6907 "bison-chpl-lib.cpp"
    break;

  case 44: /* tryable_stmt: TSERIAL do_stmt  */
#line 842 "chpl.ypp"
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
#line 6922 "bison-chpl-lib.cpp"
    break;

  case 45: /* tryable_stmt: TSYNC stmt  */
#line 853 "chpl.ypp"
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
#line 6939 "bison-chpl-lib.cpp"
    break;

  case 48: /* collect_attributes: %empty  */
#line 873 "chpl.ypp"
  {
    auto attributeGroup = context->buildAttributeGroup((yyloc));
    if (attributeGroup != nullptr) {
      context->resetAttributeGroupPartsState();
    }
    context->loopAttributes.push_back(std::move(attributeGroup));
  }
#line 6951 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 898 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), true, (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6959 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace  */
#line 902 "chpl.ypp"
 {
  // this allows us to distinguish between @attribute() and @attribute
  // but should not be needed once `@unstable "msg"` is deprecated and gone
  auto lst = new MaybeNamedActualList();
  context->noteAttribute((yylsp[-1]), (yyvsp[-1].expr), false, (yyvsp[0].exprList), lst);
 }
#line 6970 "bison-chpl-lib.cpp"
    break;

  case 56: /* attribute_decl_stmt: attribute_decl_begin STRINGLITERAL  */
#line 909 "chpl.ypp"
 {
  // this allows us to maintain @unstable "message" without requiring parens
  // but should not be needed once `@unstable "msg"` is deprecated and gone
  auto lst = new MaybeNamedActualList();
  auto actual = makeMaybeNamedActual((yyvsp[0].expr));
  lst->push_back(actual);
  context->noteAttribute((yylsp[-1]), (yyvsp[-1].expr), false, context->makeList(), lst);
 }
#line 6983 "bison-chpl-lib.cpp"
    break;

  case 57: /* attribute_decl_begin: TATMARK TIDENT  */
#line 921 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6992 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_decl_toolspace: %empty  */
#line 928 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 7000 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 932 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 7011 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: TLP TRP  */
#line 942 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 7017 "bison-chpl-lib.cpp"
    break;

  case 61: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 944 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 7025 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actual  */
#line 951 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 7034 "bison-chpl-lib.cpp"
    break;

  case 63: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 956 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 7044 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 969 "chpl.ypp"
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
#line 7065 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 989 "chpl.ypp"
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
#line 7084 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 1004 "chpl.ypp"
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
#line 7103 "bison-chpl-lib.cpp"
    break;

  case 68: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1019 "chpl.ypp"
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
#line 7120 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: %empty  */
#line 1034 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 7126 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPUBLIC  */
#line 1035 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 7133 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_access_control: TPRIVATE  */
#line 1037 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 7140 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: %empty  */
#line 1042 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 7146 "bison-chpl-lib.cpp"
    break;

  case 73: /* opt_prototype: TPROTOTYPE  */
#line 1043 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 7153 "bison-chpl-lib.cpp"
    break;

  case 74: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1049 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 7163 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR TRCBR  */
#line 1067 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 7171 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1071 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 7179 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt_body: TLCBR error TRCBR  */
#line 1075 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 7191 "bison-chpl-lib.cpp"
    break;

  case 78: /* block_stmt: block_stmt_body  */
#line 1086 "chpl.ypp"
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
#line 7210 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: toplevel_stmt  */
#line 1104 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 7216 "bison-chpl-lib.cpp"
    break;

  case 80: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1105 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7222 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: expr  */
#line 1110 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 7230 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: all_op_name  */
#line 1114 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7238 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: expr TAS expr  */
#line 1118 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 7247 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA expr  */
#line 1123 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 7255 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1127 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7263 "bison-chpl-lib.cpp"
    break;

  case 86: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1131 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 7272 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr  */
#line 1141 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7281 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: expr TAS expr  */
#line 1146 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7291 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1152 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7302 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1159 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 7311 "bison-chpl-lib.cpp"
    break;

  case 91: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1164 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7322 "bison-chpl-lib.cpp"
    break;

  case 92: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1171 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7334 "bison-chpl-lib.cpp"
    break;

  case 93: /* opt_only_ls: %empty  */
#line 1181 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7340 "bison-chpl-lib.cpp"
    break;

  case 94: /* opt_only_ls: renames_ls  */
#line 1182 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7346 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1187 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7355 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1192 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7367 "bison-chpl-lib.cpp"
    break;

  case 97: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1200 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7380 "bison-chpl-lib.cpp"
    break;

  case 98: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1209 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7394 "bison-chpl-lib.cpp"
    break;

  case 99: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1219 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7406 "bison-chpl-lib.cpp"
    break;

  case 100: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1227 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7419 "bison-chpl-lib.cpp"
    break;

  case 101: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1236 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7433 "bison-chpl-lib.cpp"
    break;

  case 102: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1249 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7442 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_expr: expr  */
#line 1285 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 7450 "bison-chpl-lib.cpp"
    break;

  case 121: /* import_expr: expr TDOT all_op_name  */
#line 1289 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7459 "bison-chpl-lib.cpp"
    break;

  case 122: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1294 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7468 "bison-chpl-lib.cpp"
    break;

  case 123: /* import_expr: expr TAS ident_use  */
#line 1299 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 7478 "bison-chpl-lib.cpp"
    break;

  case 124: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1305 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 7488 "bison-chpl-lib.cpp"
    break;

  case 125: /* import_ls: import_expr  */
#line 1313 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7494 "bison-chpl-lib.cpp"
    break;

  case 126: /* import_ls: import_ls TCOMMA import_expr  */
#line 1314 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7500 "bison-chpl-lib.cpp"
    break;

  case 127: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1319 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7510 "bison-chpl-lib.cpp"
    break;

  case 128: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1328 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7518 "bison-chpl-lib.cpp"
    break;

  case 129: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1332 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7526 "bison-chpl-lib.cpp"
    break;

  case 130: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1336 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7534 "bison-chpl-lib.cpp"
    break;

  case 131: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1340 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7542 "bison-chpl-lib.cpp"
    break;

  case 132: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1344 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7550 "bison-chpl-lib.cpp"
    break;

  case 133: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1348 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7558 "bison-chpl-lib.cpp"
    break;

  case 134: /* opt_label_ident: %empty  */
#line 1356 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7564 "bison-chpl-lib.cpp"
    break;

  case 135: /* opt_label_ident: TIDENT  */
#line 1357 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7570 "bison-chpl-lib.cpp"
    break;

  case 136: /* opt_label_ident: TINIT  */
#line 1358 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7576 "bison-chpl-lib.cpp"
    break;

  case 145: /* ident_use: TIDENT  */
#line 1384 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7582 "bison-chpl-lib.cpp"
    break;

  case 146: /* ident_use: TINIT  */
#line 1385 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7588 "bison-chpl-lib.cpp"
    break;

  case 147: /* ident_use: TTHIS  */
#line 1386 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7594 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TBOOL  */
#line 1413 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7600 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TENUM  */
#line 1414 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7606 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TINT  */
#line 1415 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7612 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TUINT  */
#line 1416 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7618 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TREAL  */
#line 1417 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7624 "bison-chpl-lib.cpp"
    break;

  case 172: /* scalar_type: TIMAG  */
#line 1418 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7630 "bison-chpl-lib.cpp"
    break;

  case 173: /* scalar_type: TCOMPLEX  */
#line 1419 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7636 "bison-chpl-lib.cpp"
    break;

  case 174: /* scalar_type: TBYTES  */
#line 1420 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7642 "bison-chpl-lib.cpp"
    break;

  case 175: /* scalar_type: TSTRING  */
#line 1421 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7648 "bison-chpl-lib.cpp"
    break;

  case 176: /* scalar_type: TLOCALE  */
#line 1422 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7654 "bison-chpl-lib.cpp"
    break;

  case 177: /* scalar_type: TNOTHING  */
#line 1423 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7660 "bison-chpl-lib.cpp"
    break;

  case 178: /* scalar_type: TVOID  */
#line 1424 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7666 "bison-chpl-lib.cpp"
    break;

  case 183: /* do_stmt: TDO stmt  */
#line 1438 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7672 "bison-chpl-lib.cpp"
    break;

  case 184: /* do_stmt: block_stmt  */
#line 1439 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7678 "bison-chpl-lib.cpp"
    break;

  case 185: /* return_stmt: TRETURN TSEMI  */
#line 1444 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7689 "bison-chpl-lib.cpp"
    break;

  case 186: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1451 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7700 "bison-chpl-lib.cpp"
    break;

  case 187: /* class_level_stmt: TSEMI  */
#line 1461 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7708 "bison-chpl-lib.cpp"
    break;

  case 188: /* class_level_stmt: inner_class_level_stmt  */
#line 1465 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7718 "bison-chpl-lib.cpp"
    break;

  case 189: /* $@1: %empty  */
#line 1470 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7725 "bison-chpl-lib.cpp"
    break;

  case 190: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1472 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7734 "bison-chpl-lib.cpp"
    break;

  case 191: /* $@2: %empty  */
#line 1476 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7741 "bison-chpl-lib.cpp"
    break;

  case 192: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1478 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7750 "bison-chpl-lib.cpp"
    break;

  case 200: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1496 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7759 "bison-chpl-lib.cpp"
    break;

  case 201: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1501 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7768 "bison-chpl-lib.cpp"
    break;

  case 202: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1506 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7777 "bison-chpl-lib.cpp"
    break;

  case 203: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1511 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7785 "bison-chpl-lib.cpp"
    break;

  case 204: /* forwarding_decl_start: TFORWARDING  */
#line 1518 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7794 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_or_export: TEXTERN  */
#line 1525 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7800 "bison-chpl-lib.cpp"
    break;

  case 206: /* extern_or_export: TEXPORT  */
#line 1526 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7806 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1531 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7816 "bison-chpl-lib.cpp"
    break;

  case 208: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1541 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7825 "bison-chpl-lib.cpp"
    break;

  case 209: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1547 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7836 "bison-chpl-lib.cpp"
    break;

  case 210: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1554 "chpl.ypp"
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
#line 7858 "bison-chpl-lib.cpp"
    break;

  case 211: /* $@3: %empty  */
#line 1572 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7866 "bison-chpl-lib.cpp"
    break;

  case 212: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1576 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7875 "bison-chpl-lib.cpp"
    break;

  case 213: /* $@4: %empty  */
#line 1581 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7884 "bison-chpl-lib.cpp"
    break;

  case 214: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1586 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7893 "bison-chpl-lib.cpp"
    break;

  case 215: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1594 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7901 "bison-chpl-lib.cpp"
    break;

  case 216: /* no_loop_attributes: %empty  */
#line 1599 "chpl.ypp"
                    { context->loopAttributes.push_back(nullptr); }
#line 7907 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1602 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7913 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1603 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7919 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1608 "chpl.ypp"
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
#line 7942 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: TWHILE expr do_stmt  */
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
#line 7960 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1641 "chpl.ypp"
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
#line 7978 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1655 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7986 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1659 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7994 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1663 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8002 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: TFOR expr TIN expr do_stmt  */
#line 1667 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8010 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: TFOR expr TIN zippered_iterator do_stmt  */
#line 1671 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8018 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: TFOR expr do_stmt  */
#line 1675 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8026 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: TFOR zippered_iterator do_stmt  */
#line 1679 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8034 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1683 "chpl.ypp"
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
#line 8058 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: TFORALL expr TIN expr do_stmt  */
#line 1703 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8066 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1707 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8074 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1711 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8082 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1715 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8090 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TFORALL expr do_stmt  */
#line 1719 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8098 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: TFORALL expr forall_intent_clause do_stmt  */
#line 1723 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8106 "bison-chpl-lib.cpp"
    break;

  case 236: /* loop_stmt_base: TFORALL zippered_iterator do_stmt  */
#line 1727 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8114 "bison-chpl-lib.cpp"
    break;

  case 237: /* loop_stmt_base: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1731 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8122 "bison-chpl-lib.cpp"
    break;

  case 238: /* loop_stmt_base: TFOREACH expr TIN expr do_stmt  */
#line 1735 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8130 "bison-chpl-lib.cpp"
    break;

  case 239: /* loop_stmt_base: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1739 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8138 "bison-chpl-lib.cpp"
    break;

  case 240: /* loop_stmt_base: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1743 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8146 "bison-chpl-lib.cpp"
    break;

  case 241: /* loop_stmt_base: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1747 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8154 "bison-chpl-lib.cpp"
    break;

  case 242: /* loop_stmt_base: TFOREACH expr do_stmt  */
#line 1751 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8162 "bison-chpl-lib.cpp"
    break;

  case 243: /* loop_stmt_base: TFOREACH expr forall_intent_clause do_stmt  */
#line 1755 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8170 "bison-chpl-lib.cpp"
    break;

  case 244: /* loop_stmt_base: TFOREACH zippered_iterator do_stmt  */
#line 1759 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8178 "bison-chpl-lib.cpp"
    break;

  case 245: /* loop_stmt_base: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1763 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8186 "bison-chpl-lib.cpp"
    break;

  case 246: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1767 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8194 "bison-chpl-lib.cpp"
    break;

  case 247: /* loop_stmt_base: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1771 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8202 "bison-chpl-lib.cpp"
    break;

  case 248: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1775 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8210 "bison-chpl-lib.cpp"
    break;

  case 249: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1779 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8218 "bison-chpl-lib.cpp"
    break;

  case 250: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1783 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8226 "bison-chpl-lib.cpp"
    break;

  case 251: /* loop_stmt_base: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1787 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8234 "bison-chpl-lib.cpp"
    break;

  case 252: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1791 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8243 "bison-chpl-lib.cpp"
    break;

  case 253: /* loop_stmt_base: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1796 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8252 "bison-chpl-lib.cpp"
    break;

  case 254: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1804 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 8262 "bison-chpl-lib.cpp"
    break;

  case 255: /* if_stmt: TIF expr TTHEN stmt  */
#line 1813 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8270 "bison-chpl-lib.cpp"
    break;

  case 256: /* if_stmt: TIF expr block_stmt  */
#line 1817 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8278 "bison-chpl-lib.cpp"
    break;

  case 257: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1821 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8286 "bison-chpl-lib.cpp"
    break;

  case 258: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1825 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8294 "bison-chpl-lib.cpp"
    break;

  case 259: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1829 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8302 "bison-chpl-lib.cpp"
    break;

  case 260: /* if_stmt: TIF ifvar block_stmt  */
#line 1833 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8310 "bison-chpl-lib.cpp"
    break;

  case 261: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1837 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8318 "bison-chpl-lib.cpp"
    break;

  case 262: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1841 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8326 "bison-chpl-lib.cpp"
    break;

  case 263: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1845 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8335 "bison-chpl-lib.cpp"
    break;

  case 264: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1850 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8344 "bison-chpl-lib.cpp"
    break;

  case 265: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1855 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8353 "bison-chpl-lib.cpp"
    break;

  case 266: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1860 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8362 "bison-chpl-lib.cpp"
    break;

  case 267: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1868 "chpl.ypp"
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
#line 8381 "bison-chpl-lib.cpp"
    break;

  case 268: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1883 "chpl.ypp"
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
#line 8400 "bison-chpl-lib.cpp"
    break;

  case 269: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1901 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8408 "bison-chpl-lib.cpp"
    break;

  case 270: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1905 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8416 "bison-chpl-lib.cpp"
    break;

  case 271: /* ifc_formal_ls: ifc_formal  */
#line 1911 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8422 "bison-chpl-lib.cpp"
    break;

  case 272: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1912 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8428 "bison-chpl-lib.cpp"
    break;

  case 273: /* ifc_formal: ident_def  */
#line 1917 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8434 "bison-chpl-lib.cpp"
    break;

  case 286: /* implements_type_ident: implements_type_error_ident  */
#line 1935 "chpl.ypp"
  {
    context->syntax((yyloc), "type '%s' cannot implement an interface.", (yyvsp[0].uniqueStr).c_str());
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8443 "bison-chpl-lib.cpp"
    break;

  case 293: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1961 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8451 "bison-chpl-lib.cpp"
    break;

  case 294: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1965 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 8459 "bison-chpl-lib.cpp"
    break;

  case 295: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1969 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8467 "bison-chpl-lib.cpp"
    break;

  case 296: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1976 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8475 "bison-chpl-lib.cpp"
    break;

  case 297: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1980 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 8483 "bison-chpl-lib.cpp"
    break;

  case 298: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1984 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8491 "bison-chpl-lib.cpp"
    break;

  case 299: /* try_stmt: TTRY tryable_stmt  */
#line 1991 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8499 "bison-chpl-lib.cpp"
    break;

  case 300: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1995 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8507 "bison-chpl-lib.cpp"
    break;

  case 301: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1999 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8515 "bison-chpl-lib.cpp"
    break;

  case 302: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 2003 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8523 "bison-chpl-lib.cpp"
    break;

  case 303: /* catch_expr_ls: %empty  */
#line 2009 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8529 "bison-chpl-lib.cpp"
    break;

  case 304: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2010 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8535 "bison-chpl-lib.cpp"
    break;

  case 305: /* catch_expr: TCATCH block_stmt  */
#line 2015 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8543 "bison-chpl-lib.cpp"
    break;

  case 306: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2019 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8551 "bison-chpl-lib.cpp"
    break;

  case 307: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2023 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8559 "bison-chpl-lib.cpp"
    break;

  case 308: /* catch_expr_inner: ident_def  */
#line 2030 "chpl.ypp"
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
#line 8576 "bison-chpl-lib.cpp"
    break;

  case 309: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2043 "chpl.ypp"
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
#line 8593 "bison-chpl-lib.cpp"
    break;

  case 310: /* throw_stmt: TTHROW expr TSEMI  */
#line 2059 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8603 "bison-chpl-lib.cpp"
    break;

  case 311: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2067 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8611 "bison-chpl-lib.cpp"
    break;

  case 312: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2071 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8621 "bison-chpl-lib.cpp"
    break;

  case 313: /* when_stmt_ls: %empty  */
#line 2079 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8627 "bison-chpl-lib.cpp"
    break;

  case 314: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2080 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8633 "bison-chpl-lib.cpp"
    break;

  case 315: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2085 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8641 "bison-chpl-lib.cpp"
    break;

  case 316: /* when_stmt: TOTHERWISE stmt  */
#line 2089 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8650 "bison-chpl-lib.cpp"
    break;

  case 317: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2094 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 8660 "bison-chpl-lib.cpp"
    break;

  case 318: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2103 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8669 "bison-chpl-lib.cpp"
    break;

  case 319: /* manager_expr: expr TAS ident_def  */
#line 2108 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8677 "bison-chpl-lib.cpp"
    break;

  case 320: /* manager_expr: expr  */
#line 2112 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8685 "bison-chpl-lib.cpp"
    break;

  case 321: /* manager_expr_ls: manager_expr  */
#line 2118 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8691 "bison-chpl-lib.cpp"
    break;

  case 322: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2119 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8697 "bison-chpl-lib.cpp"
    break;

  case 323: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2124 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8705 "bison-chpl-lib.cpp"
    break;

  case 324: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2133 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8714 "bison-chpl-lib.cpp"
    break;

  case 325: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2138 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8725 "bison-chpl-lib.cpp"
    break;

  case 326: /* class_start: class_tag ident_def  */
#line 2149 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8733 "bison-chpl-lib.cpp"
    break;

  case 327: /* class_tag: TCLASS  */
#line 2155 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8739 "bison-chpl-lib.cpp"
    break;

  case 328: /* class_tag: TRECORD  */
#line 2156 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8745 "bison-chpl-lib.cpp"
    break;

  case 329: /* class_tag: TUNION  */
#line 2157 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8751 "bison-chpl-lib.cpp"
    break;

  case 330: /* opt_inherit: %empty  */
#line 2161 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8757 "bison-chpl-lib.cpp"
    break;

  case 331: /* opt_inherit: TCOLON expr_ls  */
#line 2162 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8763 "bison-chpl-lib.cpp"
    break;

  case 332: /* class_level_stmt_ls: %empty  */
#line 2166 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8772 "bison-chpl-lib.cpp"
    break;

  case 333: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2171 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8780 "bison-chpl-lib.cpp"
    break;

  case 334: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2175 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8788 "bison-chpl-lib.cpp"
    break;

  case 335: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2179 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8796 "bison-chpl-lib.cpp"
    break;

  case 336: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2183 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8804 "bison-chpl-lib.cpp"
    break;

  case 337: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2190 "chpl.ypp"
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
#line 8825 "bison-chpl-lib.cpp"
    break;

  case 338: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2207 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8839 "bison-chpl-lib.cpp"
    break;

  case 339: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2220 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8847 "bison-chpl-lib.cpp"
    break;

  case 340: /* enum_ls: enum_item  */
#line 2227 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8856 "bison-chpl-lib.cpp"
    break;

  case 341: /* enum_ls: enum_ls TCOMMA  */
#line 2232 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8866 "bison-chpl-lib.cpp"
    break;

  case 342: /* $@5: %empty  */
#line 2238 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8875 "bison-chpl-lib.cpp"
    break;

  case 343: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2243 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8884 "bison-chpl-lib.cpp"
    break;

  case 344: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2248 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8893 "bison-chpl-lib.cpp"
    break;

  case 345: /* $@6: %empty  */
#line 2253 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8902 "bison-chpl-lib.cpp"
    break;

  case 346: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2258 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8911 "bison-chpl-lib.cpp"
    break;

  case 347: /* enum_item: ident_def  */
#line 2268 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8922 "bison-chpl-lib.cpp"
    break;

  case 348: /* enum_item: ident_def TASSIGN expr  */
#line 2275 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8935 "bison-chpl-lib.cpp"
    break;

  case 349: /* lambda_decl_start: TLAMBDA  */
#line 2287 "chpl.ypp"
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
#line 8951 "bison-chpl-lib.cpp"
    break;

  case 350: /* $@7: %empty  */
#line 2304 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8961 "bison-chpl-lib.cpp"
    break;

  case 351: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2310 "chpl.ypp"
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
#line 8983 "bison-chpl-lib.cpp"
    break;

  case 353: /* linkage_spec: linkage_spec_empty  */
#line 2333 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8989 "bison-chpl-lib.cpp"
    break;

  case 354: /* linkage_spec: TINLINE  */
#line 2334 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8996 "bison-chpl-lib.cpp"
    break;

  case 355: /* linkage_spec: TOVERRIDE  */
#line 2336 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 9003 "bison-chpl-lib.cpp"
    break;

  case 356: /* opt_fn_type_formal_ls: %empty  */
#line 2341 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 9009 "bison-chpl-lib.cpp"
    break;

  case 357: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2342 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9015 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_type_formal_ls: fn_type_formal  */
#line 2346 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9021 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2347 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9027 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_type_formal: named_formal  */
#line 2352 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9033 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2355 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 9039 "bison-chpl-lib.cpp"
    break;

  case 362: /* fn_type_formal: formal_type  */
#line 2357 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 9045 "bison-chpl-lib.cpp"
    break;

  case 363: /* opt_fn_type_ret_type: %empty  */
#line 2361 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 9051 "bison-chpl-lib.cpp"
    break;

  case 364: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2362 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9057 "bison-chpl-lib.cpp"
    break;

  case 365: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2375 "chpl.ypp"
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
#line 9076 "bison-chpl-lib.cpp"
    break;

  case 366: /* $@8: %empty  */
#line 2393 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 9086 "bison-chpl-lib.cpp"
    break;

  case 367: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2399 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 9099 "bison-chpl-lib.cpp"
    break;

  case 368: /* $@9: %empty  */
#line 2408 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 9109 "bison-chpl-lib.cpp"
    break;

  case 369: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2414 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 9122 "bison-chpl-lib.cpp"
    break;

  case 370: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2426 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 9130 "bison-chpl-lib.cpp"
    break;

  case 371: /* $@10: %empty  */
#line 2435 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();

    // May never have been built if there was a syntax error.
    if ((yyvsp[-4].functionParts).errorExpr == nullptr) {
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name->name());
    }
  }
#line 9144 "bison-chpl-lib.cpp"
    break;

  case 372: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2445 "chpl.ypp"
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
#line 9167 "bison-chpl-lib.cpp"
    break;

  case 373: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2467 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9179 "bison-chpl-lib.cpp"
    break;

  case 374: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2475 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9191 "bison-chpl-lib.cpp"
    break;

  case 375: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
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
#line 9206 "bison-chpl-lib.cpp"
    break;

  case 376: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2494 "chpl.ypp"
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
#line 9221 "bison-chpl-lib.cpp"
    break;

  case 377: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2505 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 9231 "bison-chpl-lib.cpp"
    break;

  case 378: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2514 "chpl.ypp"
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
#line 9247 "bison-chpl-lib.cpp"
    break;

  case 380: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2529 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 9253 "bison-chpl-lib.cpp"
    break;

  case 383: /* fn_ident: ident_def TBANG  */
#line 2536 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 9263 "bison-chpl-lib.cpp"
    break;

  case 423: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2593 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 9269 "bison-chpl-lib.cpp"
    break;

  case 424: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2594 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9275 "bison-chpl-lib.cpp"
    break;

  case 425: /* opt_formal_ls: %empty  */
#line 2598 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 9281 "bison-chpl-lib.cpp"
    break;

  case 426: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2599 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9287 "bison-chpl-lib.cpp"
    break;

  case 427: /* req_formal_ls: TLP TRP  */
#line 2603 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 9293 "bison-chpl-lib.cpp"
    break;

  case 428: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2604 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9299 "bison-chpl-lib.cpp"
    break;

  case 429: /* formal_ls_inner: formal  */
#line 2608 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9305 "bison-chpl-lib.cpp"
    break;

  case 430: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2609 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9311 "bison-chpl-lib.cpp"
    break;

  case 431: /* formal_ls: %empty  */
#line 2613 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 9317 "bison-chpl-lib.cpp"
    break;

  case 432: /* formal_ls: formal_ls_inner  */
#line 2614 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9323 "bison-chpl-lib.cpp"
    break;

  case 436: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2628 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9331 "bison-chpl-lib.cpp"
    break;

  case 437: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2633 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 9339 "bison-chpl-lib.cpp"
    break;

  case 438: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2638 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9347 "bison-chpl-lib.cpp"
    break;

  case 439: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2643 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 9355 "bison-chpl-lib.cpp"
    break;

  case 440: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2648 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9363 "bison-chpl-lib.cpp"
    break;

  case 441: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2653 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 9371 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_formal_intent_tag: %empty  */
#line 2659 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 9380 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_formal_intent_tag: required_intent_tag  */
#line 2664 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 9389 "bison-chpl-lib.cpp"
    break;

  case 444: /* required_intent_tag: TIN  */
#line 2671 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 9395 "bison-chpl-lib.cpp"
    break;

  case 445: /* required_intent_tag: TINOUT  */
#line 2672 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 9401 "bison-chpl-lib.cpp"
    break;

  case 446: /* required_intent_tag: TOUT  */
#line 2673 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 9407 "bison-chpl-lib.cpp"
    break;

  case 447: /* required_intent_tag: TCONST TIN  */
#line 2674 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 9413 "bison-chpl-lib.cpp"
    break;

  case 448: /* required_intent_tag: TCONST TREF  */
#line 2675 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9419 "bison-chpl-lib.cpp"
    break;

  case 449: /* required_intent_tag: TCONST  */
#line 2676 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9425 "bison-chpl-lib.cpp"
    break;

  case 450: /* required_intent_tag: TPARAM  */
#line 2677 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9431 "bison-chpl-lib.cpp"
    break;

  case 451: /* required_intent_tag: TREF  */
#line 2678 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9437 "bison-chpl-lib.cpp"
    break;

  case 452: /* required_intent_tag: TTYPE  */
#line 2679 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9443 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_this_intent_tag: %empty  */
#line 2683 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9449 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_this_intent_tag: TPARAM  */
#line 2684 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9455 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_this_intent_tag: TREF  */
#line 2685 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9461 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_this_intent_tag: TCONST TREF  */
#line 2686 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9467 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_this_intent_tag: TCONST  */
#line 2687 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9473 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_this_intent_tag: TTYPE  */
#line 2688 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9479 "bison-chpl-lib.cpp"
    break;

  case 459: /* proc_iter_or_op: TPROC  */
#line 2692 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9485 "bison-chpl-lib.cpp"
    break;

  case 460: /* proc_iter_or_op: TITER  */
#line 2693 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9491 "bison-chpl-lib.cpp"
    break;

  case 461: /* proc_iter_or_op: TOPERATOR  */
#line 2694 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9497 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_ret_tag: %empty  */
#line 2698 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9503 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_ret_tag: TOUT  */
#line 2699 "chpl.ypp"
                    { (yyval.returnTag) = Function::OUT; }
#line 9509 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_ret_tag: TCONST  */
#line 2700 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 9515 "bison-chpl-lib.cpp"
    break;

  case 465: /* opt_ret_tag: TCONST TREF  */
#line 2701 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 9521 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_ret_tag: TREF  */
#line 2702 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 9527 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_ret_tag: TPARAM  */
#line 2703 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 9533 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_ret_tag: TTYPE  */
#line 2704 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 9539 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_throws_error: %empty  */
#line 2708 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9545 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_throws_error: TTHROWS  */
#line 2709 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9551 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_function_body_stmt: TSEMI  */
#line 2712 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9557 "bison-chpl-lib.cpp"
    break;

  case 472: /* opt_function_body_stmt: function_body_stmt  */
#line 2713 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9563 "bison-chpl-lib.cpp"
    break;

  case 473: /* function_body_stmt: block_stmt_body  */
#line 2717 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9569 "bison-chpl-lib.cpp"
    break;

  case 474: /* function_body_stmt: TDO toplevel_stmt  */
#line 2718 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 9575 "bison-chpl-lib.cpp"
    break;

  case 475: /* function_body_stmt: return_stmt  */
#line 2719 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt), &((yylsp[0]))); }
#line 9581 "bison-chpl-lib.cpp"
    break;

  case 476: /* query_expr: TQUERIEDIDENT  */
#line 2723 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9587 "bison-chpl-lib.cpp"
    break;

  case 477: /* opt_lifetime_where: %empty  */
#line 2728 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9593 "bison-chpl-lib.cpp"
    break;

  case 478: /* opt_lifetime_where: TWHERE expr  */
#line 2730 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9599 "bison-chpl-lib.cpp"
    break;

  case 479: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2732 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9605 "bison-chpl-lib.cpp"
    break;

  case 480: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2734 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9611 "bison-chpl-lib.cpp"
    break;

  case 481: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2736 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9617 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_components_expr: lifetime_expr  */
#line 2741 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9623 "bison-chpl-lib.cpp"
    break;

  case 483: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2743 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9629 "bison-chpl-lib.cpp"
    break;

  case 484: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2748 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9635 "bison-chpl-lib.cpp"
    break;

  case 485: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2750 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9641 "bison-chpl-lib.cpp"
    break;

  case 486: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2752 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9647 "bison-chpl-lib.cpp"
    break;

  case 487: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2754 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9653 "bison-chpl-lib.cpp"
    break;

  case 488: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2756 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9659 "bison-chpl-lib.cpp"
    break;

  case 489: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2758 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9665 "bison-chpl-lib.cpp"
    break;

  case 490: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2760 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9671 "bison-chpl-lib.cpp"
    break;

  case 491: /* lifetime_ident: TIDENT  */
#line 2764 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9677 "bison-chpl-lib.cpp"
    break;

  case 492: /* lifetime_ident: TINIT  */
#line 2765 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9683 "bison-chpl-lib.cpp"
    break;

  case 493: /* lifetime_ident: TTHIS  */
#line 2766 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9689 "bison-chpl-lib.cpp"
    break;

  case 494: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2771 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9698 "bison-chpl-lib.cpp"
    break;

  case 495: /* type_alias_decl_stmt_start: TTYPE  */
#line 2780 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9706 "bison-chpl-lib.cpp"
    break;

  case 496: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2784 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9715 "bison-chpl-lib.cpp"
    break;

  case 497: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2792 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9723 "bison-chpl-lib.cpp"
    break;

  case 498: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2796 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9731 "bison-chpl-lib.cpp"
    break;

  case 499: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2803 "chpl.ypp"
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
#line 9753 "bison-chpl-lib.cpp"
    break;

  case 500: /* opt_init_type: %empty  */
#line 2823 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9759 "bison-chpl-lib.cpp"
    break;

  case 501: /* opt_init_type: TASSIGN expr  */
#line 2825 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9765 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_type: TPARAM  */
#line 2829 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9771 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_type: TCONST TREF  */
#line 2830 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9777 "bison-chpl-lib.cpp"
    break;

  case 504: /* var_decl_type: TREF  */
#line 2831 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9783 "bison-chpl-lib.cpp"
    break;

  case 505: /* var_decl_type: TCONST  */
#line 2832 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9789 "bison-chpl-lib.cpp"
    break;

  case 506: /* var_decl_type: TVAR  */
#line 2833 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9795 "bison-chpl-lib.cpp"
    break;

  case 507: /* $@11: %empty  */
#line 2838 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9804 "bison-chpl-lib.cpp"
    break;

  case 508: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2842 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9813 "bison-chpl-lib.cpp"
    break;

  case 509: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2847 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9822 "bison-chpl-lib.cpp"
    break;

  case 510: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2855 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9830 "bison-chpl-lib.cpp"
    break;

  case 511: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2859 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9838 "bison-chpl-lib.cpp"
    break;

  case 512: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2866 "chpl.ypp"
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
#line 9860 "bison-chpl-lib.cpp"
    break;

  case 513: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2884 "chpl.ypp"
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
#line 9880 "bison-chpl-lib.cpp"
    break;

  case 514: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2903 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9888 "bison-chpl-lib.cpp"
    break;

  case 515: /* tuple_var_decl_component: ident_def  */
#line 2907 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9896 "bison-chpl-lib.cpp"
    break;

  case 516: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2911 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9904 "bison-chpl-lib.cpp"
    break;

  case 517: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2918 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9910 "bison-chpl-lib.cpp"
    break;

  case 518: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2920 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9916 "bison-chpl-lib.cpp"
    break;

  case 519: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2922 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9922 "bison-chpl-lib.cpp"
    break;

  case 520: /* opt_init_expr: %empty  */
#line 2928 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9928 "bison-chpl-lib.cpp"
    break;

  case 521: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2929 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9934 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2930 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9940 "bison-chpl-lib.cpp"
    break;

  case 523: /* formal_or_ret_type_expr: expr  */
#line 2934 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9946 "bison-chpl-lib.cpp"
    break;

  case 524: /* ret_type: formal_or_ret_type_expr  */
#line 2938 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9952 "bison-chpl-lib.cpp"
    break;

  case 525: /* ret_type: reserved_type_ident_use  */
#line 2939 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9958 "bison-chpl-lib.cpp"
    break;

  case 526: /* ret_type: error  */
#line 2940 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9964 "bison-chpl-lib.cpp"
    break;

  case 527: /* colon_ret_type: TCOLON ret_type  */
#line 2944 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9970 "bison-chpl-lib.cpp"
    break;

  case 528: /* colon_ret_type: error  */
#line 2945 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9976 "bison-chpl-lib.cpp"
    break;

  case 529: /* opt_ret_type: %empty  */
#line 2949 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9982 "bison-chpl-lib.cpp"
    break;

  case 531: /* opt_type: %empty  */
#line 2954 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9988 "bison-chpl-lib.cpp"
    break;

  case 532: /* opt_type: TCOLON expr  */
#line 2955 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9994 "bison-chpl-lib.cpp"
    break;

  case 533: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2956 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10000 "bison-chpl-lib.cpp"
    break;

  case 534: /* opt_type: error  */
#line 2957 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 10006 "bison-chpl-lib.cpp"
    break;

  case 535: /* formal_type: formal_or_ret_type_expr  */
#line 2961 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 10012 "bison-chpl-lib.cpp"
    break;

  case 536: /* formal_type: reserved_type_ident_use  */
#line 2962 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10018 "bison-chpl-lib.cpp"
    break;

  case 537: /* colon_formal_type: TCOLON formal_type  */
#line 2966 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10024 "bison-chpl-lib.cpp"
    break;

  case 538: /* opt_colon_formal_type: %empty  */
#line 2970 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 10030 "bison-chpl-lib.cpp"
    break;

  case 539: /* opt_colon_formal_type: colon_formal_type  */
#line 2971 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 10036 "bison-chpl-lib.cpp"
    break;

  case 540: /* expr_ls: expr  */
#line 2977 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10042 "bison-chpl-lib.cpp"
    break;

  case 541: /* expr_ls: expr_ls TCOMMA expr  */
#line 2978 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10048 "bison-chpl-lib.cpp"
    break;

  case 542: /* tuple_component: TUNDERSCORE  */
#line 2982 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10054 "bison-chpl-lib.cpp"
    break;

  case 543: /* tuple_component: opt_try_expr  */
#line 2983 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 10060 "bison-chpl-lib.cpp"
    break;

  case 544: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2988 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 10068 "bison-chpl-lib.cpp"
    break;

  case 545: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2992 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10076 "bison-chpl-lib.cpp"
    break;

  case 546: /* opt_actual_ls: %empty  */
#line 2998 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 10082 "bison-chpl-lib.cpp"
    break;

  case 547: /* opt_actual_ls: actual_ls  */
#line 2999 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 10088 "bison-chpl-lib.cpp"
    break;

  case 548: /* actual_ls: actual_expr  */
#line 3004 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10097 "bison-chpl-lib.cpp"
    break;

  case 549: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3009 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10107 "bison-chpl-lib.cpp"
    break;

  case 550: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3017 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10113 "bison-chpl-lib.cpp"
    break;

  case 551: /* actual_expr: opt_try_expr  */
#line 3018 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10119 "bison-chpl-lib.cpp"
    break;

  case 552: /* ident_expr: ident_use  */
#line 3022 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10125 "bison-chpl-lib.cpp"
    break;

  case 553: /* ident_expr: scalar_type  */
#line 3023 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10131 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TSINGLE expr  */
#line 3036 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10137 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3038 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10143 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3040 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10149 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3042 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10155 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 3044 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 10165 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TATOMIC expr  */
#line 3050 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10171 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TSYNC expr  */
#line 3052 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10177 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TOWNED  */
#line 3055 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10183 "bison-chpl-lib.cpp"
    break;

  case 567: /* sub_type_level_expr: TOWNED expr  */
#line 3057 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10189 "bison-chpl-lib.cpp"
    break;

  case 568: /* sub_type_level_expr: TUNMANAGED  */
#line 3059 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10195 "bison-chpl-lib.cpp"
    break;

  case 569: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3061 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10201 "bison-chpl-lib.cpp"
    break;

  case 570: /* sub_type_level_expr: TSHARED  */
#line 3063 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10207 "bison-chpl-lib.cpp"
    break;

  case 571: /* sub_type_level_expr: TSHARED expr  */
#line 3065 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10213 "bison-chpl-lib.cpp"
    break;

  case 572: /* sub_type_level_expr: TBORROWED  */
#line 3067 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10219 "bison-chpl-lib.cpp"
    break;

  case 573: /* sub_type_level_expr: TBORROWED expr  */
#line 3069 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10225 "bison-chpl-lib.cpp"
    break;

  case 574: /* sub_type_level_expr: TCLASS  */
#line 3071 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10231 "bison-chpl-lib.cpp"
    break;

  case 575: /* sub_type_level_expr: TRECORD  */
#line 3073 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10237 "bison-chpl-lib.cpp"
    break;

  case 576: /* for_expr: no_loop_attributes for_expr_base  */
#line 3077 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 10243 "bison-chpl-lib.cpp"
    break;

  case 577: /* for_expr_base: TFOR expr TIN expr TDO expr  */
#line 3082 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10257 "bison-chpl-lib.cpp"
    break;

  case 578: /* for_expr_base: TFOR expr TIN zippered_iterator TDO expr  */
#line 3092 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10271 "bison-chpl-lib.cpp"
    break;

  case 579: /* for_expr_base: TFOR expr TDO expr  */
#line 3102 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10284 "bison-chpl-lib.cpp"
    break;

  case 580: /* for_expr_base: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 3111 "chpl.ypp"
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
#line 10303 "bison-chpl-lib.cpp"
    break;

  case 581: /* for_expr_base: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3126 "chpl.ypp"
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
#line 10322 "bison-chpl-lib.cpp"
    break;

  case 582: /* for_expr_base: TFOR expr TDO TIF expr TTHEN expr  */
#line 3141 "chpl.ypp"
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
#line 10341 "bison-chpl-lib.cpp"
    break;

  case 583: /* for_expr_base: TFORALL expr TIN expr TDO expr  */
#line 3156 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10355 "bison-chpl-lib.cpp"
    break;

  case 584: /* for_expr_base: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3166 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10369 "bison-chpl-lib.cpp"
    break;

  case 585: /* for_expr_base: TFORALL expr TDO expr  */
#line 3176 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10382 "bison-chpl-lib.cpp"
    break;

  case 586: /* for_expr_base: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3185 "chpl.ypp"
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
#line 10401 "bison-chpl-lib.cpp"
    break;

  case 587: /* for_expr_base: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3200 "chpl.ypp"
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
#line 10420 "bison-chpl-lib.cpp"
    break;

  case 588: /* for_expr_base: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3215 "chpl.ypp"
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
#line 10439 "bison-chpl-lib.cpp"
    break;

  case 589: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3232 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 10445 "bison-chpl-lib.cpp"
    break;

  case 590: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3237 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 10453 "bison-chpl-lib.cpp"
    break;

  case 591: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3241 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 10461 "bison-chpl-lib.cpp"
    break;

  case 592: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3245 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10469 "bison-chpl-lib.cpp"
    break;

  case 593: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3249 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10477 "bison-chpl-lib.cpp"
    break;

  case 594: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3253 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10485 "bison-chpl-lib.cpp"
    break;

  case 595: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3257 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10493 "bison-chpl-lib.cpp"
    break;

  case 596: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3261 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10501 "bison-chpl-lib.cpp"
    break;

  case 597: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3268 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10515 "bison-chpl-lib.cpp"
    break;

  case 598: /* nil_expr: TNIL  */
#line 3285 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10521 "bison-chpl-lib.cpp"
    break;

  case 606: /* opt_task_intent_ls: %empty  */
#line 3303 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10527 "bison-chpl-lib.cpp"
    break;

  case 607: /* opt_task_intent_ls: task_intent_clause  */
#line 3304 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10533 "bison-chpl-lib.cpp"
    break;

  case 608: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3309 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10543 "bison-chpl-lib.cpp"
    break;

  case 609: /* task_intent_ls: intent_expr  */
#line 3317 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10549 "bison-chpl-lib.cpp"
    break;

  case 610: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3318 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10555 "bison-chpl-lib.cpp"
    break;

  case 611: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3323 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10565 "bison-chpl-lib.cpp"
    break;

  case 612: /* forall_intent_ls: intent_expr  */
#line 3331 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10571 "bison-chpl-lib.cpp"
    break;

  case 613: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3332 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10577 "bison-chpl-lib.cpp"
    break;

  case 614: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3337 "chpl.ypp"
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
#line 10595 "bison-chpl-lib.cpp"
    break;

  case 615: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3351 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10603 "bison-chpl-lib.cpp"
    break;

  case 616: /* intent_expr: expr TREDUCE ident_expr  */
#line 3355 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10611 "bison-chpl-lib.cpp"
    break;

  case 617: /* task_var_prefix: TCONST  */
#line 3361 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10617 "bison-chpl-lib.cpp"
    break;

  case 618: /* task_var_prefix: TIN  */
#line 3362 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10623 "bison-chpl-lib.cpp"
    break;

  case 619: /* task_var_prefix: TCONST TIN  */
#line 3363 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10629 "bison-chpl-lib.cpp"
    break;

  case 620: /* task_var_prefix: TREF  */
#line 3364 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10635 "bison-chpl-lib.cpp"
    break;

  case 621: /* task_var_prefix: TCONST TREF  */
#line 3365 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10641 "bison-chpl-lib.cpp"
    break;

  case 622: /* task_var_prefix: TVAR  */
#line 3366 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10647 "bison-chpl-lib.cpp"
    break;

  case 623: /* new_maybe_decorated: TNEW  */
#line 3371 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10653 "bison-chpl-lib.cpp"
    break;

  case 624: /* new_maybe_decorated: TNEW TOWNED  */
#line 3373 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10659 "bison-chpl-lib.cpp"
    break;

  case 625: /* new_maybe_decorated: TNEW TSHARED  */
#line 3375 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10665 "bison-chpl-lib.cpp"
    break;

  case 626: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3377 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10671 "bison-chpl-lib.cpp"
    break;

  case 627: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3379 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10677 "bison-chpl-lib.cpp"
    break;

  case 628: /* new_expr: new_maybe_decorated expr  */
#line 3385 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10685 "bison-chpl-lib.cpp"
    break;

  case 629: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
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
    (yyval.expr) = context->buildNewExpr((yyloc), New::OWNED, call.release());
  }
#line 10701 "bison-chpl-lib.cpp"
    break;

  case 630: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3404 "chpl.ypp"
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
#line 10717 "bison-chpl-lib.cpp"
    break;

  case 631: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3416 "chpl.ypp"
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
#line 10734 "bison-chpl-lib.cpp"
    break;

  case 632: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3429 "chpl.ypp"
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
#line 10752 "bison-chpl-lib.cpp"
    break;

  case 633: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3446 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10760 "bison-chpl-lib.cpp"
    break;

  case 634: /* range_literal_expr: expr TDOTDOT expr  */
#line 3453 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10769 "bison-chpl-lib.cpp"
    break;

  case 635: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3458 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10778 "bison-chpl-lib.cpp"
    break;

  case 636: /* range_literal_expr: expr TDOTDOT  */
#line 3463 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10787 "bison-chpl-lib.cpp"
    break;

  case 637: /* range_literal_expr: TDOTDOT expr  */
#line 3468 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10797 "bison-chpl-lib.cpp"
    break;

  case 638: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3474 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10807 "bison-chpl-lib.cpp"
    break;

  case 639: /* range_literal_expr: TDOTDOT  */
#line 3480 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10817 "bison-chpl-lib.cpp"
    break;

  case 640: /* cast_expr: expr TCOLON expr  */
#line 3510 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10825 "bison-chpl-lib.cpp"
    break;

  case 641: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3517 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10833 "bison-chpl-lib.cpp"
    break;

  case 642: /* super_expr: fn_expr  */
#line 3523 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10839 "bison-chpl-lib.cpp"
    break;

  case 646: /* expr: sub_type_level_expr TQUESTION  */
#line 3532 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10845 "bison-chpl-lib.cpp"
    break;

  case 647: /* expr: TQUESTION  */
#line 3534 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10851 "bison-chpl-lib.cpp"
    break;

  case 651: /* expr: fn_type  */
#line 3539 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10857 "bison-chpl-lib.cpp"
    break;

  case 661: /* opt_expr: %empty  */
#line 3553 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10863 "bison-chpl-lib.cpp"
    break;

  case 662: /* opt_expr: expr  */
#line 3554 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10869 "bison-chpl-lib.cpp"
    break;

  case 663: /* opt_try_expr: TTRY expr  */
#line 3558 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10875 "bison-chpl-lib.cpp"
    break;

  case 664: /* opt_try_expr: TTRYBANG expr  */
#line 3559 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10881 "bison-chpl-lib.cpp"
    break;

  case 665: /* opt_try_expr: super_expr  */
#line 3560 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10887 "bison-chpl-lib.cpp"
    break;

  case 671: /* call_base_expr: expr TBANG  */
#line 3577 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10895 "bison-chpl-lib.cpp"
    break;

  case 672: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3580 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10901 "bison-chpl-lib.cpp"
    break;

  case 675: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3587 "chpl.ypp"
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
#line 10917 "bison-chpl-lib.cpp"
    break;

  case 676: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3599 "chpl.ypp"
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
#line 10933 "bison-chpl-lib.cpp"
    break;

  case 677: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3611 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10941 "bison-chpl-lib.cpp"
    break;

  case 678: /* dot_expr: expr TDOT ident_use  */
#line 3618 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10947 "bison-chpl-lib.cpp"
    break;

  case 679: /* dot_expr: expr TDOT TTYPE  */
#line 3620 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10953 "bison-chpl-lib.cpp"
    break;

  case 680: /* dot_expr: expr TDOT TDOMAIN  */
#line 3622 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10959 "bison-chpl-lib.cpp"
    break;

  case 681: /* dot_expr: expr TDOT TLOCALE  */
#line 3624 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10965 "bison-chpl-lib.cpp"
    break;

  case 682: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3626 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10975 "bison-chpl-lib.cpp"
    break;

  case 683: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3632 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10985 "bison-chpl-lib.cpp"
    break;

  case 684: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3644 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10991 "bison-chpl-lib.cpp"
    break;

  case 685: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3646 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10999 "bison-chpl-lib.cpp"
    break;

  case 686: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3650 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11007 "bison-chpl-lib.cpp"
    break;

  case 687: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3654 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11015 "bison-chpl-lib.cpp"
    break;

  case 688: /* bool_literal: TFALSE  */
#line 3660 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 11021 "bison-chpl-lib.cpp"
    break;

  case 689: /* bool_literal: TTRUE  */
#line 3661 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 11027 "bison-chpl-lib.cpp"
    break;

  case 690: /* str_bytes_literal: STRINGLITERAL  */
#line 3665 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11033 "bison-chpl-lib.cpp"
    break;

  case 691: /* str_bytes_literal: BYTESLITERAL  */
#line 3666 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11039 "bison-chpl-lib.cpp"
    break;

  case 694: /* literal_expr: INTLITERAL  */
#line 3672 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 11045 "bison-chpl-lib.cpp"
    break;

  case 695: /* literal_expr: REALLITERAL  */
#line 3673 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 11051 "bison-chpl-lib.cpp"
    break;

  case 696: /* literal_expr: IMAGLITERAL  */
#line 3674 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 11057 "bison-chpl-lib.cpp"
    break;

  case 697: /* literal_expr: CSTRINGLITERAL  */
#line 3675 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 11063 "bison-chpl-lib.cpp"
    break;

  case 698: /* literal_expr: TNONE  */
#line 3676 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 11069 "bison-chpl-lib.cpp"
    break;

  case 699: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3678 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11078 "bison-chpl-lib.cpp"
    break;

  case 700: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3683 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11087 "bison-chpl-lib.cpp"
    break;

  case 701: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3688 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 11096 "bison-chpl-lib.cpp"
    break;

  case 702: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3693 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 11105 "bison-chpl-lib.cpp"
    break;

  case 703: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3698 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 11114 "bison-chpl-lib.cpp"
    break;

  case 704: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3703 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 11123 "bison-chpl-lib.cpp"
    break;

  case 705: /* assoc_expr_ls: expr TALIAS expr  */
#line 3712 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 11132 "bison-chpl-lib.cpp"
    break;

  case 706: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3717 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 11142 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TPLUS expr  */
#line 3725 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11148 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TMINUS expr  */
#line 3726 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11154 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TSTAR expr  */
#line 3727 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11160 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TDIVIDE expr  */
#line 3728 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11166 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3729 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11172 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3730 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11178 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TMOD expr  */
#line 3731 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11184 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TEQUAL expr  */
#line 3732 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11190 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3733 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11196 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3734 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11202 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3735 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11208 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr TLESS expr  */
#line 3736 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11214 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr TGREATER expr  */
#line 3737 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11220 "bison-chpl-lib.cpp"
    break;

  case 720: /* binary_op_expr: expr TBAND expr  */
#line 3738 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11226 "bison-chpl-lib.cpp"
    break;

  case 721: /* binary_op_expr: expr TBOR expr  */
#line 3739 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11232 "bison-chpl-lib.cpp"
    break;

  case 722: /* binary_op_expr: expr TBXOR expr  */
#line 3740 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11238 "bison-chpl-lib.cpp"
    break;

  case 723: /* binary_op_expr: expr TAND expr  */
#line 3741 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11244 "bison-chpl-lib.cpp"
    break;

  case 724: /* binary_op_expr: expr TOR expr  */
#line 3742 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11250 "bison-chpl-lib.cpp"
    break;

  case 725: /* binary_op_expr: expr TEXP expr  */
#line 3743 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11256 "bison-chpl-lib.cpp"
    break;

  case 726: /* binary_op_expr: expr TBY expr  */
#line 3744 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11262 "bison-chpl-lib.cpp"
    break;

  case 727: /* binary_op_expr: expr TALIGN expr  */
#line 3745 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11268 "bison-chpl-lib.cpp"
    break;

  case 728: /* binary_op_expr: expr THASH expr  */
#line 3746 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11274 "bison-chpl-lib.cpp"
    break;

  case 729: /* binary_op_expr: expr TDMAPPED expr  */
#line 3747 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11280 "bison-chpl-lib.cpp"
    break;

  case 730: /* unary_op_expr: TPLUS expr  */
#line 3751 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11286 "bison-chpl-lib.cpp"
    break;

  case 731: /* unary_op_expr: TMINUS expr  */
#line 3752 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11292 "bison-chpl-lib.cpp"
    break;

  case 732: /* unary_op_expr: TMINUSMINUS expr  */
#line 3753 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11298 "bison-chpl-lib.cpp"
    break;

  case 733: /* unary_op_expr: TPLUSPLUS expr  */
#line 3754 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11304 "bison-chpl-lib.cpp"
    break;

  case 734: /* unary_op_expr: TBANG expr  */
#line 3755 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11310 "bison-chpl-lib.cpp"
    break;

  case 735: /* unary_op_expr: expr TBANG  */
#line 3756 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 11318 "bison-chpl-lib.cpp"
    break;

  case 736: /* unary_op_expr: TBNOT expr  */
#line 3759 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11324 "bison-chpl-lib.cpp"
    break;

  case 737: /* reduce_expr: expr TREDUCE expr  */
#line 3764 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11332 "bison-chpl-lib.cpp"
    break;

  case 738: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3768 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11340 "bison-chpl-lib.cpp"
    break;

  case 739: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3772 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11348 "bison-chpl-lib.cpp"
    break;

  case 740: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3776 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11356 "bison-chpl-lib.cpp"
    break;

  case 741: /* scan_expr: expr TSCAN expr  */
#line 3783 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11364 "bison-chpl-lib.cpp"
    break;

  case 742: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3787 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11372 "bison-chpl-lib.cpp"
    break;

  case 743: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3791 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11380 "bison-chpl-lib.cpp"
    break;

  case 744: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3795 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11388 "bison-chpl-lib.cpp"
    break;


#line 11392 "bison-chpl-lib.cpp"

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
