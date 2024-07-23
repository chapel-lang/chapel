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
  YYSYMBOL_327_12 = 327,                   /* $@12  */
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
#line 345 "chpl.ypp"

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
#define YYLAST   17822

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  182
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  203
/* YYNRULES -- Number of rules.  */
#define YYNRULES  733
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1261

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
       0,   654,   654,   658,   659,   664,   665,   673,   677,   687,
     692,   699,   700,   701,   702,   703,   704,   705,   706,   707,
     708,   709,   727,   728,   729,   737,   746,   755,   759,   766,
     774,   775,   776,   777,   778,   779,   780,   781,   783,   803,
     811,   828,   844,   861,   878,   894,   915,   916,   920,   930,
     931,   935,   939,   940,   944,   951,   959,   962,   972,   974,
     979,   985,   990,   999,  1003,  1024,  1041,  1058,  1076,  1077,
    1079,  1084,  1085,  1090,  1108,  1113,  1118,  1130,  1149,  1150,
    1154,  1158,  1162,  1167,  1171,  1175,  1183,  1188,  1194,  1199,
    1208,  1209,  1213,  1218,  1226,  1235,  1243,  1254,  1262,  1263,
    1264,  1265,  1266,  1267,  1268,  1269,  1270,  1271,  1272,  1273,
    1274,  1275,  1276,  1277,  1278,  1290,  1294,  1299,  1304,  1310,
    1319,  1320,  1324,  1333,  1337,  1341,  1345,  1349,  1353,  1362,
    1363,  1364,  1368,  1369,  1370,  1371,  1372,  1376,  1377,  1378,
    1390,  1391,  1392,  1397,  1398,  1399,  1400,  1401,  1402,  1403,
    1404,  1405,  1406,  1407,  1408,  1409,  1410,  1411,  1412,  1413,
    1414,  1415,  1419,  1420,  1421,  1422,  1423,  1424,  1425,  1426,
    1427,  1428,  1429,  1430,  1437,  1438,  1439,  1440,  1444,  1445,
    1449,  1456,  1466,  1470,  1476,  1476,  1482,  1482,  1491,  1492,
    1493,  1494,  1495,  1496,  1497,  1501,  1506,  1511,  1516,  1523,
    1531,  1532,  1536,  1545,  1552,  1561,  1580,  1579,  1589,  1588,
    1601,  1607,  1610,  1611,  1620,  1621,  1622,  1626,  1649,  1668,
    1687,  1692,  1697,  1702,  1727,  1732,  1737,  1742,  1747,  1752,
    1757,  1762,  1767,  1772,  1777,  1782,  1787,  1792,  1797,  1803,
    1812,  1821,  1825,  1829,  1833,  1837,  1841,  1845,  1849,  1853,
    1858,  1863,  1868,  1876,  1892,  1911,  1915,  1922,  1923,  1928,
    1933,  1934,  1935,  1936,  1937,  1938,  1939,  1940,  1941,  1942,
    1943,  1944,  1949,  1954,  1955,  1956,  1957,  1966,  1967,  1971,
    1975,  1979,  1986,  1990,  1994,  2001,  2005,  2009,  2013,  2020,
    2021,  2025,  2029,  2033,  2040,  2054,  2071,  2079,  2084,  2094,
    2095,  2099,  2103,  2108,  2117,  2122,  2126,  2133,  2134,  2138,
    2148,  2154,  2166,  2173,  2174,  2175,  2179,  2180,  2184,  2188,
    2192,  2196,  2200,  2208,  2227,  2240,  2247,  2252,  2259,  2258,
    2268,  2274,  2273,  2288,  2295,  2307,  2325,  2322,  2350,  2354,
    2355,  2357,  2362,  2363,  2367,  2368,  2372,  2375,  2377,  2382,
    2383,  2394,  2414,  2413,  2429,  2428,  2446,  2456,  2453,  2480,
    2488,  2496,  2507,  2518,  2527,  2542,  2543,  2547,  2548,  2549,
    2558,  2559,  2560,  2561,  2562,  2563,  2564,  2565,  2566,  2567,
    2568,  2569,  2570,  2571,  2572,  2573,  2574,  2575,  2576,  2577,
    2578,  2579,  2580,  2581,  2582,  2586,  2587,  2588,  2589,  2590,
    2591,  2592,  2593,  2594,  2595,  2596,  2597,  2602,  2603,  2607,
    2608,  2612,  2613,  2617,  2618,  2622,  2623,  2627,  2628,  2632,
    2636,  2640,  2645,  2650,  2655,  2660,  2668,  2672,  2680,  2681,
    2682,  2683,  2684,  2685,  2686,  2687,  2688,  2692,  2693,  2694,
    2695,  2696,  2697,  2701,  2702,  2703,  2707,  2708,  2709,  2710,
    2711,  2712,  2713,  2717,  2718,  2721,  2722,  2726,  2727,  2731,
    2736,  2737,  2739,  2741,  2743,  2748,  2750,  2755,  2757,  2759,
    2761,  2763,  2765,  2767,  2772,  2773,  2774,  2778,  2787,  2791,
    2799,  2803,  2810,  2833,  2834,  2839,  2840,  2841,  2842,  2843,
    2848,  2847,  2857,  2856,  2866,  2871,  2879,  2883,  2890,  2910,
    2929,  2933,  2940,  2942,  2944,  2951,  2952,  2953,  2957,  2961,
    2962,  2963,  2967,  2968,  2972,  2973,  2977,  2978,  2979,  2980,
    2984,  2985,  2989,  2993,  2994,  3000,  3001,  3005,  3009,  3013,
    3020,  3021,  3025,  3030,  3039,  3040,  3044,  3045,  3052,  3053,
    3054,  3055,  3056,  3057,  3059,  3061,  3063,  3065,  3071,  3073,
    3076,  3078,  3080,  3082,  3084,  3086,  3088,  3090,  3092,  3094,
    3099,  3103,  3107,  3111,  3115,  3124,  3133,  3145,  3149,  3153,
    3157,  3161,  3165,  3169,  3173,  3180,  3198,  3206,  3207,  3208,
    3209,  3210,  3211,  3212,  3216,  3217,  3221,  3225,  3232,  3239,
    3249,  3250,  3254,  3258,  3262,  3269,  3270,  3271,  3272,  3273,
    3274,  3275,  3276,  3277,  3278,  3282,  3284,  3286,  3288,  3290,
    3296,  3303,  3315,  3327,  3340,  3357,  3364,  3369,  3374,  3379,
    3385,  3391,  3421,  3428,  3435,  3436,  3440,  3442,  3443,  3445,
    3447,  3448,  3449,  3450,  3453,  3454,  3455,  3456,  3457,  3458,
    3459,  3460,  3461,  3465,  3466,  3470,  3471,  3472,  3476,  3477,
    3478,  3479,  3488,  3489,  3492,  3493,  3494,  3498,  3510,  3522,
    3529,  3531,  3533,  3535,  3537,  3539,  3548,  3554,  3558,  3562,
    3569,  3570,  3574,  3575,  3579,  3580,  3581,  3582,  3583,  3584,
    3585,  3586,  3591,  3596,  3601,  3606,  3611,  3620,  3625,  3634,
    3635,  3636,  3637,  3638,  3639,  3640,  3641,  3642,  3643,  3644,
    3645,  3646,  3647,  3648,  3649,  3650,  3651,  3652,  3653,  3654,
    3655,  3656,  3660,  3661,  3662,  3663,  3664,  3665,  3668,  3672,
    3676,  3680,  3684,  3691,  3695,  3699,  3703,  3711,  3712,  3713,
    3714,  3715,  3716,  3717
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
  "var_decl_stmt", "$@11", "$@12", "var_decl_stmt_inner_ls",
  "var_decl_stmt_inner", "tuple_var_decl_component",
  "tuple_var_decl_stmt_inner_ls", "opt_init_expr",
  "formal_or_ret_type_expr", "ret_type", "colon_ret_type", "opt_ret_type",
  "opt_type", "formal_type", "colon_formal_type", "opt_colon_formal_type",
  "expr_ls", "tuple_component", "tuple_expr_ls", "opt_actual_ls",
  "actual_ls", "actual_expr", "ident_expr", "sub_type_level_expr",
  "for_expr", "for_expr_base", "bracket_loop_expr",
  "bracket_loop_expr_base", "cond_expr", "nil_expr", "stmt_level_expr",
  "opt_task_intent_ls", "task_intent_clause", "task_intent_ls",
  "intent_expr", "task_var_prefix", "new_maybe_decorated", "new_expr",
  "let_expr", "range_literal_expr", "cast_expr", "tuple_expand_expr",
  "super_expr", "expr", "opt_expr", "opt_try_expr", "lhs_expr",
  "call_base_expr", "call_expr", "dot_expr", "parenthesized_expr",
  "bool_literal", "str_bytes_literal", "literal_expr", "assoc_expr_ls",
  "binary_op_expr", "unary_op_expr", "reduce_expr", "scan_expr",
  "reduce_scan_op_expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-883)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-734)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -883,   102,  3692,  -883,    86,   115,  -883,  -883,  -883,  -883,
    -883,  -883,  -883,  -883, 11956,    60,    64,   205, 13644,   220,
   17593,    60,   225,   129,   255,    64,  5346, 11956,   165, 17649,
    -883,   389,   357,  -883, 10041,   377,   306,   231,  -883,   391,
     274, 17649, 17649, 17649,  -883,  2650, 10909,   421, 11956, 11956,
     294,  -883,   437,   445, 11956,  -883, 13644,  -883, 11956,   508,
     351,   336,   372,  3875,   515, 17705,  -883, 11956,  8135, 11956,
   10909, 13644, 11956,   436,   522,   411,  5346,   540, 11956,   545,
    7097,  7097,  -883,   551,  -883, 13644,  -883,   555, 10214,  -883,
   11956,  -883, 11956,  -883,  -883, 13471, 11956,  -883, 10387,  -883,
    -883,  -883,  4290,  8308, 11956,  -883,  4994,  -883,  -883,  -883,
    -883, 17311,   612,  -883,   468,   453,  -883,   206,  -883,  -883,
    -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,  7443,  -883,  7616,  -883,  -883,   101,  -883,  -883,  -883,
     576,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,   495,
   17649,  -883,   475,   477,   417,  -883,   232,  -883,  -883,  -883,
     405,   242,  -883,  -883, 17649,  1768,  -883,   485,   492,  -883,
    -883,  -883,   494,   497, 11956,   501,   505,  -883,  -883,  -883,
   17110,  3226,   -74,   510,   518,  -883,  -883,   432,  -883,  -883,
    -883,  -883,  -883,   207,  -883,  -883,  -883, 11956,  -883, 17649,
    -883, 11956, 11956,     7,   628,   452,  -883,  -883,  -883,  -883,
   17110,   463,  -883,  -883,    42,  5346,  -883,  -883,  -883,   523,
     183,   527,  -883,   238,  -883,   533,  -883,   -60, 17110,  8481,
    -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883, 17649,
   17649,   -17, 14239,  -883,  -883,   607,  8481,   534,   528,   338,
    5346,  2956,    91,    15,  -883,  5346,  -883,  -883, 15223,  -883,
      23, 15754,   305,  -883,   530,   536,  -883, 15223,   183,   305,
    -883,  8481,  3082,  3082,    41,    -4,  -883,     3,  -883,  -883,
    -883,  -883,  7962,  7962,  -883,   529,  -883,   531,   554,  -883,
    -883,  3483,   559,  -883,  -883, 17110,   -53, 11956, 11956,  -883,
      96,  -883,  -883, 17110,   544, 15794,  -883, 15223,   183, 17110,
     543,  8481,  -883, 17110, 15834, 11956,  -883,  -883,  -883,  -883,
    -883,   183,   547,   217,   217,  1458,   305,   305,   257,  -883,
    -883,  4466,   -64, 11956,   569,   -45,   546,  1458,   712,  -883,
    -883, 17649,  -883, 11956,  -883,  4290,  -883,   124,  -883,   607,
    -883,  -883,   719,   550,  4642, 11956,  -883, 11956,   657,  -883,
    -883, 14893,   166,   495, 17110,   489, 11956,  5346,   645,  -883,
    -883, 10041, 10560, 11082,  -883,  -883,  -883, 17649, 11956,   558,
    -883, 17389,   288,   405,  -883,  -883,  -883,  -883,   638,  -883,
    -883,  -883,  -883,   144,   639,  -883,  -883,  -883, 13303,   606,
      -7,  -883,   567,   179,   466,  -883,   586, 11956, 11956, 11956,
   11082, 11082, 11956,   483, 11956, 11956, 11956, 11956, 11956,   532,
   13471, 11956, 11956, 11956, 11956, 11956, 11956, 11956, 11956, 11956,
   11956, 11956, 11956, 11956, 11956, 11956, 11956,   667,  -883,  -883,
    -883,  -883,  -883, 10214, 10214,  -883,  -883,  -883,  -883, 10214,
    -883,  -883, 10214, 10214,  8481,  8654, 11082, 11082, 15914,   565,
    -883, 11255, 11956, 17649,  -883,  6399,  -883,  -883,  -883,  2650,
    -883, 11956,  -883,   614,   570,   591,  -883,  -883,   615,   617,
    5346,   710,  5346,  -883,   711, 11956,   680,   578,  -883, 10214,
   17649,  -883,  -883,  2956,  -883,  -883,   110,  -883, 12129,   624,
   11956,  2650,  -883,  -883, 11956,  -883, 17445, 11956, 11956,  -883,
     238,   580,  -883,  -883,  -883,  -883, 17649,  -883,   405, 13167,
    2956,   610, 12129,  -883, 17110, 17110,  -883,   527,  -883,    38,
    -883,  8481,   585,  -883, 15223,   739,   739,  -883,  -883,  -883,
    -883, 11431,  -883, 15954,  8830,  9003,  -883,  -883,  -883, 17110,
   11082, 11082,  -883,   468,  9176,  -883,   301,  -883,  4818,  -883,
     194, 15994,   196, 14984, 17649,  6921,  6748,  -883,   495,   593,
    -883,   238,  -883, 15304,   652, 17649,    43, 15223,   600, 13761,
     -49,   107, 16074,   -65,    14, 14723,   131,   627,  1944,   604,
   17537,   651,   -55,  -883,  -883,   116,  -883,  -883,    76,  -883,
    -883,  6575,  -883,   671,  -883,   611,   629,  -883,   635,  -883,
     636,   637,   640,   641,   642,  -883,   643,   644,   646,   647,
     648,   650,   653,  -883,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,  -883,  -883,  -883, 11956,   662,   658,   611,  -883,
     611,  -883, 11956,  -883, 17649,  -883,  -883,  -883,  1237,  1237,
     481,  -883,   481,  -883,   481,  1921,   613,  1286,  1561,   183,
     217,  -883,   517,  -883,  -883,  -883,  -883,  -883,  1458,  2939,
     481,  1779,  1779,  1237,  1779,  1779,   675,   217,  2939,   519,
     675,   305,   305,   217,  1458,   623,   660,   666,   668,   669,
     674,   621,   631,  -883,   481,  -883,   481, 11956, 10214,    70,
   15383,   677,   649,   168,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,   121,  -883,   855, 17150,   393,   200, 17110, 10214,
    -883, 10214, 11956, 11956,   774,  5346,   777, 16153,  5346, 15463,
   17649,  -883,   127,  -883,   161,  -883,   195,  2956,    91,  -883,
   17110,  9349,  -883, 17110,  -883,  -883,  -883, 17649, 16194, 16234,
    2650,  -883,   610,   686,  -883,   204, 12129,  -883,   229,  -883,
   11956,  -883,   676,   -21,   673,  -883,  2093,  -883,  -883,  -883,
    -883,  -883,  -883,  -883,  -883,   113, 14802,  -883,  -883,   239,
    -883,  -883,  -883,  -883, 11956,  -883,   119, 13867, 11956,  -883,
   11956,  6921,  6748,  -883,  9522,   459,  9695,   464,   491,  9868,
    7789,   500,   246,  -883,  -883,  -883, 16314,   693,   685,   681,
    -883, 17649,  2650, 11082,    43, 11956,   806,  -883,  -883, 11956,
   17110,  5346,   679, 11082, 11604,  4114,   683, 11956, 11780,  -883,
    -883,    43,  5522, 11082,  -883,    43,  -883, 10214,   684,  2194,
    -883,  -883, 11956,    54,  -883,   840,  -883,   671,  -883,  -883,
    -883,  -883,  -883,     2,   521,  -883, 16354,  -883, 14027,  -883,
    -883, 17110,  -883,   688,   687,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,  -883,   243, 11082, 13761, 12302, 11082, 10214,  -883,
    -883,  -883,  7270,  -883,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,  -883,  -883,  -883,    53, 11082, 11082,  -883,  -883,
    -883, 17110, 17110,  5346,  -883,  5346, 11956,  -883,  5346,   822,
     700,   701, 17649,   527,  -883,  -883,   624,  -883,  -883,  -883,
     696,   697,   313,   229, 11956,   671,   610,  -883, 10736,  -883,
    -883, 17110,  -883,  5346, 11956,  -883,  -883,  -883, 17649,  -883,
     727,   527, 11082,  5346, 11082, 10214,  -883,  -883,  -883,  -883,
    -883,   517,  -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,  -883,  -883,  -883,  -883,  6921,  -883,  -883, 16400,
   15555,   315,   709,  6921,  -883, 11956,  -883,  -883, 14185,   347,
     386,    60, 16506,  -883, 16552, 11956,   250,  -883,  5346,   -46,
   15634,  -883,  -883, 17110,  5346, 17110,  -883, 16598,  -883, 10041,
   17110,    19, 15063,  -883,   254,  -883,  -883,  -883,  -883,  3215,
   17293,  -883, 17110, 17649,   612,  -883,     2,   245, 11956,  -883,
     731,   706,  -883,   611,   611,  -883,  -883,  -883,   707, 16704,
   11956,   849, 16750,   322,  -883,   624,   327,   344,  -883,  -883,
   17110,   850,  5346,  -883,  -883,  -883,  -883,  -883,  8481,  8481,
    -883,  -883,  -883,  -883,  -883,   229, 17110,  -883,   145,    74,
     715, 11956,  -883,   -16, 15714,    14, 15144,  -883,   133, 11956,
    6921,  6748,  -883,  -883,  -883, 16790, 17110, 14205,  -883,  -883,
    -883,    43,    43,  -883, 15223,  -883,  -883,  5698,   714,  5874,
     717,  -883, 11956, 14318,  6050,  -883,    43,  6226,  -883,    43,
     728, 17293,  -883,  -883,  -883, 17537,  -883,  -883,  -883,   125,
    -883,   -38,  -883,   367, 16910,    65,  -883,  -883,  -883, 12475,
   12648, 16870, 12821, 12994,  -883,  -883,  5346,  -883,   722,   723,
    -883,  -883,  5346,  -883,   527, 17110,  5346,  5346,  -883, 17110,
     397,   732, 11956,  -883,  -883,  -883,  -883, 10041,  -883, 17110,
    5346, 10041,  -883, 17110,  5346, 17110,  5346, 10041, 17110,  -883,
   10041, 17110,  -883,  -883,  -883,  -883,    61,  -883, 11956,   245,
     125,   125,   125,   125,   125,   125,   245,  5170,  -883,  -883,
    -883,  -883, 11956, 11956, 11956, 11956, 11956,  -883,   737,   738,
     652,  -883,  -883,  -883, 17110, 14399,  -883, 14478,  -883, 16153,
   14559, 14638,  -883, 17110,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,   756,  -883, 16950, 16990, 17030, 17070,  -883,  -883,  5346,
    5346,  5346,  5346, 11956, 11956, 11956, 11956, 16153, 16153, 16153,
   16153
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   140,   459,    55,   676,   677,
     678,   672,   673,   679,   211,   584,   129,   162,   556,   169,
     558,   584,   168,   490,   488,   129,     0,   211,   277,   163,
     201,   200,   670,   199,   211,   167,    68,   278,   340,   164,
     141,     0,     0,     0,   335,     0,   211,   171,   211,   211,
     605,   576,   680,   172,   211,   341,   550,   485,   211,     0,
       0,   186,   184,   426,   166,   559,   487,   211,   211,   211,
     211,   554,   211,     0,   170,     0,     0,   142,   211,   671,
     211,   211,   478,   165,   315,   552,   489,   173,   211,   729,
     211,   731,   211,   732,   733,   621,   211,   730,   211,   629,
     182,   728,     0,   211,   211,     4,     0,     5,    10,    11,
      12,    48,    51,    52,    56,     0,    46,    71,    13,    77,
      14,    15,    16,    17,    30,   536,   537,    23,    47,   183,
     193,   211,   202,   643,   194,    18,     0,    32,    31,    20,
       0,   272,    19,   639,    22,    36,    33,    34,   192,   316,
       0,   190,     0,     0,   636,   339,     0,   633,   188,   356,
     446,   437,   631,   191,     0,     0,   189,   648,   627,   626,
     630,   540,   538,     0,   211,   637,   638,   642,   641,   640,
       0,   539,     0,   649,   650,   651,   674,   675,   632,   542,
     541,   634,   635,     0,    29,   558,   163,   211,   141,     0,
     559,   211,   211,     0,     0,   636,   648,   538,   637,   638,
     548,   539,   649,   650,     0,     0,   585,   130,   131,     0,
     557,     0,   479,     0,   486,     0,    21,     0,   525,   211,
     137,   143,   155,   149,   148,   157,   134,   147,   158,   144,
     138,   159,   132,   160,   153,   146,   154,   152,   150,   151,
     133,   135,   145,   156,   161,   139,   325,   136,   210,     0,
       0,     0,     0,    70,    69,    71,   211,     0,     0,     0,
       0,     0,     0,     0,   496,     0,   179,    41,     0,   307,
       0,   306,   714,   609,   606,   607,   608,   492,   551,   715,
       7,   211,   338,   338,   433,   176,   428,   177,   429,   430,
     434,   435,   175,   174,   436,   426,   521,     0,   343,   344,
     346,     0,   427,   520,   348,   508,     0,   211,   211,   180,
     633,   624,   647,   625,     0,     0,    44,     0,   555,   543,
       0,   211,    45,   549,     0,   211,   285,    48,   289,   286,
     289,   553,     0,   716,   718,   619,   713,   712,     0,    74,
      78,     0,     0,   211,     0,     0,   527,   620,     0,     6,
     313,     0,   200,   211,   314,     0,    49,     0,     9,    71,
      50,    53,     0,    60,     0,   211,    72,   211,     0,   490,
     198,     0,   672,   316,   644,   206,   211,     0,   214,   215,
     216,   211,   211,   211,   213,   560,   567,     0,   211,     0,
     312,     0,   426,   446,   444,   445,   443,   364,   448,   447,
     451,   450,   452,     0,   441,   438,   439,   442,     0,   483,
       0,   480,     0,     0,   628,    35,   610,   211,   211,   211,
     211,   211,   211,   717,   211,   211,   211,   211,   211,     0,
     618,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   211,   211,   395,   402,   403,
     404,   399,   401,   211,   211,   397,   400,   398,   396,   211,
     406,   405,   211,   211,   211,   211,   211,   211,     0,     0,
     214,   211,   211,     0,   589,     0,    37,    25,    38,     0,
      26,   211,    39,   536,     0,   531,   532,   535,     0,     0,
       0,   246,     0,   395,   242,   211,     0,     0,    24,   211,
       0,   256,    27,     0,   500,   502,     0,   519,   211,   505,
     211,     0,   178,    40,   211,   309,     0,   211,   211,    42,
       0,     0,   187,   185,   431,   432,     0,   427,   446,   426,
       0,   523,   211,   122,   645,   646,   354,     0,   181,     0,
      43,   211,     0,   296,     0,   287,   288,    28,    76,    75,
      79,   211,   681,     0,   211,   211,   668,   666,     8,   492,
     211,   211,   212,    56,   211,    54,     0,    65,     0,   120,
       0,   115,     0,    86,     0,   211,   211,   195,   316,     0,
     208,     0,   205,   584,     0,     0,     0,     0,     0,   568,
       0,     0,   525,     0,     0,     0,   283,   317,     0,     0,
       0,   333,     0,   326,   413,     0,   415,   419,     0,   449,
     513,     0,   515,   453,   440,   411,   137,   391,   143,   389,
     149,   148,   147,   144,   138,   393,   159,   160,   146,   150,
     133,   145,   161,   388,   370,   373,   371,   372,   394,   383,
     374,   387,   379,   377,   390,   378,   376,   381,   386,   375,
     380,   384,   385,   382,   392,   211,   367,     0,   411,   368,
     411,   365,   211,   482,     0,   477,   495,   494,   709,   708,
     711,   720,   719,   724,   723,   705,   702,   703,   704,   622,
     692,   140,     0,   662,   663,   142,   661,   660,   616,   696,
     707,   701,   699,   710,   700,   698,   690,   695,   697,   706,
     689,   693,   694,   691,   617,     0,     0,     0,     0,     0,
       0,     0,     0,   722,   721,   726,   725,   211,   211,     0,
       0,   283,     0,   595,   596,   602,   601,   603,   598,   604,
     600,   587,     0,   590,     0,     0,     0,     0,   526,   211,
     545,   211,   211,   211,   245,     0,   241,     0,     0,     0,
       0,   544,     0,   259,     0,   257,     0,   503,     0,   518,
     517,   211,   498,   615,   497,   308,   305,     0,     0,     0,
       0,   659,   523,   349,   345,     0,   211,   524,   505,   347,
     211,   353,     0,     0,     0,   546,     0,   290,   682,   623,
     667,   528,   527,   669,   529,     0,     0,    57,    58,     0,
      61,    63,    67,    66,   211,    97,     0,     0,   211,    92,
     211,   211,   211,    64,   211,   370,   211,   371,   372,   211,
     211,   382,     0,   407,   408,    81,    80,    91,     0,     0,
     318,     0,     0,   211,     0,   211,     0,   219,   218,   211,
     569,     0,     0,   211,   211,     0,     0,   211,   211,   685,
     230,     0,     0,   211,   228,     0,   280,   211,     0,   338,
     324,   330,   211,   328,   323,   426,   414,   453,   511,   510,
     509,   512,   454,   460,   426,   363,     0,   369,     0,   359,
     360,   484,   481,     0,     0,   128,   126,   127,   125,   124,
     123,   657,   658,     0,   211,   683,   211,   211,   211,   588,
     597,   599,   211,   586,   162,   169,   168,   167,   164,   171,
     172,   166,   170,   165,   173,     0,   211,   211,   491,   534,
     533,   254,   253,     0,   248,     0,   211,   244,     0,   250,
       0,   282,     0,     0,   501,   504,   505,   506,   507,   304,
       0,     0,     0,   505,   211,   453,   523,   522,   211,   422,
     420,   355,   298,     0,   211,   297,   300,   547,     0,   291,
     294,     0,   211,     0,   211,   211,    59,   121,   118,    98,
     110,   104,   103,   102,   112,    99,   113,   108,   101,   109,
     107,   105,   106,   100,   111,   114,   211,   117,   116,    88,
      87,     0,     0,   211,   196,   211,   197,   318,   338,     0,
       0,   584,   584,   222,     0,   211,     0,   238,     0,     0,
       0,   684,   236,   570,     0,   687,   686,     0,   231,   211,
     563,     0,     0,   229,     0,   311,   186,   184,   310,   338,
     338,   319,   334,     0,     0,   416,   460,     0,   211,   357,
     418,     0,   366,   411,   411,   664,   665,   282,     0,     0,
     211,     0,     0,     0,   591,   505,   648,   648,   247,   243,
     575,   249,     0,    73,   279,   258,   255,   499,   211,   211,
     493,   423,   421,   350,   351,   505,   410,   302,     0,     0,
       0,   211,   292,     0,     0,     0,     0,    62,     0,   211,
     211,   211,    93,    95,    84,    83,    82,   338,   203,   209,
     207,     0,     0,   217,     0,   240,   239,     0,     0,     0,
       0,   237,   211,     0,     0,   226,     0,     0,   224,     0,
     284,   338,   321,   320,   329,     0,   336,   474,   475,     0,
     476,   462,   465,     0,   461,     0,   412,   361,   362,   211,
     211,     0,   211,   211,   284,   592,     0,   252,     0,     0,
     425,   424,     0,   301,     0,   295,     0,     0,   119,    89,
       0,     0,   211,   204,   221,   220,   223,   211,   234,   572,
       0,   211,   232,   571,     0,   688,     0,   211,   562,   227,
     211,   561,   225,   281,   322,   332,     0,   473,   211,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   455,   457,
     358,   456,   211,   211,   211,   211,   211,   251,   611,   612,
     303,   293,    94,    96,    85,     0,   235,     0,   233,   566,
       0,     0,   337,   464,   466,   467,   470,   471,   472,   468,
     469,   463,   458,     0,     0,     0,     0,   613,   614,     0,
       0,     0,     0,   211,   211,   211,   211,   574,   573,   565,
     564
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -883,  -883,  -883,     0,   -56,  2261,  -883,   277,   794,  -883,
    -883,   467,  -392,  -111,  -883,   339,  -883,  -883,   -66,  -883,
    -883,    37,   656,  -883,  -541,  2883,   539,  -571,  -883,  -795,
    -883,  -883,  -883,   100,  -883,  -883,  -883,   890,  -883,  1160,
     -98,  -883,  -883,  -457,   241,  -883,  -835,  -883,  -883,   269,
    -883,  -883,  -883,  -883,  -883,  -883,  -883,  -883,    10,  -883,
    -159,   549,  -278,  -883,   535,  -883,  -883,   -19,  1442,  -883,
    -883,  -883,  -883,   588,  -883,   -34,  -883,  -883,  -883,  -883,
     406,  -883,  -883,  -883,  -104,  -883,  -351,  -800,  -883,  -883,
    -883,  -883,  -883,  -599,  -883,    44,  -883,  -883,  -883,  -883,
    -883,   396,  -883,   -37,  -883,  -883,  -883,  -883,   552,  -883,
    -883,  -883,  -883,    51,  -408,  -177,  -784,  -882,  -648,  -883,
      57,  -883,    67,   -50,   654,   -48,  -883,  -883,  -341,  -808,
    -883,  -249,  -883,   -97,  -256,  -246,  -697,  -883,  -883,   114,
     283,  -883,  -202,   827,  -883,  -883,  -160,   442,   197,  -417,
    -762,  -598,  -883,  -883,  -883,  -593,  -512,  -883,  -747,   -10,
       6,  -883,  -263,  -493,  -555,    -2,  -883,  -883,  -883,  -883,
    -883,  -883,    56,  -883,   -13,  -563,  -883,    58,  -883,  -883,
     384,   513,  -883,  -883,  -883,  -883,  2415,  -883,   -23,   725,
    -883,  1207,  1408,  -883,  -883,  -883,  -883,  -883,  -883,  -883,
    -883,  -883,  -467
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   350,   106,   107,   108,   109,   110,   367,
     368,   111,   112,   113,   114,   373,   575,   809,   810,   115,
     116,   117,   378,   118,   119,   120,   351,   837,   582,   838,
     121,   122,   997,   579,   580,   123,   124,   219,   255,   272,
     125,   257,   126,   306,   529,   127,   128,   293,   292,   129,
     130,   131,   132,   133,   134,   591,   841,   135,   203,   137,
     393,   394,   600,   138,   261,   139,   764,   765,   204,   141,
     142,   143,   144,   555,   797,   971,   145,   146,   793,   966,
     279,   280,   147,   148,   149,   150,   399,   869,   151,   152,
     612,  1043,  1044,   613,   153,   205,  1196,   155,   156,   307,
     308,   309,   955,   157,   321,   547,   790,   158,   159,  1145,
     160,   161,   667,   668,   833,   834,   835,   959,   885,   403,
     615,  1051,   616,   617,   311,   537,   418,   407,   413,   883,
    1210,  1211,   162,  1049,  1141,  1142,  1143,   163,   164,   420,
     421,   673,   165,   166,   223,   530,   273,   274,   515,   516,
     772,   313,   881,   622,   623,   519,   314,   787,   788,   352,
     354,   355,   494,   495,   496,   206,   168,   169,   395,   170,
     396,   171,   207,   173,   215,   216,   742,   743,   744,   174,
     208,   209,   177,   178,   179,   322,   180,   385,   497,   211,
     182,   212,   213,   185,   186,   187,   188,   603,   189,   190,
     191,   192,   193
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   371,   105,   507,   473,   423,   791,   305,   221,   610,
     669,   871,   136,   310,   832,   312,   762,   227,   746,   811,
     889,   489,   890,   880,   167,   877,   960,  1002,   531,   383,
     789,   320,   589,   998,  1041,   953,   136,   852,   856,   792,
    1008,   861,   865,   484,   482,   324,   154,   275,   480,   389,
     390,   320,  1124,  -277,   517,   963,   275,   316,   172,  -331,
    -278,   769,   618,   520,  1047,   342,   320,   217,   552,  1046,
     154,  1081,   214,   265,   167,   214,   275,   517,   167,   167,
     356,  1198,   172,   858,   561,   505,   136,   500,   491,   534,
     136,   136,   517,   873,  1207,   491,   766,   964,  1207,   474,
     167,   475,     3,   565,   167,   214,   492,   275,   562,  -516,
    1199,   859,   136,   543,  -299,   604,   136,   874,   904,   218,
     154,  1048,   691,   785,   154,   154,   386,   851,  1137,   566,
    1117,   493,   172,   535,   387,   214,   172,   172,  -516,  -516,
     214,   674,   388,   389,   390,   620,   154,  1084,   369,   386,
     154,   965,   681,   683,   365,   853,  -299,   387,   172,   675,
    1166,   972,   172,   521,   879,   388,   389,   390,   493,   229,
     386,   524,  -260,  1041,   198,   946,   266,  -514,  1162,  -516,
    1138,   214,   481,  -516,  1077,   365,   388,   389,   390,   360,
     365,  1082,   320,   493,   365,  -516,   930,   783,   723,   725,
     518,  -516,  -327,  1160,  1132,  1133,  -514,  1107,   571,  1085,
    -299,   721,   722,   167,   365,   485,   910,  -516,   854,  1104,
     546,   391,   491,   518,   695,   136,  -327,  -516,   214,   320,
    1140,  1208,   365,   493,   214,   903,   365,   839,   518,  -516,
     222,   670,   856,   865,   391,   365,   905,  -516,  1137,   429,
    1001,  -514,   194,   375,   320,   854,   364,  -516,   767,   154,
     911,   491,  -261,  -514,   875,   391,    24,  -352,   167,   912,
     414,   172,  1041,   167,   957,   751,   392,  -267,   588,    84,
     136,  1003,  -266,   855,   768,   136,   404,   277,   794,   973,
     876,   621,   376,   604,   320,   913,  1194,   866,   476,   570,
    1138,   941,   477,  1155,   867,  1168,  1171,   405,   430,   942,
    -514,   326,   431,   283,   154,  -514,   294,   406,    57,   154,
     392,   377,   415,  1161,   777,   433,   172,   521,   780,   747,
      66,   172,  1065,   439,   416,   943,   296,   429,   229,  1139,
     298,   697,   814,   767,   818,   677,   305,   224,   521,   167,
    1140,   560,   767,   417,    86,   771,  1083,   336,   339,   433,
     815,   136,   819,   167,   437,   299,   928,   439,   300,   944,
      59,   442,   167,   284,  1034,   136,   493,   493,   956,   267,
     301,   958,   601,   -70,   136,   167,   175,   975,   607,   842,
     263,   751,   285,   264,  1003,   154,   430,   136,   491,   304,
     431,   258,   751,   -70,   266,  1147,  1148,   172,   286,   154,
     175,   493,  1004,   976,  -275,  1063,   671,  1057,   154,   -69,
     811,   172,   -70,   194,  1115,  1098,   320,   320,  1130,   558,
     172,   154,   320,   408,  -265,   320,   320,   320,   320,   -69,
     716,   717,  1197,   172,  1134,   746,   718,   433,  -262,   719,
     720,   -70,   437,   493,   438,   439,  1118,  1120,   -69,   442,
     175,   521,   614,  1003,   175,   175,   449,   194,  1126,  1129,
     751,   729,   320,   812,   455,  -594,   493,  1040,  -269,  1080,
     669,  1102,   409,   305,   927,   410,   175,   -69,   477,   310,
     175,   312,  -593,  1200,  -273,   674,  1154,   411,   167,   371,
     167,  -594,  -270,  1235,  1236,  1237,  1238,  1239,  1240,   365,
     136,   510,   136,  1109,   320,   176,   412,   290,  -593,   523,
    1201,   525,  1202,  1203,   291,  1204,  1205,   320,   320,  1170,
    1118,  1120,  1126,  1129,   521,   691,  1195,   320,   330,   176,
      38,   802,   802,  -338,   154,  1003,   154,   337,   337,   294,
    -731,   429,  1110,   692,  -731,  -732,   172,   294,   172,  -732,
     805,   532,   533,  1222,  -338,  1011,   693,    55,   550,   296,
     801,   804,  -264,   298,  -338,  1019,   167,   296,   560,  -268,
     844,   298,  -733,  -581,   331,  1031,  -733,   198,   136,   176,
    -655,  -728,  -655,   176,   176,  -728,   694,  -274,   299,   175,
     590,   300,  -276,    59,  1209,  -656,   299,  -656,  -263,   300,
     430,   358,  -271,   301,   431,   176,  1040,     7,   372,   176,
     952,   301,   154,   433,   374,  -655,  1058,  -655,   437,  1061,
     493,   439,   304,   397,   172,   442,  -652,   695,  -652,  -654,
     304,  -654,   398,   696,   432,   429,   401,  1131,   681,   723,
     402,  -578,  1135,   493,   175,  1209,  -653,   424,  -653,   175,
    -577,   433,   434,   425,   435,   436,   437,  -582,   438,   439,
     440,  -583,   441,   442,   443,   444,  -580,   446,   447,   448,
     449,   450,  1010,   452,  -579,   483,   453,   454,   455,   487,
     893,   320,   894,   376,  1093,  -417,  1095,   456,   365,   490,
     508,   509,   539,   527,   430,   538,   542,   429,   431,   528,
     548,  1054,   320,   557,   320,  1040,   551,   564,   978,   697,
     567,   568,   573,   574,   584,   595,   929,   181,   176,   608,
     619,   624,   672,   676,   320,   175,   439,   715,   728,   751,
     749,   752,   925,   753,   750,   755,   758,   760,   948,   175,
     771,   181,   761,   167,   781,   433,   167,   786,   175,   795,
     437,   796,   438,   439,   840,   136,   430,   442,   136,   493,
     431,   175,   845,   849,   449,   491,   870,   872,   882,  -140,
     453,   454,   455,   176,   884,  -162,  -169,  -168,   176,   895,
    -167,  -164,  -141,  -171,  -172,   901,  -166,  -170,  -142,   154,
    -165,   181,   154,  -173,   887,   181,   181,   902,   888,   933,
     493,   172,   935,  1039,   172,  1158,  1159,   433,   434,   305,
     435,   436,   437,   909,   438,   439,   896,   181,   305,   442,
     320,   181,   897,   954,   898,   899,   449,   847,   848,  1016,
     900,  1003,   453,   454,   455,   860,   864,   967,   962,   167,
     908,  1006,  1007,   167,  1015,  1018,  1035,  1072,   691,  1024,
     167,   136,  1055,  1056,   176,   136,  1073,  1074,   294,  1078,
    1079,   320,   136,   914,  1091,  1103,   915,   493,   176,   875,
    1146,   916,  1152,  1149,   175,  1156,   175,   176,   296,  1164,
    1180,   196,   298,  1184,  1193,   154,  1218,  1219,  1223,   154,
     176,   917,  1247,  1248,  1199,   366,   154,   172,   918,  1097,
     198,   172,   807,   578,   977,   225,   572,   299,   172,   919,
     300,   506,    59,  1075,  1066,  1067,   596,   920,   556,   371,
     775,   167,   301,   167,  1090,   784,   167,   592,   320,  1053,
     181,  1050,  1045,   136,   921,   136,   505,  1232,   136,  1136,
    1241,   304,  1039,  1234,  1089,  1009,   922,   892,   380,   536,
     695,   167,   175,   774,   945,     0,     0,   923,     0,     0,
    1064,   167,   924,  1088,     0,     0,     0,   154,     0,   154,
     493,   493,   154,   136,     0,     0,     0,     0,     0,   172,
       0,   172,     0,     0,   172,   181,     0,     0,  1111,  1112,
     181,     0,     0,     0,     0,     0,     0,   154,     0,     0,
       0,     0,     0,   176,     0,   176,   167,   154,     0,   172,
     371,     0,   167,     0,   371,     0,     0,     0,   136,   172,
       0,     0,     0,     0,   136,     0,     0,     0,     0,     0,
       0,   320,   320,     0,     0,     0,     0,   864,   505,     0,
     505,  1039,     0,   505,   505,     0,     0,     0,     0,     0,
       0,     0,   154,     0,     0,     0,     0,     0,   154,     0,
     167,     0,     0,     0,   172,     0,   181,     0,     0,     0,
     172,     0,   136,     0,     0,  1013,     0,     0,     0,     0,
     181,   176,     0,     0,     0,     0,     0,     0,     0,   181,
       0,     0,  1028,     0,     0,     0,  1033,     0,     0,     0,
       0,     0,   181,     0,     0,   167,   154,   167,     0,     0,
       0,     0,   167,     0,     0,   167,     0,   136,   172,   136,
       0,     0,     0,     0,   136,     0,     0,   136,     0,   175,
       0,     0,   175,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   167,     0,     0,     0,     0,     0,
     167,   154,     0,   154,   167,   167,   136,     0,   154,     0,
       0,   154,   136,   172,     0,   172,   136,   136,   167,     0,
     172,     0,   167,   172,   167,     0,     0,     0,     0,   256,
     136,     0,     0,     0,   136,     0,   136,     0,     0,     0,
     154,   268,   269,   270,     0,   167,   154,  1242,     0,   183,
     154,   154,   172,     0,     0,     0,     0,   136,   172,     0,
       0,     0,   172,   172,   154,   181,     0,   181,   154,     0,
     154,     0,     0,   183,     0,   175,   172,     0,     0,   175,
     172,     0,   172,     0,     0,     0,   175,   167,   167,   167,
     167,   154,     0,     0,     0,     0,     0,     0,     0,   136,
     136,   136,   136,   172,     0,     0,     0,     0,   176,   429,
       0,   176,  1125,  1128,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   183,     0,     0,     0,   183,   183,     0,
       0,     0,     0,   154,   154,   154,   154,     0,     0,     0,
       0,     0,     0,   181,     0,   172,   172,   172,   172,   183,
     400,     0,     0,   183,     0,     0,     0,   175,   429,   175,
       0,     0,   175,     0,   419,     0,     0,     0,   430,     0,
    1163,     0,   431,     0,     0,     0,  1125,  1128,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   175,     0,     0,
       0,     0,  1174,  1175,     0,  1176,     0,   175,     0,   479,
       0,     0,   432,     0,   176,     0,     0,  1189,   176,     0,
    1192,     0,     0,     0,     0,   176,     0,   430,     0,   433,
     434,   431,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,     0,   446,   447,   448,   449,   450,
     451,   452,   175,     0,   453,   454,   455,     0,   175,     0,
     184,     0,     0,     0,     0,   456,     0,     0,     0,   498,
     499,     0,   183,     0,     0,     0,     0,     0,   433,   434,
       0,   514,   436,   437,   184,   438,   439,     0,     0,     0,
     442,     0,     0,     0,   140,     0,   176,   449,   176,     0,
       0,   176,     0,   453,   454,   455,   175,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   140,     0,
       0,   541,     0,     0,     0,     0,   176,   183,     0,     0,
     181,     0,   183,   181,   184,     0,   176,     0,   184,   184,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   175,     0,   175,     0,     0,     0,     0,   175,     0,
     184,   175,     0,     0,   184,     0,     0,     0,   140,     0,
       0,   256,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   176,     0,     0,     0,     0,     0,   176,     0,     0,
     175,     0,     0,     0,   140,     0,   175,     0,   140,   430,
     175,   175,     0,   431,     0,     0,     0,   606,   183,     0,
       0,   611,     0,     0,   175,     0,     0,     0,   175,     0,
     175,     0,   183,     0,     0,     0,   181,     0,   666,     0,
     181,   183,     0,     0,     0,   176,     0,   181,     0,     0,
       0,   175,     0,   429,   183,     0,     0,     0,     0,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,   448,   449,
       0,     0,   452,   184,     0,   453,   454,   455,     0,     0,
     176,     0,   176,   175,   175,   175,   175,   176,     0,     0,
     176,     0,     0,   731,     0,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,   431,   140,   181,     0,
     181,     0,     0,   181,     0,     0,     0,     0,     0,   176,
     763,     0,     0,   514,     0,   176,     0,     0,   184,   176,
     176,     0,     0,   184,     0,     0,   776,     0,   181,     0,
       0,     0,     0,   176,     0,     0,   782,   176,   181,   176,
     514,     0,     0,   433,   434,     0,     0,   183,   437,   183,
     438,   439,   140,     0,     0,   442,     0,   140,     0,     0,
     176,     0,   449,     0,     0,     0,     0,     0,   453,   454,
     455,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   181,   823,     0,     0,     0,     0,   181,
       0,     0,     0,     0,     0,   846,     0,     0,     0,   184,
       0,     0,   176,   176,   176,   176,     0,     0,     0,   422,
     611,   230,     0,   184,     0,     0,     0,     0,     0,     0,
       0,     0,   184,     0,     0,   183,   231,   232,     0,   233,
       0,     0,     0,   140,   234,   184,     0,   181,     0,     0,
       0,     0,   235,     0,     0,     0,     0,   140,   236,     0,
       0,   429,     0,     0,   237,     0,   140,     0,   238,     0,
       0,   239,     0,   240,     0,     0,     0,     0,     0,   140,
       0,     0,   241,     0,   419,     0,     0,     0,     0,   242,
     243,     0,   181,     0,   181,     0,     0,   244,     0,   181,
       0,     0,   181,     0,     0,     0,     0,   245,     0,     0,
       0,     0,     0,     0,     0,     0,   246,   247,     0,   248,
     430,   249,     0,   250,   431,     0,   251,     0,     0,     0,
     252,   181,   253,     0,     0,   254,     0,   181,     0,     0,
       0,   181,   181,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   181,     0,     0,   184,   181,
     184,   181,     0,     0,     0,     0,     0,     0,     0,     0,
     940,   433,   434,     0,   435,   436,   437,   514,   438,   439,
     440,     0,   181,   442,     0,     0,     0,   949,     0,   448,
     449,   271,   140,   452,   140,   868,   453,   454,   455,  -318,
       0,     0,     0,   429,     0,     0,   970,   456,     0,     0,
       0,     0,   183,     0,     0,   183,     0,  -318,     0,     0,
       0,  -318,  -318,     0,   181,   181,   181,   181,     0,     0,
    -318,     0,  -318,  -318,     0,     0,   184,     0,  -318,     0,
       0,     0,     0,     0,     0,  -318,     0,     0,  -318,     0,
       0,   419,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,   431,  -318,     0,  -318,
     140,     0,  -318,     0,  -318,     0,  -318,     0,  -318,  -318,
       0,  -318,     0,     0,  -318,     0,  -318,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   666,     0,
       0,     0,     0,     0,     0,  -318,     0,  -318,   183,     0,
    -318,     0,   183,   433,   434,     0,   435,   436,   437,   183,
     438,   439,   440,     0,   441,   442,   443,   444,     0,   446,
     447,   448,   449,   450,     0,   452,     0,     0,   453,   454,
     455,     0,     0,     0,     0,     0,   230,     0,     0,   456,
       0,     0,   763,     0,     0,     0,     0,     0,     0,     0,
    -318,   231,   232,     0,   233,     0,  -318,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,   235,   970,     0,
       0,     0,     0,   236,     0,     0,     0,     0,     0,   237,
     183,     0,   183,   238,     0,   183,   239,     0,   240,     0,
       0,     0,     0,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,   184,   242,   243,   184,     0,     0,     0,
     183,     0,   244,     0,     0,     0,     0,     0,     0,     0,
     183,     0,   245,     0,     0,     0,     0,     0,     0,     0,
       0,   246,   247,     0,   248,     0,   249,   140,   250,     7,
     140,   251,     0,   611,     0,   252,     0,   253,     0,     0,
     254,     0,     0,     0,     0,     0,     0,   360,     0,     0,
       0,    23,    24,     0,     0,   183,     0,     0,     0,     0,
     361,   183,    30,   362,     0,     0,     0,     0,    33,     0,
       0,     0,     0,     0,     0,    38,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   184,
       0,     0,     0,   184,   365,     0,   968,   363,     0,     0,
     184,     0,    55,     0,    57,     0,    59,     0,  1036,   183,
       0,  1037,     0,     0,   364,     0,    66,   226,     0,     0,
       0,     0,     0,   140,     0,   611,     0,   140,     0,     0,
       0,     0,     0,     0,   140,    82,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   183,     0,   183,     0,     0,     0,
       0,   183,     0,     0,   183,     0,     0,   332,     0,     0,
       0,   184,     0,   184,     0,     0,   184,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     100,     0,     0,   183,     0,     0,  1038,   359,     0,   183,
       0,   184,     0,   183,   183,   140,     0,   140,     0,     0,
     140,   184,     0,     0,     0,     0,     0,   183,     0,     0,
       0,   183,     0,   183,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   140,     0,     0,     0,     0,
       0,     0,     0,     0,   183,   140,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   184,     0,     0,   210,
       0,     0,   184,   220,     0,     0,     0,     0,     0,     0,
       0,     0,   228,     0,     0,     0,     0,     0,     0,   262,
       0,     0,     0,     0,     0,     0,   183,   183,   183,   183,
     140,   278,     0,   281,   282,     0,   140,     0,     0,   287,
       0,   288,     0,   289,     0,     0,   486,     0,   315,     0,
     184,     0,   228,   323,   325,   327,   328,   329,     0,     0,
       0,   333,     0,   334,     0,     0,     0,     0,     0,     0,
     341,     0,     0,   323,     0,   343,     0,   344,     0,     0,
     345,   346,     0,   347,   140,     0,     0,   228,   323,   357,
       0,     0,     0,     0,     0,   184,     0,   184,     0,     0,
       0,   512,   184,     0,     0,   184,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   381,     0,   384,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   140,
       0,   140,     0,     0,   184,     0,   140,     0,     0,   140,
     184,     0,     0,     0,   184,   184,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   184,   426,
       0,     0,   184,     0,   184,     0,     0,     0,   140,     0,
       0,     0,     0,     0,   140,     0,     0,     0,   140,   140,
       0,     0,   478,     0,     0,   184,   333,   228,     0,     0,
       0,     0,   140,     0,     0,     0,   140,     0,   140,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   323,     0,     0,     0,   594,   140,
       0,     0,     0,   230,     0,     0,     0,   184,   184,   184,
     184,     0,     0,     0,     0,     0,     0,     0,   231,   232,
       0,   233,     0,     0,     0,     0,   234,     0,     0,     0,
       0,   323,     0,     0,   235,     0,     0,     0,     0,     0,
     236,   140,   140,   140,   140,     0,   237,     0,     0,     0,
     238,     0,     0,   239,     0,   240,   323,     0,     0,     0,
       0,     0,     0,     0,   241,     0,     0,   329,   333,     0,
       0,   242,   243,     0,     0,     0,     0,     0,     0,   244,
       0,     0,   544,   545,     0,     0,     0,     0,     0,   245,
       0,     0,     0,     0,     0,     0,   323,     0,   246,   247,
     554,   248,     0,   249,     0,   250,     0,     0,   251,     0,
       0,   754,   252,   756,   253,     0,     0,   254,   563,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   569,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     581,     0,   583,     0,     0,     0,     0,     0,     0,     0,
       0,   593,     0,     0,     0,     0,   597,   602,   605,     0,
       0,     0,     0,   228,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   271,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   678,   679,   680,   682,   684,   685,     0,   686,
     687,   688,   689,   690,     0,   698,   699,   700,   701,   702,
     703,   704,   705,   706,   707,   708,   709,   710,   711,   712,
     713,   714,     0,     0,     0,     0,     0,     0,   323,   323,
       0,     0,     0,     0,   323,     0,     0,   323,   323,   323,
     323,   724,   726,     0,     0,     0,   602,   730,     0,     0,
     745,     0,     0,     0,     0,     0,   748,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   757,     0,     0,
     759,     0,     0,     0,   323,     0,     0,     0,     0,   276,
       0,     0,     0,   770,     0,   773,     0,     0,     0,   281,
       0,     0,   778,   779,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   276,   315,     0,     0,   315,     0,   230,
       0,     0,     0,   338,   340,     0,   323,     0,     0,     0,
       0,   429,     0,     0,   231,   232,   748,   233,     0,   323,
     323,     0,   234,     0,     0,   228,   806,     0,     0,   323,
     235,     0,     0,     0,   370,     0,   236,     0,     0,     0,
     836,   836,   237,     0,     0,     0,   238,     0,     0,   239,
       0,   240,     0,     0,   850,     0,   934,     0,     0,   937,
     241,     0,     0,     0,     0,     0,     0,   242,   243,     0,
     430,     0,     0,     0,   431,   244,   315,     0,     0,     0,
       0,     0,     0,     0,     0,   245,     0,     0,     0,     0,
       0,     0,     0,     0,   246,   247,     0,   248,     0,   249,
       0,   250,     0,     0,   251,     0,     0,     0,   252,     0,
     253,     0,     0,   254,     0,     0,     0,     0,     0,     0,
     886,   433,   434,     0,   435,   436,   437,   891,   438,   439,
     440,     0,     0,   442,   443,   444,     0,   446,   447,   448,
     449,     0,     0,   452,   488,   360,   453,   454,   455,    23,
      24,     0,  1017,     0,     0,     0,  1022,   456,   361,     0,
      30,   362,     0,   522,     0,     0,    33,     0,     0,   513,
       0,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,     0,   757,   323,   501,   504,     0,     0,     0,     0,
       0,     0,   511,     0,     0,   363,     0,     0,     0,     0,
      55,   276,    57,   276,   323,     0,   323,   931,   932,     0,
     276,     0,   364,     0,    66,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,     0,     0,     0,
       0,     0,     0,    82,  1068,    84,  1069,     0,    86,  1071,
       0,   315,     0,     0,     0,   961,     0,     0,     0,     0,
     276,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     0,     0,     0,  1087,     0,     0,     0,     0,   581,
       0,     0,     0,   999,  1022,  1000,   836,   836,   360,   343,
       0,   344,    23,    24,   346,   347,     0,     0,     0,     0,
       0,   361,     0,    30,   362,     0,     0,     0,  1012,    33,
    1014,     0,     0,     0,   228,     0,    38,     0,  1020,   748,
    1023,     0,  1025,  1027,     0,     0,     0,  1030,  1032,  1116,
       0,     0,   323,     0,     0,  1121,     0,  1042,   363,     0,
       0,     0,     0,    55,     0,    57,     0,   358,     0,  1036,
       0,     0,  1037,     0,     0,   364,     0,    66,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1059,
    1023,  1030,  1062,   323,     0,     0,    82,   745,    84,     0,
       0,    86,     0,  1157,     0,     0,     0,     0,     0,     0,
       0,   682,   724,     0,     0,     0,     0,     0,     0,     0,
       0,  1070,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,     0,     0,   315,
       0,     0,     0,  1086,     0,     0,     0,     0,  1178,   228,
    1182,   100,     0,     0,     0,   522,     0,  1094,   522,  1096,
     323,     0,     0,     0,     0,     0,   472,     0,     0,  -652,
       0,  -652,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   836,     0,     0,     0,     0,     0,  1217,  1105,     0,
    1106,     0,     0,  1220,     0,     0,     0,  1178,  1182,     0,
    1114,     0,     0,     0,     0,     0,     0,   276,     0,     0,
       0,  1226,     0,     0,  1123,  1228,     0,   756,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1144,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1151,     0,     0,     0,   276,
     276,     0,     0,     0,     0,     0,   230,   276,   276,     0,
       0,     0,     0,   323,   323,     0,     0,     0,     0,     0,
       0,   231,   232,     0,   233,     0,  1165,     0,     0,   234,
     756,   756,   756,   756,  1169,   836,   836,   235,     0,     0,
       0,     0,     0,   236,     0,     0,     0,     0,     0,   237,
       0,     0,  1179,   238,  1183,     0,   239,  1185,   240,  1188,
       0,     0,  1191,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,     0,   242,   243,     0,     0,     0,     0,
       0,     0,   244,     0,  1179,  1183,     0,  1188,  1191,     0,
       0,     0,   245,     0,     0,     0,     0,     0,     0,     0,
       0,   246,   247,     0,   248,     0,   249,  1224,   250,     0,
       0,   251,  1225,     0,     0,   252,  1227,   253,     0,     0,
     254,  1229,  1230,     0,     0,  1231,     0,     0,     0,     0,
       0,     0,     0,  1233,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1243,  1244,  1229,
    1245,  1246,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   939,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   540,     0,     0,     0,
       0,     0,     0,     0,  1257,  1258,  1259,  1260,  1257,  1258,
    1259,  1260,     0,     0,     0,     0,     0,     0,     0,   969,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   276,
       0,     0,    -2,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,    15,    16,
      17,    18,     0,    19,     0,    20,    21,  -211,    22,    23,
      24,    25,    26,    27,     0,  -211,    28,   276,    29,     0,
      30,    31,    32,  -211,  -211,  -211,    33,    34,    35,   -68,
       0,    36,    37,    38,   276,    39,  -338,    40,   276,    41,
      42,    43,    44,    45,     0,    46,    47,    48,    49,   -68,
      50,    51,     0,    52,    53,    54,     0,  -338,     0,     0,
      55,    56,    57,    58,    59,    60,    61,  -338,   -68,    62,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,    76,     0,    77,    78,     0,
      79,    80,    81,    82,    83,    84,    85,   -68,    86,    87,
       0,     0,  -211,     0,    88,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,  1076,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,  1092,    97,    98,    99,   100,     0,
       0,   101,     0,   102,     0,   103,     0,  -211,     0,     0,
     104,     0,     0,     0,     0,     0,     0,     0,     0,     6,
       0,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,     0,    18,     0,     0,     0,   195,     0,
       0,     0,     0,   294,     0,     0,     0,     0,     0,     0,
       0,   196,     0,     0,   276,   276,  -211,  -211,  -211,     0,
     197,     0,     0,   296,     0,     0,     0,   298,     0,     0,
       0,     0,   199,     0,     0,    44,    45,     0,     0,     0,
       0,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,     0,   299,     0,    56,   300,    58,    59,    60,     0,
       0,     0,     0,    63,     0,   200,     0,   301,     0,     0,
       0,     0,   276,    71,     0,    73,     0,    75,   276,   276,
       0,     0,     0,     0,     0,     0,   304,     0,     0,    85,
       0,     0,     0,     0,   276,   276,     0,   276,     0,     0,
      89,     0,     0,     0,     0,     0,   504,     0,     0,   276,
       0,     0,   276,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,  1221,   103,  -342,
    -211,     0,     0,   104,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   504,     0,
     504,     0,     0,   504,   504,     4,     0,     5,     6,     7,
       8,     9,    10,    11,    12,    13,     0,  -683,     0,    14,
      15,    16,    17,    18,  -683,    19,     0,    20,    21,  -211,
      22,    23,    24,    25,    26,    27,  -683,  -211,    28,  -683,
      29,     0,    30,    31,    32,  -211,  -211,  -211,    33,    34,
      35,   -68,     0,    36,    37,    38,     0,    39,  -338,    40,
       0,    41,    42,    43,    44,    45,     0,    46,    47,    48,
      49,   -68,    50,    51,     0,    52,    53,    54,     0,  -338,
       0,     0,    55,    56,    57,    58,     0,    60,    61,  -338,
     -68,    62,    63,    64,    65,  -683,    66,    67,    68,  -683,
      69,    70,    71,    72,    73,    74,    75,    76,     0,    77,
      78,     0,    79,    80,    81,    82,    83,    84,    85,   -68,
      86,    87,     0,     0,  -211,     0,    88,     0,     0,  -683,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,  -683,    92,  -683,
    -683,  -683,  -683,  -683,  -683,  -683,     0,  -683,  -683,  -683,
    -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,  -683,    99,
     100,  -683,  -683,  -683,     0,   102,  -683,   103,     0,  -211,
       0,   348,  -683,     5,     6,     7,     8,     9,    10,    11,
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
       0,   102,   349,   103,     0,  -211,     0,     4,   104,     5,
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
      98,    99,   100,     0,     0,   101,     0,   102,   559,   103,
       0,  -211,     0,   576,   104,     5,     6,     7,     8,     9,
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
       0,   101,     0,   102,   577,   103,     0,  -211,     0,     4,
     104,     5,     6,     7,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,    15,    16,    17,    18,     0,    19,
       0,    20,    21,  -211,    22,    23,    24,    25,    26,    27,
       0,  -211,    28,     0,    29,     0,    30,    31,    32,  -211,
    -211,  -211,    33,    34,    35,   -68,     0,    36,    37,    38,
       0,    39,  -338,    40,     0,    41,    42,    43,    44,    45,
       0,    46,    47,    48,    49,   -68,    50,    51,     0,    52,
      53,    54,     0,  -338,     0,     0,    55,    56,    57,    58,
      59,    60,    61,  -338,   -68,    62,    63,    64,    65,     0,
      66,    67,    68,     0,    69,    70,    71,    72,    73,    74,
      75,    76,     0,    77,    78,     0,    79,    80,    81,    82,
      83,    84,    85,   -68,    86,    87,     0,     0,  -211,     0,
      88,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
     813,   103,     0,  -211,     0,     4,   104,     5,     6,     7,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
      15,    16,    17,    18,     0,    19,     0,    20,    21,  -211,
      22,    23,    24,    25,    26,    27,     0,  -211,    28,     0,
      29,     0,    30,    31,    32,  -211,  -211,  -211,    33,    34,
      35,   -68,     0,    36,    37,    38,     0,    39,  -338,    40,
       0,    41,    42,    43,    44,    45,     0,    46,    47,    48,
      49,   -68,    50,    51,     0,    52,    53,    54,     0,  -338,
       0,     0,    55,    56,    57,    58,   358,    60,    61,  -338,
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
      57,    58,    59,    60,    61,  -338,   -68,    62,    63,    64,
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
      33,    34,    35,   -68,     0,    36,    37,    38,     0,    39,
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
      30,    31,    32,  -211,  -211,  -211,    33,  1029,    35,   -68,
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
    -211,  -211,    33,  1177,    35,   -68,     0,    36,    37,    38,
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
      29,     0,    30,    31,    32,  -211,  -211,  -211,    33,  1181,
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
      32,  -211,  -211,  -211,    33,  1187,    35,   -68,     0,    36,
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
      33,  1190,    35,   -68,     0,    36,    37,    38,     0,    39,
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
     732,  -211,     5,     6,   104,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,   733,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
    -211,  -211,  -211,     0,   197,    35,     0,   734,     0,    37,
       0,   735,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,   736,     0,    56,   737,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,   738,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   201,     0,    77,     0,     0,    79,     0,     0,
     739,    83,     0,    85,     0,   740,    87,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     202,     0,   103,   741,  -211,     0,   878,   104,     5,     6,
       0,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   195,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,   295,
       0,   196,     0,     0,     0,    32,  -211,  -211,  -211,     0,
     197,    35,     0,     0,     0,   297,     0,     0,    39,     0,
     198,     0,   199,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   200,     0,     0,     0,     0,
       0,     0,     0,    71,   302,    73,    74,    75,   303,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,    85,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,     0,
    -211,     5,     6,   104,     8,     9,    10,    11,    12,    13,
       0,   627,     0,    14,     0,     0,    17,    18,   629,    19,
       0,   195,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   196,     0,     0,     0,    32,     0,
       0,     0,     0,   197,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   198,   635,   199,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   200,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,   201,     0,    77,     0,     0,    79,     0,     0,     0,
      83,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    89,   503,   458,   459,   460,   461,   462,
       0,     0,   465,   466,   467,   468,     0,   470,   471,     0,
     824,   825,   826,   827,   828,   648,     0,   649,     0,    95,
       0,   650,   651,   652,   653,   654,   655,   656,   829,   658,
     659,    97,   830,    99,   -90,   661,   662,   831,   664,   202,
       0,   103,     0,     0,     5,     6,   104,     8,     9,    10,
      11,    12,    13,     0,   627,     0,    14,     0,     0,    17,
      18,   629,    19,     0,   195,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,     0,     0,     0,     0,   197,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   198,   635,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   201,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    89,   503,   458,   459,
     460,   461,   462,     0,     0,   465,   466,   467,   468,     0,
     470,   471,     0,   824,   825,   826,   827,   828,   648,     0,
     649,     0,    95,     0,   650,   651,   652,   653,   654,   655,
     656,   829,   658,   659,    97,   830,    99,     0,   661,   662,
     831,   664,   202,     0,   103,     0,     0,     0,     0,   104,
       5,     6,     7,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,    15,     0,    17,    18,     0,    19,     0,
     195,    21,     0,    22,     0,     0,     0,     0,    27,     0,
       0,    28,     0,   196,     0,     0,     0,    32,     0,     0,
       0,     0,    34,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
      46,    47,    48,    49,     0,    50,    51,     0,    52,    53,
     335,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   200,     0,     0,
       0,     0,     0,    69,    70,    71,    72,    73,    74,    75,
      76,     0,    77,    78,     0,    79,     0,     0,     0,    83,
       0,    85,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   102,     0,
     103,     0,     0,     5,     6,   104,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,     0,   195,     0,     0,    22,     0,   733,     0,
       0,     0,     0,     0,    28,     0,   196,     0,     0,     0,
      32,     0,     0,     0,     0,   197,    35,     0,   734,     0,
      37,     0,   735,    39,     0,   198,     0,   199,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,   736,     0,    56,
     737,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     200,     0,   738,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   201,     0,    77,     0,     0,    79,     0,
       0,   739,    83,     0,    85,     0,   740,    87,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,     0,     0,     5,     6,   104,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,   195,     0,     0,    22,
     379,    24,     0,     0,     0,     0,     0,    28,     0,   196,
       0,     0,     0,    32,     0,     0,     0,     0,   197,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   198,     0,
     199,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,   363,     0,     0,     0,
       0,     0,    56,    57,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   200,     0,    66,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   201,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,    85,     0,    86,
      87,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   202,     0,   103,     0,     0,     5,
       6,   104,     8,     9,    10,   382,    12,    13,     0,     0,
       0,    14,     0,     0,    17,    18,     0,    19,     0,    20,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   196,     0,     0,     0,    32,  -211,  -211,  -211,
       0,   197,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   198,     0,   199,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,    65,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   201,
       0,    77,     0,     0,    79,     0,     0,     0,    83,    84,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   202,     0,   103,
       0,  -211,     5,     6,   104,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
       0,     0,     0,     0,   197,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
    -727,     0,     0,     0,  -727,     0,     0,    71,    72,    73,
      74,    75,   201,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,  -380,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,  -380,     0,     0,   101,     0,
     202,  -380,   103,     0,     0,     5,     6,   104,     8,     9,
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
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   202,     0,   103,     0,  -211,     5,     6,
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
      77,     0,     0,    79,   317,   318,     0,    83,     0,    85,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,   319,     0,     0,   101,     0,   202,     0,   103,     0,
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
     353,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,     0,     0,     5,     6,   104,     8,     9,    10,
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
     101,     0,   202,     0,   103,  -530,     0,     5,     6,   104,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,   195,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     196,     0,     0,     0,    32,     0,     0,     0,     0,   197,
      35,     0,     0,     0,    37,     0,     0,    39,     0,   198,
       0,   199,     0,     0,    44,    45,     0,     0,    47,     0,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
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
    -530,     0,   104,     5,     6,     0,     8,     9,    10,    11,
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
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,   800,     0,     5,     6,   104,     8,
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
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   202,     0,   103,   803,     0,     5,
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
     808,     0,     5,     6,   104,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
       0,     0,     0,     0,   197,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,   947,
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
      63,    64,   200,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   201,     0,    77,     0,     0,
      79,     0,     0,     0,    83,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
    -388,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,  -388,     0,
       0,   101,     0,   202,  -388,   103,     0,     0,     5,     6,
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
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,  -373,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,  -373,     0,     0,   101,     0,   202,  -373,   103,     0,
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
      75,   201,     0,    77,     0,     0,    79,     0,     0,     0,
      83,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,  -381,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,  -381,     0,     0,   101,     0,   202,
    -381,   103,     0,     0,     5,     6,   104,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   195,     0,     0,    22,     0,   259,
       0,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,     0,     0,     0,     0,   197,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   198,     0,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   201,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,    85,     0,   260,    87,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   202,     0,   103,     0,     0,     5,     6,   104,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,   195,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     196,     0,     0,     0,    32,     0,     0,     0,     0,   197,
      35,     0,     0,     0,    37,     0,     0,    39,     0,   198,
       0,   199,     0,     0,    44,    45,     0,     0,    47,     0,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
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
      60,     0,     0,     0,     0,    63,    64,   200,  -727,     0,
       0,     0,  -727,     0,     0,    71,    72,    73,    74,    75,
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
      32,     0,     0,     0,     0,   197,    35,     0,     0,     0,
      37,     0,     0,    39,     0,   198,     0,   199,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     200,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   201,     0,    77,     0,     0,    79,     0,
       0,     0,    83,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,   598,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,     0,     0,   599,     0,   104,     5,
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
      92,    93,    94,     0,  -409,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   202,     0,   103,
    -409,     0,     5,     6,   104,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,   275,    28,     0,   196,     0,     0,     0,    32,
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
     102,     0,   103,     0,     0,     5,     6,   104,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,     0,     0,
      17,    18,     0,    19,     0,   195,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   196,     0,
       0,     0,    32,     0,     0,     0,     0,   197,    35,     0,
       0,     0,    37,     0,     0,    39,     0,   198,     0,   199,
       0,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,    64,   200,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   201,     0,    77,     0,     0,
      79,     0,     0,     0,    83,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,   598,     0,    89,     0,     0,
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
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,     0,
       0,   599,     0,   104,     5,     6,     0,     8,     9,    10,
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
     101,     0,   202,   798,   103,     0,     0,     5,     6,   104,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,   195,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,    28,     0,
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
       0,     0,     0,   101,     0,   202,     0,   103,     0,     0,
    1021,     0,   104,     5,     6,     0,     8,     9,    10,    11,
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
       0,   202,     0,   103,     0,     0,  1026,     0,   104,     5,
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
      98,    99,     0,     0,     0,   101,     0,   202,     0,   103,
       0,     0,     5,     6,   104,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,   295,     0,   196,     0,     0,     0,    32,
       0,     0,     0,     0,   197,    35,     0,     0,     0,   297,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,   302,    73,
      74,    75,   303,     0,    77,     0,     0,    79,     0,     0,
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
       0,     0,    32,     0,     0,     0,     0,  1060,    35,     0,
       0,     0,    37,     0,     0,    39,     0,   198,     0,   199,
       0,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,    64,   200,     0,     0,     0,     0,     0,     0,     0,
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
    1212,    35,     0,     0,     0,    37,     0,     0,    39,     0,
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
       0,     5,     6,   104,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,   195,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   196,     0,     0,     0,    32,     0,
       0,     0,     0,  1213,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   198,     0,   199,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   200,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,   201,     0,    77,     0,     0,    79,     0,     0,     0,
      83,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,     0,     0,     5,     6,   104,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   195,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,     0,     0,     0,     0,  1215,    35,     0,     0,
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
      22,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     196,     0,     0,     0,    32,     0,     0,     0,     0,  1216,
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
       0,     0,     0,   101,     0,   202,     0,   103,     0,     0,
       0,     6,   104,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,     0,    18,     0,     0,     0,
     195,     0,     0,     0,     0,   294,     0,     0,     0,     0,
       0,     0,     0,   196,     0,     0,     0,     0,  -211,  -211,
    -211,     0,   197,     0,     0,   296,     0,     0,     0,   298,
       0,     0,     0,     0,   199,     0,     0,    44,    45,     0,
       0,     0,     0,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,     0,   299,     0,    56,   300,    58,    59,
      60,     0,     0,     0,     0,    63,     0,   200,     0,   301,
       0,     0,     0,     0,     0,    71,     0,    73,     0,    75,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   625,     0,   626,     0,     0,    90,
      91,    92,    93,    94,     0,     0,   627,     0,    95,     0,
       0,   628,   232,   629,   630,     0,     0,    96,     0,   631,
      97,    98,    99,     0,     0,     0,   101,   235,   202,   196,
     103,     0,  -211,   236,     0,   104,     0,     0,     0,   632,
       0,     0,     0,   238,     0,     0,   633,     0,   634,   635,
       0,     0,     0,     0,     0,     0,     0,   636,     0,     0,
       0,     0,     0,     0,   242,   637,     0,     0,     0,     0,
       0,     0,   244,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   638,     0,     0,     0,     0,     0,     0,     0,
       0,   246,   247,     0,   639,     0,   249,     0,   640,     0,
       0,   251,     0,     0,     0,   641,     0,   253,     0,     0,
     642,     0,     0,     0,     0,     0,     0,     0,     0,   503,
     458,   459,   460,   461,   462,     0,     0,   465,   466,   467,
     468,     0,   470,   471,     0,   643,   644,   645,   646,   647,
     648,     0,   649,     0,     0,     0,   650,   651,   652,   653,
     654,   655,   656,   657,   658,   659,     0,   660,     0,     0,
     661,   662,   663,   664,     5,     6,   665,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   195,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,  -211,  -211,  -211,     0,   197,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   198,     0,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   201,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,     0,    98,    99,     0,     0,     0,
     101,     0,   202,     0,   103,     0,  -211,     5,     6,     0,
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
       0,    87,     0,     0,     5,     6,     0,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   195,     0,    90,    22,    92,     0,
       0,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,  -211,  -211,  -211,     0,   197,    35,     0,    99,
       0,    37,     0,     0,    39,   202,   198,   103,   199,  -211,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   201,     0,    77,     0,     0,    79,
     691,     0,     0,    83,     0,    85,     0,     0,    87,     0,
     627,     0,     0,     0,     0,   979,   980,   629,   981,     0,
       0,     0,     0,   982,     0,     0,     0,     0,     0,     0,
       0,   693,     0,    90,     0,    92,     0,     0,     0,     0,
       0,     0,     0,   983,     0,     0,     0,   984,     0,     0,
     985,     0,   198,   635,     0,     0,    99,     0,     0,     0,
       0,   694,     0,     0,   103,     0,  -211,     0,     0,   986,
       0,     0,     0,     0,     0,     0,   987,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   988,     0,     0,     0,
       0,     0,     0,     0,     0,   989,   990,     0,   991,     0,
     992,     0,   695,     0,     0,     0,     0,     0,   696,   993,
       0,   994,     0,     0,   995,     0,     0,     0,     0,     0,
       0,     0,     0,   503,   458,   459,   460,   461,   462,     0,
       0,   465,   466,   467,   468,     0,   470,   471,     0,   643,
     644,   645,   646,   647,   648,     0,   649,     0,     0,     0,
     650,   651,   652,   653,   654,   655,   656,   657,   658,   659,
     230,   660,     0,     0,   661,   662,   663,   664,   996,     0,
     627,     0,     0,     0,     0,   231,   232,   629,   233,     0,
       0,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,   235,     0,     0,     0,     0,     0,   236,     0,     0,
       0,     0,     0,   237,     0,     0,     0,   238,     0,     0,
     239,     0,   240,   635,     0,     0,     0,     0,     0,     0,
       0,   241,     0,     0,     0,     0,     0,     0,   242,   243,
       0,     0,     0,     0,     0,     0,   244,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   245,     0,     0,     0,
       0,     0,     0,     0,     0,   246,   247,     0,   248,     0,
     249,     0,   250,     0,     0,   251,     0,     0,     0,   252,
       0,   253,     0,     0,   254,     0,     0,     0,     0,     0,
       0,     0,     0,   503,   458,   459,   460,   461,   462,     0,
       0,   465,   466,   467,   468,     0,   470,   471,     0,   643,
     644,   645,   646,   647,   648,     0,   649,     0,     0,     0,
     650,   651,   652,   653,   654,   655,   656,   657,   658,   659,
       7,   660,     0,     0,   661,   662,   663,   664,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   360,     0,
       7,     0,    23,    24,     0,     0,     0,     0,     0,     0,
       0,   361,     0,    30,   362,     0,     0,     0,   360,    33,
       0,     0,    23,    24,     0,     0,    38,     0,     0,     0,
       0,   361,     0,    30,   362,     0,     0,     0,     0,    33,
       0,     0,   427,     0,     0,     0,    38,     0,   363,   428,
       0,     0,     0,    55,     0,    57,     0,    59,     0,  1036,
       0,   429,  1037,     0,     0,   364,     0,    66,   363,     0,
       0,     0,     0,    55,     0,    57,     0,    59,     0,  1036,
       0,     0,  1037,     0,     0,   364,    82,    66,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
     430,   427,     0,     0,   431,     0,     0,     0,   428,     0,
       0,     0,     0,   502,     0,     0,     0,     0,     0,     0,
     429,   100,     0,     0,     0,     0,     0,  1108,     0,     0,
       0,     0,     0,     0,   432,   503,   458,   459,   460,   461,
     462,   100,     0,   465,   466,   467,   468,  1173,   470,   471,
       0,   433,   434,     0,   435,   436,   437,     0,   438,   439,
     440,     0,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,     0,     0,   453,   454,   455,   430,
     365,     0,   427,   431,     0,     0,     0,   456,     0,   428,
       0,     0,  1186,     0,     0,     0,     0,     0,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   432,   503,   458,   459,   460,   461,   462,
       0,     0,   465,   466,   467,   468,     0,   470,   471,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,     0,   453,   454,   455,     0,   365,
     430,   427,     0,     0,   431,     0,   456,     0,   428,     0,
       0,     0,     0,  1249,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   432,   503,   458,   459,   460,   461,
     462,     0,     0,   465,   466,   467,   468,     0,   470,   471,
       0,   433,   434,     0,   435,   436,   437,     0,   438,   439,
     440,     0,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,     0,     0,   453,   454,   455,   430,
     365,     0,   427,   431,     0,     0,     0,   456,     0,   428,
       0,     0,  1250,     0,     0,     0,     0,     0,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   432,   503,   458,   459,   460,   461,   462,
       0,     0,   465,   466,   467,   468,     0,   470,   471,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,     0,   453,   454,   455,     0,   365,
     430,   427,     0,     0,   431,     0,   456,     0,   428,     0,
       0,     0,     0,  1251,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   432,   503,   458,   459,   460,   461,
     462,     0,     0,   465,   466,   467,   468,     0,   470,   471,
       0,   433,   434,     0,   435,   436,   437,     0,   438,   439,
     440,     0,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,     0,     0,   453,   454,   455,   430,
     365,     0,     0,   431,     0,     0,   427,   456,     0,     0,
       0,     0,  1252,   428,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   429,   862,     0,     0,     0,
       0,     0,     0,   432,   503,   458,   459,   460,   461,   462,
       0,   863,   465,   466,   467,   468,     0,   470,   471,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,     0,   453,   454,   455,     0,   365,
       0,     0,     0,     0,   430,   427,   456,     0,   431,     0,
       0,     0,   428,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   429,   275,     0,     0,     0,     0,
       0,     0,     0,     0,   214,     0,     0,     0,   432,     0,
     974,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,     0,     0,
     453,   454,   455,   430,   365,     0,     0,   431,     0,     0,
       0,   456,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,   428,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   214,     0,   429,     0,   432,     0,     0,
     585,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   586,     0,   453,
     454,   455,     0,   365,     0,     0,     0,     0,     0,     0,
     456,     0,     0,     0,   430,     0,     0,     0,   431,     0,
       0,     0,     0,     0,     0,     0,     0,   427,   820,     0,
       0,     0,     0,     0,   428,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   429,     0,   432,     0,
       0,   821,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   822,   587,
     453,   454,   455,     0,     0,     0,     0,     0,     0,     0,
       0,   456,     0,     0,     0,   430,   427,     0,     0,   431,
       0,     0,     0,   428,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   429,  1127,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
       0,   453,   454,   455,   430,     0,     0,   427,   431,     0,
       0,     0,   456,     0,   428,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   429,   275,     0,     0,
       0,     0,     0,     0,   214,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,     0,     0,
     453,   454,   455,     0,   365,   430,   427,     0,     0,   431,
       0,   456,     0,   428,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   429,   275,     0,     0,     0,
       0,     0,     0,     0,     0,   214,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
       0,   453,   454,   455,   430,   365,     0,   427,   431,     0,
       0,     0,   456,     0,   428,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   432,     0,
       0,     0,   843,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,     0,     0,
     453,   454,   455,     0,   365,   430,   427,     0,     0,   431,
       0,   456,     0,   428,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   429,   906,     0,     0,     0,
       0,     0,     0,     0,     0,   214,     0,     0,     0,   432,
       0,   907,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
       0,   453,   454,   455,   430,     0,   427,     0,   431,     0,
       0,     0,   456,   428,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,     0,     0,
     453,   454,   455,     0,   430,     0,     0,     0,   431,     0,
       0,   456,     0,     0,     0,     0,     0,   938,   427,     0,
       0,     0,     0,     0,     0,   428,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   429,   432,     0,
       0,     0,  1100,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,     0,  1101,
     453,   454,   455,     0,   365,     0,     0,     0,     0,     0,
       0,   456,     0,     0,     0,     0,   430,   427,     0,     0,
     431,     0,     0,     0,   428,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   433,   434,     0,
     435,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
       0,     0,   453,   454,   455,   430,     0,   427,     0,   431,
       0,     0,     0,   456,   428,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,     0,   214,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,   427,   526,     0,
       0,     0,     0,     0,   428,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,   429,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
       0,   453,   454,   455,     0,   430,     0,   427,     0,   431,
    1119,     0,   456,     0,   428,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,     0,   214,     0,     0,     0,   432,
       0,     0,     0,     0,     0,   430,     0,   427,     0,   431,
       0,     0,     0,     0,   428,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,   429,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   432,
       0,   453,   454,   455,     0,   430,     0,     0,     0,   431,
    1167,     0,   456,     0,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   432,
       0,   453,   454,   455,     0,   430,     0,   427,     0,   431,
       0,     0,   456,     0,   428,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,   429,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   432,
       0,   453,   454,   455,     0,   549,     0,   427,     0,     0,
       0,     0,   456,     0,   428,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,   429,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
     553,   453,   454,   455,     0,   430,     0,   427,   816,   431,
       0,     0,   456,     0,   428,     0,     0,     0,   727,     0,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   432,
       0,     0,     0,     0,     0,   430,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   432,
       0,   453,   454,   455,     0,   430,     0,   427,     0,   431,
       0,     0,   456,     0,   428,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,   429,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   432,
       0,   453,   454,   455,     0,     0,     0,     0,   799,     0,
       0,     0,   456,     0,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   817,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
       0,   453,   454,   455,     0,   430,   427,     0,     0,   431,
       0,     0,   456,   428,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   429,     0,     0,   936,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   857,   432,
       0,     0,     0,     0,     0,     0,     0,   427,     0,     0,
       0,     0,     0,     0,   428,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,   429,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
       0,   453,   454,   455,   430,     0,     0,   427,   431,     0,
       0,     0,   456,     0,   428,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,   430,     0,     0,     0,   431,
       0,     0,     0,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,     0,   432,
     453,   454,   455,     0,     0,   430,     0,   427,  1005,   431,
       0,   456,     0,     0,   428,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,   429,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   432,
       0,   453,   454,   455,     0,     0,     0,   427,   950,     0,
       0,     0,   456,     0,   428,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,   429,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
       0,   453,   454,   455,     0,   430,     0,     0,   951,   431,
       0,     0,   456,   427,  1099,     0,     0,     0,     0,     0,
     428,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,   432,
       0,     0,     0,     0,     0,   430,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   432,
       0,   453,   454,   455,     0,     0,     0,     0,     0,     0,
       0,   430,   456,     0,     0,   431,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   427,
       0,   453,   454,   455,     0,   432,   428,     0,  1052,     0,
       0,     0,   456,     0,     0,     0,     0,     0,   429,     0,
       0,     0,   433,   434,     0,   435,   436,   437,     0,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   427,     0,   453,   454,   455,
       0,     0,   428,     0,     0,     0,     0,     0,   456,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   427,     0,     0,     0,     0,     0,     0,   428,     0,
       0,     0,     0,     0,     0,     0,     0,   214,     0,     0,
     429,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,   431,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,     0,     0,   453,   454,   455,     0,   432,     0,     0,
       0,     0,     0,     0,   456,     0,     0,     0,     0,   430,
       0,     0,     0,   431,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   427,  1113,   453,
     454,   455,  1122,   432,   428,     0,     0,     0,     0,     0,
     456,     0,     0,     0,     0,     0,   429,     0,     0,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   427,     0,   453,   454,   455,     0,     0,
     428,     0,     0,     0,     0,     0,   456,     0,     0,     0,
       0,     0,   429,  1153,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   430,     0,     0,     0,   431,
       0,     0,     0,   427,  1172,     0,     0,     0,     0,     0,
     428,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   430,     0,     0,     0,   431,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
       0,   453,   454,   455,     0,   432,     0,     0,     0,     0,
    1150,   430,   456,   427,     0,   431,     0,     0,     0,     0,
     428,     0,   433,   434,     0,   435,   436,   437,     0,   438,
     439,   440,   429,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   432,     0,   453,   454,   455,
       0,     0,     0,   427,     0,     0,     0,     0,   456,     0,
     428,     0,   433,   434,     0,   435,   436,   437,     0,   438,
     439,   440,   429,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,     0,     0,   453,   454,   455,
       0,   430,     0,   427,     0,   431,     0,     0,   456,     0,
     428,     0,  1206,     0,  1214,     0,     0,     0,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   432,     0,     0,     0,     0,
       0,   430,     0,   427,     0,   431,     0,     0,     0,     0,
     428,     0,   433,   434,     0,   435,   436,   437,     0,   438,
     439,   440,   429,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   432,     0,   453,   454,   455,
       0,   430,     0,   427,     0,   431,     0,     0,   456,     0,
     428,     0,   433,   434,  1253,   435,   436,   437,     0,   438,
     439,   440,   429,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   432,     0,   453,   454,   455,
       0,   430,     0,   427,     0,   431,     0,     0,   456,     0,
     428,     0,   433,   434,  1254,   435,   436,   437,     0,   438,
     439,   440,   429,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   432,     0,   453,   454,   455,
       0,   430,     0,   427,     0,   431,     0,     0,   456,     0,
     428,     0,   433,   434,  1255,   435,   436,   437,     0,   438,
     439,   440,   429,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   432,     0,   453,   454,   455,
       0,   430,     0,   427,     0,   431,     0,     0,   456,     0,
     428,     0,   433,   434,  1256,   435,   436,   437,     0,   438,
     439,   440,   429,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   432,     0,   453,   454,   455,
       0,   430,     0,     0,     0,   431,     0,     0,   456,     0,
       0,     0,   433,   434,     0,   435,   436,   437,     0,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   432,     0,   453,   454,   455,
       0,   926,     0,     0,     0,   431,     0,     0,   456,     0,
       0,     0,   433,   434,     0,   435,   436,   437,     0,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   432,     0,   453,   454,   455,
       0,     0,     0,     0,     0,     0,     0,     0,   456,     0,
       0,     0,   433,   434,     0,   435,   436,   437,     7,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,     0,   360,   453,   454,   455,
      23,    24,     0,     0,     0,     0,     0,     0,   456,   361,
       0,    30,   362,     0,   360,     0,     0,    33,    23,    24,
       0,     0,     0,     0,    38,     0,     0,   361,     0,    30,
     362,     0,     0,     0,     0,    33,     0,     0,     0,     0,
       0,     0,    38,     0,     0,  -338,   363,     0,     0,     0,
       0,    55,     0,    57,     0,     0,     0,  1036,   -68,     0,
    1037,     0,     0,   364,   363,    66,  -338,     0,     0,    55,
     609,    57,   230,     0,     7,    61,  -338,   -68,    62,     0,
       0,   364,     0,    66,    82,     0,    84,   231,   232,    86,
     233,     0,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,    82,   235,    84,     0,     0,    86,     0,   236,
       0,     0,     0,     0,     0,   237,     0,     0,     0,   238,
       0,     0,   239,     0,   240,     0,     0,     0,   230,     0,
       0,     0,     0,   241,     0,     0,     0,     0,     0,   100,
     242,   243,     0,   231,   232,     0,   233,     0,   244,     0,
       0,   234,     0,    24,     0,     0,     0,   100,   245,   235,
       0,     0,   365,     0,     0,   236,     0,   246,   247,     0,
     248,   237,   249,     0,   250,   238,     0,   251,   239,     0,
     240,   252,     0,   253,     0,     0,   254,     0,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,     0,     0,
       0,     0,     0,     0,   244,    57,     0,     0,     0,     0,
       0,     0,     0,     0,   245,     0,     0,    66,     0,     0,
     230,     0,     7,   246,   247,     0,   248,     0,   249,     0,
     250,     0,     0,   251,     0,   231,   232,   252,   233,   253,
       0,    86,   254,   234,     0,     0,     0,     0,     0,     0,
       0,   235,     0,     0,     0,     0,     0,   236,     0,     0,
       0,     0,     0,   237,     0,     0,     0,   238,     0,     0,
     239,     0,   240,     0,     0,     0,  -313,     0,     0,     0,
       0,   241,     0,     0,     0,     0,     0,     0,   242,   243,
       0,  -313,  -313,     0,  -313,     0,   244,     0,     0,  -313,
       0,     0,     0,     0,     0,     0,   245,  -313,     0,     0,
       0,     0,     0,  -313,     0,   246,   247,     0,   248,  -313,
     249,     0,   250,  -313,     0,   251,  -313,     0,  -313,   252,
       0,   253,   230,     0,   254,     0,     0,  -313,     0,     0,
       0,     0,     0,     0,  -313,  -313,     0,   231,   232,     0,
     233,     0,  -313,     0,     0,   234,     0,     0,     0,     0,
       0,     0,  -313,   235,     0,     0,     0,     0,     0,   236,
       0,  -313,  -313,     0,  -313,   237,  -313,     0,  -313,   238,
       0,  -313,   239,     0,   240,  -313,     0,  -313,  -314,     0,
    -313,     0,     0,   241,     0,     0,     0,     0,     0,     0,
     242,   243,     0,  -314,  -314,     0,  -314,     0,   244,     0,
       0,  -314,     0,     0,     0,     0,     0,     0,   245,  -314,
       0,     0,     0,     0,     0,  -314,     0,   246,   247,     0,
     248,  -314,   249,     0,   250,  -314,     0,   251,  -314,     0,
    -314,   252,     0,   253,     0,     0,   254,     0,     0,  -314,
       0,     0,     0,     0,     0,     0,  -314,  -314,     0,     0,
       0,     0,     0,     0,  -314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -314,     0,     0,     0,     0,     0,
       0,     0,     0,  -314,  -314,     0,  -314,     0,  -314,     0,
    -314,     0,     0,  -314,     0,     0,     0,  -314,     0,  -314,
       0,     0,  -314
};

static const yytype_int16 yycheck[] =
{
       2,   112,     2,   266,   181,   165,   547,    63,    21,   401,
     418,   610,     2,    63,   585,    63,   509,    27,   485,   574,
     668,   223,   670,   621,    26,   618,   788,   822,   291,   133,
     542,    68,   383,   817,   869,   782,    26,   600,   601,     1,
     840,   604,   605,     1,   203,    68,     2,    33,    41,    42,
      43,    88,    33,    57,     1,    76,    33,    67,     2,     5,
      57,   518,   403,    48,    62,    88,   103,     3,   331,   877,
      26,   953,   121,    36,    76,   121,    33,     1,    80,    81,
     103,   119,    26,   148,   148,   262,    76,   104,   148,    48,
      80,    81,     1,   148,    33,   148,   513,   118,    33,   173,
     102,   175,     0,   148,   106,   121,   166,    33,   172,    33,
     148,   176,   102,   166,    76,   393,   106,   172,    48,    55,
      76,   119,     3,   540,    80,    81,    25,   176,     3,   174,
     176,   229,    76,    92,    33,   121,    80,    81,    62,    48,
     121,   148,    41,    42,    43,     1,   102,   955,   111,    25,
     106,   172,   430,   431,   171,    48,   118,    33,   102,   166,
     176,    48,   106,   148,   621,    41,    42,    43,   266,   173,
      25,   148,    57,  1008,    55,   768,   173,    33,    33,   126,
      55,   121,   175,   107,   946,   171,    41,    42,    43,    23,
     171,   953,   229,   291,   171,   119,   751,   538,   476,   477,
     147,   148,   148,  1085,  1039,  1040,    62,  1007,   367,   956,
     172,   474,   475,   215,   171,   173,    48,   126,   148,  1003,
     124,   120,   148,   147,   105,   215,   172,   174,   121,   266,
     105,   166,   171,   331,   121,   728,   171,   588,   147,   148,
     111,   418,   805,   806,   120,   171,   176,   171,     3,    32,
     821,   107,   166,    47,   291,   148,    90,   166,   148,   215,
      92,   148,    57,   119,   148,   120,    28,   171,   270,   148,
      28,   215,  1107,   275,   786,   148,   175,    57,   382,   113,
     270,   148,    57,   176,   174,   275,    54,    46,   551,   176,
     174,   147,    86,   571,   331,   174,  1131,   166,    91,   175,
      55,   174,    95,  1065,   173,   172,  1101,    75,    91,   148,
     166,    70,    95,    19,   270,   171,    28,    85,    80,   275,
     175,   115,    80,  1085,   526,   142,   270,   148,   530,   489,
      92,   275,   925,   150,    92,   174,    48,    32,   173,    94,
      52,   439,   148,   148,   148,   166,   402,    92,   148,   351,
     105,   351,   148,   111,   116,   126,   954,    80,    81,   142,
     166,   351,   166,   365,   147,    77,   166,   150,    80,   174,
      82,   154,   374,    79,   867,   365,   474,   475,   174,   105,
      92,   152,   392,    47,   374,   387,     2,   148,   398,   591,
      84,   148,    98,    87,   148,   351,    91,   387,   148,   111,
      95,    12,   148,    67,   173,  1053,  1054,   351,   114,   365,
      26,   509,   166,   174,    57,   908,   418,   174,   374,    47,
     975,   365,    86,   166,   174,   996,   463,   464,   174,   172,
     374,   387,   469,    28,    57,   472,   473,   474,   475,    67,
     463,   464,  1139,   387,  1043,   912,   469,   142,    57,   472,
     473,   115,   147,   551,   149,   150,  1019,  1020,    86,   154,
      76,   148,   174,   148,    80,    81,   161,   166,  1031,  1032,
     148,   481,   509,   172,   169,   148,   574,   869,    57,   166,
     888,   166,    77,   539,    91,    80,   102,   115,    95,   539,
     106,   539,   148,   126,    57,   148,   174,    92,   500,   610,
     502,   174,    57,  1200,  1201,  1202,  1203,  1204,  1205,   171,
     500,   173,   502,   166,   551,     2,   111,     9,   174,   278,
     153,   280,   155,   156,   173,   158,   159,   564,   565,  1100,
    1093,  1094,  1095,  1096,   148,     3,  1135,   574,   102,    26,
      51,   564,   565,    54,   500,   148,   502,    80,    81,    28,
      91,    32,   166,    21,    95,    91,   500,    28,   502,    95,
     570,   292,   293,   166,    75,   843,    34,    78,   327,    48,
     564,   565,    57,    52,    85,   853,   578,    48,   578,    57,
     593,    52,    91,   166,   173,   863,    95,    55,   578,    76,
     173,    91,   175,    80,    81,    95,    64,    57,    77,   215,
     111,    80,    57,    82,  1145,   173,    77,   175,    57,    80,
      91,    82,    57,    92,    95,   102,  1008,     5,   150,   106,
     780,    92,   578,   142,   171,   173,   904,   175,   147,   907,
     728,   150,   111,    57,   578,   154,   173,   105,   175,   173,
     111,   175,   147,   111,   125,    32,   171,  1039,   926,   927,
     173,   166,  1044,   751,   270,  1196,   173,   165,   175,   275,
     166,   142,   143,   166,   145,   146,   147,   166,   149,   150,
     151,   166,   153,   154,   155,   156,   166,   158,   159,   160,
     161,   162,   842,   164,   166,    57,   167,   168,   169,   166,
     173,   728,   175,    86,   972,   174,   974,   178,   171,   166,
     166,   173,   148,   173,    91,   174,   147,    32,    95,   173,
     166,   888,   749,   166,   751,  1107,   173,   148,   816,   817,
     174,     9,     3,   173,    67,    80,   749,     2,   215,   171,
      92,    92,   126,   166,   771,   351,   150,    70,   173,   148,
     126,   126,   744,   126,   174,    35,    35,    67,   771,   365,
     126,    26,   174,   755,   174,   142,   758,   147,   374,   174,
     147,    22,   149,   150,   171,   755,    91,   154,   758,   867,
      95,   387,   120,   173,   161,   148,   172,   126,   107,   150,
     167,   168,   169,   270,   173,   150,   150,   150,   275,   166,
     150,   150,   150,   150,   150,   174,   150,   150,   150,   755,
     150,    76,   758,   150,   142,    80,    81,   176,   150,    35,
     908,   755,    35,   869,   758,  1078,  1079,   142,   143,   875,
     145,   146,   147,   174,   149,   150,   166,   102,   884,   154,
     867,   106,   166,   147,   166,   166,   161,   596,   597,   849,
     166,   148,   167,   168,   169,   604,   605,   174,   172,   851,
     173,   166,   171,   855,    48,   176,   172,    35,     3,   176,
     862,   851,   174,   176,   351,   855,   166,   166,    28,   173,
     173,   908,   862,    18,   147,   166,    21,   975,   365,   148,
     174,    26,    33,   176,   500,    35,   502,   374,    48,   174,
     176,    36,    52,   176,   166,   851,   174,   174,   166,   855,
     387,    46,   165,   165,   148,   111,   862,   851,    53,   975,
      55,   855,   573,   374,   814,    25,   367,    77,   862,    64,
      80,   265,    82,   942,   926,   927,   391,    72,   340,  1040,
     524,   933,    92,   935,   968,   539,   938,   385,   975,   888,
     215,   884,   875,   933,    89,   935,  1123,  1196,   938,  1046,
    1206,   111,  1008,  1199,   964,   841,   101,   674,   131,   305,
     105,   963,   578,   521,   767,    -1,    -1,   112,    -1,    -1,
     912,   973,   117,   963,    -1,    -1,    -1,   933,    -1,   935,
    1078,  1079,   938,   973,    -1,    -1,    -1,    -1,    -1,   933,
      -1,   935,    -1,    -1,   938,   270,    -1,    -1,  1011,  1012,
     275,    -1,    -1,    -1,    -1,    -1,    -1,   963,    -1,    -1,
      -1,    -1,    -1,   500,    -1,   502,  1018,   973,    -1,   963,
    1131,    -1,  1024,    -1,  1135,    -1,    -1,    -1,  1018,   973,
      -1,    -1,    -1,    -1,  1024,    -1,    -1,    -1,    -1,    -1,
      -1,  1078,  1079,    -1,    -1,    -1,    -1,   806,  1225,    -1,
    1227,  1107,    -1,  1230,  1231,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1018,    -1,    -1,    -1,    -1,    -1,  1024,    -1,
    1072,    -1,    -1,    -1,  1018,    -1,   351,    -1,    -1,    -1,
    1024,    -1,  1072,    -1,    -1,   844,    -1,    -1,    -1,    -1,
     365,   578,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   374,
      -1,    -1,   861,    -1,    -1,    -1,   865,    -1,    -1,    -1,
      -1,    -1,   387,    -1,    -1,  1117,  1072,  1119,    -1,    -1,
      -1,    -1,  1124,    -1,    -1,  1127,    -1,  1117,  1072,  1119,
      -1,    -1,    -1,    -1,  1124,    -1,    -1,  1127,    -1,   755,
      -1,    -1,   758,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1156,    -1,    -1,    -1,    -1,    -1,
    1162,  1117,    -1,  1119,  1166,  1167,  1156,    -1,  1124,    -1,
      -1,  1127,  1162,  1117,    -1,  1119,  1166,  1167,  1180,    -1,
    1124,    -1,  1184,  1127,  1186,    -1,    -1,    -1,    -1,    29,
    1180,    -1,    -1,    -1,  1184,    -1,  1186,    -1,    -1,    -1,
    1156,    41,    42,    43,    -1,  1207,  1162,  1207,    -1,     2,
    1166,  1167,  1156,    -1,    -1,    -1,    -1,  1207,  1162,    -1,
      -1,    -1,  1166,  1167,  1180,   500,    -1,   502,  1184,    -1,
    1186,    -1,    -1,    26,    -1,   851,  1180,    -1,    -1,   855,
    1184,    -1,  1186,    -1,    -1,    -1,   862,  1249,  1250,  1251,
    1252,  1207,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1249,
    1250,  1251,  1252,  1207,    -1,    -1,    -1,    -1,   755,    32,
      -1,   758,  1031,  1032,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,  1249,  1250,  1251,  1252,    -1,    -1,    -1,
      -1,    -1,    -1,   578,    -1,  1249,  1250,  1251,  1252,   102,
     150,    -1,    -1,   106,    -1,    -1,    -1,   933,    32,   935,
      -1,    -1,   938,    -1,   164,    -1,    -1,    -1,    91,    -1,
    1089,    -1,    95,    -1,    -1,    -1,  1095,  1096,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   963,    -1,    -1,
      -1,    -1,  1111,  1112,    -1,  1114,    -1,   973,    -1,   199,
      -1,    -1,   125,    -1,   851,    -1,    -1,  1126,   855,    -1,
    1129,    -1,    -1,    -1,    -1,   862,    -1,    91,    -1,   142,
     143,    95,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,    -1,   158,   159,   160,   161,   162,
     163,   164,  1018,    -1,   167,   168,   169,    -1,  1024,    -1,
       2,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,   259,
     260,    -1,   215,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   271,   146,   147,    26,   149,   150,    -1,    -1,    -1,
     154,    -1,    -1,    -1,     2,    -1,   933,   161,   935,    -1,
      -1,   938,    -1,   167,   168,   169,  1072,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
      -1,   311,    -1,    -1,    -1,    -1,   963,   270,    -1,    -1,
     755,    -1,   275,   758,    76,    -1,   973,    -1,    80,    81,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1117,    -1,  1119,    -1,    -1,    -1,    -1,  1124,    -1,
     102,  1127,    -1,    -1,   106,    -1,    -1,    -1,    76,    -1,
      -1,   361,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1018,    -1,    -1,    -1,    -1,    -1,  1024,    -1,    -1,
    1156,    -1,    -1,    -1,   102,    -1,  1162,    -1,   106,    91,
    1166,  1167,    -1,    95,    -1,    -1,    -1,   397,   351,    -1,
      -1,   401,    -1,    -1,  1180,    -1,    -1,    -1,  1184,    -1,
    1186,    -1,   365,    -1,    -1,    -1,   851,    -1,   418,    -1,
     855,   374,    -1,    -1,    -1,  1072,    -1,   862,    -1,    -1,
      -1,  1207,    -1,    32,   387,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,    -1,
      -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,   160,   161,
      -1,    -1,   164,   215,    -1,   167,   168,   169,    -1,    -1,
    1117,    -1,  1119,  1249,  1250,  1251,  1252,  1124,    -1,    -1,
    1127,    -1,    -1,   483,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    95,   215,   933,    -1,
     935,    -1,    -1,   938,    -1,    -1,    -1,    -1,    -1,  1156,
     510,    -1,    -1,   513,    -1,  1162,    -1,    -1,   270,  1166,
    1167,    -1,    -1,   275,    -1,    -1,   526,    -1,   963,    -1,
      -1,    -1,    -1,  1180,    -1,    -1,   536,  1184,   973,  1186,
     540,    -1,    -1,   142,   143,    -1,    -1,   500,   147,   502,
     149,   150,   270,    -1,    -1,   154,    -1,   275,    -1,    -1,
    1207,    -1,   161,    -1,    -1,    -1,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1018,   584,    -1,    -1,    -1,    -1,  1024,
      -1,    -1,    -1,    -1,    -1,   595,    -1,    -1,    -1,   351,
      -1,    -1,  1249,  1250,  1251,  1252,    -1,    -1,    -1,     1,
     610,     3,    -1,   365,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   374,    -1,    -1,   578,    18,    19,    -1,    21,
      -1,    -1,    -1,   351,    26,   387,    -1,  1072,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,   365,    40,    -1,
      -1,    32,    -1,    -1,    46,    -1,   374,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,   387,
      -1,    -1,    64,    -1,   674,    -1,    -1,    -1,    -1,    71,
      72,    -1,  1117,    -1,  1119,    -1,    -1,    79,    -1,  1124,
      -1,    -1,  1127,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,   101,
      91,   103,    -1,   105,    95,    -1,   108,    -1,    -1,    -1,
     112,  1156,   114,    -1,    -1,   117,    -1,  1162,    -1,    -1,
      -1,  1166,  1167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1180,    -1,    -1,   500,  1184,
     502,  1186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     760,   142,   143,    -1,   145,   146,   147,   767,   149,   150,
     151,    -1,  1207,   154,    -1,    -1,    -1,   777,    -1,   160,
     161,   173,   500,   164,   502,     1,   167,   168,   169,     5,
      -1,    -1,    -1,    32,    -1,    -1,   796,   178,    -1,    -1,
      -1,    -1,   755,    -1,    -1,   758,    -1,    23,    -1,    -1,
      -1,    27,    28,    -1,  1249,  1250,  1251,  1252,    -1,    -1,
      36,    -1,    38,    39,    -1,    -1,   578,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    54,    -1,
      -1,   841,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    95,    73,    -1,    75,
     578,    -1,    78,    -1,    80,    -1,    82,    -1,    84,    85,
      -1,    87,    -1,    -1,    90,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   888,    -1,
      -1,    -1,    -1,    -1,    -1,   111,    -1,   113,   851,    -1,
     116,    -1,   855,   142,   143,    -1,   145,   146,   147,   862,
     149,   150,   151,    -1,   153,   154,   155,   156,    -1,   158,
     159,   160,   161,   162,    -1,   164,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,   178,
      -1,    -1,   942,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    18,    19,    -1,    21,    -1,   172,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,   968,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
     933,    -1,   935,    50,    -1,   938,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,   755,    71,    72,   758,    -1,    -1,    -1,
     963,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     973,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,    -1,   101,    -1,   103,   755,   105,     5,
     758,   108,    -1,  1043,    -1,   112,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    27,    28,    -1,    -1,  1018,    -1,    -1,    -1,    -1,
      36,  1024,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   851,
      -1,    -1,    -1,   855,   171,    -1,   173,    73,    -1,    -1,
     862,    -1,    78,    -1,    80,    -1,    82,    -1,    84,  1072,
      -1,    87,    -1,    -1,    90,    -1,    92,    26,    -1,    -1,
      -1,    -1,    -1,   851,    -1,  1135,    -1,   855,    -1,    -1,
      -1,    -1,    -1,    -1,   862,   111,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1117,    -1,  1119,    -1,    -1,    -1,
      -1,  1124,    -1,    -1,  1127,    -1,    -1,    76,    -1,    -1,
      -1,   933,    -1,   935,    -1,    -1,   938,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,    -1,  1156,    -1,    -1,   172,   106,    -1,  1162,
      -1,   963,    -1,  1166,  1167,   933,    -1,   935,    -1,    -1,
     938,   973,    -1,    -1,    -1,    -1,    -1,  1180,    -1,    -1,
      -1,  1184,    -1,  1186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   963,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1207,   973,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1018,    -1,    -1,    14,
      -1,    -1,  1024,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    -1,  1249,  1250,  1251,  1252,
    1018,    46,    -1,    48,    49,    -1,  1024,    -1,    -1,    54,
      -1,    56,    -1,    58,    -1,    -1,   215,    -1,    63,    -1,
    1072,    -1,    67,    68,    69,    70,    71,    72,    -1,    -1,
      -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    -1,    -1,    88,    -1,    90,    -1,    92,    -1,    -1,
      95,    96,    -1,    98,  1072,    -1,    -1,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,  1117,    -1,  1119,    -1,    -1,
      -1,   270,  1124,    -1,    -1,  1127,   275,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1117,
      -1,  1119,    -1,    -1,  1156,    -1,  1124,    -1,    -1,  1127,
    1162,    -1,    -1,    -1,  1166,  1167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1180,   174,
      -1,    -1,  1184,    -1,  1186,    -1,    -1,    -1,  1156,    -1,
      -1,    -1,    -1,    -1,  1162,    -1,    -1,    -1,  1166,  1167,
      -1,    -1,   197,    -1,    -1,  1207,   201,   202,    -1,    -1,
      -1,    -1,  1180,    -1,    -1,    -1,  1184,    -1,  1186,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   229,    -1,    -1,    -1,   387,  1207,
      -1,    -1,    -1,     3,    -1,    -1,    -1,  1249,  1250,  1251,
    1252,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,
      -1,    21,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,
      -1,   266,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      40,  1249,  1250,  1251,  1252,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,   291,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,   302,   303,    -1,
      -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    -1,   317,   318,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,   331,    -1,    98,    99,
     335,   101,    -1,   103,    -1,   105,    -1,    -1,   108,    -1,
      -1,   500,   112,   502,   114,    -1,    -1,   117,   353,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   363,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     375,    -1,   377,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   386,    -1,    -1,    -1,    -1,   391,   392,   393,    -1,
      -1,    -1,    -1,   398,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   427,   428,   429,   430,   431,   432,    -1,   434,
     435,   436,   437,   438,    -1,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,    -1,    -1,    -1,    -1,    -1,    -1,   463,   464,
      -1,    -1,    -1,    -1,   469,    -1,    -1,   472,   473,   474,
     475,   476,   477,    -1,    -1,    -1,   481,   482,    -1,    -1,
     485,    -1,    -1,    -1,    -1,    -1,   491,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   502,    -1,    -1,
     505,    -1,    -1,    -1,   509,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,   518,    -1,   520,    -1,    -1,    -1,   524,
      -1,    -1,   527,   528,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,   539,    -1,    -1,   542,    -1,     3,
      -1,    -1,    -1,    80,    81,    -1,   551,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    18,    19,   561,    21,    -1,   564,
     565,    -1,    26,    -1,    -1,   570,   571,    -1,    -1,   574,
      34,    -1,    -1,    -1,   111,    -1,    40,    -1,    -1,    -1,
     585,   586,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    -1,    -1,   599,    -1,   755,    -1,    -1,   758,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,
      91,    -1,    -1,    -1,    95,    79,   621,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,    -1,   101,    -1,   103,
      -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,    -1,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
     665,   142,   143,    -1,   145,   146,   147,   672,   149,   150,
     151,    -1,    -1,   154,   155,   156,    -1,   158,   159,   160,
     161,    -1,    -1,   164,   221,    23,   167,   168,   169,    27,
      28,    -1,   851,    -1,    -1,    -1,   855,   178,    36,    -1,
      38,    39,    -1,   862,    -1,    -1,    44,    -1,    -1,   173,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   727,   728,   261,   262,    -1,    -1,    -1,    -1,
      -1,    -1,   269,    -1,    -1,    73,    -1,    -1,    -1,    -1,
      78,   278,    80,   280,   749,    -1,   751,   752,   753,    -1,
     287,    -1,    90,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   771,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   933,   113,   935,    -1,   116,   938,
      -1,   786,    -1,    -1,    -1,   790,    -1,    -1,    -1,    -1,
     327,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       5,    -1,    -1,    -1,   963,    -1,    -1,    -1,    -1,   814,
      -1,    -1,    -1,   818,   973,   820,   821,   822,    23,   824,
      -1,   826,    27,    28,   829,   830,    -1,    -1,    -1,    -1,
      -1,    36,    -1,    38,    39,    -1,    -1,    -1,   843,    44,
     845,    -1,    -1,    -1,   849,    -1,    51,    -1,   853,   854,
     855,    -1,   857,   858,    -1,    -1,    -1,   862,   863,  1018,
      -1,    -1,   867,    -1,    -1,  1024,    -1,   872,    73,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    84,
      -1,    -1,    87,    -1,    -1,    90,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   904,
     905,   906,   907,   908,    -1,    -1,   111,   912,   113,    -1,
      -1,   116,    -1,  1072,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   926,   927,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   936,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,    -1,    -1,   954,
      -1,    -1,    -1,   958,    -1,    -1,    -1,    -1,  1117,   964,
    1119,   166,    -1,    -1,    -1,  1124,    -1,   972,  1127,   974,
     975,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,   173,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   996,    -1,    -1,    -1,    -1,    -1,  1156,  1003,    -1,
    1005,    -1,    -1,  1162,    -1,    -1,    -1,  1166,  1167,    -1,
    1015,    -1,    -1,    -1,    -1,    -1,    -1,   554,    -1,    -1,
      -1,  1180,    -1,    -1,  1029,  1184,    -1,  1186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1048,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1060,    -1,    -1,    -1,   596,
     597,    -1,    -1,    -1,    -1,    -1,     3,   604,   605,    -1,
      -1,    -1,    -1,  1078,  1079,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    19,    -1,    21,    -1,  1091,    -1,    -1,    26,
    1249,  1250,  1251,  1252,  1099,  1100,  1101,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,  1117,    50,  1119,    -1,    53,  1122,    55,  1124,
      -1,    -1,  1127,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,  1149,  1150,    -1,  1152,  1153,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,    -1,   101,    -1,   103,  1172,   105,    -1,
      -1,   108,  1177,    -1,    -1,   112,  1181,   114,    -1,    -1,
     117,  1186,  1187,    -1,    -1,  1190,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1198,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1212,  1213,  1214,
    1215,  1216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   759,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1249,  1250,  1251,  1252,  1253,  1254,
    1255,  1256,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   796,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   806,
      -1,    -1,     0,     1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    -1,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,   844,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,   861,    53,    54,    55,   865,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,   105,   106,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,    -1,   120,    -1,   122,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   943,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,   971,   163,   164,   165,   166,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     4,
      -1,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    19,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    36,    -1,    -1,  1031,  1032,    41,    42,    43,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,    -1,
      -1,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    80,    81,    82,    83,    -1,
      -1,    -1,    -1,    88,    -1,    90,    -1,    92,    -1,    -1,
      -1,    -1,  1089,    98,    -1,   100,    -1,   102,  1095,  1096,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,  1111,  1112,    -1,  1114,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,  1123,    -1,    -1,  1126,
      -1,    -1,  1129,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,  1164,   173,   174,
     175,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1225,    -1,
    1227,    -1,    -1,  1230,  1231,     1,    -1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    13,    -1,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    -1,    63,    64,    65,
      66,    67,    68,    69,    -1,    71,    72,    73,    -1,    75,
      -1,    -1,    78,    79,    80,    81,    -1,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,   105,
     106,    -1,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,    -1,   120,    -1,   122,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,    -1,   171,   172,   173,    -1,   175,
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
     166,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,
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
       1,   175,     3,     4,   178,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    -1,    26,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    43,    -1,    45,    46,    -1,    48,    -1,    50,
      -1,    52,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    77,    -1,    79,    80,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,
     111,   112,    -1,   114,    -1,   116,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,   174,   175,    -1,     1,   178,     3,     4,
      -1,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    43,    -1,
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
     175,     3,     4,   178,     6,     7,     8,     9,    10,    11,
      -1,    13,    -1,    15,    -1,    -1,    18,    19,    20,    21,
      -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    56,    57,    -1,    -1,    60,    61,
      -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,
      -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
     112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,   131,
      -1,    -1,   134,   135,   136,   137,    -1,   139,   140,    -1,
     142,   143,   144,   145,   146,   147,    -1,   149,    -1,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
      -1,   173,    -1,    -1,     3,     4,   178,     6,     7,     8,
       9,    10,    11,    -1,    13,    -1,    15,    -1,    -1,    18,
      19,    20,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    56,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,   131,    -1,    -1,   134,   135,   136,   137,    -1,
     139,   140,    -1,   142,   143,   144,   145,   146,   147,    -1,
     149,    -1,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,   167,   168,
     169,   170,   171,    -1,   173,    -1,    -1,    -1,    -1,   178,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    16,    -1,    18,    19,    -1,    21,    -1,
      23,    24,    -1,    26,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,
      63,    64,    65,    66,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,   102,
     103,    -1,   105,   106,    -1,   108,    -1,    -1,    -1,   112,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,     3,     4,   178,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,
      -1,    21,    -1,    23,    -1,    -1,    26,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    45,    46,    -1,    48,    -1,
      50,    -1,    52,    53,    -1,    55,    -1,    57,    -1,    -1,
      60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,
      -1,    71,    72,    -1,    -1,    -1,    -1,    77,    -1,    79,
      80,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,
      90,    -1,    92,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,   103,    -1,   105,    -1,    -1,   108,    -1,
      -1,   111,   112,    -1,   114,    -1,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,    -1,     3,     4,   178,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,
      27,    28,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    -1,    83,    -1,    -1,    -1,
      -1,    88,    89,    90,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,   103,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,   112,    -1,   114,    -1,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,     3,
       4,   178,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    43,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,
      64,    -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,
      -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
      -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,   113,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,   175,     3,     4,   178,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      91,    -1,    -1,    -1,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,   148,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,    -1,
     171,   172,   173,    -1,    -1,     3,     4,   178,     6,     7,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,   175,     3,     4,
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
     105,    -1,    -1,   108,   109,   110,    -1,   112,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
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
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,    -1,     3,     4,   178,     6,     7,     8,
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
      66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,
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
     176,    -1,   178,     3,     4,    -1,     6,     7,     8,     9,
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
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,   174,    -1,     3,     4,   178,     6,
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
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,   174,    -1,     3,
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
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    70,
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
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,   103,    -1,   105,    -1,    -1,
     108,    -1,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
     148,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,
      -1,   169,    -1,   171,   172,   173,    -1,    -1,     3,     4,
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
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,   148,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,    -1,   171,   172,   173,    -1,
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
     102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
     112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,   148,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,   166,    -1,    -1,   169,    -1,   171,
     172,   173,    -1,    -1,     3,     4,   178,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,   112,    -1,   114,    -1,   116,   117,    -1,
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
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    91,    -1,
      -1,    -1,    95,    -1,    -1,    98,    99,   100,   101,   102,
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
      -1,    -1,    -1,   123,    -1,   125,    -1,    -1,    -1,    -1,
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
     144,   145,   146,    -1,   148,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
     174,    -1,     3,     4,   178,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,    -1,    -1,    -1,    40,
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
     171,    -1,   173,    -1,    -1,     3,     4,   178,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,
      18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,
      -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,
      68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,   103,    -1,   105,    -1,    -1,
     108,    -1,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,    -1,
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
     169,    -1,   171,   172,   173,    -1,    -1,     3,     4,   178,
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
     176,    -1,   178,     3,     4,    -1,     6,     7,     8,     9,
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
     101,   102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,
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
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
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
     102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
     112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,    -1,     3,     4,   178,     6,     7,     8,
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
      -1,     4,   178,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    41,    42,
      43,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    -1,    -1,    66,    -1,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    80,    81,    82,
      83,    -1,    -1,    -1,    -1,    88,    -1,    90,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    98,    -1,   100,    -1,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,     3,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    13,    -1,   151,    -1,
      -1,    18,    19,    20,    21,    -1,    -1,   160,    -1,    26,
     163,   164,   165,    -1,    -1,    -1,   169,    34,   171,    36,
     173,    -1,   175,    40,    -1,   178,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,    -1,   101,    -1,   103,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,   112,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,   131,    -1,    -1,   134,   135,   136,
     137,    -1,   139,   140,    -1,   142,   143,   144,   145,   146,
     147,    -1,   149,    -1,    -1,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,   164,    -1,    -1,
     167,   168,   169,   170,     3,     4,   173,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    43,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,    -1,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,     3,     4,    -1,
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
      -1,   117,    -1,    -1,     3,     4,    -1,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    -1,    21,    -1,    23,    -1,   142,    26,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    43,    -1,    45,    46,    -1,   165,
      -1,    50,    -1,    -1,    53,   171,    55,   173,    57,   175,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,    -1,   105,    -1,    -1,   108,
       3,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,    -1,
      13,    -1,    -1,    -1,    -1,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,   142,    -1,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    56,    -1,    -1,   165,    -1,    -1,    -1,
      -1,    64,    -1,    -1,   173,    -1,   175,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,    -1,   101,    -1,
     103,    -1,   105,    -1,    -1,    -1,    -1,    -1,   111,   112,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,   129,   130,   131,    -1,
      -1,   134,   135,   136,   137,    -1,   139,   140,    -1,   142,
     143,   144,   145,   146,   147,    -1,   149,    -1,    -1,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
       3,   164,    -1,    -1,   167,   168,   169,   170,   171,    -1,
      13,    -1,    -1,    -1,    -1,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,    -1,   101,    -1,
     103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,
      -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,   129,   130,   131,    -1,
      -1,   134,   135,   136,   137,    -1,   139,   140,    -1,   142,
     143,   144,   145,   146,   147,    -1,   149,    -1,    -1,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
       5,   164,    -1,    -1,   167,   168,   169,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
       5,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    36,    -1,    38,    39,    -1,    -1,    -1,    23,    44,
      -1,    -1,    27,    28,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    13,    -1,    -1,    -1,    51,    -1,    73,    20,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    84,
      -1,    32,    87,    -1,    -1,    90,    -1,    92,    73,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    84,
      -1,    -1,    87,    -1,    -1,    90,   111,    92,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    13,    -1,    -1,    95,    -1,    -1,    -1,    20,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      32,   166,    -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,   166,    -1,   134,   135,   136,   137,   172,   139,   140,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    91,
     171,    -1,    13,    95,    -1,    -1,    -1,   178,    -1,    20,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,   131,
      -1,    -1,   134,   135,   136,   137,    -1,   139,   140,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,   171,
      91,    13,    -1,    -1,    95,    -1,   178,    -1,    20,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,    -1,    -1,   134,   135,   136,   137,    -1,   139,   140,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    91,
     171,    -1,    13,    95,    -1,    -1,    -1,   178,    -1,    20,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,   131,
      -1,    -1,   134,   135,   136,   137,    -1,   139,   140,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,   171,
      91,    13,    -1,    -1,    95,    -1,   178,    -1,    20,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,    -1,    -1,   134,   135,   136,   137,    -1,   139,   140,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    91,
     171,    -1,    -1,    95,    -1,    -1,    13,   178,    -1,    -1,
      -1,    -1,   104,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,   131,
      -1,    48,   134,   135,   136,   137,    -1,   139,   140,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,   171,
      -1,    -1,    -1,    -1,    91,    13,   178,    -1,    95,    -1,
      -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,   125,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    91,   171,    -1,    -1,    95,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    32,    -1,   125,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    74,    -1,   167,
     168,   169,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    14,    -1,
      -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,   125,    -1,
      -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    74,   166,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    91,    13,    -1,    -1,    95,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    91,    -1,    -1,    13,    95,    -1,
      -1,    -1,   178,    -1,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,   171,    91,    13,    -1,    -1,    95,
      -1,   178,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    91,   171,    -1,    13,    95,    -1,
      -1,    -1,   178,    -1,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,   171,    91,    13,    -1,    -1,    95,
      -1,   178,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,   125,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    91,    -1,    13,    -1,    95,    -1,
      -1,    -1,   178,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    -1,   104,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,   125,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    74,
     167,   168,   169,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    91,    13,    -1,    -1,
      95,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,    91,    -1,    13,    -1,    95,
      -1,    -1,    -1,   178,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    14,    -1,
      -1,    -1,    -1,    -1,    20,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    32,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,    91,    -1,    13,    -1,    95,
     176,    -1,   178,    -1,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    13,    -1,    95,
      -1,    -1,    -1,    -1,    20,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    32,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   125,
      -1,   167,   168,   169,    -1,    91,    -1,    -1,    -1,    95,
     176,    -1,   178,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   125,
      -1,   167,   168,   169,    -1,    91,    -1,    13,    -1,    95,
      -1,    -1,   178,    -1,    20,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    32,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   125,
      -1,   167,   168,   169,    -1,   171,    -1,    13,    -1,    -1,
      -1,    -1,   178,    -1,    20,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    32,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
     166,   167,   168,   169,    -1,    91,    -1,    13,    14,    95,
      -1,    -1,   178,    -1,    20,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   125,
      -1,   167,   168,   169,    -1,    91,    -1,    13,    -1,    95,
      -1,    -1,   178,    -1,    20,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    32,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   125,
      -1,   167,   168,   169,    -1,    -1,    -1,    -1,   174,    -1,
      -1,    -1,   178,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,    91,    13,    -1,    -1,    95,
      -1,    -1,   178,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    32,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    91,    -1,    -1,    13,    95,    -1,
      -1,    -1,   178,    -1,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,   125,
     167,   168,   169,    -1,    -1,    91,    -1,    13,    14,    95,
      -1,   178,    -1,    -1,    20,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    32,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   125,
      -1,   167,   168,   169,    -1,    -1,    -1,    13,   174,    -1,
      -1,    -1,   178,    -1,    20,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    32,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,    91,    -1,    -1,   174,    95,
      -1,    -1,   178,    13,    14,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   125,
      -1,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,   178,    -1,    -1,    95,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    13,
      -1,   167,   168,   169,    -1,   125,    20,    -1,   174,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    13,    -1,   167,   168,   169,
      -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      32,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    95,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    95,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    13,   166,   167,
     168,   169,   124,   125,    20,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    13,    -1,   167,   168,   169,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    13,    14,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    95,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,   125,    -1,    -1,    -1,    -1,
     176,    91,   178,    13,    -1,    95,    -1,    -1,    -1,    -1,
      20,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    32,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   125,    -1,   167,   168,   169,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,   178,    -1,
      20,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    32,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,    91,    -1,    13,    -1,    95,    -1,    -1,   178,    -1,
      20,    -1,    62,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    13,    -1,    95,    -1,    -1,    -1,    -1,
      20,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    32,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   125,    -1,   167,   168,   169,
      -1,    91,    -1,    13,    -1,    95,    -1,    -1,   178,    -1,
      20,    -1,   142,   143,   104,   145,   146,   147,    -1,   149,
     150,   151,    32,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   125,    -1,   167,   168,   169,
      -1,    91,    -1,    13,    -1,    95,    -1,    -1,   178,    -1,
      20,    -1,   142,   143,   104,   145,   146,   147,    -1,   149,
     150,   151,    32,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   125,    -1,   167,   168,   169,
      -1,    91,    -1,    13,    -1,    95,    -1,    -1,   178,    -1,
      20,    -1,   142,   143,   104,   145,   146,   147,    -1,   149,
     150,   151,    32,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   125,    -1,   167,   168,   169,
      -1,    91,    -1,    13,    -1,    95,    -1,    -1,   178,    -1,
      20,    -1,   142,   143,   104,   145,   146,   147,    -1,   149,
     150,   151,    32,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   125,    -1,   167,   168,   169,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,   178,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   125,    -1,   167,   168,   169,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,   178,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   125,    -1,   167,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,     5,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    23,   167,   168,   169,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,   178,    36,
      -1,    38,    39,    -1,    23,    -1,    -1,    44,    27,    28,
      -1,    -1,    -1,    -1,    51,    -1,    -1,    36,    -1,    38,
      39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    -1,    -1,    54,    73,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    -1,    -1,    84,    67,    -1,
      87,    -1,    -1,    90,    73,    92,    75,    -1,    -1,    78,
       1,    80,     3,    -1,     5,    84,    85,    86,    87,    -1,
      -1,    90,    -1,    92,   111,    -1,   113,    18,    19,   116,
      21,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    34,   113,    -1,    -1,   116,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   166,
      71,    72,    -1,    18,    19,    -1,    21,    -1,    79,    -1,
      -1,    26,    -1,    28,    -1,    -1,    -1,   166,    89,    34,
      -1,    -1,   171,    -1,    -1,    40,    -1,    98,    99,    -1,
     101,    46,   103,    -1,   105,    50,    -1,   108,    53,    -1,
      55,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    92,    -1,    -1,
       3,    -1,     5,    98,    99,    -1,   101,    -1,   103,    -1,
     105,    -1,    -1,   108,    -1,    18,    19,   112,    21,   114,
      -1,   116,   117,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,     3,    -1,    -1,    -1,
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
      -1,   108,    53,    -1,    55,   112,    -1,   114,     3,    -1,
     117,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    -1,    18,    19,    -1,    21,    -1,    79,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    89,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    98,    99,    -1,
     101,    46,   103,    -1,   105,    50,    -1,   108,    53,    -1,
      55,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,    -1,   101,    -1,   103,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,   112,    -1,   114,
      -1,    -1,   117
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
     292,   293,   314,   319,   320,   324,   325,   347,   348,   349,
     351,   353,   354,   355,   361,   362,   363,   364,   365,   366,
     368,   371,   372,   373,   374,   375,   376,   377,   378,   380,
     381,   382,   383,   384,   166,    23,    36,    45,    55,    57,
      90,   103,   171,   240,   250,   277,   347,   354,   362,   363,
     368,   371,   373,   374,   121,   356,   357,     3,    55,   219,
     368,   356,   111,   326,    92,   219,   187,   341,   368,   173,
       3,    18,    19,    21,    26,    34,    40,    46,    50,    53,
      55,    64,    71,    72,    79,    89,    98,    99,   101,   103,
     105,   108,   112,   114,   117,   220,   221,   223,    12,    28,
     116,   246,   368,    84,    87,   203,   173,   105,   221,   221,
     221,   173,   221,   328,   329,    33,   207,   226,   368,   262,
     263,   368,   368,    19,    79,    98,   114,   368,   368,   368,
       9,   173,   230,   229,    28,    34,    48,    50,    52,    77,
      80,    92,    99,   103,   111,   186,   225,   281,   282,   283,
     305,   306,   307,   333,   338,   368,   341,   109,   110,   166,
     285,   286,   367,   368,   370,   368,   226,   368,   368,   368,
     102,   173,   187,   368,   368,    73,   189,   193,   207,   189,
     207,   368,   370,   368,   368,   368,   368,   368,     1,   172,
     185,   208,   341,   152,   342,   343,   370,   368,    82,   187,
      23,    36,    39,    73,    90,   171,   190,   191,   192,   203,
     207,   195,   150,   197,   171,    47,    86,   115,   204,    27,
     325,   368,     9,   266,   368,   369,    25,    33,    41,    42,
      43,   120,   175,   242,   243,   350,   352,    57,   147,   268,
     221,   171,   173,   301,    54,    75,    85,   309,    28,    77,
      80,    92,   111,   310,    28,    80,    92,   111,   308,   221,
     321,   322,     1,   328,   165,   166,   368,    13,    20,    32,
      91,    95,   125,   142,   143,   145,   146,   147,   149,   150,
     151,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   167,   168,   169,   178,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   170,   297,   173,   175,    91,    95,   368,   221,
      41,   175,   242,    57,     1,   173,   187,   166,   207,   324,
     166,   148,   166,   222,   344,   345,   346,   370,   221,   221,
     104,   207,   104,   126,   207,   297,   204,   344,   166,   173,
     173,   207,   187,   173,   221,   330,   331,     1,   147,   337,
      48,   148,   187,   226,   148,   226,    14,   173,   173,   226,
     327,   344,   231,   231,    48,    92,   306,   307,   174,   148,
     173,   221,   147,   166,   368,   368,   124,   287,   166,   171,
     226,   173,   344,   166,   368,   255,   255,   166,   172,   172,
     185,   148,   172,   368,   148,   148,   174,   174,     9,   368,
     175,   242,   243,     3,   173,   198,     1,   172,   208,   215,
     216,   368,   210,   368,    67,    37,    74,   166,   266,   268,
     111,   237,   290,   368,   187,    80,   246,   368,   123,   176,
     244,   341,   368,   379,   244,   368,   221,   341,   171,     1,
     194,   221,   272,   275,   174,   302,   304,   305,   310,    92,
       1,   147,   335,   336,    92,     1,     3,    13,    18,    20,
      21,    26,    46,    53,    55,    56,    64,    72,    89,   101,
     105,   112,   117,   142,   143,   144,   145,   146,   147,   149,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     164,   167,   168,   169,   170,   173,   221,   294,   295,   296,
     297,   347,   126,   323,   148,   166,   166,   166,   368,   368,
     368,   244,   368,   244,   368,   368,   368,   368,   368,   368,
     368,     3,    21,    34,    64,   105,   111,   222,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   368,   368,    70,   370,   370,   370,   370,
     370,   344,   344,   244,   368,   244,   368,   104,   173,   341,
     368,   221,     1,    28,    48,    52,    77,    80,    92,   111,
     116,   174,   358,   359,   360,   368,   384,   328,   368,   126,
     174,   148,   126,   126,   187,    35,   187,   368,    35,   368,
      67,   174,   345,   221,   248,   249,   331,   148,   174,   225,
     368,   126,   332,   368,   329,   262,   221,   324,   368,   368,
     324,   174,   221,   310,   283,   331,   147,   339,   340,   338,
     288,   206,     1,   260,   344,   174,    22,   256,   172,   174,
     174,   342,   370,   174,   342,   341,   368,   197,   174,   199,
     200,   346,   172,   172,   148,   166,    14,   150,   148,   166,
      14,    37,    74,   221,   142,   143,   144,   145,   146,   160,
     164,   169,   209,   296,   297,   298,   368,   209,   211,   268,
     171,   238,   324,    48,   356,   120,   221,   226,   226,   173,
     368,   176,   357,    48,   148,   176,   357,   124,   148,   176,
     226,   357,    33,    48,   226,   357,   166,   173,     1,   269,
     172,   275,   126,   148,   172,   148,   174,   337,     1,   225,
     333,   334,   107,   311,   173,   300,   368,   142,   150,   300,
     300,   368,   322,   173,   175,   166,   166,   166,   166,   166,
     166,   174,   176,   345,    48,   176,    33,    48,   173,   174,
      48,    92,   148,   174,    18,    21,    26,    46,    53,    64,
      72,    89,   101,   112,   117,   347,    91,    91,   166,   370,
     346,   368,   368,    35,   187,    35,    35,   187,   104,   207,
     221,   174,   148,   174,   174,   330,   337,    70,   370,   221,
     174,   174,   328,   340,   147,   284,   174,   338,   152,   299,
     332,   368,   172,    76,   118,   172,   261,   174,   173,   207,
     221,   257,    48,   176,    48,   148,   174,   215,   222,    18,
      19,    21,    26,    46,    50,    53,    72,    79,    89,    98,
      99,   101,   103,   112,   114,   117,   171,   214,   298,   368,
     368,   209,   211,   148,   166,    14,   166,   171,   269,   321,
     328,   244,   368,   226,   368,    48,   341,   187,   176,   244,
     368,   176,   187,   368,   176,   368,   176,   368,   226,    45,
     368,   244,   368,   226,   345,   172,    84,    87,   172,   186,
     194,   228,   368,   273,   274,   304,   311,    62,   119,   315,
     302,   303,   174,   295,   297,   174,   176,   174,   244,   368,
      45,   244,   368,   345,   359,   337,   347,   347,   187,   187,
     368,   187,    35,   166,   166,   249,   207,   332,   173,   173,
     166,   299,   332,   333,   311,   340,   368,   187,   240,   341,
     257,   147,   207,   244,   368,   244,   368,   200,   209,    14,
      37,    74,   166,   166,   298,   368,   368,   269,   172,   166,
     166,   356,   356,   166,   368,   174,   187,   176,   357,   176,
     357,   187,   124,   368,    33,   226,   357,    33,   226,   357,
     174,   194,   228,   228,   275,   194,   315,     3,    55,    94,
     105,   316,   317,   318,   368,   291,   174,   300,   300,   176,
     176,   368,    33,    33,   174,   332,    35,   187,   344,   344,
     299,   332,    33,   226,   174,   368,   176,   176,   172,   368,
     209,   211,    14,   172,   226,   226,   226,    45,   187,   368,
     176,    45,   187,   368,   176,   368,   104,    45,   368,   226,
      45,   368,   226,   166,   228,   275,   278,   318,   119,   148,
     126,   153,   155,   156,   158,   159,    62,    33,   166,   206,
     312,   313,    45,    45,   104,    45,    45,   187,   174,   174,
     187,   207,   166,   166,   368,   368,   187,   368,   187,   368,
     368,   368,   313,   368,   317,   318,   318,   318,   318,   318,
     318,   316,   185,   368,   368,   368,   368,   165,   165,   104,
     104,   104,   104,   104,   104,   104,   104,   368,   368,   368,
     368
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
     326,   325,   327,   325,   325,   325,   328,   328,   329,   329,
     330,   330,   331,   331,   331,   332,   332,   332,   333,   334,
     334,   334,   335,   335,   336,   336,   337,   337,   337,   337,
     338,   338,   339,   340,   340,   341,   341,   342,   343,   343,
     344,   344,   345,   345,   346,   346,   347,   347,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     349,   350,   350,   350,   350,   350,   350,   351,   352,   352,
     352,   352,   352,   352,   352,   353,   354,   355,   355,   355,
     355,   355,   355,   355,   356,   356,   357,   357,   357,   357,
     358,   358,   359,   359,   359,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   361,   361,   361,   361,   361,
     362,   362,   362,   362,   362,   363,   364,   364,   364,   364,
     364,   364,   365,   366,   367,   367,   368,   368,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   369,   369,   370,   370,   370,   371,   371,
     371,   371,   372,   372,   372,   372,   372,   373,   373,   373,
     374,   374,   374,   374,   374,   374,   375,   375,   375,   375,
     376,   376,   377,   377,   378,   378,   378,   378,   378,   378,
     378,   378,   378,   378,   378,   378,   378,   379,   379,   380,
     380,   380,   380,   380,   380,   380,   380,   380,   380,   380,
     380,   380,   380,   380,   380,   380,   380,   380,   380,   380,
     380,   380,   381,   381,   381,   381,   381,   381,   381,   382,
     382,   382,   382,   383,   383,   383,   383,   384,   384,   384,
     384,   384,   384,   384
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
       0,     5,     0,     6,     3,     3,     1,     3,     3,     5,
       1,     3,     1,     2,     3,     0,     2,     2,     1,     1,
       1,     1,     2,     1,     0,     1,     0,     2,     2,     1,
       1,     1,     2,     0,     1,     1,     3,     1,     3,     3,
       0,     1,     1,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     4,     4,     4,     5,     2,     2,
       1,     2,     1,     2,     1,     2,     1,     2,     1,     1,
       2,     6,     6,     4,     9,     9,     7,     2,     2,     3,
       4,     6,     6,     9,     9,     6,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     4,     3,     4,     2,
       1,     3,     4,     3,     3,     1,     1,     2,     1,     2,
       1,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       2,     8,     8,     9,     9,     4,     3,     3,     2,     2,
       2,     1,     3,     4,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     4,     4,     4,
       3,     3,     3,     3,     5,     5,     3,     4,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     4,     4,     5,     4,     5,     3,     5,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     1,     1,     1
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
#line 654 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 5950 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 658 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 5956 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 659 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 5962 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 666 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 5970 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 674 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 5978 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 678 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 5987 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 688 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 5995 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 693 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6003 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 700 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6009 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 701 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6015 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 702 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6021 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 703 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6027 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 704 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6033 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 705 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6039 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 706 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6045 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 707 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6051 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 708 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6057 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: TDEFER stmt  */
#line 710 "chpl.ypp"
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
#line 6079 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 727 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6085 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 728 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6091 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: TINIT TTHIS TSEMI  */
#line 730 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr));
    auto node = Init::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6103 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 738 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6116 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 747 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6129 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TLABEL ident_def stmt  */
#line 756 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6137 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 760 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6148 "bison-chpl-lib.cpp"
    break;

  case 29: /* stmt_base: error TSEMI  */
#line 767 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6157 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: assignment_stmt  */
#line 774 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6163 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: if_stmt  */
#line 775 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6169 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: loop_stmt  */
#line 776 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6175 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: select_stmt  */
#line 777 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6181 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: manage_stmt  */
#line 778 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6187 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 779 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6193 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: throw_stmt  */
#line 780 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6199 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 782 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6205 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 784 "chpl.ypp"
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
#line 6229 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 804 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6241 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 812 "chpl.ypp"
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
#line 6262 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TLOCAL do_stmt  */
#line 829 "chpl.ypp"
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
#line 6282 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TON expr do_stmt  */
#line 845 "chpl.ypp"
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
#line 6303 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 862 "chpl.ypp"
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
#line 6324 "bison-chpl-lib.cpp"
    break;

  case 44: /* tryable_stmt: TSERIAL do_stmt  */
#line 879 "chpl.ypp"
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
#line 6344 "bison-chpl-lib.cpp"
    break;

  case 45: /* tryable_stmt: TSYNC stmt  */
#line 895 "chpl.ypp"
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
#line 6366 "bison-chpl-lib.cpp"
    break;

  case 48: /* collect_attributes: %empty  */
#line 920 "chpl.ypp"
  {
    auto attributeGroup = context->buildAttributeGroup((yyloc));
    if (attributeGroup != nullptr) {
      context->resetAttributeGroupPartsState();
    }
    context->loopAttributes.push_back(std::move(attributeGroup));
  }
#line 6378 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 945 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6386 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATTRIBUTEIDENT  */
#line 952 "chpl.ypp"
  {
    auto ident = context->buildAttributeIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6395 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 959 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6403 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 963 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6414 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 973 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6420 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 975 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6428 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 979 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6436 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 986 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6445 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 991 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6455 "bison-chpl-lib.cpp"
    break;

  case 64: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 1004 "chpl.ypp"
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
#line 6477 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 1025 "chpl.ypp"
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
#line 6498 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 1042 "chpl.ypp"
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
#line 6519 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1059 "chpl.ypp"
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
#line 6538 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1076 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6544 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1077 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6551 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1079 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6558 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1084 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6564 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1085 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6571 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1091 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6581 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1109 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-1]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6590 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1114 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6599 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR error TRCBR  */
#line 1119 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6612 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt: block_stmt_body  */
#line 1131 "chpl.ypp"
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
#line 6631 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1149 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6637 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1150 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6643 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1155 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6651 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1159 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6659 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1163 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6668 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1168 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6676 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1172 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6684 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1176 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6693 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1184 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6702 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1189 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6712 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1195 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6721 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1200 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6732 "bison-chpl-lib.cpp"
    break;

  case 90: /* opt_only_ls: %empty  */
#line 1208 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6738 "bison-chpl-lib.cpp"
    break;

  case 91: /* opt_only_ls: renames_ls  */
#line 1209 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6744 "bison-chpl-lib.cpp"
    break;

  case 92: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1214 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6753 "bison-chpl-lib.cpp"
    break;

  case 93: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1219 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6765 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1227 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6778 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1236 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6790 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1244 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6803 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1255 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6812 "bison-chpl-lib.cpp"
    break;

  case 115: /* import_expr: expr  */
#line 1291 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6820 "bison-chpl-lib.cpp"
    break;

  case 116: /* import_expr: expr TDOT all_op_name  */
#line 1295 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6829 "bison-chpl-lib.cpp"
    break;

  case 117: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1300 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6838 "bison-chpl-lib.cpp"
    break;

  case 118: /* import_expr: expr TAS ident_use  */
#line 1305 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6848 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1311 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6858 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_ls: import_expr  */
#line 1319 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6864 "bison-chpl-lib.cpp"
    break;

  case 121: /* import_ls: import_ls TCOMMA import_expr  */
#line 1320 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6870 "bison-chpl-lib.cpp"
    break;

  case 122: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1325 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6880 "bison-chpl-lib.cpp"
    break;

  case 123: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1334 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6888 "bison-chpl-lib.cpp"
    break;

  case 124: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1338 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6896 "bison-chpl-lib.cpp"
    break;

  case 125: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1342 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6904 "bison-chpl-lib.cpp"
    break;

  case 126: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1346 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6912 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1350 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6920 "bison-chpl-lib.cpp"
    break;

  case 128: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1354 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 6928 "bison-chpl-lib.cpp"
    break;

  case 129: /* opt_label_ident: %empty  */
#line 1362 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 6934 "bison-chpl-lib.cpp"
    break;

  case 130: /* opt_label_ident: TIDENT  */
#line 1363 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6940 "bison-chpl-lib.cpp"
    break;

  case 131: /* opt_label_ident: TINIT  */
#line 1364 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6946 "bison-chpl-lib.cpp"
    break;

  case 140: /* ident_use: TIDENT  */
#line 1390 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6952 "bison-chpl-lib.cpp"
    break;

  case 141: /* ident_use: TINIT  */
#line 1391 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6958 "bison-chpl-lib.cpp"
    break;

  case 142: /* ident_use: TTHIS  */
#line 1392 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6964 "bison-chpl-lib.cpp"
    break;

  case 162: /* scalar_type: TBOOL  */
#line 1419 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6970 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TENUM  */
#line 1420 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6976 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TINT  */
#line 1421 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6982 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TUINT  */
#line 1422 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6988 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TREAL  */
#line 1423 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6994 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TIMAG  */
#line 1424 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7000 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TCOMPLEX  */
#line 1425 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7006 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TBYTES  */
#line 1426 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7012 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TSTRING  */
#line 1427 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7018 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TLOCALE  */
#line 1428 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7024 "bison-chpl-lib.cpp"
    break;

  case 172: /* scalar_type: TNOTHING  */
#line 1429 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7030 "bison-chpl-lib.cpp"
    break;

  case 173: /* scalar_type: TVOID  */
#line 1430 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7036 "bison-chpl-lib.cpp"
    break;

  case 178: /* do_stmt: TDO stmt  */
#line 1444 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7042 "bison-chpl-lib.cpp"
    break;

  case 179: /* do_stmt: block_stmt  */
#line 1445 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7048 "bison-chpl-lib.cpp"
    break;

  case 180: /* return_stmt: TRETURN TSEMI  */
#line 1450 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7059 "bison-chpl-lib.cpp"
    break;

  case 181: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1457 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7070 "bison-chpl-lib.cpp"
    break;

  case 182: /* class_level_stmt: TSEMI  */
#line 1467 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7078 "bison-chpl-lib.cpp"
    break;

  case 183: /* class_level_stmt: inner_class_level_stmt  */
#line 1471 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7088 "bison-chpl-lib.cpp"
    break;

  case 184: /* $@1: %empty  */
#line 1476 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7095 "bison-chpl-lib.cpp"
    break;

  case 185: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1478 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7104 "bison-chpl-lib.cpp"
    break;

  case 186: /* $@2: %empty  */
#line 1482 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7111 "bison-chpl-lib.cpp"
    break;

  case 187: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1484 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7120 "bison-chpl-lib.cpp"
    break;

  case 195: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1502 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7129 "bison-chpl-lib.cpp"
    break;

  case 196: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1507 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7138 "bison-chpl-lib.cpp"
    break;

  case 197: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1512 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7147 "bison-chpl-lib.cpp"
    break;

  case 198: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1517 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7155 "bison-chpl-lib.cpp"
    break;

  case 199: /* forwarding_decl_start: TFORWARDING  */
#line 1524 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7164 "bison-chpl-lib.cpp"
    break;

  case 200: /* extern_or_export: TEXTERN  */
#line 1531 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7170 "bison-chpl-lib.cpp"
    break;

  case 201: /* extern_or_export: TEXPORT  */
#line 1532 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7176 "bison-chpl-lib.cpp"
    break;

  case 202: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1537 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7186 "bison-chpl-lib.cpp"
    break;

  case 203: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1547 "chpl.ypp"
  {
    auto headerLoc = context->makeSpannedLocation((yylsp[-5]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7196 "bison-chpl-lib.cpp"
    break;

  case 204: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1554 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    auto headerLoc = context->makeSpannedLocation((yylsp[-6]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7208 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1562 "chpl.ypp"
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
#line 7230 "bison-chpl-lib.cpp"
    break;

  case 206: /* $@3: %empty  */
#line 1580 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7238 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1584 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7247 "bison-chpl-lib.cpp"
    break;

  case 208: /* $@4: %empty  */
#line 1589 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7256 "bison-chpl-lib.cpp"
    break;

  case 209: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1594 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7265 "bison-chpl-lib.cpp"
    break;

  case 210: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1602 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7273 "bison-chpl-lib.cpp"
    break;

  case 211: /* no_loop_attributes: %empty  */
#line 1607 "chpl.ypp"
                    { context->loopAttributes.push_back(nullptr); }
#line 7279 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1610 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7285 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1611 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7291 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1627 "chpl.ypp"
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
#line 7318 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1650 "chpl.ypp"
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
#line 7341 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1669 "chpl.ypp"
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
#line 7364 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1688 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7373 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1693 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7382 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1698 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7391 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1703 "chpl.ypp"
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
#line 7420 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1728 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7429 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: for_loop_kw expr TIN expr task_intent_clause do_stmt  */
#line 1733 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7438 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1738 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7447 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator task_intent_clause do_stmt  */
#line 1743 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7456 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1748 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7465 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: for_loop_kw expr task_intent_clause do_stmt  */
#line 1753 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7474 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1758 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7483 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: for_loop_kw zippered_iterator task_intent_clause do_stmt  */
#line 1763 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7492 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1768 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7501 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TLSBR expr_ls TIN expr task_intent_clause TRSBR stmt  */
#line 1773 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7510 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1778 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7519 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator task_intent_clause TRSBR stmt  */
#line 1783 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7528 "bison-chpl-lib.cpp"
    break;

  case 236: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1788 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7537 "bison-chpl-lib.cpp"
    break;

  case 237: /* loop_stmt_base: TLSBR expr_ls task_intent_clause TRSBR stmt  */
#line 1793 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7546 "bison-chpl-lib.cpp"
    break;

  case 238: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1798 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7556 "bison-chpl-lib.cpp"
    break;

  case 239: /* loop_stmt_base: TLSBR zippered_iterator task_intent_clause TRSBR stmt  */
#line 1804 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7566 "bison-chpl-lib.cpp"
    break;

  case 240: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1813 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7576 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF expr TTHEN stmt  */
#line 1822 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7584 "bison-chpl-lib.cpp"
    break;

  case 242: /* if_stmt: TIF expr block_stmt  */
#line 1826 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7592 "bison-chpl-lib.cpp"
    break;

  case 243: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1830 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7600 "bison-chpl-lib.cpp"
    break;

  case 244: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1834 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7608 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1838 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7616 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF ifvar block_stmt  */
#line 1842 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7624 "bison-chpl-lib.cpp"
    break;

  case 247: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1846 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7632 "bison-chpl-lib.cpp"
    break;

  case 248: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1850 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7640 "bison-chpl-lib.cpp"
    break;

  case 249: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1854 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7649 "bison-chpl-lib.cpp"
    break;

  case 250: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1859 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7658 "bison-chpl-lib.cpp"
    break;

  case 251: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1864 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7667 "bison-chpl-lib.cpp"
    break;

  case 252: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1869 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7676 "bison-chpl-lib.cpp"
    break;

  case 253: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1877 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                   /*attributeGroup*/ nullptr,
                                   Decl::DEFAULT_VISIBILITY,
                                   Decl::DEFAULT_LINKAGE,
                                   /*linkageName*/ nullptr,
                                   /*destinationExpr*/ nullptr,
                                   /*name*/ (yyvsp[-2].uniqueStr),
                                   Variable::VAR,
                                   /*isConfig*/ false,
                                   context->currentScopeIsAggregate(),
                                   /*typeExpression*/ nullptr,
                                   toOwned((yyvsp[0].expr)));
    (yyval.expr) = varDecl.release();
  }
#line 7696 "bison-chpl-lib.cpp"
    break;

  case 254: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1893 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                   /*attributeGroup*/ nullptr,
                                   Decl::DEFAULT_VISIBILITY,
                                   Decl::DEFAULT_LINKAGE,
                                   /*linkageName*/ nullptr,
                                   /*destinationExpr*/ nullptr,
                                   /*name*/ (yyvsp[-2].uniqueStr),
                                   Variable::CONST,
                                   /*isConfig*/ false,
                                   context->currentScopeIsAggregate(),
                                   /*typeExpression*/ nullptr,
                                   toOwned((yyvsp[0].expr)));
    (yyval.expr) = varDecl.release();
  }
#line 7716 "bison-chpl-lib.cpp"
    break;

  case 255: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1912 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-4]), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7724 "bison-chpl-lib.cpp"
    break;

  case 256: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1916 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7732 "bison-chpl-lib.cpp"
    break;

  case 257: /* ifc_formal_ls: ifc_formal  */
#line 1922 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7738 "bison-chpl-lib.cpp"
    break;

  case 258: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1923 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7744 "bison-chpl-lib.cpp"
    break;

  case 259: /* ifc_formal: ident_def  */
#line 1928 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7750 "bison-chpl-lib.cpp"
    break;

  case 279: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1972 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7758 "bison-chpl-lib.cpp"
    break;

  case 280: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1976 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7766 "bison-chpl-lib.cpp"
    break;

  case 281: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1980 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7774 "bison-chpl-lib.cpp"
    break;

  case 282: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1987 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7782 "bison-chpl-lib.cpp"
    break;

  case 283: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1991 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7790 "bison-chpl-lib.cpp"
    break;

  case 284: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1995 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7798 "bison-chpl-lib.cpp"
    break;

  case 285: /* try_stmt: TTRY tryable_stmt  */
#line 2002 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7806 "bison-chpl-lib.cpp"
    break;

  case 286: /* try_stmt: TTRYBANG tryable_stmt  */
#line 2006 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7814 "bison-chpl-lib.cpp"
    break;

  case 287: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 2010 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7822 "bison-chpl-lib.cpp"
    break;

  case 288: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 2014 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7830 "bison-chpl-lib.cpp"
    break;

  case 289: /* catch_expr_ls: %empty  */
#line 2020 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7836 "bison-chpl-lib.cpp"
    break;

  case 290: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2021 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7842 "bison-chpl-lib.cpp"
    break;

  case 291: /* catch_expr: TCATCH block_stmt  */
#line 2026 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 7850 "bison-chpl-lib.cpp"
    break;

  case 292: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2030 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 7858 "bison-chpl-lib.cpp"
    break;

  case 293: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2034 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 7866 "bison-chpl-lib.cpp"
    break;

  case 294: /* catch_expr_inner: ident_def  */
#line 2041 "chpl.ypp"
  {
    (yyval.expr) = Variable::build(BUILDER, LOC((yyloc)), /*attributeGroup*/ nullptr,
                         Decl::DEFAULT_VISIBILITY,
                         Decl::DEFAULT_LINKAGE,
                         /*linkageName*/ nullptr,
                         /*destinationExpr*/ nullptr,
                         /*name*/ (yyvsp[0].uniqueStr),
                         Variable::VAR,
                         /*isConfig*/ false,
                         /*isField*/ false,
                         /*typeExpression*/ nullptr,
                         /*initExpression*/ nullptr).release();
  }
#line 7884 "bison-chpl-lib.cpp"
    break;

  case 295: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2055 "chpl.ypp"
  {
    (yyval.expr) = Variable::build(BUILDER, LOC((yyloc)), /*attributeGroup*/ nullptr,
                         Decl::DEFAULT_VISIBILITY,
                         Decl::DEFAULT_LINKAGE,
                         /*linkageName*/ nullptr,
                         /*destinationExpr*/ nullptr,
                         /*name*/ (yyvsp[-2].uniqueStr),
                         Variable::VAR,
                         /*isConfig*/ false,
                         /*isField*/ false,
                         /*typeExpression*/ toOwned((yyvsp[0].expr)),
                         /*initExpression*/ nullptr).release();
  }
#line 7902 "bison-chpl-lib.cpp"
    break;

  case 296: /* throw_stmt: TTHROW expr TSEMI  */
#line 2072 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7912 "bison-chpl-lib.cpp"
    break;

  case 297: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2080 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), context->makeSpannedLocation((yylsp[-4]), (yylsp[-3])), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7921 "bison-chpl-lib.cpp"
    break;

  case 298: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2085 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7932 "bison-chpl-lib.cpp"
    break;

  case 299: /* when_stmt_ls: %empty  */
#line 2094 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 7938 "bison-chpl-lib.cpp"
    break;

  case 300: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2095 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7944 "bison-chpl-lib.cpp"
    break;

  case 301: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2100 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), context->makeSpannedLocation((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 7952 "bison-chpl-lib.cpp"
    break;

  case 302: /* when_stmt: TOTHERWISE stmt  */
#line 2104 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-1]), nullptr, blockOrDo);
  }
#line 7961 "bison-chpl-lib.cpp"
    break;

  case 303: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2109 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-3]), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 7971 "bison-chpl-lib.cpp"
    break;

  case 304: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2118 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 7980 "bison-chpl-lib.cpp"
    break;

  case 305: /* manager_expr: expr TAS ident_def  */
#line 2123 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 7988 "bison-chpl-lib.cpp"
    break;

  case 306: /* manager_expr: expr  */
#line 2127 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 7996 "bison-chpl-lib.cpp"
    break;

  case 307: /* manager_expr_ls: manager_expr  */
#line 2133 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8002 "bison-chpl-lib.cpp"
    break;

  case 308: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2134 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8008 "bison-chpl-lib.cpp"
    break;

  case 309: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2139 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), locHeader, (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8017 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2149 "chpl.ypp"
    {
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8027 "bison-chpl-lib.cpp"
    break;

  case 311: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2155 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8039 "bison-chpl-lib.cpp"
    break;

  case 312: /* class_start: class_tag ident_def  */
#line 2167 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8047 "bison-chpl-lib.cpp"
    break;

  case 313: /* class_tag: TCLASS  */
#line 2173 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8053 "bison-chpl-lib.cpp"
    break;

  case 314: /* class_tag: TRECORD  */
#line 2174 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8059 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_tag: TUNION  */
#line 2175 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8065 "bison-chpl-lib.cpp"
    break;

  case 316: /* opt_inherit: %empty  */
#line 2179 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8071 "bison-chpl-lib.cpp"
    break;

  case 317: /* opt_inherit: TCOLON expr_ls  */
#line 2180 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8077 "bison-chpl-lib.cpp"
    break;

  case 318: /* class_level_stmt_ls: %empty  */
#line 2184 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8086 "bison-chpl-lib.cpp"
    break;

  case 319: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2189 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8094 "bison-chpl-lib.cpp"
    break;

  case 320: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2193 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8102 "bison-chpl-lib.cpp"
    break;

  case 321: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2197 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8110 "bison-chpl-lib.cpp"
    break;

  case 322: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2201 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8118 "bison-chpl-lib.cpp"
    break;

  case 323: /* enum_decl_stmt: enum_header TLCBR enum_ls TRCBR  */
#line 2209 "chpl.ypp"
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
#line 8141 "bison-chpl-lib.cpp"
    break;

  case 324: /* enum_decl_stmt: enum_header TLCBR error TRCBR  */
#line 2228 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-3].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8155 "bison-chpl-lib.cpp"
    break;

  case 325: /* enum_header: TENUM ident_def  */
#line 2241 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Enum);
  }
#line 8163 "bison-chpl-lib.cpp"
    break;

  case 326: /* enum_ls: enum_item  */
#line 2248 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8172 "bison-chpl-lib.cpp"
    break;

  case 327: /* enum_ls: enum_ls TCOMMA  */
#line 2253 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8182 "bison-chpl-lib.cpp"
    break;

  case 328: /* $@5: %empty  */
#line 2259 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8191 "bison-chpl-lib.cpp"
    break;

  case 329: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2264 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8200 "bison-chpl-lib.cpp"
    break;

  case 330: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2269 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8209 "bison-chpl-lib.cpp"
    break;

  case 331: /* $@6: %empty  */
#line 2274 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8218 "bison-chpl-lib.cpp"
    break;

  case 332: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2279 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8227 "bison-chpl-lib.cpp"
    break;

  case 333: /* enum_item: ident_def  */
#line 2289 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8238 "bison-chpl-lib.cpp"
    break;

  case 334: /* enum_item: ident_def TASSIGN expr  */
#line 2296 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8251 "bison-chpl-lib.cpp"
    break;

  case 335: /* lambda_decl_start: TLAMBDA  */
#line 2308 "chpl.ypp"
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
#line 8267 "bison-chpl-lib.cpp"
    break;

  case 336: /* $@7: %empty  */
#line 2325 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8277 "bison-chpl-lib.cpp"
    break;

  case 337: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2331 "chpl.ypp"
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
#line 8299 "bison-chpl-lib.cpp"
    break;

  case 339: /* linkage_spec: linkage_spec_empty  */
#line 2354 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8305 "bison-chpl-lib.cpp"
    break;

  case 340: /* linkage_spec: TINLINE  */
#line 2355 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8312 "bison-chpl-lib.cpp"
    break;

  case 341: /* linkage_spec: TOVERRIDE  */
#line 2357 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8319 "bison-chpl-lib.cpp"
    break;

  case 342: /* opt_fn_type_formal_ls: %empty  */
#line 2362 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8325 "bison-chpl-lib.cpp"
    break;

  case 343: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2363 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8331 "bison-chpl-lib.cpp"
    break;

  case 344: /* fn_type_formal_ls: fn_type_formal  */
#line 2367 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8337 "bison-chpl-lib.cpp"
    break;

  case 345: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2368 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8343 "bison-chpl-lib.cpp"
    break;

  case 346: /* fn_type_formal: named_formal  */
#line 2373 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8349 "bison-chpl-lib.cpp"
    break;

  case 347: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2376 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8355 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_type_formal: formal_type  */
#line 2378 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8361 "bison-chpl-lib.cpp"
    break;

  case 349: /* opt_fn_type_ret_type: %empty  */
#line 2382 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8367 "bison-chpl-lib.cpp"
    break;

  case 350: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2383 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8373 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2396 "chpl.ypp"
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
#line 8392 "bison-chpl-lib.cpp"
    break;

  case 352: /* $@8: %empty  */
#line 2414 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8402 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2420 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8415 "bison-chpl-lib.cpp"
    break;

  case 354: /* $@9: %empty  */
#line 2429 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8425 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2435 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8438 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2447 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8446 "bison-chpl-lib.cpp"
    break;

  case 357: /* $@10: %empty  */
#line 2456 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8454 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2460 "chpl.ypp"
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
#line 8476 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2481 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8488 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2489 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8500 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2497 "chpl.ypp"
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
#line 8515 "bison-chpl-lib.cpp"
    break;

  case 362: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2508 "chpl.ypp"
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
#line 8530 "bison-chpl-lib.cpp"
    break;

  case 363: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2519 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8540 "bison-chpl-lib.cpp"
    break;

  case 364: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2528 "chpl.ypp"
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
#line 8556 "bison-chpl-lib.cpp"
    break;

  case 366: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2543 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8562 "bison-chpl-lib.cpp"
    break;

  case 369: /* fn_ident: ident_def TBANG  */
#line 2550 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8572 "bison-chpl-lib.cpp"
    break;

  case 409: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2607 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8578 "bison-chpl-lib.cpp"
    break;

  case 410: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2608 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8584 "bison-chpl-lib.cpp"
    break;

  case 411: /* opt_formal_ls: %empty  */
#line 2612 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8590 "bison-chpl-lib.cpp"
    break;

  case 412: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2613 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8596 "bison-chpl-lib.cpp"
    break;

  case 413: /* req_formal_ls: TLP TRP  */
#line 2617 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8602 "bison-chpl-lib.cpp"
    break;

  case 414: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2618 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8608 "bison-chpl-lib.cpp"
    break;

  case 415: /* formal_ls_inner: formal  */
#line 2622 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8614 "bison-chpl-lib.cpp"
    break;

  case 416: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2623 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8620 "bison-chpl-lib.cpp"
    break;

  case 417: /* formal_ls: %empty  */
#line 2627 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8626 "bison-chpl-lib.cpp"
    break;

  case 418: /* formal_ls: formal_ls_inner  */
#line 2628 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8632 "bison-chpl-lib.cpp"
    break;

  case 420: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2637 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8640 "bison-chpl-lib.cpp"
    break;

  case 421: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2642 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8648 "bison-chpl-lib.cpp"
    break;

  case 422: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2647 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8656 "bison-chpl-lib.cpp"
    break;

  case 423: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2652 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8664 "bison-chpl-lib.cpp"
    break;

  case 424: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2657 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8672 "bison-chpl-lib.cpp"
    break;

  case 425: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2662 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8680 "bison-chpl-lib.cpp"
    break;

  case 426: /* opt_formal_intent_tag: %empty  */
#line 2668 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 8689 "bison-chpl-lib.cpp"
    break;

  case 427: /* opt_formal_intent_tag: required_intent_tag  */
#line 2673 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8698 "bison-chpl-lib.cpp"
    break;

  case 428: /* required_intent_tag: TIN  */
#line 2680 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 8704 "bison-chpl-lib.cpp"
    break;

  case 429: /* required_intent_tag: TINOUT  */
#line 2681 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 8710 "bison-chpl-lib.cpp"
    break;

  case 430: /* required_intent_tag: TOUT  */
#line 2682 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 8716 "bison-chpl-lib.cpp"
    break;

  case 431: /* required_intent_tag: TCONST TIN  */
#line 2683 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 8722 "bison-chpl-lib.cpp"
    break;

  case 432: /* required_intent_tag: TCONST TREF  */
#line 2684 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 8728 "bison-chpl-lib.cpp"
    break;

  case 433: /* required_intent_tag: TCONST  */
#line 2685 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 8734 "bison-chpl-lib.cpp"
    break;

  case 434: /* required_intent_tag: TPARAM  */
#line 2686 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 8740 "bison-chpl-lib.cpp"
    break;

  case 435: /* required_intent_tag: TREF  */
#line 2687 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 8746 "bison-chpl-lib.cpp"
    break;

  case 436: /* required_intent_tag: TTYPE  */
#line 2688 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 8752 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_this_intent_tag: %empty  */
#line 2692 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 8758 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_this_intent_tag: TPARAM  */
#line 2693 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 8764 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_this_intent_tag: TREF  */
#line 2694 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 8770 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: TCONST TREF  */
#line 2695 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 8776 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_this_intent_tag: TCONST  */
#line 2696 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 8782 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_this_intent_tag: TTYPE  */
#line 2697 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 8788 "bison-chpl-lib.cpp"
    break;

  case 443: /* proc_iter_or_op: TPROC  */
#line 2701 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8794 "bison-chpl-lib.cpp"
    break;

  case 444: /* proc_iter_or_op: TITER  */
#line 2702 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8800 "bison-chpl-lib.cpp"
    break;

  case 445: /* proc_iter_or_op: TOPERATOR  */
#line 2703 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8806 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_ret_tag: %empty  */
#line 2707 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 8812 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_ret_tag: TOUT  */
#line 2708 "chpl.ypp"
                    { (yyval.returnTag) = Function::OUT; }
#line 8818 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_ret_tag: TCONST  */
#line 2709 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 8824 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_ret_tag: TCONST TREF  */
#line 2710 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 8830 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_ret_tag: TREF  */
#line 2711 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 8836 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_ret_tag: TPARAM  */
#line 2712 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 8842 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: TTYPE  */
#line 2713 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 8848 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_throws_error: %empty  */
#line 2717 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8854 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_throws_error: TTHROWS  */
#line 2718 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8860 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_function_body_stmt: TSEMI  */
#line 2721 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8866 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_function_body_stmt: function_body_stmt  */
#line 2722 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8872 "bison-chpl-lib.cpp"
    break;

  case 457: /* function_body_stmt: block_stmt_body  */
#line 2726 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8878 "bison-chpl-lib.cpp"
    break;

  case 458: /* function_body_stmt: TDO toplevel_stmt  */
#line 2727 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 8884 "bison-chpl-lib.cpp"
    break;

  case 459: /* query_expr: TQUERIEDIDENT  */
#line 2731 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 8890 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_lifetime_where: %empty  */
#line 2736 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 8896 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_lifetime_where: TWHERE expr  */
#line 2738 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 8902 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2740 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 8908 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2742 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 8914 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2744 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 8920 "bison-chpl-lib.cpp"
    break;

  case 465: /* lifetime_components_expr: lifetime_expr  */
#line 2749 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8926 "bison-chpl-lib.cpp"
    break;

  case 466: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2751 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8932 "bison-chpl-lib.cpp"
    break;

  case 467: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2756 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8938 "bison-chpl-lib.cpp"
    break;

  case 468: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2758 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8944 "bison-chpl-lib.cpp"
    break;

  case 469: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2760 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8950 "bison-chpl-lib.cpp"
    break;

  case 470: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2762 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8956 "bison-chpl-lib.cpp"
    break;

  case 471: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2764 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8962 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2766 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8968 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2768 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 8974 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_ident: TIDENT  */
#line 2772 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8980 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_ident: TINIT  */
#line 2773 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8986 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_ident: TTHIS  */
#line 2774 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8992 "bison-chpl-lib.cpp"
    break;

  case 477: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2779 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9001 "bison-chpl-lib.cpp"
    break;

  case 478: /* type_alias_decl_stmt_start: TTYPE  */
#line 2788 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9009 "bison-chpl-lib.cpp"
    break;

  case 479: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2792 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9018 "bison-chpl-lib.cpp"
    break;

  case 480: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2800 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9026 "bison-chpl-lib.cpp"
    break;

  case 481: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2804 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9034 "bison-chpl-lib.cpp"
    break;

  case 482: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2811 "chpl.ypp"
  {
    // TODO (dlongnecke-cray): Add a helper to build this and var_decl_stmt.
    auto node = Variable::build(BUILDER, LOC((yyloc)),
                                /*attributeGroup*/ nullptr,
                                context->visibility,
                                context->linkage,
                                context->consumeVarDeclLinkageName(),
                                context->consumeVarDestinationExpr(),
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
#line 9058 "bison-chpl-lib.cpp"
    break;

  case 483: /* opt_init_type: %empty  */
#line 2833 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9064 "bison-chpl-lib.cpp"
    break;

  case 484: /* opt_init_type: TASSIGN expr  */
#line 2835 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9070 "bison-chpl-lib.cpp"
    break;

  case 485: /* var_decl_type: TPARAM  */
#line 2839 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9076 "bison-chpl-lib.cpp"
    break;

  case 486: /* var_decl_type: TCONST TREF  */
#line 2840 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9082 "bison-chpl-lib.cpp"
    break;

  case 487: /* var_decl_type: TREF  */
#line 2841 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9088 "bison-chpl-lib.cpp"
    break;

  case 488: /* var_decl_type: TCONST  */
#line 2842 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9094 "bison-chpl-lib.cpp"
    break;

  case 489: /* var_decl_type: TVAR  */
#line 2843 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9100 "bison-chpl-lib.cpp"
    break;

  case 490: /* $@11: %empty  */
#line 2848 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9109 "bison-chpl-lib.cpp"
    break;

  case 491: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2852 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9118 "bison-chpl-lib.cpp"
    break;

  case 492: /* $@12: %empty  */
#line 2857 "chpl.ypp"
  {
    // Use a mid-rule action to preserve the 'on' expr
    context->noteVarDestinationExpr((yyvsp[0].expr));
  }
#line 9127 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_stmt: TON expr $@12 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2862 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9136 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2867 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9145 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_stmt: var_decl_type error TSEMI  */
#line 2872 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = { .comments=nullptr, .stmt=ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release() };
    context->resetDeclStateOnError();
  }
#line 9154 "bison-chpl-lib.cpp"
    break;

  case 496: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2880 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9162 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2884 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9170 "bison-chpl-lib.cpp"
    break;

  case 498: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2891 "chpl.ypp"
    {
      // delay building the attributes for a variable, then update it later
      // in the context during the call to buildVarOrMultiDeclStmt
      auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                     /*attributGroup*/nullptr,
                                     context->visibility,
                                     context->linkage,
                                     context->consumeVarDeclLinkageName(),
                                     context->consumeVarDestinationExpr(),
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
#line 9194 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2911 "chpl.ypp"
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
#line 9214 "bison-chpl-lib.cpp"
    break;

  case 500: /* tuple_var_decl_component: ident_def  */
#line 2930 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9222 "bison-chpl-lib.cpp"
    break;

  case 501: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2934 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9230 "bison-chpl-lib.cpp"
    break;

  case 502: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2941 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9236 "bison-chpl-lib.cpp"
    break;

  case 503: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2943 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9242 "bison-chpl-lib.cpp"
    break;

  case 504: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2945 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9248 "bison-chpl-lib.cpp"
    break;

  case 505: /* opt_init_expr: %empty  */
#line 2951 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9254 "bison-chpl-lib.cpp"
    break;

  case 506: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2952 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9260 "bison-chpl-lib.cpp"
    break;

  case 507: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2953 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9266 "bison-chpl-lib.cpp"
    break;

  case 508: /* formal_or_ret_type_expr: expr  */
#line 2957 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9272 "bison-chpl-lib.cpp"
    break;

  case 509: /* ret_type: formal_or_ret_type_expr  */
#line 2961 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9278 "bison-chpl-lib.cpp"
    break;

  case 510: /* ret_type: reserved_type_ident_use  */
#line 2962 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9284 "bison-chpl-lib.cpp"
    break;

  case 511: /* ret_type: error  */
#line 2963 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9290 "bison-chpl-lib.cpp"
    break;

  case 512: /* colon_ret_type: TCOLON ret_type  */
#line 2967 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9296 "bison-chpl-lib.cpp"
    break;

  case 513: /* colon_ret_type: error  */
#line 2968 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9302 "bison-chpl-lib.cpp"
    break;

  case 514: /* opt_ret_type: %empty  */
#line 2972 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9308 "bison-chpl-lib.cpp"
    break;

  case 516: /* opt_type: %empty  */
#line 2977 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9314 "bison-chpl-lib.cpp"
    break;

  case 517: /* opt_type: TCOLON expr  */
#line 2978 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9320 "bison-chpl-lib.cpp"
    break;

  case 518: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2979 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9326 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_type: error  */
#line 2980 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9332 "bison-chpl-lib.cpp"
    break;

  case 520: /* formal_type: formal_or_ret_type_expr  */
#line 2984 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9338 "bison-chpl-lib.cpp"
    break;

  case 521: /* formal_type: reserved_type_ident_use  */
#line 2985 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9344 "bison-chpl-lib.cpp"
    break;

  case 522: /* colon_formal_type: TCOLON formal_type  */
#line 2989 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9350 "bison-chpl-lib.cpp"
    break;

  case 523: /* opt_colon_formal_type: %empty  */
#line 2993 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9356 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_colon_formal_type: colon_formal_type  */
#line 2994 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9362 "bison-chpl-lib.cpp"
    break;

  case 525: /* expr_ls: expr  */
#line 3000 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9368 "bison-chpl-lib.cpp"
    break;

  case 526: /* expr_ls: expr_ls TCOMMA expr  */
#line 3001 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9374 "bison-chpl-lib.cpp"
    break;

  case 527: /* tuple_component: opt_try_expr  */
#line 3005 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9380 "bison-chpl-lib.cpp"
    break;

  case 528: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 3010 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9388 "bison-chpl-lib.cpp"
    break;

  case 529: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 3014 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9396 "bison-chpl-lib.cpp"
    break;

  case 530: /* opt_actual_ls: %empty  */
#line 3020 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9402 "bison-chpl-lib.cpp"
    break;

  case 531: /* opt_actual_ls: actual_ls  */
#line 3021 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9408 "bison-chpl-lib.cpp"
    break;

  case 532: /* actual_ls: actual_expr  */
#line 3026 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9417 "bison-chpl-lib.cpp"
    break;

  case 533: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3031 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9427 "bison-chpl-lib.cpp"
    break;

  case 534: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3039 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9433 "bison-chpl-lib.cpp"
    break;

  case 535: /* actual_expr: opt_try_expr  */
#line 3040 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9439 "bison-chpl-lib.cpp"
    break;

  case 536: /* ident_expr: ident_use  */
#line 3044 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9445 "bison-chpl-lib.cpp"
    break;

  case 537: /* ident_expr: scalar_type  */
#line 3045 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9451 "bison-chpl-lib.cpp"
    break;

  case 543: /* sub_type_level_expr: TSINGLE expr  */
#line 3058 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9457 "bison-chpl-lib.cpp"
    break;

  case 544: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3060 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9463 "bison-chpl-lib.cpp"
    break;

  case 545: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3062 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9469 "bison-chpl-lib.cpp"
    break;

  case 546: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3064 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9475 "bison-chpl-lib.cpp"
    break;

  case 547: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3066 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9485 "bison-chpl-lib.cpp"
    break;

  case 548: /* sub_type_level_expr: TATOMIC expr  */
#line 3072 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9491 "bison-chpl-lib.cpp"
    break;

  case 549: /* sub_type_level_expr: TSYNC expr  */
#line 3074 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9497 "bison-chpl-lib.cpp"
    break;

  case 550: /* sub_type_level_expr: TOWNED  */
#line 3077 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9503 "bison-chpl-lib.cpp"
    break;

  case 551: /* sub_type_level_expr: TOWNED expr  */
#line 3079 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9509 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TUNMANAGED  */
#line 3081 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9515 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3083 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9521 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TSHARED  */
#line 3085 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9527 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TSHARED expr  */
#line 3087 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9533 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TBORROWED  */
#line 3089 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9539 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TBORROWED expr  */
#line 3091 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9545 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TCLASS  */
#line 3093 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9551 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TRECORD  */
#line 3095 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9557 "bison-chpl-lib.cpp"
    break;

  case 560: /* for_expr: no_loop_attributes for_expr_base  */
#line 3099 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9563 "bison-chpl-lib.cpp"
    break;

  case 561: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3104 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9571 "bison-chpl-lib.cpp"
    break;

  case 562: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3108 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9579 "bison-chpl-lib.cpp"
    break;

  case 563: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3112 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-2]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9587 "bison-chpl-lib.cpp"
    break;

  case 564: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3116 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9600 "bison-chpl-lib.cpp"
    break;

  case 565: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3125 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9613 "bison-chpl-lib.cpp"
    break;

  case 566: /* for_expr_base: for_loop_kw expr TDO TIF expr TTHEN expr  */
#line 3134 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-5]), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9626 "bison-chpl-lib.cpp"
    break;

  case 567: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3145 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9632 "bison-chpl-lib.cpp"
    break;

  case 568: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3150 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9640 "bison-chpl-lib.cpp"
    break;

  case 569: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3154 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9648 "bison-chpl-lib.cpp"
    break;

  case 570: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3158 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9656 "bison-chpl-lib.cpp"
    break;

  case 571: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3162 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9664 "bison-chpl-lib.cpp"
    break;

  case 572: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3166 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9672 "bison-chpl-lib.cpp"
    break;

  case 573: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3170 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9680 "bison-chpl-lib.cpp"
    break;

  case 574: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3174 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9688 "bison-chpl-lib.cpp"
    break;

  case 575: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3181 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9702 "bison-chpl-lib.cpp"
    break;

  case 576: /* nil_expr: TNIL  */
#line 3198 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9708 "bison-chpl-lib.cpp"
    break;

  case 584: /* opt_task_intent_ls: %empty  */
#line 3216 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9714 "bison-chpl-lib.cpp"
    break;

  case 585: /* opt_task_intent_ls: task_intent_clause  */
#line 3217 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9720 "bison-chpl-lib.cpp"
    break;

  case 586: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3222 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9728 "bison-chpl-lib.cpp"
    break;

  case 587: /* task_intent_clause: TWITH TLP TRP  */
#line 3226 "chpl.ypp"
  {
    auto err = context->syntax((yyloc), "'with' clause cannot be empty.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-2]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9739 "bison-chpl-lib.cpp"
    break;

  case 588: /* task_intent_clause: TWITH TLP error TRP  */
#line 3233 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[-2]), "invalid intent expression in 'with' clause.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9750 "bison-chpl-lib.cpp"
    break;

  case 589: /* task_intent_clause: TWITH error  */
#line 3240 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[0]), "missing parentheses around 'with' clause intents.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-1]), dummyLoc, dummyLoc, dummyLoc, exprs);
  }
#line 9761 "bison-chpl-lib.cpp"
    break;

  case 590: /* task_intent_ls: intent_expr  */
#line 3249 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9767 "bison-chpl-lib.cpp"
    break;

  case 591: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3250 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9773 "bison-chpl-lib.cpp"
    break;

  case 592: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3255 "chpl.ypp"
  {
    (yyval.expr) = context->buildTaskIntent((yyloc), (yylsp[-2]), nullptr, (yyvsp[-2].expr), (yyvsp[-3].taskIntent), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9781 "bison-chpl-lib.cpp"
    break;

  case 593: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3259 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9789 "bison-chpl-lib.cpp"
    break;

  case 594: /* intent_expr: expr TREDUCE ident_expr  */
#line 3263 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9797 "bison-chpl-lib.cpp"
    break;

  case 595: /* task_var_prefix: TCONST  */
#line 3269 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST);          }
#line 9803 "bison-chpl-lib.cpp"
    break;

  case 596: /* task_var_prefix: TIN  */
#line 3270 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::IN);             }
#line 9809 "bison-chpl-lib.cpp"
    break;

  case 597: /* task_var_prefix: TCONST TIN  */
#line 3271 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_IN);       }
#line 9815 "bison-chpl-lib.cpp"
    break;

  case 598: /* task_var_prefix: TREF  */
#line 3272 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::REF);            }
#line 9821 "bison-chpl-lib.cpp"
    break;

  case 599: /* task_var_prefix: TCONST TREF  */
#line 3273 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_REF);      }
#line 9827 "bison-chpl-lib.cpp"
    break;

  case 600: /* task_var_prefix: TVAR  */
#line 3274 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::VAR);            }
#line 9833 "bison-chpl-lib.cpp"
    break;

  case 601: /* task_var_prefix: TOUT  */
#line 3275 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::OUT);   }
#line 9839 "bison-chpl-lib.cpp"
    break;

  case 602: /* task_var_prefix: TINOUT  */
#line 3276 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::INOUT); }
#line 9845 "bison-chpl-lib.cpp"
    break;

  case 603: /* task_var_prefix: TPARAM  */
#line 3277 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::PARAM); }
#line 9851 "bison-chpl-lib.cpp"
    break;

  case 604: /* task_var_prefix: TTYPE  */
#line 3278 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::TYPE);  }
#line 9857 "bison-chpl-lib.cpp"
    break;

  case 605: /* new_maybe_decorated: TNEW  */
#line 3283 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 9863 "bison-chpl-lib.cpp"
    break;

  case 606: /* new_maybe_decorated: TNEW TOWNED  */
#line 3285 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 9869 "bison-chpl-lib.cpp"
    break;

  case 607: /* new_maybe_decorated: TNEW TSHARED  */
#line 3287 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 9875 "bison-chpl-lib.cpp"
    break;

  case 608: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3289 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 9881 "bison-chpl-lib.cpp"
    break;

  case 609: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3291 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 9887 "bison-chpl-lib.cpp"
    break;

  case 610: /* new_expr: new_maybe_decorated expr  */
#line 3297 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 9895 "bison-chpl-lib.cpp"
    break;

  case 611: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3304 "chpl.ypp"
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
#line 9911 "bison-chpl-lib.cpp"
    break;

  case 612: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3316 "chpl.ypp"
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
#line 9927 "bison-chpl-lib.cpp"
    break;

  case 613: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3328 "chpl.ypp"
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
#line 9944 "bison-chpl-lib.cpp"
    break;

  case 614: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
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
    (yyval.expr) = context->buildNewExpr((yyloc), New::SHARED, call.release());

  }
#line 9962 "bison-chpl-lib.cpp"
    break;

  case 615: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3358 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9970 "bison-chpl-lib.cpp"
    break;

  case 616: /* range_literal_expr: expr TDOTDOT expr  */
#line 3365 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 9979 "bison-chpl-lib.cpp"
    break;

  case 617: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3370 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 9988 "bison-chpl-lib.cpp"
    break;

  case 618: /* range_literal_expr: expr TDOTDOT  */
#line 3375 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 9997 "bison-chpl-lib.cpp"
    break;

  case 619: /* range_literal_expr: TDOTDOT expr  */
#line 3380 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10007 "bison-chpl-lib.cpp"
    break;

  case 620: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3386 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10017 "bison-chpl-lib.cpp"
    break;

  case 621: /* range_literal_expr: TDOTDOT  */
#line 3392 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10027 "bison-chpl-lib.cpp"
    break;

  case 622: /* cast_expr: expr TCOLON expr  */
#line 3422 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10035 "bison-chpl-lib.cpp"
    break;

  case 623: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3429 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10043 "bison-chpl-lib.cpp"
    break;

  case 624: /* super_expr: fn_expr  */
#line 3435 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10049 "bison-chpl-lib.cpp"
    break;

  case 628: /* expr: sub_type_level_expr TQUESTION  */
#line 3444 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10055 "bison-chpl-lib.cpp"
    break;

  case 629: /* expr: TQUESTION  */
#line 3446 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10061 "bison-chpl-lib.cpp"
    break;

  case 633: /* expr: fn_type  */
#line 3451 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10067 "bison-chpl-lib.cpp"
    break;

  case 643: /* opt_expr: %empty  */
#line 3465 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10073 "bison-chpl-lib.cpp"
    break;

  case 644: /* opt_expr: expr  */
#line 3466 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10079 "bison-chpl-lib.cpp"
    break;

  case 645: /* opt_try_expr: TTRY expr  */
#line 3470 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10085 "bison-chpl-lib.cpp"
    break;

  case 646: /* opt_try_expr: TTRYBANG expr  */
#line 3471 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10091 "bison-chpl-lib.cpp"
    break;

  case 647: /* opt_try_expr: super_expr  */
#line 3472 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10097 "bison-chpl-lib.cpp"
    break;

  case 653: /* call_base_expr: expr TBANG  */
#line 3489 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10105 "bison-chpl-lib.cpp"
    break;

  case 654: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3492 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10111 "bison-chpl-lib.cpp"
    break;

  case 657: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3499 "chpl.ypp"
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
#line 10127 "bison-chpl-lib.cpp"
    break;

  case 658: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3511 "chpl.ypp"
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
#line 10143 "bison-chpl-lib.cpp"
    break;

  case 659: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3523 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10151 "bison-chpl-lib.cpp"
    break;

  case 660: /* dot_expr: expr TDOT ident_use  */
#line 3530 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10157 "bison-chpl-lib.cpp"
    break;

  case 661: /* dot_expr: expr TDOT TTYPE  */
#line 3532 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10163 "bison-chpl-lib.cpp"
    break;

  case 662: /* dot_expr: expr TDOT TDOMAIN  */
#line 3534 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10169 "bison-chpl-lib.cpp"
    break;

  case 663: /* dot_expr: expr TDOT TLOCALE  */
#line 3536 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10175 "bison-chpl-lib.cpp"
    break;

  case 664: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3538 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10181 "bison-chpl-lib.cpp"
    break;

  case 665: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3540 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10187 "bison-chpl-lib.cpp"
    break;

  case 666: /* parenthesized_expr: TLP opt_try_expr TRP  */
#line 3549 "chpl.ypp"
  {
    // Use 'tryNote' here in case an expression like ((x)) comes along.
    BUILDER->tryNoteExprParenthLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 10197 "bison-chpl-lib.cpp"
    break;

  case 667: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3555 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10205 "bison-chpl-lib.cpp"
    break;

  case 668: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3559 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10213 "bison-chpl-lib.cpp"
    break;

  case 669: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3563 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10221 "bison-chpl-lib.cpp"
    break;

  case 670: /* bool_literal: TFALSE  */
#line 3569 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10227 "bison-chpl-lib.cpp"
    break;

  case 671: /* bool_literal: TTRUE  */
#line 3570 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10233 "bison-chpl-lib.cpp"
    break;

  case 672: /* str_bytes_literal: STRINGLITERAL  */
#line 3574 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10239 "bison-chpl-lib.cpp"
    break;

  case 673: /* str_bytes_literal: BYTESLITERAL  */
#line 3575 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10245 "bison-chpl-lib.cpp"
    break;

  case 676: /* literal_expr: INTLITERAL  */
#line 3581 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10251 "bison-chpl-lib.cpp"
    break;

  case 677: /* literal_expr: REALLITERAL  */
#line 3582 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10257 "bison-chpl-lib.cpp"
    break;

  case 678: /* literal_expr: IMAGLITERAL  */
#line 3583 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10263 "bison-chpl-lib.cpp"
    break;

  case 679: /* literal_expr: CSTRINGLITERAL  */
#line 3584 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10269 "bison-chpl-lib.cpp"
    break;

  case 680: /* literal_expr: TNONE  */
#line 3585 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10275 "bison-chpl-lib.cpp"
    break;

  case 681: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3587 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10284 "bison-chpl-lib.cpp"
    break;

  case 682: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3592 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10293 "bison-chpl-lib.cpp"
    break;

  case 683: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3597 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10302 "bison-chpl-lib.cpp"
    break;

  case 684: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3602 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10311 "bison-chpl-lib.cpp"
    break;

  case 685: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3607 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10320 "bison-chpl-lib.cpp"
    break;

  case 686: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3612 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10329 "bison-chpl-lib.cpp"
    break;

  case 687: /* assoc_expr_ls: expr TALIAS expr  */
#line 3621 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10338 "bison-chpl-lib.cpp"
    break;

  case 688: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3626 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10348 "bison-chpl-lib.cpp"
    break;

  case 689: /* binary_op_expr: expr TPLUS expr  */
#line 3634 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10354 "bison-chpl-lib.cpp"
    break;

  case 690: /* binary_op_expr: expr TMINUS expr  */
#line 3635 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10360 "bison-chpl-lib.cpp"
    break;

  case 691: /* binary_op_expr: expr TSTAR expr  */
#line 3636 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10366 "bison-chpl-lib.cpp"
    break;

  case 692: /* binary_op_expr: expr TDIVIDE expr  */
#line 3637 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10372 "bison-chpl-lib.cpp"
    break;

  case 693: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3638 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10378 "bison-chpl-lib.cpp"
    break;

  case 694: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3639 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10384 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr TMOD expr  */
#line 3640 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10390 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TEQUAL expr  */
#line 3641 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10396 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3642 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10402 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3643 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10408 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3644 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10414 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TLESS expr  */
#line 3645 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10420 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TGREATER expr  */
#line 3646 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10426 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TBAND expr  */
#line 3647 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10432 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TBOR expr  */
#line 3648 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10438 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TBXOR expr  */
#line 3649 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10444 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TAND expr  */
#line 3650 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10450 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TOR expr  */
#line 3651 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10456 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TEXP expr  */
#line 3652 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10462 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TBY expr  */
#line 3653 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10468 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TALIGN expr  */
#line 3654 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10474 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr THASH expr  */
#line 3655 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10480 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TDMAPPED expr  */
#line 3656 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10486 "bison-chpl-lib.cpp"
    break;

  case 712: /* unary_op_expr: TPLUS expr  */
#line 3660 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10492 "bison-chpl-lib.cpp"
    break;

  case 713: /* unary_op_expr: TMINUS expr  */
#line 3661 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10498 "bison-chpl-lib.cpp"
    break;

  case 714: /* unary_op_expr: TMINUSMINUS expr  */
#line 3662 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10504 "bison-chpl-lib.cpp"
    break;

  case 715: /* unary_op_expr: TPLUSPLUS expr  */
#line 3663 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10510 "bison-chpl-lib.cpp"
    break;

  case 716: /* unary_op_expr: TBANG expr  */
#line 3664 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10516 "bison-chpl-lib.cpp"
    break;

  case 717: /* unary_op_expr: expr TBANG  */
#line 3665 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10524 "bison-chpl-lib.cpp"
    break;

  case 718: /* unary_op_expr: TBNOT expr  */
#line 3668 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10530 "bison-chpl-lib.cpp"
    break;

  case 719: /* reduce_expr: expr TREDUCE expr  */
#line 3673 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10538 "bison-chpl-lib.cpp"
    break;

  case 720: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3677 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10546 "bison-chpl-lib.cpp"
    break;

  case 721: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3681 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10554 "bison-chpl-lib.cpp"
    break;

  case 722: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3685 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10562 "bison-chpl-lib.cpp"
    break;

  case 723: /* scan_expr: expr TSCAN expr  */
#line 3692 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10570 "bison-chpl-lib.cpp"
    break;

  case 724: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3696 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10578 "bison-chpl-lib.cpp"
    break;

  case 725: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3700 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10586 "bison-chpl-lib.cpp"
    break;

  case 726: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3704 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10594 "bison-chpl-lib.cpp"
    break;


#line 10598 "bison-chpl-lib.cpp"

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
