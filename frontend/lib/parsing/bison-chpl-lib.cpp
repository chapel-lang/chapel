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
#line 325 "chpl.ypp"

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
#define YYLAST   20558

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  182
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  204
/* YYNRULES -- Number of rules.  */
#define YYNRULES  750
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1327

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
       0,   632,   632,   636,   637,   642,   643,   651,   655,   665,
     670,   677,   678,   679,   680,   681,   682,   683,   684,   685,
     686,   687,   700,   701,   702,   710,   719,   728,   732,   739,
     747,   748,   749,   750,   751,   752,   753,   754,   756,   771,
     779,   791,   802,   814,   826,   837,   853,   854,   858,   868,
     869,   873,   877,   878,   882,   889,   897,   900,   910,   912,
     917,   923,   928,   937,   941,   961,   976,   991,  1007,  1008,
    1010,  1015,  1016,  1021,  1039,  1043,  1047,  1058,  1077,  1078,
    1082,  1086,  1090,  1095,  1099,  1103,  1113,  1118,  1124,  1131,
    1136,  1143,  1154,  1155,  1159,  1164,  1172,  1181,  1191,  1199,
    1208,  1221,  1229,  1230,  1231,  1232,  1233,  1234,  1235,  1236,
    1237,  1238,  1239,  1240,  1241,  1242,  1243,  1244,  1245,  1257,
    1261,  1266,  1271,  1277,  1286,  1287,  1291,  1300,  1304,  1308,
    1312,  1316,  1320,  1329,  1330,  1331,  1335,  1336,  1337,  1338,
    1339,  1343,  1344,  1345,  1357,  1358,  1359,  1364,  1365,  1366,
    1367,  1368,  1369,  1370,  1371,  1372,  1373,  1374,  1375,  1376,
    1377,  1378,  1379,  1380,  1381,  1382,  1386,  1387,  1388,  1389,
    1390,  1391,  1392,  1393,  1394,  1395,  1396,  1397,  1404,  1405,
    1406,  1407,  1411,  1412,  1416,  1423,  1433,  1437,  1443,  1443,
    1449,  1449,  1458,  1459,  1460,  1461,  1462,  1463,  1464,  1468,
    1473,  1478,  1483,  1490,  1498,  1499,  1503,  1512,  1518,  1526,
    1545,  1544,  1554,  1553,  1566,  1572,  1575,  1576,  1580,  1599,
    1613,  1627,  1631,  1635,  1639,  1643,  1647,  1651,  1655,  1675,
    1679,  1683,  1687,  1691,  1695,  1699,  1703,  1707,  1711,  1715,
    1719,  1723,  1727,  1731,  1735,  1739,  1743,  1747,  1751,  1755,
    1759,  1763,  1768,  1776,  1785,  1789,  1793,  1797,  1801,  1805,
    1809,  1813,  1817,  1822,  1827,  1832,  1840,  1855,  1873,  1877,
    1884,  1885,  1890,  1895,  1896,  1897,  1898,  1899,  1900,  1901,
    1902,  1903,  1904,  1905,  1906,  1911,  1916,  1917,  1918,  1919,
    1928,  1929,  1933,  1937,  1941,  1948,  1952,  1956,  1963,  1967,
    1971,  1975,  1982,  1983,  1987,  1991,  1995,  2002,  2015,  2031,
    2039,  2043,  2052,  2053,  2057,  2061,  2066,  2075,  2080,  2084,
    2091,  2092,  2096,  2105,  2110,  2121,  2128,  2129,  2130,  2134,
    2135,  2139,  2143,  2147,  2151,  2155,  2163,  2181,  2194,  2201,
    2206,  2213,  2212,  2222,  2228,  2227,  2242,  2249,  2261,  2279,
    2276,  2304,  2308,  2309,  2311,  2316,  2317,  2321,  2322,  2326,
    2329,  2331,  2336,  2337,  2348,  2368,  2367,  2383,  2382,  2400,
    2410,  2407,  2441,  2449,  2457,  2468,  2479,  2488,  2503,  2504,
    2508,  2509,  2510,  2519,  2520,  2521,  2522,  2523,  2524,  2525,
    2526,  2527,  2528,  2529,  2530,  2531,  2532,  2533,  2534,  2535,
    2536,  2537,  2538,  2539,  2540,  2541,  2542,  2543,  2547,  2548,
    2549,  2550,  2551,  2552,  2553,  2554,  2555,  2556,  2557,  2558,
    2563,  2564,  2568,  2569,  2573,  2574,  2578,  2579,  2583,  2584,
    2588,  2589,  2593,  2597,  2598,  2602,  2606,  2611,  2616,  2621,
    2626,  2634,  2638,  2646,  2647,  2648,  2649,  2650,  2651,  2652,
    2653,  2654,  2658,  2659,  2660,  2661,  2662,  2663,  2667,  2668,
    2669,  2673,  2674,  2675,  2676,  2677,  2678,  2679,  2683,  2684,
    2687,  2688,  2692,  2693,  2694,  2698,  2703,  2704,  2706,  2708,
    2710,  2715,  2717,  2722,  2724,  2726,  2728,  2730,  2732,  2734,
    2739,  2740,  2741,  2745,  2754,  2758,  2766,  2770,  2777,  2798,
    2799,  2804,  2805,  2806,  2807,  2808,  2813,  2812,  2821,  2829,
    2833,  2840,  2858,  2877,  2881,  2885,  2892,  2894,  2896,  2903,
    2904,  2905,  2909,  2913,  2914,  2915,  2919,  2920,  2924,  2925,
    2929,  2930,  2931,  2932,  2936,  2937,  2941,  2945,  2946,  2952,
    2953,  2957,  2958,  2962,  2966,  2973,  2974,  2978,  2983,  2992,
    2993,  2997,  2998,  3005,  3006,  3007,  3008,  3009,  3010,  3012,
    3014,  3016,  3018,  3024,  3026,  3029,  3031,  3033,  3035,  3037,
    3039,  3041,  3043,  3045,  3047,  3052,  3056,  3066,  3076,  3085,
    3100,  3115,  3130,  3140,  3150,  3159,  3174,  3189,  3207,  3211,
    3215,  3219,  3223,  3227,  3231,  3235,  3242,  3260,  3268,  3269,
    3270,  3271,  3272,  3273,  3274,  3278,  3279,  3283,  3290,  3291,
    3295,  3304,  3305,  3309,  3323,  3327,  3334,  3335,  3336,  3337,
    3338,  3339,  3343,  3345,  3347,  3349,  3351,  3357,  3364,  3376,
    3388,  3401,  3418,  3425,  3430,  3435,  3440,  3446,  3452,  3482,
    3489,  3496,  3497,  3501,  3503,  3504,  3506,  3508,  3509,  3510,
    3511,  3514,  3515,  3516,  3517,  3518,  3519,  3520,  3521,  3522,
    3526,  3527,  3531,  3532,  3533,  3537,  3538,  3539,  3540,  3549,
    3550,  3553,  3554,  3555,  3559,  3571,  3583,  3590,  3592,  3594,
    3596,  3598,  3600,  3609,  3610,  3614,  3618,  3625,  3626,  3630,
    3631,  3635,  3636,  3637,  3638,  3639,  3640,  3641,  3642,  3647,
    3652,  3657,  3662,  3667,  3676,  3681,  3690,  3691,  3692,  3693,
    3694,  3695,  3696,  3697,  3698,  3699,  3700,  3701,  3702,  3703,
    3704,  3705,  3706,  3707,  3708,  3709,  3710,  3711,  3712,  3716,
    3717,  3718,  3719,  3720,  3721,  3724,  3728,  3732,  3736,  3740,
    3747,  3751,  3755,  3759,  3767,  3768,  3769,  3770,  3771,  3772,
    3773
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

#define YYPACT_NINF (-1122)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-751)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1122,    66,  4439, -1122,     7,    63, -1122, -1122, -1122, -1122,
   -1122, -1122, -1122, 13415,    73,   230,   171, 15496,   186, 20329,
      73,   250,   201,   231,   230,  5847, 13415,   177, 20385, -1122,
     386,   332, -1122, 10951,   351,    19,   239, -1122,   404,   336,
   20385, 20385, 20385, -1122,  3798, 12007,   412, 13415, 13415,    56,
   -1122,   424,   432, 13415, -1122, 15496, -1122, 13415,   443,   371,
     316,   457,  4077,   472, 20441, -1122, 13415,  9367, 13415, 12007,
   15496, 13415,   459,   498,   400,  5847,   536, 13415,   547,  7959,
    7959, -1122,   550, -1122, 15496, -1122,   553, 11127, -1122,   470,
   13415, -1122, 13415, -1122, -1122, 15323, 13415, -1122, 11303, -1122,
   -1122, -1122,  4791,  8487, 13415, -1122,  5495, -1122, -1122, -1122,
   -1122,  2863,   478, -1122,   479,   480, -1122,    87, -1122, -1122,
   -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122,
   -1122,  8135, -1122,  8663, -1122, -1122,    39, -1122, -1122, -1122,
     604, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122,   514,
   20385, -1122, 20073,   495,   148, -1122,   193, -1122, -1122, -1122,
     216,   300, -1122, -1122, 20385,  3798, -1122,   504,   509, -1122,
   -1122, -1122,   511,   512, 13415,   516,   517, -1122, -1122, -1122,
   19582,  2386,   312,   518,   519, -1122, -1122,   429, -1122, -1122,
   -1122, -1122, -1122,   -25, -1122, -1122, -1122, 13415, -1122, 20385,
   -1122, 13415, 13415,    51,   619,   440, -1122, -1122, -1122, -1122,
   19582,   463, -1122, -1122,   507,  5847, -1122, -1122, -1122,   521,
     139,   520, -1122,   226, -1122,   522, -1122,   284, 19582,  9543,
   -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122,
   -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122,
   -1122, -1122, -1122, -1122, -1122, -1122,   523, -1122, -1122, 20385,
   20385,   -17, 15959, -1122, -1122,   605,  9543,   526,   525,   225,
    5847,  2258,    49,     0, -1122,  5847, -1122, -1122, 17342, -1122,
      12, 17904,   778, -1122,   528,   529, -1122, 17342,   139,   778,
   -1122,  9543,  1746,  1746,    23,    -8, -1122,    33, -1122, -1122,
   -1122, -1122,  9719,  9719, -1122,   395, -1122,   531,   559, -1122,
   -1122,  3364,   548, -1122, -1122, 19582,   372, 13415, 13415, -1122,
      94, -1122, -1122, 19582,   544, 17945, -1122, 17342,   139, 19582,
     538,  9543, -1122, 19582, 17985, -1122, -1122, -1122, -1122, -1122,
     139,   546, -1122,   206,   206,   599,   778,   778,   104, -1122,
   -1122,  4967,   127, -1122, 13415,   138,   145, -1122,   599,   705,
   -1122, -1122, 20385, -1122, -1122,  4791, -1122,    69, -1122,   605,
   -1122, -1122,   711,   542,  5143, 13415, -1122, 13415,   652, -1122,
   -1122, 16852,    50,   514, 19582,   263, 13415,  5847, 11479, 12183,
   12183, 10951, 11655, -1122, -1122, -1122, 20385, 13415,   549, -1122,
     551, 20161,   593,   156, -1122,   327,   216, -1122, -1122, -1122,
   -1122,   635, -1122, -1122, -1122, -1122,   117,   638, -1122, -1122,
   -1122,  4259,   606,   384, -1122,   387,   481, -1122,   580, 13415,
   13415, 13415, 12183, 12183, 13415,   484, 13415, 13415, 13415, 13415,
   13415,   380, 15323, 13415, 13415, 13415, 13415, 13415, 13415, 13415,
   13415, 13415, 13415, 13415, 13415, 13415, 13415, 13415, 13415,   664,
   -1122, -1122, -1122, -1122, -1122, 11127, 11127, -1122, -1122, -1122,
   -1122, 11127, -1122, -1122, 11127, 11127,  9543,  9895, 12183, 12183,
   18066,   561, 13415, 13415, 12359, 20385,  8839, -1122, -1122, -1122,
    3798, -1122, 13415, -1122,   610,   563,   591, -1122, -1122, -1122,
     621,   624,  5847,   717,  5847, -1122,   721, 13415,   692,   587,
   -1122, 11127, 20385, -1122, -1122, -1122,  2258, -1122, -1122,   187,
   -1122, 13591,   639, 13415,  3798, -1122, -1122, 13415, -1122, 20217,
   13415, 13415, -1122,   595, -1122, -1122, -1122, -1122, 20273, -1122,
     216, 15147, -1122,  2258, -1122,   617, 13591, -1122, 19582, 19582,
   -1122,   520, -1122,    37, -1122, 11127,   596, -1122,   751,   751,
   -1122, -1122, -1122, -1122, 12535, -1122, 18025,  9015, -1122,  9191,
   -1122, -1122, 11479, 12183, 12183, -1122,   479, 10071, -1122,   233,
   -1122,  5319, -1122,   389, 18146,   397, 16944, 20385,  7783,  7607,
   -1122,   514,   602, -1122,   226, -1122,  1888,   658, 20385,   607,
      30, 17020,    11,  2962,    11, 16684,    30, 17342, 15614,   -54,
     134, 18226,   103,   -67,   631,  1789, -1122, -1122, 13415,   115,
   -1122, -1122,   191, -1122, -1122,    81, -1122, -1122,  7431, -1122,
     676, -1122,   614,   640, -1122,   642, -1122,   643,   644,   646,
     647,   648, -1122,   650,   655,   656,   657,   660,   661,   662,
   -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122,
   -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122,
   -1122, -1122, 13415,   671,   665,   614, -1122,   614, -1122, 13415,
   -1122, 20385, -1122, -1122,  3889,  3889,   508, -1122,   508, -1122,
     508, 19810,   634,  1318,  1541,   139,   206, -1122,   491, -1122,
   -1122, -1122, -1122, -1122,   599, 19860,   508,  2925,  2925,  3889,
    2925,  2925,   755,   206, 19860, 19780,   755,   778,   778,   206,
     599,   623,   651,   654,   667,   668,   669,   653,   663, -1122,
     508, -1122,   508, 13415, 11127, 17427, 17503,    29,   641,   175,
   -1122, -1122, -1122,   219, -1122,   992, 19627,   281,   409, 19582,
   11127, -1122, 11127, 13415, 13415,   782,  5847,   788, 18306,  5847,
   17544, 20385, -1122,   220, -1122,   256, -1122,   261,  2258,    49,
   -1122, 19582, 10247, -1122, 19582, -1122, -1122, -1122, 20385, 18347,
   18387, -1122,   617,   677, -1122,   278, 13591, -1122,   238, -1122,
   13415, -1122,   659,    62,   666, -1122,  1403, -1122, -1122, -1122,
   -1122, -1122, -1122, -1122, 17105, 16760,   170, -1122, -1122,   279,
   -1122, -1122, -1122, -1122, 13415, -1122,   247, 15717, 13415, -1122,
   12711,  7783,  7607, -1122, 10423,   355, 10599,   453,   462, 10775,
    8311,   493,   416, -1122, -1122, -1122, 18540,   678,   670,   675,
   -1122, 20385,  3798, 12183,    30, 13415,   781, 13415, -1122,  6023,
   12183, -1122,   681, -1122,    30,  6199, 12183, -1122,    30, -1122,
      30, 12183, -1122,    30, -1122, -1122, 19582,  5847,   672, 12183,
   12887,  4615,   674, 13415, 13063, -1122, -1122, 11127,   679,   590,
   19582, 20385,   478,   448, -1122,   676, -1122, -1122, -1122, -1122,
   -1122,    22,   431, -1122, 18580, -1122, 15877, -1122, -1122, 19582,
   -1122,   683,   682, -1122, -1122, -1122, -1122, -1122, -1122, -1122,
   -1122,   280, 13767, 12183, 13943, 12183, 12183, 15614, 11127, -1122,
   -1122,  8839, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122,
   -1122, -1122, -1122, -1122,    38, 12183, 12183, -1122, -1122, -1122,
   19582, 19582,  5847, -1122,  5847, 13415, -1122,  5847,   803,   689,
     693, 20385,   520, -1122, -1122,   639, -1122, -1122, -1122,   687,
     688,   238, 13415,   676,   617, -1122, 11831, -1122, -1122, 19582,
   -1122,  5847, 13415, -1122, -1122, -1122, 20385, -1122,   715,   520,
   12183, 12183, 12183,  5847, 11127, -1122, -1122, -1122, -1122, -1122,
     491, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122,
   -1122, -1122, -1122, -1122, -1122,  7783, -1122, -1122, 18620,   199,
   17697,   428,   697,  7783, -1122, 13415, -1122, -1122,  3447,   441,
     442,    73, 18701, -1122, 18741, 13415,   293, 10951, 19582,    32,
   17589,  8839, -1122, 10951, 19582,    24, 17181, -1122, -1122,    11,
   17266, -1122, -1122,  5847,   -36, 17742, -1122, -1122, 19582,  5847,
   19582, -1122, 18787,   296, -1122, -1122, -1122, -1122, 19954, 20022,
   -1122, -1122, 20161, -1122,    22,   165, 13415, -1122,   718,   699,
   -1122,   614,   614, -1122, -1122, -1122, 13415,   835, 18894, 13415,
     843, 18940,   700, 18980,   309, -1122,   639,   356,   377, -1122,
   -1122, 19582,   845,  5847, -1122, -1122, -1122, -1122, -1122,  9543,
    9543, -1122, -1122, -1122, -1122,   238, 19582, -1122,    84,    21,
     703, 13415, -1122,    30, 17342,    11, 17266,   -18, 17787, -1122,
     172, 13239,  7783,  7607,  7783,  7607, -1122, -1122, -1122, 19101,
   19582, 19771, -1122, -1122, -1122,    30,    30, -1122, 17342, -1122,
   16039,  6375, -1122,  6551, -1122,   385, -1122, 16119,  6727, -1122,
      30,  6903, -1122,    30, -1122,    30, -1122,    30, -1122,  7079,
     704,  7255,   707, -1122, 13415,   719, 20022, -1122, -1122, -1122,
   -1122, -1122, -1122,   254, -1122,   113, -1122,   465, 19141,    83,
   -1122, -1122, -1122, 19061, 14119, 14295, 19182, 14471, 14647, 14823,
   14999, -1122, -1122,  5847, -1122,   713,   716, -1122, -1122,  5847,
   -1122,   520, 19582,  5847,  5847, -1122, -1122, 19582,   445,   723,
     466,   725, 13415, -1122, -1122, -1122, -1122,  5847, 10951, 19582,
   10951, 19582,  8839, -1122,  5847, 10951, 19582, -1122, 10951, 19582,
   -1122, -1122, -1122, 10951, -1122, 19582,  5847, 10951, -1122, 19582,
    5847, 19582, -1122, -1122,    27, -1122, 13415,   165,   254,   254,
     254,   254,   254,   254,   165,  5671, -1122, -1122, -1122, -1122,
   -1122, 13415, 13415, 13415, 13415, 13415, 13415, 13415, 13415, -1122,
     727,   728,   658, -1122, -1122, -1122, -1122, -1122, 19582, 18306,
   16199, 16279, -1122, 18306, 16359, 16439, 16519, -1122, 16599, -1122,
   -1122, 19582, -1122, -1122, -1122, -1122, -1122, -1122, -1122,   733,
   -1122, 19262, 19303, 19379, 19420, 19465, 19541, -1122, -1122,  5847,
    5847,  5847,  5847,  5847,  5847, 13415, 13415, 13415, 13415, 13415,
   13415, 18306, 18306, 18306, 18306, 18306, 18306
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
      12,    48,    51,    52,    56,     0,    46,    71,    13,    77,
      14,    15,    16,    17,    30,   551,   552,    23,    47,   187,
     197,   215,   206,   660,   198,    18,     0,    32,    31,    20,
       0,   285,    19,   656,    22,    36,    33,    34,   196,   329,
       0,   194,     0,     0,   653,   352,     0,   650,   192,   369,
     461,   452,   648,   195,     0,     0,   193,   665,   644,   643,
     647,   555,   553,     0,   215,   654,   655,   659,   658,   657,
       0,   554,     0,   666,   667,   668,   691,   692,   649,   557,
     556,   651,   652,     0,    29,   573,   167,   215,   145,     0,
     574,   215,   215,     0,     0,   653,   665,   553,   654,   655,
     563,   554,   666,   667,     0,     0,   606,   134,   135,     0,
     572,     0,   495,     0,   502,     0,    21,     0,   539,   215,
     141,   147,   159,   153,   152,   161,   138,   151,   162,   148,
     142,   163,   136,   164,   157,   150,   158,   156,   154,   155,
     137,   139,   149,   160,   165,   143,     0,   140,   214,     0,
       0,     0,     0,    70,    69,    71,   215,     0,     0,     0,
       0,     0,     0,     0,   509,     0,   183,    41,     0,   320,
       0,   319,   731,   626,   623,   624,   625,     0,   566,   732,
       7,   215,   351,   351,   448,   180,   443,   181,   444,   445,
     449,   450,   179,   178,   451,   441,   535,     0,   356,   357,
     359,     0,   442,   534,   361,   522,     0,   215,   215,   184,
     650,   641,   664,   642,     0,     0,    44,     0,   570,   558,
       0,   215,    45,   564,     0,   298,    48,   302,   299,   302,
     568,     0,    55,   733,   735,   636,   730,   729,     0,    74,
      78,     0,     0,   541,   215,     0,     0,   542,   637,     0,
       6,   326,     0,   204,   327,     0,    49,     0,     9,    71,
      50,    53,     0,    60,     0,   215,    72,   215,     0,   506,
     202,     0,   689,   329,   661,   210,   215,     0,   215,   215,
     215,   215,   215,   217,   575,   588,     0,   215,     0,   325,
       0,     0,   346,     0,   339,   441,   461,   459,   460,   458,
     377,   463,   462,   466,   465,   467,     0,   456,   453,   454,
     457,     0,   499,     0,   496,     0,   645,    35,   627,   215,
     215,   215,   215,   215,   215,   734,   215,   215,   215,   215,
     215,     0,   635,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   408,
     415,   416,   417,   412,   414,   215,   215,   410,   413,   411,
     409,   215,   419,   418,   215,   215,   215,   215,   215,   215,
       0,     0,   215,   215,   215,     0,   215,    37,    25,    38,
       0,    26,   215,    39,   551,     0,   546,   547,   550,   338,
       0,     0,     0,   259,     0,   408,   255,   215,     0,     0,
      24,   215,     0,   269,    27,   513,     0,   514,   516,     0,
     533,   215,   519,   215,     0,   182,    40,   215,   322,     0,
     215,   215,    42,     0,   191,   189,   446,   447,     0,   442,
     461,   441,   434,     0,   433,   537,   215,   126,   662,   663,
     367,     0,   185,     0,    43,   215,     0,   309,   300,   301,
      28,    76,    75,    79,   215,   698,     0,   215,   683,   215,
     685,     8,   215,   215,   215,   216,    56,   215,    54,     0,
      65,     0,   124,     0,   119,     0,    86,     0,   215,   215,
     199,   329,     0,   212,     0,   209,   605,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   589,     0,
       0,   539,     0,   296,   330,     0,   337,   343,   215,   341,
     336,   426,     0,   428,   432,     0,   464,   527,     0,   529,
     468,   455,   424,   141,   404,   147,   402,   153,   152,   151,
     148,   142,   406,   163,   164,   150,   154,   137,   149,   165,
     401,   383,   386,   384,   385,   407,   396,   387,   400,   392,
     390,   403,   391,   389,   394,   399,   388,   393,   397,   398,
     395,   405,   215,   380,     0,   424,   381,   424,   378,   215,
     498,     0,   493,   508,   726,   725,   728,   737,   736,   741,
     740,   722,   719,   720,   721,   639,   709,   144,     0,   679,
     680,   146,   678,   677,   633,   713,   724,   718,   716,   727,
     717,   715,   707,   712,   714,   723,   706,   710,   711,   708,
     634,     0,     0,     0,     0,     0,     0,     0,     0,   739,
     738,   743,   742,   215,   215,     0,     0,     0,   296,   616,
     617,   619,   621,     0,   608,     0,     0,     0,     0,   540,
     215,   560,   215,   215,   215,   258,     0,   254,     0,     0,
       0,     0,   559,     0,   272,     0,   270,     0,   517,     0,
     532,   531,   215,   511,   632,   510,   321,   318,     0,     0,
       0,   676,   537,   362,   358,     0,   215,   538,   519,   360,
     215,   366,     0,     0,     0,   561,     0,   303,   699,   640,
     684,   543,   686,   544,     0,     0,     0,    57,    58,     0,
      61,    63,    67,    66,   215,   101,     0,     0,   215,    94,
     215,   215,   215,    64,   215,   383,   215,   384,   385,   215,
     215,   395,     0,   420,   421,    81,    80,    93,     0,     0,
     331,     0,     0,   215,     0,   215,     0,   215,   227,     0,
     215,   226,     0,   235,     0,     0,   215,   233,     0,   243,
       0,   215,   241,     0,   220,   219,   590,     0,     0,   215,
     215,     0,     0,   215,   215,   702,   293,   215,     0,   351,
     347,     0,     0,   441,   427,   468,   525,   524,   523,   526,
     469,   476,   441,   376,     0,   382,     0,   372,   373,   500,
     497,     0,     0,   132,   130,   131,   129,   128,   127,   674,
     675,     0,   215,   215,   215,   215,   215,   700,   215,   618,
     620,   215,   607,   166,   173,   172,   171,   168,   175,   176,
     170,   174,   169,   177,     0,   215,   215,   507,   549,   548,
     267,   266,     0,   261,     0,   215,   257,     0,   263,     0,
     295,     0,     0,   515,   518,   519,   520,   521,   317,     0,
       0,   519,   215,   468,   537,   536,   215,   437,   435,   368,
     311,     0,   215,   310,   313,   562,     0,   304,   307,     0,
     215,   215,   215,     0,   215,    59,   125,   122,   102,   114,
     108,   107,   106,   116,   103,   117,   112,   105,   113,   111,
     109,   110,   104,   115,   118,   215,   121,   120,    89,    88,
      87,     0,     0,   215,   200,   215,   201,   331,   351,     0,
       0,   605,   605,   223,     0,   215,     0,   215,   578,     0,
       0,   215,   236,   215,   584,     0,     0,   234,   244,     0,
       0,   242,   251,     0,     0,     0,   701,   249,   591,     0,
     704,   703,     0,     0,   324,   190,   188,   323,   351,   351,
     332,   342,     0,   429,   476,     0,   215,   370,   431,     0,
     379,   424,   424,   681,   682,   295,   215,     0,     0,   215,
       0,     0,     0,     0,     0,   609,   519,   665,   665,   260,
     256,   596,   262,     0,    73,   292,   271,   268,   512,   215,
     215,   438,   436,   363,   364,   519,   423,   315,     0,     0,
       0,   215,   305,     0,     0,     0,     0,     0,     0,    62,
       0,   215,   215,   215,   215,   215,    95,    98,    84,    83,
      82,   351,   207,   213,   211,     0,     0,   218,     0,   253,
       0,     0,   225,     0,   224,     0,   611,     0,     0,   231,
       0,     0,   229,     0,   239,     0,   237,     0,   252,     0,
       0,     0,     0,   250,   215,   297,   351,   334,   333,   345,
     349,   490,   491,     0,   492,   478,   481,     0,   477,     0,
     425,   374,   375,     0,   215,   215,     0,   215,   215,   215,
     215,   297,   613,     0,   265,     0,     0,   440,   439,     0,
     314,     0,   308,     0,     0,   123,    91,    90,     0,     0,
       0,     0,   215,   208,   222,   221,   228,     0,   215,   577,
     215,   576,   215,   610,     0,   215,   583,   232,   215,   582,
     230,   240,   238,   215,   247,   593,     0,   215,   245,   592,
       0,   705,   294,   335,     0,   489,   215,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   470,   472,   474,   371,
     471,   215,   215,   215,   215,   215,   215,   215,   215,   264,
     628,   629,   316,   306,    97,   100,    96,    99,    85,   581,
       0,     0,   612,   587,     0,     0,     0,   248,     0,   246,
     350,   480,   482,   483,   486,   487,   488,   484,   485,   479,
     473,     0,     0,     0,     0,     0,     0,   630,   631,     0,
       0,     0,     0,     0,     0,   215,   215,   215,   215,   215,
     215,   580,   579,   586,   585,   595,   594
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1122, -1122, -1122,     4,   -60,  2509, -1122,   417,   783, -1122,
   -1122,   488,  -144,  -107, -1122,   319, -1122, -1122,   -88, -1122,
   -1122,    16,   645, -1122,  -548,  3150,   533,  -576, -1122,  -792,
   -1122, -1122, -1122,    85, -1122, -1122, -1122,   879, -1122,   110,
    -100, -1122, -1122,  -472,  1060, -1121,  -851, -1122, -1122,   306,
   -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122,    15, -1122,
     541,  -335, -1122,   524, -1122, -1122,   -40,  1727, -1122, -1122,
   -1122, -1122,   573, -1122,   -63, -1122, -1122, -1122, -1122,   390,
   -1122, -1122, -1122,  -104, -1122,  -351,  -806, -1122, -1122, -1122,
   -1122, -1122,  -390, -1122,    82, -1122, -1122, -1122, -1122, -1122,
     378, -1122,  1668, -1122, -1122, -1122, -1122,   545, -1122, -1122,
   -1122, -1122,    25,  -407,  -174,  -784,  -915,  -655, -1122,    26,
   -1122,    43,   391,   -49,   630,   -47, -1122, -1122,  -361,  -817,
   -1122,  -303, -1122,  -121,  -310,  -301,  -787, -1122, -1122,   107,
     269, -1122,  -199,   823, -1122,  -156,   436,   188,  -456,  -762,
    -603, -1122, -1122, -1122,  -598,  -515, -1122,  -740,    -5,    68,
   -1122,  -256,  -493,  -536,    -2, -1122, -1122, -1122, -1122, -1122,
   -1122,   223, -1122,   -19, -1122, -1122,  -474, -1122,  -884, -1122,
   -1122,   434,   750, -1122, -1122, -1122, -1122,  2760, -1122,   -51,
    1255, -1122,  1444,  1490, -1122, -1122, -1122, -1122, -1122, -1122,
   -1122, -1122, -1122,  -482
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   350,   106,   757,   108,   109,   110,   367,
     368,   111,   112,   113,   114,   373,   578,   809,   810,   115,
     116,   117,   378,   118,   119,   120,   351,   837,   585,   838,
     121,   122,  1006,   582,   583,   123,   124,   219,   255,   272,
     125,   257,   126,   306,   851,   127,   128,   293,   292,   129,
     130,   131,   132,   133,   134,   594,   841,   135,   203,   137,
     393,   600,   138,   261,   139,   765,   766,   204,   141,   142,
     143,   144,   558,   797,   979,   145,   146,   793,   974,   279,
     280,   147,   148,   149,   150,   398,   879,   151,   152,   403,
     881,   882,   404,   153,   205,  1244,   155,   156,   307,   308,
     309,   963,   157,   321,   551,   790,   158,   159,  1179,   160,
     161,   674,   675,   833,   834,   835,   967,   893,   406,   622,
    1069,   623,   545,   624,   311,   539,   421,   410,   416,   891,
    1259,  1260,   162,  1067,  1175,  1176,  1177,   163,   164,   423,
     424,   680,   165,   166,   223,   273,   274,   518,   519,   773,
     313,   889,   629,   630,   522,   314,   787,   788,   352,   355,
     356,   495,   496,   497,   206,   168,   169,   394,   170,   395,
     171,   207,   173,   215,   216,   743,   858,  1145,   744,   745,
     174,   208,   209,   177,   178,   179,   322,   180,   385,   498,
     211,   182,   212,   213,   185,   186,   187,   188,   612,   189,
     190,   191,   192,   193
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   221,   305,   791,   747,   371,   105,   475,   401,   425,
     509,   617,   832,   310,   676,   312,   324,   136,   763,   794,
     897,   227,   898,   167,   490,   888,   968,   885,  1060,   383,
    1012,   789,   592,  1007,  1018,   533,   341,  1085,   792,   520,
     136,   811,   961,   275,   275,   625,  1101,   523,  -290,   770,
     520,   265,   357,   275,   602,   604,  1148,   609,  1258,  1255,
     767,   316,   275,   386,  1141,   478,     3,   852,  1064,   479,
     536,   387,   361,   167,   283,   556,   916,   167,   167,   388,
     389,   390,   520,  1065,   154,   852,   502,   785,   507,  -291,
     136,   482,   483,   386,   136,   136,  -530,   687,   689,   876,
     167,   387,   263,   852,   167,   264,   877,   154,   386,   572,
     573,   390,  -312,  -530,   537,  1255,  1199,   136,   627,  -273,
      67,   136,   867,  1258,   388,   389,   390,   369,   854,   494,
     860,   863,   852,   375,   284,   868,   872,   971,   256,   364,
    1159,  1066,  -530,   729,   731,   852,  1104,  1146,   524,  -528,
     268,   269,   270,   285,   365,  -312,   887,   154,  1203,   391,
     527,   154,   154,    83,  -530,   229,   494,  1060,  1171,   492,
     286,   955,   376,   194,  -530,  -530,    67,   870,  -528,   783,
     972,   869,   365,   365,   154,   521,  -530,  -530,   154,   391,
    1197,   494,   365,  1098,   214,   365,   521,  -530,   365,  1102,
    -530,   365,   377,   365,   391,   917,   266,  1167,  1168,  -312,
    -528,  1131,  -530,   167,   392,  -530,   939,   982,   550,  1172,
     727,   728,   919,  -528,  1105,   172,   484,  -274,   521,  1128,
     136,   494,  1246,   217,   973,  1122,  -528,   431,   602,   609,
     839,   911,  -280,   411,   574,  1011,   407,   677,   172,  1256,
     697,   874,  -530,    23,   365,   852,  -344,  1171,  1173,   392,
     399,  1247,   402,  -340,   628,  -365,   920,   408,   167,  1174,
     194,   965,  1123,   167,   422,   564,   561,   409,   591,   875,
    1060,   435,   870,  -528,   218,   136,   567,  -340,  -528,   441,
     136,   852,   412,   569,   371,   413,   432,   154,   172,   565,
     433,   198,   172,   172,   619,    56,  -279,   414,  1172,   481,
     871,   222,   568,    37,  -602,  1243,  -351,    65,   492,   570,
    1013,  -672,   224,  -672,  1192,   172,   415,   417,   620,   172,
     778,  1209,   872,  1211,   748,   768,  1086,  -351,  1282,   883,
      54,   703,    85,  1198,  1205,   305,   983,  -351,   435,   167,
     229,   701,   154,   439,   294,   563,   441,   154,  1174,  1103,
     444,   769,   -70,   167,   772,   884,   136,   921,   752,   500,
     501,   936,   167,   593,   296,   479,   494,   494,   298,   418,
     136,   517,   -70,   697,  1053,   167,  1245,   610,  -288,   136,
     966,   419,   614,   922,   950,   842,   365,   258,   512,   194,
     698,   -70,   136,   299,   951,   812,   300,  -278,    58,   768,
     420,   494,   266,   699,   722,   723,  1181,  1182,   301,   678,
     724,   544,   294,   725,   726,  1084,   768,   984,   752,  1120,
     952,   -70,   492,   154,   198,   953,   175,   304,   172,   747,
     267,   492,   296,   700,   752,  -748,   298,   154,   811,  -748,
     493,   290,   964,   985,  1075,   494,   154,   752,   294,   175,
    -275,  1293,  1294,  1295,  1296,  1297,  1298,  1139,  -282,   154,
    1165,   299,   256,   342,   300,   294,   359,   494,   296,   737,
    -286,   305,   298,  1191,   701,   476,   301,   477,  -283,   676,
     702,  1061,   310,   172,   312,   296,   335,   338,   172,   298,
     167,   621,   167,   -69,  -615,   304,   613,   299,  1021,   175,
     300,   402,    58,   175,   175,  1029,   357,   136,   357,   136,
     492,  1035,   301,   -69,   299,  -614,  1039,   300,  -277,    58,
    -615,   673,   681,  1222,  1044,   524,   175,   814,   547,   301,
     175,   304,   -69,  -749,   291,   818,  1208,  -749,  1210,   747,
     682,  -614,  -750,   683,  -281,   815,  -750,   524,   304,  1223,
     330,  1150,  1153,   819,  1013,  1155,  1157,   336,   336,   806,
    1160,  1162,   -69,   331,   172,   937,  1013,   844,  1077,   167,
    1080,  1082,  1014,  -745,   154,   563,   154,  -745,   172,   681,
     524,  1248,  -287,  1013,  1126,   738,   136,   172,   534,   535,
     687,   729,  -673,  -289,  -673,  -430,  -276,  1133,  1134,  -284,
     172,  1274,   361,  -672,  1013,  -672,    22,    23,  1249,    89,
    1250,  1251,   764,  1252,  1253,   362,   517,    29,   363,   372,
     431,  1257,  1276,    32,   494,   801,  -669,   803,  -669,   777,
      37,  1150,  1153,  1160,  1162,  1113,  1115,  1117,   544,   175,
     435,   374,   494,   517,  -671,   439,  -671,  -670,   441,  -670,
     396,   397,   444,   154,   901,   431,   902,    54,   405,    56,
    -599,    58,  1169,  1055,   426,   485,  1056,  -598,   427,   364,
     486,    65,  -603,  -604,  -601,  -600,  1020,   488,   491,   432,
     376,   365,   510,   433,   499,   546,  1257,   823,   511,   938,
      81,   530,   531,    83,   175,   540,    85,   541,   846,   175,
     552,   555,   560,   571,   576,   577,   987,   703,   587,   618,
     615,   957,  1072,   616,   432,   172,   626,   172,   433,   631,
     441,    89,   679,   721,   734,  1059,   750,   751,  1062,   752,
     747,   435,   436,   934,   437,   438,   439,   753,   440,   441,
     754,   756,   176,   444,   167,   759,   100,   167,   761,   450,
     451,   762,  1057,   454,   786,   772,   455,   456,   457,   781,
     795,   136,   796,   840,   136,   176,   435,   494,   845,   492,
     847,   439,   890,   440,   441,   175,   431,   892,   444,   903,
    -144,   422,  -166,  -173,  -172,   451,  -171,  -168,  -145,   175,
    -175,   455,   456,   457,   172,  -176,  -170,  -174,   175,   431,
    -146,  -169,  -177,   895,   918,   896,   942,   904,   494,  1058,
     905,   175,   944,   305,   962,   176,  1013,   909,  1025,   176,
     176,   970,   305,   906,   907,   908,  1016,  1093,   154,   910,
     975,   154,  1026,  1195,  1196,   432,  1017,   167,  1043,   433,
    1049,  1054,   176,   167,  1031,  1094,   176,  1073,  1074,  1095,
    1099,  1100,  1111,  1127,   136,   167,   883,  1184,   432,   167,
     136,   949,   433,  1180,  1059,  1187,  1189,  1201,   517,  1193,
    1236,  1247,   136,  1240,   494,  1242,   136,  1270,   958,  1275,
    1271,  1277,  1307,  1308,   366,   807,  1119,   435,   436,   986,
     437,   438,   439,   225,   440,   441,   978,   581,   575,   444,
     508,  1096,   559,  1110,  1166,   606,   451,   776,  1068,   784,
     435,  1071,   455,   456,   457,   439,  1063,   440,   441,   782,
     595,   154,   444,  1087,  1088,   538,   175,   154,   175,   451,
     167,  1290,   167,  1170,  1299,   167,  1292,   457,  1019,   154,
     900,   422,   371,   154,   380,   371,   954,   136,  1058,   136,
     775,     0,   136,     0,     0,   176,   507,  1109,     0,   167,
       0,     0,     0,   507,     0,     0,     0,     0,     0,   172,
       0,   167,   172,     0,     0,     0,  1108,  1059,     0,     0,
       0,   402,     0,     0,     0,   697,     0,     0,   136,   494,
     494,     0,  1135,  1136,     0,     0,   673,     0,     0,   923,
       0,     0,   924,     0,     0,   175,     0,   925,     0,     0,
     176,     0,     0,     0,   154,   176,   154,   196,     0,   154,
       0,     0,     0,     0,     0,     0,     0,   926,     0,     0,
       0,   167,     0,     0,   927,     0,   198,   167,     0,     0,
       0,     0,     0,   154,     0,   928,     0,     0,   136,   371,
       0,   764,     0,   929,   136,   154,     0,     0,     0,     0,
       0,  1058,   172,     0,     0,     0,     0,     0,   172,     0,
     930,     0,     0,     0,     0,     0,   978,     0,     0,     0,
     172,   167,   931,     0,   172,     0,   701,     0,     0,     0,
       0,   176,     0,   932,     0,   277,   507,   507,   136,   933,
     507,   507,   507,     0,   507,   176,     0,     0,     0,     0,
       0,     0,     0,     0,   176,   154,     0,     0,     0,   326,
       0,   154,     0,     0,     0,     0,     0,   176,     0,   167,
       0,   167,     0,     0,     0,     0,   167,     0,     0,   167,
       0,     0,     0,     0,     0,     0,   136,   167,   136,   167,
       0,     0,     0,   136,     0,   172,   136,   172,     0,     0,
     172,     0,   402,     0,   136,   154,   136,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     175,   167,     0,   175,   172,     0,     0,   167,     0,     0,
       0,   167,   167,     0,     0,     0,   172,     0,   136,     0,
       0,     0,     0,     0,   136,   167,     0,     0,   136,   136,
       0,     0,   167,   154,     0,   154,     0,     0,     0,     0,
     154,     0,   136,   154,   167,     0,     0,     0,   167,   136,
       0,   154,     0,   154,     0,     0,     0,     0,     0,     0,
       0,   136,   176,   167,   176,   136,     0,   181,     0,  1300,
       0,     0,     0,     0,     0,     0,   172,     0,     0,     0,
     136,     0,   172,     0,     0,   154,     0,     0,     0,     0,
     181,   154,     0,   175,     0,   154,   154,     0,     0,   175,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   154,
       0,   175,     0,     0,     0,   175,   154,   167,   167,   167,
     167,   167,   167,     0,     0,     0,   172,     0,   154,     0,
       0,     0,   154,     0,   136,   136,   136,   136,   136,   136,
     181,   176,     0,     0,   181,   181,     0,   154,   526,     0,
     528,     0,     0,     0,     0,     0,     0,   532,     0,   431,
       0,     0,     0,     0,     0,     0,     0,   181,     0,     0,
       0,   181,     0,     0,   172,     0,   172,     0,     0,     0,
       0,   172,     0,     0,   172,     0,   175,     0,   175,     0,
       0,   175,   172,     0,   172,     0,     0,   554,     0,     0,
       0,   154,   154,   154,   154,   154,   154,     0,     0,     0,
       0,     0,     0,     0,     0,   175,   230,     0,   432,     0,
       0,     0,   433,     0,     0,     0,   172,   175,     0,     0,
     231,   232,   172,   233,     0,     0,   172,   172,   234,     0,
       0,     0,     0,     0,     0,     0,   235,     0,     0,     0,
     172,     0,   236,     0,     0,     0,   183,   172,   237,     0,
       0,     0,   238,     0,     0,   239,     0,   240,     0,   172,
     435,   436,     0,   172,   438,   439,   241,   440,   441,   183,
     181,     0,   444,   242,   243,     0,     0,   175,   172,   451,
       0,   244,     0,   175,     0,   455,   456,   457,     0,     0,
       0,   245,   184,     0,     0,     0,     0,     0,     0,     0,
     246,   247,     0,   248,     0,   249,   176,   250,     0,   176,
     251,     0,     0,     0,   252,   184,     0,   253,     0,   183,
     254,     0,     0,   183,   183,   181,     0,   175,     0,     0,
     181,     0,   172,   172,   172,   172,   172,   172,     0,     0,
       0,     0,     0,     0,     0,     0,   183,     0,     0,     0,
     183,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   184,     0,     0,     0,   184,
     184,     0,   431,     0,   365,   175,   976,   175,     0,     0,
       0,     0,   175,     0,     0,   175,     0,     0,     0,     0,
       0,     0,   184,   175,     0,   175,   184,     0,     0,   176,
       0,     0,     0,     0,     0,   176,   181,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   176,     0,     0,
     181,   176,     0,     0,     0,     0,     0,   175,     0,   181,
       0,   432,     0,   175,     0,   433,     0,   175,   175,     0,
       0,     0,   181,     0,     0,     0,     0,     0,     0,     0,
       0,   175,     0,     0,     0,     0,     0,     0,   175,   183,
     848,     0,   853,   857,   859,   862,   864,   865,     0,     0,
     175,     0,     0,     0,   175,     0,     0,     0,     0,     0,
       0,     0,     0,   435,   436,     0,     0,     0,   439,   175,
     440,   441,   176,     0,   176,   444,     0,   176,     0,     0,
       0,     0,   451,     0,     0,   184,     0,     0,   455,   456,
     457,     0,     0,     0,   183,     0,     0,     0,     0,   183,
       0,   176,     0,     0,     0,     0,     0,     0,     0,   140,
       0,     0,     0,   176,     0,   320,     0,     0,     0,     0,
       0,     0,     0,   175,   175,   175,   175,   175,   175,     0,
       0,     0,   140,     0,     0,   320,     0,   181,     0,   181,
     184,     0,     0,     0,     0,   184,     0,     0,   361,     0,
       0,   320,    22,    23,     0,     0,     0,     0,     0,     0,
       0,   362,     0,    29,   363,     0,     0,     0,     0,    32,
     878,     0,     0,   176,     0,   183,    37,     0,     0,   176,
       0,     0,   140,     0,     0,     0,     0,     0,     0,   183,
       0,  -331,     0,     0,     0,  -331,  -331,     0,   183,     0,
       0,     0,     0,    54,  -331,    56,  -331,  -331,     0,   140,
       0,   183,  -331,   140,     0,   364,   181,    65,     0,  -331,
       0,   184,  -331,   176,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   184,    81,     0,     0,    83,
       0,     0,    85,  -331,   184,   857,  -331,     0,  -331,     0,
    -331,     0,  -331,  -331,     0,  -331,     0,   184,  -331,     0,
    -331,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   176,     0,   176,     0,     0,     0,   320,   176,  -331,
     429,   176,  -331,     0,  1023,  -331,     0,   430,     0,   176,
       0,   176,     0,     0,  1032,     0,     0,     0,  1037,   431,
    1038,     0,     0,  1041,     0,     0,     0,     0,     0,     0,
    -331,     0,     0,     0,   320,   843,     0,     0,     0,     0,
       0,     0,   140,   176,     0,     0,   183,     0,   183,   176,
       0,     0,     0,   176,   176,  -331,     0,     0,     0,   320,
       0,  -331,     0,     0,     0,     0,     0,   176,     0,     0,
       0,     0,     0,     0,   176,     0,     0,     0,   432,     0,
       0,     0,   433,     0,     0,     0,   176,     0,     0,     0,
     176,     0,   184,     0,   184,     0,     0,   140,     0,   320,
       0,     0,   140,     0,     0,   176,     0,     0,     0,   214,
       0,   181,     0,   434,   181,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   183,     0,     0,     0,     0,
     435,   436,     0,   437,   438,   439,     0,   440,   441,   442,
       0,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,     0,     0,   455,   456,   457,     0,   176,
     176,   176,   176,   176,   176,     0,   458,     0,     0,     0,
       0,   184,     0,     0,     0,     0,     0,     0,   140,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1142,
    1144,     0,   140,     0,     0,  1149,  1152,     0,     0,  1154,
    1156,   140,     0,     0,   181,     0,     0,     0,     0,     0,
     181,     0,     0,     0,   140,     0,     0,     0,     0,     0,
       0,     0,   181,     0,     0,     0,   181,     0,     0,     0,
       0,     0,     0,   320,   320,     0,     0,     0,     0,   320,
       0,     0,   320,   320,   320,   320,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1200,
       0,     0,     0,  1142,  1144,  1149,  1152,     0,     0,   320,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1214,  1215,   181,  1216,   181,
     183,     0,   181,   183,     0,     0,     0,     0,     0,     0,
    1227,     0,     0,  1230,     0,  1231,     0,  1232,     0,     0,
       0,     0,     0,   320,     0,     0,   181,     0,     0,   140,
       0,   140,     0,     0,     0,   320,     0,   320,   181,     0,
       0,     0,     0,     0,     0,   320,   184,     0,     0,   184,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   230,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   231,   232,     0,   233,     0,
       0,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,   235,     0,   183,     0,     0,     0,   236,   181,   183,
       0,     0,     0,   237,   181,     0,     0,   238,   140,     0,
     239,   183,   240,     0,     0,   183,     0,     0,     0,     0,
       0,   241,     0,     0,     0,     0,     0,     0,   242,   243,
       0,     0,     0,     0,     0,     0,   244,     0,     0,   184,
       0,     0,     0,     0,     0,   184,   245,     0,   181,     0,
       0,     0,     0,     0,     0,   246,   247,   184,   248,     0,
     249,   184,   250,     0,     0,   251,     0,     0,     0,   252,
     515,     0,   253,     0,     0,   254,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   183,     0,   183,     0,
       0,   183,     0,     0,     0,     0,   181,     0,   181,     0,
       0,     0,   320,   181,     0,     0,   181,     0,     0,     0,
       0,     0,     0,     0,   181,   183,   181,     0,   320,     0,
     320,     0,     0,     0,     0,     0,     0,   183,     0,     0,
       0,   516,   184,     0,   184,     0,     0,   184,     0,     0,
     320,     0,     0,     0,     0,     0,     0,     0,   181,     0,
       0,     0,     0,     0,   181,     0,     0,     0,   181,   181,
       0,   184,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   181,   184,     0,     0,     0,     0,     0,   181,
       0,     0,     0,   140,     0,     0,   140,   183,     0,     0,
       0,   181,     0,   183,     0,   181,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     181,   107,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,     0,     0,     0,
       0,     0,     0,   184,   226,     0,     0,   183,     0,   184,
       0,     0,     0,     0,     0,   320,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   474,     0,     0,  -669,
       0,  -669,     0,     0,   181,   181,   181,   181,   181,   181,
       0,     0,     0,     0,     0,     0,   140,     0,     0,     0,
       0,     0,   140,   184,   332,   183,   320,   183,     0,     0,
       0,     0,   183,     0,   140,   183,     0,     0,   140,     0,
       0,     0,     0,   183,     0,   183,     0,     0,     0,     0,
       0,   107,     0,     0,     0,   360,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   184,     0,   184,     0,     0,     0,   183,   184,     0,
       0,   184,     0,   183,     0,     0,     0,   183,   183,   184,
       0,   184,   320,     0,     0,     0,     0,     0,     0,     0,
       0,   183,     0,     0,     0,     0,     0,     0,   183,   140,
       0,   140,     0,     0,   140,     0,     0,     0,     0,     0,
     183,     0,     0,   184,   183,     0,     0,     0,     0,   184,
       0,     0,     0,   184,   184,     0,     0,     0,   140,   183,
       0,     0,     0,     0,     0,     0,     0,   184,     0,     0,
     140,     0,     0,     0,   184,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   487,     0,   184,     0,     0,     0,
     184,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   184,     0,     0,     0,     0,
       0,     0,     0,   183,   183,   183,   183,   183,   183,     0,
       0,     0,     0,     0,     0,     0,     0,   320,   320,     0,
     140,     0,     0,   210,     0,     0,   140,   220,     0,   514,
       0,     0,     0,     0,   525,     0,   228,     0,     0,     0,
       0,     0,     0,   262,     0,     0,     0,     0,     0,   184,
     184,   184,   184,   184,   184,   278,     0,   281,   282,     0,
       0,     0,     0,   287,     0,   288,     0,   289,     0,     0,
     140,     0,   315,     0,     0,     0,   228,   323,   325,   327,
     328,   329,     0,     0,     0,   333,     0,   334,     0,     0,
       0,     0,     0,     0,   340,     0,     0,   323,     0,     0,
     343,     0,   344,     0,     0,   345,   346,     0,   347,     0,
     107,     0,   228,   323,   358,     0,     0,     0,   140,     0,
     140,     0,     0,     0,   107,   140,     0,     0,   140,     0,
       0,     0,     0,   107,     0,   361,   140,     0,   140,    22,
      23,   381,     0,   384,     0,     0,   597,     0,   362,     0,
      29,   363,     0,     0,     0,     0,    32,     0,     0,     0,
       0,     0,     0,    37,     0,     0,  -351,     0,     0,     0,
     140,     0,     0,     0,     0,     0,   140,     0,     0,   -68,
     140,   140,     0,     0,   428,     0,     0,  -351,     0,     0,
      54,     0,    56,     0,   140,     0,    60,  -351,   -68,    61,
       0,   140,   364,     0,    65,     0,   431,   480,     0,     0,
       0,   333,   228,   140,     0,     0,     0,   140,     0,     0,
       0,     0,     0,    81,   429,     0,    83,     0,     0,    85,
       0,   430,   140,     0,     0,     0,     0,     0,     0,   323,
       0,     0,     0,   431,   855,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   856,
       0,   755,     0,     0,     0,   432,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,   323,     0,     0,   100,
       0,     0,     0,     0,   365,     0,   140,   140,   140,   140,
     140,   140,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   323,   432,     0,     0,     0,   433,     0,     0,     0,
       0,     0,   329,   333,     0,     0,     0,   435,   436,     0,
     437,   438,   439,     0,   440,   441,   442,   548,   549,   444,
       0,     0,     0,   852,     0,   450,   451,   434,     0,   454,
     107,   323,   455,   456,   457,     0,     0,     0,     0,     0,
       0,     0,     0,   458,   435,   436,     0,   437,   438,   439,
       0,   440,   441,   442,   566,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,     0,     0,   455,
     456,   457,     0,   365,     0,   584,     0,   586,     0,     0,
     458,     0,     0,     0,     0,     0,   596,     0,   601,   603,
     605,   607,   611,     0,     0,     0,     0,   228,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   684,
     685,   686,   688,   690,   691,   276,   692,   693,   694,   695,
     696,     0,   704,   705,   706,   707,   708,   709,   710,   711,
     712,   713,   714,   715,   716,   717,   718,   719,   720,   276,
       0,     0,     0,     0,     0,   323,   323,     0,     0,   337,
     339,   323,     0,     0,   323,   323,   323,   323,   730,   732,
       0,     0,   735,   736,   611,     0,   746,     0,     0,     0,
       0,     0,   749,     0,     0,     0,     0,     0,     0,     0,
       0,   370,     0,     0,   758,   943,     0,   760,   946,     0,
       0,   323,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   771,     0,   774,     0,     0,     0,   281,     0,     0,
     779,   780,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   315,     0,     0,     0,     0,   315,     0,     0,     0,
       0,     0,     0,     0,     0,   323,     0,     0,     0,     0,
       0,     0,     0,     0,   749,     0,     0,   323,     0,   323,
       0,     0,   804,   805,   228,     0,     0,   323,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   836,   836,
       0,     0,     0,     0,     0,     0,     0,     0,   525,     0,
       0,     0,     0,     0,   525,     0,     0,   230,   866,     0,
       0,   489,     0,     0,     0,     0,  1042,     0,   880,     0,
    1047,   231,   232,     0,   233,     0,     0,     0,   315,   234,
       0,     0,     0,     0,     0,     0,     0,   235,     0,     0,
       0,     0,     0,   236,     0,     0,     0,     0,     0,   237,
       0,   503,   506,   238,     0,     0,   239,     0,   240,   513,
       0,     0,     0,     0,     0,     0,     0,   241,   276,     0,
     276,     0,   894,     0,   242,   243,     0,   276,     0,   899,
       0,     0,   244,     0,     0,     0,     0,     0,     0,     0,
       0,  1089,   245,  1090,     0,     0,  1092,     0,     0,     0,
       0,   246,   247,     0,   248,     0,   249,     0,   250,   361,
       0,   251,     0,    22,    23,   252,   542,   276,   253,     0,
    1107,   254,   362,     0,    29,   363,     0,     0,     0,     0,
      32,     0,  1047,   758,   323,     0,     0,    37,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     323,     0,   323,   940,   941,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
    1055,     0,   323,  1056,     0,     0,   364,   543,    65,     0,
       0,     0,     0,     0,     0,     0,   315,     0,     0,     0,
     969,     0,  1158,     0,     0,     0,     0,    81,  1163,     0,
      83,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   584,     0,     0,     0,  1008,     0,
    1010,   836,   836,     0,   343,     0,   344,     0,    89,   346,
     347,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1194,  1022,     0,  1024,     0,   228,     0,  1028,
    1030,     0,     0,   100,     0,  1034,  1036,     0,     0,  1132,
       0,  1040,     0,     0,     0,     0,     0,     0,     0,  1045,
     749,  1048,     0,  1050,  1052,     0,     0,   323,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     525,     0,   525,     0,     0,     0,     0,   525,     0,     0,
     525,     0,     0,     0,     0,     0,     0,     0,  1234,     0,
    1238,     0,  1028,  1078,  1034,  1081,  1083,  1048,   323,     0,
       0,   746,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   688,   730,     0,     0,     0,
       0,     0,  1269,     0,     0,  1091,     0,     0,  1272,     0,
       0,     0,  1234,  1238,     0,     0,     0,     0,     0,     0,
       0,     0,   315,     0,     0,     0,  1106,     0,     0,     0,
       0,     0,   228,     0,     0,     0,     0,     0,     0,     0,
    1114,  1116,  1118,     0,   323,  1287,     0,     0,     0,  1289,
     276,   276,   276,   276,   276,   276,   276,   276,     0,     0,
       0,     0,     0,     0,   107,   836,     0,     0,     0,     0,
       0,     0,     0,  1129,     0,  1130,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1138,     0,  1140,     0,     0,
       0,   746,     0,  1147,     0,     0,     0,     0,     0,     0,
       0,   230,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   231,   232,     0,   233,     0,
       0,     0,     0,   234,     0,     0,  1178,     0,     0,     0,
       0,   235,     0,     0,     0,     0,  1183,   236,     0,  1186,
       0,     0,     0,   237,     0,     0,     0,   238,     0,     0,
     239,     0,   240,     0,     0,     0,     0,     0,     0,   323,
     323,   241,     0,     0,     0,     0,     0,     0,   242,   243,
       0,  1202,     0,     0,     0,     0,   244,     0,     0,     0,
       0,  1207,   836,   836,   836,   836,   245,     0,     0,     0,
       0,     0,     0,     0,     0,   246,   247,     0,   248,     0,
     249,  1219,   250,  1221,     0,   251,     0,     0,  1226,   252,
     948,  1229,   253,     0,     0,   254,     0,     0,     0,  1235,
     431,  1239,     0,     0,  1241,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1219,  1221,   977,  1226,  1229,  1235,
    1239,     0,     0,     0,   276,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   271,  1278,     0,     0,     0,     0,  1279,  1280,   432,
    1281,     0,   746,   433,  1283,  1284,     0,     0,  1285,     0,
       0,     0,     0,  1286,   276,     0,     0,  1288,     0,     0,
       0,     0,     0,     0,   276,     0,  1291,     0,   276,     0,
     276,     0,     0,   276,   434,     0,     0,     0,     0,     0,
       0,  1279,  1301,  1302,  1283,  1303,  1304,  1305,  1306,     0,
       0,   435,   436,     0,   437,   438,   439,     0,   440,   441,
     442,     0,   443,   444,   445,   446,     0,   448,   449,   450,
     451,   452,   453,   454,     0,     0,   455,   456,   457,     0,
       0,     0,     0,     0,     0,     0,     0,   458,     0,  1321,
    1322,  1323,  1324,  1325,  1326,  1321,  1322,  1323,  1324,  1325,
    1326,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,     0,    17,     0,     0,     0,   195,
       0,     0,  1097,     0,   294,     0,     0,     0,     0,     0,
       0,     0,   196,     0,     0,     0,     0,  -215,  -215,     0,
       0,   197,     0,     0,   296,     0,     0,     0,   298,  1112,
       0,     0,     0,   199,     0,     0,    43,    44,     0,     0,
       0,     0,    48,     0,    49,    50,     0,     0,     0,     0,
       0,     0,     0,   299,     0,    55,   300,    57,    58,    59,
       0,     0,     0,     0,    62,     0,   200,     0,   301,     0,
       0,     0,     0,     0,    70,     0,    72,     0,    74,   276,
     276,     0,     0,     0,     0,   276,   276,   304,     0,   276,
     276,    84,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   202,     0,
     103,  -355,  -215,     0,     0,   104,     0,     0,     0,   276,
     632,     0,   633,   276,   276,   276,   276,     0,     0,     0,
       0,   634,     0,     0,     0,     0,   635,   232,   636,   637,
       0,     0,     0,     0,   638,   276,   276,     0,   276,     0,
     506,     0,   235,     0,   196,     0,     0,   506,   236,     0,
     276,     0,     0,   276,   639,   276,     0,   276,   238,     0,
       0,   640,     0,   641,   642,     0,     0,     0,     0,     0,
       0,     0,   643,     0,     0,     0,     0,     0,     0,   242,
     644,     0,     0,     0,     0,     0,     0,   244,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   645,     0,     0,
       0,  1273,     0,     0,     0,     0,   246,   247,     0,   646,
       0,   249,     0,   647,     0,     0,   251,     0,     0,     0,
     648,     0,     0,   253,     0,     0,   649,     0,     0,     0,
       0,     0,     0,     0,     0,   505,   460,   461,   462,   463,
     464,     0,     0,   467,   468,   469,   470,     0,   472,   473,
       0,   650,   651,   652,   653,   654,   655,     0,   656,     0,
       0,     0,   657,   658,   659,   660,   661,   662,   663,   664,
     665,   666,     0,   667,     0,     0,   668,   669,   670,   671,
     506,   506,   672,     0,   506,   506,   506,     0,   506,    -2,
       4,     0,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -215,    21,    22,    23,    24,    25,    26,
       0,  -215,    27,     0,    28,     0,    29,    30,    31,  -215,
    -215,  -215,    32,    33,    34,   -68,     0,    35,    36,    37,
       0,    38,  -351,    39,     0,    40,    41,    42,    43,    44,
       0,    45,    46,    47,    48,   -68,    49,    50,     0,    51,
      52,    53,     0,  -351,     0,     0,    54,    55,    56,    57,
      58,    59,    60,  -351,   -68,    61,    62,    63,    64,     0,
      65,    66,    67,     0,    68,    69,    70,    71,    72,    73,
      74,    75,     0,    76,    77,     0,    78,    79,    80,    81,
      82,     0,    83,    84,   -68,    85,    86,     0,     0,  -215,
       0,    87,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,   100,     0,     0,   101,     0,
     102,     0,   103,     0,  -215,     0,     4,   104,     5,     6,
       7,     8,     9,    10,    11,    12,     0,  -700,     0,    13,
      14,    15,    16,    17,  -700,    18,     0,    19,    20,  -215,
      21,    22,    23,    24,    25,    26,  -700,  -215,    27,  -700,
      28,     0,    29,    30,    31,  -215,  -215,  -215,    32,    33,
      34,   -68,     0,    35,    36,    37,     0,    38,  -351,    39,
       0,    40,    41,    42,    43,    44,     0,    45,    46,    47,
      48,   -68,    49,    50,     0,    51,    52,    53,     0,  -351,
       0,     0,    54,    55,    56,    57,     0,    59,    60,  -351,
     -68,    61,    62,    63,    64,  -700,    65,    66,    67,  -700,
      68,    69,    70,    71,    72,    73,    74,    75,     0,    76,
      77,     0,    78,    79,    80,    81,    82,     0,    83,    84,
     -68,    85,    86,     0,     0,  -215,     0,    87,     0,     0,
    -700,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,  -700,    92,
    -700,  -700,  -700,  -700,  -700,  -700,  -700,     0,  -700,  -700,
    -700,  -700,  -700,  -700,  -700,  -700,  -700,  -700,  -700,  -700,
      99,   100,  -700,  -700,  -700,     0,   102,  -700,   103,     0,
    -215,     0,   348,  -700,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -215,    21,    22,    23,    24,
      25,    26,     0,  -215,    27,     0,    28,     0,    29,    30,
      31,  -215,  -215,  -215,    32,    33,    34,   -68,     0,    35,
      36,    37,     0,    38,  -351,    39,     0,    40,    41,    42,
      43,    44,     0,    45,    46,    47,    48,   -68,    49,    50,
       0,    51,    52,    53,     0,  -351,     0,     0,    54,    55,
      56,    57,    58,    59,    60,  -351,   -68,    61,    62,    63,
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
    -351,    39,     0,    40,    41,    42,    43,    44,     0,    45,
      46,    47,    48,   -68,    49,    50,     0,    51,    52,    53,
       0,  -351,     0,     0,    54,    55,    56,    57,    58,    59,
      60,  -351,   -68,    61,    62,    63,    64,     0,    65,    66,
      67,     0,    68,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,     0,    78,    79,    80,    81,    82,     0,
      83,    84,   -68,    85,    86,     0,     0,  -215,     0,    87,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,   100,     0,     0,   101,     0,   102,   562,
     103,     0,  -215,     0,   579,   104,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,  -215,    21,    22,
      23,    24,    25,    26,     0,  -215,    27,     0,    28,     0,
      29,    30,    31,  -215,  -215,  -215,    32,    33,    34,   -68,
       0,    35,    36,    37,     0,    38,  -351,    39,     0,    40,
      41,    42,    43,    44,     0,    45,    46,    47,    48,   -68,
      49,    50,     0,    51,    52,    53,     0,  -351,     0,     0,
      54,    55,    56,    57,    58,    59,    60,  -351,   -68,    61,
      62,    63,    64,     0,    65,    66,    67,     0,    68,    69,
      70,    71,    72,    73,    74,    75,     0,    76,    77,     0,
      78,    79,    80,    81,    82,     0,    83,    84,   -68,    85,
      86,     0,     0,  -215,     0,    87,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,   100,
       0,     0,   101,     0,   102,   580,   103,     0,  -215,     0,
       4,   104,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -215,    21,    22,    23,    24,    25,    26,
       0,  -215,    27,     0,    28,     0,    29,    30,    31,  -215,
    -215,  -215,    32,    33,    34,   -68,     0,    35,    36,    37,
       0,    38,  -351,    39,     0,    40,    41,    42,    43,    44,
       0,    45,    46,    47,    48,   -68,    49,    50,     0,    51,
      52,    53,     0,  -351,     0,     0,    54,    55,    56,    57,
      58,    59,    60,  -351,   -68,    61,    62,    63,    64,     0,
      65,    66,    67,     0,    68,    69,    70,    71,    72,    73,
      74,    75,     0,    76,    77,     0,    78,    79,    80,    81,
      82,     0,    83,    84,   -68,    85,    86,     0,     0,  -215,
       0,    87,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,   100,     0,     0,   101,     0,
     102,   813,   103,     0,  -215,     0,     4,   104,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -215,
      21,    22,    23,    24,    25,    26,     0,  -215,    27,     0,
      28,     0,    29,    30,    31,  -215,  -215,  -215,    32,    33,
      34,   -68,     0,    35,    36,    37,     0,    38,  -351,    39,
       0,    40,    41,    42,    43,    44,     0,    45,    46,    47,
      48,   -68,    49,    50,     0,    51,    52,    53,     0,  -351,
       0,     0,    54,    55,    56,    57,   359,    59,    60,  -351,
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
      36,    37,     0,    38,  -351,    39,     0,    40,    41,    42,
      43,    44,     0,    45,    46,    47,    48,   -68,    49,    50,
       0,    51,    52,    53,     0,  -351,     0,     0,    54,    55,
      56,    57,    58,    59,    60,  -351,   -68,    61,    62,    63,
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
    -351,    39,     0,    40,    41,    42,    43,    44,     0,    45,
      46,    47,    48,   -68,    49,    50,     0,    51,    52,    53,
       0,  -351,     0,     0,    54,    55,    56,    57,     0,    59,
      60,  -351,   -68,    61,    62,    63,    64,     0,    65,    66,
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
      29,    30,    31,  -215,  -215,  -215,    32,  1027,    34,   -68,
       0,    35,    36,    37,     0,    38,  -351,    39,     0,    40,
      41,    42,    43,    44,     0,    45,    46,    47,    48,   -68,
      49,    50,     0,    51,    52,    53,     0,  -351,     0,     0,
      54,    55,    56,    57,     0,    59,    60,  -351,   -68,    61,
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
    -215,  -215,    32,  1033,    34,   -68,     0,    35,    36,    37,
       0,    38,  -351,    39,     0,    40,    41,    42,    43,    44,
       0,    45,    46,    47,    48,   -68,    49,    50,     0,    51,
      52,    53,     0,  -351,     0,     0,    54,    55,    56,    57,
       0,    59,    60,  -351,   -68,    61,    62,    63,    64,     0,
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
      28,     0,    29,    30,    31,  -215,  -215,  -215,    32,  1218,
      34,   -68,     0,    35,    36,    37,     0,    38,  -351,    39,
       0,    40,    41,    42,    43,    44,     0,    45,    46,    47,
      48,   -68,    49,    50,     0,    51,    52,    53,     0,  -351,
       0,     0,    54,    55,    56,    57,     0,    59,    60,  -351,
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
      31,  -215,  -215,  -215,    32,  1220,    34,   -68,     0,    35,
      36,    37,     0,    38,  -351,    39,     0,    40,    41,    42,
      43,    44,     0,    45,    46,    47,    48,   -68,    49,    50,
       0,    51,    52,    53,     0,  -351,     0,     0,    54,    55,
      56,    57,     0,    59,    60,  -351,   -68,    61,    62,    63,
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
      32,  1225,    34,   -68,     0,    35,    36,    37,     0,    38,
    -351,    39,     0,    40,    41,    42,    43,    44,     0,    45,
      46,    47,    48,   -68,    49,    50,     0,    51,    52,    53,
       0,  -351,     0,     0,    54,    55,    56,    57,     0,    59,
      60,  -351,   -68,    61,    62,    63,    64,     0,    65,    66,
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
      29,    30,    31,  -215,  -215,  -215,    32,  1228,    34,   -68,
       0,    35,    36,    37,     0,    38,  -351,    39,     0,    40,
      41,    42,    43,    44,     0,    45,    46,    47,    48,   -68,
      49,    50,     0,    51,    52,    53,     0,  -351,     0,     0,
      54,    55,    56,    57,     0,    59,    60,  -351,   -68,    61,
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
    -215,  -215,    32,  1233,    34,   -68,     0,    35,    36,    37,
       0,    38,  -351,    39,     0,    40,    41,    42,    43,    44,
       0,    45,    46,    47,    48,   -68,    49,    50,     0,    51,
      52,    53,     0,  -351,     0,     0,    54,    55,    56,    57,
       0,    59,    60,  -351,   -68,    61,    62,    63,    64,     0,
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
      28,     0,    29,    30,    31,  -215,  -215,  -215,    32,  1237,
      34,   -68,     0,    35,    36,    37,     0,    38,  -351,    39,
       0,    40,    41,    42,    43,    44,     0,    45,    46,    47,
      48,   -68,    49,    50,     0,    51,    52,    53,     0,  -351,
       0,     0,    54,    55,    56,    57,     0,    59,    60,  -351,
     -68,    61,    62,    63,    64,     0,    65,    66,    67,     0,
      68,    69,    70,    71,    72,    73,    74,    75,     0,    76,
      77,     0,    78,    79,    80,    81,    82,     0,    83,    84,
     -68,    85,    86,     0,     0,  -215,     0,    87,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,   100,     0,     0,   101,     0,   102,     0,   103,     0,
    -215,     0,   886,   104,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,   295,     0,   196,     0,     0,     0,
      31,  -215,  -215,     0,     0,   197,    34,     0,     0,     0,
     297,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,     0,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,     0,     0,     0,     0,     0,     0,     0,    70,   302,
      72,    73,    74,   303,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   202,     0,   103,     0,  -215,     0,     0,   104,
       5,     6,     7,     8,     9,    10,    11,    12,     0,   634,
       0,    13,     0,     0,    16,    17,   636,    18,     0,   195,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      27,     0,   196,     0,     0,     0,    31,     0,     0,     0,
       0,   197,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   198,   642,   199,     0,     0,    43,    44,     0,     0,
      46,     0,    48,     0,    49,    50,     0,    51,    52,     0,
       0,     0,     0,     0,     0,    55,     0,    57,     0,    59,
       0,     0,     0,     0,    62,    63,   200,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   201,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    88,   505,   460,   461,   462,   463,   464,     0,
       0,   467,   468,   469,   470,     0,   472,   473,     0,   824,
     825,   826,   827,   828,   655,     0,   656,     0,    95,     0,
     657,   658,   659,   660,   661,   662,   663,   829,   665,   666,
      97,   830,    99,   -92,   668,   669,   831,   671,   202,     0,
     103,     0,     0,     0,     0,   104,     5,     6,     7,     8,
       9,    10,    11,    12,     0,   634,     0,    13,     0,     0,
      16,    17,   636,    18,     0,   195,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   196,     0,
       0,     0,    31,     0,     0,     0,     0,   197,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   198,   642,   199,
       0,     0,    43,    44,     0,     0,    46,     0,    48,     0,
      49,    50,     0,    51,    52,     0,     0,     0,     0,     0,
       0,    55,     0,    57,     0,    59,     0,     0,     0,     0,
      62,    63,   200,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   201,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    88,   505,
     460,   461,   462,   463,   464,     0,     0,   467,   468,   469,
     470,     0,   472,   473,     0,   824,   825,   826,   827,   828,
     655,     0,   656,     0,    95,     0,   657,   658,   659,   660,
     661,   662,   663,   829,   665,   666,    97,   830,    99,     0,
     668,   669,   831,   671,   202,     0,   103,     0,     0,     0,
       0,   104,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,     0,    16,    17,     0,    18,
       0,   195,    20,     0,    21,     0,     0,     0,     0,    26,
       0,     0,    27,     0,   196,     0,     0,     0,    31,     0,
       0,     0,     0,    33,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   198,     0,   199,     0,     0,    43,    44,
       0,    45,    46,    47,    48,     0,    49,    50,     0,    51,
      52,    53,     0,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,     0,     0,     0,    62,    63,   200,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,    73,
      74,    75,     0,    76,    77,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     102,     0,   103,     0,     0,     0,     0,   104,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,   379,    23,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   198,
       0,   199,     0,     0,    43,    44,     0,     0,    46,     0,
      48,     0,    49,    50,     0,    51,    52,     0,     0,     0,
       0,     0,     0,    55,    56,    57,     0,    59,     0,     0,
       0,     0,    62,    63,   200,     0,    65,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,   201,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,     0,    84,
       0,    85,    86,     0,     0,     0,     0,     0,     0,     0,
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
     200,  -744,     0,     0,     0,  -744,     0,     0,    70,    71,
      72,    73,    74,   201,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,  -393,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,  -393,     0,     0,
     101,     0,   202,  -393,   103,     0,     0,     0,     0,   104,
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
       0,    76,     0,     0,    78,   317,   318,     0,    82,   353,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,   354,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   202,     0,
     103,     0,     0,     0,     0,   104,     5,     6,     7,     8,
       9,   382,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,    19,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   196,     0,
       0,     0,    31,  -215,  -215,     0,     0,   197,    34,     0,
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
       0,   195,     0,     0,    21,     0,   739,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,     0,
       0,     0,     0,   197,    34,     0,   740,     0,    36,     0,
       0,    38,     0,   198,     0,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,     0,    51,
      52,     0,     0,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,     0,     0,     0,    62,    63,   200,     0,
     741,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   201,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,   742,    86,     0,     0,     0,
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
       0,     0,    78,   317,   318,     0,    82,   353,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,   800,
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
     318,     0,    82,   353,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   202,     0,   103,   802,     0,     0,     0,   104,
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
      97,    98,    99,   319,     0,     0,   101,     0,   202,     0,
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
      78,   317,   318,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   202,     0,   103,  -545,     0,     0,
       0,   104,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   195,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,  -215,
    -215,     0,     0,   197,    34,     0,     0,     0,    36,     0,
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
     202,     0,   103,     0,  -215,     0,     0,   104,     5,     6,
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
      99,     0,     0,     0,   101,     0,   202,     0,   103,     0,
       0,  -545,     0,   104,     5,     6,     7,     8,     9,    10,
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
     101,     0,   202,     0,   103,   808,     0,     0,     0,   104,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   195,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      27,     0,   196,     0,     0,     0,    31,     0,     0,     0,
       0,   197,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   198,     0,   199,     0,     0,    43,    44,     0,     0,
      46,     0,    48,     0,    49,    50,   956,    51,    52,     0,
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
       0,  -401,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,  -401,
       0,     0,   101,     0,   202,  -401,   103,     0,     0,     0,
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
       0,    90,    91,    92,    93,    94,     0,  -386,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,  -386,     0,     0,   101,     0,
     202,  -386,   103,     0,     0,     0,     0,   104,     5,     6,
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
      93,    94,     0,  -394,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,  -394,     0,     0,   101,     0,   202,  -394,   103,     0,
       0,     0,     0,   104,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,   259,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,     0,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,   201,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,     0,    84,     0,   260,    86,     0,
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
     103,     0,     0,     0,     0,   104,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   195,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   196,     0,
       0,     0,    31,     0,     0,     0,     0,   197,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   198,     0,   199,
       0,     0,    43,    44,     0,     0,    46,     0,    48,     0,
      49,    50,     0,    51,    52,     0,     0,     0,     0,     0,
       0,    55,     0,    57,     0,    59,     0,     0,     0,     0,
      62,    63,   200,  -744,     0,     0,     0,  -744,     0,     0,
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
       0,     0,     0,   197,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   198,     0,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,     0,    51,
      52,     0,     0,     0,     0,     0,     0,    55,   598,    57,
       0,    59,     0,     0,     0,     0,    62,    63,   200,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   201,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,   599,     0,    88,     0,     0,     0,     0,     0,
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
       0,     0,    86,     0,     0,     0,     0,     0,   599,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,     0,
       0,   608,     0,   104,     5,     6,     7,     8,     9,    10,
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
       0,     0,     0,    90,    91,    92,    93,    94,     0,  -422,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   202,     0,   103,  -422,     0,     0,     0,   104,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   195,
       0,     0,    21,     0,     0,     0,     0,     0,     0,   275,
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
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   102,     0,
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
      86,     0,     0,     0,     0,     0,   599,     0,    88,     0,
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
      74,   201,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     202,     0,   103,     0,     0,   608,     0,   104,     5,     6,
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
      99,     0,     0,     0,   101,     0,   202,   798,   103,     0,
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
       0,     0,    82,  1009,     0,    84,     0,     0,    86,     0,
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
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   202,     0,
     103,     0,     0,  1046,     0,   104,     5,     6,     7,     8,
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
       0,     0,   101,     0,   202,     0,   103,     0,     0,  1051,
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
      82,  1206,     0,    84,     0,     0,    86,     0,     0,     0,
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
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,     0,
       0,     0,     0,   104,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,   295,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
     297,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,     0,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,     0,     0,     0,     0,     0,     0,     0,    70,   302,
      72,    73,    74,   303,     0,    76,     0,     0,    78,     0,
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
       0,  1076,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   198,     0,   199,     0,     0,    43,    44,     0,     0,
      46,     0,    48,     0,    49,    50,     0,    51,    52,     0,
       0,     0,     0,     0,     0,    55,     0,    57,     0,    59,
       0,     0,     0,     0,    62,    63,   200,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   201,
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
       0,     0,    31,     0,     0,     0,     0,  1079,    34,     0,
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
       0,     0,     0,  1262,    34,     0,     0,     0,    36,     0,
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
     196,     0,     0,     0,    31,     0,     0,     0,     0,  1263,
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
      31,     0,     0,     0,     0,  1265,    34,     0,     0,     0,
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
       0,  1266,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   198,     0,   199,     0,     0,    43,    44,     0,     0,
      46,     0,    48,     0,    49,    50,     0,    51,    52,     0,
       0,     0,     0,     0,     0,    55,     0,    57,     0,    59,
       0,     0,     0,     0,    62,    63,   200,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,   201,
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
       0,     0,    31,     0,     0,     0,     0,  1267,    34,     0,
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
       0,     0,     0,  1268,    34,     0,     0,     0,    36,     0,
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
      95,     6,     7,     8,     9,    10,    11,    12,     0,    96,
       0,    13,    97,    98,    99,    17,     0,     0,   101,   195,
     202,     0,   103,     0,   294,     0,     0,   104,     0,     0,
       0,     0,   196,     0,     0,     0,     0,  -215,  -215,     0,
       0,   197,     0,     0,   296,     0,     0,     0,   298,     0,
       0,     0,     0,   199,     0,     0,    43,    44,     0,     0,
       0,     0,    48,     0,    49,    50,     0,     0,     0,     0,
       0,     0,     0,   299,     0,    55,   300,    57,    58,    59,
       0,     0,     0,     0,    62,     0,   200,     0,   301,     0,
       0,     0,     0,     0,    70,     0,    72,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,   304,     0,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   202,     0,
     103,     0,  -215,     0,     0,   104,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   195,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   196,     0,
       0,     0,    31,  -215,  -215,     0,     0,   197,    34,     0,
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
       0,     0,   101,     0,   202,     0,   103,     0,  -215,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,  -215,  -215,     0,     0,
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
       0,     0,     0,    31,  -215,  -215,     0,     0,   197,    34,
       0,    99,     0,    36,     0,     0,    38,   202,   198,   103,
     199,  -215,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   201,     0,    76,     0,
     697,    78,     0,     0,     0,    82,     0,     0,    84,   634,
       0,    86,     0,     0,   988,   989,   636,   990,     0,     0,
       0,     0,   991,     0,     0,     0,     0,     0,     0,     0,
     699,     0,     0,     0,     0,     0,    90,     0,    92,     0,
       0,     0,   992,     0,     0,     0,   993,     0,     0,   994,
       0,   198,   642,     0,     0,     0,     0,     0,     0,    99,
     700,     0,     0,     0,     0,     0,     0,   103,   995,  -215,
       0,     0,     0,     0,     0,   996,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   997,     0,     0,     0,     0,
       0,     0,     0,     0,   998,   999,     0,  1000,     0,  1001,
       0,   701,     0,     0,     0,     0,     0,   702,  1002,     0,
       0,  1003,     0,     0,  1004,     0,     0,     0,     0,     0,
       0,     0,     0,   505,   460,   461,   462,   463,   464,     0,
       0,   467,   468,   469,   470,     0,   472,   473,     0,   650,
     651,   652,   653,   654,   655,     0,   656,     0,     0,     0,
     657,   658,   659,   660,   661,   662,   663,   664,   665,   666,
     230,   667,     0,     0,   668,   669,   670,   671,  1005,   634,
       0,     0,     0,     0,   231,   232,   636,   233,     0,     0,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
     235,     0,     0,     0,     0,     0,   236,     0,     0,     0,
       0,     0,   237,     0,     0,     0,   238,     0,     0,   239,
       0,   240,   642,     0,     0,     0,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,   242,   243,     0,
       0,     0,     0,     0,     0,   244,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   245,     0,     0,     0,     0,
       0,   429,     0,     0,   246,   247,     0,   248,   430,   249,
       0,   250,     0,     0,   251,     0,     0,     0,   252,     0,
     431,   253,     0,     0,   254,     0,     0,     0,     0,     0,
       0,     0,     0,   505,   460,   461,   462,   463,   464,     0,
       0,   467,   468,   469,   470,     0,   472,   473,     0,   650,
     651,   652,   653,   654,   655,     0,   656,     0,     0,     0,
     657,   658,   659,   660,   661,   662,   663,   664,   665,   666,
       0,   667,     0,     0,   668,   669,   670,   671,     0,   432,
       0,   429,     0,   433,     0,     0,     0,     0,   430,     0,
       0,     0,   504,     0,     0,     0,     0,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,   505,   460,   461,   462,   463,
     464,     0,     0,   467,   468,   469,   470,     0,   472,   473,
       0,   435,   436,     0,   437,   438,   439,     0,   440,   441,
     442,     0,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,     0,     0,   455,   456,   457,   432,
     365,   429,     0,   433,     0,     0,     0,   458,   430,     0,
       0,     0,  1217,     0,     0,     0,     0,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,   505,   460,   461,   462,   463,
     464,     0,     0,   467,   468,   469,   470,     0,   472,   473,
       0,   435,   436,     0,   437,   438,   439,     0,   440,   441,
     442,     0,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,     0,     0,   455,   456,   457,   432,
     365,   429,     0,   433,     0,     0,     0,   458,   430,     0,
       0,     0,  1224,     0,     0,     0,     0,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,   505,   460,   461,   462,   463,
     464,     0,     0,   467,   468,   469,   470,     0,   472,   473,
       0,   435,   436,     0,   437,   438,   439,     0,   440,   441,
     442,     0,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,     0,     0,   455,   456,   457,   432,
     365,   429,     0,   433,     0,     0,     0,   458,   430,     0,
       0,     0,  1309,     0,     0,     0,     0,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,   505,   460,   461,   462,   463,
     464,     0,     0,   467,   468,   469,   470,     0,   472,   473,
       0,   435,   436,     0,   437,   438,   439,     0,   440,   441,
     442,     0,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,     0,     0,   455,   456,   457,   432,
     365,   429,     0,   433,     0,     0,     0,   458,   430,     0,
       0,     0,  1310,     0,     0,     0,     0,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,   505,   460,   461,   462,   463,
     464,     0,     0,   467,   468,   469,   470,     0,   472,   473,
       0,   435,   436,     0,   437,   438,   439,     0,   440,   441,
     442,     0,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,     0,     0,   455,   456,   457,   432,
     365,   429,     0,   433,     0,     0,     0,   458,   430,     0,
       0,     0,  1311,     0,     0,     0,     0,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,   505,   460,   461,   462,   463,
     464,     0,     0,   467,   468,   469,   470,     0,   472,   473,
       0,   435,   436,     0,   437,   438,   439,     0,   440,   441,
     442,     0,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,     0,     0,   455,   456,   457,   432,
     365,   429,     0,   433,     0,     0,     0,   458,   430,     0,
       0,     0,  1312,     0,     0,     0,     0,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,   505,   460,   461,   462,   463,
     464,     0,     0,   467,   468,   469,   470,     0,   472,   473,
       0,   435,   436,     0,   437,   438,   439,     0,   440,   441,
     442,     0,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,     0,     0,   455,   456,   457,   432,
     365,   429,     0,   433,     0,     0,     0,   458,   430,     0,
       0,     0,  1313,     0,     0,     0,     0,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,   505,   460,   461,   462,   463,
     464,     0,     0,   467,   468,   469,   470,     0,   472,   473,
       0,   435,   436,     0,   437,   438,   439,     0,   440,   441,
     442,     0,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,     0,     0,   455,   456,   457,   432,
     365,     0,     0,   433,     0,     0,   429,   458,     0,     0,
       0,     0,  1314,   430,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   431,   275,     0,     0,     0,
       0,     0,     0,     0,   434,   505,   460,   461,   462,   463,
     464,   861,     0,   467,   468,   469,   470,     0,   472,   473,
       0,   435,   436,     0,   437,   438,   439,     0,   440,   441,
     442,     0,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,     0,     0,   455,   456,   457,     0,
     365,     0,   429,     0,   432,     0,     0,   458,   433,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   431,   275,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   852,     0,   981,     0,   434,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   435,   436,     0,   437,
     438,   439,     0,   440,   441,   442,     0,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,     0,
     432,   455,   456,   457,   433,   365,     0,     0,     0,     0,
       0,     0,   458,     0,   429,     0,     0,     0,     0,     0,
       0,   430,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   852,     0,   431,     0,   434,     0,     0,   588,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   435,   436,     0,   437,   438,   439,     0,   440,
     441,   442,     0,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   589,     0,   455,   456,   457,
       0,   365,     0,     0,     0,     0,     0,     0,   458,     0,
       0,     0,   432,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   429,   820,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   431,     0,   434,     0,     0,
     821,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   435,   436,     0,   437,   438,   439,
       0,   440,   441,   442,     0,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   822,   590,   455,
     456,   457,     0,     0,     0,     0,     0,     0,     0,     0,
     458,     0,   429,     0,   432,     0,     0,     0,   433,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   431,   849,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   850,     0,   434,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   435,   436,     0,   437,
     438,   439,     0,   440,   441,   442,     0,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,     0,
     432,   455,   456,   457,   433,     0,     0,   429,     0,     0,
       0,     0,   458,     0,   430,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   431,   275,     0,     0,
       0,     0,     0,     0,     0,   434,     0,     0,     0,     0,
       0,     0,   980,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   435,   436,     0,   437,   438,   439,     0,   440,
     441,   442,     0,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,     0,     0,   455,   456,   457,
       0,   365,     0,   429,     0,   432,     0,     0,   458,   433,
     430,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   431,  1151,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   435,   436,     0,
     437,   438,   439,     0,   440,   441,   442,     0,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
       0,   432,   455,   456,   457,   433,   365,     0,   429,     0,
       0,     0,     0,   458,     0,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   431,   275,     0,
       0,     0,   852,     0,     0,     0,   434,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,     0,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,     0,     0,   455,   456,
     457,     0,   365,     0,   429,     0,   432,     0,     0,   458,
     433,   430,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   431,   275,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   852,     0,     0,
       0,   434,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   435,   436,
       0,   437,   438,   439,     0,   440,   441,   442,     0,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,     0,   432,   455,   456,   457,   433,   365,     0,   429,
       0,     0,     0,     0,   458,     0,   430,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   431,   912,
       0,     0,     0,     0,     0,     0,     0,   434,     0,     0,
       0,     0,     0,     0,   913,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   435,   436,     0,   437,   438,   439,
       0,   440,   441,   442,     0,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,     0,     0,   455,
     456,   457,     0,   365,     0,   429,     0,   432,     0,     0,
     458,   433,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   431,   914,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     915,     0,   434,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,   435,
     436,     0,   437,   438,   439,   431,   440,   441,   442,     0,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,     0,   432,   455,   456,   457,   433,     0,     0,
       0,   429,     0,     0,     0,   458,     0,     0,   430,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,  1143,     0,     0,     0,     0,     0,     0,   434,     0,
       0,     0,     0,     0,   432,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,   435,   436,   947,   437,   438,
     439,     0,   440,   441,   442,     0,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,     0,   434,
     455,   456,   457,     0,     0,     0,     0,     0,     0,   432,
       0,   458,     0,   433,     0,     0,   435,   436,     0,   437,
     438,   439,     0,   440,   441,   442,     0,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   429,
       0,   455,   456,   457,   434,   365,   430,     0,     0,     0,
       0,     0,   458,     0,     0,     0,     0,     0,   431,     0,
       0,   435,   436,  1124,   437,   438,   439,     0,   440,   441,
     442,     0,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   429,     0,   455,   456,   457,     0,
     365,   430,     0,     0,     0,     0,     0,   458,     0,     0,
    1125,     0,     0,   431,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,   430,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   431,     0,
       0,     0,   434,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   432,     0,     0,     0,   433,     0,     0,   435,
     436,     0,   437,   438,   439,     0,   440,   441,   442,     0,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,     0,   852,   455,   456,   457,   434,     0,     0,
       0,     0,     0,     0,     0,   458,     0,   432,     0,     0,
       0,   433,     0,     0,   435,   436,     0,   437,   438,   439,
       0,   440,   441,   442,     0,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,     0,   852,   455,
     456,   457,   434,     0,     0,     0,   429,   529,  1161,     0,
     458,     0,     0,   430,     0,     0,     0,     0,     0,   435,
     436,     0,   437,   438,   439,   431,   440,   441,   442,     0,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,     0,     0,   455,   456,   457,   429,     0,     0,
       0,     0,     0,  1204,   430,   458,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   432,     0,     0,   429,   433,     0,
       0,     0,     0,     0,   430,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
       0,     0,     0,     0,     0,   432,     0,   429,     0,   433,
       0,     0,     0,     0,   430,     0,   435,   436,     0,   437,
     438,   439,     0,   440,   441,   442,   431,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,     0,
     434,   455,   456,   457,     0,   432,     0,     0,   429,   433,
       0,     0,   458,     0,     0,   430,     0,   435,   436,     0,
     437,   438,   439,     0,   440,   441,   442,   431,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     434,     0,   455,   456,   457,   432,   553,     0,     0,   433,
       0,     0,     0,   458,     0,     0,     0,   435,   436,     0,
     437,   438,   439,     0,   440,   441,   442,     0,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     434,   557,   455,   456,   457,     0,   432,     0,   429,   816,
     433,     0,     0,   458,     0,   430,     0,   435,   436,   733,
     437,   438,   439,     0,   440,   441,   442,   431,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
       0,   434,   455,   456,   457,     0,     0,     0,     0,   799,
       0,     0,     0,   458,     0,     0,     0,     0,   435,   436,
       0,   437,   438,   439,     0,   440,   441,   442,     0,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,     0,     0,   455,   456,   457,   432,     0,   429,     0,
     433,     0,     0,     0,   458,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   434,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   435,   436,
       0,   437,   438,   439,     0,   440,   817,   442,     0,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,     0,     0,   455,   456,   457,   432,     0,   429,     0,
     433,     0,     0,     0,   458,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   431,     0,     0,
     945,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     873,   434,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,   430,     0,   435,   436,
       0,   437,   438,   439,     0,   440,   441,   442,   431,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,     0,     0,   455,   456,   457,   432,     0,     0,   429,
     433,     0,     0,     0,   458,     0,   430,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   431,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   434,     0,     0,     0,     0,     0,   432,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,   435,   436,
       0,   437,   438,   439,     0,   440,   441,   442,     0,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,     0,   434,   455,   456,   457,     0,   432,     0,     0,
       0,   433,     0,     0,   458,     0,     0,     0,     0,   435,
     436,     0,   437,   438,   439,     0,   440,   441,   442,     0,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   434,     0,   455,   456,   457,     0,     0,     0,
       0,   959,     0,     0,     0,   458,     0,     0,     0,   435,
     436,     0,   437,   438,   439,     0,   440,   441,   442,     0,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   429,  1015,   455,   456,   457,     0,     0,   430,
       0,   960,     0,     0,     0,   458,     0,     0,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     432,     0,   429,  1121,   433,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   434,     0,     0,     0,     0,
     432,     0,     0,     0,   433,     0,     0,     0,     0,     0,
       0,     0,   435,   436,     0,   437,   438,   439,     0,   440,
     441,   442,     0,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   434,     0,   455,   456,   457,
     432,     0,     0,   429,   433,     0,     0,     0,   458,     0,
     430,     0,   435,   436,     0,   437,   438,   439,     0,   440,
     441,   442,   431,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   434,     0,   455,   456,   457,
       0,     0,     0,   429,  1070,     0,     0,     0,   458,     0,
     430,     0,   435,   436,     0,   437,   438,   439,     0,   440,
     441,   442,   431,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,     0,     0,   455,   456,   457,
       0,   432,     0,     0,     0,   433,     0,     0,   458,   429,
       0,     0,     0,     0,     0,     0,   430,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   431,     0,
       0,     0,   214,     0,     0,     0,   434,     0,     0,     0,
       0,   432,     0,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,     0,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   434,     0,   455,   456,
     457,     0,     0,     0,     0,     0,     0,   432,     0,   458,
       0,   433,     0,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,     0,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   429,  1137,   455,   456,
     457,  1164,   434,   430,     0,     0,     0,     0,     0,   458,
       0,     0,     0,     0,     0,   431,  1185,     0,     0,   435,
     436,     0,   437,   438,   439,     0,   440,   441,   442,     0,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   429,     0,   455,   456,   457,     0,     0,   430,
       0,     0,     0,     0,     0,   458,     0,     0,     0,     0,
       0,   431,  1188,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   432,     0,     0,     0,   433,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,     0,   434,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     432,     0,     0,     0,   433,     0,   435,   436,     0,   437,
     438,   439,     0,   440,   441,   442,     0,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,     0,
       0,   455,   456,   457,     0,   434,     0,     0,     0,     0,
     432,     0,   458,   429,   433,     0,     0,     0,     0,     0,
     430,     0,   435,   436,     0,   437,   438,   439,     0,   440,
     441,   442,   431,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   434,     0,   455,   456,   457,
       0,     0,     0,   429,  1212,     0,     0,     0,   458,     0,
     430,     0,   435,   436,     0,   437,   438,   439,     0,   440,
     441,   442,   431,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,     0,     0,   455,   456,   457,
       0,   432,     0,   429,     0,   433,  1190,     0,   458,     0,
     430,     0,     0,     0,  1261,     0,     0,     0,     0,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   434,     0,     0,     0,
       0,   432,     0,     0,   429,   433,     0,     0,     0,     0,
       0,   430,  1254,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,   431,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   434,     0,   455,   456,
     457,   432,     0,     0,     0,   433,     0,     0,     0,   458,
       0,     0,     0,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,     0,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   434,     0,   455,   456,
     457,     0,   432,     0,   429,     0,   433,     0,     0,   458,
       0,   430,     0,   435,   436,  1264,   437,   438,   439,     0,
     440,   441,   442,   431,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,     0,   434,   455,   456,
     457,     0,     0,     0,     0,   429,     0,     0,     0,   458,
       0,     0,   430,     0,   435,   436,     0,   437,   438,   439,
       0,   440,   441,   442,   431,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,     0,     0,   455,
     456,   457,   432,     0,     0,     0,   433,     0,     0,     0,
     458,     0,     0,     0,     0,  1315,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   434,     0,     0,
       0,   429,     0,   432,     0,     0,     0,   433,   430,     0,
       0,     0,     0,     0,   435,   436,  1316,   437,   438,   439,
     431,   440,   441,   442,     0,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,     0,   434,   455,
     456,   457,   429,     0,     0,     0,     0,     0,     0,   430,
     458,     0,     0,     0,     0,   435,   436,     0,   437,   438,
     439,   431,   440,   441,   442,     0,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,     0,   432,
     455,   456,   457,   433,     0,     0,     0,   429,     0,     0,
       0,   458,  1317,     0,   430,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,   434,     0,     0,     0,     0,     0,
     432,     0,     0,     0,   433,     0,     0,     0,     0,     0,
       0,   435,   436,  1318,   437,   438,   439,     0,   440,   441,
     442,     0,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,     0,   434,   455,   456,   457,     0,
       0,     0,     0,   429,     0,   432,     0,   458,     0,   433,
     430,     0,   435,   436,     0,   437,   438,   439,  1319,   440,
     441,   442,   431,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,     0,     0,   455,   456,   457,
     434,     0,     0,     0,   429,     0,     0,     0,   458,     0,
       0,   430,     0,     0,     0,     0,     0,   435,   436,     0,
     437,   438,   439,   431,   440,   441,   442,     0,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
       0,   432,   455,   456,   457,   433,     0,     0,     0,   429,
       0,     0,     0,   458,  1320,     0,   430,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   431,     0,
       0,     0,     0,     0,     0,     0,   434,     0,     0,     0,
       0,     0,   432,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,     0,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,     0,   434,   455,   456,
     457,     0,     0,     0,     0,     0,     0,   935,     0,   458,
       0,   433,     0,     0,   435,   436,     0,   437,   438,   439,
       0,   440,   441,   442,     0,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,     0,     0,   455,
     456,   457,   434,     0,     0,     0,     0,     0,     0,     0,
     458,     0,     0,     0,     0,     0,     0,     0,     0,   435,
     436,     0,   437,   438,   439,     0,   440,   441,   442,     0,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,     0,   361,   455,   456,   457,    22,    23,     0,
       0,     0,     0,     0,     0,   458,   362,     0,    29,   363,
       0,   431,     0,     0,    32,     0,     0,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,  1055,     0,     0,  1056,     0,     0,
     364,     0,    65,     0,     0,     0,     0,     0,     0,     0,
     432,     0,     0,     0,   433,     0,     0,     0,     0,     0,
       0,    81,     0,     0,    83,     0,     0,    85,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,     0,     0,
     432,     0,     0,     0,   433,   434,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   435,   436,     0,   437,   438,   439,     0,   440,
     441,   442,     0,   443,   444,   445,   446,   100,   448,   449,
     450,   451,   452,  1213,   454,     0,     0,   455,   456,   457,
     432,     0,   435,   436,   433,   437,   438,   439,   458,   440,
     441,   442,     0,   443,   444,   445,   446,     0,   448,   449,
     450,   451,   452,     0,   454,     0,   361,   455,   456,   457,
      22,    23,     0,     0,     0,     0,     0,     0,   458,   362,
       0,    29,   363,     0,     0,     0,     0,    32,     0,     0,
       0,     0,   435,   436,    37,   437,   438,   439,     0,   440,
     441,   442,     0,     0,   444,   445,   446,     0,   448,   449,
     450,   451,     0,     0,   454,     0,     0,   455,   456,   457,
       0,    54,     0,    56,     0,   359,     0,  1055,   458,     0,
    1056,     0,     0,   364,   361,    65,     0,     0,    22,    23,
       0,     0,     0,     0,     0,     0,     0,   362,     0,    29,
     363,     0,     0,     0,    81,    32,     0,    83,     0,     0,
      85,     0,    37,     0,   400,     0,   230,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     231,   232,     0,   233,     0,    89,     0,     0,   234,    54,
       0,    56,     0,     0,     0,  1055,   235,     0,  1056,     0,
       0,   364,   236,    65,     0,     0,     0,     0,   237,     0,
     100,     0,   238,     0,     0,   239,     0,   240,     0,     0,
       0,     0,    81,     0,     0,    83,   241,     0,    85,     0,
       0,     0,     0,   242,   243,     0,     0,     0,     0,     0,
       0,   244,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   245,     0,    89,   230,     0,     0,     0,     0,     0,
     246,   247,     0,   248,     0,   249,     0,   250,   231,   232,
     251,   233,     0,     0,   252,     0,   234,   253,   100,     0,
     254,     0,     0,     0,   235,     0,     0,     0,     0,     0,
     236,     0,     0,     0,     0,     0,   237,     0,     0,     0,
     238,     0,     0,   239,    89,   240,     0,     0,     0,     0,
     230,     0,     0,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,     0,   231,   232,     0,   233,     0,   244,
       0,     0,   234,     0,    23,     0,     0,     0,     0,   245,
     235,     0,     0,     0,     0,     0,   236,     0,   246,   247,
       0,   248,   237,   249,     0,   250,   238,     0,   251,   239,
       0,   240,   252,     0,     0,   253,   230,     0,   254,     0,
     241,     0,     0,     0,     0,     0,     0,   242,   243,     0,
     231,   232,     0,   233,     0,   244,    56,     0,   234,     0,
       0,     0,    89,     0,     0,   245,   235,     0,    65,     0,
       0,     0,   236,     0,   246,   247,     0,   248,   237,   249,
       0,   250,   238,     0,   251,   239,     0,   240,   252,     0,
       0,   253,  -326,    85,   254,     0,   241,     0,     0,     0,
       0,     0,     0,   242,   243,     0,  -326,  -326,     0,  -326,
       0,   244,     0,     0,  -326,     0,     0,     0,     0,     0,
       0,   245,  -326,     0,     0,     0,     0,     0,  -326,     0,
     246,   247,     0,   248,  -326,   249,     0,   250,  -326,     0,
     251,  -326,     0,  -326,   252,   542,     0,   253,   230,     0,
     254,     0,  -326,     0,     0,     0,     0,     0,     0,  -326,
    -326,     0,   231,   232,     0,   233,     0,  -326,     0,     0,
     234,     0,     0,     0,     0,     0,     0,  -326,   235,     0,
       0,     0,     0,     0,   236,     0,  -326,  -326,     0,  -326,
     237,  -326,     0,  -326,   238,     0,  -326,   239,     0,   240,
    -326,     0,     0,  -326,  -327,     0,  -326,     0,   241,     0,
       0,     0,     0,     0,     0,   242,   243,     0,  -327,  -327,
       0,  -327,     0,   244,     0,     0,  -327,     0,     0,     0,
       0,     0,     0,   245,  -327,     0,     0,     0,     0,     0,
    -327,     0,   246,   247,     0,   248,  -327,   249,     0,   250,
    -327,     0,   251,  -327,     0,  -327,   252,     0,     0,   253,
       0,     0,   254,     0,  -327,     0,     0,     0,     0,     0,
       0,  -327,  -327,     0,     0,     0,     0,     0,     0,  -327,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -327,
       0,     0,     0,     0,     0,     0,     0,     0,  -327,  -327,
       0,  -327,     0,  -327,     0,  -327,     0,     0,  -327,     0,
       0,     0,  -327,     0,     0,  -327,     0,     0,  -327
};

static const yytype_int16 yycheck[] =
{
       2,    20,    62,   551,   486,   112,     2,   181,   152,   165,
     266,   401,   588,    62,   421,    62,    67,     2,   511,   555,
     675,    26,   677,    25,   223,   628,   788,   625,   879,   133,
     822,   546,   383,   817,   840,   291,    87,   921,     1,     1,
      25,   577,   782,    32,    32,   406,   961,    47,    56,   521,
       1,    35,   103,    32,   389,   390,    32,   392,  1179,    32,
     516,    66,    32,    24,    32,    90,     0,   121,   885,    94,
      47,    32,    22,    75,    18,   331,    47,    79,    80,    40,
      41,    42,     1,    61,     2,   121,   103,   543,   262,    56,
      75,    40,    41,    24,    79,    80,    47,   432,   433,   166,
     102,    32,    83,   121,   106,    86,   173,    25,    24,    40,
      41,    42,    75,    32,    91,    32,    32,   102,     1,    56,
      93,   106,   176,  1244,    40,    41,    42,   111,   602,   229,
     604,   605,   121,    46,    78,   609,   610,    75,    28,    89,
     176,   119,    61,   478,   479,   121,   963,  1031,   148,    32,
      40,    41,    42,    97,   171,   118,   628,    75,   176,   120,
     148,    79,    80,   113,   126,   173,   266,  1018,     3,   148,
     114,   769,    85,   166,    93,   126,    93,   148,    61,   540,
     118,    47,   171,   171,   102,   147,   148,   106,   106,   120,
    1105,   291,   171,   955,   121,   171,   147,   148,   171,   961,
     119,   171,   115,   171,   120,   176,   173,  1058,  1059,   172,
      93,  1017,   174,   215,   175,   166,   752,    47,   124,    54,
     476,   477,    47,   106,   964,     2,   175,    56,   147,  1013,
     215,   331,   119,     3,   172,    36,   119,    31,   573,   574,
     591,   734,    56,    27,   175,   821,    53,   421,    25,   166,
       3,   148,   171,    27,   171,   121,   141,     3,    93,   175,
     150,   148,   152,   148,   147,   171,    91,    74,   270,   104,
     166,   786,    73,   275,   164,   148,   172,    84,   382,   176,
    1131,   142,   148,   166,    54,   270,   148,   172,   171,   150,
     275,   121,    76,   148,   401,    79,    90,   215,    75,   172,
      94,    54,    79,    80,   148,    79,    56,    91,    54,   199,
     176,   110,   174,    50,   166,  1166,    53,    91,   148,   174,
     148,   173,    91,   175,  1086,   102,   110,    27,   172,   106,
     529,  1123,   806,  1125,   490,   148,   934,    74,  1222,   148,
      77,   441,   116,  1105,   172,   405,   176,    84,   142,   351,
     173,   104,   270,   147,    27,   351,   150,   275,   104,   962,
     154,   174,    46,   365,   126,   174,   351,   148,   148,   259,
     260,    90,   374,   110,    47,    94,   476,   477,    51,    79,
     365,   271,    66,     3,   877,   387,  1173,   392,    56,   374,
     152,    91,   397,   174,   174,   594,   171,    11,   173,   166,
      20,    85,   387,    76,   148,   172,    79,    56,    81,   148,
     110,   511,   173,    33,   465,   466,  1071,  1072,    91,   421,
     471,   311,    27,   474,   475,   918,   148,   148,   148,  1005,
     174,   115,   148,   351,    54,   174,     2,   110,   215,   921,
     104,   148,    47,    63,   148,    90,    51,   365,   984,    94,
     166,     8,   174,   174,   174,   555,   374,   148,    27,    25,
      56,  1248,  1249,  1250,  1251,  1252,  1253,   174,    56,   387,
     174,    76,   362,     3,    79,    27,    81,   577,    47,   484,
      56,   541,    51,   174,   104,   173,    91,   175,    56,   896,
     110,   881,   541,   270,   541,    47,    79,    80,   275,    51,
     502,   174,   504,    46,   148,   110,   396,    76,   843,    75,
      79,   401,    81,    79,    80,   850,   567,   502,   569,   504,
     148,   856,    91,    66,    76,   148,   861,    79,    56,    81,
     174,   421,   148,   148,   869,   148,   102,   148,   166,    91,
     106,   110,    85,    90,   173,   148,  1122,    94,  1124,  1031,
     166,   174,    90,   166,    56,   166,    94,   148,   110,   174,
     101,  1035,  1036,   166,   148,  1039,  1040,    79,    80,   574,
    1044,  1045,   115,   173,   351,   166,   148,   596,   913,   581,
     915,   916,   166,    90,   502,   581,   504,    94,   365,   148,
     148,   126,    56,   148,   166,   485,   581,   374,   292,   293,
     935,   936,   173,    56,   175,   174,    56,   166,   166,    56,
     387,   166,    22,   173,   148,   175,    26,    27,   153,   141,
     155,   156,   512,   158,   159,    35,   516,    37,    38,   150,
      31,  1179,   166,    43,   734,   567,   173,   569,   175,   529,
      50,  1115,  1116,  1117,  1118,   980,   981,   982,   538,   215,
     142,   171,   752,   543,   173,   147,   175,   173,   150,   175,
      56,   147,   154,   581,   173,    31,   175,    77,   173,    79,
     166,    81,  1062,    83,   165,    56,    86,   166,   166,    89,
     173,    91,   166,   166,   166,   166,   842,   166,   166,    90,
      85,   171,   166,    94,   171,   147,  1244,   587,   173,   750,
     110,   173,   173,   113,   270,   174,   116,   148,   598,   275,
     166,   173,   166,     8,     3,   173,   816,   817,    66,   126,
     171,   772,   896,   172,    90,   502,    91,   504,    94,    91,
     150,   141,   126,    69,   173,   879,   126,   174,   882,   148,
    1222,   142,   143,   745,   145,   146,   147,   126,   149,   150,
     126,    34,     2,   154,   756,    34,   166,   759,    66,   160,
     161,   174,   172,   164,   147,   126,   167,   168,   169,   174,
     174,   756,    21,   171,   759,    25,   142,   877,   120,   148,
     173,   147,   106,   149,   150,   351,    31,   173,   154,   166,
     150,   681,   150,   150,   150,   161,   150,   150,   150,   365,
     150,   167,   168,   169,   581,   150,   150,   150,   374,    31,
     150,   150,   150,   142,   173,   150,    34,   166,   918,   879,
     166,   387,    34,   883,   147,    75,   148,   174,    47,    79,
      80,   172,   892,   166,   166,   166,   166,    34,   756,   176,
     174,   759,   847,  1099,  1100,    90,   171,   849,   176,    94,
     176,   172,   102,   855,   173,   166,   106,   174,   176,   166,
     173,   173,   147,   166,   849,   867,   148,    32,    90,   871,
     855,   761,    94,   174,  1018,    32,   176,   174,   768,    34,
     176,   148,   867,   176,   984,   166,   871,   174,   778,   166,
     174,   166,   165,   165,   111,   576,   984,   142,   143,   814,
     145,   146,   147,    24,   149,   150,   796,   374,   367,   154,
     265,   951,   339,   976,  1058,   391,   161,   527,   892,   541,
     142,   896,   167,   168,   169,   147,   883,   149,   150,   538,
     385,   849,   154,   935,   936,   305,   502,   855,   504,   161,
     942,  1244,   944,  1064,  1254,   947,  1247,   169,   841,   867,
     681,   841,  1059,   871,   131,  1062,   768,   942,  1018,   944,
     524,    -1,   947,    -1,    -1,   215,  1140,   972,    -1,   971,
      -1,    -1,    -1,  1147,    -1,    -1,    -1,    -1,    -1,   756,
      -1,   983,   759,    -1,    -1,    -1,   971,  1131,    -1,    -1,
      -1,   881,    -1,    -1,    -1,     3,    -1,    -1,   983,  1099,
    1100,    -1,  1021,  1022,    -1,    -1,   896,    -1,    -1,    17,
      -1,    -1,    20,    -1,    -1,   581,    -1,    25,    -1,    -1,
     270,    -1,    -1,    -1,   942,   275,   944,    35,    -1,   947,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      -1,  1043,    -1,    -1,    52,    -1,    54,  1049,    -1,    -1,
      -1,    -1,    -1,   971,    -1,    63,    -1,    -1,  1043,  1166,
      -1,   951,    -1,    71,  1049,   983,    -1,    -1,    -1,    -1,
      -1,  1131,   849,    -1,    -1,    -1,    -1,    -1,   855,    -1,
      88,    -1,    -1,    -1,    -1,    -1,   976,    -1,    -1,    -1,
     867,  1093,   100,    -1,   871,    -1,   104,    -1,    -1,    -1,
      -1,   351,    -1,   111,    -1,    45,  1280,  1281,  1093,   117,
    1284,  1285,  1286,    -1,  1288,   365,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   374,  1043,    -1,    -1,    -1,    69,
      -1,  1049,    -1,    -1,    -1,    -1,    -1,   387,    -1,  1141,
      -1,  1143,    -1,    -1,    -1,    -1,  1148,    -1,    -1,  1151,
      -1,    -1,    -1,    -1,    -1,    -1,  1141,  1159,  1143,  1161,
      -1,    -1,    -1,  1148,    -1,   942,  1151,   944,    -1,    -1,
     947,    -1,  1062,    -1,  1159,  1093,  1161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     756,  1193,    -1,   759,   971,    -1,    -1,  1199,    -1,    -1,
      -1,  1203,  1204,    -1,    -1,    -1,   983,    -1,  1193,    -1,
      -1,    -1,    -1,    -1,  1199,  1217,    -1,    -1,  1203,  1204,
      -1,    -1,  1224,  1141,    -1,  1143,    -1,    -1,    -1,    -1,
    1148,    -1,  1217,  1151,  1236,    -1,    -1,    -1,  1240,  1224,
      -1,  1159,    -1,  1161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1236,   502,  1255,   504,  1240,    -1,     2,    -1,  1255,
      -1,    -1,    -1,    -1,    -1,    -1,  1043,    -1,    -1,    -1,
    1255,    -1,  1049,    -1,    -1,  1193,    -1,    -1,    -1,    -1,
      25,  1199,    -1,   849,    -1,  1203,  1204,    -1,    -1,   855,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1217,
      -1,   867,    -1,    -1,    -1,   871,  1224,  1309,  1310,  1311,
    1312,  1313,  1314,    -1,    -1,    -1,  1093,    -1,  1236,    -1,
      -1,    -1,  1240,    -1,  1309,  1310,  1311,  1312,  1313,  1314,
      75,   581,    -1,    -1,    79,    80,    -1,  1255,   278,    -1,
     280,    -1,    -1,    -1,    -1,    -1,    -1,   287,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,
      -1,   106,    -1,    -1,  1141,    -1,  1143,    -1,    -1,    -1,
      -1,  1148,    -1,    -1,  1151,    -1,   942,    -1,   944,    -1,
      -1,   947,  1159,    -1,  1161,    -1,    -1,   327,    -1,    -1,
      -1,  1309,  1310,  1311,  1312,  1313,  1314,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   971,     3,    -1,    90,    -1,
      -1,    -1,    94,    -1,    -1,    -1,  1193,   983,    -1,    -1,
      17,    18,  1199,    20,    -1,    -1,  1203,  1204,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
    1217,    -1,    39,    -1,    -1,    -1,     2,  1224,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,  1236,
     142,   143,    -1,  1240,   146,   147,    63,   149,   150,    25,
     215,    -1,   154,    70,    71,    -1,    -1,  1043,  1255,   161,
      -1,    78,    -1,  1049,    -1,   167,   168,   169,    -1,    -1,
      -1,    88,     2,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    -1,   100,    -1,   102,   756,   104,    -1,   759,
     107,    -1,    -1,    -1,   111,    25,    -1,   114,    -1,    75,
     117,    -1,    -1,    79,    80,   270,    -1,  1093,    -1,    -1,
     275,    -1,  1309,  1310,  1311,  1312,  1313,  1314,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    79,
      80,    -1,    31,    -1,   171,  1141,   173,  1143,    -1,    -1,
      -1,    -1,  1148,    -1,    -1,  1151,    -1,    -1,    -1,    -1,
      -1,    -1,   102,  1159,    -1,  1161,   106,    -1,    -1,   849,
      -1,    -1,    -1,    -1,    -1,   855,   351,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   867,    -1,    -1,
     365,   871,    -1,    -1,    -1,    -1,    -1,  1193,    -1,   374,
      -1,    90,    -1,  1199,    -1,    94,    -1,  1203,  1204,    -1,
      -1,    -1,   387,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1217,    -1,    -1,    -1,    -1,    -1,    -1,  1224,   215,
     600,    -1,   602,   603,   604,   605,   606,   607,    -1,    -1,
    1236,    -1,    -1,    -1,  1240,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,    -1,    -1,   147,  1255,
     149,   150,   942,    -1,   944,   154,    -1,   947,    -1,    -1,
      -1,    -1,   161,    -1,    -1,   215,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,   270,    -1,    -1,    -1,    -1,   275,
      -1,   971,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,
      -1,    -1,    -1,   983,    -1,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1309,  1310,  1311,  1312,  1313,  1314,    -1,
      -1,    -1,    25,    -1,    -1,    87,    -1,   502,    -1,   504,
     270,    -1,    -1,    -1,    -1,   275,    -1,    -1,    22,    -1,
      -1,   103,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    37,    38,    -1,    -1,    -1,    -1,    43,
       1,    -1,    -1,  1043,    -1,   351,    50,    -1,    -1,  1049,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,   365,
      -1,    22,    -1,    -1,    -1,    26,    27,    -1,   374,    -1,
      -1,    -1,    -1,    77,    35,    79,    37,    38,    -1,   102,
      -1,   387,    43,   106,    -1,    89,   581,    91,    -1,    50,
      -1,   351,    53,  1093,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   365,   110,    -1,    -1,   113,
      -1,    -1,   116,    74,   374,   805,    77,    -1,    79,    -1,
      81,    -1,    83,    84,    -1,    86,    -1,   387,    89,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1141,    -1,  1143,    -1,    -1,    -1,   229,  1148,   110,
      12,  1151,   113,    -1,   844,   116,    -1,    19,    -1,  1159,
      -1,  1161,    -1,    -1,   854,    -1,    -1,    -1,   858,    31,
     860,    -1,    -1,   863,    -1,    -1,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,   266,    47,    -1,    -1,    -1,    -1,
      -1,    -1,   215,  1193,    -1,    -1,   502,    -1,   504,  1199,
      -1,    -1,    -1,  1203,  1204,   166,    -1,    -1,    -1,   291,
      -1,   172,    -1,    -1,    -1,    -1,    -1,  1217,    -1,    -1,
      -1,    -1,    -1,    -1,  1224,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    94,    -1,    -1,    -1,  1236,    -1,    -1,    -1,
    1240,    -1,   502,    -1,   504,    -1,    -1,   270,    -1,   331,
      -1,    -1,   275,    -1,    -1,  1255,    -1,    -1,    -1,   121,
      -1,   756,    -1,   125,   759,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   581,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,  1309,
    1310,  1311,  1312,  1313,  1314,    -1,   178,    -1,    -1,    -1,
      -1,   581,    -1,    -1,    -1,    -1,    -1,    -1,   351,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1029,
    1030,    -1,   365,    -1,    -1,  1035,  1036,    -1,    -1,  1039,
    1040,   374,    -1,    -1,   849,    -1,    -1,    -1,    -1,    -1,
     855,    -1,    -1,    -1,   387,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   867,    -1,    -1,    -1,   871,    -1,    -1,    -1,
      -1,    -1,    -1,   465,   466,    -1,    -1,    -1,    -1,   471,
      -1,    -1,   474,   475,   476,   477,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1109,
      -1,    -1,    -1,  1113,  1114,  1115,  1116,    -1,    -1,   511,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1135,  1136,   942,  1138,   944,
     756,    -1,   947,   759,    -1,    -1,    -1,    -1,    -1,    -1,
    1150,    -1,    -1,  1153,    -1,  1155,    -1,  1157,    -1,    -1,
      -1,    -1,    -1,   555,    -1,    -1,   971,    -1,    -1,   502,
      -1,   504,    -1,    -1,    -1,   567,    -1,   569,   983,    -1,
      -1,    -1,    -1,    -1,    -1,   577,   756,    -1,    -1,   759,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,   849,    -1,    -1,    -1,    39,  1043,   855,
      -1,    -1,    -1,    45,  1049,    -1,    -1,    49,   581,    -1,
      52,   867,    54,    -1,    -1,   871,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,   849,
      -1,    -1,    -1,    -1,    -1,   855,    88,    -1,  1093,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,   867,   100,    -1,
     102,   871,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
     112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   942,    -1,   944,    -1,
      -1,   947,    -1,    -1,    -1,    -1,  1141,    -1,  1143,    -1,
      -1,    -1,   734,  1148,    -1,    -1,  1151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1159,   971,  1161,    -1,   750,    -1,
     752,    -1,    -1,    -1,    -1,    -1,    -1,   983,    -1,    -1,
      -1,   173,   942,    -1,   944,    -1,    -1,   947,    -1,    -1,
     772,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1193,    -1,
      -1,    -1,    -1,    -1,  1199,    -1,    -1,    -1,  1203,  1204,
      -1,   971,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1217,   983,    -1,    -1,    -1,    -1,    -1,  1224,
      -1,    -1,    -1,   756,    -1,    -1,   759,  1043,    -1,    -1,
      -1,  1236,    -1,  1049,    -1,  1240,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1255,     2,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,    -1,    -1,  1043,    25,    -1,    -1,  1093,    -1,  1049,
      -1,    -1,    -1,    -1,    -1,   877,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,   173,
      -1,   175,    -1,    -1,  1309,  1310,  1311,  1312,  1313,  1314,
      -1,    -1,    -1,    -1,    -1,    -1,   849,    -1,    -1,    -1,
      -1,    -1,   855,  1093,    75,  1141,   918,  1143,    -1,    -1,
      -1,    -1,  1148,    -1,   867,  1151,    -1,    -1,   871,    -1,
      -1,    -1,    -1,  1159,    -1,  1161,    -1,    -1,    -1,    -1,
      -1,   102,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1141,    -1,  1143,    -1,    -1,    -1,  1193,  1148,    -1,
      -1,  1151,    -1,  1199,    -1,    -1,    -1,  1203,  1204,  1159,
      -1,  1161,   984,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1217,    -1,    -1,    -1,    -1,    -1,    -1,  1224,   942,
      -1,   944,    -1,    -1,   947,    -1,    -1,    -1,    -1,    -1,
    1236,    -1,    -1,  1193,  1240,    -1,    -1,    -1,    -1,  1199,
      -1,    -1,    -1,  1203,  1204,    -1,    -1,    -1,   971,  1255,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1217,    -1,    -1,
     983,    -1,    -1,    -1,  1224,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   215,    -1,  1236,    -1,    -1,    -1,
    1240,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1255,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1309,  1310,  1311,  1312,  1313,  1314,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1099,  1100,    -1,
    1043,    -1,    -1,    13,    -1,    -1,  1049,    17,    -1,   270,
      -1,    -1,    -1,    -1,   275,    -1,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,  1309,
    1310,  1311,  1312,  1313,  1314,    45,    -1,    47,    48,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,
    1093,    -1,    62,    -1,    -1,    -1,    66,    67,    68,    69,
      70,    71,    -1,    -1,    -1,    75,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    87,    -1,    -1,
      90,    -1,    92,    -1,    -1,    95,    96,    -1,    98,    -1,
     351,    -1,   102,   103,   104,    -1,    -1,    -1,  1141,    -1,
    1143,    -1,    -1,    -1,   365,  1148,    -1,    -1,  1151,    -1,
      -1,    -1,    -1,   374,    -1,    22,  1159,    -1,  1161,    26,
      27,   131,    -1,   133,    -1,    -1,   387,    -1,    35,    -1,
      37,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,
    1193,    -1,    -1,    -1,    -1,    -1,  1199,    -1,    -1,    66,
    1203,  1204,    -1,    -1,   174,    -1,    -1,    74,    -1,    -1,
      77,    -1,    79,    -1,  1217,    -1,    83,    84,    85,    86,
      -1,  1224,    89,    -1,    91,    -1,    31,   197,    -1,    -1,
      -1,   201,   202,  1236,    -1,    -1,    -1,  1240,    -1,    -1,
      -1,    -1,    -1,   110,    12,    -1,   113,    -1,    -1,   116,
      -1,    19,  1255,    -1,    -1,    -1,    -1,    -1,    -1,   229,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      -1,   502,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,   266,    -1,    -1,   166,
      -1,    -1,    -1,    -1,   171,    -1,  1309,  1310,  1311,  1312,
    1313,  1314,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   291,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,   302,   303,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,   317,   318,   154,
      -1,    -1,    -1,   121,    -1,   160,   161,   125,    -1,   164,
     581,   331,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   178,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,   354,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,   171,    -1,   375,    -1,   377,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,   386,    -1,   388,   389,
     390,   391,   392,    -1,    -1,    -1,    -1,   397,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   429,
     430,   431,   432,   433,   434,    45,   436,   437,   438,   439,
     440,    -1,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,    69,
      -1,    -1,    -1,    -1,    -1,   465,   466,    -1,    -1,    79,
      80,   471,    -1,    -1,   474,   475,   476,   477,   478,   479,
      -1,    -1,   482,   483,   484,    -1,   486,    -1,    -1,    -1,
      -1,    -1,   492,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    -1,   504,   756,    -1,   507,   759,    -1,
      -1,   511,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   521,    -1,   523,    -1,    -1,    -1,   527,    -1,    -1,
     530,   531,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   541,    -1,    -1,    -1,    -1,   546,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   555,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   564,    -1,    -1,   567,    -1,   569,
      -1,    -1,   572,   573,   574,    -1,    -1,   577,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   588,   589,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   849,    -1,
      -1,    -1,    -1,    -1,   855,    -1,    -1,     3,   608,    -1,
      -1,   221,    -1,    -1,    -1,    -1,   867,    -1,   618,    -1,
     871,    17,    18,    -1,    20,    -1,    -1,    -1,   628,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,   261,   262,    49,    -1,    -1,    52,    -1,    54,   269,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,   278,    -1,
     280,    -1,   672,    -1,    70,    71,    -1,   287,    -1,   679,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   942,    88,   944,    -1,    -1,   947,    -1,    -1,    -1,
      -1,    97,    98,    -1,   100,    -1,   102,    -1,   104,    22,
      -1,   107,    -1,    26,    27,   111,   112,   327,   114,    -1,
     971,   117,    35,    -1,    37,    38,    -1,    -1,    -1,    -1,
      43,    -1,   983,   733,   734,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     750,    -1,   752,   753,   754,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      83,    -1,   772,    86,    -1,    -1,    89,   173,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   786,    -1,    -1,    -1,
     790,    -1,  1043,    -1,    -1,    -1,    -1,   110,  1049,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   814,    -1,    -1,    -1,   818,    -1,
     820,   821,   822,    -1,   824,    -1,   826,    -1,   141,   829,
     830,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1093,   843,    -1,   845,    -1,   847,    -1,   849,
     850,    -1,    -1,   166,    -1,   855,   856,    -1,    -1,   172,
      -1,   861,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   869,
     870,   871,    -1,   873,   874,    -1,    -1,   877,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1141,    -1,  1143,    -1,    -1,    -1,    -1,  1148,    -1,    -1,
    1151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1159,    -1,
    1161,    -1,   912,   913,   914,   915,   916,   917,   918,    -1,
      -1,   921,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   935,   936,    -1,    -1,    -1,
      -1,    -1,  1193,    -1,    -1,   945,    -1,    -1,  1199,    -1,
      -1,    -1,  1203,  1204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   962,    -1,    -1,    -1,   966,    -1,    -1,    -1,
      -1,    -1,   972,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     980,   981,   982,    -1,   984,  1236,    -1,    -1,    -1,  1240,
     600,   601,   602,   603,   604,   605,   606,   607,    -1,    -1,
      -1,    -1,    -1,    -1,  1255,  1005,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1013,    -1,  1015,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1025,    -1,  1027,    -1,    -1,
      -1,  1031,    -1,  1033,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,  1066,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    -1,  1076,    39,    -1,  1079,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,  1099,
    1100,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,  1111,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      -1,  1121,  1122,  1123,  1124,  1125,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    -1,   100,    -1,
     102,  1141,   104,  1143,    -1,   107,    -1,    -1,  1148,   111,
     760,  1151,   114,    -1,    -1,   117,    -1,    -1,    -1,  1159,
      31,  1161,    -1,    -1,  1164,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1184,  1185,   796,  1187,  1188,  1189,
    1190,    -1,    -1,    -1,   804,   805,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   173,  1212,    -1,    -1,    -1,    -1,  1217,  1218,    90,
    1220,    -1,  1222,    94,  1224,  1225,    -1,    -1,  1228,    -1,
      -1,    -1,    -1,  1233,   844,    -1,    -1,  1237,    -1,    -1,
      -1,    -1,    -1,    -1,   854,    -1,  1246,    -1,   858,    -1,
     860,    -1,    -1,   863,   125,    -1,    -1,    -1,    -1,    -1,
      -1,  1261,  1262,  1263,  1264,  1265,  1266,  1267,  1268,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,    -1,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,  1309,
    1310,  1311,  1312,  1313,  1314,  1315,  1316,  1317,  1318,  1319,
    1320,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    18,    -1,    -1,    -1,    22,
      -1,    -1,   952,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,   979,
      -1,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      -1,    -1,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    79,    80,    81,    82,
      -1,    -1,    -1,    -1,    87,    -1,    89,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    99,    -1,   101,  1029,
    1030,    -1,    -1,    -1,    -1,  1035,  1036,   110,    -1,  1039,
    1040,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,   174,   175,    -1,    -1,   178,    -1,    -1,    -1,  1109,
       1,    -1,     3,  1113,  1114,  1115,  1116,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    25,  1135,  1136,    -1,  1138,    -1,
    1140,    -1,    33,    -1,    35,    -1,    -1,  1147,    39,    -1,
    1150,    -1,    -1,  1153,    45,  1155,    -1,  1157,    49,    -1,
      -1,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,  1201,    -1,    -1,    -1,    -1,    97,    98,    -1,   100,
      -1,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,
     131,    -1,    -1,   134,   135,   136,   137,    -1,   139,   140,
      -1,   142,   143,   144,   145,   146,   147,    -1,   149,    -1,
      -1,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,    -1,   164,    -1,    -1,   167,   168,   169,   170,
    1280,  1281,   173,    -1,  1284,  1285,  1286,    -1,  1288,     0,
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
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
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
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    12,
      -1,    14,    -1,    -1,    17,    18,    19,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    55,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,   131,    -1,
      -1,   134,   135,   136,   137,    -1,   139,   140,    -1,   142,
     143,   144,   145,   146,   147,    -1,   149,    -1,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,    -1,
     173,    -1,    -1,    -1,    -1,   178,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    12,    -1,    14,    -1,    -1,
      17,    18,    19,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    55,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,   131,    -1,    -1,   134,   135,   136,
     137,    -1,   139,   140,    -1,   142,   143,   144,   145,   146,
     147,    -1,   149,    -1,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
     167,   168,   169,   170,   171,    -1,   173,    -1,    -1,    -1,
      -1,   178,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    -1,    17,    18,    -1,    20,
      -1,    22,    23,    -1,    25,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,
      -1,    62,    63,    64,    65,    -1,    67,    68,    -1,    70,
      71,    72,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,   104,   105,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,    -1,    -1,    -1,   178,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    26,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    79,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      89,    90,    -1,    -1,    -1,    94,    -1,    -1,    97,    98,
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
      -1,   104,    -1,    -1,   107,   108,   109,    -1,   111,   112,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,    -1,    -1,   178,     3,     4,     5,     6,
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
     107,    -1,    -1,    -1,   111,    -1,   113,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,
      -1,   178,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    47,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,   116,   117,    -1,    -1,    -1,
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
      -1,    -1,   107,   108,   109,    -1,   111,   112,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,   174,
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
     109,    -1,   111,   112,    -1,   114,    -1,    -1,   117,    -1,
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
     163,   164,   165,   166,    -1,    -1,   169,    -1,   171,    -1,
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
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
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
     171,    -1,   173,    -1,   175,    -1,    -1,   178,     3,     4,
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
      63,    -1,    65,    -1,    67,    68,    69,    70,    71,    -1,
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
      -1,   148,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,
      -1,    -1,   169,    -1,   171,   172,   173,    -1,    -1,    -1,
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
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,   148,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,    -1,   171,   172,   173,    -1,
      -1,    -1,    -1,   178,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,   116,   117,    -1,
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
      -1,   104,    -1,    -1,   107,   108,   109,    -1,   111,    -1,
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
      87,    88,    89,    90,    -1,    -1,    -1,    94,    -1,    -1,
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
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,
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
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,   123,    -1,
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
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,   148,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,   174,    -1,    -1,    -1,   178,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    32,
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
     117,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,
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
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,   172,   173,    -1,
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
      -1,    -1,   111,   112,    -1,   114,    -1,    -1,   117,    -1,
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
     111,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
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
     151,     4,     5,     6,     7,     8,     9,    10,    -1,   160,
      -1,    14,   163,   164,   165,    18,    -1,    -1,   169,    22,
     171,    -1,   173,    -1,    27,    -1,    -1,   178,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,   175,    -1,    -1,   178,     3,     4,     5,     6,
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
     171,    -1,    12,    -1,    90,    -1,    -1,   178,    94,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    47,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      90,   167,   168,   169,    94,   171,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    31,    -1,   125,    -1,    -1,    36,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    73,    -1,   167,   168,   169,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    13,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,   125,    -1,    -1,
      36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    73,   166,   167,
     168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,   171,    -1,    12,    -1,    90,    -1,    -1,   178,    94,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    90,   167,   168,   169,    94,   171,    -1,    12,
      -1,    -1,    -1,    -1,   178,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,   171,    -1,    12,    -1,    90,    -1,    -1,
     178,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    -1,   125,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    31,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    90,   167,   168,   169,    94,    -1,    -1,
      -1,    12,    -1,    -1,    -1,   178,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   103,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,   125,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,   178,    -1,    94,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    12,
      -1,   167,   168,   169,   125,   171,    19,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,   142,   143,    36,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    12,    -1,   167,   168,   169,    -1,
     171,    19,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      73,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,   121,   167,   168,   169,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,   121,   167,
     168,   169,   125,    -1,    -1,    -1,    12,    13,   176,    -1,
     178,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    31,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    12,    -1,    -1,
      -1,    -1,    -1,   176,    19,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    12,    94,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    12,    -1,    94,
      -1,    -1,    -1,    -1,    19,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    31,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
     125,   167,   168,   169,    -1,    90,    -1,    -1,    12,    94,
      -1,    -1,   178,    -1,    -1,    19,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    31,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     125,    -1,   167,   168,   169,    90,   171,    -1,    -1,    94,
      -1,    -1,    -1,   178,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     125,   166,   167,   168,   169,    -1,    90,    -1,    12,    13,
      94,    -1,    -1,   178,    -1,    19,    -1,   142,   143,   103,
     145,   146,   147,    -1,   149,   150,   151,    31,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,   125,   167,   168,   169,    -1,    -1,    -1,    -1,   174,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    90,    -1,    12,    -1,
      94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    90,    -1,    12,    -1,
      94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    31,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    90,    -1,    -1,    12,
      94,    -1,    -1,    -1,   178,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,   125,   167,   168,   169,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,   178,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   125,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,   174,    -1,    -1,    -1,   178,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    12,    13,   167,   168,   169,    -1,    -1,    19,
      -1,   174,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    12,    13,    94,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   125,    -1,   167,   168,   169,
      90,    -1,    -1,    12,    94,    -1,    -1,    -1,   178,    -1,
      19,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    31,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   125,    -1,   167,   168,   169,
      -1,    -1,    -1,    12,   174,    -1,    -1,    -1,   178,    -1,
      19,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    31,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,    90,    -1,    -1,    -1,    94,    -1,    -1,   178,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   125,    -1,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,   178,
      -1,    94,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    12,   166,   167,   168,
     169,   124,   125,    19,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    12,    -1,   167,   168,   169,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
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
      -1,    90,    -1,    12,    -1,    94,   176,    -1,   178,    -1,
      19,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    12,    94,    -1,    -1,    -1,    -1,
      -1,    19,    61,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    31,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   125,    -1,   167,   168,
     169,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,   178,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   125,    -1,   167,   168,
     169,    -1,    90,    -1,    12,    -1,    94,    -1,    -1,   178,
      -1,    19,    -1,   142,   143,   103,   145,   146,   147,    -1,
     149,   150,   151,    31,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,   125,   167,   168,
     169,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,   178,
      -1,    -1,    19,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    31,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    12,    -1,    90,    -1,    -1,    -1,    94,    19,    -1,
      -1,    -1,    -1,    -1,   142,   143,   103,   145,   146,   147,
      31,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,   125,   167,
     168,   169,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
     178,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
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
      -1,    90,   167,   168,   169,    94,    -1,    -1,    -1,    12,
      -1,    -1,    -1,   178,   103,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,   125,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,   178,
      -1,    94,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    22,   167,   168,   169,    26,    27,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    35,    -1,    37,    38,
      -1,    31,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    86,    -1,    -1,
      89,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    94,   125,    -1,    -1,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   166,   158,   159,
     160,   161,   162,   172,   164,    -1,    -1,   167,   168,   169,
      90,    -1,   142,   143,    94,   145,   146,   147,   178,   149,
     150,   151,    -1,   153,   154,   155,   156,    -1,   158,   159,
     160,   161,   162,    -1,   164,    -1,    22,   167,   168,   169,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,   178,    35,
      -1,    37,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      -1,    -1,   142,   143,    50,   145,   146,   147,    -1,   149,
     150,   151,    -1,    -1,   154,   155,   156,    -1,   158,   159,
     160,   161,    -1,    -1,   164,    -1,    -1,   167,   168,   169,
      -1,    77,    -1,    79,    -1,    81,    -1,    83,   178,    -1,
      86,    -1,    -1,    89,    22,    91,    -1,    -1,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    37,
      38,    -1,    -1,    -1,   110,    43,    -1,   113,    -1,    -1,
     116,    -1,    50,    -1,     1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    18,    -1,    20,    -1,   141,    -1,    -1,    25,    77,
      -1,    79,    -1,    -1,    -1,    83,    33,    -1,    86,    -1,
      -1,    89,    39,    91,    -1,    -1,    -1,    -1,    45,    -1,
     166,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,
      -1,    -1,   110,    -1,    -1,   113,    63,    -1,   116,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,   141,     3,    -1,    -1,    -1,    -1,    -1,
      97,    98,    -1,   100,    -1,   102,    -1,   104,    17,    18,
     107,    20,    -1,    -1,   111,    -1,    25,   114,   166,    -1,
     117,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,   141,    54,    -1,    -1,    -1,    -1,
       3,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    17,    18,    -1,    20,    -1,    78,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    88,
      33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    97,    98,
      -1,   100,    45,   102,    -1,   104,    49,    -1,   107,    52,
      -1,    54,   111,    -1,    -1,   114,     3,    -1,   117,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      17,    18,    -1,    20,    -1,    78,    79,    -1,    25,    -1,
      -1,    -1,   141,    -1,    -1,    88,    33,    -1,    91,    -1,
      -1,    -1,    39,    -1,    97,    98,    -1,   100,    45,   102,
      -1,   104,    49,    -1,   107,    52,    -1,    54,   111,    -1,
      -1,   114,     3,   116,   117,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    17,    18,    -1,    20,
      -1,    78,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      97,    98,    -1,   100,    45,   102,    -1,   104,    49,    -1,
     107,    52,    -1,    54,   111,   112,    -1,   114,     3,    -1,
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
     207,   195,   150,   197,   171,    46,    85,   115,   204,    26,
     325,   369,     8,   265,   369,   370,    24,    32,    40,    41,
      42,   120,   175,   242,   349,   351,    56,   147,   267,   221,
       1,   194,   221,   271,   274,   173,   300,    53,    74,    84,
     309,    27,    76,    79,    91,   110,   310,    27,    79,    91,
     110,   308,   221,   321,   322,   327,   165,   166,   369,    12,
      19,    31,    90,    94,   125,   142,   143,   145,   146,   147,
     149,   150,   151,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   167,   168,   169,   178,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   170,   296,   173,   175,    90,    94,
     369,   221,    40,    41,   175,    56,   173,   187,   166,   207,
     324,   166,   148,   166,   222,   343,   344,   345,   371,   171,
     221,   221,   103,   207,   103,   126,   207,   296,   204,   343,
     166,   173,   173,   207,   187,   112,   173,   221,   329,   330,
       1,   147,   336,    47,   148,   187,   226,   148,   226,    13,
     173,   173,   226,   343,   231,   231,    47,    91,   306,   307,
     174,   148,   112,   173,   221,   304,   147,   166,   369,   369,
     124,   286,   166,   171,   226,   173,   343,   166,   254,   254,
     166,   172,   172,   185,   148,   172,   369,   148,   174,   148,
     174,     8,    40,    41,   175,   242,     3,   173,   198,     1,
     172,   208,   215,   216,   369,   210,   369,    66,    36,    73,
     166,   265,   267,   110,   237,   289,   369,   187,    79,   123,
     243,   369,   243,   369,   243,   369,   245,   369,   176,   243,
     340,   369,   380,   221,   340,   171,   172,   274,   126,   148,
     172,   174,   301,   303,   305,   310,    91,     1,   147,   334,
     335,    91,     1,     3,    12,    17,    19,    20,    25,    45,
      52,    54,    55,    63,    71,    88,   100,   104,   111,   117,
     142,   143,   144,   145,   146,   147,   149,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   164,   167,   168,
     169,   170,   173,   221,   293,   294,   295,   296,   346,   126,
     323,   148,   166,   166,   369,   369,   369,   243,   369,   243,
     369,   369,   369,   369,   369,   369,   369,     3,    20,    33,
      63,   104,   110,   222,   369,   369,   369,   369,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   369,   369,
     369,    69,   371,   371,   371,   371,   371,   343,   343,   243,
     369,   243,   369,   103,   173,   369,   369,   340,   221,    27,
      47,    91,   116,   357,   360,   361,   369,   385,   327,   369,
     126,   174,   148,   126,   126,   187,    34,   187,   369,    34,
     369,    66,   174,   344,   221,   247,   248,   330,   148,   174,
     225,   369,   126,   331,   369,   328,   261,   221,   324,   369,
     369,   174,   304,   310,   282,   330,   147,   338,   339,   337,
     287,   206,     1,   259,   345,   174,    21,   255,   172,   174,
     174,   341,   174,   341,   369,   369,   340,   197,   174,   199,
     200,   345,   172,   172,   148,   166,    13,   150,   148,   166,
      13,    36,    73,   221,   142,   143,   144,   145,   146,   160,
     164,   169,   209,   295,   296,   297,   369,   209,   211,   267,
     171,   238,   324,    47,   355,   120,   221,   173,   226,    32,
      47,   226,   121,   226,   358,    32,    47,   226,   358,   226,
     358,    47,   226,   358,   226,   226,   369,   176,   358,    47,
     148,   176,   358,   124,   148,   176,   166,   173,     1,   268,
     369,   272,   273,   148,   174,   336,     1,   225,   332,   333,
     106,   311,   173,   299,   369,   142,   150,   299,   299,   369,
     322,   173,   175,   166,   166,   166,   166,   166,   166,   174,
     176,   344,    32,    47,    32,    47,    47,   176,   173,    47,
      91,   148,   174,    17,    20,    25,    45,    52,    63,    71,
      88,   100,   111,   117,   346,    90,    90,   166,   371,   345,
     369,   369,    34,   187,    34,    34,   187,   103,   207,   221,
     174,   148,   174,   174,   329,   336,    69,   371,   221,   174,
     174,   339,   147,   283,   174,   337,   152,   298,   331,   369,
     172,    75,   118,   172,   260,   174,   173,   207,   221,   256,
      47,    47,    47,   176,   148,   174,   215,   222,    17,    18,
      20,    25,    45,    49,    52,    71,    78,    88,    97,    98,
     100,   102,   111,   114,   117,   171,   214,   297,   369,   112,
     369,   209,   211,   148,   166,    13,   166,   171,   268,   321,
     327,   243,   369,   226,   369,    47,   340,    44,   369,   243,
     369,   173,   226,    44,   369,   243,   369,   226,   226,   243,
     369,   226,   187,   176,   243,   369,   176,   187,   369,   176,
     369,   176,   369,   344,   172,    83,    86,   172,   186,   194,
     228,   274,   194,   303,   311,    61,   119,   315,   301,   302,
     174,   294,   296,   174,   176,   174,    44,   243,   369,    44,
     243,   369,   243,   369,   344,   360,   336,   346,   346,   187,
     187,   369,   187,    34,   166,   166,   248,   207,   331,   173,
     173,   298,   331,   332,   311,   339,   369,   187,   240,   340,
     256,   147,   207,   243,   369,   243,   369,   243,   369,   200,
     209,    13,    36,    73,    36,    73,   166,   166,   297,   369,
     369,   268,   172,   166,   166,   355,   355,   166,   369,   174,
     369,    32,   226,    32,   226,   359,   360,   369,    32,   226,
     358,    32,   226,   358,   226,   358,   226,   358,   187,   176,
     358,   176,   358,   187,   124,   174,   194,   228,   228,   274,
     315,     3,    54,    93,   104,   316,   317,   318,   369,   290,
     174,   299,   299,   369,    32,    32,   369,    32,    32,   176,
     176,   174,   331,    34,   187,   343,   343,   298,   331,    32,
     226,   174,   369,   176,   176,   172,   112,   369,   209,   211,
     209,   211,    13,   172,   226,   226,   226,   103,    44,   369,
      44,   369,   148,   174,   103,    44,   369,   226,    44,   369,
     226,   226,   226,    44,   187,   369,   176,    44,   187,   369,
     176,   369,   166,   228,   277,   318,   119,   148,   126,   153,
     155,   156,   158,   159,    61,    32,   166,   206,   227,   312,
     313,   103,    44,    44,   103,    44,    44,    44,    44,   187,
     174,   174,   187,   207,   166,   166,   166,   166,   369,   369,
     369,   369,   360,   369,   369,   369,   369,   187,   369,   187,
     313,   369,   317,   318,   318,   318,   318,   318,   318,   316,
     185,   369,   369,   369,   369,   369,   369,   165,   165,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   369,   369,   369,   369,   369,   369
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
#line 632 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6521 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 636 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6527 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 637 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6533 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 644 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6541 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 652 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6549 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 656 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6558 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 666 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6566 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 671 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6574 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 678 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6580 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 679 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6586 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 680 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6592 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 681 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6598 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 682 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6604 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 683 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6610 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 684 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6616 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 685 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6622 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6628 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: TDEFER stmt  */
#line 688 "chpl.ypp"
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
#line 6645 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 700 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6651 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 701 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6657 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: TINIT TTHIS TSEMI  */
#line 703 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr));
    auto node = Init::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6669 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 711 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6682 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 720 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6695 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TLABEL ident_def stmt  */
#line 729 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6703 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 733 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6714 "bison-chpl-lib.cpp"
    break;

  case 29: /* stmt_base: error TSEMI  */
#line 740 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6723 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: assignment_stmt  */
#line 747 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6729 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: if_stmt  */
#line 748 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6735 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: loop_stmt  */
#line 749 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6741 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: select_stmt  */
#line 750 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6747 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: manage_stmt  */
#line 751 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6753 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 752 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6759 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: throw_stmt  */
#line 753 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6765 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 755 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6771 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 757 "chpl.ypp"
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
#line 6790 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 772 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6802 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 780 "chpl.ypp"
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
#line 6818 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TLOCAL do_stmt  */
#line 792 "chpl.ypp"
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
#line 6833 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TON expr do_stmt  */
#line 803 "chpl.ypp"
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
#line 6849 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 815 "chpl.ypp"
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
#line 6865 "bison-chpl-lib.cpp"
    break;

  case 44: /* tryable_stmt: TSERIAL do_stmt  */
#line 827 "chpl.ypp"
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
#line 6880 "bison-chpl-lib.cpp"
    break;

  case 45: /* tryable_stmt: TSYNC stmt  */
#line 838 "chpl.ypp"
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
#line 6897 "bison-chpl-lib.cpp"
    break;

  case 48: /* collect_attributes: %empty  */
#line 858 "chpl.ypp"
  {
    auto attributeGroup = context->buildAttributeGroup((yyloc));
    if (attributeGroup != nullptr) {
      context->resetAttributeGroupPartsState();
    }
    context->loopAttributes.push_back(std::move(attributeGroup));
  }
#line 6909 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 883 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6917 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATMARK TIDENT  */
#line 890 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6926 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 897 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6934 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 901 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6945 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 911 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6951 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 913 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6959 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 917 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6967 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 924 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6976 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 929 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6986 "bison-chpl-lib.cpp"
    break;

  case 64: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 942 "chpl.ypp"
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
#line 7007 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 962 "chpl.ypp"
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
#line 7026 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 977 "chpl.ypp"
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
#line 7045 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 992 "chpl.ypp"
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
#line 7062 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1007 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 7068 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1008 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 7075 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1010 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 7082 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1015 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 7088 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1016 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 7095 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1022 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 7105 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1040 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 7113 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1044 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 7121 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR error TRCBR  */
#line 1048 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 7133 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt: block_stmt_body  */
#line 1059 "chpl.ypp"
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
#line 7152 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1077 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 7158 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1078 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7164 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1083 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 7172 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1087 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7180 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1091 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 7189 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1096 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 7197 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1100 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7205 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1104 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 7214 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1114 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7223 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1119 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7233 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1125 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7244 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1132 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 7253 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1137 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7264 "bison-chpl-lib.cpp"
    break;

  case 91: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1144 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7276 "bison-chpl-lib.cpp"
    break;

  case 92: /* opt_only_ls: %empty  */
#line 1154 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7282 "bison-chpl-lib.cpp"
    break;

  case 93: /* opt_only_ls: renames_ls  */
#line 1155 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7288 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1160 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7297 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1165 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7309 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1173 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7322 "bison-chpl-lib.cpp"
    break;

  case 97: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1182 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7336 "bison-chpl-lib.cpp"
    break;

  case 98: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1192 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7348 "bison-chpl-lib.cpp"
    break;

  case 99: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1200 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7361 "bison-chpl-lib.cpp"
    break;

  case 100: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1209 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7375 "bison-chpl-lib.cpp"
    break;

  case 101: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1222 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7384 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_expr: expr  */
#line 1258 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 7392 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_expr: expr TDOT all_op_name  */
#line 1262 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7401 "bison-chpl-lib.cpp"
    break;

  case 121: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1267 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7410 "bison-chpl-lib.cpp"
    break;

  case 122: /* import_expr: expr TAS ident_use  */
#line 1272 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 7420 "bison-chpl-lib.cpp"
    break;

  case 123: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1278 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 7430 "bison-chpl-lib.cpp"
    break;

  case 124: /* import_ls: import_expr  */
#line 1286 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7436 "bison-chpl-lib.cpp"
    break;

  case 125: /* import_ls: import_ls TCOMMA import_expr  */
#line 1287 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7442 "bison-chpl-lib.cpp"
    break;

  case 126: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1292 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7452 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1301 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7460 "bison-chpl-lib.cpp"
    break;

  case 128: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1305 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7468 "bison-chpl-lib.cpp"
    break;

  case 129: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1309 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7476 "bison-chpl-lib.cpp"
    break;

  case 130: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1313 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7484 "bison-chpl-lib.cpp"
    break;

  case 131: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1317 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7492 "bison-chpl-lib.cpp"
    break;

  case 132: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1321 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7500 "bison-chpl-lib.cpp"
    break;

  case 133: /* opt_label_ident: %empty  */
#line 1329 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7506 "bison-chpl-lib.cpp"
    break;

  case 134: /* opt_label_ident: TIDENT  */
#line 1330 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7512 "bison-chpl-lib.cpp"
    break;

  case 135: /* opt_label_ident: TINIT  */
#line 1331 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7518 "bison-chpl-lib.cpp"
    break;

  case 144: /* ident_use: TIDENT  */
#line 1357 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7524 "bison-chpl-lib.cpp"
    break;

  case 145: /* ident_use: TINIT  */
#line 1358 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7530 "bison-chpl-lib.cpp"
    break;

  case 146: /* ident_use: TTHIS  */
#line 1359 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7536 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TBOOL  */
#line 1386 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7542 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TENUM  */
#line 1387 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7548 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TINT  */
#line 1388 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7554 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TUINT  */
#line 1389 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7560 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TREAL  */
#line 1390 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7566 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TIMAG  */
#line 1391 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7572 "bison-chpl-lib.cpp"
    break;

  case 172: /* scalar_type: TCOMPLEX  */
#line 1392 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7578 "bison-chpl-lib.cpp"
    break;

  case 173: /* scalar_type: TBYTES  */
#line 1393 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7584 "bison-chpl-lib.cpp"
    break;

  case 174: /* scalar_type: TSTRING  */
#line 1394 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7590 "bison-chpl-lib.cpp"
    break;

  case 175: /* scalar_type: TLOCALE  */
#line 1395 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7596 "bison-chpl-lib.cpp"
    break;

  case 176: /* scalar_type: TNOTHING  */
#line 1396 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7602 "bison-chpl-lib.cpp"
    break;

  case 177: /* scalar_type: TVOID  */
#line 1397 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7608 "bison-chpl-lib.cpp"
    break;

  case 182: /* do_stmt: TDO stmt  */
#line 1411 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7614 "bison-chpl-lib.cpp"
    break;

  case 183: /* do_stmt: block_stmt  */
#line 1412 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7620 "bison-chpl-lib.cpp"
    break;

  case 184: /* return_stmt: TRETURN TSEMI  */
#line 1417 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7631 "bison-chpl-lib.cpp"
    break;

  case 185: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1424 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7642 "bison-chpl-lib.cpp"
    break;

  case 186: /* class_level_stmt: TSEMI  */
#line 1434 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7650 "bison-chpl-lib.cpp"
    break;

  case 187: /* class_level_stmt: inner_class_level_stmt  */
#line 1438 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7660 "bison-chpl-lib.cpp"
    break;

  case 188: /* $@1: %empty  */
#line 1443 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7667 "bison-chpl-lib.cpp"
    break;

  case 189: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1445 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7676 "bison-chpl-lib.cpp"
    break;

  case 190: /* $@2: %empty  */
#line 1449 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7683 "bison-chpl-lib.cpp"
    break;

  case 191: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1451 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7692 "bison-chpl-lib.cpp"
    break;

  case 199: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1469 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7701 "bison-chpl-lib.cpp"
    break;

  case 200: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1474 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7710 "bison-chpl-lib.cpp"
    break;

  case 201: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1479 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7719 "bison-chpl-lib.cpp"
    break;

  case 202: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1484 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7727 "bison-chpl-lib.cpp"
    break;

  case 203: /* forwarding_decl_start: TFORWARDING  */
#line 1491 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7736 "bison-chpl-lib.cpp"
    break;

  case 204: /* extern_or_export: TEXTERN  */
#line 1498 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7742 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_or_export: TEXPORT  */
#line 1499 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7748 "bison-chpl-lib.cpp"
    break;

  case 206: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1504 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7758 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1514 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7767 "bison-chpl-lib.cpp"
    break;

  case 208: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1520 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7778 "bison-chpl-lib.cpp"
    break;

  case 209: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1527 "chpl.ypp"
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
#line 7800 "bison-chpl-lib.cpp"
    break;

  case 210: /* $@3: %empty  */
#line 1545 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7808 "bison-chpl-lib.cpp"
    break;

  case 211: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1549 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7817 "bison-chpl-lib.cpp"
    break;

  case 212: /* $@4: %empty  */
#line 1554 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7826 "bison-chpl-lib.cpp"
    break;

  case 213: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1559 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7835 "bison-chpl-lib.cpp"
    break;

  case 214: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1567 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7843 "bison-chpl-lib.cpp"
    break;

  case 215: /* no_loop_attributes: %empty  */
#line 1572 "chpl.ypp"
                    { context->loopAttributes.push_back(nullptr); }
#line 7849 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1575 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7855 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1576 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7861 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
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
                               toOwned((yyvsp[-1].expr)),
                               context->popLoopAttributeGroup());
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7884 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1600 "chpl.ypp"
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
#line 7902 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1614 "chpl.ypp"
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
#line 7920 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1628 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7928 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1632 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7936 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1636 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7944 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: TFOR expr TIN expr do_stmt  */
#line 1640 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7952 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: TFOR expr TIN zippered_iterator do_stmt  */
#line 1644 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7960 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: TFOR expr do_stmt  */
#line 1648 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7968 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: TFOR zippered_iterator do_stmt  */
#line 1652 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7976 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1656 "chpl.ypp"
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
#line 8000 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: TFORALL expr TIN expr do_stmt  */
#line 1676 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8008 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1680 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8016 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1684 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8024 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1688 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8032 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TFORALL expr do_stmt  */
#line 1692 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8040 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TFORALL expr forall_intent_clause do_stmt  */
#line 1696 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8048 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: TFORALL zippered_iterator do_stmt  */
#line 1700 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8056 "bison-chpl-lib.cpp"
    break;

  case 236: /* loop_stmt_base: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1704 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8064 "bison-chpl-lib.cpp"
    break;

  case 237: /* loop_stmt_base: TFOREACH expr TIN expr do_stmt  */
#line 1708 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8072 "bison-chpl-lib.cpp"
    break;

  case 238: /* loop_stmt_base: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1712 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8080 "bison-chpl-lib.cpp"
    break;

  case 239: /* loop_stmt_base: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1716 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8088 "bison-chpl-lib.cpp"
    break;

  case 240: /* loop_stmt_base: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1720 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8096 "bison-chpl-lib.cpp"
    break;

  case 241: /* loop_stmt_base: TFOREACH expr do_stmt  */
#line 1724 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8104 "bison-chpl-lib.cpp"
    break;

  case 242: /* loop_stmt_base: TFOREACH expr forall_intent_clause do_stmt  */
#line 1728 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8112 "bison-chpl-lib.cpp"
    break;

  case 243: /* loop_stmt_base: TFOREACH zippered_iterator do_stmt  */
#line 1732 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8120 "bison-chpl-lib.cpp"
    break;

  case 244: /* loop_stmt_base: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1736 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8128 "bison-chpl-lib.cpp"
    break;

  case 245: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1740 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8136 "bison-chpl-lib.cpp"
    break;

  case 246: /* loop_stmt_base: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1744 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8144 "bison-chpl-lib.cpp"
    break;

  case 247: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1748 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8152 "bison-chpl-lib.cpp"
    break;

  case 248: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1752 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8160 "bison-chpl-lib.cpp"
    break;

  case 249: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1756 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8168 "bison-chpl-lib.cpp"
    break;

  case 250: /* loop_stmt_base: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1760 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8176 "bison-chpl-lib.cpp"
    break;

  case 251: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1764 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8185 "bison-chpl-lib.cpp"
    break;

  case 252: /* loop_stmt_base: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1769 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8194 "bison-chpl-lib.cpp"
    break;

  case 253: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1777 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 8204 "bison-chpl-lib.cpp"
    break;

  case 254: /* if_stmt: TIF expr TTHEN stmt  */
#line 1786 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8212 "bison-chpl-lib.cpp"
    break;

  case 255: /* if_stmt: TIF expr block_stmt  */
#line 1790 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8220 "bison-chpl-lib.cpp"
    break;

  case 256: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1794 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8228 "bison-chpl-lib.cpp"
    break;

  case 257: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1798 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8236 "bison-chpl-lib.cpp"
    break;

  case 258: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1802 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8244 "bison-chpl-lib.cpp"
    break;

  case 259: /* if_stmt: TIF ifvar block_stmt  */
#line 1806 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8252 "bison-chpl-lib.cpp"
    break;

  case 260: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1810 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8260 "bison-chpl-lib.cpp"
    break;

  case 261: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1814 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8268 "bison-chpl-lib.cpp"
    break;

  case 262: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1818 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8277 "bison-chpl-lib.cpp"
    break;

  case 263: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1823 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8286 "bison-chpl-lib.cpp"
    break;

  case 264: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1828 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8295 "bison-chpl-lib.cpp"
    break;

  case 265: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1833 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8304 "bison-chpl-lib.cpp"
    break;

  case 266: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1841 "chpl.ypp"
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
#line 8323 "bison-chpl-lib.cpp"
    break;

  case 267: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1856 "chpl.ypp"
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
#line 8342 "bison-chpl-lib.cpp"
    break;

  case 268: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1874 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8350 "bison-chpl-lib.cpp"
    break;

  case 269: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1878 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8358 "bison-chpl-lib.cpp"
    break;

  case 270: /* ifc_formal_ls: ifc_formal  */
#line 1884 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8364 "bison-chpl-lib.cpp"
    break;

  case 271: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1885 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8370 "bison-chpl-lib.cpp"
    break;

  case 272: /* ifc_formal: ident_def  */
#line 1890 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8376 "bison-chpl-lib.cpp"
    break;

  case 292: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1934 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8384 "bison-chpl-lib.cpp"
    break;

  case 293: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1938 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 8392 "bison-chpl-lib.cpp"
    break;

  case 294: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1942 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8400 "bison-chpl-lib.cpp"
    break;

  case 295: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1949 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8408 "bison-chpl-lib.cpp"
    break;

  case 296: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1953 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 8416 "bison-chpl-lib.cpp"
    break;

  case 297: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1957 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8424 "bison-chpl-lib.cpp"
    break;

  case 298: /* try_stmt: TTRY tryable_stmt  */
#line 1964 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8432 "bison-chpl-lib.cpp"
    break;

  case 299: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1968 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8440 "bison-chpl-lib.cpp"
    break;

  case 300: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1972 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8448 "bison-chpl-lib.cpp"
    break;

  case 301: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1976 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8456 "bison-chpl-lib.cpp"
    break;

  case 302: /* catch_expr_ls: %empty  */
#line 1982 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8462 "bison-chpl-lib.cpp"
    break;

  case 303: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1983 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8468 "bison-chpl-lib.cpp"
    break;

  case 304: /* catch_expr: TCATCH block_stmt  */
#line 1988 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8476 "bison-chpl-lib.cpp"
    break;

  case 305: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1992 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8484 "bison-chpl-lib.cpp"
    break;

  case 306: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1996 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8492 "bison-chpl-lib.cpp"
    break;

  case 307: /* catch_expr_inner: ident_def  */
#line 2003 "chpl.ypp"
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
#line 8509 "bison-chpl-lib.cpp"
    break;

  case 308: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2016 "chpl.ypp"
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
#line 8526 "bison-chpl-lib.cpp"
    break;

  case 309: /* throw_stmt: TTHROW expr TSEMI  */
#line 2032 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8536 "bison-chpl-lib.cpp"
    break;

  case 310: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2040 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8544 "bison-chpl-lib.cpp"
    break;

  case 311: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2044 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8554 "bison-chpl-lib.cpp"
    break;

  case 312: /* when_stmt_ls: %empty  */
#line 2052 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8560 "bison-chpl-lib.cpp"
    break;

  case 313: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2053 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8566 "bison-chpl-lib.cpp"
    break;

  case 314: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2058 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8574 "bison-chpl-lib.cpp"
    break;

  case 315: /* when_stmt: TOTHERWISE stmt  */
#line 2062 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8583 "bison-chpl-lib.cpp"
    break;

  case 316: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2067 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 8593 "bison-chpl-lib.cpp"
    break;

  case 317: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2076 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8602 "bison-chpl-lib.cpp"
    break;

  case 318: /* manager_expr: expr TAS ident_def  */
#line 2081 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8610 "bison-chpl-lib.cpp"
    break;

  case 319: /* manager_expr: expr  */
#line 2085 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8618 "bison-chpl-lib.cpp"
    break;

  case 320: /* manager_expr_ls: manager_expr  */
#line 2091 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8624 "bison-chpl-lib.cpp"
    break;

  case 321: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2092 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8630 "bison-chpl-lib.cpp"
    break;

  case 322: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2097 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8638 "bison-chpl-lib.cpp"
    break;

  case 323: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2106 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8647 "bison-chpl-lib.cpp"
    break;

  case 324: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2111 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8658 "bison-chpl-lib.cpp"
    break;

  case 325: /* class_start: class_tag ident_def  */
#line 2122 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8666 "bison-chpl-lib.cpp"
    break;

  case 326: /* class_tag: TCLASS  */
#line 2128 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8672 "bison-chpl-lib.cpp"
    break;

  case 327: /* class_tag: TRECORD  */
#line 2129 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8678 "bison-chpl-lib.cpp"
    break;

  case 328: /* class_tag: TUNION  */
#line 2130 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8684 "bison-chpl-lib.cpp"
    break;

  case 329: /* opt_inherit: %empty  */
#line 2134 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8690 "bison-chpl-lib.cpp"
    break;

  case 330: /* opt_inherit: TCOLON expr_ls  */
#line 2135 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8696 "bison-chpl-lib.cpp"
    break;

  case 331: /* class_level_stmt_ls: %empty  */
#line 2139 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8705 "bison-chpl-lib.cpp"
    break;

  case 332: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2144 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8713 "bison-chpl-lib.cpp"
    break;

  case 333: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2148 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8721 "bison-chpl-lib.cpp"
    break;

  case 334: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2152 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8729 "bison-chpl-lib.cpp"
    break;

  case 335: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2156 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8737 "bison-chpl-lib.cpp"
    break;

  case 336: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2164 "chpl.ypp"
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
#line 8759 "bison-chpl-lib.cpp"
    break;

  case 337: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2182 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8773 "bison-chpl-lib.cpp"
    break;

  case 338: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2195 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yylsp[-1]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8781 "bison-chpl-lib.cpp"
    break;

  case 339: /* enum_ls: enum_item  */
#line 2202 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8790 "bison-chpl-lib.cpp"
    break;

  case 340: /* enum_ls: enum_ls TCOMMA  */
#line 2207 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8800 "bison-chpl-lib.cpp"
    break;

  case 341: /* $@5: %empty  */
#line 2213 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8809 "bison-chpl-lib.cpp"
    break;

  case 342: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2218 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8818 "bison-chpl-lib.cpp"
    break;

  case 343: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2223 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8827 "bison-chpl-lib.cpp"
    break;

  case 344: /* $@6: %empty  */
#line 2228 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8836 "bison-chpl-lib.cpp"
    break;

  case 345: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2233 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8845 "bison-chpl-lib.cpp"
    break;

  case 346: /* enum_item: ident_def  */
#line 2243 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8856 "bison-chpl-lib.cpp"
    break;

  case 347: /* enum_item: ident_def TASSIGN expr  */
#line 2250 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8869 "bison-chpl-lib.cpp"
    break;

  case 348: /* lambda_decl_start: TLAMBDA  */
#line 2262 "chpl.ypp"
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
#line 8885 "bison-chpl-lib.cpp"
    break;

  case 349: /* $@7: %empty  */
#line 2279 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8895 "bison-chpl-lib.cpp"
    break;

  case 350: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2285 "chpl.ypp"
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
#line 8917 "bison-chpl-lib.cpp"
    break;

  case 352: /* linkage_spec: linkage_spec_empty  */
#line 2308 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8923 "bison-chpl-lib.cpp"
    break;

  case 353: /* linkage_spec: TINLINE  */
#line 2309 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8930 "bison-chpl-lib.cpp"
    break;

  case 354: /* linkage_spec: TOVERRIDE  */
#line 2311 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8937 "bison-chpl-lib.cpp"
    break;

  case 355: /* opt_fn_type_formal_ls: %empty  */
#line 2316 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8943 "bison-chpl-lib.cpp"
    break;

  case 356: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2317 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8949 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_type_formal_ls: fn_type_formal  */
#line 2321 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8955 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2322 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8961 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_type_formal: named_formal  */
#line 2327 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8967 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2330 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8973 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_type_formal: formal_type  */
#line 2332 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8979 "bison-chpl-lib.cpp"
    break;

  case 362: /* opt_fn_type_ret_type: %empty  */
#line 2336 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8985 "bison-chpl-lib.cpp"
    break;

  case 363: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2337 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8991 "bison-chpl-lib.cpp"
    break;

  case 364: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2350 "chpl.ypp"
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
#line 9010 "bison-chpl-lib.cpp"
    break;

  case 365: /* $@8: %empty  */
#line 2368 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 9020 "bison-chpl-lib.cpp"
    break;

  case 366: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2374 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 9033 "bison-chpl-lib.cpp"
    break;

  case 367: /* $@9: %empty  */
#line 2383 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 9043 "bison-chpl-lib.cpp"
    break;

  case 368: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2389 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 9056 "bison-chpl-lib.cpp"
    break;

  case 369: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2401 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 9064 "bison-chpl-lib.cpp"
    break;

  case 370: /* $@10: %empty  */
#line 2410 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();

    // May never have been built if there was a syntax error.
    if ((yyvsp[-4].functionParts).errorExpr == nullptr) {
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name->name());
    }
  }
#line 9078 "bison-chpl-lib.cpp"
    break;

  case 371: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2420 "chpl.ypp"
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
#line 9101 "bison-chpl-lib.cpp"
    break;

  case 372: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2442 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9113 "bison-chpl-lib.cpp"
    break;

  case 373: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2450 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9125 "bison-chpl-lib.cpp"
    break;

  case 374: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2458 "chpl.ypp"
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
#line 9140 "bison-chpl-lib.cpp"
    break;

  case 375: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2469 "chpl.ypp"
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
#line 9155 "bison-chpl-lib.cpp"
    break;

  case 376: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2480 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 9165 "bison-chpl-lib.cpp"
    break;

  case 377: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2489 "chpl.ypp"
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
#line 9181 "bison-chpl-lib.cpp"
    break;

  case 379: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2504 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 9187 "bison-chpl-lib.cpp"
    break;

  case 382: /* fn_ident: ident_def TBANG  */
#line 2511 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 9197 "bison-chpl-lib.cpp"
    break;

  case 422: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2568 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 9203 "bison-chpl-lib.cpp"
    break;

  case 423: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2569 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9209 "bison-chpl-lib.cpp"
    break;

  case 424: /* opt_formal_ls: %empty  */
#line 2573 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 9215 "bison-chpl-lib.cpp"
    break;

  case 425: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2574 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9221 "bison-chpl-lib.cpp"
    break;

  case 426: /* req_formal_ls: TLP TRP  */
#line 2578 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 9227 "bison-chpl-lib.cpp"
    break;

  case 427: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2579 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9233 "bison-chpl-lib.cpp"
    break;

  case 428: /* formal_ls_inner: formal  */
#line 2583 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9239 "bison-chpl-lib.cpp"
    break;

  case 429: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2584 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9245 "bison-chpl-lib.cpp"
    break;

  case 430: /* formal_ls: %empty  */
#line 2588 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 9251 "bison-chpl-lib.cpp"
    break;

  case 431: /* formal_ls: formal_ls_inner  */
#line 2589 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9257 "bison-chpl-lib.cpp"
    break;

  case 435: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2603 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9265 "bison-chpl-lib.cpp"
    break;

  case 436: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2608 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 9273 "bison-chpl-lib.cpp"
    break;

  case 437: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2613 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9281 "bison-chpl-lib.cpp"
    break;

  case 438: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2618 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 9289 "bison-chpl-lib.cpp"
    break;

  case 439: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2623 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9297 "bison-chpl-lib.cpp"
    break;

  case 440: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2628 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 9305 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_formal_intent_tag: %empty  */
#line 2634 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 9314 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_formal_intent_tag: required_intent_tag  */
#line 2639 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 9323 "bison-chpl-lib.cpp"
    break;

  case 443: /* required_intent_tag: TIN  */
#line 2646 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 9329 "bison-chpl-lib.cpp"
    break;

  case 444: /* required_intent_tag: TINOUT  */
#line 2647 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 9335 "bison-chpl-lib.cpp"
    break;

  case 445: /* required_intent_tag: TOUT  */
#line 2648 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 9341 "bison-chpl-lib.cpp"
    break;

  case 446: /* required_intent_tag: TCONST TIN  */
#line 2649 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 9347 "bison-chpl-lib.cpp"
    break;

  case 447: /* required_intent_tag: TCONST TREF  */
#line 2650 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9353 "bison-chpl-lib.cpp"
    break;

  case 448: /* required_intent_tag: TCONST  */
#line 2651 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9359 "bison-chpl-lib.cpp"
    break;

  case 449: /* required_intent_tag: TPARAM  */
#line 2652 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9365 "bison-chpl-lib.cpp"
    break;

  case 450: /* required_intent_tag: TREF  */
#line 2653 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9371 "bison-chpl-lib.cpp"
    break;

  case 451: /* required_intent_tag: TTYPE  */
#line 2654 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9377 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_this_intent_tag: %empty  */
#line 2658 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9383 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_this_intent_tag: TPARAM  */
#line 2659 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9389 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_this_intent_tag: TREF  */
#line 2660 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9395 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_this_intent_tag: TCONST TREF  */
#line 2661 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9401 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_this_intent_tag: TCONST  */
#line 2662 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9407 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_this_intent_tag: TTYPE  */
#line 2663 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9413 "bison-chpl-lib.cpp"
    break;

  case 458: /* proc_iter_or_op: TPROC  */
#line 2667 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9419 "bison-chpl-lib.cpp"
    break;

  case 459: /* proc_iter_or_op: TITER  */
#line 2668 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9425 "bison-chpl-lib.cpp"
    break;

  case 460: /* proc_iter_or_op: TOPERATOR  */
#line 2669 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9431 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_ret_tag: %empty  */
#line 2673 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9437 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_ret_tag: TOUT  */
#line 2674 "chpl.ypp"
                    { (yyval.returnTag) = Function::OUT; }
#line 9443 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_ret_tag: TCONST  */
#line 2675 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 9449 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_ret_tag: TCONST TREF  */
#line 2676 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 9455 "bison-chpl-lib.cpp"
    break;

  case 465: /* opt_ret_tag: TREF  */
#line 2677 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 9461 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_ret_tag: TPARAM  */
#line 2678 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 9467 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_ret_tag: TTYPE  */
#line 2679 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 9473 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_throws_error: %empty  */
#line 2683 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9479 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_throws_error: TTHROWS  */
#line 2684 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9485 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_function_body_stmt: TSEMI  */
#line 2687 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9491 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_function_body_stmt: function_body_stmt  */
#line 2688 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9497 "bison-chpl-lib.cpp"
    break;

  case 472: /* function_body_stmt: block_stmt_body  */
#line 2692 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9503 "bison-chpl-lib.cpp"
    break;

  case 473: /* function_body_stmt: TDO toplevel_stmt  */
#line 2693 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 9509 "bison-chpl-lib.cpp"
    break;

  case 474: /* function_body_stmt: return_stmt  */
#line 2694 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt), &((yylsp[0]))); }
#line 9515 "bison-chpl-lib.cpp"
    break;

  case 475: /* query_expr: TQUERIEDIDENT  */
#line 2698 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9521 "bison-chpl-lib.cpp"
    break;

  case 476: /* opt_lifetime_where: %empty  */
#line 2703 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9527 "bison-chpl-lib.cpp"
    break;

  case 477: /* opt_lifetime_where: TWHERE expr  */
#line 2705 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9533 "bison-chpl-lib.cpp"
    break;

  case 478: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2707 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9539 "bison-chpl-lib.cpp"
    break;

  case 479: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2709 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9545 "bison-chpl-lib.cpp"
    break;

  case 480: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2711 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9551 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_components_expr: lifetime_expr  */
#line 2716 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9557 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2718 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9563 "bison-chpl-lib.cpp"
    break;

  case 483: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2723 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9569 "bison-chpl-lib.cpp"
    break;

  case 484: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2725 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9575 "bison-chpl-lib.cpp"
    break;

  case 485: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2727 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9581 "bison-chpl-lib.cpp"
    break;

  case 486: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2729 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9587 "bison-chpl-lib.cpp"
    break;

  case 487: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2731 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9593 "bison-chpl-lib.cpp"
    break;

  case 488: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2733 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9599 "bison-chpl-lib.cpp"
    break;

  case 489: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2735 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9605 "bison-chpl-lib.cpp"
    break;

  case 490: /* lifetime_ident: TIDENT  */
#line 2739 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9611 "bison-chpl-lib.cpp"
    break;

  case 491: /* lifetime_ident: TINIT  */
#line 2740 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9617 "bison-chpl-lib.cpp"
    break;

  case 492: /* lifetime_ident: TTHIS  */
#line 2741 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9623 "bison-chpl-lib.cpp"
    break;

  case 493: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2746 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9632 "bison-chpl-lib.cpp"
    break;

  case 494: /* type_alias_decl_stmt_start: TTYPE  */
#line 2755 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9640 "bison-chpl-lib.cpp"
    break;

  case 495: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2759 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9649 "bison-chpl-lib.cpp"
    break;

  case 496: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2767 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9657 "bison-chpl-lib.cpp"
    break;

  case 497: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2771 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9665 "bison-chpl-lib.cpp"
    break;

  case 498: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2778 "chpl.ypp"
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
#line 9687 "bison-chpl-lib.cpp"
    break;

  case 499: /* opt_init_type: %empty  */
#line 2798 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9693 "bison-chpl-lib.cpp"
    break;

  case 500: /* opt_init_type: TASSIGN expr  */
#line 2800 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9699 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_type: TPARAM  */
#line 2804 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9705 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_type: TCONST TREF  */
#line 2805 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9711 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_type: TREF  */
#line 2806 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9717 "bison-chpl-lib.cpp"
    break;

  case 504: /* var_decl_type: TCONST  */
#line 2807 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9723 "bison-chpl-lib.cpp"
    break;

  case 505: /* var_decl_type: TVAR  */
#line 2808 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9729 "bison-chpl-lib.cpp"
    break;

  case 506: /* $@11: %empty  */
#line 2813 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9738 "bison-chpl-lib.cpp"
    break;

  case 507: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2817 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9747 "bison-chpl-lib.cpp"
    break;

  case 508: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2822 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9756 "bison-chpl-lib.cpp"
    break;

  case 509: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2830 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9764 "bison-chpl-lib.cpp"
    break;

  case 510: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2834 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9772 "bison-chpl-lib.cpp"
    break;

  case 511: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2841 "chpl.ypp"
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
#line 9794 "bison-chpl-lib.cpp"
    break;

  case 512: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2859 "chpl.ypp"
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
#line 9814 "bison-chpl-lib.cpp"
    break;

  case 513: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2878 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9822 "bison-chpl-lib.cpp"
    break;

  case 514: /* tuple_var_decl_component: ident_def  */
#line 2882 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9830 "bison-chpl-lib.cpp"
    break;

  case 515: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2886 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9838 "bison-chpl-lib.cpp"
    break;

  case 516: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2893 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9844 "bison-chpl-lib.cpp"
    break;

  case 517: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2895 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9850 "bison-chpl-lib.cpp"
    break;

  case 518: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2897 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9856 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_init_expr: %empty  */
#line 2903 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9862 "bison-chpl-lib.cpp"
    break;

  case 520: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2904 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9868 "bison-chpl-lib.cpp"
    break;

  case 521: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2905 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9874 "bison-chpl-lib.cpp"
    break;

  case 522: /* formal_or_ret_type_expr: expr  */
#line 2909 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9880 "bison-chpl-lib.cpp"
    break;

  case 523: /* ret_type: formal_or_ret_type_expr  */
#line 2913 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9886 "bison-chpl-lib.cpp"
    break;

  case 524: /* ret_type: reserved_type_ident_use  */
#line 2914 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9892 "bison-chpl-lib.cpp"
    break;

  case 525: /* ret_type: error  */
#line 2915 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9898 "bison-chpl-lib.cpp"
    break;

  case 526: /* colon_ret_type: TCOLON ret_type  */
#line 2919 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9904 "bison-chpl-lib.cpp"
    break;

  case 527: /* colon_ret_type: error  */
#line 2920 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9910 "bison-chpl-lib.cpp"
    break;

  case 528: /* opt_ret_type: %empty  */
#line 2924 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9916 "bison-chpl-lib.cpp"
    break;

  case 530: /* opt_type: %empty  */
#line 2929 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9922 "bison-chpl-lib.cpp"
    break;

  case 531: /* opt_type: TCOLON expr  */
#line 2930 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9928 "bison-chpl-lib.cpp"
    break;

  case 532: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2931 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9934 "bison-chpl-lib.cpp"
    break;

  case 533: /* opt_type: error  */
#line 2932 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9940 "bison-chpl-lib.cpp"
    break;

  case 534: /* formal_type: formal_or_ret_type_expr  */
#line 2936 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9946 "bison-chpl-lib.cpp"
    break;

  case 535: /* formal_type: reserved_type_ident_use  */
#line 2937 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9952 "bison-chpl-lib.cpp"
    break;

  case 536: /* colon_formal_type: TCOLON formal_type  */
#line 2941 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9958 "bison-chpl-lib.cpp"
    break;

  case 537: /* opt_colon_formal_type: %empty  */
#line 2945 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9964 "bison-chpl-lib.cpp"
    break;

  case 538: /* opt_colon_formal_type: colon_formal_type  */
#line 2946 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9970 "bison-chpl-lib.cpp"
    break;

  case 539: /* expr_ls: expr  */
#line 2952 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9976 "bison-chpl-lib.cpp"
    break;

  case 540: /* expr_ls: expr_ls TCOMMA expr  */
#line 2953 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9982 "bison-chpl-lib.cpp"
    break;

  case 541: /* tuple_component: TUNDERSCORE  */
#line 2957 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9988 "bison-chpl-lib.cpp"
    break;

  case 542: /* tuple_component: opt_try_expr  */
#line 2958 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9994 "bison-chpl-lib.cpp"
    break;

  case 543: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2963 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 10002 "bison-chpl-lib.cpp"
    break;

  case 544: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2967 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10010 "bison-chpl-lib.cpp"
    break;

  case 545: /* opt_actual_ls: %empty  */
#line 2973 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 10016 "bison-chpl-lib.cpp"
    break;

  case 546: /* opt_actual_ls: actual_ls  */
#line 2974 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 10022 "bison-chpl-lib.cpp"
    break;

  case 547: /* actual_ls: actual_expr  */
#line 2979 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10031 "bison-chpl-lib.cpp"
    break;

  case 548: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2984 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10041 "bison-chpl-lib.cpp"
    break;

  case 549: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2992 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10047 "bison-chpl-lib.cpp"
    break;

  case 550: /* actual_expr: opt_try_expr  */
#line 2993 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10053 "bison-chpl-lib.cpp"
    break;

  case 551: /* ident_expr: ident_use  */
#line 2997 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10059 "bison-chpl-lib.cpp"
    break;

  case 552: /* ident_expr: scalar_type  */
#line 2998 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10065 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TSINGLE expr  */
#line 3011 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10071 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3013 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10077 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3015 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10083 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3017 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10089 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 3019 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 10099 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TATOMIC expr  */
#line 3025 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10105 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TSYNC expr  */
#line 3027 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10111 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TOWNED  */
#line 3030 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10117 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TOWNED expr  */
#line 3032 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10123 "bison-chpl-lib.cpp"
    break;

  case 567: /* sub_type_level_expr: TUNMANAGED  */
#line 3034 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10129 "bison-chpl-lib.cpp"
    break;

  case 568: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3036 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10135 "bison-chpl-lib.cpp"
    break;

  case 569: /* sub_type_level_expr: TSHARED  */
#line 3038 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10141 "bison-chpl-lib.cpp"
    break;

  case 570: /* sub_type_level_expr: TSHARED expr  */
#line 3040 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10147 "bison-chpl-lib.cpp"
    break;

  case 571: /* sub_type_level_expr: TBORROWED  */
#line 3042 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10153 "bison-chpl-lib.cpp"
    break;

  case 572: /* sub_type_level_expr: TBORROWED expr  */
#line 3044 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10159 "bison-chpl-lib.cpp"
    break;

  case 573: /* sub_type_level_expr: TCLASS  */
#line 3046 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10165 "bison-chpl-lib.cpp"
    break;

  case 574: /* sub_type_level_expr: TRECORD  */
#line 3048 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10171 "bison-chpl-lib.cpp"
    break;

  case 575: /* for_expr: no_loop_attributes for_expr_base  */
#line 3052 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 10177 "bison-chpl-lib.cpp"
    break;

  case 576: /* for_expr_base: TFOR expr TIN expr TDO expr  */
#line 3057 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10191 "bison-chpl-lib.cpp"
    break;

  case 577: /* for_expr_base: TFOR expr TIN zippered_iterator TDO expr  */
#line 3067 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10205 "bison-chpl-lib.cpp"
    break;

  case 578: /* for_expr_base: TFOR expr TDO expr  */
#line 3077 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10218 "bison-chpl-lib.cpp"
    break;

  case 579: /* for_expr_base: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 3086 "chpl.ypp"
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
#line 10237 "bison-chpl-lib.cpp"
    break;

  case 580: /* for_expr_base: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3101 "chpl.ypp"
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
#line 10256 "bison-chpl-lib.cpp"
    break;

  case 581: /* for_expr_base: TFOR expr TDO TIF expr TTHEN expr  */
#line 3116 "chpl.ypp"
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
#line 10275 "bison-chpl-lib.cpp"
    break;

  case 582: /* for_expr_base: TFORALL expr TIN expr TDO expr  */
#line 3131 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10289 "bison-chpl-lib.cpp"
    break;

  case 583: /* for_expr_base: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3141 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10303 "bison-chpl-lib.cpp"
    break;

  case 584: /* for_expr_base: TFORALL expr TDO expr  */
#line 3151 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10316 "bison-chpl-lib.cpp"
    break;

  case 585: /* for_expr_base: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3160 "chpl.ypp"
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
#line 10335 "bison-chpl-lib.cpp"
    break;

  case 586: /* for_expr_base: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3175 "chpl.ypp"
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
#line 10354 "bison-chpl-lib.cpp"
    break;

  case 587: /* for_expr_base: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3190 "chpl.ypp"
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
#line 10373 "bison-chpl-lib.cpp"
    break;

  case 588: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3207 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 10379 "bison-chpl-lib.cpp"
    break;

  case 589: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3212 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 10387 "bison-chpl-lib.cpp"
    break;

  case 590: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3216 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 10395 "bison-chpl-lib.cpp"
    break;

  case 591: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3220 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10403 "bison-chpl-lib.cpp"
    break;

  case 592: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3224 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10411 "bison-chpl-lib.cpp"
    break;

  case 593: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3228 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10419 "bison-chpl-lib.cpp"
    break;

  case 594: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3232 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10427 "bison-chpl-lib.cpp"
    break;

  case 595: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3236 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10435 "bison-chpl-lib.cpp"
    break;

  case 596: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3243 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10449 "bison-chpl-lib.cpp"
    break;

  case 597: /* nil_expr: TNIL  */
#line 3260 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10455 "bison-chpl-lib.cpp"
    break;

  case 605: /* opt_task_intent_ls: %empty  */
#line 3278 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10461 "bison-chpl-lib.cpp"
    break;

  case 606: /* opt_task_intent_ls: task_intent_clause  */
#line 3279 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10467 "bison-chpl-lib.cpp"
    break;

  case 607: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3284 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 10475 "bison-chpl-lib.cpp"
    break;

  case 608: /* task_intent_ls: intent_expr  */
#line 3290 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10481 "bison-chpl-lib.cpp"
    break;

  case 609: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3291 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10487 "bison-chpl-lib.cpp"
    break;

  case 610: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3296 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10497 "bison-chpl-lib.cpp"
    break;

  case 611: /* forall_intent_ls: intent_expr  */
#line 3304 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10503 "bison-chpl-lib.cpp"
    break;

  case 612: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3305 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10509 "bison-chpl-lib.cpp"
    break;

  case 613: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
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
#line 10527 "bison-chpl-lib.cpp"
    break;

  case 614: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3324 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10535 "bison-chpl-lib.cpp"
    break;

  case 615: /* intent_expr: expr TREDUCE ident_expr  */
#line 3328 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10543 "bison-chpl-lib.cpp"
    break;

  case 616: /* task_var_prefix: TCONST  */
#line 3334 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10549 "bison-chpl-lib.cpp"
    break;

  case 617: /* task_var_prefix: TIN  */
#line 3335 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10555 "bison-chpl-lib.cpp"
    break;

  case 618: /* task_var_prefix: TCONST TIN  */
#line 3336 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10561 "bison-chpl-lib.cpp"
    break;

  case 619: /* task_var_prefix: TREF  */
#line 3337 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10567 "bison-chpl-lib.cpp"
    break;

  case 620: /* task_var_prefix: TCONST TREF  */
#line 3338 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10573 "bison-chpl-lib.cpp"
    break;

  case 621: /* task_var_prefix: TVAR  */
#line 3339 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10579 "bison-chpl-lib.cpp"
    break;

  case 622: /* new_maybe_decorated: TNEW  */
#line 3344 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10585 "bison-chpl-lib.cpp"
    break;

  case 623: /* new_maybe_decorated: TNEW TOWNED  */
#line 3346 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10591 "bison-chpl-lib.cpp"
    break;

  case 624: /* new_maybe_decorated: TNEW TSHARED  */
#line 3348 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10597 "bison-chpl-lib.cpp"
    break;

  case 625: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3350 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10603 "bison-chpl-lib.cpp"
    break;

  case 626: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3352 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10609 "bison-chpl-lib.cpp"
    break;

  case 627: /* new_expr: new_maybe_decorated expr  */
#line 3358 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10617 "bison-chpl-lib.cpp"
    break;

  case 628: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
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
#line 10633 "bison-chpl-lib.cpp"
    break;

  case 629: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
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
#line 10649 "bison-chpl-lib.cpp"
    break;

  case 630: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
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
#line 10666 "bison-chpl-lib.cpp"
    break;

  case 631: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
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
#line 10684 "bison-chpl-lib.cpp"
    break;

  case 632: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3419 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10692 "bison-chpl-lib.cpp"
    break;

  case 633: /* range_literal_expr: expr TDOTDOT expr  */
#line 3426 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10701 "bison-chpl-lib.cpp"
    break;

  case 634: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3431 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10710 "bison-chpl-lib.cpp"
    break;

  case 635: /* range_literal_expr: expr TDOTDOT  */
#line 3436 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10719 "bison-chpl-lib.cpp"
    break;

  case 636: /* range_literal_expr: TDOTDOT expr  */
#line 3441 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10729 "bison-chpl-lib.cpp"
    break;

  case 637: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3447 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10739 "bison-chpl-lib.cpp"
    break;

  case 638: /* range_literal_expr: TDOTDOT  */
#line 3453 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10749 "bison-chpl-lib.cpp"
    break;

  case 639: /* cast_expr: expr TCOLON expr  */
#line 3483 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10757 "bison-chpl-lib.cpp"
    break;

  case 640: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3490 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10765 "bison-chpl-lib.cpp"
    break;

  case 641: /* super_expr: fn_expr  */
#line 3496 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10771 "bison-chpl-lib.cpp"
    break;

  case 645: /* expr: sub_type_level_expr TQUESTION  */
#line 3505 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10777 "bison-chpl-lib.cpp"
    break;

  case 646: /* expr: TQUESTION  */
#line 3507 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10783 "bison-chpl-lib.cpp"
    break;

  case 650: /* expr: fn_type  */
#line 3512 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10789 "bison-chpl-lib.cpp"
    break;

  case 660: /* opt_expr: %empty  */
#line 3526 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10795 "bison-chpl-lib.cpp"
    break;

  case 661: /* opt_expr: expr  */
#line 3527 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10801 "bison-chpl-lib.cpp"
    break;

  case 662: /* opt_try_expr: TTRY expr  */
#line 3531 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10807 "bison-chpl-lib.cpp"
    break;

  case 663: /* opt_try_expr: TTRYBANG expr  */
#line 3532 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10813 "bison-chpl-lib.cpp"
    break;

  case 664: /* opt_try_expr: super_expr  */
#line 3533 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10819 "bison-chpl-lib.cpp"
    break;

  case 670: /* call_base_expr: expr TBANG  */
#line 3550 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10827 "bison-chpl-lib.cpp"
    break;

  case 671: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3553 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10833 "bison-chpl-lib.cpp"
    break;

  case 674: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
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
#line 10849 "bison-chpl-lib.cpp"
    break;

  case 675: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
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
#line 10865 "bison-chpl-lib.cpp"
    break;

  case 676: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3584 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10873 "bison-chpl-lib.cpp"
    break;

  case 677: /* dot_expr: expr TDOT ident_use  */
#line 3591 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10879 "bison-chpl-lib.cpp"
    break;

  case 678: /* dot_expr: expr TDOT TTYPE  */
#line 3593 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10885 "bison-chpl-lib.cpp"
    break;

  case 679: /* dot_expr: expr TDOT TDOMAIN  */
#line 3595 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10891 "bison-chpl-lib.cpp"
    break;

  case 680: /* dot_expr: expr TDOT TLOCALE  */
#line 3597 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10897 "bison-chpl-lib.cpp"
    break;

  case 681: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3599 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10903 "bison-chpl-lib.cpp"
    break;

  case 682: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3601 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10909 "bison-chpl-lib.cpp"
    break;

  case 683: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3609 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10915 "bison-chpl-lib.cpp"
    break;

  case 684: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3611 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10923 "bison-chpl-lib.cpp"
    break;

  case 685: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3615 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10931 "bison-chpl-lib.cpp"
    break;

  case 686: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3619 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10939 "bison-chpl-lib.cpp"
    break;

  case 687: /* bool_literal: TFALSE  */
#line 3625 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10945 "bison-chpl-lib.cpp"
    break;

  case 688: /* bool_literal: TTRUE  */
#line 3626 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10951 "bison-chpl-lib.cpp"
    break;

  case 689: /* str_bytes_literal: STRINGLITERAL  */
#line 3630 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10957 "bison-chpl-lib.cpp"
    break;

  case 690: /* str_bytes_literal: BYTESLITERAL  */
#line 3631 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10963 "bison-chpl-lib.cpp"
    break;

  case 693: /* literal_expr: INTLITERAL  */
#line 3637 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10969 "bison-chpl-lib.cpp"
    break;

  case 694: /* literal_expr: REALLITERAL  */
#line 3638 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10975 "bison-chpl-lib.cpp"
    break;

  case 695: /* literal_expr: IMAGLITERAL  */
#line 3639 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10981 "bison-chpl-lib.cpp"
    break;

  case 696: /* literal_expr: CSTRINGLITERAL  */
#line 3640 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10987 "bison-chpl-lib.cpp"
    break;

  case 697: /* literal_expr: TNONE  */
#line 3641 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10993 "bison-chpl-lib.cpp"
    break;

  case 698: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3643 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11002 "bison-chpl-lib.cpp"
    break;

  case 699: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3648 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11011 "bison-chpl-lib.cpp"
    break;

  case 700: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3653 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 11020 "bison-chpl-lib.cpp"
    break;

  case 701: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3658 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 11029 "bison-chpl-lib.cpp"
    break;

  case 702: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3663 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 11038 "bison-chpl-lib.cpp"
    break;

  case 703: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3668 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 11047 "bison-chpl-lib.cpp"
    break;

  case 704: /* assoc_expr_ls: expr TALIAS expr  */
#line 3677 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 11056 "bison-chpl-lib.cpp"
    break;

  case 705: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3682 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 11066 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TPLUS expr  */
#line 3690 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11072 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TMINUS expr  */
#line 3691 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11078 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TSTAR expr  */
#line 3692 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11084 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TDIVIDE expr  */
#line 3693 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11090 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3694 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11096 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3695 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11102 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TMOD expr  */
#line 3696 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11108 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TEQUAL expr  */
#line 3697 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11114 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3698 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11120 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3699 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11126 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3700 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11132 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TLESS expr  */
#line 3701 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11138 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr TGREATER expr  */
#line 3702 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11144 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr TBAND expr  */
#line 3703 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11150 "bison-chpl-lib.cpp"
    break;

  case 720: /* binary_op_expr: expr TBOR expr  */
#line 3704 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11156 "bison-chpl-lib.cpp"
    break;

  case 721: /* binary_op_expr: expr TBXOR expr  */
#line 3705 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11162 "bison-chpl-lib.cpp"
    break;

  case 722: /* binary_op_expr: expr TAND expr  */
#line 3706 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11168 "bison-chpl-lib.cpp"
    break;

  case 723: /* binary_op_expr: expr TOR expr  */
#line 3707 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11174 "bison-chpl-lib.cpp"
    break;

  case 724: /* binary_op_expr: expr TEXP expr  */
#line 3708 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11180 "bison-chpl-lib.cpp"
    break;

  case 725: /* binary_op_expr: expr TBY expr  */
#line 3709 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11186 "bison-chpl-lib.cpp"
    break;

  case 726: /* binary_op_expr: expr TALIGN expr  */
#line 3710 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11192 "bison-chpl-lib.cpp"
    break;

  case 727: /* binary_op_expr: expr THASH expr  */
#line 3711 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11198 "bison-chpl-lib.cpp"
    break;

  case 728: /* binary_op_expr: expr TDMAPPED expr  */
#line 3712 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11204 "bison-chpl-lib.cpp"
    break;

  case 729: /* unary_op_expr: TPLUS expr  */
#line 3716 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11210 "bison-chpl-lib.cpp"
    break;

  case 730: /* unary_op_expr: TMINUS expr  */
#line 3717 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11216 "bison-chpl-lib.cpp"
    break;

  case 731: /* unary_op_expr: TMINUSMINUS expr  */
#line 3718 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11222 "bison-chpl-lib.cpp"
    break;

  case 732: /* unary_op_expr: TPLUSPLUS expr  */
#line 3719 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11228 "bison-chpl-lib.cpp"
    break;

  case 733: /* unary_op_expr: TBANG expr  */
#line 3720 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11234 "bison-chpl-lib.cpp"
    break;

  case 734: /* unary_op_expr: expr TBANG  */
#line 3721 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 11242 "bison-chpl-lib.cpp"
    break;

  case 735: /* unary_op_expr: TBNOT expr  */
#line 3724 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11248 "bison-chpl-lib.cpp"
    break;

  case 736: /* reduce_expr: expr TREDUCE expr  */
#line 3729 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11256 "bison-chpl-lib.cpp"
    break;

  case 737: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3733 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11264 "bison-chpl-lib.cpp"
    break;

  case 738: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3737 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11272 "bison-chpl-lib.cpp"
    break;

  case 739: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3741 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11280 "bison-chpl-lib.cpp"
    break;

  case 740: /* scan_expr: expr TSCAN expr  */
#line 3748 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11288 "bison-chpl-lib.cpp"
    break;

  case 741: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3752 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11296 "bison-chpl-lib.cpp"
    break;

  case 742: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3756 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11304 "bison-chpl-lib.cpp"
    break;

  case 743: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3760 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11312 "bison-chpl-lib.cpp"
    break;


#line 11316 "bison-chpl-lib.cpp"

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
