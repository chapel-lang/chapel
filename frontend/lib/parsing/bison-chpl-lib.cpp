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
  YYSYMBOL_TATTRIBUTEIDENT = 5,            /* TATTRIBUTEIDENT  */
  YYSYMBOL_INTLITERAL = 6,                 /* INTLITERAL  */
  YYSYMBOL_REALLITERAL = 7,                /* REALLITERAL  */
  YYSYMBOL_IMAGLITERAL = 8,                /* IMAGLITERAL  */
  YYSYMBOL_STRINGLITERAL = 9,              /* STRINGLITERAL  */
  YYSYMBOL_BYTESLITERAL = 10,              /* BYTESLITERAL  */
  YYSYMBOL_CSTRINGLITERAL = 11,            /* CSTRINGLITERAL  */
  YYSYMBOL_EXTERNCODE = 12,                /* EXTERNCODE  */
  YYSYMBOL_TALIGN = 13,                    /* TALIGN  */
  YYSYMBOL_TAS = 14,                       /* TAS  */
  YYSYMBOL_TATOMIC = 15,                   /* TATOMIC  */
  YYSYMBOL_TBEGIN = 16,                    /* TBEGIN  */
  YYSYMBOL_TBREAK = 17,                    /* TBREAK  */
  YYSYMBOL_TBOOL = 18,                     /* TBOOL  */
  YYSYMBOL_TBORROWED = 19,                 /* TBORROWED  */
  YYSYMBOL_TBY = 20,                       /* TBY  */
  YYSYMBOL_TBYTES = 21,                    /* TBYTES  */
  YYSYMBOL_TCATCH = 22,                    /* TCATCH  */
  YYSYMBOL_TCLASS = 23,                    /* TCLASS  */
  YYSYMBOL_TCOBEGIN = 24,                  /* TCOBEGIN  */
  YYSYMBOL_TCOFORALL = 25,                 /* TCOFORALL  */
  YYSYMBOL_TCOMPLEX = 26,                  /* TCOMPLEX  */
  YYSYMBOL_TCONFIG = 27,                   /* TCONFIG  */
  YYSYMBOL_TCONST = 28,                    /* TCONST  */
  YYSYMBOL_TCONTINUE = 29,                 /* TCONTINUE  */
  YYSYMBOL_TDEFER = 30,                    /* TDEFER  */
  YYSYMBOL_TDELETE = 31,                   /* TDELETE  */
  YYSYMBOL_TDMAPPED = 32,                  /* TDMAPPED  */
  YYSYMBOL_TDO = 33,                       /* TDO  */
  YYSYMBOL_TDOMAIN = 34,                   /* TDOMAIN  */
  YYSYMBOL_TELSE = 35,                     /* TELSE  */
  YYSYMBOL_TENUM = 36,                     /* TENUM  */
  YYSYMBOL_TEXCEPT = 37,                   /* TEXCEPT  */
  YYSYMBOL_TEXPORT = 38,                   /* TEXPORT  */
  YYSYMBOL_TEXTERN = 39,                   /* TEXTERN  */
  YYSYMBOL_TFALSE = 40,                    /* TFALSE  */
  YYSYMBOL_TFOR = 41,                      /* TFOR  */
  YYSYMBOL_TFORALL = 42,                   /* TFORALL  */
  YYSYMBOL_TFOREACH = 43,                  /* TFOREACH  */
  YYSYMBOL_TFORWARDING = 44,               /* TFORWARDING  */
  YYSYMBOL_TIF = 45,                       /* TIF  */
  YYSYMBOL_TIMAG = 46,                     /* TIMAG  */
  YYSYMBOL_TIMPORT = 47,                   /* TIMPORT  */
  YYSYMBOL_TIN = 48,                       /* TIN  */
  YYSYMBOL_TINCLUDE = 49,                  /* TINCLUDE  */
  YYSYMBOL_TINDEX = 50,                    /* TINDEX  */
  YYSYMBOL_TINLINE = 51,                   /* TINLINE  */
  YYSYMBOL_TINOUT = 52,                    /* TINOUT  */
  YYSYMBOL_TINT = 53,                      /* TINT  */
  YYSYMBOL_TITER = 54,                     /* TITER  */
  YYSYMBOL_TINIT = 55,                     /* TINIT  */
  YYSYMBOL_TINITEQUALS = 56,               /* TINITEQUALS  */
  YYSYMBOL_TIMPLEMENTS = 57,               /* TIMPLEMENTS  */
  YYSYMBOL_TINTERFACE = 58,                /* TINTERFACE  */
  YYSYMBOL_TLABEL = 59,                    /* TLABEL  */
  YYSYMBOL_TLAMBDA = 60,                   /* TLAMBDA  */
  YYSYMBOL_TLET = 61,                      /* TLET  */
  YYSYMBOL_TLIFETIME = 62,                 /* TLIFETIME  */
  YYSYMBOL_TLOCAL = 63,                    /* TLOCAL  */
  YYSYMBOL_TLOCALE = 64,                   /* TLOCALE  */
  YYSYMBOL_TMANAGE = 65,                   /* TMANAGE  */
  YYSYMBOL_TMINUSMINUS = 66,               /* TMINUSMINUS  */
  YYSYMBOL_TMODULE = 67,                   /* TMODULE  */
  YYSYMBOL_TNEW = 68,                      /* TNEW  */
  YYSYMBOL_TNIL = 69,                      /* TNIL  */
  YYSYMBOL_TNOINIT = 70,                   /* TNOINIT  */
  YYSYMBOL_TNONE = 71,                     /* TNONE  */
  YYSYMBOL_TNOTHING = 72,                  /* TNOTHING  */
  YYSYMBOL_TON = 73,                       /* TON  */
  YYSYMBOL_TONLY = 74,                     /* TONLY  */
  YYSYMBOL_TOPERATOR = 75,                 /* TOPERATOR  */
  YYSYMBOL_TOTHERWISE = 76,                /* TOTHERWISE  */
  YYSYMBOL_TOUT = 77,                      /* TOUT  */
  YYSYMBOL_TOVERRIDE = 78,                 /* TOVERRIDE  */
  YYSYMBOL_TOWNED = 79,                    /* TOWNED  */
  YYSYMBOL_TPARAM = 80,                    /* TPARAM  */
  YYSYMBOL_TPLUSPLUS = 81,                 /* TPLUSPLUS  */
  YYSYMBOL_TPRAGMA = 82,                   /* TPRAGMA  */
  YYSYMBOL_TPRIMITIVE = 83,                /* TPRIMITIVE  */
  YYSYMBOL_TPRIVATE = 84,                  /* TPRIVATE  */
  YYSYMBOL_TPROC = 85,                     /* TPROC  */
  YYSYMBOL_TPROTOTYPE = 86,                /* TPROTOTYPE  */
  YYSYMBOL_TPUBLIC = 87,                   /* TPUBLIC  */
  YYSYMBOL_TPROCLP = 88,                   /* TPROCLP  */
  YYSYMBOL_TREAL = 89,                     /* TREAL  */
  YYSYMBOL_TRECORD = 90,                   /* TRECORD  */
  YYSYMBOL_TREDUCE = 91,                   /* TREDUCE  */
  YYSYMBOL_TREF = 92,                      /* TREF  */
  YYSYMBOL_TREQUIRE = 93,                  /* TREQUIRE  */
  YYSYMBOL_TRETURN = 94,                   /* TRETURN  */
  YYSYMBOL_TSCAN = 95,                     /* TSCAN  */
  YYSYMBOL_TSELECT = 96,                   /* TSELECT  */
  YYSYMBOL_TSERIAL = 97,                   /* TSERIAL  */
  YYSYMBOL_TSHARED = 98,                   /* TSHARED  */
  YYSYMBOL_TSINGLE = 99,                   /* TSINGLE  */
  YYSYMBOL_TSPARSE = 100,                  /* TSPARSE  */
  YYSYMBOL_TSTRING = 101,                  /* TSTRING  */
  YYSYMBOL_TSUBDOMAIN = 102,               /* TSUBDOMAIN  */
  YYSYMBOL_TSYNC = 103,                    /* TSYNC  */
  YYSYMBOL_TTHEN = 104,                    /* TTHEN  */
  YYSYMBOL_TTHIS = 105,                    /* TTHIS  */
  YYSYMBOL_TTHROW = 106,                   /* TTHROW  */
  YYSYMBOL_TTHROWS = 107,                  /* TTHROWS  */
  YYSYMBOL_TTRUE = 108,                    /* TTRUE  */
  YYSYMBOL_TTRY = 109,                     /* TTRY  */
  YYSYMBOL_TTRYBANG = 110,                 /* TTRYBANG  */
  YYSYMBOL_TTYPE = 111,                    /* TTYPE  */
  YYSYMBOL_TUINT = 112,                    /* TUINT  */
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
#line 326 "chpl.ypp"

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
#define YYLAST   17696

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  182
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  204
/* YYNRULES -- Number of rules.  */
#define YYNRULES  727
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1252

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
       0,   636,   636,   640,   641,   646,   647,   655,   659,   669,
     674,   681,   682,   683,   684,   685,   686,   687,   688,   689,
     690,   691,   709,   710,   711,   719,   728,   737,   741,   748,
     756,   757,   758,   759,   760,   761,   762,   763,   765,   785,
     793,   810,   826,   843,   860,   876,   897,   898,   902,   912,
     913,   917,   921,   922,   926,   933,   941,   944,   954,   956,
     961,   967,   972,   981,   985,  1006,  1023,  1040,  1058,  1059,
    1061,  1066,  1067,  1072,  1090,  1095,  1100,  1112,  1131,  1132,
    1136,  1140,  1144,  1149,  1153,  1157,  1165,  1170,  1176,  1181,
    1190,  1191,  1195,  1200,  1208,  1217,  1225,  1236,  1244,  1245,
    1246,  1247,  1248,  1249,  1250,  1251,  1252,  1253,  1254,  1255,
    1256,  1257,  1258,  1259,  1260,  1272,  1276,  1281,  1286,  1292,
    1301,  1302,  1306,  1315,  1319,  1323,  1327,  1331,  1335,  1344,
    1345,  1346,  1350,  1351,  1352,  1353,  1354,  1358,  1359,  1360,
    1372,  1373,  1374,  1379,  1380,  1381,  1382,  1383,  1384,  1385,
    1386,  1387,  1388,  1389,  1390,  1391,  1392,  1393,  1394,  1395,
    1396,  1397,  1401,  1402,  1403,  1404,  1405,  1406,  1407,  1408,
    1409,  1410,  1411,  1412,  1419,  1420,  1421,  1422,  1426,  1427,
    1431,  1438,  1448,  1452,  1458,  1458,  1464,  1464,  1473,  1474,
    1475,  1476,  1477,  1478,  1479,  1483,  1488,  1493,  1498,  1505,
    1513,  1514,  1518,  1527,  1534,  1543,  1562,  1561,  1571,  1570,
    1583,  1589,  1592,  1593,  1602,  1603,  1604,  1608,  1631,  1650,
    1669,  1674,  1679,  1684,  1709,  1714,  1719,  1724,  1729,  1734,
    1739,  1744,  1749,  1754,  1759,  1764,  1769,  1774,  1779,  1785,
    1794,  1803,  1807,  1811,  1815,  1819,  1823,  1827,  1831,  1835,
    1840,  1845,  1850,  1858,  1873,  1891,  1895,  1902,  1903,  1908,
    1913,  1914,  1915,  1916,  1917,  1918,  1919,  1920,  1921,  1922,
    1923,  1924,  1929,  1934,  1935,  1936,  1937,  1946,  1947,  1951,
    1955,  1959,  1966,  1970,  1974,  1981,  1985,  1989,  1993,  2000,
    2001,  2005,  2009,  2013,  2020,  2033,  2049,  2057,  2062,  2072,
    2073,  2077,  2081,  2086,  2095,  2100,  2104,  2111,  2112,  2116,
    2126,  2132,  2144,  2151,  2152,  2153,  2157,  2158,  2162,  2166,
    2170,  2174,  2178,  2186,  2205,  2218,  2225,  2230,  2237,  2236,
    2246,  2252,  2251,  2266,  2273,  2285,  2303,  2300,  2328,  2332,
    2333,  2335,  2340,  2341,  2345,  2346,  2350,  2353,  2355,  2360,
    2361,  2372,  2392,  2391,  2407,  2406,  2424,  2434,  2431,  2458,
    2466,  2474,  2485,  2496,  2505,  2520,  2521,  2525,  2526,  2527,
    2536,  2537,  2538,  2539,  2540,  2541,  2542,  2543,  2544,  2545,
    2546,  2547,  2548,  2549,  2550,  2551,  2552,  2553,  2554,  2555,
    2556,  2557,  2558,  2559,  2560,  2564,  2565,  2566,  2567,  2568,
    2569,  2570,  2571,  2572,  2573,  2574,  2575,  2580,  2581,  2585,
    2586,  2590,  2591,  2595,  2596,  2600,  2601,  2605,  2606,  2610,
    2614,  2618,  2623,  2628,  2633,  2638,  2646,  2650,  2658,  2659,
    2660,  2661,  2662,  2663,  2664,  2665,  2666,  2670,  2671,  2672,
    2673,  2674,  2675,  2679,  2680,  2681,  2685,  2686,  2687,  2688,
    2689,  2690,  2691,  2695,  2696,  2699,  2700,  2704,  2705,  2709,
    2714,  2715,  2717,  2719,  2721,  2726,  2728,  2733,  2735,  2737,
    2739,  2741,  2743,  2745,  2750,  2751,  2752,  2756,  2765,  2769,
    2777,  2781,  2788,  2810,  2811,  2816,  2817,  2818,  2819,  2820,
    2825,  2824,  2833,  2838,  2846,  2850,  2857,  2876,  2895,  2899,
    2906,  2908,  2910,  2917,  2918,  2919,  2923,  2927,  2928,  2929,
    2933,  2934,  2938,  2939,  2943,  2944,  2945,  2946,  2950,  2951,
    2955,  2959,  2960,  2966,  2967,  2971,  2975,  2979,  2986,  2987,
    2991,  2996,  3005,  3006,  3010,  3011,  3018,  3019,  3020,  3021,
    3022,  3023,  3025,  3027,  3029,  3031,  3037,  3039,  3042,  3044,
    3046,  3048,  3050,  3052,  3054,  3056,  3058,  3060,  3065,  3069,
    3073,  3077,  3081,  3090,  3099,  3111,  3115,  3119,  3123,  3127,
    3131,  3135,  3139,  3146,  3164,  3172,  3173,  3174,  3175,  3176,
    3177,  3178,  3182,  3183,  3187,  3194,  3195,  3199,  3208,  3209,
    3213,  3228,  3232,  3239,  3240,  3241,  3242,  3243,  3244,  3248,
    3250,  3252,  3254,  3256,  3262,  3269,  3281,  3293,  3306,  3323,
    3330,  3335,  3340,  3345,  3351,  3357,  3387,  3394,  3401,  3402,
    3406,  3408,  3409,  3411,  3413,  3414,  3415,  3416,  3419,  3420,
    3421,  3422,  3423,  3424,  3425,  3426,  3427,  3431,  3432,  3436,
    3437,  3438,  3442,  3443,  3444,  3445,  3454,  3455,  3458,  3459,
    3460,  3464,  3476,  3488,  3495,  3497,  3499,  3501,  3503,  3505,
    3514,  3520,  3524,  3528,  3535,  3536,  3540,  3541,  3545,  3546,
    3547,  3548,  3549,  3550,  3551,  3552,  3557,  3562,  3567,  3572,
    3577,  3586,  3591,  3600,  3601,  3602,  3603,  3604,  3605,  3606,
    3607,  3608,  3609,  3610,  3611,  3612,  3613,  3614,  3615,  3616,
    3617,  3618,  3619,  3620,  3621,  3622,  3626,  3627,  3628,  3629,
    3630,  3631,  3634,  3638,  3642,  3646,  3650,  3657,  3661,  3665,
    3669,  3677,  3678,  3679,  3680,  3681,  3682,  3683
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
  "TQUERIEDIDENT", "TATTRIBUTEIDENT", "INTLITERAL", "REALLITERAL",
  "IMAGLITERAL", "STRINGLITERAL", "BYTESLITERAL", "CSTRINGLITERAL",
  "EXTERNCODE", "TALIGN", "TAS", "TATOMIC", "TBEGIN", "TBREAK", "TBOOL",
  "TBORROWED", "TBY", "TBYTES", "TCATCH", "TCLASS", "TCOBEGIN",
  "TCOFORALL", "TCOMPLEX", "TCONFIG", "TCONST", "TCONTINUE", "TDEFER",
  "TDELETE", "TDMAPPED", "TDO", "TDOMAIN", "TELSE", "TENUM", "TEXCEPT",
  "TEXPORT", "TEXTERN", "TFALSE", "TFOR", "TFORALL", "TFOREACH",
  "TFORWARDING", "TIF", "TIMAG", "TIMPORT", "TIN", "TINCLUDE", "TINDEX",
  "TINLINE", "TINOUT", "TINT", "TITER", "TINIT", "TINITEQUALS",
  "TIMPLEMENTS", "TINTERFACE", "TLABEL", "TLAMBDA", "TLET", "TLIFETIME",
  "TLOCAL", "TLOCALE", "TMANAGE", "TMINUSMINUS", "TMODULE", "TNEW", "TNIL",
  "TNOINIT", "TNONE", "TNOTHING", "TON", "TONLY", "TOPERATOR",
  "TOTHERWISE", "TOUT", "TOVERRIDE", "TOWNED", "TPARAM", "TPLUSPLUS",
  "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE", "TPUBLIC",
  "TPROCLP", "TREAL", "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN",
  "TSCAN", "TSELECT", "TSERIAL", "TSHARED", "TSINGLE", "TSPARSE",
  "TSTRING", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS", "TTHROW", "TTHROWS",
  "TTRUE", "TTRY", "TTRYBANG", "TTYPE", "TUINT", "TUNION", "TUNMANAGED",
  "TUSE", "TVAR", "TVOID", "TWHEN", "TWHERE", "TWHILE", "TWITH", "TYIELD",
  "TZIP", "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND", "TASSIGNBOR",
  "TASSIGNBXOR", "TASSIGNDIVIDE", "TASSIGNEXP", "TASSIGNLAND",
  "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD", "TASSIGNMULTIPLY",
  "TASSIGNPLUS", "TASSIGNREDUCE", "TASSIGNSL", "TASSIGNSR", "TATMARK",
  "TBANG", "TBAND", "TBNOT", "TBOR", "TBXOR", "TCOLON", "TCOMMA",
  "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER",
  "TGREATEREQUAL", "THASH", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD",
  "TNOTEQUAL", "TOR", "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT",
  "TSHIFTRIGHT", "TSTAR", "TSWAP", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR",
  "TRSBR", "TNOELSE", "TDOTDOTOPENHIGH", "TUPLUS", "TUMINUS", "TLNOT",
  "$accept", "program", "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls",
  "stmt", "stmt_base", "tryable_stmt", "decl_base", "collect_attributes",
  "attribute_receiver", "attribute_group", "attribute_decl_stmt_ls",
  "attribute_decl_stmt", "attribute_decl_begin",
  "opt_attribute_decl_toolspace", "opt_attribute_actuals",
  "attribute_actuals_ls", "attribute_actual", "module_decl_start",
  "module_decl_stmt", "opt_access_control", "opt_prototype",
  "include_module_stmt", "block_stmt_body", "block_stmt", "stmt_ls",
  "renames_ls", "use_renames_ls", "opt_only_ls", "use_stmt", "import_stmt",
  "import_internal_type_ident", "import_expr", "import_ls", "require_stmt",
  "assignment_stmt", "opt_label_ident", "reserved_word_ident", "ident_def",
  "ident_use", "internal_type_ident_def", "scalar_type",
  "reserved_type_ident_use", "do_stmt", "return_stmt", "class_level_stmt",
  "$@1", "$@2", "inner_class_level_stmt", "forwarding_decl_stmt",
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

#define YYPACT_NINF (-894)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-728)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -894,    96,  4042,  -894,   -67,    91,  -894,  -894,  -894,  -894,
    -894,  -894,  -894,  -894, 11711,    -3,    70,   112, 13572,   149,
   17467,    -3,   171,    -4,    67,    70,  5450, 11711,   125, 17523,
    -894,   141,   322,  -894,  9796,   345,   491,   162,  -894,   359,
     176, 17523, 17523, 17523,  -894,  2847, 10664,   416, 11711, 11711,
     264,  -894,   421,   431, 11711,  -894, 13572,  -894, 11711,   360,
     287,   253,   381, 12922,   436, 17579,  -894, 11711,  7890, 11711,
   10664, 13572, 11711,   284,   446,   293,  5450,   458, 11711,   495,
    6852,  6852,  -894,   509,  -894, 13572,  -894,   520,  9969,  -894,
   11711,  -894, 11711,  -894,  -894, 13399, 11711,  -894, 10142,  -894,
    -894,  -894,  4394,  8063, 11711,  -894,  5098,  -894,  -894,  -894,
    -894, 17117,   582,  -894,   442,   428,  -894,   213,  -894,  -894,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,  7025,  -894,  7198,  -894,  -894,    24,  -894,  -894,  -894,
     556,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,   480,
   17523,  -894,   462,   455,   406,  -894,   211,  -894,  -894,  -894,
     312,   301,  -894,  -894, 17523,  1643,  -894,   463,   472,  -894,
    -894,  -894,   474,   475, 11711,   476,   478,  -894,  -894,  -894,
   16934,  3328,   -88,   485,   486,  -894,  -894,   179,  -894,  -894,
    -894,  -894,  -894,   221,  -894,  -894,  -894, 11711,  -894, 17523,
    -894, 11711, 11711,    72,   581,   329,  -894,  -894,  -894,  -894,
   16934,   341,  -894,  -894,   473,  5450,  -894,  -894,  -894,   487,
     160,   488,  -894,   225,  -894,   492,  -894,   -60, 16934,  8236,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894, 17523,
   17523,   -43,  3850,  -894,  -894,   571,  8236,   496,   494,   435,
    5450,  3575,    74,    12,  -894,  5450,  -894,  -894, 15047,  -894,
      23, 15578,   693,  -894,   501,   504,  -894, 15047,   160,   693,
    -894,  8236,  1389,  1389,    57,    81,  -894,    88,  -894,  -894,
    -894,  -894,  7544,  7544,  -894,   583,  -894,   506,   521,  -894,
    -894, 13795,   531,  -894,  -894, 16934,   219, 11711, 11711,  -894,
     -42,  -894,  -894, 16934,   502, 15618,  -894, 15047,   160, 16934,
     508,  8236,  -894, 16934, 15658,  -894,  -894,  -894,  -894,  -894,
     160,   516,   377,   377,   618,   693,   693,   222,  -894,  -894,
    4570,   -14, 11711,   537,   -72,   512,   618,   679,  -894,  -894,
   17523,  -894,  -894,  4394,  -894,    68,  -894,   571,  -894,  -894,
     686,   517,  4746, 11711,  -894, 11711,   624,  -894,  -894, 14717,
      66,   480, 16934,    79, 11711,  5450,   613,  -894,  -894,  9796,
   10315, 10837,  -894,  -894,  -894, 17523, 11711,   525,  -894, 17263,
     227,   312,  -894,  -894,  -894,  -894,   605,  -894,  -894,  -894,
    -894,    36,   610,  -894,  -894,  -894, 13231,   577,   342,  -894,
     538,   346,   441,  -894,   557, 11711, 11711, 11711, 10837, 10837,
   11711,   448, 11711, 11711, 11711, 11711, 11711,   300, 13399, 11711,
   11711, 11711, 11711, 11711, 11711, 11711, 11711, 11711, 11711, 11711,
   11711, 11711, 11711, 11711, 11711,   638,  -894,  -894,  -894,  -894,
    -894,  9969,  9969,  -894,  -894,  -894,  -894,  9969,  -894,  -894,
    9969,  9969,  8236,  8409, 10837, 10837, 15738,   539,  -894, 11010,
   11711, 17523,  7717,  -894,  -894,  -894,  2847,  -894, 11711,  -894,
     585,   536,   566,  -894,  -894,   589,   590,  5450,   684,  5450,
    -894,   685, 11711,   654,   550,  -894,  9969, 17523,  -894,  -894,
    3575,  -894,  -894,   121,  -894, 11884,   601, 11711,  2847,  -894,
    -894, 11711,  -894, 17319, 11711, 11711,  -894,   559,  -894,  -894,
    -894,  -894, 17523,  -894,   312, 13095,  3575,   587, 11884,  -894,
   16934, 16934,  -894,   488,  -894,    44,  -894,  9969,   561,  -894,
     715,   715,  -894,  -894,  -894,  -894, 11186,  -894, 15778,  8585,
    8758,  -894,  -894,  -894, 10837, 10837,  -894,   442,  8931,  -894,
     258,  -894,  4922,  -894,   351, 15818,   357, 14808, 17523,  6676,
    6503,  -894,   480,   567,  -894,   225,  -894, 15128,   620, 17523,
      48, 15047,   570, 13689,     5,   118, 15898,   139,    25, 14547,
     190,   598,  1888,   576, 17411,   623,    19,  -894,  -894,   144,
    -894,  -894,    61,  -894,  -894,  3275,  -894,   643,  -894,   578,
     602,  -894,   604,  -894,   607,   612,   619,   621,   625,  -894,
     627,   631,   633,   639,   640,   641,   648,  -894,  -894,  -894,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894, 11711,
     628,   650,   578,  -894,   578,  -894, 11711,  -894, 17523,  -894,
    -894,  -894,  1644,  1644,   451,  -894,   451,  -894,   451, 12793,
     982,  1075,  1365,   160,   377,  -894,   449,  -894,  -894,  -894,
    -894,  -894,   618,  2402,   451, 12966, 12966,  1644, 12966, 12966,
     806,   377,  2402, 17094,   806,   693,   693,   377,   618,   608,
     635,   637,   642,   644,   645,   599,   579,  -894,   451,  -894,
     451, 11711,  9969,   102, 15207,   634,   170,  -894,  -894,  -894,
     158,  -894,  2737, 16974,   340,   380, 16934,  9969,  -894,  9969,
   11711, 11711,   769,  5450,   771, 15977,  5450, 15287, 17523,  -894,
     163,  -894,   185,  -894,   202,  3575,    74,  -894, 16934,  9104,
    -894, 16934,  -894,  -894,  -894, 17523, 16018, 16058,  -894,   587,
     662,  -894,   203, 11884,  -894,   232,  -894, 11711,  -894,   647,
       1,   649,  -894,  1968,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,   224, 14626,  -894,  -894,   255,  -894,  -894,  -894,
    -894, 11711,  -894,    69, 13898, 11711,  -894, 11711,  6676,  6503,
    -894,  9277,   367,  9450,   469,   470,  9623,  7371,   489,   393,
    -894,  -894,  -894, 16138,   668,   655,   653,  -894, 17523,  2847,
   10837,    48, 11711,   777,  -894,  -894, 11711, 16934,   658,  5450,
     651, 10837, 11359,  4218,   652, 11711, 11535,  -894,  -894,    48,
    5626, 10837,  -894,    48,  -894,  9969,   660,  1318,  -894,  -894,
   11711,    45,  -894,   584,  -894,   643,  -894,  -894,  -894,  -894,
    -894,     8,   409,  -894, 16178,  -894, 14058,  -894,  -894, 16934,
    -894,   659,   669,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,   260, 10837, 13689, 12057, 10837,  9969,  -894,  -894,  7717,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,    37, 10837, 10837,  -894,  -894,  -894, 16934, 16934,
    5450,  -894,  5450, 11711,  -894,  5450,   791,   670,   678, 17523,
     488,  -894,  -894,   601,  -894,  -894,  -894,   661,   677,   232,
   11711,   643,   587,  -894, 10491,  -894,  -894, 16934,  -894,  5450,
   11711,  -894,  -894,  -894, 17523,  -894,   704,   488, 10837,  5450,
   10837,  9969,  -894,  -894,  -894,  -894,  -894,   449,  -894,  -894,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,  6676,  -894,  -894, 16224, 15379,   401,   689,  6676,
    -894, 11711,  -894,  -894,  2087,   402,   403,    -3, 16330,  -894,
   16376, 11711,   265,  7717,  -894,  5450,   108, 15458,  -894,  -894,
   16934,  5450, 16934,  -894, 16422,  -894,  9796, 16934,    30, 14887,
    -894,   270,  -894,  -894,  -894,  -894,  2278,  2374,  -894, 16934,
   17523,   582,  -894,     8,   169, 11711,  -894,   708,   683,  -894,
     578,   578,  -894,  -894,  -894,   676, 16528, 11711,   825, 16574,
     278,  -894,   601,   285,   290,  -894,  -894, 16934,   824,  5450,
    -894,  -894,  -894,  -894,  -894,  8236,  8236,  -894,  -894,  -894,
    -894,   232, 16934,  -894,   216,    75,   690, 11711,  -894,   168,
   15538,    25, 14968,  -894,   298, 11711,  6676,  6503,  -894,  -894,
    -894, 16614, 16934,  2107,  -894,  -894,  -894,    48,    48,  -894,
   15047,  -894,   295,  -894,  -894,  5802,   691,  5978,   692,  -894,
   11711, 14142,  6154,  -894,    48,  6330,  -894,    48,   699,  2374,
    -894,  -894,  -894, 17411,  -894,  -894,  -894,   242,  -894,   -27,
    -894,   573, 16734,    38,  -894,  -894,  -894, 12230, 12403, 16694,
   12576, 12749,  -894,  -894,  5450,  -894,   695,   697,  -894,  -894,
    5450,  -894,   488, 16934,  5450,  5450,  -894, 16934,   410,   700,
   11711,  -894,  -894,  -894,  -894,  7717,  -894,  9796,  -894, 16934,
    5450,  9796,  -894, 16934,  5450, 16934,  5450,  9796, 16934,  -894,
    9796, 16934,  -894,  -894,  -894,  -894,    64,  -894, 11711,   169,
     242,   242,   242,   242,   242,   242,   169,  5274,  -894,  -894,
    -894,  -894, 11711, 11711, 11711, 11711, 11711,  -894,   709,   710,
     620,  -894,  -894,  -894, 16934,  -894, 14221,  -894, 14302,  -894,
   15977, 14381, 14462,  -894, 16934,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,   725,  -894, 16774, 16814, 16854, 16894,  -894,  -894,
    5450,  5450,  5450,  5450, 11711, 11711, 11711, 11711, 15977, 15977,
   15977, 15977
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   140,   459,    55,   670,   671,
     672,   666,   667,   673,   211,   582,   129,   162,   554,   169,
     556,   582,   168,   490,   488,   129,     0,   211,   277,   163,
     201,   200,   664,   199,   211,   167,    68,   278,   340,   164,
     141,     0,     0,     0,   335,     0,   211,   171,   211,   211,
     599,   574,   674,   172,   211,   341,   548,   485,   211,     0,
       0,   186,   184,   426,   166,   557,   487,   211,   211,   211,
     211,   552,   211,     0,   170,     0,     0,   142,   211,   665,
     211,   211,   478,   165,   315,   550,   489,   173,   211,   723,
     211,   725,   211,   726,   727,   615,   211,   724,   211,   623,
     182,   722,     0,   211,   211,     4,     0,     5,    10,    11,
      12,    48,    51,    52,    56,     0,    46,    71,    13,    77,
      14,    15,    16,    17,    30,   534,   535,    23,    47,   183,
     193,   211,   202,   637,   194,    18,     0,    32,    31,    20,
       0,   272,    19,   633,    22,    36,    33,    34,   192,   316,
       0,   190,     0,     0,   630,   339,     0,   627,   188,   356,
     446,   437,   625,   191,     0,     0,   189,   642,   621,   620,
     624,   538,   536,     0,   211,   631,   632,   636,   635,   634,
       0,   537,     0,   643,   644,   645,   668,   669,   626,   540,
     539,   628,   629,     0,    29,   556,   163,   211,   141,     0,
     557,   211,   211,     0,     0,   630,   642,   536,   631,   632,
     546,   537,   643,   644,     0,     0,   583,   130,   131,     0,
     555,     0,   479,     0,   486,     0,    21,     0,   523,   211,
     137,   143,   155,   149,   148,   157,   134,   147,   158,   144,
     138,   159,   132,   160,   153,   146,   154,   152,   150,   151,
     133,   135,   145,   156,   161,   139,   325,   136,   210,     0,
       0,     0,     0,    70,    69,    71,   211,     0,     0,     0,
       0,     0,     0,     0,   494,     0,   179,    41,     0,   307,
       0,   306,   708,   603,   600,   601,   602,     0,   549,   709,
       7,   211,   338,   338,   433,   176,   428,   177,   429,   430,
     434,   435,   175,   174,   436,   426,   519,     0,   343,   344,
     346,     0,   427,   518,   348,   506,     0,   211,   211,   180,
     627,   618,   641,   619,     0,     0,    44,     0,   553,   541,
       0,   211,    45,   547,     0,   285,    48,   289,   286,   289,
     551,     0,   710,   712,   613,   707,   706,     0,    74,    78,
       0,     0,   211,     0,     0,   525,   614,     0,     6,   313,
       0,   200,   314,     0,    49,     0,     9,    71,    50,    53,
       0,    60,     0,   211,    72,   211,     0,   490,   198,     0,
     666,   316,   638,   206,   211,     0,   214,   215,   216,   211,
     211,   211,   213,   558,   565,     0,   211,     0,   312,     0,
     426,   446,   444,   445,   443,   364,   448,   447,   451,   450,
     452,     0,   441,   438,   439,   442,     0,   483,     0,   480,
       0,     0,   622,    35,   604,   211,   211,   211,   211,   211,
     211,   711,   211,   211,   211,   211,   211,     0,   612,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   395,   402,   403,   404,   399,
     401,   211,   211,   397,   400,   398,   396,   211,   406,   405,
     211,   211,   211,   211,   211,   211,     0,     0,   214,   211,
     211,     0,   211,    37,    25,    38,     0,    26,   211,    39,
     534,     0,   529,   530,   533,     0,     0,     0,   246,     0,
     395,   242,   211,     0,     0,    24,   211,     0,   256,    27,
       0,   498,   500,     0,   517,   211,   503,   211,     0,   178,
      40,   211,   309,     0,   211,   211,    42,     0,   187,   185,
     431,   432,     0,   427,   446,   426,     0,   521,   211,   122,
     639,   640,   354,     0,   181,     0,    43,   211,     0,   296,
     287,   288,    28,    76,    75,    79,   211,   675,     0,   211,
     211,   662,   660,     8,   211,   211,   212,    56,   211,    54,
       0,    65,     0,   120,     0,   115,     0,    86,     0,   211,
     211,   195,   316,     0,   208,     0,   205,   582,     0,     0,
       0,     0,     0,   566,     0,     0,   523,     0,     0,     0,
     283,   317,     0,     0,     0,   333,     0,   326,   413,     0,
     415,   419,     0,   449,   511,     0,   513,   453,   440,   411,
     137,   391,   143,   389,   149,   148,   147,   144,   138,   393,
     159,   160,   146,   150,   133,   145,   161,   388,   370,   373,
     371,   372,   394,   383,   374,   387,   379,   377,   390,   378,
     376,   381,   386,   375,   380,   384,   385,   382,   392,   211,
     367,     0,   411,   368,   411,   365,   211,   482,     0,   477,
     493,   492,   703,   702,   705,   714,   713,   718,   717,   699,
     696,   697,   698,   616,   686,   140,     0,   656,   657,   142,
     655,   654,   610,   690,   701,   695,   693,   704,   694,   692,
     684,   689,   691,   700,   683,   687,   688,   685,   611,     0,
       0,     0,     0,     0,     0,     0,     0,   716,   715,   720,
     719,   211,   211,     0,     0,   283,   593,   594,   596,   598,
       0,   585,     0,     0,     0,     0,   524,   211,   543,   211,
     211,   211,   245,     0,   241,     0,     0,     0,     0,   542,
       0,   259,     0,   257,     0,   501,     0,   516,   515,   211,
     496,   609,   495,   308,   305,     0,     0,     0,   653,   521,
     349,   345,     0,   211,   522,   503,   347,   211,   353,     0,
       0,     0,   544,     0,   290,   676,   617,   661,   526,   525,
     663,   527,     0,     0,    57,    58,     0,    61,    63,    67,
      66,   211,    97,     0,     0,   211,    92,   211,   211,   211,
      64,   211,   370,   211,   371,   372,   211,   211,   382,     0,
     407,   408,    81,    80,    91,     0,     0,   318,     0,     0,
     211,     0,   211,     0,   219,   218,   211,   567,     0,     0,
       0,   211,   211,     0,     0,   211,   211,   679,   230,     0,
       0,   211,   228,     0,   280,   211,     0,   338,   324,   330,
     211,   328,   323,   426,   414,   453,   509,   508,   507,   510,
     454,   460,   426,   363,     0,   369,     0,   359,   360,   484,
     481,     0,     0,   128,   126,   127,   125,   124,   123,   651,
     652,     0,   211,   677,   211,   211,   211,   595,   597,   211,
     584,   162,   169,   168,   167,   164,   171,   172,   166,   170,
     165,   173,     0,   211,   211,   491,   532,   531,   254,   253,
       0,   248,     0,   211,   244,     0,   250,     0,   282,     0,
       0,   499,   502,   503,   504,   505,   304,     0,     0,   503,
     211,   453,   521,   520,   211,   422,   420,   355,   298,     0,
     211,   297,   300,   545,     0,   291,   294,     0,   211,     0,
     211,   211,    59,   121,   118,    98,   110,   104,   103,   102,
     112,    99,   113,   108,   101,   109,   107,   105,   106,   100,
     111,   114,   211,   117,   116,    88,    87,     0,     0,   211,
     196,   211,   197,   318,   338,     0,     0,   582,   582,   222,
       0,   211,     0,   211,   238,     0,     0,     0,   678,   236,
     568,     0,   681,   680,     0,   231,   211,   561,     0,     0,
     229,     0,   311,   186,   184,   310,   338,   338,   319,   334,
       0,     0,   416,   460,     0,   211,   357,   418,     0,   366,
     411,   411,   658,   659,   282,     0,     0,   211,     0,     0,
       0,   586,   503,   642,   642,   247,   243,   573,   249,     0,
      73,   279,   258,   255,   497,   211,   211,   423,   421,   350,
     351,   503,   410,   302,     0,     0,     0,   211,   292,     0,
       0,     0,     0,    62,     0,   211,   211,   211,    93,    95,
      84,    83,    82,   338,   203,   209,   207,     0,     0,   217,
       0,   240,     0,   588,   239,     0,     0,     0,     0,   237,
     211,     0,     0,   226,     0,     0,   224,     0,   284,   338,
     321,   320,   329,     0,   336,   474,   475,     0,   476,   462,
     465,     0,   461,     0,   412,   361,   362,   211,   211,     0,
     211,   211,   284,   590,     0,   252,     0,     0,   425,   424,
       0,   301,     0,   295,     0,     0,   119,    89,     0,     0,
     211,   204,   221,   220,   223,   211,   587,   211,   234,   570,
       0,   211,   232,   569,     0,   682,     0,   211,   560,   227,
     211,   559,   225,   281,   322,   332,     0,   473,   211,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   455,   457,
     358,   456,   211,   211,   211,   211,   211,   251,   605,   606,
     303,   293,    94,    96,    85,   589,     0,   235,     0,   233,
     564,     0,     0,   337,   464,   466,   467,   470,   471,   472,
     468,   469,   463,   458,     0,     0,     0,     0,   607,   608,
       0,     0,     0,     0,   211,   211,   211,   211,   572,   571,
     563,   562
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -894,  -894,  -894,    -1,   -58,  2798,  -894,   233,   765,  -894,
    -894,   457,  -392,  -106,  -894,   310,  -894,  -894,   -83,  -894,
    -894,    28,   614,  -894,  -530,  1817,   510,  -560,  -894,  -786,
    -894,  -894,  -894,    80,  -894,  -894,  -894,   855,  -894,    11,
    -149,  -894,  -894,  -468,   -36,  -894,  -829,  -894,  -894,   237,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,    10,  -894,
    -162,   519,  -288,  -894,   497,  -894,  -894,   -44,  1484,  -894,
    -894,  -894,  -894,   549,  -894,   -64,  -894,  -894,  -894,  -894,
     368,  -894,  -894,  -894,  -104,  -894,  -348,  -788,  -894,  -894,
    -894,  -894,  -894,  -590,  -894,   150,  -894,  -894,  -894,  -894,
    -894,   356,  -894,   124,  -894,  -894,  -894,  -894,   511,  -894,
    -894,  -894,  -894,    16,  -400,  -178,  -769,  -893,  -644,  -894,
      26,  -894,    32,   -61,   594,   -48,  -894,  -894,  -357,  -806,
    -894,  -286,  -894,  -131,  -293,  -285,  -710,  -894,  -894,    77,
     238,  -894,  -198,   776,  -894,  -156,   390,   154,   -65,  -744,
    -594,  -894,  -894,  -894,  -586,  -506,  -894,  -727,   -16,    50,
    -894,  -236,  -489,  -525,    -2,  -894,  -894,  -894,  -894,  -894,
    -894,   373,  -894,   -13,  -894,  -894,  -462,  -894,  -872,  -894,
    -894,   430,   716,  -894,  -894,  -894,  -894,  2674,  -894,   -20,
    1102,  -894,  1362,  1470,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,  -894,  -478
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   349,   106,   107,   108,   109,   110,   365,
     366,   111,   112,   113,   114,   371,   569,   796,   797,   115,
     116,   117,   376,   118,   119,   120,   350,   824,   576,   825,
     121,   122,   983,   573,   574,   123,   124,   219,   255,   272,
     125,   257,   126,   306,   852,   127,   128,   293,   292,   129,
     130,   131,   132,   133,   134,   585,   828,   135,   203,   137,
     391,   392,   594,   138,   261,   139,   752,   753,   204,   141,
     142,   143,   144,   550,   784,   957,   145,   146,   780,   952,
     279,   280,   147,   148,   149,   150,   397,   857,   151,   152,
     606,  1030,  1031,   607,   153,   205,  1186,   155,   156,   307,
     308,   309,   941,   157,   321,   543,   777,   158,   159,  1133,
     160,   161,   661,   662,   820,   821,   822,   945,   873,   401,
     609,  1038,   610,   611,   311,   533,   416,   405,   411,   871,
    1200,  1201,   162,  1036,  1129,  1130,  1131,   163,   164,   418,
     419,   667,   165,   166,   223,   273,   274,   512,   513,   760,
     313,   869,   616,   617,   516,   314,   774,   775,   351,   353,
     354,   491,   492,   493,   206,   168,   169,   393,   170,   394,
     171,   207,   173,   215,   216,   730,   844,  1102,   731,   732,
     174,   208,   209,   177,   178,   179,   322,   180,   383,   494,
     211,   182,   212,   213,   185,   186,   187,   188,   597,   189,
     190,   191,   192,   193
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   105,   310,   471,   734,   305,   369,   604,   221,   421,
     277,   227,   136,   778,   859,   312,   663,   750,   877,   819,
     878,   868,   781,   988,   167,   486,   865,  1051,  1028,   381,
     504,   946,   776,   583,   326,   984,   136,   614,   514,   994,
     256,   480,   939,   798,   612,   779,  1067,   757,   324,   384,
    -331,   316,   268,   269,   270,   527,   275,   385,   275,  1033,
     517,   497,   514,  1112,   265,   386,   387,   388,   341,  -512,
    1034,  1197,   685,   217,   167,   514,   560,   949,   167,   167,
     490,   275,   542,   355,   502,   472,   136,   473,   488,   359,
     136,   136,  1188,   384,  -514,   548,     3,  1197,  -512,   194,
     167,   385,   561,   598,   167,   530,   489,   222,   275,   386,
     387,   388,   136,   478,   387,   388,   136,   490,   214,   950,
    -299,  1189,  -514,  -514,   198,   218,   838,  1035,   363,  -352,
      38,  1103,   840,  -338,   556,  1070,   849,   853,  -277,   367,
     675,   677,   490,  -512,   389,  -278,   838,   867,  -260,   531,
     892,   838,   154,   258,  -338,  -512,   362,    55,   557,   224,
     518,   398,  -299,  -514,  -338,  1028,   841,   861,  -514,  -261,
     933,   521,  1125,   951,   689,   417,   154,   770,  1148,    84,
    -514,   839,   490,   615,   515,  -514,   717,   719,   389,  1064,
     584,   862,   320,  -327,   363,  1068,   363,  1120,  1121,   390,
    -514,   363,  -512,   565,  1198,  1093,  -267,  -512,   515,   363,
     477,  -514,   320,   167,   917,  1071,  -299,  -327,   897,   363,
    1090,   515,  -514,   488,  1126,   136,   154,   320,  -266,   838,
     154,   154,  -514,   891,   826,   363,   715,   716,   664,   838,
    -514,   384,   520,   564,   522,  1125,   363,   479,   987,  1150,
     842,   526,   154,    24,   229,   294,   154,   386,   387,   388,
     373,   266,   898,  1127,  1028,   402,   842,   943,   167,   755,
     495,   496,   958,   167,  1128,   296,   582,   598,   893,   298,
     136,   267,   511,   283,  1105,   136,   403,   846,   691,   838,
    1184,   546,   863,  1215,   843,   756,   404,  1126,   229,   374,
     -70,  1159,   431,   685,   299,    57,   899,   300,  1143,    59,
     437,   739,   474,   335,   338,   847,   475,    66,   864,   301,
     -70,   686,   537,   490,   490,   765,  1052,  1149,   375,   412,
     735,   853,   900,   929,   687,   266,   389,   928,   304,   -70,
     406,    86,   305,   284,  1154,   838,  1069,  1128,   167,   555,
     755,   755,  -650,   320,  -650,   198,   854,   490,   759,   930,
     136,   167,   285,   855,   688,   154,  1021,   488,   -70,   290,
     167,   256,   488,   136,   595,   172,   931,   942,   286,  -275,
     601,   413,   136,   167,   944,   539,   330,   829,   194,   407,
     320,   390,   408,   414,   553,   136,  1135,  1136,   490,   172,
     959,   608,  -265,   961,   409,   689,   600,  1050,   739,   427,
     605,   690,   415,   488,   665,   320,  -262,  1187,   739,   490,
     154,   734,  1084,   410,   194,   154,   739,   660,   -69,   962,
     799,   914,   175,  -592,  1044,   475,   798,   294,  -591,  1101,
    1122,   710,   711,  1165,  1118,   754,   989,   712,   -69,   172,
     713,   714,  1142,   172,   172,   320,   175,   296,  -725,  -592,
     291,   298,  -725,   723,  -591,  1027,   331,   -69,   428,  1166,
    1156,   772,   429,  -269,   310,   172,   663,   305,  -273,   172,
    1226,  1227,  1228,  1229,  1230,  1231,   299,   312,  -270,   300,
     668,    59,   725,  -264,   518,   167,   -69,   167,   369,   801,
     154,   301,  -649,  -268,  -649,   805,   175,   136,   669,   136,
     175,   175,   671,   154,  -646,  -274,  -646,   802,   751,   431,
     304,   511,   154,   806,   435,   734,  1158,   437,   518,   528,
     529,   440,   175,  1185,   764,   154,   175,   336,   336,   789,
     789,   989,   997,   769,  1106,  1108,   915,   511,   792,   989,
     668,   518,  -276,  1006,   834,   835,  1114,  1117,   989,   990,
    -726,  -727,   848,  1018,  -726,  -727,  -263,  1088,  1095,  1096,
     167,   555,  -579,   490,   831,   263,  1212,  -271,   264,  -649,
    -722,  -649,   136,  -417,  -722,   320,   320,     7,   172,   810,
     490,   320,   370,   431,   320,   320,   320,   320,   435,   372,
     833,   437,  1027,  1199,  1045,   440,   363,  1048,   507,   788,
     791,   294,   294,   395,  -648,   605,  -648,  1106,  1108,  1114,
    1117,  -647,   881,  -647,   882,   675,   717,   396,   400,  -576,
     320,   296,   296,   399,  1119,   298,   298,   422,   481,  1123,
    -575,   423,  -580,   172,  -581,   175,   482,   154,   172,   154,
     427,  -578,  -577,   484,   964,   691,  1199,   374,   487,   363,
     299,   299,   505,   300,   300,   357,    59,   506,   544,   535,
    1079,   320,  1081,   996,   524,   301,   301,   525,   538,   417,
     534,   547,   552,   320,   320,   559,   562,   734,   563,   567,
     568,   578,   320,   589,   304,   304,   602,   613,  1041,  1190,
     175,  1027,   618,   666,   670,   175,   490,   437,   709,   428,
     738,   737,   722,   429,   739,   740,   741,   916,   176,   743,
     746,   748,   154,   172,   749,   427,  1191,   759,  1192,  1193,
     912,  1194,  1195,   768,   773,   782,   172,   783,   827,   935,
     832,   167,   176,   836,   167,   172,   488,   490,   858,   860,
     870,   872,  -140,   136,  -162,   890,   136,  -169,   172,   927,
     431,   432,  -168,   433,   434,   435,   511,   436,   437,  -167,
     875,  -164,   440,   889,   883,  -141,   936,  -171,   446,   447,
     175,  -172,   450,  -166,   428,   451,   452,   453,   429,  -170,
    -142,  -165,   176,   175,   956,   999,   176,   176,  -173,  1026,
     876,   884,   175,   885,   920,   305,   922,   896,   886,   940,
     887,   888,   490,  1015,   305,   175,   989,  1020,   176,   948,
    1002,   992,   176,   953,   993,  1001,  1059,  1005,  1011,  1146,
    1147,  1003,  1022,  1042,  1065,   431,  1060,   167,   427,   417,
     435,   167,   436,   437,  1061,  1043,   320,   440,   167,   136,
    1066,  1077,  1137,   136,   447,  1089,   863,  1134,  1140,  1144,
     136,   320,   453,   320,  1152,  1183,  1213,  1170,  1174,  1208,
     172,  1209,   172,  1189,  1238,  1239,   364,   794,  1083,   503,
     225,   963,   572,   320,   566,  1062,   590,   660,   551,   763,
    1076,   771,  1040,   154,   586,  1032,   154,   428,  1037,   532,
    1223,   429,  1124,  1232,  1225,   995,   880,   378,   762,   932,
       0,  1053,  1054,     0,     0,     0,   490,   490,   167,     0,
     167,   369,     0,   167,     0,     0,     0,   175,     0,   175,
     136,   176,   136,   502,  1075,   136,  1026,     0,     0,     0,
     751,     0,     0,     0,     0,   172,     0,   167,   431,   432,
       0,   433,   434,   435,     0,   436,   437,   167,     0,  1074,
     440,     0,     0,     0,     0,   956,     0,   447,     0,   136,
       0,     0,     0,   451,   452,   453,     0,     0,     0,   320,
       0,     0,  1113,  1116,  1097,  1098,   176,     0,     0,   154,
       0,   176,     0,   154,     0,     0,     0,     0,     0,     0,
     154,     0,   175,   167,     0,     0,     0,     0,     0,   167,
       0,     0,     0,   369,   427,   136,     0,   369,     0,     0,
     320,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1026,     0,     0,   502,  1151,
     502,   605,     0,   502,   502,  1113,  1116,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   167,     0,     0,
       0,  1162,  1163,     0,  1164,     0,   176,     0,     0,   136,
     154,     0,   154,   428,     0,   154,     0,   429,  1179,   176,
       0,  1182,     0,     0,     0,   320,     0,     0,   176,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   154,
       0,   176,     0,   167,   181,   167,     0,   427,     0,   154,
     167,     0,     0,   167,     0,   136,   172,   136,     0,   172,
       0,     0,   136,     0,   431,   136,     0,     0,   181,   435,
       0,   436,   437,     0,   605,     0,   440,     0,     0,     0,
       0,     0,   167,   447,     0,     0,     0,     0,   167,   451,
     452,   453,   167,   167,   136,   154,     0,     0,     0,     0,
     136,   154,     0,     0,   136,   136,   428,     0,   167,     0,
     429,     0,   167,   175,   167,     0,   175,     0,   181,     0,
     136,     0,   181,   181,   136,     0,   136,     0,     0,   320,
     320,     0,     0,     0,     0,   167,  1233,     0,     0,     0,
       0,     0,     0,     0,   181,     0,     0,   136,   181,   154,
       0,     0,   172,   176,     0,   176,   172,   431,   432,     0,
       0,   434,   435,   172,   436,   437,     0,     0,     0,   440,
       0,     0,     0,     0,     0,     0,   447,     0,   167,   167,
     167,   167,   451,   452,   453,     0,     0,     0,     0,     0,
     136,   136,   136,   136,     0,   154,     0,   154,     0,     0,
       0,     0,   154,     0,     0,   154,     0,     0,     0,   175,
       0,     0,     0,   175,     0,     0,     0,     0,     0,     0,
     175,     0,     0,     0,     0,     0,     0,     0,   176,     0,
       0,     0,     0,   172,   154,   172,     0,     0,   172,     0,
     154,     0,     0,     0,   154,   154,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   181,     0,     0,
     154,     0,   172,     7,   154,     0,   154,     0,     0,     0,
       0,     0,   172,     0,     0,     0,     0,     0,     0,     0,
       0,   359,     0,     0,     0,    23,    24,   154,     0,     0,
     175,     0,   175,     0,   360,   175,    30,   361,     0,     0,
       0,     0,    33,     0,   183,     0,     0,     0,     0,    38,
       0,     0,   181,     0,     0,     0,     0,   181,   172,   175,
       0,     0,     0,     0,   172,     0,     0,     0,   183,   175,
     154,   154,   154,   154,     0,     0,    55,   427,    57,     0,
      59,     0,  1023,     0,     0,  1024,     0,     0,   362,     0,
      66,     0,   359,     0,     0,     0,    23,    24,     0,     0,
       0,     0,     0,     0,     0,   360,     0,    30,   361,    82,
       0,    84,   172,    33,    86,   175,     0,     0,   183,     0,
      38,   175,   183,   183,     0,     0,     0,     0,     0,     0,
       0,     0,   181,     0,     0,     0,   428,     0,     0,   176,
     429,     0,   176,     0,   183,   181,     0,    55,   183,    57,
       0,     0,   184,     0,   181,     0,     0,     0,   172,   362,
     172,    66,     0,     0,   100,   172,   140,   181,   172,   175,
    1025,     0,     0,     0,     0,     0,   184,     0,     0,     0,
      82,     0,    84,     0,     0,    86,     0,   431,   432,     0,
     140,     0,   435,     0,   436,   437,     0,   172,     0,   440,
       0,     0,     0,   172,     0,     0,   447,   172,   172,     0,
       0,     0,   451,   452,   453,   175,     0,   175,     0,     0,
       0,     0,   175,   172,     0,   175,   184,   172,     0,   172,
     184,   184,     0,     0,     0,   176,     0,     0,     0,   176,
     140,     0,     0,     0,     0,     0,   176,     0,     0,     0,
     172,     0,   184,     0,   175,     0,   184,   183,     0,     0,
     175,     0,     0,     0,   175,   175,   140,     0,     0,     0,
     140,     0,     0,     0,     0,     0,     0,     0,     0,   181,
     175,   181,     0,     0,   175,     0,   175,     0,     0,     0,
       0,     0,     0,   172,   172,   172,   172,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   175,     0,     0,
       0,     0,   183,     0,     0,     0,   176,   183,   176,     0,
       0,   176,     0,     0,   420,     0,   230,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   231,   232,     0,   233,   176,     0,     0,     0,   234,
     175,   175,   175,   175,   181,   176,   427,   235,     0,     0,
       0,     0,     0,   236,     0,   184,     0,     0,     0,   237,
       0,     0,     0,   238,     0,     0,   239,     0,   240,   140,
       0,     0,     0,     0,     0,     0,     0,   241,     0,     0,
       0,     0,   183,     0,   242,   243,     0,     0,     0,     0,
       0,   176,   244,     0,     0,   183,     0,   176,     0,     0,
       0,     0,   245,     0,   183,   428,     0,     0,     0,   429,
     184,   246,   247,     0,   248,   184,   249,   183,   250,     0,
       0,   251,     0,     0,   140,   252,     0,   253,     0,   140,
     254,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,   176,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   431,   432,     0,   433,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
     442,     0,   444,   445,   446,   447,   448,   449,   450,     0,
       0,   451,   452,   453,     0,     0,   271,     0,     0,     0,
     184,   176,   454,   176,     0,     0,     0,     0,   176,     0,
       0,   176,     0,   184,   140,     0,     0,     0,     0,     0,
       0,     0,   184,     0,     0,   181,     0,   140,   181,     0,
       0,     0,     0,     0,     0,   184,   140,     0,     0,   183,
     176,   183,     0,   276,     0,     0,   176,     0,     0,   140,
     176,   176,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   176,   276,     0,   856,
     176,     0,   176,  -318,     0,     0,     0,   337,   339,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -318,     0,   176,     0,  -318,  -318,     0,     0,     0,
       0,     0,     0,     0,  -318,     0,  -318,  -318,   368,     0,
       0,     0,  -318,     0,   183,     0,     0,     0,     0,  -318,
       0,   181,  -318,     0,     0,   181,     0,     0,     0,     0,
       0,     0,   181,     0,     0,     0,   176,   176,   176,   176,
       0,     0,     0,  -318,     0,     0,  -318,   184,  -318,   184,
    -318,   230,  -318,  -318,     0,  -318,     0,     0,  -318,     0,
    -318,   140,     0,   140,     0,     0,   231,   232,     0,   233,
       0,     0,     0,     0,   234,     0,     0,     0,     0,  -318,
       0,  -318,   235,     0,  -318,     0,     0,     0,   236,     0,
       0,     0,     0,     0,   237,     0,     0,     0,   238,     0,
       0,   239,   181,   240,   181,     0,     0,   181,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,   485,   242,
     243,     0,   184,     0,     0,     0,     0,   244,     0,     0,
       0,   181,     0,     0,  -318,     0,   140,   245,     0,     0,
    -318,   181,     0,     0,     0,     0,   246,   247,     0,   248,
       0,   249,     0,   250,     0,     0,   251,     0,   498,   501,
     252,     0,   253,     0,     0,   254,   508,     0,     0,     0,
       0,     0,     7,     0,     0,   276,     0,   276,     0,     0,
       0,     0,     0,     0,   276,   183,     0,   181,   183,     0,
     359,     0,     7,   181,    23,    24,     0,     0,     0,     0,
       0,     0,     0,   360,     0,    30,   361,     0,     0,     0,
     359,    33,     0,     0,    23,    24,     0,     0,    38,   363,
       0,   954,     0,   360,   276,    30,   361,     0,     0,     0,
       0,    33,     0,     0,     0,     0,     0,     0,    38,     0,
       0,   181,     0,     0,     0,    55,     0,    57,     0,    59,
       0,  1023,     0,     0,  1024,     0,     0,   362,     0,    66,
       0,     0,     0,     0,     0,    55,     0,    57,     0,    59,
       0,  1023,     0,     0,  1024,     0,     0,   362,    82,    66,
      84,   183,     0,    86,     0,   183,     0,   181,     0,   181,
       0,     0,   183,   184,   181,     0,   184,   181,    82,     0,
      84,     0,     0,    86,     0,     0,     0,   140,     0,     0,
     140,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   181,     0,     0,     0,
       0,     0,   181,   100,     0,     0,   181,   181,     0,  1094,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   181,   100,     0,     0,   181,     0,   181,  1161,
       0,     0,   183,     7,   183,     0,     0,   183,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   181,
       0,   359,     0,     0,     0,    23,    24,     0,     0,   184,
       0,   183,     0,   184,   360,     0,    30,   361,     0,     0,
     184,   183,    33,   140,     0,     0,     0,   140,     0,    38,
       0,     0,     0,     0,   140,     0,     0,     0,     0,     0,
       0,     0,   181,   181,   181,   181,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
     357,     0,  1023,     0,     0,  1024,     0,   183,   362,     0,
      66,     0,     0,   183,     0,     0,     0,     0,     0,     7,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
     184,    84,   184,     0,    86,   184,     0,   359,     0,     0,
       0,    23,    24,     0,   140,     0,   140,   276,   276,   140,
     360,     0,    30,   361,     0,   276,   276,     0,    33,   184,
       0,   183,     0,     0,     0,    38,     0,     0,     0,   184,
       0,     0,     0,   140,   427,     0,     0,     0,     0,     0,
       0,     0,     0,   140,   100,     0,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,     0,     0,  1023,     0,
       0,  1024,     0,     0,   362,     0,    66,   183,     0,   183,
       0,     0,     0,     0,   183,   184,     0,   183,     0,     0,
       0,   184,     0,     0,     0,    82,     0,    84,     0,   140,
      86,     0,     0,   428,     0,   140,     0,   429,     0,     0,
       0,     0,     0,     0,     0,     0,   183,     0,     0,     0,
       0,     0,   183,     0,     0,     0,   183,   183,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   184,
       0,     0,   183,     0,     0,     0,   183,     0,   183,     0,
     100,     0,     0,   140,   431,   432,     0,   433,   434,   435,
       0,   436,   437,   438,     0,     0,   440,   441,   442,   183,
     444,   445,   446,   447,   926,     0,   450,     0,     0,   451,
     452,   453,     0,     0,     0,   184,     0,   184,     0,     0,
     454,     0,   184,     0,     0,   184,     0,     0,     0,   140,
       0,   140,     0,     0,     0,     0,   140,     0,     0,   140,
     955,     0,   183,   183,   183,   183,     0,     0,     0,     0,
     276,     0,     0,     0,   184,     0,     0,     0,     0,     0,
     184,     0,     0,     0,   184,   184,     0,     0,   140,     0,
       0,     0,     0,     0,   140,     0,     0,     0,   140,   140,
     184,     0,     0,     0,   184,     0,   184,     0,   276,     0,
       0,     0,     0,     0,   140,     0,     0,     0,   140,     0,
     140,     0,     0,     0,     0,     0,   276,   184,     0,     0,
     276,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   140,     0,     0,     0,     0,     0,     0,   210,     0,
       0,     0,   220,     0,     0,     0,     0,     0,     0,     0,
       0,   228,     0,     0,     0,     0,     0,     0,   262,     0,
     184,   184,   184,   184,     0,     0,     0,     0,     0,     0,
     278,     0,   281,   282,   140,   140,   140,   140,   287,     0,
     288,     0,   289,     0,     0,     0,     0,   315,     0,     0,
     685,   228,   323,   325,   327,   328,   329,  1063,     0,     0,
     333,     0,   334,     0,     0,   901,     0,     0,   902,   340,
       0,     0,   323,   903,   342,     0,   343,     0,     0,   344,
     345,     0,   346,   196,  1078,     0,   228,   323,   356,     0,
       0,     0,     0,   904,     0,     0,     0,     0,     0,     0,
     905,     0,   198,     0,     0,     0,     0,     0,     0,     0,
       0,   906,     0,     0,     0,   379,     0,   382,     0,   907,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   226,     0,   908,     0,     0,     0,
       0,     0,     0,     0,     0,   276,   276,     0,   909,     0,
       0,     0,   689,     0,     0,     0,     0,     0,   424,   910,
     230,     0,     0,     0,   911,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   231,   232,     0,   233,     0,
       0,   476,     0,   234,   332,   333,   228,     0,     0,     0,
       0,   235,     0,     0,     0,     0,     0,   236,     0,     0,
       0,     0,   276,   237,     0,     0,     0,   238,   276,   276,
     239,     0,   240,   323,   358,     0,     0,     0,     0,     0,
       0,   241,     0,     0,   276,   276,     0,   276,   242,   243,
       0,     0,     0,     0,     0,     0,   244,     0,   501,     0,
       0,   276,     0,     0,   276,     0,   245,     0,     0,     0,
     323,     0,     0,     0,     0,   246,   247,     0,   248,     0,
     249,     0,   250,     0,     0,   251,     0,     0,     0,   252,
       0,   253,     0,     0,   254,   323,     0,     0,     0,  1211,
       0,     0,     0,     0,     0,     0,   329,   333,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   540,   541,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   323,     0,     0,     0,     0,
       0,     0,     0,   483,     0,     0,     0,     0,     0,     0,
     271,     0,     0,     0,     0,     0,   558,     0,     0,     0,
       0,     0,     0,   501,     0,   501,     0,     0,   501,   501,
       0,     0,     0,     0,     0,     0,     0,   575,     0,   577,
       0,     0,     0,     0,     0,     0,     0,     0,   587,     0,
       0,     0,     0,   591,   596,   599,     0,     0,   509,     0,
     228,     0,     0,   519,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   672,
     673,   674,   676,   678,   679,     0,   680,   681,   682,   683,
     684,     0,   692,   693,   694,   695,   696,   697,   698,   699,
     700,   701,   702,   703,   704,   705,   706,   707,   708,     0,
       0,     0,     0,     0,     0,   323,   323,     0,     0,     0,
       0,   323,     0,     0,   323,   323,   323,   323,   718,   720,
       0,     0,     0,   596,   724,     0,   733,     0,     0,     0,
       0,     0,   736,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   745,     0,     0,   747,     0,     0,     0,
     323,     0,     0,   588,     0,     0,     0,     0,     0,   758,
       0,   761,     0,     0,     0,   281,     0,     0,   766,   767,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   315,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,   323,     0,     0,     0,     0,     0,     0,     0,     0,
     736,     0,     0,   323,   323,     0,     0,     0,   228,   793,
       0,     0,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   823,   823,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   837,     0,     0,
       0,     0,     0,     0,     0,     0,   866,     0,     5,     6,
       0,     8,     9,    10,    11,    12,    13,     0,     0,   315,
      14,     0,     0,    17,    18,   742,    19,   744,   195,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,   295,
       0,   196,     0,     0,     0,    32,  -211,  -211,  -211,     0,
     197,    35,     0,     0,     0,   297,     0,     0,    39,     0,
     198,     0,   199,   874,     0,    44,    45,     0,     0,    47,
     879,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   200,     0,     0,     0,     0,
       0,     0,     0,    71,   302,    73,    74,    75,   303,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,    85,
       0,     0,    87,     0,     0,   745,   323,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   323,     0,   323,   918,   919,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,   323,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,   315,   103,     0,
    -211,   947,     0,   104,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,   468,   469,     0,
       0,     0,     0,     0,     0,   575,     0,     0,     0,   985,
       0,   986,   823,   823,     0,   342,     0,   343,     0,     0,
     345,   346,     0,     0,     0,     0,     0,     0,   470,     0,
       0,  -646,     0,  -646,   998,     0,  1000,     0,     0,     0,
     228,     0,     0,     0,     0,  1007,   736,  1010,     0,  1012,
    1014,     0,     0,     0,  1017,  1019,     0,     0,     0,   323,
       0,     0,     0,     0,  1029,     0,     0,     0,     0,     0,
       0,   921,     0,     0,   924,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1046,  1010,  1017,  1049,
     323,     0,     0,   733,     0,     0,     0,     0,   230,     0,
       0,     0,     0,     0,     0,     0,     0,   676,   718,     0,
       0,     0,     0,   231,   232,     0,   233,  1057,     0,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,   235,
       0,     0,     0,     0,   315,   236,     0,     0,  1072,     0,
       0,   237,     0,     0,   228,   238,     0,     0,   239,     0,
     240,     0,  1080,     0,  1082,   323,     0,  1004,     0,   241,
       0,  1009,     0,     0,     0,     0,   242,   243,   519,     0,
       0,     0,     0,     0,   244,     0,   823,     0,     0,     0,
       0,     0,     0,  1091,   245,  1092,     0,     0,     0,     0,
       0,     0,     0,   246,   247,  1100,   248,   733,   249,     0,
     250,     0,     0,   251,     0,     0,     0,   252,     0,   253,
    1111,     0,   254,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1132,
       0,     0,     0,     0,     0,     0,     0,     0,  1055,     0,
    1056,  1139,     0,  1058,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   323,
     323,     0,     0,     0,     0,     0,     0,  1073,   510,     0,
       0,  1153,     0,     0,     0,     0,     0,  1009,     0,  1157,
     823,   823,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1169,
       0,  1173,     0,     0,  1175,     0,  1178,     0,     0,  1181,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1104,     0,     0,     0,     0,     0,  1109,
       0,  1169,  1173,     0,  1178,  1181,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1214,     0,     0,     0,     0,   733,
       0,  1216,     0,     0,     0,  1218,     0,     0,     0,     0,
    1220,  1221,     0,     0,  1222,     0,     0,  1145,     0,     0,
       0,     0,  1224,   425,     0,     0,     0,     0,     0,     0,
     426,     0,     0,     0,     0,     0,  1234,  1235,  1220,  1236,
    1237,     0,   427,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1168,     0,  1172,     0,     0,     0,     0,
     519,     0,     0,   519,  1248,  1249,  1250,  1251,  1248,  1249,
    1250,  1251,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   428,  1207,     0,     0,   429,     0,     0,  1210,     0,
       0,     0,  1168,  1172,   499,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1217,     0,
       0,     0,  1219,     0,   744,   430,   500,   456,   457,   458,
     459,   460,     0,     0,   463,   464,   465,   466,     0,   468,
     469,     0,   431,   432,     0,   433,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,     0,     0,   451,   452,   453,
       0,   363,     0,     0,     0,     0,     0,     0,   454,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   744,   744,
     744,   744,    -2,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,    15,    16,
      17,    18,     0,    19,     0,    20,    21,  -211,    22,    23,
      24,    25,    26,    27,     0,  -211,    28,     0,    29,     0,
      30,    31,    32,  -211,  -211,  -211,    33,    34,    35,   -68,
       0,    36,    37,    38,     0,    39,  -338,    40,     0,    41,
      42,    43,    44,    45,     0,    46,    47,    48,    49,   -68,
      50,    51,     0,    52,    53,    54,     0,  -338,     0,     0,
      55,    56,    57,    58,    59,    60,    61,  -338,   -68,    62,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,    76,     0,    77,    78,     0,
      79,    80,    81,    82,    83,    84,    85,   -68,    86,    87,
       0,     0,  -211,     0,    88,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,     0,   103,     0,  -211,     0,     4,
     104,     5,     6,     7,     8,     9,    10,    11,    12,    13,
       0,  -677,     0,    14,    15,    16,    17,    18,  -677,    19,
       0,    20,    21,  -211,    22,    23,    24,    25,    26,    27,
    -677,  -211,    28,  -677,    29,     0,    30,    31,    32,  -211,
    -211,  -211,    33,    34,    35,   -68,     0,    36,    37,    38,
       0,    39,  -338,    40,     0,    41,    42,    43,    44,    45,
       0,    46,    47,    48,    49,   -68,    50,    51,     0,    52,
      53,    54,     0,  -338,     0,     0,    55,    56,    57,    58,
       0,    60,    61,  -338,   -68,    62,    63,    64,    65,  -677,
      66,    67,    68,  -677,    69,    70,    71,    72,    73,    74,
      75,    76,     0,    77,    78,     0,    79,    80,    81,    82,
      83,    84,    85,   -68,    86,    87,     0,     0,  -211,     0,
      88,     0,     0,  -677,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,  -677,    92,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
       0,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,    99,   100,  -677,  -677,  -677,     0,   102,
    -677,   103,     0,  -211,     0,   347,  -677,     5,     6,     7,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
      15,    16,    17,    18,     0,    19,     0,    20,    21,  -211,
      22,    23,    24,    25,    26,    27,     0,  -211,    28,     0,
      29,     0,    30,    31,    32,  -211,  -211,  -211,    33,    34,
      35,   -68,     0,    36,    37,    38,     0,    39,  -338,    40,
       0,    41,    42,    43,    44,    45,     0,    46,    47,    48,
      49,   -68,    50,    51,     0,    52,    53,    54,     0,  -338,
       0,     0,    55,    56,    57,    58,    59,    60,    61,  -338,
     -68,    62,    63,    64,    65,     0,    66,    67,    68,     0,
      69,    70,    71,    72,    73,    74,    75,    76,     0,    77,
      78,     0,    79,    80,    81,    82,    83,    84,    85,   -68,
      86,    87,     0,     0,  -211,     0,    88,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     100,     0,     0,   101,     0,   102,   348,   103,     0,  -211,
       0,     4,   104,     5,     6,     7,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,    15,    16,    17,    18,
       0,    19,     0,    20,    21,  -211,    22,    23,    24,    25,
      26,    27,     0,  -211,    28,     0,    29,     0,    30,    31,
      32,  -211,  -211,  -211,    33,    34,    35,   -68,     0,    36,
      37,    38,     0,    39,  -338,    40,     0,    41,    42,    43,
      44,    45,     0,    46,    47,    48,    49,   -68,    50,    51,
       0,    52,    53,    54,     0,  -338,     0,     0,    55,    56,
      57,    58,    59,    60,    61,  -338,   -68,    62,    63,    64,
      65,     0,    66,    67,    68,     0,    69,    70,    71,    72,
      73,    74,    75,    76,     0,    77,    78,     0,    79,    80,
      81,    82,    83,    84,    85,   -68,    86,    87,     0,     0,
    -211,     0,    88,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   100,     0,     0,   101,
       0,   102,   554,   103,     0,  -211,     0,   570,   104,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,    15,    16,    17,    18,     0,    19,     0,    20,
      21,  -211,    22,    23,    24,    25,    26,    27,     0,  -211,
      28,     0,    29,     0,    30,    31,    32,  -211,  -211,  -211,
      33,    34,    35,   -68,     0,    36,    37,    38,     0,    39,
    -338,    40,     0,    41,    42,    43,    44,    45,     0,    46,
      47,    48,    49,   -68,    50,    51,     0,    52,    53,    54,
       0,  -338,     0,     0,    55,    56,    57,    58,    59,    60,
      61,  -338,   -68,    62,    63,    64,    65,     0,    66,    67,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,    77,    78,     0,    79,    80,    81,    82,    83,    84,
      85,   -68,    86,    87,     0,     0,  -211,     0,    88,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,   571,   103,
       0,  -211,     0,     4,   104,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,    15,    16,
      17,    18,     0,    19,     0,    20,    21,  -211,    22,    23,
      24,    25,    26,    27,     0,  -211,    28,     0,    29,     0,
      30,    31,    32,  -211,  -211,  -211,    33,    34,    35,   -68,
       0,    36,    37,    38,     0,    39,  -338,    40,     0,    41,
      42,    43,    44,    45,     0,    46,    47,    48,    49,   -68,
      50,    51,     0,    52,    53,    54,     0,  -338,     0,     0,
      55,    56,    57,    58,    59,    60,    61,  -338,   -68,    62,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,    76,     0,    77,    78,     0,
      79,    80,    81,    82,    83,    84,    85,   -68,    86,    87,
       0,     0,  -211,     0,    88,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,   800,   103,     0,  -211,     0,     4,
     104,     5,     6,     7,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,    15,    16,    17,    18,     0,    19,
       0,    20,    21,  -211,    22,    23,    24,    25,    26,    27,
       0,  -211,    28,     0,    29,     0,    30,    31,    32,  -211,
    -211,  -211,    33,    34,    35,   -68,     0,    36,    37,    38,
       0,    39,  -338,    40,     0,    41,    42,    43,    44,    45,
       0,    46,    47,    48,    49,   -68,    50,    51,     0,    52,
      53,    54,     0,  -338,     0,     0,    55,    56,    57,    58,
     357,    60,    61,  -338,   -68,    62,    63,    64,    65,     0,
      66,    67,    68,     0,    69,    70,    71,    72,    73,    74,
      75,    76,     0,    77,    78,     0,    79,    80,    81,    82,
      83,    84,    85,   -68,    86,    87,     0,     0,  -211,     0,
      88,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
       0,   103,     0,  -211,     0,     4,   104,     5,     6,     7,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
      15,    16,    17,    18,     0,    19,     0,    20,    21,  -211,
      22,    23,    24,    25,    26,    27,     0,  -211,    28,     0,
      29,     0,    30,    31,    32,  -211,  -211,  -211,    33,    34,
      35,   -68,     0,    36,    37,    38,     0,    39,  -338,    40,
       0,    41,    42,    43,    44,    45,     0,    46,    47,    48,
      49,   -68,    50,    51,     0,    52,    53,    54,     0,  -338,
       0,     0,    55,    56,    57,    58,    59,    60,    61,  -338,
     -68,    62,    63,    64,    65,     0,    66,    67,    68,     0,
      69,    70,    71,    72,    73,    74,    75,    76,     0,    77,
      78,     0,    79,    80,    81,    82,    83,    84,    85,   -68,
      86,    87,     0,     0,  -211,     0,    88,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     100,     0,     0,   101,     0,   102,     0,   103,     0,  -211,
       0,     4,   104,     5,     6,     7,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,    15,    16,    17,    18,
       0,    19,     0,    20,    21,  -211,    22,    23,    24,    25,
      26,    27,     0,  -211,    28,     0,    29,     0,    30,    31,
      32,  -211,  -211,  -211,    33,    34,    35,   -68,     0,    36,
      37,    38,     0,    39,  -338,    40,     0,    41,    42,    43,
      44,    45,     0,    46,    47,    48,    49,   -68,    50,    51,
       0,    52,    53,    54,     0,  -338,     0,     0,    55,    56,
      57,    58,     0,    60,    61,  -338,   -68,    62,    63,    64,
      65,     0,    66,    67,    68,     0,    69,    70,    71,    72,
      73,    74,    75,    76,     0,    77,    78,     0,    79,    80,
      81,    82,    83,    84,    85,   -68,    86,    87,     0,     0,
    -211,     0,    88,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   100,     0,     0,   101,
       0,   102,     0,   103,     0,  -211,     0,     4,   104,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,    15,    16,    17,    18,     0,    19,     0,    20,
      21,  -211,    22,    23,    24,    25,    26,    27,     0,  -211,
      28,     0,    29,     0,    30,    31,    32,  -211,  -211,  -211,
      33,  1016,    35,   -68,     0,    36,    37,    38,     0,    39,
    -338,    40,     0,    41,    42,    43,    44,    45,     0,    46,
      47,    48,    49,   -68,    50,    51,     0,    52,    53,    54,
       0,  -338,     0,     0,    55,    56,    57,    58,     0,    60,
      61,  -338,   -68,    62,    63,    64,    65,     0,    66,    67,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,    77,    78,     0,    79,    80,    81,    82,    83,    84,
      85,   -68,    86,    87,     0,     0,  -211,     0,    88,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,     0,   103,
       0,  -211,     0,     4,   104,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,    15,    16,
      17,    18,     0,    19,     0,    20,    21,  -211,    22,    23,
      24,    25,    26,    27,     0,  -211,    28,     0,    29,     0,
      30,    31,    32,  -211,  -211,  -211,    33,  1167,    35,   -68,
       0,    36,    37,    38,     0,    39,  -338,    40,     0,    41,
      42,    43,    44,    45,     0,    46,    47,    48,    49,   -68,
      50,    51,     0,    52,    53,    54,     0,  -338,     0,     0,
      55,    56,    57,    58,     0,    60,    61,  -338,   -68,    62,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,    76,     0,    77,    78,     0,
      79,    80,    81,    82,    83,    84,    85,   -68,    86,    87,
       0,     0,  -211,     0,    88,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,     0,   103,     0,  -211,     0,     4,
     104,     5,     6,     7,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,    15,    16,    17,    18,     0,    19,
       0,    20,    21,  -211,    22,    23,    24,    25,    26,    27,
       0,  -211,    28,     0,    29,     0,    30,    31,    32,  -211,
    -211,  -211,    33,  1171,    35,   -68,     0,    36,    37,    38,
       0,    39,  -338,    40,     0,    41,    42,    43,    44,    45,
       0,    46,    47,    48,    49,   -68,    50,    51,     0,    52,
      53,    54,     0,  -338,     0,     0,    55,    56,    57,    58,
       0,    60,    61,  -338,   -68,    62,    63,    64,    65,     0,
      66,    67,    68,     0,    69,    70,    71,    72,    73,    74,
      75,    76,     0,    77,    78,     0,    79,    80,    81,    82,
      83,    84,    85,   -68,    86,    87,     0,     0,  -211,     0,
      88,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
       0,   103,     0,  -211,     0,     4,   104,     5,     6,     7,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
      15,    16,    17,    18,     0,    19,     0,    20,    21,  -211,
      22,    23,    24,    25,    26,    27,     0,  -211,    28,     0,
      29,     0,    30,    31,    32,  -211,  -211,  -211,    33,  1177,
      35,   -68,     0,    36,    37,    38,     0,    39,  -338,    40,
       0,    41,    42,    43,    44,    45,     0,    46,    47,    48,
      49,   -68,    50,    51,     0,    52,    53,    54,     0,  -338,
       0,     0,    55,    56,    57,    58,     0,    60,    61,  -338,
     -68,    62,    63,    64,    65,     0,    66,    67,    68,     0,
      69,    70,    71,    72,    73,    74,    75,    76,     0,    77,
      78,     0,    79,    80,    81,    82,    83,    84,    85,   -68,
      86,    87,     0,     0,  -211,     0,    88,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     100,     0,     0,   101,     0,   102,     0,   103,     0,  -211,
       0,     4,   104,     5,     6,     7,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,    15,    16,    17,    18,
       0,    19,     0,    20,    21,  -211,    22,    23,    24,    25,
      26,    27,     0,  -211,    28,     0,    29,     0,    30,    31,
      32,  -211,  -211,  -211,    33,  1180,    35,   -68,     0,    36,
      37,    38,     0,    39,  -338,    40,     0,    41,    42,    43,
      44,    45,     0,    46,    47,    48,    49,   -68,    50,    51,
       0,    52,    53,    54,     0,  -338,     0,     0,    55,    56,
      57,    58,     0,    60,    61,  -338,   -68,    62,    63,    64,
      65,     0,    66,    67,    68,     0,    69,    70,    71,    72,
      73,    74,    75,    76,     0,    77,    78,     0,    79,    80,
      81,    82,    83,    84,    85,   -68,    86,    87,     0,     0,
    -211,     0,    88,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   100,     0,     0,   101,
       0,   102,     0,   103,     0,  -211,     5,     6,   104,     8,
       9,    10,    11,    12,    13,     0,   621,     0,    14,     0,
       0,    17,    18,   623,    19,     0,   195,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   196,
       0,     0,     0,    32,     0,     0,     0,     0,   197,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   198,   629,
     199,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   200,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   201,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    89,   500,
     456,   457,   458,   459,   460,     0,     0,   463,   464,   465,
     466,     0,   468,   469,     0,   811,   812,   813,   814,   815,
     642,     0,   643,     0,    95,     0,   644,   645,   646,   647,
     648,   649,   650,   816,   652,   653,    97,   817,    99,   -90,
     655,   656,   818,   658,   202,     0,   103,     0,     0,     5,
       6,   104,     8,     9,    10,    11,    12,    13,     0,   621,
       0,    14,     0,     0,    17,    18,   623,    19,     0,   195,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   196,     0,     0,     0,    32,     0,     0,     0,
       0,   197,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   198,   629,   199,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   200,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   201,
       0,    77,     0,     0,    79,     0,     0,     0,    83,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    89,   500,   456,   457,   458,   459,   460,     0,     0,
     463,   464,   465,   466,     0,   468,   469,     0,   811,   812,
     813,   814,   815,   642,     0,   643,     0,    95,     0,   644,
     645,   646,   647,   648,   649,   650,   816,   652,   653,    97,
     817,    99,     0,   655,   656,   818,   658,   202,     0,   103,
       0,     0,     0,     0,   104,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,    15,     0,
      17,    18,     0,    19,     0,   195,    21,     0,    22,     0,
       0,     0,     0,    27,     0,     0,    28,     0,   196,     0,
       0,     0,    32,     0,     0,     0,     0,    34,    35,     0,
       0,     0,    37,     0,     0,    39,     0,   198,     0,   199,
       0,     0,    44,    45,     0,    46,    47,    48,    49,     0,
      50,    51,     0,    52,    53,    54,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,    64,   200,     0,     0,     0,     0,     0,    69,    70,
      71,    72,    73,    74,    75,    76,     0,    77,    78,     0,
      79,     0,     0,     0,    83,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   102,     0,   103,     0,     0,     5,     6,
     104,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   195,     0,
       0,    22,   377,    24,     0,     0,     0,     0,     0,    28,
       0,   196,     0,     0,     0,    32,     0,     0,     0,     0,
     197,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     198,     0,   199,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,    57,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   200,     0,    66,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   201,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,    85,
       0,    86,    87,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,     0,
       0,     5,     6,   104,     8,     9,    10,   380,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,    20,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   196,     0,     0,     0,    32,  -211,
    -211,  -211,     0,   197,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   198,     0,   199,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,    65,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,   201,     0,    77,     0,     0,    79,     0,     0,     0,
      83,    84,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,     0,  -211,     5,     6,   104,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   195,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,     0,     0,     0,     0,   197,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   198,     0,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,  -721,     0,     0,     0,  -721,     0,     0,    71,
      72,    73,    74,    75,   201,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,  -380,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,  -380,     0,     0,
     101,     0,   202,  -380,   103,     0,     0,     5,     6,   104,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,   195,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     196,     0,     0,     0,    32,  -211,  -211,  -211,     0,   197,
      35,     0,     0,     0,    37,     0,     0,    39,     0,   198,
       0,   199,     0,     0,    44,    45,     0,     0,    47,     0,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,    64,   200,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,   201,     0,    77,
       0,     0,    79,     0,     0,     0,    83,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   202,     0,   103,     0,  -211,
       5,     6,   104,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,     0,    22,     0,   726,     0,     0,     0,     0,
       0,    28,     0,   196,     0,     0,     0,    32,     0,     0,
       0,     0,   197,    35,     0,   727,     0,    37,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   200,     0,   728,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     201,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,    85,     0,   729,    87,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   202,     0,
     103,     0,     0,     5,     6,   104,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,     0,   195,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   196,     0,     0,     0,
      32,     0,     0,     0,     0,   197,    35,     0,     0,     0,
      37,     0,     0,    39,     0,   198,     0,   199,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     200,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   201,     0,    77,     0,     0,    79,   317,
     318,     0,    83,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   319,     0,     0,   101,
       0,   202,     0,   103,     0,     0,     5,     6,   104,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,   195,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   196,
       0,     0,     0,    32,     0,     0,     0,     0,   197,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   198,     0,
     199,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   200,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   201,     0,    77,     0,
       0,    79,   317,   318,     0,    83,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,   352,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   202,     0,   103,     0,     0,     5,
       6,   104,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,     0,     0,    17,    18,     0,    19,     0,   195,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   196,     0,     0,     0,    32,     0,     0,     0,
       0,   197,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   198,     0,   199,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   200,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   201,
       0,    77,     0,     0,    79,   317,   318,     0,    83,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   202,     0,   103,
    -528,     0,     5,     6,   104,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
       0,     0,     0,     0,   197,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   201,     0,    77,     0,     0,    79,   317,   318,
       0,    83,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     202,     0,   103,     0,     0,  -528,     0,   104,     5,     6,
       0,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   195,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   196,     0,     0,     0,    32,     0,     0,     0,     0,
     197,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     198,     0,   199,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   200,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   201,     0,
      77,     0,     0,    79,   317,   318,     0,    83,     0,    85,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,   787,
       0,     5,     6,   104,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,   195,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   196,     0,     0,     0,    32,     0,
       0,     0,     0,   197,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   198,     0,   199,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   200,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,   201,     0,    77,     0,     0,    79,   317,   318,     0,
      83,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,   790,     0,     5,     6,   104,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   195,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,     0,     0,     0,     0,   197,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   198,     0,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   201,     0,    77,     0,     0,    79,
     317,   318,     0,    83,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   202,     0,   103,   795,     0,     5,     6,   104,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,   195,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     196,     0,     0,     0,    32,     0,     0,     0,     0,   197,
      35,     0,     0,     0,    37,     0,     0,    39,     0,   198,
       0,   199,     0,     0,    44,    45,     0,     0,    47,     0,
      49,     0,    50,    51,   934,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,    64,   200,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,   201,     0,    77,
       0,     0,    79,   317,   318,     0,    83,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   202,     0,   103,     0,     0,
       5,     6,   104,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   196,     0,     0,     0,    32,     0,     0,
       0,     0,   197,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   200,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     201,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,    85,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,  -388,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,  -388,     0,     0,   101,     0,   202,  -388,
     103,     0,     0,     5,     6,   104,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,     0,   195,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   196,     0,     0,     0,
      32,     0,     0,     0,     0,   197,    35,     0,     0,     0,
      37,     0,     0,    39,     0,   198,     0,   199,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     200,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   201,     0,    77,     0,     0,    79,     0,
       0,     0,    83,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,  -373,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,  -373,     0,     0,   101,
       0,   202,  -373,   103,     0,     0,     5,     6,   104,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,   195,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   196,
       0,     0,     0,    32,     0,     0,     0,     0,   197,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   198,     0,
     199,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   200,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   201,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,  -381,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,  -381,
       0,     0,   101,     0,   202,  -381,   103,     0,     0,     5,
       6,   104,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,     0,     0,    17,    18,     0,    19,     0,   195,
       0,     0,    22,     0,   259,     0,     0,     0,     0,     0,
      28,     0,   196,     0,     0,     0,    32,     0,     0,     0,
       0,   197,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   198,     0,   199,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   200,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   201,
       0,    77,     0,     0,    79,     0,     0,     0,    83,     0,
      85,     0,   260,    87,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   202,     0,   103,
       0,     0,     5,     6,   104,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
       0,     0,     0,     0,   197,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   201,     0,    77,     0,     0,    79,   317,   318,
       0,    83,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     202,     0,   103,     0,     0,     5,     6,   104,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,     0,     0,
      17,    18,     0,    19,     0,   195,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   196,     0,
       0,     0,    32,     0,     0,     0,     0,   197,    35,     0,
       0,     0,    37,     0,     0,    39,     0,   198,     0,   199,
       0,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,    64,   200,  -721,     0,     0,     0,  -721,     0,     0,
      71,    72,    73,    74,    75,   201,     0,    77,     0,     0,
      79,     0,     0,     0,    83,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   202,     0,   103,     0,     0,     5,     6,
     104,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   195,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   196,     0,     0,     0,    32,     0,     0,     0,     0,
     197,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     198,     0,   199,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   200,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   201,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,    85,
       0,     0,    87,     0,     0,     0,     0,     0,   592,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,     0,
       0,   593,     0,   104,     5,     6,     0,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   195,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,     0,     0,     0,     0,   197,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   198,     0,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   201,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,  -409,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   202,     0,   103,  -409,     0,     5,     6,   104,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,   195,     0,     0,
      22,     0,     0,     0,     0,     0,     0,   275,    28,     0,
     196,     0,     0,     0,    32,     0,     0,     0,     0,   197,
      35,     0,     0,     0,    37,     0,     0,    39,     0,   198,
       0,   199,     0,     0,    44,    45,     0,     0,    47,     0,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,    64,   200,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,   201,     0,    77,
       0,     0,    79,     0,     0,     0,    83,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   102,     0,   103,     0,     0,
       5,     6,   104,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   196,     0,     0,     0,    32,     0,     0,
       0,     0,   197,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   200,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     201,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,    85,     0,     0,    87,     0,     0,     0,     0,     0,
     592,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   202,     0,
     103,     0,     0,     5,     6,   104,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,     0,   195,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   196,     0,     0,     0,
      32,     0,     0,     0,     0,   197,    35,     0,     0,     0,
      37,     0,     0,    39,     0,   198,     0,   199,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     200,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   201,     0,    77,     0,     0,    79,     0,
       0,     0,    83,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,     0,     0,   593,     0,   104,     5,
       6,     0,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,     0,     0,    17,    18,     0,    19,     0,   195,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   196,     0,     0,     0,    32,     0,     0,     0,
       0,   197,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   198,     0,   199,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   200,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   201,
       0,    77,     0,     0,    79,     0,     0,     0,    83,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   202,   785,   103,
       0,     0,     5,     6,   104,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
       0,     0,     0,     0,   197,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   201,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     202,     0,   103,     0,     0,  1008,     0,   104,     5,     6,
       0,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   195,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   196,     0,     0,     0,    32,     0,     0,     0,     0,
     197,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     198,     0,   199,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   200,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   201,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,    85,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,     0,
       0,  1013,     0,   104,     5,     6,     0,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   195,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,     0,     0,     0,     0,   197,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   198,     0,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   201,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   202,     0,   103,     0,     0,     5,     6,   104,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,   195,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,   295,     0,
     196,     0,     0,     0,    32,     0,     0,     0,     0,   197,
      35,     0,     0,     0,   297,     0,     0,    39,     0,   198,
       0,   199,     0,     0,    44,    45,     0,     0,    47,     0,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,    64,   200,     0,     0,     0,     0,     0,
       0,     0,    71,   302,    73,    74,    75,   303,     0,    77,
       0,     0,    79,     0,     0,     0,    83,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   202,     0,   103,     0,     0,
       5,     6,   104,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   196,     0,     0,     0,    32,     0,     0,
       0,     0,  1047,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   200,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     201,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,    85,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   202,     0,
     103,     0,     0,     5,     6,   104,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,     0,   195,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   196,     0,     0,     0,
      32,     0,     0,     0,     0,  1202,    35,     0,     0,     0,
      37,     0,     0,    39,     0,   198,     0,   199,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     200,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   201,     0,    77,     0,     0,    79,     0,
       0,     0,    83,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,     0,     0,     5,     6,   104,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,   195,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   196,
       0,     0,     0,    32,     0,     0,     0,     0,  1203,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   198,     0,
     199,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   200,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   201,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   202,     0,   103,     0,     0,     5,
       6,   104,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,     0,     0,    17,    18,     0,    19,     0,   195,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   196,     0,     0,     0,    32,     0,     0,     0,
       0,  1205,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   198,     0,   199,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   200,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   201,
       0,    77,     0,     0,    79,     0,     0,     0,    83,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   202,     0,   103,
       0,     0,     5,     6,   104,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
       0,     0,     0,     0,  1206,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,   427,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   201,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   428,     0,     0,     0,   429,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     202,     0,   103,     0,     0,     0,     6,   104,     8,     9,
      10,    11,    12,    13,     0,   431,   432,    14,   433,   434,
     435,    18,   436,   437,   438,   195,   439,   440,   441,   442,
     294,   444,   445,   446,   447,   448,     0,   450,   196,     0,
     451,   452,   453,  -211,  -211,  -211,     0,   197,     0,     0,
     296,   454,     0,     0,   298,     0,     0,     0,     0,   199,
       0,     0,    44,    45,     0,     0,     0,     0,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,   427,   299,
       0,    56,   300,    58,    59,    60,     0,     0,     0,     0,
      63,     0,   200,     0,   301,     0,     0,     0,     0,     0,
      71,     0,    73,     0,    75,     0,     0,     0,     0,     0,
       0,     0,     0,   304,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   428,     0,     0,
       0,   429,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   202,     0,   103,  -342,  -211,     0,     6,
     104,     8,     9,    10,    11,    12,    13,     0,   431,   432,
      14,   433,   434,   435,    18,   436,   437,   438,   195,     0,
     440,     0,     0,   294,     0,     0,   446,   447,     0,     0,
     450,   196,     0,   451,   452,   453,  -211,  -211,  -211,     0,
     197,     0,     0,   296,   454,     0,     0,   298,     0,     0,
       0,     0,   199,     0,     0,    44,    45,     0,     0,     0,
       0,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,     0,   299,     0,    56,   300,    58,    59,    60,     0,
       0,     0,     0,    63,     0,   200,     0,   301,     0,     0,
       0,     0,     0,    71,     0,    73,     0,    75,     0,     0,
       0,     0,     0,     0,     0,     0,   304,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   619,     0,   620,     0,     0,    90,    91,    92,
      93,    94,     0,     0,   621,     0,    95,     0,     0,   622,
     232,   623,   624,     0,     0,    96,     0,   625,    97,    98,
      99,     0,     0,     0,   101,   235,   202,   196,   103,     0,
    -211,   236,     0,   104,     0,     0,     0,   626,     0,     0,
       0,   238,     0,     0,   627,     0,   628,   629,     0,     0,
       0,     0,     0,     0,     0,   630,     0,     0,     0,     0,
       0,     0,   242,   631,     0,     0,     0,     0,     0,     0,
     244,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     632,     0,     0,     0,     0,     0,     0,     0,     0,   246,
     247,     0,   633,     0,   249,     0,   634,     0,     0,   251,
       0,     0,     0,   635,     0,   253,     0,     0,   636,     0,
       0,     0,     0,     0,     0,     0,     0,   500,   456,   457,
     458,   459,   460,     0,     0,   463,   464,   465,   466,     0,
     468,   469,     0,   637,   638,   639,   640,   641,   642,     0,
     643,     0,     0,     0,   644,   645,   646,   647,   648,   649,
     650,   651,   652,   653,     0,   654,     0,     0,   655,   656,
     657,   658,     5,     6,   659,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
    -211,  -211,  -211,     0,   197,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   201,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,     0,    98,    99,     0,     0,     0,   101,     0,
     202,     0,   103,     0,  -211,     5,     6,     0,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,     0,     0,
      17,    18,     0,    19,     0,   195,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   196,     0,
       0,     0,    32,  -211,  -211,  -211,     0,   197,    35,     0,
       0,     0,    37,     0,     0,    39,     0,   198,     0,   199,
       0,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,    64,   200,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   201,     0,    77,     0,     0,
      79,     0,     0,     0,    83,     0,    85,     0,     0,    87,
       0,     0,     5,     6,     0,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,    90,    22,    92,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
    -211,  -211,  -211,     0,   197,    35,     0,    99,     0,    37,
       0,     0,    39,   202,   198,   103,   199,  -211,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   201,     0,    77,     0,     0,    79,   230,     0,
       0,    83,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,   231,   232,     0,   233,     0,     0,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,   235,
       0,    90,     0,    92,     0,   236,     0,     0,     0,     0,
       0,   237,     0,     0,     0,   238,     0,     0,   239,     0,
     240,     0,     0,     0,    99,     0,     0,     0,     0,   241,
       0,     0,   103,     0,  -211,     0,   242,   243,     0,     0,
       0,     0,     0,     0,   244,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   245,     0,     0,     0,     0,     0,
       0,     0,     0,   246,   247,     0,   248,     0,   249,     0,
     250,   685,     0,   251,     0,     0,     0,   252,     0,   253,
       0,   621,   254,     0,     0,     0,   965,   966,   623,   967,
       0,     0,     0,     0,   968,     0,     0,     0,     0,     0,
       0,     0,   687,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   969,     0,     0,     0,   970,     0,
       0,   971,     0,   198,   629,     0,     0,     0,     0,     0,
       0,     0,   688,     0,     0,     0,     0,     0,   536,     0,
     972,     0,     0,     0,     0,     0,     0,   973,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   974,     0,     0,
       0,     0,     0,     0,     0,     0,   975,   976,     0,   977,
       0,   978,     0,   689,     0,     0,     0,     0,     0,   690,
     979,     0,   980,     0,     0,   981,     0,     0,     0,     0,
       0,     0,     0,     0,   500,   456,   457,   458,   459,   460,
       0,     0,   463,   464,   465,   466,     0,   468,   469,     0,
     637,   638,   639,   640,   641,   642,     0,   643,     0,     0,
       0,   644,   645,   646,   647,   648,   649,   650,   651,   652,
     653,   230,   654,     0,     0,   655,   656,   657,   658,   982,
       0,   621,     0,     0,     0,     0,   231,   232,   623,   233,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   235,     0,     0,     0,     0,     0,   236,     0,
       0,     0,     0,     0,   237,     0,     0,     0,   238,     0,
       0,   239,     0,   240,   629,     0,     0,     0,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,   242,
     243,     0,     0,     0,     0,     0,     0,   244,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   245,     0,     0,
       0,     0,     0,     0,     0,   425,   246,   247,     0,   248,
       0,   249,   426,   250,     0,     0,   251,     0,     0,     0,
     252,     0,   253,     0,   427,   254,     0,     0,     0,     0,
       0,     0,     0,     0,   500,   456,   457,   458,   459,   460,
       0,     0,   463,   464,   465,   466,     0,   468,   469,     0,
     637,   638,   639,   640,   641,   642,     0,   643,     0,     0,
       0,   644,   645,   646,   647,   648,   649,   650,   651,   652,
     653,     0,   654,     0,     0,   655,   656,   657,   658,     0,
       0,     0,     0,   428,   425,     0,     0,   429,     0,     0,
       0,   426,     0,     0,     0,     0,  1176,     0,     0,     0,
       0,     0,     0,   427,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,   500,   456,
     457,   458,   459,   460,     0,     0,   463,   464,   465,   466,
       0,   468,   469,     0,   431,   432,     0,   433,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,     0,     0,   451,
     452,   453,   428,   363,     0,   425,   429,     0,     0,     0,
     454,     0,   426,     0,     0,  1240,     0,     0,     0,     0,
       0,     0,     0,     0,   427,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   430,   500,   456,   457,
     458,   459,   460,     0,     0,   463,   464,   465,   466,     0,
     468,   469,     0,   431,   432,     0,   433,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,     0,     0,   451,   452,
     453,     0,   363,   428,   425,     0,     0,   429,     0,   454,
       0,   426,     0,     0,     0,     0,  1241,     0,     0,     0,
       0,     0,     0,   427,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,   500,   456,
     457,   458,   459,   460,     0,     0,   463,   464,   465,   466,
       0,   468,   469,     0,   431,   432,     0,   433,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,     0,     0,   451,
     452,   453,   428,   363,     0,   425,   429,     0,     0,     0,
     454,     0,   426,     0,     0,  1242,     0,     0,     0,     0,
       0,     0,     0,     0,   427,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   430,   500,   456,   457,
     458,   459,   460,     0,     0,   463,   464,   465,   466,     0,
     468,   469,     0,   431,   432,     0,   433,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,     0,     0,   451,   452,
     453,     0,   363,   428,     0,     0,     0,   429,     0,   454,
     425,     0,     0,     0,     0,     0,  1243,   426,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   427,
     850,     0,     0,     0,     0,     0,     0,   430,   500,   456,
     457,   458,   459,   460,     0,   851,   463,   464,   465,   466,
       0,   468,   469,     0,   431,   432,     0,   433,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,     0,     0,   451,
     452,   453,     0,   363,     0,     0,     0,     0,   428,   425,
     454,     0,   429,     0,     0,     0,   426,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   427,   275,
       0,     0,     0,     0,     0,     0,     0,     0,   838,     0,
       0,     0,   430,     0,   960,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
     432,     0,   433,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,     0,     0,   451,   452,   453,   428,   363,     0,
       0,   429,     0,     0,     0,   454,     0,     0,     0,     0,
     425,     0,     0,     0,     0,     0,     0,   426,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   838,     0,   427,
       0,   430,     0,     0,   579,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   431,   432,
       0,   433,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   580,     0,   451,   452,   453,     0,   363,     0,     0,
       0,     0,     0,     0,   454,     0,     0,     0,   428,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,     0,
       0,   425,   807,     0,     0,     0,     0,     0,   426,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     427,     0,   430,     0,     0,   808,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
     432,     0,   433,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   809,   581,   451,   452,   453,     0,     0,     0,
       0,     0,     0,     0,     0,   454,     0,     0,     0,   428,
     425,     0,     0,   429,     0,     0,     0,   426,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   427,
    1115,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,   433,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,     0,     0,   451,   452,   453,   428,     0,
       0,   425,   429,     0,     0,     0,   454,     0,   426,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     427,   275,     0,     0,     0,     0,     0,     0,   838,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
     432,     0,   433,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,     0,     0,   451,   452,   453,     0,   363,   428,
     425,     0,     0,   429,     0,   454,     0,   426,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   427,
     275,     0,     0,     0,     0,     0,     0,     0,     0,   838,
       0,     0,     0,   430,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,   433,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,     0,     0,   451,   452,   453,   428,   363,
       0,   425,   429,     0,     0,     0,   454,     0,   426,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     427,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,   830,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
     432,     0,   433,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,     0,     0,   451,   452,   453,     0,   363,   428,
     425,     0,     0,   429,     0,   454,     0,   426,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   427,
     894,     0,     0,     0,     0,     0,     0,     0,     0,   214,
       0,     0,     0,   430,     0,   895,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,   433,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,     0,     0,   451,   452,   453,   428,     0,
     425,     0,   429,     0,     0,     0,   454,   426,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   427,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
     432,     0,   433,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,     0,     0,   451,   452,   453,     0,   428,     0,
       0,     0,   429,     0,     0,   454,     0,     0,     0,     0,
       0,   925,   425,     0,     0,     0,     0,     0,     0,   426,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   427,   430,     0,     0,     0,  1086,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
     432,     0,   433,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,     0,  1087,   451,   452,   453,     0,   363,     0,
       0,     0,     0,     0,     0,   454,     0,     0,     0,     0,
     428,   425,     0,     0,   429,     0,     0,     0,   426,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     427,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   431,   432,     0,   433,   434,   435,     0,   436,   437,
     438,     0,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,     0,     0,   451,   452,   453,   428,
       0,   425,     0,   429,     0,     0,     0,   454,   426,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     427,     0,     0,     0,     0,     0,     0,     0,     0,   838,
       0,     0,     0,   430,     0,     0,     0,     0,     0,     0,
       0,   425,   523,     0,     0,     0,     0,     0,   426,     0,
     431,   432,     0,   433,   434,   435,     0,   436,   437,   438,
     427,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,     0,     0,   451,   452,   453,     0,   428,
       0,   425,     0,   429,  1107,     0,   454,     0,   426,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     427,     0,     0,     0,     0,     0,     0,     0,     0,   838,
       0,     0,     0,   430,     0,     0,     0,     0,     0,   428,
       0,   425,     0,   429,     0,     0,     0,     0,   426,     0,
     431,   432,     0,   433,   434,   435,     0,   436,   437,   438,
     427,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   430,     0,   451,   452,   453,     0,   428,
       0,     0,     0,   429,  1155,     0,   454,     0,     0,     0,
     431,   432,     0,   433,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   430,     0,   451,   452,   453,     0,   428,
       0,   425,     0,   429,     0,     0,   454,     0,   426,     0,
     431,   432,     0,   433,   434,   435,     0,   436,   437,   438,
     427,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   430,     0,   451,   452,   453,     0,   545,
       0,   425,     0,     0,     0,     0,   454,     0,   426,     0,
     431,   432,     0,   433,   434,   435,     0,   436,   437,   438,
     427,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,     0,   549,   451,   452,   453,     0,   428,
       0,   425,   803,   429,     0,     0,   454,     0,   426,     0,
       0,     0,   721,     0,     0,     0,     0,     0,     0,     0,
     427,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,   428,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
     431,   432,     0,   433,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   430,     0,   451,   452,   453,     0,   428,
       0,   425,     0,   429,     0,     0,   454,     0,   426,     0,
     431,   432,     0,   433,   434,   435,     0,   436,   437,   438,
     427,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   430,     0,   451,   452,   453,     0,     0,
       0,     0,   786,     0,     0,     0,   454,     0,     0,     0,
     431,   432,     0,   433,   434,   435,     0,   436,   804,   438,
       0,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,     0,     0,   451,   452,   453,     0,   428,
     425,     0,     0,   429,     0,     0,   454,   426,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   427,
       0,     0,   923,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   845,   430,     0,     0,     0,     0,     0,     0,
       0,   425,     0,     0,     0,     0,     0,     0,   426,     0,
     431,   432,     0,   433,   434,   435,     0,   436,   437,   438,
     427,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,     0,     0,   451,   452,   453,   428,     0,
       0,   425,   429,     0,     0,     0,   454,     0,   426,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     427,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,   428,
       0,     0,     0,   429,     0,     0,     0,     0,     0,   431,
     432,     0,   433,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,     0,   430,   451,   452,   453,     0,     0,   428,
       0,   425,   991,   429,     0,   454,     0,     0,   426,     0,
     431,   432,     0,   433,   434,   435,     0,   436,   437,   438,
     427,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   430,     0,   451,   452,   453,     0,     0,
       0,   425,   937,     0,     0,     0,   454,     0,   426,     0,
     431,   432,     0,   433,   434,   435,     0,   436,   437,   438,
     427,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,     0,     0,   451,   452,   453,     0,   428,
       0,     0,   938,   429,     0,     0,   454,   425,  1085,     0,
       0,     0,     0,     0,   426,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,   428,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
     431,   432,     0,   433,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   430,     0,   451,   452,   453,     0,     0,
       0,     0,     0,     0,     0,   428,   454,     0,     0,   429,
     431,   432,     0,   433,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   425,     0,   451,   452,   453,     0,   430,
     426,     0,  1039,     0,     0,     0,   454,     0,     0,     0,
       0,     0,   427,     0,     0,     0,   431,   432,     0,   433,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   425,
       0,   451,   452,   453,     0,     0,   426,     0,     0,     0,
       0,     0,   454,     0,     0,     0,     0,     0,   427,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   428,     0,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,   426,     0,     0,     0,     0,     0,     0,     0,
       0,   214,     0,     0,   427,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   428,     0,     0,
       0,   429,   431,   432,     0,   433,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,     0,     0,   451,   452,   453,
       0,   430,     0,     0,     0,     0,     0,     0,   454,     0,
       0,     0,     0,   428,     0,     0,     0,   429,   431,   432,
       0,   433,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   425,  1099,   451,   452,   453,  1110,   430,   426,     0,
       0,     0,     0,     0,   454,     0,     0,     0,     0,     0,
     427,     0,     0,     0,   431,   432,     0,   433,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   425,     0,   451,
     452,   453,     0,     0,   426,     0,     0,     0,     0,     0,
     454,     0,     0,     0,     0,     0,   427,  1141,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   428,
       0,     0,     0,   429,     0,     0,     0,   425,  1160,     0,
       0,     0,     0,     0,   426,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   428,     0,     0,     0,   429,
     431,   432,     0,   433,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,     0,     0,   451,   452,   453,     0,   430,
       0,     0,     0,     0,  1138,   428,   454,   425,     0,   429,
       0,     0,     0,     0,   426,     0,   431,   432,     0,   433,
     434,   435,     0,   436,   437,   438,   427,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   430,
       0,   451,   452,   453,     0,     0,     0,   425,     0,     0,
       0,     0,   454,     0,   426,     0,   431,   432,     0,   433,
     434,   435,     0,   436,   437,   438,   427,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,     0,
       0,   451,   452,   453,     0,   428,     0,   425,     0,   429,
       0,     0,   454,     0,   426,     0,  1196,     0,  1204,     0,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,   428,     0,   425,     0,   429,
       0,     0,     0,     0,   426,     0,   431,   432,     0,   433,
     434,   435,     0,   436,   437,   438,   427,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   430,
       0,   451,   452,   453,     0,   428,     0,   425,     0,   429,
       0,     0,   454,     0,   426,     0,   431,   432,  1244,   433,
     434,   435,     0,   436,   437,   438,   427,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   430,
       0,   451,   452,   453,     0,   428,     0,   425,     0,   429,
       0,     0,   454,     0,   426,     0,   431,   432,  1245,   433,
     434,   435,     0,   436,   437,   438,   427,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   430,
       0,   451,   452,   453,     0,   428,     0,   425,     0,   429,
       0,     0,   454,     0,   426,     0,   431,   432,  1246,   433,
     434,   435,     0,   436,   437,   438,   427,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   430,
       0,   451,   452,   453,     0,   428,     0,   425,     0,   429,
       0,     0,   454,     0,   426,     0,   431,   432,  1247,   433,
     434,   435,     0,   436,   437,   438,   427,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   430,
       0,   451,   452,   453,     0,   428,     0,     0,     0,   429,
       0,     0,   454,     0,     0,     0,   431,   432,     0,   433,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   430,
       0,   451,   452,   453,     0,   913,     0,     0,     0,   429,
       0,     0,   454,     0,     0,     0,   431,   432,     0,   433,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   430,
       0,   451,   452,   453,     0,     0,     0,     0,     0,     0,
       0,     0,   454,     0,     0,     0,   431,   432,     0,   433,
     434,   435,     0,   436,   437,   438,   427,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,     0,
     359,   451,   452,   453,    23,    24,     0,     0,     0,     0,
       0,     0,   454,   360,     0,    30,   361,     0,     0,     0,
       0,    33,     0,     0,     0,     0,     0,     0,    38,     0,
       0,  -338,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   -68,   428,     0,     0,     0,   429,
       0,     0,  -338,     0,     0,    55,     0,    57,     0,     0,
       0,    61,  -338,   -68,    62,     0,     0,   362,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
      84,     0,     0,    86,     0,     0,   431,   432,     0,   433,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
     442,     0,   444,   445,   446,   447,   448,     0,   450,     0,
       0,   451,   452,   453,   603,     0,   230,     0,     7,     0,
       0,     0,   454,     0,     0,     0,     0,     0,     0,     0,
       0,   231,   232,   100,   233,     0,     0,     0,   363,   234,
       0,     0,     0,     0,     0,     0,     0,   235,     0,     0,
       0,     0,     0,   236,     0,     0,     0,     0,     0,   237,
       0,     0,     0,   238,     0,     0,   239,     0,   240,     0,
       0,     0,   230,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,     0,   242,   243,     0,   231,   232,     0,
     233,     0,   244,     0,     0,   234,     0,    24,     0,     0,
       0,     0,   245,   235,     0,     0,     0,     0,     0,   236,
       0,   246,   247,     0,   248,   237,   249,     0,   250,   238,
       0,   251,   239,     0,   240,   252,     0,   253,     0,     0,
     254,     0,     0,   241,     0,     0,     0,     0,     0,     0,
     242,   243,     0,     0,     0,     0,     0,     0,   244,    57,
       0,     0,     0,     0,     0,     0,     0,     0,   245,     0,
       0,    66,     0,     0,   230,     0,     7,   246,   247,     0,
     248,     0,   249,     0,   250,     0,     0,   251,     0,   231,
     232,   252,   233,   253,     0,    86,   254,   234,     0,     0,
       0,     0,     0,     0,     0,   235,     0,     0,     0,     0,
       0,   236,     0,     0,     0,     0,     0,   237,     0,     0,
       0,   238,     0,     0,   239,     0,   240,     0,     0,     0,
    -313,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,   242,   243,     0,  -313,  -313,     0,  -313,     0,
     244,     0,     0,  -313,     0,     0,     0,     0,     0,     0,
     245,  -313,     0,     0,     0,     0,     0,  -313,     0,   246,
     247,     0,   248,  -313,   249,     0,   250,  -313,     0,   251,
    -313,     0,  -313,   252,     0,   253,   230,     0,   254,     0,
       0,  -313,     0,     0,     0,     0,     0,     0,  -313,  -313,
       0,   231,   232,     0,   233,     0,  -313,     0,     0,   234,
       0,     0,     0,     0,     0,     0,  -313,   235,     0,     0,
       0,     0,     0,   236,     0,  -313,  -313,     0,  -313,   237,
    -313,     0,  -313,   238,     0,  -313,   239,     0,   240,  -313,
       0,  -313,  -314,     0,  -313,     0,     0,   241,     0,     0,
       0,     0,     0,     0,   242,   243,     0,  -314,  -314,     0,
    -314,     0,   244,     0,     0,  -314,     0,     0,     0,     0,
       0,     0,   245,  -314,     0,     0,     0,     0,     0,  -314,
       0,   246,   247,     0,   248,  -314,   249,     0,   250,  -314,
       0,   251,  -314,     0,  -314,   252,     0,   253,     0,     0,
     254,     0,     0,  -314,     0,     0,     0,     0,     0,     0,
    -314,  -314,     0,     0,     0,     0,     0,     0,  -314,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -314,     0,
       0,     0,     0,     0,     0,     0,     0,  -314,  -314,     0,
    -314,     0,  -314,     0,  -314,     0,     0,  -314,     0,     0,
       0,  -314,     0,  -314,     0,     0,  -314
};

static const yytype_int16 yycheck[] =
{
       2,     2,    63,   181,   482,    63,   112,   399,    21,   165,
      46,    27,     2,   543,   604,    63,   416,   506,   662,   579,
     664,   615,   547,   809,    26,   223,   612,   899,   857,   133,
     266,   775,   538,   381,    70,   804,    26,     1,     1,   827,
      29,   203,   769,   568,   401,     1,   939,   515,    68,    25,
       5,    67,    41,    42,    43,   291,    33,    33,    33,   865,
      48,   104,     1,    33,    36,    41,    42,    43,    88,    33,
      62,    33,     3,     3,    76,     1,   148,    76,    80,    81,
     229,    33,   124,   103,   262,   173,    76,   175,   148,    23,
      80,    81,   119,    25,    33,   331,     0,    33,    62,   166,
     102,    33,   174,   391,   106,    48,   166,   111,    33,    41,
      42,    43,   102,    41,    42,    43,   106,   266,   121,   118,
      76,   148,    48,    62,    55,    55,   121,   119,   171,   171,
      51,  1003,   594,    54,   148,   941,   598,   599,    57,   111,
     428,   429,   291,   107,   120,    57,   121,   615,    57,    92,
      48,   121,     2,    12,    75,   119,    90,    78,   172,    92,
     148,   150,   118,   126,    85,   994,    48,   148,   107,    57,
     756,   148,     3,   172,   105,   164,    26,   534,  1071,   113,
     119,   176,   331,   147,   147,   148,   474,   475,   120,   933,
     111,   172,    68,   148,   171,   939,   171,  1026,  1027,   175,
     126,   171,   166,   365,   166,   993,    57,   171,   147,   171,
     199,   174,    88,   215,   739,   942,   172,   172,    48,   171,
     989,   147,   148,   148,    55,   215,    76,   103,    57,   121,
      80,    81,   171,   722,   582,   171,   472,   473,   416,   121,
     166,    25,   278,   175,   280,     3,   171,   175,   808,    33,
     148,   287,   102,    28,   173,    28,   106,    41,    42,    43,
      47,   173,    92,    94,  1093,    54,   148,   773,   270,   148,
     259,   260,    48,   275,   105,    48,   380,   565,   176,    52,
     270,   105,   271,    19,   176,   275,    75,   148,   437,   121,
    1119,   327,   148,  1165,   176,   174,    85,    55,   173,    86,
      47,  1087,   142,     3,    77,    80,   148,    80,  1052,    82,
     150,   148,    91,    80,    81,   176,    95,    92,   174,    92,
      67,    21,   311,   472,   473,   523,   912,  1071,   115,    28,
     486,   793,   174,   148,    34,   173,   120,   174,   111,    86,
      28,   116,   400,    79,   176,   121,   940,   105,   350,   350,
     148,   148,   173,   229,   175,    55,   166,   506,   126,   174,
     350,   363,    98,   173,    64,   215,   855,   148,   115,     9,
     372,   360,   148,   363,   390,     2,   174,   174,   114,    57,
     396,    80,   372,   385,   152,   166,   102,   585,   166,    77,
     266,   175,    80,    92,   172,   385,  1040,  1041,   547,    26,
     176,   174,    57,   148,    92,   105,   395,   896,   148,    32,
     399,   111,   111,   148,   416,   291,    57,  1127,   148,   568,
     270,   899,   982,   111,   166,   275,   148,   416,    47,   174,
     172,    91,     2,   148,   174,    95,   961,    28,   148,   174,
    1030,   461,   462,   148,   174,   510,   148,   467,    67,    76,
     470,   471,   174,    80,    81,   331,    26,    48,    91,   174,
     173,    52,    95,   479,   174,   857,   173,    86,    91,   174,
     172,   536,    95,    57,   535,   102,   876,   535,    57,   106,
    1190,  1191,  1192,  1193,  1194,  1195,    77,   535,    57,    80,
     148,    82,   481,    57,   148,   497,   115,   499,   604,   148,
     350,    92,   173,    57,   175,   148,    76,   497,   166,   499,
      80,    81,   166,   363,   173,    57,   175,   166,   507,   142,
     111,   510,   372,   166,   147,  1003,  1086,   150,   148,   292,
     293,   154,   102,  1123,   523,   385,   106,    80,    81,   559,
     560,   148,   830,   532,  1006,  1007,   166,   536,   564,   148,
     148,   148,    57,   841,   590,   591,  1018,  1019,   148,   166,
      91,    91,   598,   851,    95,    95,    57,   166,   166,   166,
     572,   572,   166,   722,   587,    84,   166,    57,    87,   173,
      91,   175,   572,   174,    95,   461,   462,     5,   215,   578,
     739,   467,   150,   142,   470,   471,   472,   473,   147,   171,
     589,   150,   994,  1133,   892,   154,   171,   895,   173,   559,
     560,    28,    28,    57,   173,   604,   175,  1079,  1080,  1081,
    1082,   173,   173,   175,   175,   913,   914,   147,   173,   166,
     506,    48,    48,   171,  1026,    52,    52,   165,    57,  1031,
     166,   166,   166,   270,   166,   215,   173,   497,   275,   499,
      32,   166,   166,   166,   803,   804,  1186,    86,   166,   171,
      77,    77,   166,    80,    80,    82,    82,   173,   166,   148,
     958,   547,   960,   829,   173,    92,    92,   173,   147,   668,
     174,   173,   166,   559,   560,   148,   174,  1165,     9,     3,
     173,    67,   568,    80,   111,   111,   171,    92,   876,   126,
     270,  1093,    92,   126,   166,   275,   855,   150,    70,    91,
     174,   126,   173,    95,   148,   126,   126,   737,     2,    35,
      35,    67,   572,   350,   174,    32,   153,   126,   155,   156,
     732,   158,   159,   174,   147,   174,   363,    22,   171,   759,
     120,   743,    26,   173,   746,   372,   148,   896,   172,   126,
     107,   173,   150,   743,   150,   176,   746,   150,   385,   748,
     142,   143,   150,   145,   146,   147,   755,   149,   150,   150,
     142,   150,   154,   174,   166,   150,   765,   150,   160,   161,
     350,   150,   164,   150,    91,   167,   168,   169,    95,   150,
     150,   150,    76,   363,   783,   831,    80,    81,   150,   857,
     150,   166,   372,   166,    35,   863,    35,   173,   166,   147,
     166,   166,   961,   849,   872,   385,   148,   853,   102,   172,
     836,   166,   106,   174,   171,    48,    35,   176,   176,  1065,
    1066,   173,   172,   174,   173,   142,   166,   839,    32,   828,
     147,   843,   149,   150,   166,   176,   722,   154,   850,   839,
     173,   147,   176,   843,   161,   166,   148,   174,    33,    35,
     850,   737,   169,   739,   174,   166,   166,   176,   176,   174,
     497,   174,   499,   148,   165,   165,   111,   567,   961,   265,
      25,   801,   372,   759,   365,   929,   389,   876,   339,   521,
     954,   535,   876,   743,   383,   863,   746,    91,   872,   305,
    1186,    95,  1033,  1196,  1189,   828,   668,   131,   518,   755,
      -1,   913,   914,    -1,    -1,    -1,  1065,  1066,   920,    -1,
     922,  1027,    -1,   925,    -1,    -1,    -1,   497,    -1,   499,
     920,   215,   922,  1111,   950,   925,   994,    -1,    -1,    -1,
     929,    -1,    -1,    -1,    -1,   572,    -1,   949,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   959,    -1,   949,
     154,    -1,    -1,    -1,    -1,   954,    -1,   161,    -1,   959,
      -1,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,   855,
      -1,    -1,  1018,  1019,   997,   998,   270,    -1,    -1,   839,
      -1,   275,    -1,   843,    -1,    -1,    -1,    -1,    -1,    -1,
     850,    -1,   572,  1005,    -1,    -1,    -1,    -1,    -1,  1011,
      -1,    -1,    -1,  1119,    32,  1005,    -1,  1123,    -1,    -1,
     896,  1011,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1093,    -1,    -1,  1216,  1075,
    1218,  1030,    -1,  1221,  1222,  1081,  1082,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1059,    -1,    -1,
      -1,  1097,  1098,    -1,  1100,    -1,   350,    -1,    -1,  1059,
     920,    -1,   922,    91,    -1,   925,    -1,    95,  1114,   363,
      -1,  1117,    -1,    -1,    -1,   961,    -1,    -1,   372,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   949,
      -1,   385,    -1,  1105,     2,  1107,    -1,    32,    -1,   959,
    1112,    -1,    -1,  1115,    -1,  1105,   743,  1107,    -1,   746,
      -1,    -1,  1112,    -1,   142,  1115,    -1,    -1,    26,   147,
      -1,   149,   150,    -1,  1123,    -1,   154,    -1,    -1,    -1,
      -1,    -1,  1144,   161,    -1,    -1,    -1,    -1,  1150,   167,
     168,   169,  1154,  1155,  1144,  1005,    -1,    -1,    -1,    -1,
    1150,  1011,    -1,    -1,  1154,  1155,    91,    -1,  1170,    -1,
      95,    -1,  1174,   743,  1176,    -1,   746,    -1,    76,    -1,
    1170,    -1,    80,    81,  1174,    -1,  1176,    -1,    -1,  1065,
    1066,    -1,    -1,    -1,    -1,  1197,  1197,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,    -1,    -1,  1197,   106,  1059,
      -1,    -1,   839,   497,    -1,   499,   843,   142,   143,    -1,
      -1,   146,   147,   850,   149,   150,    -1,    -1,    -1,   154,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,  1240,  1241,
    1242,  1243,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
    1240,  1241,  1242,  1243,    -1,  1105,    -1,  1107,    -1,    -1,
      -1,    -1,  1112,    -1,    -1,  1115,    -1,    -1,    -1,   839,
      -1,    -1,    -1,   843,    -1,    -1,    -1,    -1,    -1,    -1,
     850,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   572,    -1,
      -1,    -1,    -1,   920,  1144,   922,    -1,    -1,   925,    -1,
    1150,    -1,    -1,    -1,  1154,  1155,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   215,    -1,    -1,
    1170,    -1,   949,     5,  1174,    -1,  1176,    -1,    -1,    -1,
      -1,    -1,   959,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    27,    28,  1197,    -1,    -1,
     920,    -1,   922,    -1,    36,   925,    38,    39,    -1,    -1,
      -1,    -1,    44,    -1,     2,    -1,    -1,    -1,    -1,    51,
      -1,    -1,   270,    -1,    -1,    -1,    -1,   275,  1005,   949,
      -1,    -1,    -1,    -1,  1011,    -1,    -1,    -1,    26,   959,
    1240,  1241,  1242,  1243,    -1,    -1,    78,    32,    80,    -1,
      82,    -1,    84,    -1,    -1,    87,    -1,    -1,    90,    -1,
      92,    -1,    23,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,   111,
      -1,   113,  1059,    44,   116,  1005,    -1,    -1,    76,    -1,
      51,  1011,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   350,    -1,    -1,    -1,    91,    -1,    -1,   743,
      95,    -1,   746,    -1,   102,   363,    -1,    78,   106,    80,
      -1,    -1,     2,    -1,   372,    -1,    -1,    -1,  1105,    90,
    1107,    92,    -1,    -1,   166,  1112,     2,   385,  1115,  1059,
     172,    -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,
     111,    -1,   113,    -1,    -1,   116,    -1,   142,   143,    -1,
      26,    -1,   147,    -1,   149,   150,    -1,  1144,    -1,   154,
      -1,    -1,    -1,  1150,    -1,    -1,   161,  1154,  1155,    -1,
      -1,    -1,   167,   168,   169,  1105,    -1,  1107,    -1,    -1,
      -1,    -1,  1112,  1170,    -1,  1115,    76,  1174,    -1,  1176,
      80,    81,    -1,    -1,    -1,   839,    -1,    -1,    -1,   843,
      76,    -1,    -1,    -1,    -1,    -1,   850,    -1,    -1,    -1,
    1197,    -1,   102,    -1,  1144,    -1,   106,   215,    -1,    -1,
    1150,    -1,    -1,    -1,  1154,  1155,   102,    -1,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   497,
    1170,   499,    -1,    -1,  1174,    -1,  1176,    -1,    -1,    -1,
      -1,    -1,    -1,  1240,  1241,  1242,  1243,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1197,    -1,    -1,
      -1,    -1,   270,    -1,    -1,    -1,   920,   275,   922,    -1,
      -1,   925,    -1,    -1,     1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    19,    -1,    21,   949,    -1,    -1,    -1,    26,
    1240,  1241,  1242,  1243,   572,   959,    32,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,   215,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,   215,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,   350,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,  1005,    79,    -1,    -1,   363,    -1,  1011,    -1,    -1,
      -1,    -1,    89,    -1,   372,    91,    -1,    -1,    -1,    95,
     270,    98,    99,    -1,   101,   275,   103,   385,   105,    -1,
      -1,   108,    -1,    -1,   270,   112,    -1,   114,    -1,   275,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,  1059,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,    -1,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,    -1,   173,    -1,    -1,    -1,
     350,  1105,   178,  1107,    -1,    -1,    -1,    -1,  1112,    -1,
      -1,  1115,    -1,   363,   350,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   372,    -1,    -1,   743,    -1,   363,   746,    -1,
      -1,    -1,    -1,    -1,    -1,   385,   372,    -1,    -1,   497,
    1144,   499,    -1,    46,    -1,    -1,  1150,    -1,    -1,   385,
    1154,  1155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1170,    70,    -1,     1,
    1174,    -1,  1176,     5,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,  1197,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    36,    -1,    38,    39,   111,    -1,
      -1,    -1,    44,    -1,   572,    -1,    -1,    -1,    -1,    51,
      -1,   839,    54,    -1,    -1,   843,    -1,    -1,    -1,    -1,
      -1,    -1,   850,    -1,    -1,    -1,  1240,  1241,  1242,  1243,
      -1,    -1,    -1,    75,    -1,    -1,    78,   497,    80,   499,
      82,     3,    84,    85,    -1,    87,    -1,    -1,    90,    -1,
      92,   497,    -1,   499,    -1,    -1,    18,    19,    -1,    21,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,   111,
      -1,   113,    34,    -1,   116,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,   920,    55,   922,    -1,    -1,   925,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   221,    71,
      72,    -1,   572,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,   949,    -1,    -1,   166,    -1,   572,    89,    -1,    -1,
     172,   959,    -1,    -1,    -1,    -1,    98,    99,    -1,   101,
      -1,   103,    -1,   105,    -1,    -1,   108,    -1,   261,   262,
     112,    -1,   114,    -1,    -1,   117,   269,    -1,    -1,    -1,
      -1,    -1,     5,    -1,    -1,   278,    -1,   280,    -1,    -1,
      -1,    -1,    -1,    -1,   287,   743,    -1,  1005,   746,    -1,
      23,    -1,     5,  1011,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,
      23,    44,    -1,    -1,    27,    28,    -1,    -1,    51,   171,
      -1,   173,    -1,    36,   327,    38,    39,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,  1059,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    84,    -1,    -1,    87,    -1,    -1,    90,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    84,    -1,    -1,    87,    -1,    -1,    90,   111,    92,
     113,   839,    -1,   116,    -1,   843,    -1,  1105,    -1,  1107,
      -1,    -1,   850,   743,  1112,    -1,   746,  1115,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,   743,    -1,    -1,
     746,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1144,    -1,    -1,    -1,
      -1,    -1,  1150,   166,    -1,    -1,  1154,  1155,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1170,   166,    -1,    -1,  1174,    -1,  1176,   172,
      -1,    -1,   920,     5,   922,    -1,    -1,   925,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1197,
      -1,    23,    -1,    -1,    -1,    27,    28,    -1,    -1,   839,
      -1,   949,    -1,   843,    36,    -1,    38,    39,    -1,    -1,
     850,   959,    44,   839,    -1,    -1,    -1,   843,    -1,    51,
      -1,    -1,    -1,    -1,   850,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1240,  1241,  1242,  1243,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    84,    -1,    -1,    87,    -1,  1005,    90,    -1,
      92,    -1,    -1,  1011,    -1,    -1,    -1,    -1,    -1,     5,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
     920,   113,   922,    -1,   116,   925,    -1,    23,    -1,    -1,
      -1,    27,    28,    -1,   920,    -1,   922,   590,   591,   925,
      36,    -1,    38,    39,    -1,   598,   599,    -1,    44,   949,
      -1,  1059,    -1,    -1,    -1,    51,    -1,    -1,    -1,   959,
      -1,    -1,    -1,   949,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   959,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    -1,    -1,    84,    -1,
      -1,    87,    -1,    -1,    90,    -1,    92,  1105,    -1,  1107,
      -1,    -1,    -1,    -1,  1112,  1005,    -1,  1115,    -1,    -1,
      -1,  1011,    -1,    -1,    -1,   111,    -1,   113,    -1,  1005,
     116,    -1,    -1,    91,    -1,  1011,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1144,    -1,    -1,    -1,
      -1,    -1,  1150,    -1,    -1,    -1,  1154,  1155,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1059,
      -1,    -1,  1170,    -1,    -1,    -1,  1174,    -1,  1176,    -1,
     166,    -1,    -1,  1059,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,    -1,   154,   155,   156,  1197,
     158,   159,   160,   161,   747,    -1,   164,    -1,    -1,   167,
     168,   169,    -1,    -1,    -1,  1105,    -1,  1107,    -1,    -1,
     178,    -1,  1112,    -1,    -1,  1115,    -1,    -1,    -1,  1105,
      -1,  1107,    -1,    -1,    -1,    -1,  1112,    -1,    -1,  1115,
     783,    -1,  1240,  1241,  1242,  1243,    -1,    -1,    -1,    -1,
     793,    -1,    -1,    -1,  1144,    -1,    -1,    -1,    -1,    -1,
    1150,    -1,    -1,    -1,  1154,  1155,    -1,    -1,  1144,    -1,
      -1,    -1,    -1,    -1,  1150,    -1,    -1,    -1,  1154,  1155,
    1170,    -1,    -1,    -1,  1174,    -1,  1176,    -1,   831,    -1,
      -1,    -1,    -1,    -1,  1170,    -1,    -1,    -1,  1174,    -1,
    1176,    -1,    -1,    -1,    -1,    -1,   849,  1197,    -1,    -1,
     853,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1197,    -1,    -1,    -1,    -1,    -1,    -1,    14,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
    1240,  1241,  1242,  1243,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    48,    49,  1240,  1241,  1242,  1243,    54,    -1,
      56,    -1,    58,    -1,    -1,    -1,    -1,    63,    -1,    -1,
       3,    67,    68,    69,    70,    71,    72,   930,    -1,    -1,
      76,    -1,    78,    -1,    -1,    18,    -1,    -1,    21,    85,
      -1,    -1,    88,    26,    90,    -1,    92,    -1,    -1,    95,
      96,    -1,    98,    36,   957,    -1,   102,   103,   104,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,   131,    -1,   133,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1018,  1019,    -1,   101,    -1,
      -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,   174,   112,
       3,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    19,    -1,    21,    -1,
      -1,   197,    -1,    26,    76,   201,   202,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,  1075,    46,    -1,    -1,    -1,    50,  1081,  1082,
      53,    -1,    55,   229,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,  1097,  1098,    -1,  1100,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,  1111,    -1,
      -1,  1114,    -1,    -1,  1117,    -1,    89,    -1,    -1,    -1,
     266,    -1,    -1,    -1,    -1,    98,    99,    -1,   101,    -1,
     103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,
      -1,   114,    -1,    -1,   117,   291,    -1,    -1,    -1,  1152,
      -1,    -1,    -1,    -1,    -1,    -1,   302,   303,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   317,   318,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   331,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,   352,    -1,    -1,    -1,
      -1,    -1,    -1,  1216,    -1,  1218,    -1,    -1,  1221,  1222,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   373,    -1,   375,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   384,    -1,
      -1,    -1,    -1,   389,   390,   391,    -1,    -1,   270,    -1,
     396,    -1,    -1,   275,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   425,
     426,   427,   428,   429,   430,    -1,   432,   433,   434,   435,
     436,    -1,   438,   439,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,    -1,
      -1,    -1,    -1,    -1,    -1,   461,   462,    -1,    -1,    -1,
      -1,   467,    -1,    -1,   470,   471,   472,   473,   474,   475,
      -1,    -1,    -1,   479,   480,    -1,   482,    -1,    -1,    -1,
      -1,    -1,   488,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   499,    -1,    -1,   502,    -1,    -1,    -1,
     506,    -1,    -1,   385,    -1,    -1,    -1,    -1,    -1,   515,
      -1,   517,    -1,    -1,    -1,   521,    -1,    -1,   524,   525,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   535,
      -1,    -1,   538,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   547,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     556,    -1,    -1,   559,   560,    -1,    -1,    -1,   564,   565,
      -1,    -1,   568,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   579,   580,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   593,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,
      -1,     6,     7,     8,     9,    10,    11,    -1,    -1,   615,
      15,    -1,    -1,    18,    19,   497,    21,   499,    23,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    43,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    57,   659,    -1,    60,    61,    -1,    -1,    64,
     666,    66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,
      -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,   103,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,   112,    -1,   114,
      -1,    -1,   117,    -1,    -1,   721,   722,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   737,    -1,   739,   740,   741,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,   759,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,   773,   173,    -1,
     175,   777,    -1,   178,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,    -1,
      -1,    -1,    -1,    -1,    -1,   801,    -1,    -1,    -1,   805,
      -1,   807,   808,   809,    -1,   811,    -1,   813,    -1,    -1,
     816,   817,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,
      -1,   173,    -1,   175,   830,    -1,   832,    -1,    -1,    -1,
     836,    -1,    -1,    -1,    -1,   841,   842,   843,    -1,   845,
     846,    -1,    -1,    -1,   850,   851,    -1,    -1,    -1,   855,
      -1,    -1,    -1,    -1,   860,    -1,    -1,    -1,    -1,    -1,
      -1,   743,    -1,    -1,   746,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   892,   893,   894,   895,
     896,    -1,    -1,   899,    -1,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   913,   914,    -1,
      -1,    -1,    -1,    18,    19,    -1,    21,   923,    -1,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,   940,    40,    -1,    -1,   944,    -1,
      -1,    46,    -1,    -1,   950,    50,    -1,    -1,    53,    -1,
      55,    -1,   958,    -1,   960,   961,    -1,   839,    -1,    64,
      -1,   843,    -1,    -1,    -1,    -1,    71,    72,   850,    -1,
      -1,    -1,    -1,    -1,    79,    -1,   982,    -1,    -1,    -1,
      -1,    -1,    -1,   989,    89,   991,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,  1001,   101,  1003,   103,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,   112,    -1,   114,
    1016,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1035,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   920,    -1,
     922,  1047,    -1,   925,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1065,
    1066,    -1,    -1,    -1,    -1,    -1,    -1,   949,   173,    -1,
      -1,  1077,    -1,    -1,    -1,    -1,    -1,   959,    -1,  1085,
    1086,  1087,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1105,
      -1,  1107,    -1,    -1,  1110,    -1,  1112,    -1,    -1,  1115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1005,    -1,    -1,    -1,    -1,    -1,  1011,
      -1,  1137,  1138,    -1,  1140,  1141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1160,    -1,    -1,    -1,    -1,  1165,
      -1,  1167,    -1,    -1,    -1,  1171,    -1,    -1,    -1,    -1,
    1176,  1177,    -1,    -1,  1180,    -1,    -1,  1059,    -1,    -1,
      -1,    -1,  1188,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,  1202,  1203,  1204,  1205,
    1206,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1105,    -1,  1107,    -1,    -1,    -1,    -1,
    1112,    -1,    -1,  1115,  1240,  1241,  1242,  1243,  1244,  1245,
    1246,  1247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,  1144,    -1,    -1,    95,    -1,    -1,  1150,    -1,
      -1,    -1,  1154,  1155,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1170,    -1,
      -1,    -1,  1174,    -1,  1176,   125,   126,   127,   128,   129,
     130,   131,    -1,    -1,   134,   135,   136,   137,    -1,   139,
     140,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1240,  1241,
    1242,  1243,     0,     1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    -1,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,   105,   106,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,    -1,   120,    -1,   122,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,     1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      -1,    13,    -1,    15,    16,    17,    18,    19,    20,    21,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    55,    -1,    57,    58,    59,    60,    61,
      -1,    63,    64,    65,    66,    67,    68,    69,    -1,    71,
      72,    73,    -1,    75,    -1,    -1,    78,    79,    80,    81,
      -1,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,    -1,   105,   106,    -1,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,    -1,   120,    -1,
     122,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,    -1,   171,
     172,   173,    -1,   175,    -1,     1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      16,    17,    18,    19,    -1,    21,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    -1,    63,    64,    65,
      66,    67,    68,    69,    -1,    71,    72,    73,    -1,    75,
      -1,    -1,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    94,    -1,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,   105,
     106,    -1,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,    -1,   120,    -1,   122,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
     166,    -1,    -1,   169,    -1,   171,   172,   173,    -1,   175,
      -1,     1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    16,    17,    18,    19,
      -1,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    -1,    63,    64,    65,    66,    67,    68,    69,
      -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,   105,   106,    -1,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,
      -1,   171,   172,   173,    -1,   175,    -1,     1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    16,    17,    18,    19,    -1,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    55,    -1,    57,    58,    59,    60,    61,    -1,    63,
      64,    65,    66,    67,    68,    69,    -1,    71,    72,    73,
      -1,    75,    -1,    -1,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,   105,   106,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,    -1,   120,    -1,   122,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,   166,    -1,    -1,   169,    -1,   171,   172,   173,
      -1,   175,    -1,     1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    -1,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,   105,   106,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,    -1,   120,    -1,   122,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,
      -1,   169,    -1,   171,   172,   173,    -1,   175,    -1,     1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    16,    17,    18,    19,    -1,    21,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    55,    -1,    57,    58,    59,    60,    61,
      -1,    63,    64,    65,    66,    67,    68,    69,    -1,    71,
      72,    73,    -1,    75,    -1,    -1,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    96,    97,    98,    99,   100,   101,
     102,   103,    -1,   105,   106,    -1,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,    -1,   120,    -1,
     122,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,   166,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,   175,    -1,     1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      16,    17,    18,    19,    -1,    21,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    -1,    63,    64,    65,
      66,    67,    68,    69,    -1,    71,    72,    73,    -1,    75,
      -1,    -1,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    94,    -1,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,   105,
     106,    -1,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,    -1,   120,    -1,   122,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
     166,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,
      -1,     1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    16,    17,    18,    19,
      -1,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    -1,    63,    64,    65,    66,    67,    68,    69,
      -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,   105,   106,    -1,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,   175,    -1,     1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    16,    17,    18,    19,    -1,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    55,    -1,    57,    58,    59,    60,    61,    -1,    63,
      64,    65,    66,    67,    68,    69,    -1,    71,    72,    73,
      -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,   105,   106,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,    -1,   120,    -1,   122,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,   166,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,   175,    -1,     1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    -1,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,
      78,    79,    80,    81,    -1,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,   105,   106,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,    -1,   120,    -1,   122,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,     1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    16,    17,    18,    19,    -1,    21,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    55,    -1,    57,    58,    59,    60,    61,
      -1,    63,    64,    65,    66,    67,    68,    69,    -1,    71,
      72,    73,    -1,    75,    -1,    -1,    78,    79,    80,    81,
      -1,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    96,    97,    98,    99,   100,   101,
     102,   103,    -1,   105,   106,    -1,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,    -1,   120,    -1,
     122,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,   166,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,   175,    -1,     1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      16,    17,    18,    19,    -1,    21,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    -1,    63,    64,    65,
      66,    67,    68,    69,    -1,    71,    72,    73,    -1,    75,
      -1,    -1,    78,    79,    80,    81,    -1,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    94,    -1,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,   105,
     106,    -1,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,    -1,   120,    -1,   122,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
     166,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,
      -1,     1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    16,    17,    18,    19,
      -1,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    -1,    63,    64,    65,    66,    67,    68,    69,
      -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,   105,   106,    -1,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,   175,     3,     4,   178,     6,
       7,     8,     9,    10,    11,    -1,    13,    -1,    15,    -1,
      -1,    18,    19,    20,    21,    -1,    23,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    56,
      57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,
      -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,
      -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,   103,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,   112,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,   131,    -1,    -1,   134,   135,   136,
     137,    -1,   139,   140,    -1,   142,   143,   144,   145,   146,
     147,    -1,   149,    -1,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,    -1,   173,    -1,    -1,     3,
       4,   178,     6,     7,     8,     9,    10,    11,    -1,    13,
      -1,    15,    -1,    -1,    18,    19,    20,    21,    -1,    23,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    56,    57,    -1,    -1,    60,    61,    -1,    -1,
      64,    -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,
      -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
      -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,    -1,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,    -1,    -1,
     134,   135,   136,   137,    -1,   139,   140,    -1,   142,   143,
     144,   145,   146,   147,    -1,   149,    -1,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,   167,   168,   169,   170,   171,    -1,   173,
      -1,    -1,    -1,    -1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    16,    -1,
      18,    19,    -1,    21,    -1,    23,    24,    -1,    26,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,
      -1,    -1,    60,    61,    -1,    63,    64,    65,    66,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,   105,   106,    -1,
     108,    -1,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,    -1,     3,     4,
     178,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,
      -1,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,
      -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    -1,    83,    -1,
      -1,    -1,    -1,    88,    89,    90,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,   103,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,   112,    -1,   114,
      -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
      -1,     3,     4,   178,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,
      -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    43,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,
      -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,
      -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
     112,   113,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,   175,     3,     4,   178,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    91,    -1,    -1,    -1,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,   148,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,
     169,    -1,   171,   172,   173,    -1,    -1,     3,     4,   178,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    43,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,
      66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,
      -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,   103,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,   112,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,
       3,     4,   178,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    45,    46,    -1,    48,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
     103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,
      -1,   114,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,     3,     4,   178,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,
      -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,
      60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,
      -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,   103,    -1,   105,    -1,    -1,   108,   109,
     110,    -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,    -1,     3,     4,   178,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,
      -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,
      -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,   103,    -1,   105,    -1,
      -1,   108,   109,   110,    -1,   112,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,     3,
       4,   178,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,
      64,    -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,
      -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
      -1,   105,    -1,    -1,   108,   109,   110,    -1,   112,    -1,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
     174,    -1,     3,     4,   178,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,    -1,   105,    -1,    -1,   108,   109,   110,
      -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,    -1,   176,    -1,   178,     3,     4,
      -1,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,
      -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,
      -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,   103,    -1,
     105,    -1,    -1,   108,   109,   110,    -1,   112,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,   174,
      -1,     3,     4,   178,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,
      -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,
      -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,
      -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,   103,    -1,   105,    -1,    -1,   108,   109,   110,    -1,
     112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,   174,    -1,     3,     4,   178,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,    -1,   105,    -1,    -1,   108,
     109,   110,    -1,   112,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,   174,    -1,     3,     4,   178,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,
      66,    -1,    68,    69,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,
      -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,   103,    -1,   105,
      -1,    -1,   108,   109,   110,    -1,   112,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,
       3,     4,   178,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
     103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,   148,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,   166,    -1,    -1,   169,    -1,   171,   172,
     173,    -1,    -1,     3,     4,   178,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,
      -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,
      60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,
      -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,   103,    -1,   105,    -1,    -1,   108,    -1,
      -1,    -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,   148,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,
      -1,   171,   172,   173,    -1,    -1,     3,     4,   178,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,
      -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,
      -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,   103,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,   112,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,   148,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,
      -1,    -1,   169,    -1,   171,   172,   173,    -1,    -1,     3,
       4,   178,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,
      -1,    -1,    26,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,
      64,    -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,
      -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
      -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,    -1,
     114,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,    -1,     3,     4,   178,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,    -1,   105,    -1,    -1,   108,   109,   110,
      -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,    -1,     3,     4,   178,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,
      18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,
      -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,
      68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,
      88,    89,    90,    91,    -1,    -1,    -1,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,    -1,   105,    -1,    -1,
     108,    -1,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,    -1,     3,     4,
     178,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,
      -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,
      -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,   103,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,   112,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,   123,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
      -1,   176,    -1,   178,     3,     4,    -1,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,   148,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,   174,    -1,     3,     4,   178,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,
      66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,
      -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,   103,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,   112,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,
       3,     4,   178,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
     103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
     123,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,     3,     4,   178,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,
      -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,
      60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,
      -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,   103,    -1,   105,    -1,    -1,   108,    -1,
      -1,    -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,    -1,   176,    -1,   178,     3,
       4,    -1,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,
      64,    -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,
      -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
      -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,    -1,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,   172,   173,
      -1,    -1,     3,     4,   178,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,    -1,   176,    -1,   178,     3,     4,
      -1,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,
      -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,
      -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,   103,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,   112,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
      -1,   176,    -1,   178,     3,     4,    -1,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,    -1,     3,     4,   178,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,
      66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,
      -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,   103,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,   112,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,
       3,     4,   178,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
     103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,     3,     4,   178,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,
      -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,
      60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,
      -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,   103,    -1,   105,    -1,    -1,   108,    -1,
      -1,    -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,    -1,     3,     4,   178,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,
      -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,
      -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,   103,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,   112,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,     3,
       4,   178,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,
      64,    -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,
      -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
      -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,    -1,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,    -1,     3,     4,   178,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    32,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,    -1,    -1,     4,   178,     6,     7,
       8,     9,    10,    11,    -1,   142,   143,    15,   145,   146,
     147,    19,   149,   150,   151,    23,   153,   154,   155,   156,
      28,   158,   159,   160,   161,   162,    -1,   164,    36,    -1,
     167,   168,   169,    41,    42,    43,    -1,    45,    -1,    -1,
      48,   178,    -1,    -1,    52,    -1,    -1,    -1,    -1,    57,
      -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    66,    -1,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    32,    77,
      -1,    79,    80,    81,    82,    83,    -1,    -1,    -1,    -1,
      88,    -1,    90,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      98,    -1,   100,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    95,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,   174,   175,    -1,     4,
     178,     6,     7,     8,     9,    10,    11,    -1,   142,   143,
      15,   145,   146,   147,    19,   149,   150,   151,    23,    -1,
     154,    -1,    -1,    28,    -1,    -1,   160,   161,    -1,    -1,
     164,    36,    -1,   167,   168,   169,    41,    42,    43,    -1,
      45,    -1,    -1,    48,   178,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,    -1,
      -1,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    80,    81,    82,    83,    -1,
      -1,    -1,    -1,    88,    -1,    90,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    98,    -1,   100,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    13,    -1,   151,    -1,    -1,    18,
      19,    20,    21,    -1,    -1,   160,    -1,    26,   163,   164,
     165,    -1,    -1,    -1,   169,    34,   171,    36,   173,    -1,
     175,    40,    -1,   178,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,    -1,   101,    -1,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,
     129,   130,   131,    -1,    -1,   134,   135,   136,   137,    -1,
     139,   140,    -1,   142,   143,   144,   145,   146,   147,    -1,
     149,    -1,    -1,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    -1,   164,    -1,    -1,   167,   168,
     169,   170,     3,     4,   173,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    43,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,    -1,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,     3,     4,    -1,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,
      18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    43,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,
      -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,
      68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,   103,    -1,   105,    -1,    -1,
     108,    -1,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,     3,     4,    -1,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,   142,    26,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    43,    -1,    45,    46,    -1,   165,    -1,    50,
      -1,    -1,    53,   171,    55,   173,    57,   175,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,    -1,   105,    -1,    -1,   108,     3,    -1,
      -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    19,    -1,    21,    -1,    -1,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,   142,    -1,   144,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    64,
      -1,    -1,   173,    -1,   175,    -1,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,    -1,   101,    -1,   103,    -1,
     105,     3,    -1,   108,    -1,    -1,    -1,   112,    -1,   114,
      -1,    13,   117,    -1,    -1,    -1,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   173,    -1,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,   101,
      -1,   103,    -1,   105,    -1,    -1,    -1,    -1,    -1,   111,
     112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,   131,
      -1,    -1,   134,   135,   136,   137,    -1,   139,   140,    -1,
     142,   143,   144,   145,   146,   147,    -1,   149,    -1,    -1,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,     3,   164,    -1,    -1,   167,   168,   169,   170,   171,
      -1,    13,    -1,    -1,    -1,    -1,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    98,    99,    -1,   101,
      -1,   103,    20,   105,    -1,    -1,   108,    -1,    -1,    -1,
     112,    -1,   114,    -1,    32,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,   131,
      -1,    -1,   134,   135,   136,   137,    -1,   139,   140,    -1,
     142,   143,   144,   145,   146,   147,    -1,   149,    -1,    -1,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,    -1,   164,    -1,    -1,   167,   168,   169,   170,    -1,
      -1,    -1,    -1,    91,    13,    -1,    -1,    95,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,    -1,    -1,   134,   135,   136,   137,
      -1,   139,   140,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    91,   171,    -1,    13,    95,    -1,    -1,    -1,
     178,    -1,    20,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,   131,    -1,    -1,   134,   135,   136,   137,    -1,
     139,   140,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    -1,   171,    91,    13,    -1,    -1,    95,    -1,   178,
      -1,    20,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,    -1,    -1,   134,   135,   136,   137,
      -1,   139,   140,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    91,   171,    -1,    13,    95,    -1,    -1,    -1,
     178,    -1,    20,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,   131,    -1,    -1,   134,   135,   136,   137,    -1,
     139,   140,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    -1,   171,    91,    -1,    -1,    -1,    95,    -1,   178,
      13,    -1,    -1,    -1,    -1,    -1,   104,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,    -1,    48,   134,   135,   136,   137,
      -1,   139,   140,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,   171,    -1,    -1,    -1,    -1,    91,    13,
     178,    -1,    95,    -1,    -1,    -1,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,   125,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    91,   171,    -1,
      -1,    95,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    32,
      -1,   125,    -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    74,    -1,   167,   168,   169,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    14,    -1,    -1,    -1,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,   125,    -1,    -1,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    74,   166,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    91,
      13,    -1,    -1,    95,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    91,    -1,
      -1,    13,    95,    -1,    -1,    -1,   178,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,   171,    91,
      13,    -1,    -1,    95,    -1,   178,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    91,   171,
      -1,    13,    95,    -1,    -1,    -1,   178,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,   171,    91,
      13,    -1,    -1,    95,    -1,   178,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,   125,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    91,    -1,
      13,    -1,    95,    -1,    -1,    -1,   178,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,   104,    13,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,   125,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    74,   167,   168,   169,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      91,    13,    -1,    -1,    95,    -1,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    91,
      -1,    13,    -1,    95,    -1,    -1,    -1,   178,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    14,    -1,    -1,    -1,    -1,    -1,    20,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      32,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,    91,
      -1,    13,    -1,    95,   176,    -1,   178,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    13,    -1,    95,    -1,    -1,    -1,    -1,    20,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      32,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   125,    -1,   167,   168,   169,    -1,    91,
      -1,    -1,    -1,    95,   176,    -1,   178,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   125,    -1,   167,   168,   169,    -1,    91,
      -1,    13,    -1,    95,    -1,    -1,   178,    -1,    20,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      32,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   125,    -1,   167,   168,   169,    -1,   171,
      -1,    13,    -1,    -1,    -1,    -1,   178,    -1,    20,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      32,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,   166,   167,   168,   169,    -1,    91,
      -1,    13,    14,    95,    -1,    -1,   178,    -1,    20,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   125,    -1,   167,   168,   169,    -1,    91,
      -1,    13,    -1,    95,    -1,    -1,   178,    -1,    20,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      32,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   125,    -1,   167,   168,   169,    -1,    -1,
      -1,    -1,   174,    -1,    -1,    -1,   178,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,    91,
      13,    -1,    -1,    95,    -1,    -1,   178,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      32,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    91,    -1,
      -1,    13,    95,    -1,    -1,    -1,   178,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,   125,   167,   168,   169,    -1,    -1,    91,
      -1,    13,    14,    95,    -1,   178,    -1,    -1,    20,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      32,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   125,    -1,   167,   168,   169,    -1,    -1,
      -1,    13,   174,    -1,    -1,    -1,   178,    -1,    20,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      32,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,    91,
      -1,    -1,   174,    95,    -1,    -1,   178,    13,    14,    -1,
      -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   125,    -1,   167,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    91,   178,    -1,    -1,    95,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    13,    -1,   167,   168,   169,    -1,   125,
      20,    -1,   174,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    13,
      -1,   167,   168,   169,    -1,    -1,    20,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    32,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    95,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    95,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    13,   166,   167,   168,   169,   124,   125,    20,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    13,    -1,   167,
     168,   169,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    13,    14,    -1,
      -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    95,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,   125,
      -1,    -1,    -1,    -1,   176,    91,   178,    13,    -1,    95,
      -1,    -1,    -1,    -1,    20,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    32,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   125,
      -1,   167,   168,   169,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    -1,   178,    -1,    20,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    32,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,    91,    -1,    13,    -1,    95,
      -1,    -1,   178,    -1,    20,    -1,    62,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    13,    -1,    95,
      -1,    -1,    -1,    -1,    20,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    32,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   125,
      -1,   167,   168,   169,    -1,    91,    -1,    13,    -1,    95,
      -1,    -1,   178,    -1,    20,    -1,   142,   143,   104,   145,
     146,   147,    -1,   149,   150,   151,    32,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   125,
      -1,   167,   168,   169,    -1,    91,    -1,    13,    -1,    95,
      -1,    -1,   178,    -1,    20,    -1,   142,   143,   104,   145,
     146,   147,    -1,   149,   150,   151,    32,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   125,
      -1,   167,   168,   169,    -1,    91,    -1,    13,    -1,    95,
      -1,    -1,   178,    -1,    20,    -1,   142,   143,   104,   145,
     146,   147,    -1,   149,   150,   151,    32,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   125,
      -1,   167,   168,   169,    -1,    91,    -1,    13,    -1,    95,
      -1,    -1,   178,    -1,    20,    -1,   142,   143,   104,   145,
     146,   147,    -1,   149,   150,   151,    32,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   125,
      -1,   167,   168,   169,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,   178,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   125,
      -1,   167,   168,   169,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,   178,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   125,
      -1,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    32,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      23,   167,   168,   169,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    36,    -1,    38,    39,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    91,    -1,    -1,    -1,    95,
      -1,    -1,    75,    -1,    -1,    78,    -1,    80,    -1,    -1,
      -1,    84,    85,    86,    87,    -1,    -1,    90,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,    -1,   158,   159,   160,   161,   162,    -1,   164,    -1,
      -1,   167,   168,   169,     1,    -1,     3,    -1,     5,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    19,   166,    21,    -1,    -1,    -1,   171,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    -1,    18,    19,    -1,
      21,    -1,    79,    -1,    -1,    26,    -1,    28,    -1,    -1,
      -1,    -1,    89,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    98,    99,    -1,   101,    46,   103,    -1,   105,    50,
      -1,   108,    53,    -1,    55,   112,    -1,   114,    -1,    -1,
     117,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    92,    -1,    -1,     3,    -1,     5,    98,    99,    -1,
     101,    -1,   103,    -1,   105,    -1,    -1,   108,    -1,    18,
      19,   112,    21,   114,    -1,   116,   117,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
       3,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    -1,    18,    19,    -1,    21,    -1,
      79,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    98,
      99,    -1,   101,    46,   103,    -1,   105,    50,    -1,   108,
      53,    -1,    55,   112,    -1,   114,     3,    -1,   117,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      -1,    18,    19,    -1,    21,    -1,    79,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    98,    99,    -1,   101,    46,
     103,    -1,   105,    50,    -1,   108,    53,    -1,    55,   112,
      -1,   114,     3,    -1,   117,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    -1,    18,    19,    -1,
      21,    -1,    79,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    98,    99,    -1,   101,    46,   103,    -1,   105,    50,
      -1,   108,    53,    -1,    55,   112,    -1,   114,    -1,    -1,
     117,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,
     101,    -1,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,   112,    -1,   114,    -1,    -1,   117
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   183,   184,     0,     1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    15,    16,    17,    18,    19,    21,
      23,    24,    26,    27,    28,    29,    30,    31,    34,    36,
      38,    39,    40,    44,    45,    46,    49,    50,    51,    53,
      55,    57,    58,    59,    60,    61,    63,    64,    65,    66,
      68,    69,    71,    72,    73,    78,    79,    80,    81,    82,
      83,    84,    87,    88,    89,    90,    92,    93,    94,    96,
      97,    98,    99,   100,   101,   102,   103,   105,   106,   108,
     109,   110,   111,   112,   113,   114,   116,   117,   122,   125,
     142,   143,   144,   145,   146,   151,   160,   163,   164,   165,
     166,   169,   171,   173,   178,   185,   186,   187,   188,   189,
     190,   193,   194,   195,   196,   201,   202,   203,   205,   206,
     207,   212,   213,   217,   218,   222,   224,   227,   228,   231,
     232,   233,   234,   235,   236,   239,   240,   241,   245,   247,
     250,   251,   252,   253,   254,   258,   259,   264,   265,   266,
     267,   270,   271,   276,   277,   279,   280,   285,   289,   290,
     292,   293,   314,   319,   320,   324,   325,   346,   347,   348,
     350,   352,   353,   354,   362,   363,   364,   365,   366,   367,
     369,   372,   373,   374,   375,   376,   377,   378,   379,   381,
     382,   383,   384,   385,   166,    23,    36,    45,    55,    57,
      90,   103,   171,   240,   250,   277,   346,   353,   363,   364,
     369,   372,   374,   375,   121,   355,   356,     3,    55,   219,
     369,   355,   111,   326,    92,   219,   187,   340,   369,   173,
       3,    18,    19,    21,    26,    34,    40,    46,    50,    53,
      55,    64,    71,    72,    79,    89,    98,    99,   101,   103,
     105,   108,   112,   114,   117,   220,   221,   223,    12,    28,
     116,   246,   369,    84,    87,   203,   173,   105,   221,   221,
     221,   173,   221,   327,   328,    33,   207,   226,   369,   262,
     263,   369,   369,    19,    79,    98,   114,   369,   369,   369,
       9,   173,   230,   229,    28,    34,    48,    50,    52,    77,
      80,    92,    99,   103,   111,   186,   225,   281,   282,   283,
     305,   306,   307,   332,   337,   369,   340,   109,   110,   166,
     285,   286,   368,   369,   371,   369,   226,   369,   369,   369,
     102,   173,   187,   369,   369,   189,   193,   207,   189,   207,
     369,   371,   369,   369,   369,   369,   369,     1,   172,   185,
     208,   340,   152,   341,   342,   371,   369,    82,   187,    23,
      36,    39,    90,   171,   190,   191,   192,   203,   207,   195,
     150,   197,   171,    47,    86,   115,   204,    27,   325,   369,
       9,   266,   369,   370,    25,    33,    41,    42,    43,   120,
     175,   242,   243,   349,   351,    57,   147,   268,   221,   171,
     173,   301,    54,    75,    85,   309,    28,    77,    80,    92,
     111,   310,    28,    80,    92,   111,   308,   221,   321,   322,
       1,   327,   165,   166,   369,    13,    20,    32,    91,    95,
     125,   142,   143,   145,   146,   147,   149,   150,   151,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   167,   168,   169,   178,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     170,   297,   173,   175,    91,    95,   369,   221,    41,   175,
     242,    57,   173,   187,   166,   207,   324,   166,   148,   166,
     222,   343,   344,   345,   371,   221,   221,   104,   207,   104,
     126,   207,   297,   204,   343,   166,   173,   173,   207,   187,
     173,   221,   329,   330,     1,   147,   336,    48,   148,   187,
     226,   148,   226,    14,   173,   173,   226,   343,   231,   231,
      48,    92,   306,   307,   174,   148,   173,   221,   147,   166,
     369,   369,   124,   287,   166,   171,   226,   173,   343,   166,
     255,   255,   166,   172,   172,   185,   148,   172,   369,   148,
     148,   174,   174,     9,   175,   242,   243,     3,   173,   198,
       1,   172,   208,   215,   216,   369,   210,   369,    67,    37,
      74,   166,   266,   268,   111,   237,   290,   369,   187,    80,
     246,   369,   123,   176,   244,   340,   369,   380,   244,   369,
     221,   340,   171,     1,   194,   221,   272,   275,   174,   302,
     304,   305,   310,    92,     1,   147,   334,   335,    92,     1,
       3,    13,    18,    20,    21,    26,    46,    53,    55,    56,
      64,    72,    89,   101,   105,   112,   117,   142,   143,   144,
     145,   146,   147,   149,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   164,   167,   168,   169,   170,   173,
     221,   294,   295,   296,   297,   346,   126,   323,   148,   166,
     166,   166,   369,   369,   369,   244,   369,   244,   369,   369,
     369,   369,   369,   369,   369,     3,    21,    34,    64,   105,
     111,   222,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   369,    70,
     371,   371,   371,   371,   371,   343,   343,   244,   369,   244,
     369,   104,   173,   340,   369,   221,    28,    48,    92,   116,
     357,   360,   361,   369,   385,   327,   369,   126,   174,   148,
     126,   126,   187,    35,   187,   369,    35,   369,    67,   174,
     344,   221,   248,   249,   330,   148,   174,   225,   369,   126,
     331,   369,   328,   262,   221,   324,   369,   369,   174,   221,
     310,   283,   330,   147,   338,   339,   337,   288,   206,     1,
     260,   345,   174,    22,   256,   172,   174,   174,   341,   371,
     174,   341,   340,   369,   197,   174,   199,   200,   345,   172,
     172,   148,   166,    14,   150,   148,   166,    14,    37,    74,
     221,   142,   143,   144,   145,   146,   160,   164,   169,   209,
     296,   297,   298,   369,   209,   211,   268,   171,   238,   324,
      48,   355,   120,   221,   226,   226,   173,   369,   121,   176,
     358,    48,   148,   176,   358,   124,   148,   176,   226,   358,
      33,    48,   226,   358,   166,   173,     1,   269,   172,   275,
     126,   148,   172,   148,   174,   336,     1,   225,   332,   333,
     107,   311,   173,   300,   369,   142,   150,   300,   300,   369,
     322,   173,   175,   166,   166,   166,   166,   166,   166,   174,
     176,   344,    48,   176,    33,    48,   173,    48,    92,   148,
     174,    18,    21,    26,    46,    53,    64,    72,    89,   101,
     112,   117,   346,    91,    91,   166,   371,   345,   369,   369,
      35,   187,    35,    35,   187,   104,   207,   221,   174,   148,
     174,   174,   329,   336,    70,   371,   221,   174,   174,   339,
     147,   284,   174,   337,   152,   299,   331,   369,   172,    76,
     118,   172,   261,   174,   173,   207,   221,   257,    48,   176,
      48,   148,   174,   215,   222,    18,    19,    21,    26,    46,
      50,    53,    72,    79,    89,    98,    99,   101,   103,   112,
     114,   117,   171,   214,   298,   369,   369,   209,   211,   148,
     166,    14,   166,   171,   269,   321,   327,   244,   369,   226,
     369,    48,   340,   173,   187,   176,   244,   369,   176,   187,
     369,   176,   369,   176,   369,   226,    45,   369,   244,   369,
     226,   344,   172,    84,    87,   172,   186,   194,   228,   369,
     273,   274,   304,   311,    62,   119,   315,   302,   303,   174,
     295,   297,   174,   176,   174,   244,   369,    45,   244,   369,
     344,   360,   336,   346,   346,   187,   187,   369,   187,    35,
     166,   166,   249,   207,   331,   173,   173,   299,   331,   332,
     311,   339,   369,   187,   240,   340,   257,   147,   207,   244,
     369,   244,   369,   200,   209,    14,    37,    74,   166,   166,
     298,   369,   369,   269,   172,   166,   166,   355,   355,   166,
     369,   174,   359,   360,   187,   176,   358,   176,   358,   187,
     124,   369,    33,   226,   358,    33,   226,   358,   174,   194,
     228,   228,   275,   194,   315,     3,    55,    94,   105,   316,
     317,   318,   369,   291,   174,   300,   300,   176,   176,   369,
      33,    33,   174,   331,    35,   187,   343,   343,   299,   331,
      33,   226,   174,   369,   176,   176,   172,   369,   209,   211,
      14,   172,   226,   226,   226,   148,   174,    45,   187,   369,
     176,    45,   187,   369,   176,   369,   104,    45,   369,   226,
      45,   369,   226,   166,   228,   275,   278,   318,   119,   148,
     126,   153,   155,   156,   158,   159,    62,    33,   166,   206,
     312,   313,    45,    45,   104,    45,    45,   187,   174,   174,
     187,   207,   166,   166,   369,   360,   369,   187,   369,   187,
     369,   369,   369,   313,   369,   317,   318,   318,   318,   318,
     318,   318,   316,   185,   369,   369,   369,   369,   165,   165,
     104,   104,   104,   104,   104,   104,   104,   104,   369,   369,
     369,   369
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
     211,   211,   212,   212,   212,   212,   212,   213,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   215,   215,   215,   215,   215,
     216,   216,   217,   218,   218,   218,   218,   218,   218,   219,
     219,   219,   220,   220,   220,   220,   220,   221,   221,   221,
     222,   222,   222,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   225,   225,   225,   225,   226,   226,
     227,   227,   228,   228,   229,   228,   230,   228,   231,   231,
     231,   231,   231,   231,   231,   232,   232,   232,   232,   233,
     234,   234,   235,   236,   236,   236,   237,   236,   238,   236,
     239,   240,   241,   241,   242,   242,   242,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     244,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   246,   246,   247,   247,   248,   248,   249,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   251,   251,   251,   251,   251,   251,   252,
     252,   252,   253,   253,   253,   254,   254,   254,   254,   255,
     255,   256,   256,   256,   257,   257,   258,   259,   259,   260,
     260,   261,   261,   261,   262,   262,   262,   263,   263,   264,
     265,   265,   266,   267,   267,   267,   268,   268,   269,   269,
     269,   269,   269,   270,   270,   271,   272,   272,   273,   272,
     272,   274,   272,   275,   275,   276,   278,   277,   279,   280,
     280,   280,   281,   281,   282,   282,   283,   283,   283,   284,
     284,   285,   287,   286,   288,   286,   289,   291,   290,   292,
     292,   292,   292,   292,   293,   294,   294,   295,   295,   295,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   297,   298,   298,   299,
     299,   300,   300,   301,   301,   302,   302,   303,   303,   304,
     305,   305,   305,   305,   305,   305,   306,   306,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   308,   308,   308,
     308,   308,   308,   309,   309,   309,   310,   310,   310,   310,
     310,   310,   310,   311,   311,   312,   312,   313,   313,   314,
     315,   315,   315,   315,   315,   316,   316,   317,   317,   317,
     317,   317,   317,   317,   318,   318,   318,   319,   320,   320,
     321,   321,   322,   323,   323,   324,   324,   324,   324,   324,
     326,   325,   325,   325,   327,   327,   328,   328,   329,   329,
     330,   330,   330,   331,   331,   331,   332,   333,   333,   333,
     334,   334,   335,   335,   336,   336,   336,   336,   337,   337,
     338,   339,   339,   340,   340,   341,   342,   342,   343,   343,
     344,   344,   345,   345,   346,   346,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   348,   349,
     349,   349,   349,   349,   349,   350,   351,   351,   351,   351,
     351,   351,   351,   352,   353,   354,   354,   354,   354,   354,
     354,   354,   355,   355,   356,   357,   357,   358,   359,   359,
     360,   360,   360,   361,   361,   361,   361,   361,   361,   362,
     362,   362,   362,   362,   363,   363,   363,   363,   363,   364,
     365,   365,   365,   365,   365,   365,   366,   367,   368,   368,
     369,   369,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   370,   370,   371,
     371,   371,   372,   372,   372,   372,   373,   373,   373,   373,
     373,   374,   374,   374,   375,   375,   375,   375,   375,   375,
     376,   376,   376,   376,   377,   377,   378,   378,   379,   379,
     379,   379,   379,   379,   379,   379,   379,   379,   379,   379,
     379,   380,   380,   381,   381,   381,   381,   381,   381,   381,
     381,   381,   381,   381,   381,   381,   381,   381,   381,   381,
     381,   381,   381,   381,   381,   381,   382,   382,   382,   382,
     382,   382,   382,   383,   383,   383,   383,   384,   384,   384,
     384,   385,   385,   385,   385,   385,   385,   385
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     1,     3,     3,     3,     3,     3,     2,
       1,     1,     1,     1,     1,     2,     1,     3,     3,     3,
       3,     2,     3,     3,     2,     2,     1,     1,     0,     1,
       1,     1,     1,     2,     3,     1,     0,     3,     2,     3,
       0,     1,     3,     1,     4,     3,     4,     4,     0,     1,
       1,     0,     1,     6,     2,     3,     3,     1,     1,     2,
       1,     1,     3,     3,     3,     5,     1,     3,     3,     5,
       0,     1,     4,     6,     8,     6,     8,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     5,
       1,     3,     3,     4,     4,     4,     4,     4,     4,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       2,     3,     1,     1,     0,     3,     0,     3,     1,     1,
       1,     1,     1,     1,     1,     3,     5,     5,     2,     1,
       1,     1,     1,     6,     7,     3,     0,     6,     0,     6,
       2,     0,     3,     2,     1,     1,     1,     5,     3,     3,
       6,     6,     4,     6,     5,     6,     5,     6,     3,     4,
       3,     4,     6,     7,     6,     7,     4,     5,     4,     5,
       4,     4,     3,     6,     5,     4,     3,     6,     5,     6,
       5,     8,     7,     4,     4,     6,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     6,
       4,     7,     5,     3,     6,     2,     2,     3,     3,     0,
       2,     2,     3,     5,     1,     3,     3,     5,     5,     0,
       2,     3,     2,     4,     4,     3,     1,     1,     3,     3,
       5,     5,     2,     1,     1,     1,     0,     2,     0,     2,
       3,     3,     4,     4,     4,     2,     1,     2,     0,     4,
       2,     0,     5,     1,     3,     1,     0,     8,     0,     1,
       1,     1,     0,     1,     1,     3,     1,     3,     1,     0,
       2,     6,     0,     3,     0,     4,     1,     0,     7,     4,
       4,     6,     6,     4,     2,     1,     3,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     0,     3,     2,     3,     1,     3,     0,     1,     1,
       4,     5,     4,     5,     6,     6,     0,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     0,     1,     1,
       2,     1,     1,     1,     1,     1,     0,     1,     1,     2,
       1,     1,     1,     0,     1,     1,     1,     1,     2,     1,
       0,     2,     2,     4,     4,     1,     3,     3,     3,     3,
       3,     3,     3,     2,     1,     1,     1,     3,     1,     2,
       1,     3,     2,     0,     2,     1,     2,     1,     1,     1,
       0,     5,     3,     3,     1,     3,     3,     5,     1,     3,
       1,     2,     3,     0,     2,     2,     1,     1,     1,     1,
       2,     1,     0,     1,     0,     2,     2,     1,     1,     1,
       2,     0,     1,     1,     3,     1,     3,     3,     0,     1,
       1,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     4,     4,     4,     5,     2,     2,     1,     2,
       1,     2,     1,     2,     1,     2,     1,     1,     2,     6,
       6,     4,     9,     9,     7,     2,     2,     3,     4,     6,
       6,     9,     9,     6,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     4,     1,     3,     4,     1,     3,
       4,     3,     3,     1,     1,     2,     1,     2,     1,     1,
       2,     2,     2,     2,     2,     8,     8,     9,     9,     4,
       3,     3,     2,     2,     2,     1,     3,     4,     1,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     4,     4,     4,     3,     3,     3,     3,     5,     5,
       3,     4,     3,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     4,     4,     5,     4,
       5,     3,     5,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     1,     1
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
#line 5918 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 640 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 5924 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 641 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 5930 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 648 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 5938 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 656 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 5946 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 660 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 5955 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 670 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 5963 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 675 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 5971 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 682 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5977 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 683 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5983 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 684 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5989 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 685 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5995 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6001 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 687 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6007 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 688 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6013 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 689 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6019 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 690 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6025 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: TDEFER stmt  */
#line 692 "chpl.ypp"
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
#line 6047 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 709 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6053 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 710 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6059 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: TINIT TTHIS TSEMI  */
#line 712 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr));
    auto node = Init::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6071 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 720 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6084 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 729 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6097 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TLABEL ident_def stmt  */
#line 738 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6105 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 742 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6116 "bison-chpl-lib.cpp"
    break;

  case 29: /* stmt_base: error TSEMI  */
#line 749 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6125 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: assignment_stmt  */
#line 756 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6131 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: if_stmt  */
#line 757 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6137 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: loop_stmt  */
#line 758 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6143 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: select_stmt  */
#line 759 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6149 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: manage_stmt  */
#line 760 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6155 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 761 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6161 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: throw_stmt  */
#line 762 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6167 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 764 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6173 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
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
    BUILDER->noteBlockHeaderLocation(node.get(), LOC((yylsp[-2])));
    if (context->hasCurlyBracesLoc()) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC(context->curlyBracesLoc()));
      context->resetCurlyBracesLoc();
    }
    CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6197 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 786 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6209 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 794 "chpl.ypp"
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
#line 6230 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TLOCAL do_stmt  */
#line 811 "chpl.ypp"
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
#line 6250 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TON expr do_stmt  */
#line 827 "chpl.ypp"
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
#line 6271 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 844 "chpl.ypp"
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
#line 6292 "bison-chpl-lib.cpp"
    break;

  case 44: /* tryable_stmt: TSERIAL do_stmt  */
#line 861 "chpl.ypp"
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
#line 6312 "bison-chpl-lib.cpp"
    break;

  case 45: /* tryable_stmt: TSYNC stmt  */
#line 877 "chpl.ypp"
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
#line 6334 "bison-chpl-lib.cpp"
    break;

  case 48: /* collect_attributes: %empty  */
#line 902 "chpl.ypp"
  {
    auto attributeGroup = context->buildAttributeGroup((yyloc));
    if (attributeGroup != nullptr) {
      context->resetAttributeGroupPartsState();
    }
    context->loopAttributes.push_back(std::move(attributeGroup));
  }
#line 6346 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 927 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6354 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATTRIBUTEIDENT  */
#line 934 "chpl.ypp"
  {
    auto ident = context->buildAttributeIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6363 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 941 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6371 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 945 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6382 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 955 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6388 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 957 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6396 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 961 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6404 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 968 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6413 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 973 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6423 "bison-chpl-lib.cpp"
    break;

  case 64: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 986 "chpl.ypp"
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
#line 6445 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 1007 "chpl.ypp"
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
#line 6466 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 1024 "chpl.ypp"
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
#line 6487 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1041 "chpl.ypp"
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
#line 6506 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1058 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6512 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1059 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6519 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1061 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6526 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1066 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6532 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1067 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6539 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1073 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6549 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1091 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-1]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6558 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1096 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6567 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR error TRCBR  */
#line 1101 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6580 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt: block_stmt_body  */
#line 1113 "chpl.ypp"
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
#line 6599 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1131 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6605 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1132 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6611 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1137 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6619 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1141 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6627 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1145 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6636 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1150 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6644 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1154 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6652 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1158 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6661 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1166 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6670 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1171 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6680 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1177 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6689 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1182 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6700 "bison-chpl-lib.cpp"
    break;

  case 90: /* opt_only_ls: %empty  */
#line 1190 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6706 "bison-chpl-lib.cpp"
    break;

  case 91: /* opt_only_ls: renames_ls  */
#line 1191 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6712 "bison-chpl-lib.cpp"
    break;

  case 92: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1196 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6721 "bison-chpl-lib.cpp"
    break;

  case 93: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1201 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6733 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1209 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6746 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1218 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6758 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1226 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6771 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1237 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6780 "bison-chpl-lib.cpp"
    break;

  case 115: /* import_expr: expr  */
#line 1273 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6788 "bison-chpl-lib.cpp"
    break;

  case 116: /* import_expr: expr TDOT all_op_name  */
#line 1277 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6797 "bison-chpl-lib.cpp"
    break;

  case 117: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1282 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6806 "bison-chpl-lib.cpp"
    break;

  case 118: /* import_expr: expr TAS ident_use  */
#line 1287 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6816 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1293 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6826 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_ls: import_expr  */
#line 1301 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6832 "bison-chpl-lib.cpp"
    break;

  case 121: /* import_ls: import_ls TCOMMA import_expr  */
#line 1302 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6838 "bison-chpl-lib.cpp"
    break;

  case 122: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1307 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6848 "bison-chpl-lib.cpp"
    break;

  case 123: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1316 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6856 "bison-chpl-lib.cpp"
    break;

  case 124: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1320 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6864 "bison-chpl-lib.cpp"
    break;

  case 125: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1324 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6872 "bison-chpl-lib.cpp"
    break;

  case 126: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1328 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6880 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1332 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6888 "bison-chpl-lib.cpp"
    break;

  case 128: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1336 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 6896 "bison-chpl-lib.cpp"
    break;

  case 129: /* opt_label_ident: %empty  */
#line 1344 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 6902 "bison-chpl-lib.cpp"
    break;

  case 130: /* opt_label_ident: TIDENT  */
#line 1345 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6908 "bison-chpl-lib.cpp"
    break;

  case 131: /* opt_label_ident: TINIT  */
#line 1346 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6914 "bison-chpl-lib.cpp"
    break;

  case 140: /* ident_use: TIDENT  */
#line 1372 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6920 "bison-chpl-lib.cpp"
    break;

  case 141: /* ident_use: TINIT  */
#line 1373 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6926 "bison-chpl-lib.cpp"
    break;

  case 142: /* ident_use: TTHIS  */
#line 1374 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6932 "bison-chpl-lib.cpp"
    break;

  case 162: /* scalar_type: TBOOL  */
#line 1401 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6938 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TENUM  */
#line 1402 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6944 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TINT  */
#line 1403 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6950 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TUINT  */
#line 1404 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6956 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TREAL  */
#line 1405 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6962 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TIMAG  */
#line 1406 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6968 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TCOMPLEX  */
#line 1407 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6974 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TBYTES  */
#line 1408 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6980 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TSTRING  */
#line 1409 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6986 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TLOCALE  */
#line 1410 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6992 "bison-chpl-lib.cpp"
    break;

  case 172: /* scalar_type: TNOTHING  */
#line 1411 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6998 "bison-chpl-lib.cpp"
    break;

  case 173: /* scalar_type: TVOID  */
#line 1412 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7004 "bison-chpl-lib.cpp"
    break;

  case 178: /* do_stmt: TDO stmt  */
#line 1426 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7010 "bison-chpl-lib.cpp"
    break;

  case 179: /* do_stmt: block_stmt  */
#line 1427 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7016 "bison-chpl-lib.cpp"
    break;

  case 180: /* return_stmt: TRETURN TSEMI  */
#line 1432 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7027 "bison-chpl-lib.cpp"
    break;

  case 181: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1439 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7038 "bison-chpl-lib.cpp"
    break;

  case 182: /* class_level_stmt: TSEMI  */
#line 1449 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7046 "bison-chpl-lib.cpp"
    break;

  case 183: /* class_level_stmt: inner_class_level_stmt  */
#line 1453 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7056 "bison-chpl-lib.cpp"
    break;

  case 184: /* $@1: %empty  */
#line 1458 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7063 "bison-chpl-lib.cpp"
    break;

  case 185: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1460 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7072 "bison-chpl-lib.cpp"
    break;

  case 186: /* $@2: %empty  */
#line 1464 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7079 "bison-chpl-lib.cpp"
    break;

  case 187: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1466 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7088 "bison-chpl-lib.cpp"
    break;

  case 195: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1484 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7097 "bison-chpl-lib.cpp"
    break;

  case 196: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1489 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7106 "bison-chpl-lib.cpp"
    break;

  case 197: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1494 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7115 "bison-chpl-lib.cpp"
    break;

  case 198: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1499 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7123 "bison-chpl-lib.cpp"
    break;

  case 199: /* forwarding_decl_start: TFORWARDING  */
#line 1506 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7132 "bison-chpl-lib.cpp"
    break;

  case 200: /* extern_or_export: TEXTERN  */
#line 1513 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7138 "bison-chpl-lib.cpp"
    break;

  case 201: /* extern_or_export: TEXPORT  */
#line 1514 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7144 "bison-chpl-lib.cpp"
    break;

  case 202: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1519 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7154 "bison-chpl-lib.cpp"
    break;

  case 203: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1529 "chpl.ypp"
  {
    auto headerLoc = context->makeSpannedLocation((yylsp[-5]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7164 "bison-chpl-lib.cpp"
    break;

  case 204: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1536 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    auto headerLoc = context->makeSpannedLocation((yylsp[-6]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7176 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
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
#line 7198 "bison-chpl-lib.cpp"
    break;

  case 206: /* $@3: %empty  */
#line 1562 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7206 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1566 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7215 "bison-chpl-lib.cpp"
    break;

  case 208: /* $@4: %empty  */
#line 1571 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7224 "bison-chpl-lib.cpp"
    break;

  case 209: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1576 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7233 "bison-chpl-lib.cpp"
    break;

  case 210: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1584 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7241 "bison-chpl-lib.cpp"
    break;

  case 211: /* no_loop_attributes: %empty  */
#line 1589 "chpl.ypp"
                    { context->loopAttributes.push_back(nullptr); }
#line 7247 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1592 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7253 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1593 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7259 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1609 "chpl.ypp"
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
#line 7286 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1632 "chpl.ypp"
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
#line 7309 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1651 "chpl.ypp"
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
#line 7332 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1670 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7341 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1675 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7350 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1680 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7359 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1685 "chpl.ypp"
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
#line 7388 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1710 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7397 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: for_loop_kw expr TIN expr forall_intent_clause do_stmt  */
#line 1715 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7406 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1720 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7415 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1725 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7424 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1730 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7433 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: for_loop_kw expr forall_intent_clause do_stmt  */
#line 1735 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7442 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1740 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7451 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: for_loop_kw zippered_iterator forall_intent_clause do_stmt  */
#line 1745 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7460 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1750 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7469 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1755 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7478 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1760 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7487 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1765 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7496 "bison-chpl-lib.cpp"
    break;

  case 236: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1770 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7505 "bison-chpl-lib.cpp"
    break;

  case 237: /* loop_stmt_base: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1775 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7514 "bison-chpl-lib.cpp"
    break;

  case 238: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1780 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7524 "bison-chpl-lib.cpp"
    break;

  case 239: /* loop_stmt_base: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1786 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7534 "bison-chpl-lib.cpp"
    break;

  case 240: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1795 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7544 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF expr TTHEN stmt  */
#line 1804 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7552 "bison-chpl-lib.cpp"
    break;

  case 242: /* if_stmt: TIF expr block_stmt  */
#line 1808 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7560 "bison-chpl-lib.cpp"
    break;

  case 243: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1812 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7568 "bison-chpl-lib.cpp"
    break;

  case 244: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1816 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7576 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1820 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7584 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF ifvar block_stmt  */
#line 1824 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7592 "bison-chpl-lib.cpp"
    break;

  case 247: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1828 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7600 "bison-chpl-lib.cpp"
    break;

  case 248: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1832 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7608 "bison-chpl-lib.cpp"
    break;

  case 249: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1836 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7617 "bison-chpl-lib.cpp"
    break;

  case 250: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1841 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7626 "bison-chpl-lib.cpp"
    break;

  case 251: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1846 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7635 "bison-chpl-lib.cpp"
    break;

  case 252: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1851 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7644 "bison-chpl-lib.cpp"
    break;

  case 253: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1859 "chpl.ypp"
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
#line 7663 "bison-chpl-lib.cpp"
    break;

  case 254: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1874 "chpl.ypp"
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
#line 7682 "bison-chpl-lib.cpp"
    break;

  case 255: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1892 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-4]), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7690 "bison-chpl-lib.cpp"
    break;

  case 256: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1896 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7698 "bison-chpl-lib.cpp"
    break;

  case 257: /* ifc_formal_ls: ifc_formal  */
#line 1902 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7704 "bison-chpl-lib.cpp"
    break;

  case 258: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1903 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7710 "bison-chpl-lib.cpp"
    break;

  case 259: /* ifc_formal: ident_def  */
#line 1908 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7716 "bison-chpl-lib.cpp"
    break;

  case 279: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1952 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7724 "bison-chpl-lib.cpp"
    break;

  case 280: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1956 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7732 "bison-chpl-lib.cpp"
    break;

  case 281: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1960 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7740 "bison-chpl-lib.cpp"
    break;

  case 282: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1967 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7748 "bison-chpl-lib.cpp"
    break;

  case 283: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1971 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7756 "bison-chpl-lib.cpp"
    break;

  case 284: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1975 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7764 "bison-chpl-lib.cpp"
    break;

  case 285: /* try_stmt: TTRY tryable_stmt  */
#line 1982 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7772 "bison-chpl-lib.cpp"
    break;

  case 286: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1986 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7780 "bison-chpl-lib.cpp"
    break;

  case 287: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1990 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7788 "bison-chpl-lib.cpp"
    break;

  case 288: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1994 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7796 "bison-chpl-lib.cpp"
    break;

  case 289: /* catch_expr_ls: %empty  */
#line 2000 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7802 "bison-chpl-lib.cpp"
    break;

  case 290: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2001 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7808 "bison-chpl-lib.cpp"
    break;

  case 291: /* catch_expr: TCATCH block_stmt  */
#line 2006 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 7816 "bison-chpl-lib.cpp"
    break;

  case 292: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2010 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 7824 "bison-chpl-lib.cpp"
    break;

  case 293: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2014 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 7832 "bison-chpl-lib.cpp"
    break;

  case 294: /* catch_expr_inner: ident_def  */
#line 2021 "chpl.ypp"
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
#line 7849 "bison-chpl-lib.cpp"
    break;

  case 295: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2034 "chpl.ypp"
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
#line 7866 "bison-chpl-lib.cpp"
    break;

  case 296: /* throw_stmt: TTHROW expr TSEMI  */
#line 2050 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7876 "bison-chpl-lib.cpp"
    break;

  case 297: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2058 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), context->makeSpannedLocation((yylsp[-4]), (yylsp[-3])), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7885 "bison-chpl-lib.cpp"
    break;

  case 298: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2063 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7896 "bison-chpl-lib.cpp"
    break;

  case 299: /* when_stmt_ls: %empty  */
#line 2072 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 7902 "bison-chpl-lib.cpp"
    break;

  case 300: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2073 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7908 "bison-chpl-lib.cpp"
    break;

  case 301: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2078 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), context->makeSpannedLocation((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 7916 "bison-chpl-lib.cpp"
    break;

  case 302: /* when_stmt: TOTHERWISE stmt  */
#line 2082 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-1]), nullptr, blockOrDo);
  }
#line 7925 "bison-chpl-lib.cpp"
    break;

  case 303: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2087 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-3]), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 7935 "bison-chpl-lib.cpp"
    break;

  case 304: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2096 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 7944 "bison-chpl-lib.cpp"
    break;

  case 305: /* manager_expr: expr TAS ident_def  */
#line 2101 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 7952 "bison-chpl-lib.cpp"
    break;

  case 306: /* manager_expr: expr  */
#line 2105 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 7960 "bison-chpl-lib.cpp"
    break;

  case 307: /* manager_expr_ls: manager_expr  */
#line 2111 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7966 "bison-chpl-lib.cpp"
    break;

  case 308: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2112 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7972 "bison-chpl-lib.cpp"
    break;

  case 309: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2117 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), locHeader, (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 7981 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2127 "chpl.ypp"
    {
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 7991 "bison-chpl-lib.cpp"
    break;

  case 311: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2133 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8003 "bison-chpl-lib.cpp"
    break;

  case 312: /* class_start: class_tag ident_def  */
#line 2145 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8011 "bison-chpl-lib.cpp"
    break;

  case 313: /* class_tag: TCLASS  */
#line 2151 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8017 "bison-chpl-lib.cpp"
    break;

  case 314: /* class_tag: TRECORD  */
#line 2152 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8023 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_tag: TUNION  */
#line 2153 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8029 "bison-chpl-lib.cpp"
    break;

  case 316: /* opt_inherit: %empty  */
#line 2157 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8035 "bison-chpl-lib.cpp"
    break;

  case 317: /* opt_inherit: TCOLON expr_ls  */
#line 2158 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8041 "bison-chpl-lib.cpp"
    break;

  case 318: /* class_level_stmt_ls: %empty  */
#line 2162 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8050 "bison-chpl-lib.cpp"
    break;

  case 319: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2167 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8058 "bison-chpl-lib.cpp"
    break;

  case 320: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2171 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8066 "bison-chpl-lib.cpp"
    break;

  case 321: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2175 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8074 "bison-chpl-lib.cpp"
    break;

  case 322: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2179 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8082 "bison-chpl-lib.cpp"
    break;

  case 323: /* enum_decl_stmt: enum_header TLCBR enum_ls TRCBR  */
#line 2187 "chpl.ypp"
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
#line 8105 "bison-chpl-lib.cpp"
    break;

  case 324: /* enum_decl_stmt: enum_header TLCBR error TRCBR  */
#line 2206 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-3].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8119 "bison-chpl-lib.cpp"
    break;

  case 325: /* enum_header: TENUM ident_def  */
#line 2219 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Enum);
  }
#line 8127 "bison-chpl-lib.cpp"
    break;

  case 326: /* enum_ls: enum_item  */
#line 2226 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8136 "bison-chpl-lib.cpp"
    break;

  case 327: /* enum_ls: enum_ls TCOMMA  */
#line 2231 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8146 "bison-chpl-lib.cpp"
    break;

  case 328: /* $@5: %empty  */
#line 2237 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8155 "bison-chpl-lib.cpp"
    break;

  case 329: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2242 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8164 "bison-chpl-lib.cpp"
    break;

  case 330: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2247 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8173 "bison-chpl-lib.cpp"
    break;

  case 331: /* $@6: %empty  */
#line 2252 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8182 "bison-chpl-lib.cpp"
    break;

  case 332: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2257 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8191 "bison-chpl-lib.cpp"
    break;

  case 333: /* enum_item: ident_def  */
#line 2267 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8202 "bison-chpl-lib.cpp"
    break;

  case 334: /* enum_item: ident_def TASSIGN expr  */
#line 2274 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8215 "bison-chpl-lib.cpp"
    break;

  case 335: /* lambda_decl_start: TLAMBDA  */
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
#line 8231 "bison-chpl-lib.cpp"
    break;

  case 336: /* $@7: %empty  */
#line 2303 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8241 "bison-chpl-lib.cpp"
    break;

  case 337: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
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
#line 8263 "bison-chpl-lib.cpp"
    break;

  case 339: /* linkage_spec: linkage_spec_empty  */
#line 2332 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8269 "bison-chpl-lib.cpp"
    break;

  case 340: /* linkage_spec: TINLINE  */
#line 2333 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8276 "bison-chpl-lib.cpp"
    break;

  case 341: /* linkage_spec: TOVERRIDE  */
#line 2335 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8283 "bison-chpl-lib.cpp"
    break;

  case 342: /* opt_fn_type_formal_ls: %empty  */
#line 2340 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8289 "bison-chpl-lib.cpp"
    break;

  case 343: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2341 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8295 "bison-chpl-lib.cpp"
    break;

  case 344: /* fn_type_formal_ls: fn_type_formal  */
#line 2345 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8301 "bison-chpl-lib.cpp"
    break;

  case 345: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2346 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8307 "bison-chpl-lib.cpp"
    break;

  case 346: /* fn_type_formal: named_formal  */
#line 2351 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8313 "bison-chpl-lib.cpp"
    break;

  case 347: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2354 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8319 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_type_formal: formal_type  */
#line 2356 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8325 "bison-chpl-lib.cpp"
    break;

  case 349: /* opt_fn_type_ret_type: %empty  */
#line 2360 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8331 "bison-chpl-lib.cpp"
    break;

  case 350: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2361 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8337 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
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
#line 8356 "bison-chpl-lib.cpp"
    break;

  case 352: /* $@8: %empty  */
#line 2392 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8366 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2398 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8379 "bison-chpl-lib.cpp"
    break;

  case 354: /* $@9: %empty  */
#line 2407 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8389 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2413 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8402 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2425 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8410 "bison-chpl-lib.cpp"
    break;

  case 357: /* $@10: %empty  */
#line 2434 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8418 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2438 "chpl.ypp"
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
#line 8440 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2459 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8452 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2467 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8464 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
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
#line 8479 "bison-chpl-lib.cpp"
    break;

  case 362: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
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
#line 8494 "bison-chpl-lib.cpp"
    break;

  case 363: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2497 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8504 "bison-chpl-lib.cpp"
    break;

  case 364: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
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
#line 8520 "bison-chpl-lib.cpp"
    break;

  case 366: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2521 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8526 "bison-chpl-lib.cpp"
    break;

  case 369: /* fn_ident: ident_def TBANG  */
#line 2528 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8536 "bison-chpl-lib.cpp"
    break;

  case 409: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2585 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8542 "bison-chpl-lib.cpp"
    break;

  case 410: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2586 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8548 "bison-chpl-lib.cpp"
    break;

  case 411: /* opt_formal_ls: %empty  */
#line 2590 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8554 "bison-chpl-lib.cpp"
    break;

  case 412: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2591 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8560 "bison-chpl-lib.cpp"
    break;

  case 413: /* req_formal_ls: TLP TRP  */
#line 2595 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8566 "bison-chpl-lib.cpp"
    break;

  case 414: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2596 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8572 "bison-chpl-lib.cpp"
    break;

  case 415: /* formal_ls_inner: formal  */
#line 2600 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8578 "bison-chpl-lib.cpp"
    break;

  case 416: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2601 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8584 "bison-chpl-lib.cpp"
    break;

  case 417: /* formal_ls: %empty  */
#line 2605 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8590 "bison-chpl-lib.cpp"
    break;

  case 418: /* formal_ls: formal_ls_inner  */
#line 2606 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8596 "bison-chpl-lib.cpp"
    break;

  case 420: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2615 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8604 "bison-chpl-lib.cpp"
    break;

  case 421: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2620 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8612 "bison-chpl-lib.cpp"
    break;

  case 422: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2625 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8620 "bison-chpl-lib.cpp"
    break;

  case 423: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2630 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8628 "bison-chpl-lib.cpp"
    break;

  case 424: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2635 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8636 "bison-chpl-lib.cpp"
    break;

  case 425: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2640 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8644 "bison-chpl-lib.cpp"
    break;

  case 426: /* opt_formal_intent_tag: %empty  */
#line 2646 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 8653 "bison-chpl-lib.cpp"
    break;

  case 427: /* opt_formal_intent_tag: required_intent_tag  */
#line 2651 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8662 "bison-chpl-lib.cpp"
    break;

  case 428: /* required_intent_tag: TIN  */
#line 2658 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 8668 "bison-chpl-lib.cpp"
    break;

  case 429: /* required_intent_tag: TINOUT  */
#line 2659 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 8674 "bison-chpl-lib.cpp"
    break;

  case 430: /* required_intent_tag: TOUT  */
#line 2660 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 8680 "bison-chpl-lib.cpp"
    break;

  case 431: /* required_intent_tag: TCONST TIN  */
#line 2661 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 8686 "bison-chpl-lib.cpp"
    break;

  case 432: /* required_intent_tag: TCONST TREF  */
#line 2662 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 8692 "bison-chpl-lib.cpp"
    break;

  case 433: /* required_intent_tag: TCONST  */
#line 2663 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 8698 "bison-chpl-lib.cpp"
    break;

  case 434: /* required_intent_tag: TPARAM  */
#line 2664 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 8704 "bison-chpl-lib.cpp"
    break;

  case 435: /* required_intent_tag: TREF  */
#line 2665 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 8710 "bison-chpl-lib.cpp"
    break;

  case 436: /* required_intent_tag: TTYPE  */
#line 2666 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 8716 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_this_intent_tag: %empty  */
#line 2670 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 8722 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_this_intent_tag: TPARAM  */
#line 2671 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 8728 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_this_intent_tag: TREF  */
#line 2672 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 8734 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: TCONST TREF  */
#line 2673 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 8740 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_this_intent_tag: TCONST  */
#line 2674 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 8746 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_this_intent_tag: TTYPE  */
#line 2675 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 8752 "bison-chpl-lib.cpp"
    break;

  case 443: /* proc_iter_or_op: TPROC  */
#line 2679 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8758 "bison-chpl-lib.cpp"
    break;

  case 444: /* proc_iter_or_op: TITER  */
#line 2680 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8764 "bison-chpl-lib.cpp"
    break;

  case 445: /* proc_iter_or_op: TOPERATOR  */
#line 2681 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8770 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_ret_tag: %empty  */
#line 2685 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 8776 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_ret_tag: TOUT  */
#line 2686 "chpl.ypp"
                    { (yyval.returnTag) = Function::OUT; }
#line 8782 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_ret_tag: TCONST  */
#line 2687 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 8788 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_ret_tag: TCONST TREF  */
#line 2688 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 8794 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_ret_tag: TREF  */
#line 2689 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 8800 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_ret_tag: TPARAM  */
#line 2690 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 8806 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: TTYPE  */
#line 2691 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 8812 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_throws_error: %empty  */
#line 2695 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8818 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_throws_error: TTHROWS  */
#line 2696 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8824 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_function_body_stmt: TSEMI  */
#line 2699 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8830 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_function_body_stmt: function_body_stmt  */
#line 2700 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8836 "bison-chpl-lib.cpp"
    break;

  case 457: /* function_body_stmt: block_stmt_body  */
#line 2704 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8842 "bison-chpl-lib.cpp"
    break;

  case 458: /* function_body_stmt: TDO toplevel_stmt  */
#line 2705 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 8848 "bison-chpl-lib.cpp"
    break;

  case 459: /* query_expr: TQUERIEDIDENT  */
#line 2709 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 8854 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_lifetime_where: %empty  */
#line 2714 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 8860 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_lifetime_where: TWHERE expr  */
#line 2716 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 8866 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2718 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 8872 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2720 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 8878 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2722 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 8884 "bison-chpl-lib.cpp"
    break;

  case 465: /* lifetime_components_expr: lifetime_expr  */
#line 2727 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8890 "bison-chpl-lib.cpp"
    break;

  case 466: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2729 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8896 "bison-chpl-lib.cpp"
    break;

  case 467: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2734 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8902 "bison-chpl-lib.cpp"
    break;

  case 468: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2736 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8908 "bison-chpl-lib.cpp"
    break;

  case 469: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2738 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8914 "bison-chpl-lib.cpp"
    break;

  case 470: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2740 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8920 "bison-chpl-lib.cpp"
    break;

  case 471: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2742 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8926 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2744 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8932 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2746 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 8938 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_ident: TIDENT  */
#line 2750 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8944 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_ident: TINIT  */
#line 2751 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8950 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_ident: TTHIS  */
#line 2752 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8956 "bison-chpl-lib.cpp"
    break;

  case 477: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2757 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 8965 "bison-chpl-lib.cpp"
    break;

  case 478: /* type_alias_decl_stmt_start: TTYPE  */
#line 2766 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 8973 "bison-chpl-lib.cpp"
    break;

  case 479: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2770 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 8982 "bison-chpl-lib.cpp"
    break;

  case 480: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2778 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 8990 "bison-chpl-lib.cpp"
    break;

  case 481: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2782 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8998 "bison-chpl-lib.cpp"
    break;

  case 482: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2789 "chpl.ypp"
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
    BUILDER->noteDeclNameLocation(node.get(), LOC((yylsp[-1])));
    // Gather the comments like a statement does.
    (yyval.commentsAndStmt) = STMT((yylsp[-1]), node.release());
    context->clearComments();
  }
#line 9021 "bison-chpl-lib.cpp"
    break;

  case 483: /* opt_init_type: %empty  */
#line 2810 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9027 "bison-chpl-lib.cpp"
    break;

  case 484: /* opt_init_type: TASSIGN expr  */
#line 2812 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9033 "bison-chpl-lib.cpp"
    break;

  case 485: /* var_decl_type: TPARAM  */
#line 2816 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9039 "bison-chpl-lib.cpp"
    break;

  case 486: /* var_decl_type: TCONST TREF  */
#line 2817 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9045 "bison-chpl-lib.cpp"
    break;

  case 487: /* var_decl_type: TREF  */
#line 2818 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9051 "bison-chpl-lib.cpp"
    break;

  case 488: /* var_decl_type: TCONST  */
#line 2819 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9057 "bison-chpl-lib.cpp"
    break;

  case 489: /* var_decl_type: TVAR  */
#line 2820 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9063 "bison-chpl-lib.cpp"
    break;

  case 490: /* $@11: %empty  */
#line 2825 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9072 "bison-chpl-lib.cpp"
    break;

  case 491: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2829 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9081 "bison-chpl-lib.cpp"
    break;

  case 492: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2834 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9090 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_stmt: var_decl_type error TSEMI  */
#line 2839 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = { .comments=nullptr, .stmt=ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release() };
    context->resetDeclStateOnError();
  }
#line 9099 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2847 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9107 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2851 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9115 "bison-chpl-lib.cpp"
    break;

  case 496: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
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
      BUILDER->noteDeclNameLocation(varDecl.get(), LOC((yylsp[-2])));
      // gather the comments like a statement does
      (yyval.commentsAndStmt) = STMT((yylsp[-2]), varDecl.release());
      context->clearComments();
    }
#line 9138 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2877 "chpl.ypp"
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
#line 9158 "bison-chpl-lib.cpp"
    break;

  case 498: /* tuple_var_decl_component: ident_def  */
#line 2896 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9166 "bison-chpl-lib.cpp"
    break;

  case 499: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2900 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9174 "bison-chpl-lib.cpp"
    break;

  case 500: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2907 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9180 "bison-chpl-lib.cpp"
    break;

  case 501: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2909 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9186 "bison-chpl-lib.cpp"
    break;

  case 502: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2911 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9192 "bison-chpl-lib.cpp"
    break;

  case 503: /* opt_init_expr: %empty  */
#line 2917 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9198 "bison-chpl-lib.cpp"
    break;

  case 504: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2918 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9204 "bison-chpl-lib.cpp"
    break;

  case 505: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2919 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9210 "bison-chpl-lib.cpp"
    break;

  case 506: /* formal_or_ret_type_expr: expr  */
#line 2923 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9216 "bison-chpl-lib.cpp"
    break;

  case 507: /* ret_type: formal_or_ret_type_expr  */
#line 2927 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9222 "bison-chpl-lib.cpp"
    break;

  case 508: /* ret_type: reserved_type_ident_use  */
#line 2928 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9228 "bison-chpl-lib.cpp"
    break;

  case 509: /* ret_type: error  */
#line 2929 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9234 "bison-chpl-lib.cpp"
    break;

  case 510: /* colon_ret_type: TCOLON ret_type  */
#line 2933 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9240 "bison-chpl-lib.cpp"
    break;

  case 511: /* colon_ret_type: error  */
#line 2934 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9246 "bison-chpl-lib.cpp"
    break;

  case 512: /* opt_ret_type: %empty  */
#line 2938 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9252 "bison-chpl-lib.cpp"
    break;

  case 514: /* opt_type: %empty  */
#line 2943 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9258 "bison-chpl-lib.cpp"
    break;

  case 515: /* opt_type: TCOLON expr  */
#line 2944 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9264 "bison-chpl-lib.cpp"
    break;

  case 516: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2945 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9270 "bison-chpl-lib.cpp"
    break;

  case 517: /* opt_type: error  */
#line 2946 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9276 "bison-chpl-lib.cpp"
    break;

  case 518: /* formal_type: formal_or_ret_type_expr  */
#line 2950 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9282 "bison-chpl-lib.cpp"
    break;

  case 519: /* formal_type: reserved_type_ident_use  */
#line 2951 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9288 "bison-chpl-lib.cpp"
    break;

  case 520: /* colon_formal_type: TCOLON formal_type  */
#line 2955 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9294 "bison-chpl-lib.cpp"
    break;

  case 521: /* opt_colon_formal_type: %empty  */
#line 2959 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9300 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_colon_formal_type: colon_formal_type  */
#line 2960 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9306 "bison-chpl-lib.cpp"
    break;

  case 523: /* expr_ls: expr  */
#line 2966 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9312 "bison-chpl-lib.cpp"
    break;

  case 524: /* expr_ls: expr_ls TCOMMA expr  */
#line 2967 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9318 "bison-chpl-lib.cpp"
    break;

  case 525: /* tuple_component: opt_try_expr  */
#line 2971 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9324 "bison-chpl-lib.cpp"
    break;

  case 526: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2976 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9332 "bison-chpl-lib.cpp"
    break;

  case 527: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2980 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9340 "bison-chpl-lib.cpp"
    break;

  case 528: /* opt_actual_ls: %empty  */
#line 2986 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9346 "bison-chpl-lib.cpp"
    break;

  case 529: /* opt_actual_ls: actual_ls  */
#line 2987 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9352 "bison-chpl-lib.cpp"
    break;

  case 530: /* actual_ls: actual_expr  */
#line 2992 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9361 "bison-chpl-lib.cpp"
    break;

  case 531: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2997 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9371 "bison-chpl-lib.cpp"
    break;

  case 532: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3005 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9377 "bison-chpl-lib.cpp"
    break;

  case 533: /* actual_expr: opt_try_expr  */
#line 3006 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9383 "bison-chpl-lib.cpp"
    break;

  case 534: /* ident_expr: ident_use  */
#line 3010 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9389 "bison-chpl-lib.cpp"
    break;

  case 535: /* ident_expr: scalar_type  */
#line 3011 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9395 "bison-chpl-lib.cpp"
    break;

  case 541: /* sub_type_level_expr: TSINGLE expr  */
#line 3024 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9401 "bison-chpl-lib.cpp"
    break;

  case 542: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3026 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9407 "bison-chpl-lib.cpp"
    break;

  case 543: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3028 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9413 "bison-chpl-lib.cpp"
    break;

  case 544: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3030 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9419 "bison-chpl-lib.cpp"
    break;

  case 545: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 3032 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9429 "bison-chpl-lib.cpp"
    break;

  case 546: /* sub_type_level_expr: TATOMIC expr  */
#line 3038 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9435 "bison-chpl-lib.cpp"
    break;

  case 547: /* sub_type_level_expr: TSYNC expr  */
#line 3040 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9441 "bison-chpl-lib.cpp"
    break;

  case 548: /* sub_type_level_expr: TOWNED  */
#line 3043 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9447 "bison-chpl-lib.cpp"
    break;

  case 549: /* sub_type_level_expr: TOWNED expr  */
#line 3045 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9453 "bison-chpl-lib.cpp"
    break;

  case 550: /* sub_type_level_expr: TUNMANAGED  */
#line 3047 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9459 "bison-chpl-lib.cpp"
    break;

  case 551: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3049 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9465 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TSHARED  */
#line 3051 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9471 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TSHARED expr  */
#line 3053 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9477 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TBORROWED  */
#line 3055 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9483 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TBORROWED expr  */
#line 3057 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9489 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TCLASS  */
#line 3059 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9495 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TRECORD  */
#line 3061 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9501 "bison-chpl-lib.cpp"
    break;

  case 558: /* for_expr: no_loop_attributes for_expr_base  */
#line 3065 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9507 "bison-chpl-lib.cpp"
    break;

  case 559: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3070 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9515 "bison-chpl-lib.cpp"
    break;

  case 560: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3074 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9523 "bison-chpl-lib.cpp"
    break;

  case 561: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3078 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-2]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9531 "bison-chpl-lib.cpp"
    break;

  case 562: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3082 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9544 "bison-chpl-lib.cpp"
    break;

  case 563: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3091 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9557 "bison-chpl-lib.cpp"
    break;

  case 564: /* for_expr_base: for_loop_kw expr TDO TIF expr TTHEN expr  */
#line 3100 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-5]), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9570 "bison-chpl-lib.cpp"
    break;

  case 565: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3111 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9576 "bison-chpl-lib.cpp"
    break;

  case 566: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3116 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9584 "bison-chpl-lib.cpp"
    break;

  case 567: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3120 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9592 "bison-chpl-lib.cpp"
    break;

  case 568: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3124 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9600 "bison-chpl-lib.cpp"
    break;

  case 569: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3128 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9608 "bison-chpl-lib.cpp"
    break;

  case 570: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3132 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9616 "bison-chpl-lib.cpp"
    break;

  case 571: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3136 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9624 "bison-chpl-lib.cpp"
    break;

  case 572: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3140 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9632 "bison-chpl-lib.cpp"
    break;

  case 573: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3147 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9646 "bison-chpl-lib.cpp"
    break;

  case 574: /* nil_expr: TNIL  */
#line 3164 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9652 "bison-chpl-lib.cpp"
    break;

  case 582: /* opt_task_intent_ls: %empty  */
#line 3182 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9658 "bison-chpl-lib.cpp"
    break;

  case 583: /* opt_task_intent_ls: task_intent_clause  */
#line 3183 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9664 "bison-chpl-lib.cpp"
    break;

  case 584: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3188 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9672 "bison-chpl-lib.cpp"
    break;

  case 585: /* task_intent_ls: intent_expr  */
#line 3194 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9678 "bison-chpl-lib.cpp"
    break;

  case 586: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3195 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9684 "bison-chpl-lib.cpp"
    break;

  case 587: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3200 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 9694 "bison-chpl-lib.cpp"
    break;

  case 588: /* forall_intent_ls: intent_expr  */
#line 3208 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9700 "bison-chpl-lib.cpp"
    break;

  case 589: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3209 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9706 "bison-chpl-lib.cpp"
    break;

  case 590: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3214 "chpl.ypp"
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
#line 9725 "bison-chpl-lib.cpp"
    break;

  case 591: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3229 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9733 "bison-chpl-lib.cpp"
    break;

  case 592: /* intent_expr: expr TREDUCE ident_expr  */
#line 3233 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9741 "bison-chpl-lib.cpp"
    break;

  case 593: /* task_var_prefix: TCONST  */
#line 3239 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 9747 "bison-chpl-lib.cpp"
    break;

  case 594: /* task_var_prefix: TIN  */
#line 3240 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 9753 "bison-chpl-lib.cpp"
    break;

  case 595: /* task_var_prefix: TCONST TIN  */
#line 3241 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 9759 "bison-chpl-lib.cpp"
    break;

  case 596: /* task_var_prefix: TREF  */
#line 3242 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 9765 "bison-chpl-lib.cpp"
    break;

  case 597: /* task_var_prefix: TCONST TREF  */
#line 3243 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 9771 "bison-chpl-lib.cpp"
    break;

  case 598: /* task_var_prefix: TVAR  */
#line 3244 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 9777 "bison-chpl-lib.cpp"
    break;

  case 599: /* new_maybe_decorated: TNEW  */
#line 3249 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 9783 "bison-chpl-lib.cpp"
    break;

  case 600: /* new_maybe_decorated: TNEW TOWNED  */
#line 3251 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 9789 "bison-chpl-lib.cpp"
    break;

  case 601: /* new_maybe_decorated: TNEW TSHARED  */
#line 3253 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 9795 "bison-chpl-lib.cpp"
    break;

  case 602: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3255 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 9801 "bison-chpl-lib.cpp"
    break;

  case 603: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3257 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 9807 "bison-chpl-lib.cpp"
    break;

  case 604: /* new_expr: new_maybe_decorated expr  */
#line 3263 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 9815 "bison-chpl-lib.cpp"
    break;

  case 605: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3270 "chpl.ypp"
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
#line 9831 "bison-chpl-lib.cpp"
    break;

  case 606: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3282 "chpl.ypp"
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
#line 9847 "bison-chpl-lib.cpp"
    break;

  case 607: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3294 "chpl.ypp"
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
#line 9864 "bison-chpl-lib.cpp"
    break;

  case 608: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3307 "chpl.ypp"
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
#line 9882 "bison-chpl-lib.cpp"
    break;

  case 609: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3324 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9890 "bison-chpl-lib.cpp"
    break;

  case 610: /* range_literal_expr: expr TDOTDOT expr  */
#line 3331 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 9899 "bison-chpl-lib.cpp"
    break;

  case 611: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3336 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 9908 "bison-chpl-lib.cpp"
    break;

  case 612: /* range_literal_expr: expr TDOTDOT  */
#line 3341 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 9917 "bison-chpl-lib.cpp"
    break;

  case 613: /* range_literal_expr: TDOTDOT expr  */
#line 3346 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 9927 "bison-chpl-lib.cpp"
    break;

  case 614: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3352 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 9937 "bison-chpl-lib.cpp"
    break;

  case 615: /* range_literal_expr: TDOTDOT  */
#line 3358 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 9947 "bison-chpl-lib.cpp"
    break;

  case 616: /* cast_expr: expr TCOLON expr  */
#line 3388 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 9955 "bison-chpl-lib.cpp"
    break;

  case 617: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3395 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 9963 "bison-chpl-lib.cpp"
    break;

  case 618: /* super_expr: fn_expr  */
#line 3401 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 9969 "bison-chpl-lib.cpp"
    break;

  case 622: /* expr: sub_type_level_expr TQUESTION  */
#line 3410 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 9975 "bison-chpl-lib.cpp"
    break;

  case 623: /* expr: TQUESTION  */
#line 3412 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 9981 "bison-chpl-lib.cpp"
    break;

  case 627: /* expr: fn_type  */
#line 3417 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 9987 "bison-chpl-lib.cpp"
    break;

  case 637: /* opt_expr: %empty  */
#line 3431 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 9993 "bison-chpl-lib.cpp"
    break;

  case 638: /* opt_expr: expr  */
#line 3432 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9999 "bison-chpl-lib.cpp"
    break;

  case 639: /* opt_try_expr: TTRY expr  */
#line 3436 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10005 "bison-chpl-lib.cpp"
    break;

  case 640: /* opt_try_expr: TTRYBANG expr  */
#line 3437 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10011 "bison-chpl-lib.cpp"
    break;

  case 641: /* opt_try_expr: super_expr  */
#line 3438 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10017 "bison-chpl-lib.cpp"
    break;

  case 647: /* call_base_expr: expr TBANG  */
#line 3455 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10025 "bison-chpl-lib.cpp"
    break;

  case 648: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3458 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10031 "bison-chpl-lib.cpp"
    break;

  case 651: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3465 "chpl.ypp"
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
#line 10047 "bison-chpl-lib.cpp"
    break;

  case 652: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3477 "chpl.ypp"
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
#line 10063 "bison-chpl-lib.cpp"
    break;

  case 653: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3489 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10071 "bison-chpl-lib.cpp"
    break;

  case 654: /* dot_expr: expr TDOT ident_use  */
#line 3496 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10077 "bison-chpl-lib.cpp"
    break;

  case 655: /* dot_expr: expr TDOT TTYPE  */
#line 3498 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10083 "bison-chpl-lib.cpp"
    break;

  case 656: /* dot_expr: expr TDOT TDOMAIN  */
#line 3500 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10089 "bison-chpl-lib.cpp"
    break;

  case 657: /* dot_expr: expr TDOT TLOCALE  */
#line 3502 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10095 "bison-chpl-lib.cpp"
    break;

  case 658: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3504 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10101 "bison-chpl-lib.cpp"
    break;

  case 659: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3506 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10107 "bison-chpl-lib.cpp"
    break;

  case 660: /* parenthesized_expr: TLP opt_try_expr TRP  */
#line 3515 "chpl.ypp"
  {
    // Use 'tryNote' here in case an expression like ((x)) comes along.
    BUILDER->tryNoteExprParenthLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 10117 "bison-chpl-lib.cpp"
    break;

  case 661: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3521 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10125 "bison-chpl-lib.cpp"
    break;

  case 662: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3525 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10133 "bison-chpl-lib.cpp"
    break;

  case 663: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3529 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10141 "bison-chpl-lib.cpp"
    break;

  case 664: /* bool_literal: TFALSE  */
#line 3535 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10147 "bison-chpl-lib.cpp"
    break;

  case 665: /* bool_literal: TTRUE  */
#line 3536 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10153 "bison-chpl-lib.cpp"
    break;

  case 666: /* str_bytes_literal: STRINGLITERAL  */
#line 3540 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10159 "bison-chpl-lib.cpp"
    break;

  case 667: /* str_bytes_literal: BYTESLITERAL  */
#line 3541 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10165 "bison-chpl-lib.cpp"
    break;

  case 670: /* literal_expr: INTLITERAL  */
#line 3547 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10171 "bison-chpl-lib.cpp"
    break;

  case 671: /* literal_expr: REALLITERAL  */
#line 3548 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10177 "bison-chpl-lib.cpp"
    break;

  case 672: /* literal_expr: IMAGLITERAL  */
#line 3549 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10183 "bison-chpl-lib.cpp"
    break;

  case 673: /* literal_expr: CSTRINGLITERAL  */
#line 3550 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10189 "bison-chpl-lib.cpp"
    break;

  case 674: /* literal_expr: TNONE  */
#line 3551 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10195 "bison-chpl-lib.cpp"
    break;

  case 675: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3553 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10204 "bison-chpl-lib.cpp"
    break;

  case 676: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3558 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10213 "bison-chpl-lib.cpp"
    break;

  case 677: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3563 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10222 "bison-chpl-lib.cpp"
    break;

  case 678: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3568 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10231 "bison-chpl-lib.cpp"
    break;

  case 679: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3573 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10240 "bison-chpl-lib.cpp"
    break;

  case 680: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3578 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10249 "bison-chpl-lib.cpp"
    break;

  case 681: /* assoc_expr_ls: expr TALIAS expr  */
#line 3587 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10258 "bison-chpl-lib.cpp"
    break;

  case 682: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3592 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10268 "bison-chpl-lib.cpp"
    break;

  case 683: /* binary_op_expr: expr TPLUS expr  */
#line 3600 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10274 "bison-chpl-lib.cpp"
    break;

  case 684: /* binary_op_expr: expr TMINUS expr  */
#line 3601 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10280 "bison-chpl-lib.cpp"
    break;

  case 685: /* binary_op_expr: expr TSTAR expr  */
#line 3602 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10286 "bison-chpl-lib.cpp"
    break;

  case 686: /* binary_op_expr: expr TDIVIDE expr  */
#line 3603 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10292 "bison-chpl-lib.cpp"
    break;

  case 687: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3604 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10298 "bison-chpl-lib.cpp"
    break;

  case 688: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3605 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10304 "bison-chpl-lib.cpp"
    break;

  case 689: /* binary_op_expr: expr TMOD expr  */
#line 3606 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10310 "bison-chpl-lib.cpp"
    break;

  case 690: /* binary_op_expr: expr TEQUAL expr  */
#line 3607 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10316 "bison-chpl-lib.cpp"
    break;

  case 691: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3608 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10322 "bison-chpl-lib.cpp"
    break;

  case 692: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3609 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10328 "bison-chpl-lib.cpp"
    break;

  case 693: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3610 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10334 "bison-chpl-lib.cpp"
    break;

  case 694: /* binary_op_expr: expr TLESS expr  */
#line 3611 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10340 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr TGREATER expr  */
#line 3612 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10346 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TBAND expr  */
#line 3613 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10352 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TBOR expr  */
#line 3614 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10358 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TBXOR expr  */
#line 3615 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10364 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TAND expr  */
#line 3616 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10370 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TOR expr  */
#line 3617 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10376 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TEXP expr  */
#line 3618 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10382 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TBY expr  */
#line 3619 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10388 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TALIGN expr  */
#line 3620 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10394 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr THASH expr  */
#line 3621 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10400 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TDMAPPED expr  */
#line 3622 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10406 "bison-chpl-lib.cpp"
    break;

  case 706: /* unary_op_expr: TPLUS expr  */
#line 3626 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10412 "bison-chpl-lib.cpp"
    break;

  case 707: /* unary_op_expr: TMINUS expr  */
#line 3627 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10418 "bison-chpl-lib.cpp"
    break;

  case 708: /* unary_op_expr: TMINUSMINUS expr  */
#line 3628 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10424 "bison-chpl-lib.cpp"
    break;

  case 709: /* unary_op_expr: TPLUSPLUS expr  */
#line 3629 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10430 "bison-chpl-lib.cpp"
    break;

  case 710: /* unary_op_expr: TBANG expr  */
#line 3630 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10436 "bison-chpl-lib.cpp"
    break;

  case 711: /* unary_op_expr: expr TBANG  */
#line 3631 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10444 "bison-chpl-lib.cpp"
    break;

  case 712: /* unary_op_expr: TBNOT expr  */
#line 3634 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10450 "bison-chpl-lib.cpp"
    break;

  case 713: /* reduce_expr: expr TREDUCE expr  */
#line 3639 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10458 "bison-chpl-lib.cpp"
    break;

  case 714: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3643 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10466 "bison-chpl-lib.cpp"
    break;

  case 715: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3647 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10474 "bison-chpl-lib.cpp"
    break;

  case 716: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3651 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10482 "bison-chpl-lib.cpp"
    break;

  case 717: /* scan_expr: expr TSCAN expr  */
#line 3658 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10490 "bison-chpl-lib.cpp"
    break;

  case 718: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3662 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10498 "bison-chpl-lib.cpp"
    break;

  case 719: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3666 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10506 "bison-chpl-lib.cpp"
    break;

  case 720: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3670 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10514 "bison-chpl-lib.cpp"
    break;


#line 10518 "bison-chpl-lib.cpp"

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
